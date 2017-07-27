#include "TrackCutChecker.h"

#include <cmath>
#include <sstream>
#include <iostream>


// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

using namespace lcio ;
using namespace marlin ;
using namespace std ;



//written by Andreas Alexander Maier spring 2016
//andreas.alexander.maier@cern.ch


double DeltaR(double eta0,double phi0, double eta1, double phi1){
   Double_t deta = eta0-eta1;
   Double_t dphi = TVector2::Phi_mpi_pi(phi0-phi1);
   return TMath::Sqrt( deta*deta+dphi*dphi );
}
double EtaFromTheta(double theta){
  return -TMath::Log(TMath::Tan(theta/2));
}



TrackCutChecker aTrackCutChecker ;


TrackCutChecker::TrackCutChecker() : Processor("TrackCutChecker") {
    // modify processor description
    _description = "TrackCutChecker does whatever it does ..." ;
    // register steering parameters: name, description, class-variable, default value
    registerInputCollection( LCIO::MCPARTICLE,
                                "CollectionName",
                                "Name of the MCParticle collection",
                                m_inputParticleCollection,
                                std::string("MCParticle"));

    registerInputCollection( LCIO::TRACK,
                             "TrackCollectionName",
                             "Track collection name",
                             m_inputTrackCollection,
                             std::string("SiTracks"));

    registerInputCollection( LCIO::LCRELATION,
                             "TrackRelationCollectionName",
                             "Track relation collection name",
                             m_inputTrackRelationCollection,
                             std::string("SiTrackRelations"));

    registerOutputCollection(LCIO::RECONSTRUCTEDPARTICLE,
                             "PFOCollectionName",
                             "PFO Collection Name",
                             m_outputPandoraPFOCollection,
                             std::string("PandoraPFOs"));

    registerInputCollection( LCIO::LCRELATION,
                             "RecoMCTruthLinkName",
                             "PFO relation collection name",
                             m_inputPFORelationCollection,
                             std::string("RecoMCTruthLink"));

    registerOutputCollection(LCIO::RECONSTRUCTEDPARTICLE,
                             "jetOut",
                             "Jet Collection Name",
                             m_outputJetkt7,
                             std::string("Jetkt7"));

    registerProcessorParameter( "TreeName",
                               "Name of the root tree",
                               m_treeName,
                               std::string("tracktree"));

    registerProcessorParameter( "OutputFileName",
                               "Name of the output file",
                               m_outfileName,
                               std::string("output.root"));
}




void TrackCutChecker::init() {
    streamlog_out(MESSAGE) << "   init called  " << std::endl ;
    printParameters() ;
    _nRun = 0 ;
    _nEvt = 0 ;
    h_res_mc_pfo_etot=new TH1F("h_res_mc_pfo_etot","h_res_mc_pfo_etot;(pfo_etot-mc_etot)/mc_etot;number of events",50,-1,1);
    h_res_mc_jet_etot=new TH1F("h_res_mc_jet_etot","h_res_mc_jet_etot;(jet_etot-mc_etot)/mc_etot;number of events",50,-1,1);
    h_dR_jet_pfo=new TH1F("h_dR_jet_pfo","h_dR_jet_pfo;#DeltaR to jet0;number of pfos",50,0,3.5);
}


void TrackCutChecker::processRunHeader( LCRunHeader* run) {
    _nRun++ ;
}



