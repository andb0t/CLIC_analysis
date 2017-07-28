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
                             "Isolated Lepton Collection Name", m_IsolatedLepton_collection, 
                             std::string("IsolatedLeptonCollection"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "durham_4jets Collection Name", m_durham_4jets_collection, 
                             std::string("durham_4jets"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "kt_4jets_radius05 Collection Name", m_kt_4jets_radius05_collection, 
                             std::string("kt_4jets_radius05"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "kt_4jets_radius07 Collection Name", m_kt_4jets_radius07_collection, 
                             std::string("kt_4jets_radius07"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "kt_4jets_radius10 Collection Name", m_kt_4jets_radius10_collection, 
                             std::string("kt_4jets_radius10"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "kt_4jets_radius12 Collection Name", m_kt_4jets_radius12_collection, 
                             std::string("kt_4jets_radius12"));
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, "A jet collection", 
                             "kt_4jets_radius15 Collection Name", m_kt_4jets_radius15_collection, 
                             std::string("kt_4jets_radius15"));

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
    inputCollections.push_back(m_IsolatedLepton_collection);
    inputCollections.push_back(m_durham_4jets_collection);
    inputCollections.push_back(m_kt_4jets_radius05_collection);
    inputCollections.push_back(m_kt_4jets_radius07_collection);
    inputCollections.push_back(m_kt_4jets_radius10_collection);
    inputCollections.push_back(m_kt_4jets_radius12_collection);
    inputCollections.push_back(m_kt_4jets_radius15_collection);

    streamlog_out(MESSAGE) << "First event: initializing globals..." << std::endl ;
    ntupleFile=new TFile(m_outfileName.c_str(), "RECREATE");
    rawTree = new TTree("rawTree", "rawTree");
    int buffsize = 32000; //default buffer size 32KB
    rawTree->Branch("lep_n",&lep_n) ;
    rawTree->Branch("lep_etot",&lep_etot) ;
    rawTree->Branch("lep_pt","std::vector<double >",&lep_pt,buffsize,0) ;
    rawTree->Branch("lep_theta","std::vector<double >",&lep_theta,buffsize,0) ;
    rawTree->Branch("lep_phi","std::vector<double >",&lep_phi,buffsize,0) ;
    rawTree->Branch("lep_e","std::vector<double >",&lep_e,buffsize,0) ;

    rawTree->Branch("jet_DH_n",&jet_DH_n) ;
    rawTree->Branch("jet_DH_n",&jet_DH_etot) ;
    rawTree->Branch("jet_DH_pt","std::vector<double >",&jet_DH_pt,buffsize,0) ;
    rawTree->Branch("jet_DH_theta","std::vector<double >",&jet_DH_theta,buffsize,0) ;
    rawTree->Branch("jet_DH_phi","std::vector<double >",&jet_DH_phi,buffsize,0) ;
    rawTree->Branch("jet_DH_e","std::vector<double >",&jet_DH_e,buffsize,0) ;

    rawTree->Branch("jet_KT_R05_n",&jet_KT_R05_n) ;
    rawTree->Branch("jet_KT_R05_n",&jet_KT_R05_etot) ;
    rawTree->Branch("jet_KT_R05_pt","std::vector<double >",&jet_KT_R05_pt,buffsize,0) ;
    rawTree->Branch("jet_KT_R05_theta","std::vector<double >",&jet_KT_R05_theta,buffsize,0) ;
    rawTree->Branch("jet_KT_R05_phi","std::vector<double >",&jet_KT_R05_phi,buffsize,0) ;
    rawTree->Branch("jet_KT_R05_e","std::vector<double >",&jet_KT_R05_e,buffsize,0) ;

    rawTree->Branch("jet_KT_R07_n",&jet_KT_R07_n) ;
    rawTree->Branch("jet_KT_R07_n",&jet_KT_R07_etot) ;
    rawTree->Branch("jet_KT_R07_pt","std::vector<double >",&jet_KT_R07_pt,buffsize,0) ;
    rawTree->Branch("jet_KT_R07_theta","std::vector<double >",&jet_KT_R07_theta,buffsize,0) ;
    rawTree->Branch("jet_KT_R07_phi","std::vector<double >",&jet_KT_R07_phi,buffsize,0) ;
    rawTree->Branch("jet_KT_R07_e","std::vector<double >",&jet_KT_R07_e,buffsize,0) ;

    rawTree->Branch("jet_KT_R10_n",&jet_KT_R10_n) ;
    rawTree->Branch("jet_KT_R10_n",&jet_KT_R10_etot) ;
    rawTree->Branch("jet_KT_R10_pt","std::vector<double >",&jet_KT_R10_pt,buffsize,0) ;
    rawTree->Branch("jet_KT_R10_theta","std::vector<double >",&jet_KT_R10_theta,buffsize,0) ;
    rawTree->Branch("jet_KT_R10_phi","std::vector<double >",&jet_KT_R10_phi,buffsize,0) ;
    rawTree->Branch("jet_KT_R10_e","std::vector<double >",&jet_KT_R10_e,buffsize,0) ;

    rawTree->Branch("jet_KT_R12_n",&jet_KT_R12_n) ;
    rawTree->Branch("jet_KT_R12_n",&jet_KT_R12_etot) ;
    rawTree->Branch("jet_KT_R12_pt","std::vector<double >",&jet_KT_R12_pt,buffsize,0) ;
    rawTree->Branch("jet_KT_R12_theta","std::vector<double >",&jet_KT_R12_theta,buffsize,0) ;
    rawTree->Branch("jet_KT_R12_phi","std::vector<double >",&jet_KT_R12_phi,buffsize,0) ;
    rawTree->Branch("jet_KT_R12_e","std::vector<double >",&jet_KT_R12_e,buffsize,0) ;

    rawTree->Branch("jet_KT_R15_n",&jet_KT_R15_n) ;
    rawTree->Branch("jet_KT_R15_n",&jet_KT_R15_etot) ;
    rawTree->Branch("jet_KT_R15_pt","std::vector<double >",&jet_KT_R15_pt,buffsize,0) ;
    rawTree->Branch("jet_KT_R15_theta","std::vector<double >",&jet_KT_R15_theta,buffsize,0) ;
    rawTree->Branch("jet_KT_R15_phi","std::vector<double >",&jet_KT_R15_phi,buffsize,0) ;
    rawTree->Branch("jet_KT_R15_e","std::vector<double >",&jet_KT_R15_e,buffsize,0) ;
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

  jet_DH_n = 0;
  jet_DH_etot = 0;
  jet_DH_pt.clear();
  jet_DH_theta.clear();
  jet_DH_phi.clear();
  jet_DH_e.clear();

  jet_KT_R05_n = 0;
  jet_KT_R05_etot = 0;
  jet_KT_R05_pt.clear();
  jet_KT_R05_theta.clear();
  jet_KT_R05_phi.clear();
  jet_KT_R05_e.clear();

  jet_KT_R07_n = 0;
  jet_KT_R07_etot = 0;
  jet_KT_R07_pt.clear();
  jet_KT_R07_theta.clear();
  jet_KT_R07_phi.clear();
  jet_KT_R07_e.clear();

  jet_KT_R10_n = 0;
  jet_KT_R10_etot = 0;
  jet_KT_R10_pt.clear();
  jet_KT_R10_theta.clear();
  jet_KT_R10_phi.clear();
  jet_KT_R10_e.clear();

  jet_KT_R12_n = 0;
  jet_KT_R12_etot = 0;
  jet_KT_R12_pt.clear();
  jet_KT_R12_theta.clear();
  jet_KT_R12_phi.clear();
  jet_KT_R12_e.clear();

  jet_KT_R15_n = 0;
  jet_KT_R15_etot = 0;
  jet_KT_R15_pt.clear();
  jet_KT_R15_theta.clear();
  jet_KT_R15_phi.clear();
  jet_KT_R15_e.clear();

  lep_n = 0;
  lep_etot = 0;
  lep_pt.clear();
  lep_theta.clear();
  lep_phi.clear();
  lep_e.clear();
  
  return;
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
  
  if (collName == m_IsolatedLepton_collection){
    ++lep_n;
    lep_etot+=fourvec.E();
    lep_pt.push_back(fourvec.Pt());
    lep_theta.push_back(fourvec.Theta());
    lep_phi.push_back(fourvec.Phi());
    lep_e.push_back(fourvec.E());
  }
  else if (collName == m_durham_4jets_collection){
    ++jet_DH_n;
    jet_DH_etot+=fourvec.E();
    jet_DH_pt.push_back(fourvec.Pt());
    jet_DH_theta.push_back(fourvec.Theta());
    jet_DH_phi.push_back(fourvec.Phi());
    jet_DH_e.push_back(fourvec.E());
  }
  else if (collName == m_kt_4jets_radius05_collection){
    ++jet_KT_R05_n;
    jet_KT_R05_etot+=fourvec.E();
    jet_KT_R05_pt.push_back(fourvec.Pt());
    jet_KT_R05_theta.push_back(fourvec.Theta());
    jet_KT_R05_phi.push_back(fourvec.Phi());
    jet_KT_R05_e.push_back(fourvec.E());
  }
  else if (collName == m_kt_4jets_radius07_collection){
    ++jet_KT_R07_n;
    jet_KT_R07_etot+=fourvec.E();
    jet_KT_R07_pt.push_back(fourvec.Pt());
    jet_KT_R07_theta.push_back(fourvec.Theta());
    jet_KT_R07_phi.push_back(fourvec.Phi());
    jet_KT_R07_e.push_back(fourvec.E());
  }
  else if (collName == m_kt_4jets_radius10_collection){
    ++jet_KT_R10_n;
    jet_KT_R10_etot+=fourvec.E();
    jet_KT_R10_pt.push_back(fourvec.Pt());
    jet_KT_R10_theta.push_back(fourvec.Theta());
    jet_KT_R10_phi.push_back(fourvec.Phi());
    jet_KT_R10_e.push_back(fourvec.E());
  }
  else if (collName == m_kt_4jets_radius12_collection){
    ++jet_KT_R12_n;
    jet_KT_R12_etot+=fourvec.E();
    jet_KT_R12_pt.push_back(fourvec.Pt());
    jet_KT_R12_theta.push_back(fourvec.Theta());
    jet_KT_R12_phi.push_back(fourvec.Phi());
    jet_KT_R12_e.push_back(fourvec.E());
  }
  else if (collName == m_kt_4jets_radius15_collection){
    ++jet_KT_R15_n;
    jet_KT_R15_etot+=fourvec.E();
    jet_KT_R15_pt.push_back(fourvec.Pt());
    jet_KT_R15_theta.push_back(fourvec.Theta());
    jet_KT_R15_phi.push_back(fourvec.Phi());
    jet_KT_R15_e.push_back(fourvec.E());
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