//written by Andreas Alexander Maier July 2017
//andreas.alexander.maier@cern.ch
#include "ntuple_maker.h"


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
}


void ntuple_maker::processRunHeader( LCRunHeader* run) {
    _nRun++ ;
}



void ntuple_maker::processEvent( LCEvent * evt ) {

  streamlog_out(MESSAGE) << "   Processing event: " << evt->getEventNumber() << "   in run:  " << evt->getRunNumber() << std::endl ;


  if( isFirstEvent() ) {
    streamlog_out(MESSAGE) << "First event: initializing globals..." << std::endl ;
    ntupleFile=new TFile(m_outfileName.c_str(), "RECREATE");
    rawTree = new TTree("rawTree", "rawTree");
    int buffsize = 32000; //default buffer size 32KB
    rawTree->Branch("jet_n",&jet_n) ;
    rawTree->Branch("jet_etot",&jet_etot) ;
    rawTree->Branch("jet_pt","std::vector<double >",&jet_pt,buffsize,0) ;
    rawTree->Branch("jet_theta","std::vector<double >",&jet_theta,buffsize,0) ;
    rawTree->Branch("jet_phi","std::vector<double >",&jet_phi,buffsize,0) ;
    rawTree->Branch("jet_e","std::vector<double >",&jet_e,buffsize,0) ;
  }
  clearEventVariables();

  streamlog_out(MESSAGE) <<"Analyzing collections:"<<std::endl;
  for (int iColl = 0; iColl < inputCollections.size(); ++iColl)
  {
    LCCollection* thisCollection = 0 ;
    getCollection(thisCollection, inputCollections.at(iColl), evt);
    streamlog_out(MESSAGE) << " " << inputCollections.at(iColl) <<":                 "<<thisCollection<<std::endl;

    if( thisCollection != NULL && iColl == 2){
      streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<thisCollection<< std::endl ;
      for(int i=0; i< thisCollection->getNumberOfElements() ; i++){
        ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>( thisCollection->getElementAt( i ) ) ;
        jet_fourvec.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
        jet_pt.push_back(jet_fourvec.Pt());
        jet_theta.push_back(jet_fourvec.Theta());
        jet_phi.push_back(jet_fourvec.Phi());
        jet_e.push_back(jet_fourvec.E());
        jet_etot+=jet_fourvec.E();
        ++jet_n;
      }
    }else{
      streamlog_out(MESSAGE) << " Warning: collection " << inputCollections.at(iColl) <<" not available. Skip!"<<std::endl;
    }
  }


  // Get the collection of Jets
  // LCCollection* IsolatedLepton_collection = 0 ;
  // getCollection(IsolatedLepton_collection, m_IsolatedLepton_collection, evt);
  // LCCollection* durham_4jets_collection = 0 ;
  // getCollection(durham_4jets_collection, m_durham_4jets_collection, evt);
  // LCCollection* kt_4jets_radius05_collection = 0 ;
  // getCollection(kt_4jets_radius05_collection, m_kt_4jets_radius05_collection, evt);
  // LCCollection* kt_4jets_radius07_collection = 0 ;
  // getCollection(kt_4jets_radius07_collection, m_kt_4jets_radius07_collection, evt);
  // LCCollection* kt_4jets_radius10_collection = 0 ;
  // getCollection(kt_4jets_radius10_collection, m_kt_4jets_radius10_collection, evt);
  // LCCollection* kt_4jets_radius12_collection = 0 ;
  // getCollection(kt_4jets_radius12_collection, m_kt_4jets_radius12_collection, evt);
  // LCCollection* kt_4jets_radius15_collection = 0 ;
  // getCollection(kt_4jets_radius15_collection, m_kt_4jets_radius15_collection, evt);
  

  // streamlog_out(MESSAGE) <<"Collections present:"<<std::endl;
  // streamlog_out(MESSAGE) <<" IsolatedLepton_collection                 "<<IsolatedLepton_collection<<std::endl;
  // streamlog_out(MESSAGE) <<" durham_4jets_collection                   "<<durham_4jets_collection<<std::endl;
  // streamlog_out(MESSAGE) <<" kt_4jets_radius05_collection              "<<kt_4jets_radius05_collection<<std::endl;
  // streamlog_out(MESSAGE) <<" kt_4jets_radius07_collection              "<<kt_4jets_radius07_collection<<std::endl;
  // streamlog_out(MESSAGE) <<" kt_4jets_radius10_collection              "<<kt_4jets_radius10_collection<<std::endl;
  // streamlog_out(MESSAGE) <<" kt_4jets_radius12_collection              "<<kt_4jets_radius12_collection<<std::endl;
  // streamlog_out(MESSAGE) <<" kt_4jets_radius15_collection              "<<kt_4jets_radius15_collection<<std::endl;
  // streamlog_out(MESSAGE) <<"Analyzing..."<<std::endl;

  // if( kt_4jets_radius05_collection != NULL ){
  //   streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<kt_4jets_radius05_collection<< std::endl ;
  //   for(int i=0; i< kt_4jets_radius05_collection->getNumberOfElements() ; i++){
  //     ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>( kt_4jets_radius05_collection->getElementAt( i ) ) ;
  //     jet_fourvec.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
  //     jet_pt.push_back(jet_fourvec.Pt());
  //     jet_theta.push_back(jet_fourvec.Theta());
  //     jet_phi.push_back(jet_fourvec.Phi());
  //     jet_e.push_back(jet_fourvec.E());
  //     jet_etot+=jet_fourvec.E();
  //     ++jet_n;
  //   }
  // }

  streamlog_out(MESSAGE) << "Event "<<_nEvt<<": Fill tree..." << std::endl ;
  rawTree->Fill();

  _nEvt ++ ;
  
}



void ntuple_maker::check( LCEvent * evt ) {
    // nothing to check here - could be used to fill checkplots in reconstruction processor
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
  jet_fourvec.SetPxPyPzE(0,0,0,0);
  jet_n=0;
  jet_etot=0;
  jet_pt.clear();
  jet_theta.clear();
  jet_phi.clear();
  jet_e.clear();
  
  return;
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