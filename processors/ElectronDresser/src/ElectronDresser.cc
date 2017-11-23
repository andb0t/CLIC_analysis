#include "ElectronDresser.h"

const int MAX_COLL_NAME_WIDTH = 20;

double DeltaR(double eta0, double phi0, double eta1, double phi1) {
  Double_t deta = eta0 - eta1;
  Double_t dphi = TVector2::Phi_mpi_pi(phi0 - phi1);
  return TMath::Sqrt(deta * deta + dphi * dphi);
}
double EtaFromTheta(double theta) { return -TMath::Log(TMath::Tan(theta / 2)); }

ElectronDresser aElectronDresser;

ElectronDresser::ElectronDresser() : Processor("ElectronDresser") {
  // modify processor description
  _description = "ElectronDresser does whatever it does ...";

  // register parameters: collection type, aribtrary name, arbitrary description, class-variable, default value
  // input
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "", "", m_pfos, std::string("LooseSelectedPandoraPFANewPFOs"));
  registerInputCollection(LCIO::MCPARTICLE, "", "", m_mc_particles, std::string("MCParticlesSkimmed"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "", "", m_IsolatedLepton, std::string("IsolatedLeptonCollection"));
  // output
  registerOutputCollection(LCIO::RECONSTRUCTEDPARTICLE, "", "", m_DressedLepton,
                           std::string("DressedIsolatedLeptonCollection"));
}

void ElectronDresser::init() {
  streamlog_out(MESSAGE) << "   init called  " << std::endl;
  printParameters();
  _nRun = 0;
  _nEvt = 0;

  streamlog_out(MESSAGE) << "First event: initializing globals..." << std::endl;
}

void ElectronDresser::processRunHeader(LCRunHeader* run) {
  run = run;
  _nRun++;
}

void ElectronDresser::processEvent(LCEvent* evt) {
  streamlog_out(MESSAGE) << "   Processing event: " << evt->getEventNumber() << "   in run:  " << evt->getRunNumber()
                         << std::endl;

  clearEventVariables();

  streamlog_out(MESSAGE) << "Analyzing collections:" << std::endl;
  fillMCInfo(evt);
  fillPFOs(evt);
  fillLeptons(evt);
  dressLeptons(evt);

  _nEvt++;
}

void ElectronDresser::end() {
  printParameters();
  streamlog_out(MESSAGE) << "ElectronDresser::end()  " << name() << " processed " << _nEvt << " events in " << _nRun
                         << " runs " << std::endl;
}