void TrackCutChecker::processEvent( LCEvent * evt ) {

  streamlog_out(MESSAGE) << "   Processing event: " << evt->getEventNumber() << "   in run:  " << evt->getRunNumber() << std::endl ;


  if( isFirstEvent() ) {
    streamlog_out(MESSAGE) << "First event: initializing globals..." << std::endl ;
    trackfile=new TFile(m_outfileName.c_str(), "RECREATE");
    tracktree = new TTree(m_treeName.c_str(),m_treeName.c_str());
    int buffsize = 32000; //default buffer size 32KB
    tracktree->Branch("mc_n",&mc_n) ;
    tracktree->Branch("mc_etot",&mc_etot) ;
    tracktree->Branch("mc_pdg","std::vector<int >",&mc_pdg,buffsize,0) ;
    tracktree->Branch("mc_pt","std::vector<double >",&mc_pt,buffsize,0) ;
    tracktree->Branch("mc_theta","std::vector<double >",&mc_theta,buffsize,0) ;
    tracktree->Branch("mc_phi","std::vector<double >",&mc_phi,buffsize,0) ;
    tracktree->Branch("mc_e","std::vector<double >",&mc_e,buffsize,0) ;
    tracktree->Branch("trk_n",&trk_n) ;
    tracktree->Branch("trk_etot",&trk_etot) ;
    tracktree->Branch("trk_pdg","std::vector<int >",&trk_pdg,buffsize,0) ;
    tracktree->Branch("trk_pt","std::vector<double >",&trk_pt,buffsize,0) ;
    tracktree->Branch("trk_theta","std::vector<double >",&trk_theta,buffsize,0) ;
    tracktree->Branch("trk_phi","std::vector<double >",&trk_phi,buffsize,0) ;
    tracktree->Branch("trk_e","std::vector<double >",&trk_e,buffsize,0) ;
    tracktree->Branch("trk_hit_n","std::vector<int >",&trk_hit_n,buffsize,0) ;
    tracktree->Branch("trk_chi2ndf","std::vector<double >",&trk_chi2,buffsize,0) ;
    tracktree->Branch("pfo_n",&pfo_n) ;
    tracktree->Branch("pfo_etot",&pfo_etot) ;
    tracktree->Branch("pfo_pdg","std::vector<int >",&pfo_pdg,buffsize,0) ;
    tracktree->Branch("pfo_pt","std::vector<double >",&pfo_pt,buffsize,0) ;
    tracktree->Branch("pfo_theta","std::vector<double >",&pfo_theta,buffsize,0) ;
    tracktree->Branch("pfo_phi","std::vector<double >",&pfo_phi,buffsize,0) ;
    tracktree->Branch("pfo_e","std::vector<double >",&pfo_e,buffsize,0) ;
    tracktree->Branch("pfo_trk_n","std::vector<int >",&pfo_trk_n,buffsize,0) ;
    tracktree->Branch("pfo_trk_relation","std::vector<int >",&pfo_trk_relation,buffsize,0) ;
    tracktree->Branch("pfo_trk_pdg","std::vector<int >",&pfo_trk_pdg,buffsize,0) ;
    tracktree->Branch("pfo_trk_phi","std::vector<double >",&pfo_trk_phi,buffsize,0) ;
    tracktree->Branch("pfo_trk_omega","std::vector<double >",&pfo_trk_omega,buffsize,0) ;
    tracktree->Branch("pfo_trk_dedx","std::vector<double >",&pfo_trk_dedx,buffsize,0) ;
    tracktree->Branch("jet_n",&jet_n) ;
    tracktree->Branch("jet_etot",&jet_etot) ;
    tracktree->Branch("jet_pt","std::vector<double >",&jet_pt,buffsize,0) ;
    tracktree->Branch("jet_theta","std::vector<double >",&jet_theta,buffsize,0) ;
    tracktree->Branch("jet_phi","std::vector<double >",&jet_phi,buffsize,0) ;
    tracktree->Branch("jet_e","std::vector<double >",&jet_e,buffsize,0) ;
    tracktree->Branch("jet_pfo_relation",&jet_pfo_relation) ;

  }//end is first event
  clearEventVariables();





  // Get the collection of MC particles
  LCCollection* particleCollection = 0 ;
  // streamlog_out(MESSAGE) << "Event "<<_nEvt<<": getCollection "<<m_inputParticleCollection<<"..." << std::endl ;
  getCollection(particleCollection, m_inputParticleCollection, evt); //if(particleCollection == 0) return;

  // Get the collection of tracks
  LCCollection* trackCollection = 0 ;
  // streamlog_out(MESSAGE) << "Event "<<_nEvt<<": getCollection "<<m_inputTrackCollection<<"..." << std::endl ;
  getCollection(trackCollection, m_inputTrackCollection, evt); //if(trackCollection == 0) return;

  // Get the collection of tracks
  LCCollection* trackRelationCollection = 0 ;
  // streamlog_out(MESSAGE) << "Event "<<_nEvt<<": getCollection "<<m_inputTrackRelationCollection<<"..." << std::endl ;
  getCollection(trackRelationCollection, m_inputTrackRelationCollection, evt); //if(trackRelationCollection == 0) return;

  // Get the collection of PFOs
  LCCollection* PFOCollection = 0 ;
  // streamlog_out(MESSAGE) << "Event "<<_nEvt<<": getCollection "<<m_outputPandoraPFOCollection<<"..." << std::endl ;
  getCollection(PFOCollection, m_outputPandoraPFOCollection, evt); //if(PFOCollection == 0) return;

  // Get the collection of tracks
  LCCollection* PFORelationCollection = 0 ;
  // streamlog_out(MESSAGE) << "Event "<<_nEvt<<": getCollection "<<m_inputPFORelationCollection<<"..." << std::endl ;
  getCollection(PFORelationCollection, m_inputPFORelationCollection, evt); //if(PFORelationCollection == 0) return;

  // Get the collection of Jets
  LCCollection* jetCollection = 0 ;
  // streamlog_out(MESSAGE) << "Event "<<_nEvt<<": getCollection "<<m_outputJetkt7<<"..." << std::endl ;
  getCollection(jetCollection, m_outputJetkt7, evt); //if(jetCollection == 0) return;


  streamlog_out(MESSAGE) <<"Collections present:"<<endl;
  streamlog_out(MESSAGE) <<" particleCollection         "<<particleCollection<<endl;
  streamlog_out(MESSAGE) <<" trackCollection            "<<trackCollection<<endl;
  streamlog_out(MESSAGE) <<" trackRelationCollection    "<<trackRelationCollection<<endl;
  streamlog_out(MESSAGE) <<" PFOCollection              "<<PFOCollection<<endl;
  streamlog_out(MESSAGE) <<" PFORelationCollection      "<<PFORelationCollection<<endl;
  streamlog_out(MESSAGE) <<" jetCollection              "<<jetCollection<<endl;
  streamlog_out(MESSAGE) <<"Analyzing..."<<endl;


  // Fill info from collections
  if( particleCollection != NULL ){
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<m_inputParticleCollection<< std::endl ;
    for(int i=0; i< particleCollection->getNumberOfElements() ; i++){
      MCParticle* particle = dynamic_cast<MCParticle*>( particleCollection->getElementAt( i ) ) ;
      if (particle->getGeneratorStatus()!=1) continue;
      mc_pdg.push_back(particle->getPDG());
      mc_particle.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
      mc_pt.push_back(mc_particle.Pt());
      mc_theta.push_back(mc_particle.Theta());
      mc_phi.push_back(mc_particle.Phi());
      mc_e.push_back(mc_particle.E());
      mc_etot+=mc_particle.E();
      ++mc_n;
    }
  }

  if( trackCollection != NULL && trackRelationCollection != NULL){
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<m_inputTrackCollection<< std::endl ;
    trk_n = trackCollection->getNumberOfElements()  ;
    // Create the relations navigator
    LCRelationNavigator* relation = new LCRelationNavigator( trackRelationCollection );
    for(int i=0; i< trk_n ; i++){
      Track* track = dynamic_cast<Track*>( trackCollection->getElementAt(i) ) ;
      const LCObjectVec& mcparticleVector = relation->getRelatedToObjects( track );
      // Take the first MC particle (there should only be one)
      if (mcparticleVector.size() > 1){
        std::cout << "WARNING: more than 1 related particle for this track!!!" <<m_inputTrackCollection<< std::endl ;
      }
      MCParticle* particle = dynamic_cast<MCParticle*>(mcparticleVector.at(0));
      trk_pdg.push_back(particle->getPDG());
      trk_particle.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
      trk_pt.push_back(trk_particle.Pt());
      trk_theta.push_back(trk_particle.Theta());
      trk_phi.push_back(trk_particle.Phi());
      trk_e.push_back(trk_particle.E());
      trk_etot+=trk_particle.E();
      trk_hit_n.push_back(track->getTrackerHits().size());
      trk_chi2.push_back(track->getChi2());
      trk_ndf.push_back(track->getNdf());
      trk_chi2ndf.push_back(track->getChi2()/track->getNdf());
    }
  }

  if( PFOCollection != NULL && PFORelationCollection != NULL){
    streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<m_outputPandoraPFOCollection<< std::endl ;
    pfo_n = PFOCollection->getNumberOfElements()  ;
    // Create the relations navigator
    LCRelationNavigator* relation = new LCRelationNavigator( PFORelationCollection );
    for(int i=0; i< pfo_n ; i++){
      ReconstructedParticle* pfo = dynamic_cast<ReconstructedParticle*>( PFOCollection->getElementAt( i ) ) ;

      //now get pfo pdg info from related MC particle
      const LCObjectVec& mcparticleVector = relation->getRelatedToObjects( pfo );
      // Take the first MC particle (there should only be one)
      if (mcparticleVector.size() > 1){
        std::cout << "WARNING: more than 1 related particle for this track!!!" <<m_outputPandoraPFOCollection<< std::endl ;
      }
      MCParticle* particle = dynamic_cast<MCParticle*>(mcparticleVector.at(0));
      pfo_pdg.push_back(particle->getPDG());
      ParticleID* pfoID=dynamic_cast<ParticleID*>(pfo->getParticleIDUsed());
      if (pfoID){
        streamlog_out(MESSAGE)<<"PDG info saved for this pfo! From pfo: "<<pfoID->getPDG()<<" from relation: "<< particle->getPDG()<<""<<std::endl;
        // streamlog_out(MESSAGE)<<"Saving preferred PDG ("<<pfoID->getPDG()<<") for pfo "<<i<<std::endl;
        // pfo_pdg.push_back(pfoID->getPDG());
        // streamlog_out(MESSAGE)<<"We have "<<pfo->getParticleIDs().size()<<" possible particle ids for this pfo"<<std::endl;
        // //loop on particle IDs sorted by likelihood
        // for (int j = 0; j < pfo->getParticleIDs().size(); ++j)
        // {
        //   streamlog_out(MESSAGE)<<"pfo "<<i<<": reading particleID "<<j<<std::endl;
        //   ParticleID* pfoIDsorted=dynamic_cast<ParticleID*>(pfo->getParticleIDs().at(j));
        //   // pfo+pdg_list.push_back(pfo->getParticleIDs()); //syntax not clear
        // }
      }else{
        // streamlog_out(MESSAGE)<<"No PDG info saved for this pfo!"<<std::endl;
      }
      pfo_fourvec.SetPxPyPzE(pfo->getMomentum()[0],pfo->getMomentum()[1],pfo->getMomentum()[2],pfo->getEnergy());
      pfo_pt.push_back(pfo_fourvec.Pt());
      pfo_theta.push_back(pfo_fourvec.Theta());
      pfo_phi.push_back(pfo_fourvec.Phi());
      pfo_e.push_back(pfo_fourvec.E());
      pfo_etot+=pfo_fourvec.E();
      pfo_trk_n.push_back(pfo->getTracks().size());
      for (int j = 0; j < pfo_trk_n.at(i); ++j)
      {
        streamlog_out(DEBUG)<<"Analysing track "<<j<<" of pfo "<<i<<"..."<<std::endl;
        Track* track = dynamic_cast<Track*>( pfo->getTracks().at(j) ) ;
        //do something with the tracks
        pfo_trk_relation.push_back(i);
        pfo_trk_pdg.push_back(0);
        pfo_trk_phi.push_back(track->getPhi());
        pfo_trk_omega.push_back(track->getOmega());
        pfo_trk_dedx.push_back(track->getdEdx());
      }
    }

    if( jetCollection != NULL ){
      streamlog_out(MESSAGE) << "Event "<<_nEvt<<": loop over collection " <<m_outputJetkt7<< std::endl ;
      for(int i=0; i< jetCollection->getNumberOfElements() ; i++){
        ReconstructedParticle* particle = dynamic_cast<ReconstructedParticle*>( jetCollection->getElementAt( i ) ) ;
        jet_fourvec.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
        jet_pt.push_back(jet_fourvec.Pt());
        jet_theta.push_back(jet_fourvec.Theta());
        jet_phi.push_back(jet_fourvec.Phi());
        jet_e.push_back(jet_fourvec.E());
        jet_etot+=jet_fourvec.E();
        if( PFOCollection != NULL && PFORelationCollection != NULL){
          std::vector<double> tmp;
          jet_pfo_relation.push_back(tmp);
          for(int pfo_idx=0;pfo_idx<pfo_n;pfo_idx++){
            double dR_jet_pfo=DeltaR(EtaFromTheta(jet_theta.back()),jet_phi.back(),EtaFromTheta(pfo_theta.at(pfo_idx)),pfo_phi.at(pfo_idx));
            if (i==0) h_dR_jet_pfo->Fill(dR_jet_pfo);
            if (dR_jet_pfo<TMath::Pi()/2){
              // printf("Jet %d contains pfo %d with DeltaR=%.2f!\n",i,pfo_idx,dR_jet_pfo );
              jet_pfo_relation.back().push_back(pfo_idx);
            }
          }
        }
        ++jet_n;
      }
    }
  }


  // Fill hisograms
  h_res_mc_pfo_etot->Fill((pfo_etot-mc_etot)/mc_etot);
  h_res_mc_jet_etot->Fill((jet_etot-mc_etot)/mc_etot);


  streamlog_out(MESSAGE) << "Event "<<_nEvt<<": Fill tree..." << std::endl ;
  tracktree->Fill();

  _nEvt ++ ;
  
}



