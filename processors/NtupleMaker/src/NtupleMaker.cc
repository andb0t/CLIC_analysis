//written by Andreas Alexander Maier July 2017
//andreas.alexander.maier@cern.ch
#include "NtupleMaker.h"


const int MAX_COLL_NAME_WIDTH = 20;


double DeltaR(double eta0,double phi0, double eta1, double phi1){
   Double_t deta = eta0-eta1;
   Double_t dphi = TVector2::Phi_mpi_pi(phi0-phi1);
   return TMath::Sqrt( deta*deta+dphi*dphi );
}
double EtaFromTheta(double theta){
  return -TMath::Log(TMath::Tan(theta/2));
}



NtupleMaker aNtupleMaker ;


NtupleMaker::NtupleMaker() : Processor("NtupleMaker") {
    // modify processor description
    _description = "NtupleMaker does whatever it does ..." ;

    // input
    // register input parameters: collection type, aribtrary name, arbitrary description, class-variable, default value
    registerInputCollection( LCIO::MCPARTICLE           , "", "", m_mc_particles, std::string("MCParticlesSkimmed"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_IsolatedLepton, std::string("IsolatedLeptonCollection"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_kt_R07, std::string("kt_R07"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_kt_R10, std::string("kt_R10"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_kt_R12, std::string("kt_R12"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_vlc_R06, std::string("vlc_R06"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_vlc_R08, std::string("vlc_R08"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_vlc_R10, std::string("vlc_R10"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_vlc_R08_g05, std::string("vlc_R08_g05"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_vlc_R08_g10, std::string("vlc_R08_g10"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "", "", m_pfos, std::string("LooseSelectedPandoraPFANewPFOs"));


    //output
    // register steering parameters: arbitrary name, arbitrary description, class-variable, default value
    registerProcessorParameter( "OutputFileName", "Name of the output file",
                               m_outfileName,
                               std::string("output.root"));
}




void NtupleMaker::init() {
    streamlog_out(MESSAGE) << "   init called  " << std::endl ;
    printParameters() ;
    _nRun = 0 ;
    _nEvt = 0 ;
    recoInputCollections.push_back(m_IsolatedLepton);
    recoInputCollections.push_back(m_kt_R07);
    recoInputCollections.push_back(m_kt_R10);
    recoInputCollections.push_back(m_kt_R12);
    recoInputCollections.push_back(m_vlc_R06);
    recoInputCollections.push_back(m_vlc_R08);
    recoInputCollections.push_back(m_vlc_R10);
    recoInputCollections.push_back(m_vlc_R08_g05);
    recoInputCollections.push_back(m_vlc_R08_g10);

    streamlog_out(MESSAGE) << "First event: initializing globals..." << std::endl ;
    ntupleFile=new TFile(m_outfileName.c_str(), "RECREATE");
    rawTree = new TTree("rawTree", "rawTree");
    int buffsize = 32000; //default buffer size 32KB

    rawTree->Branch("beam_e",&beam_e) ;
    rawTree->Branch("beam_m",&beam_m) ;

    rawTree->Branch("miss_pt",&miss_pt) ;
    rawTree->Branch("miss_theta",&miss_theta) ;
    rawTree->Branch("miss_phi",&miss_phi) ;
    rawTree->Branch("miss_e",&miss_e) ;

    rawTree->Branch("mc_qq_m",&mc_qq_m) ;
    rawTree->Branch("mc_ln_m",&mc_ln_m) ;
    rawTree->Branch("mc_n",&mc_n) ;
    rawTree->Branch("mc_gen_status","std::vector<int >",&mc_gen_status,buffsize,0) ;
    rawTree->Branch("mc_type","std::vector<int >",&mc_type,buffsize,0) ;
    rawTree->Branch("mc_pt","std::vector<double >",&mc_pt,buffsize,0) ;
    rawTree->Branch("mc_theta","std::vector<double >",&mc_theta,buffsize,0) ;
    rawTree->Branch("mc_phi","std::vector<double >",&mc_phi,buffsize,0) ;
    rawTree->Branch("mc_e","std::vector<double >",&mc_e,buffsize,0) ;
    rawTree->Branch("mc_charge","std::vector<double >",&mc_charge,buffsize,0) ;

    rawTree->Branch("lep_n",&lep_n) ;
    rawTree->Branch("lep_etot",&lep_etot) ;
    rawTree->Branch("lep_type","std::vector<int >",&lep_type,buffsize,0) ;
    rawTree->Branch("lep_pt","std::vector<double >",&lep_pt,buffsize,0) ;
    rawTree->Branch("lep_theta","std::vector<double >",&lep_theta,buffsize,0) ;
    rawTree->Branch("lep_phi","std::vector<double >",&lep_phi,buffsize,0) ;
    rawTree->Branch("lep_e","std::vector<double >",&lep_e,buffsize,0) ;
    rawTree->Branch("lep_charge","std::vector<double >",&lep_charge,buffsize,0) ;

    rawTree->Branch("jet_kt_R07_n",&jet_kt_R07_n) ;
    rawTree->Branch("jet_kt_R07_etot",&jet_kt_R07_etot) ;
    rawTree->Branch("jet_kt_R07_pt","std::vector<double >",&jet_kt_R07_pt,buffsize,0) ;
    rawTree->Branch("jet_kt_R07_theta","std::vector<double >",&jet_kt_R07_theta,buffsize,0) ;
    rawTree->Branch("jet_kt_R07_phi","std::vector<double >",&jet_kt_R07_phi,buffsize,0) ;
    rawTree->Branch("jet_kt_R07_e","std::vector<double >",&jet_kt_R07_e,buffsize,0) ;
    rawTree->Branch("jet_kt_R07_charge","std::vector<double >",&jet_kt_R07_charge,buffsize,0) ;

    rawTree->Branch("jet_kt_R10_n",&jet_kt_R10_n) ;
    rawTree->Branch("jet_kt_R10_etot",&jet_kt_R10_etot) ;
    rawTree->Branch("jet_kt_R10_pt","std::vector<double >",&jet_kt_R10_pt,buffsize,0) ;
    rawTree->Branch("jet_kt_R10_theta","std::vector<double >",&jet_kt_R10_theta,buffsize,0) ;
    rawTree->Branch("jet_kt_R10_phi","std::vector<double >",&jet_kt_R10_phi,buffsize,0) ;
    rawTree->Branch("jet_kt_R10_e","std::vector<double >",&jet_kt_R10_e,buffsize,0) ;
    rawTree->Branch("jet_kt_R10_charge","std::vector<double >",&jet_kt_R10_charge,buffsize,0) ;

    rawTree->Branch("jet_kt_R12_n",&jet_kt_R12_n) ;
    rawTree->Branch("jet_kt_R12_etot",&jet_kt_R12_etot) ;
    rawTree->Branch("jet_kt_R12_pt","std::vector<double >",&jet_kt_R12_pt,buffsize,0) ;
    rawTree->Branch("jet_kt_R12_theta","std::vector<double >",&jet_kt_R12_theta,buffsize,0) ;
    rawTree->Branch("jet_kt_R12_phi","std::vector<double >",&jet_kt_R12_phi,buffsize,0) ;
    rawTree->Branch("jet_kt_R12_e","std::vector<double >",&jet_kt_R12_e,buffsize,0) ;
    rawTree->Branch("jet_kt_R12_charge","std::vector<double >",&jet_kt_R12_charge,buffsize,0) ;

    rawTree->Branch("jet_vlc_R06_n",&jet_vlc_R06_n) ;
    rawTree->Branch("jet_vlc_R06_etot",&jet_vlc_R06_etot) ;
    rawTree->Branch("jet_vlc_R06_pt","std::vector<double >",&jet_vlc_R06_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_R06_theta","std::vector<double >",&jet_vlc_R06_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_R06_phi","std::vector<double >",&jet_vlc_R06_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_R06_e","std::vector<double >",&jet_vlc_R06_e,buffsize,0) ;
    rawTree->Branch("jet_vlc_R06_charge","std::vector<double >",&jet_vlc_R06_charge,buffsize,0) ;

    rawTree->Branch("jet_vlc_R08_n",&jet_vlc_R08_n) ;
    rawTree->Branch("jet_vlc_R08_etot",&jet_vlc_R08_etot) ;
    rawTree->Branch("jet_vlc_R08_pt","std::vector<double >",&jet_vlc_R08_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_theta","std::vector<double >",&jet_vlc_R08_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_phi","std::vector<double >",&jet_vlc_R08_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_e","std::vector<double >",&jet_vlc_R08_e,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_charge","std::vector<double >",&jet_vlc_R08_charge,buffsize,0) ;

    rawTree->Branch("jet_vlc_R10_n",&jet_vlc_R10_n) ;
    rawTree->Branch("jet_vlc_R10_etot",&jet_vlc_R10_etot) ;
    rawTree->Branch("jet_vlc_R10_pt","std::vector<double >",&jet_vlc_R10_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_R10_theta","std::vector<double >",&jet_vlc_R10_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_R10_phi","std::vector<double >",&jet_vlc_R10_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_R10_e","std::vector<double >",&jet_vlc_R10_e,buffsize,0) ;
    rawTree->Branch("jet_vlc_R10_charge","std::vector<double >",&jet_vlc_R10_charge,buffsize,0) ;

    rawTree->Branch("jet_vlc_R08_g05_n",&jet_vlc_R08_g05_n) ;
    rawTree->Branch("jet_vlc_R08_g05_etot",&jet_vlc_R08_g05_etot) ;
    rawTree->Branch("jet_vlc_R08_g05_pt","std::vector<double >",&jet_vlc_R08_g05_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_g05_theta","std::vector<double >",&jet_vlc_R08_g05_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_g05_phi","std::vector<double >",&jet_vlc_R08_g05_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_g05_e","std::vector<double >",&jet_vlc_R08_g05_e,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_g05_charge","std::vector<double >",&jet_vlc_R08_g05_charge,buffsize,0) ;

    rawTree->Branch("jet_vlc_R08_g10_n",&jet_vlc_R08_g10_n) ;
    rawTree->Branch("jet_vlc_R08_g10_etot",&jet_vlc_R08_g10_etot) ;
    rawTree->Branch("jet_vlc_R08_g10_pt","std::vector<double >",&jet_vlc_R08_g10_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_g10_theta","std::vector<double >",&jet_vlc_R08_g10_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_g10_phi","std::vector<double >",&jet_vlc_R08_g10_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_g10_e","std::vector<double >",&jet_vlc_R08_g10_e,buffsize,0) ;
    rawTree->Branch("jet_vlc_R08_g10_charge","std::vector<double >",&jet_vlc_R08_g10_charge,buffsize,0) ;

}


void NtupleMaker::processRunHeader( LCRunHeader* run) {
    run = run;
    _nRun++ ;
}



void NtupleMaker::processEvent( LCEvent * evt ) {

  streamlog_out(MESSAGE) << "   Processing event: " << evt->getEventNumber() << "   in run:  " << evt->getRunNumber() << std::endl ;
  if (isFirstEvent()){

  }

  clearEventVariables();

  streamlog_out(MESSAGE) <<"Analyzing collections:"<<std::endl;
  for (unsigned int iColl = 0; iColl < recoInputCollections.size(); ++iColl)
  {
    fillRecoParticles(recoInputCollections.at(iColl), evt);
  }
  fillMCInfo(evt);
  fillMissingEnergy(evt);
  streamlog_out(MESSAGE) << "Event "<<_nEvt<<": Fill tree..." << std::endl ;
  rawTree->Fill();

  _nEvt ++ ;
}


void NtupleMaker::end(){
  streamlog_out(MESSAGE) << "Write tree..." << std::endl ;
  ntupleFile->cd();
  rawTree->Write();
  // rawTree->Print("all");
  ntupleFile->Close();


  printParameters() ;
  streamlog_out(MESSAGE) << "NtupleMaker::end()  " << name() << " processed " << _nEvt << " events in " << _nRun << " runs "<< std::endl ;
}


void NtupleMaker::clearEventVariables(){
  fourvec.SetPxPyPzE(0,0,0,0);
  tmp0vec.SetPxPyPzE(0,0,0,0);
  tmp1vec.SetPxPyPzE(0,0,0,0);
  tmp2vec.SetPxPyPzE(0,0,0,0);
  tmp3vec.SetPxPyPzE(0,0,0,0);

  beam_e = 0;
  beam_m = 0;
  mc_qq_m = 0;
  mc_ln_m = 0;

  miss_pt = 0;
  miss_theta = 0;
  miss_phi = 0;
  miss_e = 0;

  mc_n = 0;
  mc_gen_status.clear();
  mc_type.clear();
  mc_pt.clear();
  mc_theta.clear();
  mc_phi.clear();
  mc_e.clear();
  mc_charge.clear();

  lep_n = 0;
  lep_etot = 0;
  lep_type.clear();
  lep_pt.clear();
  lep_theta.clear();
  lep_phi.clear();
  lep_e.clear();
  lep_charge.clear();

  jet_kt_R07_n = 0;
  jet_kt_R07_etot = 0;
  jet_kt_R07_pt.clear();
  jet_kt_R07_theta.clear();
  jet_kt_R07_phi.clear();
  jet_kt_R07_e.clear();
  jet_kt_R07_charge.clear();

  jet_kt_R10_n = 0;
  jet_kt_R10_etot = 0;
  jet_kt_R10_pt.clear();
  jet_kt_R10_theta.clear();
  jet_kt_R10_phi.clear();
  jet_kt_R10_e.clear();
  jet_kt_R10_charge.clear();

  jet_kt_R12_n = 0;
  jet_kt_R12_etot = 0;
  jet_kt_R12_pt.clear();
  jet_kt_R12_theta.clear();
  jet_kt_R12_phi.clear();
  jet_kt_R12_e.clear();
  jet_kt_R12_charge.clear();

  jet_vlc_R06_n = 0;
  jet_vlc_R06_etot = 0;
  jet_vlc_R06_pt.clear();
  jet_vlc_R06_theta.clear();
  jet_vlc_R06_phi.clear();
  jet_vlc_R06_e.clear();
  jet_vlc_R06_charge.clear();

  jet_vlc_R08_n = 0;
  jet_vlc_R08_etot = 0;
  jet_vlc_R08_pt.clear();
  jet_vlc_R08_theta.clear();
  jet_vlc_R08_phi.clear();
  jet_vlc_R08_e.clear();
  jet_vlc_R08_charge.clear();

  jet_vlc_R10_n = 0;
  jet_vlc_R10_etot = 0;
  jet_vlc_R10_pt.clear();
  jet_vlc_R10_theta.clear();
  jet_vlc_R10_phi.clear();
  jet_vlc_R10_e.clear();
  jet_vlc_R10_charge.clear();

  jet_vlc_R08_g05_n = 0;
  jet_vlc_R08_g05_etot = 0;
  jet_vlc_R08_g05_pt.clear();
  jet_vlc_R08_g05_theta.clear();
  jet_vlc_R08_g05_phi.clear();
  jet_vlc_R08_g05_e.clear();
  jet_vlc_R08_g05_charge.clear();

  jet_vlc_R08_g10_n = 0;
  jet_vlc_R08_g10_etot = 0;
  jet_vlc_R08_g10_pt.clear();
  jet_vlc_R08_g10_theta.clear();
  jet_vlc_R08_g10_phi.clear();
  jet_vlc_R08_g10_e.clear();
  jet_vlc_R08_g10_charge.clear();

}
void NtupleMaker::fillMissingEnergy(LCEvent * evt ){
  try {
    jet_vlc_R08_pt.at(0);
    jet_vlc_R08_pt.at(1);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << "Out of Range error: " << oor.what() << '\n';
    printf("Run particle reconstruction before missing energy reconstruction!\n");
    return;
  }
  try {
    mc_e.at(0);
    mc_e.at(1);
  }
  catch (const std::out_of_range& oor) {
    std::cerr << "Out of Range error: " << oor.what() << '\n';
    printf("Run MC reconstruction before missing energy reconstruction to determine s!\n");
    return;
  }
  fourvec.SetPxPyPzE(0, 0, 0, beam_e);
  // printf("\nMissing vec ini: pt %.3f theta %.3f phi %.3f e %.3f m %.3f\n", fourvec.Pt(), fourvec.Theta(), fourvec.Phi(), fourvec.E(), fourvec.M());
  for (unsigned int i=0; i<jet_vlc_R08_pt.size() ; i++){
    tmp0vec.SetPtEtaPhiE(jet_vlc_R08_pt.at(i), EtaFromTheta(jet_vlc_R08_theta.at(i)), jet_vlc_R08_phi.at(i), jet_vlc_R08_e.at(i));
    fourvec -= tmp0vec;
    // printf("Missing vec after jet %d: pt %.3f theta %.3f phi %.3f e %.3f m %.3f\n", i, fourvec.Pt(), fourvec.Theta(), fourvec.Phi(), fourvec.E(), fourvec.M());
  }
  for (unsigned int i=0; i<lep_pt.size() ; i++){
    tmp0vec.SetPtEtaPhiE(lep_pt.at(i), EtaFromTheta(lep_theta.at(i)), lep_phi.at(i), lep_e.at(i));
    fourvec -= tmp0vec;
    // printf("Missing vec after lep %d: pt %.3f theta %.3f phi %.3f e %.3f m %.3f\n", i, fourvec.Pt(), fourvec.Theta(), fourvec.Phi(), fourvec.E(), fourvec.M());
  }
  // remove mass
  fourvec.SetE(sqrt(pow(fourvec.Px(),2) + pow(fourvec.Py(),2) + pow(fourvec.Pz(),2)));
  // fill missing energy
  miss_pt = fourvec.Pt();
  miss_theta = fourvec.Theta();
  miss_phi = fourvec.Phi();
  miss_e = fourvec.E();
}
void NtupleMaker::fillMCInfo(LCEvent * evt ){
  std::string collName = m_mc_particles;
  LCCollection* thisCollection = 0 ;
  getCollection(thisCollection, collName, evt);
  if( thisCollection != NULL){
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<std::left << std::setw(MAX_COLL_NAME_WIDTH)<<collName <<": "<<thisCollection<< std::endl ;
    for(int i=0; i< thisCollection->getNumberOfElements() ; i++){
      if (i > 11){break; }
      /*
        (0, 1) electron and positron before ISR/FSR
        (2, 3) after
        (4, 5) the ISR/FSR photons
        (6, 7) first two quarks
        (8, 9) first two leptons, charged (8) and neutrino (9)
        (10, 11) the final ISR/FSR photons
      */
      EVENT::MCParticle* particle = dynamic_cast<EVENT::MCParticle*>(thisCollection->getElementAt(i));
      fourvec.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
      mc_gen_status.push_back(particle->getGeneratorStatus());
      mc_type.push_back(particle->getPDG());
      mc_pt.push_back(fourvec.Pt());
      mc_theta.push_back(fourvec.Theta());
      mc_phi.push_back(fourvec.Phi());
      mc_e.push_back(fourvec.E());
      mc_charge.push_back(particle->getCharge());
      if (i == 2) tmp0vec = fourvec;
      if (i == 3) {
        tmp1vec = fourvec;
        beam_e = (tmp0vec + tmp1vec).E();
        beam_m = (tmp0vec + tmp1vec).M();
      }
      if (i == 6) tmp0vec = fourvec;
      if (i == 7) {
        tmp1vec = fourvec;
        mc_qq_m = (tmp0vec + tmp1vec).M();
      }
      if (i == 8) tmp2vec = fourvec;
      if (i == 9) {
        tmp3vec = fourvec;
        mc_ln_m = (tmp2vec + tmp3vec).M();
      }
    }
    mc_n = thisCollection->getNumberOfElements();
  }else{
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": Warning: collection " << collName <<" not available. Skip!"<<std::endl;
  }
}
void NtupleMaker::fillRecoParticles(std::string collName, LCEvent * evt ){
  LCCollection* thisCollection = 0 ;
  getCollection(thisCollection, collName, evt);
  if( thisCollection != NULL){
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<std::left << std::setw(MAX_COLL_NAME_WIDTH)<<collName <<": "<<thisCollection<< std::endl ;
    std::vector<int> index = orderByPt(thisCollection);
    for(int i=0; i< thisCollection->getNumberOfElements() ; i++){
      ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>( thisCollection->getElementAt( index[i] ) ) ;
      fillVectors(collName, particle);
    }
  }else{
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": Warning: collection " << collName <<" not available. Skip!"<<std::endl;
  }
}
std::vector<int> NtupleMaker::orderByPt(LCCollection* thisCollection){
  std::vector<int> index;
  std::vector<double> ptVec;
  for(int i=0; i< thisCollection->getNumberOfElements() ; i++){
    ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>( thisCollection->getElementAt( i ) ) ;
    fourvec.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
    ptVec.push_back(fourvec.Pt());
    index.push_back(i);
  }
  // now get index depending on pt order
  std::sort(index.begin(), index.end(),
    [&](const int& a, const int& b) { return (ptVec[a] > ptVec[b]); }
  );
  return index;
}
void NtupleMaker::fillVectors(std::string collName, ReconstructedParticle* particle){
  fourvec.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
  if (collName == m_IsolatedLepton){
    if (abs(particle->getType()) < 20){
      ++lep_n;
      lep_etot+=fourvec.E();
      lep_type.push_back(particle->getType());
      lep_pt.push_back(fourvec.Pt());
      lep_theta.push_back(fourvec.Theta());
      lep_phi.push_back(fourvec.Phi());
      lep_e.push_back(fourvec.E());
      lep_charge.push_back(particle->getCharge());
    }
  }
  else if (collName == m_kt_R07){
    ++jet_kt_R07_n;
    jet_kt_R07_etot+=fourvec.E();
    jet_kt_R07_pt.push_back(fourvec.Pt());
    jet_kt_R07_theta.push_back(fourvec.Theta());
    jet_kt_R07_phi.push_back(fourvec.Phi());
    jet_kt_R07_e.push_back(fourvec.E());
    jet_kt_R07_charge.push_back(particle->getCharge());
  }
  else if (collName == m_kt_R10){
    ++jet_kt_R10_n;
    jet_kt_R10_etot+=fourvec.E();
    jet_kt_R10_pt.push_back(fourvec.Pt());
    jet_kt_R10_theta.push_back(fourvec.Theta());
    jet_kt_R10_phi.push_back(fourvec.Phi());
    jet_kt_R10_e.push_back(fourvec.E());
    jet_kt_R10_charge.push_back(particle->getCharge());
  }
  else if (collName == m_kt_R12){
    ++jet_kt_R12_n;
    jet_kt_R12_etot+=fourvec.E();
    jet_kt_R12_pt.push_back(fourvec.Pt());
    jet_kt_R12_theta.push_back(fourvec.Theta());
    jet_kt_R12_phi.push_back(fourvec.Phi());
    jet_kt_R12_e.push_back(fourvec.E());
    jet_kt_R12_charge.push_back(particle->getCharge());
  }
  else if (collName == m_vlc_R06){
    ++jet_vlc_R06_n;
    jet_vlc_R06_etot+=fourvec.E();
    jet_vlc_R06_pt.push_back(fourvec.Pt());
    jet_vlc_R06_theta.push_back(fourvec.Theta());
    jet_vlc_R06_phi.push_back(fourvec.Phi());
    jet_vlc_R06_e.push_back(fourvec.E());
    jet_vlc_R06_charge.push_back(particle->getCharge());
  }
  else if (collName == m_vlc_R08){
    ++jet_vlc_R08_n;
    jet_vlc_R08_etot+=fourvec.E();
    jet_vlc_R08_pt.push_back(fourvec.Pt());
    jet_vlc_R08_theta.push_back(fourvec.Theta());
    jet_vlc_R08_phi.push_back(fourvec.Phi());
    jet_vlc_R08_e.push_back(fourvec.E());
    jet_vlc_R08_charge.push_back(particle->getCharge());
  }
  else if (collName == m_vlc_R10){
    ++jet_vlc_R10_n;
    jet_vlc_R10_etot+=fourvec.E();
    jet_vlc_R10_pt.push_back(fourvec.Pt());
    jet_vlc_R10_theta.push_back(fourvec.Theta());
    jet_vlc_R10_phi.push_back(fourvec.Phi());
    jet_vlc_R10_e.push_back(fourvec.E());
    jet_vlc_R10_charge.push_back(particle->getCharge());
  }
  else if (collName == m_vlc_R08_g05){
    ++jet_vlc_R08_g05_n;
    jet_vlc_R08_g05_etot+=fourvec.E();
    jet_vlc_R08_g05_pt.push_back(fourvec.Pt());
    jet_vlc_R08_g05_theta.push_back(fourvec.Theta());
    jet_vlc_R08_g05_phi.push_back(fourvec.Phi());
    jet_vlc_R08_g05_e.push_back(fourvec.E());
    jet_vlc_R08_g05_charge.push_back(particle->getCharge());
  }
  else if (collName == m_vlc_R08_g10){
    ++jet_vlc_R08_g10_n;
    jet_vlc_R08_g10_etot+=fourvec.E();
    jet_vlc_R08_g10_pt.push_back(fourvec.Pt());
    jet_vlc_R08_g10_theta.push_back(fourvec.Theta());
    jet_vlc_R08_g10_phi.push_back(fourvec.Phi());
    jet_vlc_R08_g10_e.push_back(fourvec.E());
    jet_vlc_R08_g10_charge.push_back(particle->getCharge());
  }
}
void NtupleMaker::getCollection(LCCollection* &collection, std::string collectionName, LCEvent* evt){
  try{
    collection = evt->getCollection( collectionName ) ;
  }
  catch(DataNotAvailableException &e){
    std::cout<<"- cannot get collections !!"<<std::endl;
    streamlog_out(DEBUG4) << "Collection " << collectionName.c_str() << " is unavailable" << std::endl;
    return;
  }
  return;
}