void ElectronDresser::clearEventVariables() {
  fourvec.SetPxPyPzE(0, 0, 0, 0);
  tmpvec.SetPxPyPzE(0, 0, 0, 0);

  mc_n = 0;
  mc_gen_status.clear();
  mc_type.clear();
  mc_pt.clear();
  mc_theta.clear();
  mc_eta.clear();
  mc_phi.clear();
  mc_e.clear();
  mc_charge.clear();

  pfo_n = 0;
  pfo_type.clear();
  pfo_pt.clear();
  pfo_theta.clear();
  pfo_eta.clear();
  pfo_phi.clear();
  pfo_e.clear();
  pfo_charge.clear();

  lep_n    = 0;
  lep_etot = 0;
  lep_type.clear();
  lep_pt.clear();
  lep_theta.clear();
  lep_eta.clear();
  lep_phi.clear();
  lep_e.clear();
  lep_charge.clear();
}
void ElectronDresser::fillMCInfo(LCEvent* evt) {
  std::string   collName       = m_mc_particles;
  LCCollection* thisCollection = 0;
  getCollection(thisCollection, collName, evt);
  if (thisCollection != NULL) {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": loop over collection " << std::left << std::setw(MAX_COLL_NAME_WIDTH)
                           << collName << ": " << thisCollection << std::endl;
    for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
      EVENT::MCParticle* particle = dynamic_cast<EVENT::MCParticle*>(thisCollection->getElementAt(i));
      fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                         particle->getEnergy());
      mc_gen_status.push_back(particle->getGeneratorStatus());
      mc_type.push_back(particle->getPDG());
      mc_pt.push_back(fourvec.Pt());
      mc_theta.push_back(fourvec.Theta());
      mc_eta.push_back(fourvec.Eta());
      mc_phi.push_back(fourvec.Phi());
      mc_e.push_back(fourvec.E());
      mc_charge.push_back(particle->getCharge());
    }
    mc_n = thisCollection->getNumberOfElements();
  } else {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": Warning: collection " << collName << " not available. Skip!"
                           << std::endl;
  }
}
void ElectronDresser::fillPFOs(LCEvent* evt) {
  std::string   collName       = m_pfos;
  LCCollection* thisCollection = 0;
  getCollection(thisCollection, collName, evt);
  if (thisCollection != NULL) {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": loop over collection " << std::left << std::setw(MAX_COLL_NAME_WIDTH)
                           << collName << ": " << thisCollection << std::endl;
    for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
      ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>(thisCollection->getElementAt(i));
      fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                         particle->getEnergy());
      if (abs(particle->getType()) < 20) {
        ++pfo_n;
        pfo_type.push_back(particle->getType());
        pfo_pt.push_back(fourvec.Pt());
        pfo_theta.push_back(fourvec.Theta());
        pfo_eta.push_back(fourvec.Eta());
        pfo_phi.push_back(fourvec.Phi());
        pfo_e.push_back(fourvec.E());
        pfo_charge.push_back(particle->getCharge());
      }
    }
  } else {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": Warning: collection " << collName << " not available. Skip!"
                           << std::endl;
  }
}
void ElectronDresser::fillLeptons(LCEvent* evt) {
  std::string   collName       = m_IsolatedLepton;
  LCCollection* thisCollection = 0;
  getCollection(thisCollection, collName, evt);
  if (thisCollection != NULL) {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": loop over collection " << std::left << std::setw(MAX_COLL_NAME_WIDTH)
                           << collName << ": " << thisCollection << std::endl;
    for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
      ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>(thisCollection->getElementAt(i));
      fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                         particle->getEnergy());
      if (abs(particle->getType()) < 20) {
        ++lep_n;
        lep_etot += fourvec.E();
        lep_type.push_back(particle->getType());
        lep_pt.push_back(fourvec.Pt());
        lep_theta.push_back(fourvec.Theta());
        lep_eta.push_back(fourvec.Eta());
        lep_phi.push_back(fourvec.Phi());
        lep_e.push_back(fourvec.E());
        lep_charge.push_back(particle->getCharge());
      }
    }
  } else {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": Warning: collection " << collName << " not available. Skip!"
                           << std::endl;
  }
}
void ElectronDresser::dressLeptons(LCEvent* evt) {
  streamlog_out(MESSAGE) << "Dressing leptons..." << std::endl;
  streamlog_out(MESSAGE) << "mc_n: " << mc_n << std::endl;
  streamlog_out(MESSAGE) << "pfo_n: " << pfo_n << std::endl;
  streamlog_out(MESSAGE) << "lep_n: " << lep_n << std::endl;

  LCCollectionVec* dressedCollection = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);

  std::vector<int> usedPFOs;

  for (int i = 0; i < lep_n; ++i) {
    fourvec.SetPtEtaPhiE(lep_pt.at(i), lep_eta.at(i), lep_phi.at(i), lep_e.at(i));
    for (int j = 0; j < pfo_n; ++j) {
      if (pfo_type.at(j) == 22) {
        tmpvec.SetPtEtaPhiE(pfo_pt.at(j), pfo_eta.at(j), pfo_phi.at(j), pfo_e.at(j));
        double dR = DeltaR(fourvec.Eta(), fourvec.Phi(), tmpvec.Eta(), tmpvec.Phi());
        printf("Analyzing lepton %d and photon pfo %d: dR = %f\n", i, j, dR);
        if (dR < ELECTRON_DRESS_MAX_DR) {
          if (std::find(usedPFOs.begin(), usedPFOs.end(), j) != usedPFOs.end()) {
            printf(
                "Warning: two leptons to be dressed with same PFO! Should not happen if dR_leptons > dR_dressRadius! Skip "
                "this PFO!\n");
            continue;
          }
          printf("Got one! Dressing Lepton %d with PFO %d: dR = %f, type %d\n", i, j, dR, pfo_type.at(j));
          fourvec += tmpvec;
          usedPFOs.push_back(j);
        }
      }
    }
    IMPL::ReconstructedParticleImpl* pReconstructedParticle = new IMPL::ReconstructedParticleImpl();
    const double                     momentum[3]            = {fourvec.Px(), fourvec.Py(), fourvec.Pz()};
    pReconstructedParticle->setMomentum(momentum);
    pReconstructedParticle->setEnergy(fourvec.E());
    pReconstructedParticle->setMass(fourvec.M());
    pReconstructedParticle->setCharge(lep_charge.at(i));
    pReconstructedParticle->setType(lep_type.at(i));
    dressedCollection->addElement(pReconstructedParticle);
  }

  evt->addCollection(dressedCollection, m_DressedLepton);
}
void ElectronDresser::getCollection(LCCollection*& collection, std::string collectionName, LCEvent* evt) {
  try {
    collection = evt->getCollection(collectionName);
  } catch (DataNotAvailableException& e) {
    std::cout << "- cannot get collections !!" << std::endl;
    streamlog_out(DEBUG4) << "Collection " << collectionName.c_str() << " is unavailable" << std::endl;
    return;
  }
  return;
}