void TrackCutChecker::check( LCEvent * evt ) {
    // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void TrackCutChecker::end(){
  streamlog_out(MESSAGE) << "Write tree..." << std::endl ;
  trackfile->cd();
  h_res_mc_pfo_etot->Write();
  h_res_mc_jet_etot->Write();
  h_dR_jet_pfo->Write();
  tracktree->Write();
  // tracktree->Print("all");
  trackfile->Close();


  printParameters() ;
  streamlog_out(MESSAGE) << "TrackCutChecker::end()  " << name() << " processed " << _nEvt << " events in " << _nRun << " runs "<< std::endl ;
}


void TrackCutChecker::clearEventVariables(){
  mc_particle.SetPxPyPzE(0,0,0,0);
  mc_n=0;
  mc_etot=0;
  mc_pdg.clear();
  mc_pt.clear();
  mc_theta.clear();
  mc_phi.clear();
  mc_e.clear();

  trk_particle.SetPxPyPzE(0,0,0,0);
  trk_n=0;
  trk_etot=0;
  trk_pdg.clear();
  trk_pt.clear();
  trk_theta.clear();
  trk_phi.clear();
  trk_e.clear();
  trk_hit_n.clear();
  trk_chi2.clear();
  trk_ndf.clear();
  trk_chi2ndf.clear();

  pfo_n=0;
  pfo_etot=0;
  pfo_pdg.clear();
  pfo_pt.clear();
  pfo_theta.clear();
  pfo_phi.clear();
  pfo_e.clear();

  pfo_trk_n.clear();
  pfo_trk_relation.clear();
  pfo_trk_pdg.clear();
  pfo_trk_phi.clear();
  pfo_trk_omega.clear();
  pfo_trk_dedx.clear();

  jet_fourvec.SetPxPyPzE(0,0,0,0);
  jet_n=0;
  jet_etot=0;
  jet_pt.clear();
  jet_theta.clear();
  jet_phi.clear();
  jet_e.clear();
  jet_pfo_relation.clear();
  
  return;
}
void TrackCutChecker::getCollection(LCCollection* &collection, std::string collectionName, LCEvent* evt){
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