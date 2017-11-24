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
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "", "", _m_pfos, std::string("LooseSelectedPandoraPFANewPFOs"));
  registerInputCollection(LCIO::MCPARTICLE, "", "", _m_mc_particles, std::string("MCParticlesSkimmed"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "", "", _m_IsolatedLepton, std::string("IsolatedLeptonCollection"));
  // output
  registerOutputCollection(LCIO::RECONSTRUCTEDPARTICLE, "", "", _m_DressedLepton, std::string("DressedLeptonCollection"));
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
  _fourvec.SetPxPyPzE(0, 0, 0, 0);
  _tmpvec.SetPxPyPzE(0, 0, 0, 0);

  _mc_n = 0;
  _mc_gen_status.clear();
  _mc_type.clear();
  _mc_pt.clear();
  _mc_theta.clear();
  _mc_eta.clear();
  _mc_phi.clear();
  _mc_e.clear();
  _mc_charge.clear();

  _pfo_n = 0;
  _pfo_type.clear();
  _pfo_pt.clear();
  _pfo_theta.clear();
  _pfo_eta.clear();
  _pfo_phi.clear();
  _pfo_e.clear();
  _pfo_charge.clear();

  _lep_n    = 0;
  _lep_etot = 0;
  _lep_type.clear();
  _lep_pt.clear();
  _lep_theta.clear();
  _lep_eta.clear();
  _lep_phi.clear();
  _lep_e.clear();
  _lep_charge.clear();
}
void ElectronDresser::fillMCInfo(LCEvent* evt) {
  std::string   collName       = _m_mc_particles;
  LCCollection* thisCollection = 0;
  getCollection(thisCollection, collName, evt);
  if (thisCollection != NULL) {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": loop over collection " << std::left << std::setw(MAX_COLL_NAME_WIDTH)
                           << collName << ": " << thisCollection << std::endl;
    for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
      EVENT::MCParticle* particle = dynamic_cast<EVENT::MCParticle*>(thisCollection->getElementAt(i));
      _fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                          particle->getEnergy());
      _mc_gen_status.push_back(particle->getGeneratorStatus());
      _mc_type.push_back(particle->getPDG());
      _mc_pt.push_back(_fourvec.Pt());
      _mc_theta.push_back(_fourvec.Theta());
      _mc_eta.push_back(_fourvec.Eta());
      _mc_phi.push_back(_fourvec.Phi());
      _mc_e.push_back(_fourvec.E());
      _mc_charge.push_back(particle->getCharge());
    }
    _mc_n = thisCollection->getNumberOfElements();
  } else {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": Warning: collection " << collName << " not available. Skip!"
                           << std::endl;
  }
}
void ElectronDresser::fillPFOs(LCEvent* evt) {
  std::string   collName       = _m_pfos;
  LCCollection* thisCollection = 0;
  getCollection(thisCollection, collName, evt);
  if (thisCollection != NULL) {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": loop over collection " << std::left << std::setw(MAX_COLL_NAME_WIDTH)
                           << collName << ": " << thisCollection << std::endl;
    for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
      ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>(thisCollection->getElementAt(i));
      _fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                          particle->getEnergy());
      if (abs(particle->getType()) < 20) {
        ++_pfo_n;
        _pfo_type.push_back(particle->getType());
        _pfo_pt.push_back(_fourvec.Pt());
        _pfo_theta.push_back(_fourvec.Theta());
        _pfo_eta.push_back(_fourvec.Eta());
        _pfo_phi.push_back(_fourvec.Phi());
        _pfo_e.push_back(_fourvec.E());
        _pfo_charge.push_back(particle->getCharge());
      }
    }
  } else {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": Warning: collection " << collName << " not available. Skip!"
                           << std::endl;
  }
}
void ElectronDresser::fillLeptons(LCEvent* evt) {
  std::string   collName       = _m_IsolatedLepton;
  LCCollection* thisCollection = 0;
  getCollection(thisCollection, collName, evt);
  if (thisCollection != NULL) {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": loop over collection " << std::left << std::setw(MAX_COLL_NAME_WIDTH)
                           << collName << ": " << thisCollection << std::endl;
    for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
      ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>(thisCollection->getElementAt(i));
      _fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                          particle->getEnergy());
      if (abs(particle->getType()) < 20) {
        ++_lep_n;
        _lep_etot += _fourvec.E();
        _lep_type.push_back(particle->getType());
        _lep_pt.push_back(_fourvec.Pt());
        _lep_theta.push_back(_fourvec.Theta());
        _lep_eta.push_back(_fourvec.Eta());
        _lep_phi.push_back(_fourvec.Phi());
        _lep_e.push_back(_fourvec.E());
        _lep_charge.push_back(particle->getCharge());
      }
    }
  } else {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": Warning: collection " << collName << " not available. Skip!"
                           << std::endl;
  }
}
void ElectronDresser::dressLeptons(LCEvent* evt) {
  streamlog_out(MESSAGE) << "Dressing leptons..." << std::endl;
  streamlog_out(MESSAGE) << "mc_n: " << _mc_n << std::endl;
  streamlog_out(MESSAGE) << "pfo_n: " << _pfo_n << std::endl;
  streamlog_out(MESSAGE) << "lep_n: " << _lep_n << std::endl;

  LCCollectionVec* dressedCollection = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);

  std::vector<int> usedPFOs;

  for (int i = 0; i < _lep_n; ++i) {
    _fourvec.SetPtEtaPhiE(_lep_pt.at(i), _lep_eta.at(i), _lep_phi.at(i), _lep_e.at(i));
    for (int j = 0; j < _pfo_n; ++j) {
      if (_pfo_type.at(j) == 22) {
        _tmpvec.SetPtEtaPhiE(_pfo_pt.at(j), _pfo_eta.at(j), _pfo_phi.at(j), _pfo_e.at(j));
        double dR = DeltaR(_fourvec.Eta(), _fourvec.Phi(), _tmpvec.Eta(), _tmpvec.Phi());
        streamlog_out(MESSAGE) << "Analyzing lepton " << i << " and photon pfo " << j << ": dR = " << dR << std::endl;
        if (dR < ELECTRON_DRESS_MAX_DR) {
          if (std::find(usedPFOs.begin(), usedPFOs.end(), j) != usedPFOs.end()) {
            streamlog_out(MESSAGE) << "Warning: two leptons to be dressed with same PFO! Should not happen if dR_leptons > "
                                      "dR_dressRadius! Skip this PFO!"
                                   << std::endl;
            continue;
          }
          streamlog_out(MESSAGE) << "Got one! Dressing Lepton " << i << " with PFO " << j << ": dR = " << dR << ", type "
                                 << _pfo_type.at(j) << std::endl;
          _fourvec += _tmpvec;
          usedPFOs.push_back(j);
        }
      }
    }
    IMPL::ReconstructedParticleImpl* pReconstructedParticle = new IMPL::ReconstructedParticleImpl();
    const double                     momentum[3]            = {_fourvec.Px(), _fourvec.Py(), _fourvec.Pz()};
    pReconstructedParticle->setMomentum(momentum);
    pReconstructedParticle->setEnergy(_fourvec.E());
    pReconstructedParticle->setMass(_fourvec.M());
    pReconstructedParticle->setCharge(_lep_charge.at(i));
    pReconstructedParticle->setType(_lep_type.at(i));
    dressedCollection->addElement(pReconstructedParticle);
  }

  evt->addCollection(dressedCollection, _m_DressedLepton);
}
void ElectronDresser::getCollection(LCCollection*& collection, std::string collectionName, LCEvent* evt) {
  try {
    collection = evt->getCollection(collectionName);
  } catch (DataNotAvailableException& e) {
    streamlog_out(MESSAGE) << "- cannot get collections !!" << std::endl;
    streamlog_out(DEBUG4) << "Collection " << collectionName.c_str() << " is unavailable" << std::endl;
    return;
  }
  return;
}
