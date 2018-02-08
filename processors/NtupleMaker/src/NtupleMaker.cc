#include "NtupleMaker.h"

const int MAX_COLL_NAME_WIDTH = 20;

double DeltaR(double eta0, double phi0, double eta1, double phi1) {
  Double_t deta = eta0 - eta1;
  Double_t dphi = TVector2::Phi_mpi_pi(phi0 - phi1);
  return TMath::Sqrt(deta * deta + dphi * dphi);
}
double EtaFromTheta(double theta) { return -TMath::Log(TMath::Tan(theta / 2)); }

NtupleMaker aNtupleMaker;

NtupleMaker::NtupleMaker() : Processor("NtupleMaker") {
  // modify processor description
  _description = "NtupleMaker does whatever it does ...";

  // register input parameters: collection type, aribtrary name, arbitrary description, class-variable, default value
  // input
  registerInputCollection(LCIO::MCPARTICLE, "MCParticlesSkimmed", "", _m_mc_particles, std::string("MCParticlesSkimmed"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "IsolatedLeptonCollection", "", _m_IsolatedLepton,
                          std::string("IsolatedLeptonCollection"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "kt_R07", "", _m_kt_R07, std::string("kt_R07"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "kt_R10", "", _m_kt_R10, std::string("kt_R10"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "kt_R12", "", _m_kt_R12, std::string("kt_R12"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "vlc_R06", "", _m_vlc_R06, std::string("vlc_R06"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "vlc_R08", "", _m_vlc_R08, std::string("vlc_R08"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "vlc_R10", "", _m_vlc_R10, std::string("vlc_R10"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "vlc_R08_g05", "", _m_vlc_R08_g05, std::string("vlc_R08_g05"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "vlc_R08_g10", "", _m_vlc_R08_g10, std::string("vlc_R08_g10"));

  // register steering parameters: arbitrary name, arbitrary description, class-variable, default value
  registerProcessorParameter("OutputFileName", "", _m_outfileName, std::string("output.root"));
}

void NtupleMaker::init() {
  streamlog_out(MESSAGE) << "   init called  " << std::endl;
  printParameters();
  _nRun         = 0;
  _nEvt         = 0;
  _hist_m_W_lep = new TH1F("hist_m_W_lep", "hist_m_W_lep", 50, 0, 200);
  _hist_m_W_had = new TH1F("hist_m_W_had", "hist_m_W_had", 50, 0, 200);
  _recoInputCollections.push_back(_m_IsolatedLepton);
  _recoInputCollections.push_back(_m_kt_R07);
  _recoInputCollections.push_back(_m_kt_R10);
  _recoInputCollections.push_back(_m_kt_R12);
  _recoInputCollections.push_back(_m_vlc_R06);
  _recoInputCollections.push_back(_m_vlc_R08);
  _recoInputCollections.push_back(_m_vlc_R10);
  _recoInputCollections.push_back(_m_vlc_R08_g05);
  _recoInputCollections.push_back(_m_vlc_R08_g10);

  streamlog_out(MESSAGE) << "First event: initializing globals..." << std::endl;
  _ntupleFile  = new TFile(_m_outfileName.c_str(), "RECREATE");
  _rawTree     = new TTree("rawTree", "rawTree");
  int buffsize = 32000;  //default buffer size 32KB

  _rawTree->Branch("qq_m", &_qq_m);
  _rawTree->Branch("ln_m", &_ln_m);
  _rawTree->Branch("beam_e", &_beam_e);
  _rawTree->Branch("beam_m", &_beam_m);

  _rawTree->Branch("miss_pt", &_miss_pt);
  _rawTree->Branch("miss_theta", &_miss_theta);
  _rawTree->Branch("miss_phi", &_miss_phi);
  _rawTree->Branch("miss_e", &_miss_e);

  _rawTree->Branch("mc_beam_e", &_mc_beam_e);
  _rawTree->Branch("mc_beam_m", &_mc_beam_m);
  _rawTree->Branch("mc_qq_m", &_mc_qq_m);
  _rawTree->Branch("mc_ln_m", &_mc_ln_m);
  _rawTree->Branch("mc_n", &_mc_n);
  _rawTree->Branch("mc_gen_status", "std::vector<int >", &_mc_gen_status, buffsize, 0);
  _rawTree->Branch("mc_type", "std::vector<int >", &_mc_type, buffsize, 0);
  _rawTree->Branch("mc_pt", "std::vector<double >", &_mc_pt, buffsize, 0);
  _rawTree->Branch("mc_theta", "std::vector<double >", &_mc_theta, buffsize, 0);
  _rawTree->Branch("mc_phi", "std::vector<double >", &_mc_phi, buffsize, 0);
  _rawTree->Branch("mc_e", "std::vector<double >", &_mc_e, buffsize, 0);
  _rawTree->Branch("mc_charge", "std::vector<double >", &_mc_charge, buffsize, 0);

  _rawTree->Branch("lep_n", &_lep_n);
  _rawTree->Branch("lep_etot", &_lep_etot);
  _rawTree->Branch("lep_type", "std::vector<int >", &_lep_type, buffsize, 0);
  _rawTree->Branch("lep_pt", "std::vector<double >", &_lep_pt, buffsize, 0);
  _rawTree->Branch("lep_theta", "std::vector<double >", &_lep_theta, buffsize, 0);
  _rawTree->Branch("lep_phi", "std::vector<double >", &_lep_phi, buffsize, 0);
  _rawTree->Branch("lep_e", "std::vector<double >", &_lep_e, buffsize, 0);
  _rawTree->Branch("lep_charge", "std::vector<double >", &_lep_charge, buffsize, 0);

  _rawTree->Branch("jet_kt_R07_n", &_jet_kt_R07_n);
  _rawTree->Branch("jet_kt_R07_etot", &_jet_kt_R07_etot);
  _rawTree->Branch("jet_kt_R07_pt", "std::vector<double >", &_jet_kt_R07_pt, buffsize, 0);
  _rawTree->Branch("jet_kt_R07_theta", "std::vector<double >", &_jet_kt_R07_theta, buffsize, 0);
  _rawTree->Branch("jet_kt_R07_phi", "std::vector<double >", &_jet_kt_R07_phi, buffsize, 0);
  _rawTree->Branch("jet_kt_R07_e", "std::vector<double >", &_jet_kt_R07_e, buffsize, 0);
  _rawTree->Branch("jet_kt_R07_charge", "std::vector<double >", &_jet_kt_R07_charge, buffsize, 0);

  _rawTree->Branch("jet_kt_R10_n", &_jet_kt_R10_n);
  _rawTree->Branch("jet_kt_R10_etot", &_jet_kt_R10_etot);
  _rawTree->Branch("jet_kt_R10_pt", "std::vector<double >", &_jet_kt_R10_pt, buffsize, 0);
  _rawTree->Branch("jet_kt_R10_theta", "std::vector<double >", &_jet_kt_R10_theta, buffsize, 0);
  _rawTree->Branch("jet_kt_R10_phi", "std::vector<double >", &_jet_kt_R10_phi, buffsize, 0);
  _rawTree->Branch("jet_kt_R10_e", "std::vector<double >", &_jet_kt_R10_e, buffsize, 0);
  _rawTree->Branch("jet_kt_R10_charge", "std::vector<double >", &_jet_kt_R10_charge, buffsize, 0);

  _rawTree->Branch("jet_kt_R12_n", &_jet_kt_R12_n);
  _rawTree->Branch("jet_kt_R12_etot", &_jet_kt_R12_etot);
  _rawTree->Branch("jet_kt_R12_pt", "std::vector<double >", &_jet_kt_R12_pt, buffsize, 0);
  _rawTree->Branch("jet_kt_R12_theta", "std::vector<double >", &_jet_kt_R12_theta, buffsize, 0);
  _rawTree->Branch("jet_kt_R12_phi", "std::vector<double >", &_jet_kt_R12_phi, buffsize, 0);
  _rawTree->Branch("jet_kt_R12_e", "std::vector<double >", &_jet_kt_R12_e, buffsize, 0);
  _rawTree->Branch("jet_kt_R12_charge", "std::vector<double >", &_jet_kt_R12_charge, buffsize, 0);

  _rawTree->Branch("jet_vlc_R06_n", &_jet_vlc_R06_n);
  _rawTree->Branch("jet_vlc_R06_etot", &_jet_vlc_R06_etot);
  _rawTree->Branch("jet_vlc_R06_pt", "std::vector<double >", &_jet_vlc_R06_pt, buffsize, 0);
  _rawTree->Branch("jet_vlc_R06_theta", "std::vector<double >", &_jet_vlc_R06_theta, buffsize, 0);
  _rawTree->Branch("jet_vlc_R06_phi", "std::vector<double >", &_jet_vlc_R06_phi, buffsize, 0);
  _rawTree->Branch("jet_vlc_R06_e", "std::vector<double >", &_jet_vlc_R06_e, buffsize, 0);
  _rawTree->Branch("jet_vlc_R06_charge", "std::vector<double >", &_jet_vlc_R06_charge, buffsize, 0);

  _rawTree->Branch("jet_vlc_R08_n", &_jet_vlc_R08_n);
  _rawTree->Branch("jet_vlc_R08_etot", &_jet_vlc_R08_etot);
  _rawTree->Branch("jet_vlc_R08_pt", "std::vector<double >", &_jet_vlc_R08_pt, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_theta", "std::vector<double >", &_jet_vlc_R08_theta, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_phi", "std::vector<double >", &_jet_vlc_R08_phi, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_e", "std::vector<double >", &_jet_vlc_R08_e, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_charge", "std::vector<double >", &_jet_vlc_R08_charge, buffsize, 0);

  _rawTree->Branch("jet_vlc_R10_n", &_jet_vlc_R10_n);
  _rawTree->Branch("jet_vlc_R10_etot", &_jet_vlc_R10_etot);
  _rawTree->Branch("jet_vlc_R10_pt", "std::vector<double >", &_jet_vlc_R10_pt, buffsize, 0);
  _rawTree->Branch("jet_vlc_R10_theta", "std::vector<double >", &_jet_vlc_R10_theta, buffsize, 0);
  _rawTree->Branch("jet_vlc_R10_phi", "std::vector<double >", &_jet_vlc_R10_phi, buffsize, 0);
  _rawTree->Branch("jet_vlc_R10_e", "std::vector<double >", &_jet_vlc_R10_e, buffsize, 0);
  _rawTree->Branch("jet_vlc_R10_charge", "std::vector<double >", &_jet_vlc_R10_charge, buffsize, 0);

  _rawTree->Branch("jet_vlc_R08_g05_n", &_jet_vlc_R08_g05_n);
  _rawTree->Branch("jet_vlc_R08_g05_etot", &_jet_vlc_R08_g05_etot);
  _rawTree->Branch("jet_vlc_R08_g05_pt", "std::vector<double >", &_jet_vlc_R08_g05_pt, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_g05_theta", "std::vector<double >", &_jet_vlc_R08_g05_theta, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_g05_phi", "std::vector<double >", &_jet_vlc_R08_g05_phi, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_g05_e", "std::vector<double >", &_jet_vlc_R08_g05_e, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_g05_charge", "std::vector<double >", &_jet_vlc_R08_g05_charge, buffsize, 0);

  _rawTree->Branch("jet_vlc_R08_g10_n", &_jet_vlc_R08_g10_n);
  _rawTree->Branch("jet_vlc_R08_g10_etot", &_jet_vlc_R08_g10_etot);
  _rawTree->Branch("jet_vlc_R08_g10_pt", "std::vector<double >", &_jet_vlc_R08_g10_pt, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_g10_theta", "std::vector<double >", &_jet_vlc_R08_g10_theta, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_g10_phi", "std::vector<double >", &_jet_vlc_R08_g10_phi, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_g10_e", "std::vector<double >", &_jet_vlc_R08_g10_e, buffsize, 0);
  _rawTree->Branch("jet_vlc_R08_g10_charge", "std::vector<double >", &_jet_vlc_R08_g10_charge, buffsize, 0);
}

void NtupleMaker::processRunHeader(LCRunHeader* run) {
  run = run;
  _nRun++;
}

void NtupleMaker::processEvent(LCEvent* evt) {
  streamlog_out(MESSAGE) << "   Processing event: " << evt->getEventNumber() << "   in run:  " << evt->getRunNumber()
                         << std::endl;

  clearEventVariables();

  streamlog_out(MESSAGE) << "Analyzing collections:" << std::endl;
  for (unsigned int iColl = 0; iColl < _recoInputCollections.size(); ++iColl) {
    fillRecoParticles(_recoInputCollections.at(iColl), evt);
  }
  fillMCInfo(evt);
  fillMissingEnergy();
  fillBeamEnergy();
  fillOtherVars();
  fillHistograms();
  streamlog_out(MESSAGE) << "Event " << _nEvt << ": Fill tree..." << std::endl;
  _rawTree->Fill();

  _nEvt++;
}

void NtupleMaker::end() {
  streamlog_out(MESSAGE) << "Write tree..." << std::endl;
  _ntupleFile->cd();
  _rawTree->Write();
  _hist_m_W_lep->Write();
  _hist_m_W_had->Write();
  // _rawTree->Print("all");
  _ntupleFile->Close();

  printParameters();
  streamlog_out(MESSAGE) << "NtupleMaker::end()  " << name() << " processed " << _nEvt << " events in " << _nRun << " runs "
                         << std::endl;
}

void NtupleMaker::clearEventVariables() {
  _fourvec.SetPxPyPzE(0, 0, 0, 0);
  _tmp0vec.SetPxPyPzE(0, 0, 0, 0);
  _tmp1vec.SetPxPyPzE(0, 0, 0, 0);
  _tmp2vec.SetPxPyPzE(0, 0, 0, 0);
  _tmp3vec.SetPxPyPzE(0, 0, 0, 0);

  _qq_m   = 0;
  _ln_m   = 0;
  _beam_e = 0;
  _beam_m = 0;

  _miss_pt    = 0;
  _miss_theta = 0;
  _miss_phi   = 0;
  _miss_e     = 0;

  _mc_n      = 0;
  _mc_beam_e = 0;
  _mc_beam_m = 0;
  _mc_qq_m   = 0;
  _mc_ln_m   = 0;
  _mc_gen_status.clear();
  _mc_type.clear();
  _mc_pt.clear();
  _mc_theta.clear();
  _mc_phi.clear();
  _mc_e.clear();
  _mc_charge.clear();

  _lep_n    = 0;
  _lep_etot = 0;
  _lep_type.clear();
  _lep_pt.clear();
  _lep_theta.clear();
  _lep_phi.clear();
  _lep_e.clear();
  _lep_charge.clear();

  _jet_kt_R07_n    = 0;
  _jet_kt_R07_etot = 0;
  _jet_kt_R07_pt.clear();
  _jet_kt_R07_theta.clear();
  _jet_kt_R07_phi.clear();
  _jet_kt_R07_e.clear();
  _jet_kt_R07_charge.clear();

  _jet_kt_R10_n    = 0;
  _jet_kt_R10_etot = 0;
  _jet_kt_R10_pt.clear();
  _jet_kt_R10_theta.clear();
  _jet_kt_R10_phi.clear();
  _jet_kt_R10_e.clear();
  _jet_kt_R10_charge.clear();

  _jet_kt_R12_n    = 0;
  _jet_kt_R12_etot = 0;
  _jet_kt_R12_pt.clear();
  _jet_kt_R12_theta.clear();
  _jet_kt_R12_phi.clear();
  _jet_kt_R12_e.clear();
  _jet_kt_R12_charge.clear();

  _jet_vlc_R06_n    = 0;
  _jet_vlc_R06_etot = 0;
  _jet_vlc_R06_pt.clear();
  _jet_vlc_R06_theta.clear();
  _jet_vlc_R06_phi.clear();
  _jet_vlc_R06_e.clear();
  _jet_vlc_R06_charge.clear();

  _jet_vlc_R08_n    = 0;
  _jet_vlc_R08_etot = 0;
  _jet_vlc_R08_pt.clear();
  _jet_vlc_R08_theta.clear();
  _jet_vlc_R08_phi.clear();
  _jet_vlc_R08_e.clear();
  _jet_vlc_R08_charge.clear();

  _jet_vlc_R10_n    = 0;
  _jet_vlc_R10_etot = 0;
  _jet_vlc_R10_pt.clear();
  _jet_vlc_R10_theta.clear();
  _jet_vlc_R10_phi.clear();
  _jet_vlc_R10_e.clear();
  _jet_vlc_R10_charge.clear();

  _jet_vlc_R08_g05_n    = 0;
  _jet_vlc_R08_g05_etot = 0;
  _jet_vlc_R08_g05_pt.clear();
  _jet_vlc_R08_g05_theta.clear();
  _jet_vlc_R08_g05_phi.clear();
  _jet_vlc_R08_g05_e.clear();
  _jet_vlc_R08_g05_charge.clear();

  _jet_vlc_R08_g10_n    = 0;
  _jet_vlc_R08_g10_etot = 0;
  _jet_vlc_R08_g10_pt.clear();
  _jet_vlc_R08_g10_theta.clear();
  _jet_vlc_R08_g10_phi.clear();
  _jet_vlc_R08_g10_e.clear();
  _jet_vlc_R08_g10_charge.clear();
}
void NtupleMaker::fillMissingEnergy() {
  try {
    _jet_vlc_R08_pt.at(0);
    _jet_vlc_R08_pt.at(1);
  } catch (const std::out_of_range& oor) {
    std::cerr << "Out of Range error: " << oor.what() << '\n';
    streamlog_out(MESSAGE) << "Run particle reconstruction before missing energy reconstruction!" << std::endl;
    return;
  }

  // start with empty lorentzvector
  _fourvec.SetPxPyPzE(0, 0, 0, 0);
  // printf("\nMissing vec ini: pt %.3f theta %.3f phi %.3f e %.3f m %.3f\n", _fourvec.Pt(), _fourvec.Theta(), _fourvec.Phi(), _fourvec.E(), _fourvec.M());

  // fill reconstructed visible particles
  for (unsigned int i = 0; i < _jet_vlc_R08_pt.size(); i++) {
    _tmp0vec.SetPtEtaPhiE(_jet_vlc_R08_pt.at(i), EtaFromTheta(_jet_vlc_R08_theta.at(i)), _jet_vlc_R08_phi.at(i),
                          _jet_vlc_R08_e.at(i));
    _fourvec += _tmp0vec;
    // printf("Missing vec after jet %d: pt %.3f theta %.3f phi %.3f e %.3f m %.3f\n", i, _fourvec.Pt(), _fourvec.Theta(), _fourvec.Phi(), _fourvec.E(), _fourvec.M());
  }
  for (unsigned int i = 0; i < _lep_pt.size(); i++) {
    _tmp0vec.SetPtEtaPhiE(_lep_pt.at(i), EtaFromTheta(_lep_theta.at(i)), _lep_phi.at(i),
                          _lep_e.at(i));
    _fourvec += _tmp0vec;
    // printf("Missing vec after lep %d: pt %.3f theta %.3f phi %.3f e %.3f m %.3f\n", i, _fourvec.Pt(), _fourvec.Theta(), _fourvec.Phi(), _fourvec.E(), _fourvec.M());
  }

  // take opposite
  _fourvec *= -1;

  // remove mass
  _fourvec.SetE(sqrt(pow(_fourvec.Px(), 2) + pow(_fourvec.Py(), 2) + pow(_fourvec.Pz(), 2)));

  // fill missing energy
  _miss_pt    = _fourvec.Pt();
  _miss_theta = _fourvec.Theta();
  _miss_phi   = _fourvec.Phi();
  _miss_e     = _fourvec.E();
}
void NtupleMaker::fillMCInfo(LCEvent* evt) {
  std::string   collName       = _m_mc_particles;
  LCCollection* thisCollection = 0;
  getCollection(thisCollection, collName, evt);
  if (thisCollection != NULL) {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": loop over collection " << std::left << std::setw(MAX_COLL_NAME_WIDTH)
                           << collName << ": " << thisCollection << std::endl;
    for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
      if (i > 11) {
        break;
      }
      /*
        (0, 1) electron and positron before ISR/FSR
        (2, 3) after
        (4, 5) the ISR/FSR photons
        (6, 7) first two quarks
        (8, 9) first two leptons, charged (8) and neutrino (9)
        (10, 11) the final ISR/FSR photons
      */
      EVENT::MCParticle* particle = dynamic_cast<EVENT::MCParticle*>(thisCollection->getElementAt(i));
      _fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                          particle->getEnergy());
      _mc_gen_status.push_back(particle->getGeneratorStatus());
      _mc_type.push_back(particle->getPDG());
      _mc_pt.push_back(_fourvec.Pt());
      _mc_theta.push_back(_fourvec.Theta());
      _mc_phi.push_back(_fourvec.Phi());
      _mc_e.push_back(_fourvec.E());
      _mc_charge.push_back(particle->getCharge());
      if (i == 2)
        _tmp0vec = _fourvec;
      if (i == 3) {
        _tmp1vec   = _fourvec;
        _mc_beam_e = (_tmp0vec + _tmp1vec).E();
        _mc_beam_m = (_tmp0vec + _tmp1vec).M();
      }
      if (i == 6)
        _tmp0vec = _fourvec;
      if (i == 7) {
        _tmp1vec = _fourvec;
        _mc_qq_m = (_tmp0vec + _tmp1vec).M();
      }
      if (i == 8)
        _tmp2vec = _fourvec;
      if (i == 9) {
        _tmp3vec = _fourvec;
        _mc_ln_m = (_tmp2vec + _tmp3vec).M();
      }
    }
    _mc_n = thisCollection->getNumberOfElements();
  } else {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": Warning: collection " << collName << " not available. Skip!"
                           << std::endl;
  }
}
void NtupleMaker::fillRecoParticles(std::string collName, LCEvent* evt) {
  LCCollection* thisCollection = 0;
  getCollection(thisCollection, collName, evt);
  if (thisCollection != NULL) {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": loop over collection " << std::left << std::setw(MAX_COLL_NAME_WIDTH)
                           << collName << ": " << thisCollection << std::endl;
    std::vector<int> index = orderByPt(thisCollection);
    for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
      ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>(thisCollection->getElementAt(index[i]));
      fillVectors(collName, particle);
    }
  } else {
    streamlog_out(MESSAGE) << "Event " << _nEvt << ": Warning: collection " << collName << " not available. Skip!"
                           << std::endl;
  }
}
std::vector<int> NtupleMaker::orderByPt(LCCollection* thisCollection) {
  std::vector<int>    index;
  std::vector<double> ptVec;
  for (int i = 0; i < thisCollection->getNumberOfElements(); i++) {
    ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>(thisCollection->getElementAt(i));
    _fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                        particle->getEnergy());
    ptVec.push_back(_fourvec.Pt());
    index.push_back(i);
  }
  // now get index depending on pt order
  std::sort(index.begin(), index.end(), [&](const int& a, const int& b) { return (ptVec[a] > ptVec[b]); });
  return index;
}
void NtupleMaker::fillVectors(std::string collName, ReconstructedParticle* particle) {
  _fourvec.SetPxPyPzE(particle->getMomentum()[0], particle->getMomentum()[1], particle->getMomentum()[2],
                      particle->getEnergy());
  if (collName == _m_IsolatedLepton) {
    if (abs(particle->getType()) < 20) {
      ++_lep_n;
      _lep_etot += _fourvec.E();
      _lep_type.push_back(particle->getType());
      _lep_pt.push_back(_fourvec.Pt());
      _lep_theta.push_back(_fourvec.Theta());
      _lep_phi.push_back(_fourvec.Phi());
      _lep_e.push_back(_fourvec.E());
      _lep_charge.push_back(particle->getCharge());
    }
  } else if (collName == _m_kt_R07) {
    ++_jet_kt_R07_n;
    _jet_kt_R07_etot += _fourvec.E();
    _jet_kt_R07_pt.push_back(_fourvec.Pt());
    _jet_kt_R07_theta.push_back(_fourvec.Theta());
    _jet_kt_R07_phi.push_back(_fourvec.Phi());
    _jet_kt_R07_e.push_back(_fourvec.E());
    _jet_kt_R07_charge.push_back(particle->getCharge());
  } else if (collName == _m_kt_R10) {
    ++_jet_kt_R10_n;
    _jet_kt_R10_etot += _fourvec.E();
    _jet_kt_R10_pt.push_back(_fourvec.Pt());
    _jet_kt_R10_theta.push_back(_fourvec.Theta());
    _jet_kt_R10_phi.push_back(_fourvec.Phi());
    _jet_kt_R10_e.push_back(_fourvec.E());
    _jet_kt_R10_charge.push_back(particle->getCharge());
  } else if (collName == _m_kt_R12) {
    ++_jet_kt_R12_n;
    _jet_kt_R12_etot += _fourvec.E();
    _jet_kt_R12_pt.push_back(_fourvec.Pt());
    _jet_kt_R12_theta.push_back(_fourvec.Theta());
    _jet_kt_R12_phi.push_back(_fourvec.Phi());
    _jet_kt_R12_e.push_back(_fourvec.E());
    _jet_kt_R12_charge.push_back(particle->getCharge());
  } else if (collName == _m_vlc_R06) {
    ++_jet_vlc_R06_n;
    _jet_vlc_R06_etot += _fourvec.E();
    _jet_vlc_R06_pt.push_back(_fourvec.Pt());
    _jet_vlc_R06_theta.push_back(_fourvec.Theta());
    _jet_vlc_R06_phi.push_back(_fourvec.Phi());
    _jet_vlc_R06_e.push_back(_fourvec.E());
    _jet_vlc_R06_charge.push_back(particle->getCharge());
  } else if (collName == _m_vlc_R08) {
    ++_jet_vlc_R08_n;
    _jet_vlc_R08_etot += _fourvec.E();
    _jet_vlc_R08_pt.push_back(_fourvec.Pt());
    _jet_vlc_R08_theta.push_back(_fourvec.Theta());
    _jet_vlc_R08_phi.push_back(_fourvec.Phi());
    _jet_vlc_R08_e.push_back(_fourvec.E());
    _jet_vlc_R08_charge.push_back(particle->getCharge());
  } else if (collName == _m_vlc_R10) {
    ++_jet_vlc_R10_n;
    _jet_vlc_R10_etot += _fourvec.E();
    _jet_vlc_R10_pt.push_back(_fourvec.Pt());
    _jet_vlc_R10_theta.push_back(_fourvec.Theta());
    _jet_vlc_R10_phi.push_back(_fourvec.Phi());
    _jet_vlc_R10_e.push_back(_fourvec.E());
    _jet_vlc_R10_charge.push_back(particle->getCharge());
  } else if (collName == _m_vlc_R08_g05) {
    ++_jet_vlc_R08_g05_n;
    _jet_vlc_R08_g05_etot += _fourvec.E();
    _jet_vlc_R08_g05_pt.push_back(_fourvec.Pt());
    _jet_vlc_R08_g05_theta.push_back(_fourvec.Theta());
    _jet_vlc_R08_g05_phi.push_back(_fourvec.Phi());
    _jet_vlc_R08_g05_e.push_back(_fourvec.E());
    _jet_vlc_R08_g05_charge.push_back(particle->getCharge());
  } else if (collName == _m_vlc_R08_g10) {
    ++_jet_vlc_R08_g10_n;
    _jet_vlc_R08_g10_etot += _fourvec.E();
    _jet_vlc_R08_g10_pt.push_back(_fourvec.Pt());
    _jet_vlc_R08_g10_theta.push_back(_fourvec.Theta());
    _jet_vlc_R08_g10_phi.push_back(_fourvec.Phi());
    _jet_vlc_R08_g10_e.push_back(_fourvec.E());
    _jet_vlc_R08_g10_charge.push_back(particle->getCharge());
  }
}
void NtupleMaker::fillBeamEnergy() {
  _beam_e = 0;
  _beam_m = 0;
}
void NtupleMaker::fillOtherVars() {
  if (_jet_vlc_R08_pt.size() == 2) {
    _tmp0vec.SetPtEtaPhiE(_jet_vlc_R08_pt.at(0), EtaFromTheta(_jet_vlc_R08_theta.at(0)), _jet_vlc_R08_phi.at(0),
                          _jet_vlc_R08_e.at(0));
    _tmp1vec.SetPtEtaPhiE(_jet_vlc_R08_pt.at(1), EtaFromTheta(_jet_vlc_R08_theta.at(1)), _jet_vlc_R08_phi.at(1),
                          _jet_vlc_R08_e.at(1));
    _qq_m = (_tmp0vec + _tmp1vec).M();
  }
  if (_lep_pt.size() > 0) {
    _tmp0vec.SetPtEtaPhiE(_lep_pt.at(0), EtaFromTheta(_lep_theta.at(0)), _lep_phi.at(0), _lep_e.at(0));
    _tmp1vec.SetPtEtaPhiE(_miss_pt, EtaFromTheta(_miss_theta), _miss_phi, _miss_e);
    _ln_m = (_tmp0vec + _tmp1vec).M();
  }
}
void NtupleMaker::fillHistograms() {
  _hist_m_W_had->Fill(_qq_m);
  _hist_m_W_lep->Fill(_ln_m);
}
void NtupleMaker::getCollection(LCCollection*& collection, std::string collectionName, LCEvent* evt) {
  try {
    collection = evt->getCollection(collectionName);
  } catch (DataNotAvailableException& e) {
    streamlog_out(MESSAGE) << "- cannot get collections !!" << std::endl;
    streamlog_out(DEBUG4) << "Collection " << collectionName.c_str() << " is unavailable" << std::endl;
    return;
  }
  return;
}
