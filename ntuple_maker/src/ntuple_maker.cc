//written by Andreas Alexander Maier July 2017
//andreas.alexander.maier@cern.ch
#include "ntuple_maker.h"


const int MAX_COLL_NAME_WIDTH = 20;


double DeltaR(double eta0,double phi0, double eta1, double phi1){
   Double_t deta = eta0-eta1;
   Double_t dphi = TVector2::Phi_mpi_pi(phi0-phi1);
   return TMath::Sqrt( deta*deta+dphi*dphi );
}
double EtaFromTheta(double theta){
  return -TMath::Log(TMath::Tan(theta/2));
}



ntuple_maker antuple_maker ;


ntuple_maker::ntuple_maker() : Processor("ntuple_maker") {
    // modify processor description
    _description = "ntuple_maker does whatever it does ..." ;

    // input
    // register input parameters: collection type, name, description, class-variable, default value
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A lepton collection", 
                             "Isolated Lepton Collection Name", m_IsolatedLepton, 
                             std::string("IsolatedLeptonCollection"));

    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "kt_4jets_radius07 Collection Name", m_kt_4j_R07, 
                             std::string("kt_4jets_radius07"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "kt_4jets_radius10 Collection Name", m_kt_4j_R10, 
                             std::string("kt_4jets_radius10"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "kt_4jets_radius12 Collection Name", m_kt_4j_R12, 
                             std::string("kt_4jets_radius12"));

    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                         "vlc_4jets_radius06 Collection Name", m_vlc_4j_R06, 
                         std::string("vlc_4jets_radius06"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                         "vlc_4jets_radius08 Collection Name", m_vlc_4j_R08, 
                         std::string("vlc_4jets_radius08"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                         "vlc_4jets_radius10 Collection Name", m_vlc_4j_R10, 
                         std::string("vlc_4jets_radius10"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                         "vlc_4jets_radius08_gamma05 Collection Name", m_vlc_4j_R08_g05, 
                         std::string("vlc_4jets_radius08_gamma05"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                         "vlc_4jets_radius08_gamma10 Collection Name", m_vlc_4j_R08_g10, 
                         std::string("vlc_4jets_radius08_gamma10"));


    //output
    // register steering parameters: name, description, class-variable, default value
    registerProcessorParameter( "OutputFileName", "Name of the output file",
                               m_outfileName,
                               std::string("output.root"));
}




void ntuple_maker::init() {
    streamlog_out(MESSAGE) << "   init called  " << std::endl ;
    printParameters() ;
    _nRun = 0 ;
    _nEvt = 0 ;
    inputCollections.push_back(m_IsolatedLepton);
    inputCollections.push_back(m_kt_4j_R07);
    inputCollections.push_back(m_kt_4j_R10);
    inputCollections.push_back(m_kt_4j_R12);
    inputCollections.push_back(m_vlc_4j_R06);
    inputCollections.push_back(m_vlc_4j_R08);
    inputCollections.push_back(m_vlc_4j_R10);
    inputCollections.push_back(m_vlc_4j_R08_g05);
    inputCollections.push_back(m_vlc_4j_R08_g10);

    streamlog_out(MESSAGE) << "First event: initializing globals..." << std::endl ;
    ntupleFile=new TFile(m_outfileName.c_str(), "RECREATE");
    rawTree = new TTree("rawTree", "rawTree");
    int buffsize = 32000; //default buffer size 32KB
    rawTree->Branch("lep_n",&lep_n) ;
    rawTree->Branch("lep_etot",&lep_etot) ;
    rawTree->Branch("lep_type","std::vector<int >",&lep_type,buffsize,0) ;
    rawTree->Branch("lep_pt","std::vector<double >",&lep_pt,buffsize,0) ;
    rawTree->Branch("lep_theta","std::vector<double >",&lep_theta,buffsize,0) ;
    rawTree->Branch("lep_phi","std::vector<double >",&lep_phi,buffsize,0) ;
    rawTree->Branch("lep_e","std::vector<double >",&lep_e,buffsize,0) ;


    rawTree->Branch("jet_kt_4j_R07_n",&jet_kt_4j_R07_n) ;
    rawTree->Branch("jet_kt_4j_R07_etot",&jet_kt_4j_R07_etot) ;
    rawTree->Branch("jet_kt_4j_R07_pt","std::vector<double >",&jet_kt_4j_R07_pt,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R07_theta","std::vector<double >",&jet_kt_4j_R07_theta,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R07_phi","std::vector<double >",&jet_kt_4j_R07_phi,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R07_e","std::vector<double >",&jet_kt_4j_R07_e,buffsize,0) ;

    rawTree->Branch("jet_kt_4j_R10_n",&jet_kt_4j_R10_n) ;
    rawTree->Branch("jet_kt_4j_R10_etot",&jet_kt_4j_R10_etot) ;
    rawTree->Branch("jet_kt_4j_R10_pt","std::vector<double >",&jet_kt_4j_R10_pt,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R10_theta","std::vector<double >",&jet_kt_4j_R10_theta,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R10_phi","std::vector<double >",&jet_kt_4j_R10_phi,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R10_e","std::vector<double >",&jet_kt_4j_R10_e,buffsize,0) ;

    rawTree->Branch("jet_kt_4j_R12_n",&jet_kt_4j_R12_n) ;
    rawTree->Branch("jet_kt_4j_R12_etot",&jet_kt_4j_R12_etot) ;
    rawTree->Branch("jet_kt_4j_R12_pt","std::vector<double >",&jet_kt_4j_R12_pt,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R12_theta","std::vector<double >",&jet_kt_4j_R12_theta,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R12_phi","std::vector<double >",&jet_kt_4j_R12_phi,buffsize,0) ;
    rawTree->Branch("jet_kt_4j_R12_e","std::vector<double >",&jet_kt_4j_R12_e,buffsize,0) ;


    rawTree->Branch("jet_vlc_4j_R06_n",&jet_vlc_4j_R06_n) ;
    rawTree->Branch("jet_vlc_4j_R06_etot",&jet_vlc_4j_R06_etot) ;
    rawTree->Branch("jet_vlc_4j_R06_pt","std::vector<double >",&jet_vlc_4j_R06_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R06_theta","std::vector<double >",&jet_vlc_4j_R06_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R06_phi","std::vector<double >",&jet_vlc_4j_R06_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R06_e","std::vector<double >",&jet_vlc_4j_R06_e,buffsize,0) ;

    rawTree->Branch("jet_vlc_4j_R08_n",&jet_vlc_4j_R08_n) ;
    rawTree->Branch("jet_vlc_4j_R08_etot",&jet_vlc_4j_R08_etot) ;
    rawTree->Branch("jet_vlc_4j_R08_pt","std::vector<double >",&jet_vlc_4j_R08_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_theta","std::vector<double >",&jet_vlc_4j_R08_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_phi","std::vector<double >",&jet_vlc_4j_R08_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_e","std::vector<double >",&jet_vlc_4j_R08_e,buffsize,0) ;

    rawTree->Branch("jet_vlc_4j_R10_n",&jet_vlc_4j_R10_n) ;
    rawTree->Branch("jet_vlc_4j_R10_etot",&jet_vlc_4j_R10_etot) ;
    rawTree->Branch("jet_vlc_4j_R10_pt","std::vector<double >",&jet_vlc_4j_R10_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R10_theta","std::vector<double >",&jet_vlc_4j_R10_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R10_phi","std::vector<double >",&jet_vlc_4j_R10_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R10_e","std::vector<double >",&jet_vlc_4j_R10_e,buffsize,0) ;

    rawTree->Branch("jet_vlc_4j_R08_g05_n",&jet_vlc_4j_R08_g05_n) ;
    rawTree->Branch("jet_vlc_4j_R08_g05_etot",&jet_vlc_4j_R08_g05_etot) ;
    rawTree->Branch("jet_vlc_4j_R08_g05_pt","std::vector<double >",&jet_vlc_4j_R08_g05_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_g05_theta","std::vector<double >",&jet_vlc_4j_R08_g05_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_g05_phi","std::vector<double >",&jet_vlc_4j_R08_g05_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_g05_e","std::vector<double >",&jet_vlc_4j_R08_g05_e,buffsize,0) ;

    rawTree->Branch("jet_vlc_4j_R08_g10_n",&jet_vlc_4j_R08_g10_n) ;
    rawTree->Branch("jet_vlc_4j_R08_g10_etot",&jet_vlc_4j_R08_g10_etot) ;
    rawTree->Branch("jet_vlc_4j_R08_g10_pt","std::vector<double >",&jet_vlc_4j_R08_g10_pt,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_g10_theta","std::vector<double >",&jet_vlc_4j_R08_g10_theta,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_g10_phi","std::vector<double >",&jet_vlc_4j_R08_g10_phi,buffsize,0) ;
    rawTree->Branch("jet_vlc_4j_R08_g10_e","std::vector<double >",&jet_vlc_4j_R08_g10_e,buffsize,0) ;

}


void ntuple_maker::processRunHeader( LCRunHeader* run) {
    run = run;
    _nRun++ ;
}



void ntuple_maker::processEvent( LCEvent * evt ) {

  streamlog_out(MESSAGE) << "   Processing event: " << evt->getEventNumber() << "   in run:  " << evt->getRunNumber() << std::endl ;
  if (isFirstEvent()){

  }

  clearEventVariables();

  streamlog_out(MESSAGE) <<"Analyzing collections:"<<std::endl;
  for (unsigned int iColl = 0; iColl < inputCollections.size(); ++iColl)
  {
    fillRecoParticleEventVariables(inputCollections.at(iColl), evt);
  }
  streamlog_out(MESSAGE) << "Event "<<_nEvt<<": Fill tree..." << std::endl ;
  rawTree->Fill();

  _nEvt ++ ;
}


void ntuple_maker::end(){
  streamlog_out(MESSAGE) << "Write tree..." << std::endl ;
  ntupleFile->cd();
  rawTree->Write();
  // rawTree->Print("all");
  ntupleFile->Close();


  printParameters() ;
  streamlog_out(MESSAGE) << "ntuple_maker::end()  " << name() << " processed " << _nEvt << " events in " << _nRun << " runs "<< std::endl ;
}


void ntuple_maker::clearEventVariables(){
  fourvec.SetPxPyPzE(0,0,0,0);

  lep_n = 0;
  lep_etot = 0;
  lep_type.clear();
  lep_pt.clear();
  lep_theta.clear();
  lep_phi.clear();
  lep_e.clear();
  

  jet_kt_4j_R07_n = 0;
  jet_kt_4j_R07_etot = 0;
  jet_kt_4j_R07_pt.clear();
  jet_kt_4j_R07_theta.clear();
  jet_kt_4j_R07_phi.clear();
  jet_kt_4j_R07_e.clear();

  jet_kt_4j_R10_n = 0;
  jet_kt_4j_R10_etot = 0;
  jet_kt_4j_R10_pt.clear();
  jet_kt_4j_R10_theta.clear();
  jet_kt_4j_R10_phi.clear();
  jet_kt_4j_R10_e.clear();

  jet_kt_4j_R12_n = 0;
  jet_kt_4j_R12_etot = 0;
  jet_kt_4j_R12_pt.clear();
  jet_kt_4j_R12_theta.clear();
  jet_kt_4j_R12_phi.clear();
  jet_kt_4j_R12_e.clear();


  jet_vlc_4j_R06_n = 0;
  jet_vlc_4j_R06_etot = 0;
  jet_vlc_4j_R06_pt.clear();
  jet_vlc_4j_R06_theta.clear();
  jet_vlc_4j_R06_phi.clear();
  jet_vlc_4j_R06_e.clear();

  jet_vlc_4j_R08_n = 0;
  jet_vlc_4j_R08_etot = 0;
  jet_vlc_4j_R08_pt.clear();
  jet_vlc_4j_R08_theta.clear();
  jet_vlc_4j_R08_phi.clear();
  jet_vlc_4j_R08_e.clear();

  jet_vlc_4j_R10_n = 0;
  jet_vlc_4j_R10_etot = 0;
  jet_vlc_4j_R10_pt.clear();
  jet_vlc_4j_R10_theta.clear();
  jet_vlc_4j_R10_phi.clear();
  jet_vlc_4j_R10_e.clear();

  jet_vlc_4j_R08_g05_n = 0;
  jet_vlc_4j_R08_g05_etot = 0;
  jet_vlc_4j_R08_g05_pt.clear();
  jet_vlc_4j_R08_g05_theta.clear();
  jet_vlc_4j_R08_g05_phi.clear();
  jet_vlc_4j_R08_g05_e.clear();

  jet_vlc_4j_R08_g10_n = 0;
  jet_vlc_4j_R08_g10_etot = 0;
  jet_vlc_4j_R08_g10_pt.clear();
  jet_vlc_4j_R08_g10_theta.clear();
  jet_vlc_4j_R08_g10_phi.clear();
  jet_vlc_4j_R08_g10_e.clear();

}
void ntuple_maker::fillRecoParticleEventVariables(std::string collName, LCEvent * evt ){
  LCCollection* thisCollection = 0 ;
  getCollection(thisCollection, collName, evt);

  if( thisCollection != NULL){
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<std::left << std::setw(MAX_COLL_NAME_WIDTH)<<collName <<": "<<thisCollection<< std::endl ;
    for(int i=0; i< thisCollection->getNumberOfElements() ; i++){
      ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>( thisCollection->getElementAt( i ) ) ;
      fillVectors(collName, particle);
    }
  }else{
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": Warning: collection " << collName <<" not available. Skip!"<<std::endl;
  }
}
void ntuple_maker::fillVectors(std::string collName, ReconstructedParticle* particle){
  fourvec.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
  
  if (collName == m_IsolatedLepton){
    if (particle->getType() < 20){
      ++lep_n;
      lep_etot+=fourvec.E();
      lep_type.push_back(particle->getType());
      lep_pt.push_back(fourvec.Pt());
      lep_theta.push_back(fourvec.Theta());
      lep_phi.push_back(fourvec.Phi());
      lep_e.push_back(fourvec.E());
    }
  }
  else if (collName == m_kt_4j_R07){
    ++jet_kt_4j_R07_n;
    jet_kt_4j_R07_etot+=fourvec.E();
    jet_kt_4j_R07_pt.push_back(fourvec.Pt());
    jet_kt_4j_R07_theta.push_back(fourvec.Theta());
    jet_kt_4j_R07_phi.push_back(fourvec.Phi());
    jet_kt_4j_R07_e.push_back(fourvec.E());
  }
  else if (collName == m_kt_4j_R10){
    ++jet_kt_4j_R10_n;
    jet_kt_4j_R10_etot+=fourvec.E();
    jet_kt_4j_R10_pt.push_back(fourvec.Pt());
    jet_kt_4j_R10_theta.push_back(fourvec.Theta());
    jet_kt_4j_R10_phi.push_back(fourvec.Phi());
    jet_kt_4j_R10_e.push_back(fourvec.E());
  }
  else if (collName == m_kt_4j_R12){
    ++jet_kt_4j_R12_n;
    jet_kt_4j_R12_etot+=fourvec.E();
    jet_kt_4j_R12_pt.push_back(fourvec.Pt());
    jet_kt_4j_R12_theta.push_back(fourvec.Theta());
    jet_kt_4j_R12_phi.push_back(fourvec.Phi());
    jet_kt_4j_R12_e.push_back(fourvec.E());
  }
  else if (collName == m_vlc_4j_R06){
    ++jet_vlc_4j_R06_n;
    jet_vlc_4j_R06_etot+=fourvec.E();
    jet_vlc_4j_R06_pt.push_back(fourvec.Pt());
    jet_vlc_4j_R06_theta.push_back(fourvec.Theta());
    jet_vlc_4j_R06_phi.push_back(fourvec.Phi());
    jet_vlc_4j_R06_e.push_back(fourvec.E());
  }
  else if (collName == m_vlc_4j_R08){
    ++jet_vlc_4j_R08_n;
    jet_vlc_4j_R08_etot+=fourvec.E();
    jet_vlc_4j_R08_pt.push_back(fourvec.Pt());
    jet_vlc_4j_R08_theta.push_back(fourvec.Theta());
    jet_vlc_4j_R08_phi.push_back(fourvec.Phi());
    jet_vlc_4j_R08_e.push_back(fourvec.E());
  }
  else if (collName == m_vlc_4j_R10){
    ++jet_vlc_4j_R10_n;
    jet_vlc_4j_R10_etot+=fourvec.E();
    jet_vlc_4j_R10_pt.push_back(fourvec.Pt());
    jet_vlc_4j_R10_theta.push_back(fourvec.Theta());
    jet_vlc_4j_R10_phi.push_back(fourvec.Phi());
    jet_vlc_4j_R10_e.push_back(fourvec.E());
  }
  else if (collName == m_vlc_4j_R08_g05){
    ++jet_vlc_4j_R08_g05_n;
    jet_vlc_4j_R08_g05_etot+=fourvec.E();
    jet_vlc_4j_R08_g05_pt.push_back(fourvec.Pt());
    jet_vlc_4j_R08_g05_theta.push_back(fourvec.Theta());
    jet_vlc_4j_R08_g05_phi.push_back(fourvec.Phi());
    jet_vlc_4j_R08_g05_e.push_back(fourvec.E());
  }
  else if (collName == m_vlc_4j_R08_g10){
    ++jet_vlc_4j_R08_g10_n;
    jet_vlc_4j_R08_g10_etot+=fourvec.E();
    jet_vlc_4j_R08_g10_pt.push_back(fourvec.Pt());
    jet_vlc_4j_R08_g10_theta.push_back(fourvec.Theta());
    jet_vlc_4j_R08_g10_phi.push_back(fourvec.Phi());
    jet_vlc_4j_R08_g10_e.push_back(fourvec.E());
  }
}
void ntuple_maker::getCollection(LCCollection* &collection, std::string collectionName, LCEvent* evt){
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