 /*
 * TagWWstoringProcessor.cpp
 *
 *  Created on: 22.11.2016
 *      Author: Rickard Stroem (CERN) - lars.rickad.stroem@cern.ch
 *           ROOT Booking Processor for Top Asymmetry Analysis
 */

#include <TagWWstoringProcessor.h>

#include <IMPL/LCCollectionVec.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/MCParticle.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TMath.h>

TagWWstoringProcessor aTagWWstoringProcessor;

TagWWstoringProcessor::TagWWstoringProcessor() : Processor("TagWWstoringProcessor"),
							   _mcpColName( "" ),
							   _recoColName( "" ),
							   _jetColFat( "" ),
								 _jetColFatTrimmed( "" ),
							   _jetColSmall( "" ),
								 _particleInCol( "" ),

						     //_tm_out( false ),
							
							   _outFileName( "" ),
							   _treeName( "" ),
							   
							   _nRun( 0 ),
							   _nEvt( 0 ),  
				
							   _out( NULL ),
							   _tree( NULL ),
							   
							   _nPFO( 0 ),
							   _nMCP( 0 ),
							   
							   _minPt( 0. ), 
							   _Et( 0. ), 
							   _ev_inv_mass( 0. ), 

					
								 _daughters( 0. ),
								 _fatJetCol( 0. ),
 								 _fatJetColTrimmed( 0 ),
						 		 _smallJetCol( 0. ),
						 		 _smallJets( 0 ),
								 _particleCol( 0 ),

								 _particlesInEvent( 0 ),

							   _jetCharge( 0 ),
							   _jetType( 0 ),
							   _fatJetPt( 0 ),
							   _jetEnergy( 0 ),
							   _jetEta( 0 ),
							   _jetMass( 0 ),
							   _jetTheta( 0 ),
							   _jetPhi( 0 ),

								 _smallJetPt( 0 ),
								 _smallJetMass( 0 ),
						 		 _smallJetPtComb( 0 ),								
				 			   _smallJetMassComb( 0 ),
								
								 _smallJetMassW( 0 ),
								 _smallJetMassZ( 0 ),

								 _fatJetMassW( 0 ),
								 _fatJetMassZ( 0 ),

								 _vecJetQuark( 0 ),

								 _WZtag( 3 ),
								 _singleWZtag( 3 ),
	
							   _pdg_d( 0 ),
							   _pdg_u( 0 ),
							   _pdg_s( 0 ),
							   _pdg_c( 0 ),
							   _pdg_b( 0 ),
							   _pdg_t( 0 ),
							   
							   _pdg_electron( 0 ),
							   _pdg_velectron( 0 ),
							   _pdg_muon( 0 ),
							   _pdg_vmuon( 0 ),
							   _pdg_tau( 0 ),
							   _pdg_vtau( 0 ),

							   _pdg_photon( 0 ),
							   _pdg_Z( 0 ),
							   _pdg_W( 0 ),

							   _t_mass( 0. ),
								 _w_t_mass( 0. ),
								 _w_mass( 0. ),
								 _z_mass( 0. ),
								 _w_width( 0. ),
								 _z_width( 0. ),

								 jet1( 0.,0.,0.,0. ),
								 jet2( 0.,0.,0.,0. ),

								 trimmedJet1( 0.,0.,0.,0. ),
								 trimmedJet2( 0.,0.,0.,0. ),

								 smalljet1( 0.,0.,0.,0. ),
								 smalljet2( 0.,0.,0.,0. ),
								 smalljet3( 0.,0.,0.,0. ),
								 smalljet4( 0.,0.,0.,0. ),

							

								 _smallJetComb1( 0.,0.,0.,0. ),
								 _smallJetComb2( 0.,0.,0.,0. ),

								 _mcFatJet1( 0.,0.,0.,0. ),
								 _mcFatJet2( 0.,0.,0.,0. ),
								 
								 quark1( 0.,0.,0.,0. ),
								 quark2( 0.,0.,0.,0. ),
								 quark3( 0.,0.,0.,0. ),
								 quark4( 0.,0.,0.,0. ),
						 
								
								 quark12( 0.,0.,0.,0. ),
								 quark34( 0.,0.,0.,0. ),
								 quark13( 0.,0.,0.,0. ),
								 quark24( 0.,0.,0.,0. ),
								 quark14( 0.,0.,0.,0. ),
						     quark23( 0.,0.,0.,0. ),

								 charge1( 0. ),
								 charge2( 0. ),
								 charge3( 0. ),
								 charge4( 0. ),

								 flavour1( 0. ),
								 flavour2( 0. ),
								 flavour3( 0. ),
								 flavour4( 0. ),

								 _fatJetMass( 0 ),
								 _deltaRqq( 0 ),
								 _deltaRqqqq( 0. ),
								 _deltaRjj( 0. ),
								 _deltaPhiqq( 0. ),
								 _deltaPhiqqqq( 0 ),
								 _deltaPhijj( 0. ),

						 _deltaTheta( 0. ),
						 _deltaRtheta( 0. ),

							   _pjList( 0 ),

								_use_4jet( 0 ),
								_useTrimming( 0 ),
								_useAllParticles( 0 )

{
  _description = "Using the FastJet library to identify jets using _";
  
  // the input & output collections
  registerInputCollection(LCIO::MCPARTICLE, "mcParticles",
			  "Name of the MCParticle collection",
			  _mcpColName, std::string("MCParticlesSkimmed"));

  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "generalParticleIn", 
			  "A list of all PFO objects",
			  _recoColName, std::string("SelectedPandoraPFANewPFOs"));

  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "fatJetCollection",
			   "A list of all reconstructed fat-jets",
			   _jetColFat, std::string("JetOutFat"));

  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "fatJetCollectionTrimmed",
			   "A list of all reconstructed fat-jets",
			   _jetColFatTrimmed, std::string("JetOutFatTrimmed"));
	
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "smallJetCollection",
			   "A list of all reconstructed smallR-jets",
			   _jetColSmall, std::string("JetOutSmall"));


  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "particleInCol",
			   "Reconstruced particles to save",
			   _particleInCol, std::string("TightSelectedPandoraPFANewPFOs"));

  //Output parameter
	//registerProcessorParameter("tm_out", "Set terminal output",
			     //_tm_out, false);  
	registerProcessorParameter("outFileName", "Name of the output root file",
			     _outFileName, std::string("WWana.root"));
  registerProcessorParameter("treeName", "Name of the tree",
			     _treeName, std::string("WWanaTree"));

  registerProcessorParameter("useFourJet", "4 jets using",
			     _use_4jet, false);

  registerProcessorParameter("useTrimming", "saving trimmed jets",
			     _useTrimming, false);

  registerProcessorParameter("useAllParticles", "store all PFOS in event",
			     _useAllParticles, false);
 
}

TagWWstoringProcessor::~TagWWstoringProcessor(){
}

void TagWWstoringProcessor::init(){

  printParameters();

  //Initiate event variables
  _nRun = 0;
  _nEvt = 0;

  //Initate root file
  _out = new TFile(_outFileName.c_str(),"RECREATE");
  
  //Initiate root tree
  _tree = new TTree(_treeName.c_str(),_treeName.c_str());

  //Set up general event branches

  _tree->Branch("eventInvMass",&_ev_inv_mass,"eventInvMass/D");

  //Set up jet branches
  _tree->Branch("fatJetPt","std::vector<float>",&_fatJetPt);
  _tree->Branch("fatJetMass","std::vector<float>",&_fatJetMass);
 	_tree->Branch("deltaRqq","std::vector<float>",&_deltaRqq);
 	_tree->Branch("deltaRqqqq",&_deltaRqqqq,"deltaRqqqq/D");
 	_tree->Branch("deltaRjj",&_deltaRjj,"deltaRjj/D");
 	_tree->Branch("deltaPhiqq","std::vector<float>",&_deltaPhiqq);
	_tree->Branch("deltaPhiqqqq",&_deltaPhiqqqq,"deltaPhiqqqq/D");
 	_tree->Branch("deltaPhijj",&_deltaPhijj,"deltaPhijj/D");

	_tree->Branch("smallJetPt","std::vector<float>",&_smallJetPt);
	_tree->Branch("smallJetMass","std::vector<float>",&_smallJetMass);
	//	_tree->Branch("smallJets","std::vector<TLorentzVector>",&_smallJets);
	_tree->Branch("smallJetPtComb","std::vector<float>",&_smallJetPtComb);
	_tree->Branch("smallJetMassComb","std::vector<float>",&_smallJetMassComb);

	_tree->Branch("vecJetQuark","std::vector<float>",&_vecJetQuark);


	_tree->Branch("mcFatJet1","TLorentzVector",&_mcFatJet1);
	_tree->Branch("mcFatJet2","TLorentzVector",&_mcFatJet2);
	_tree->Branch("fatJet1","TLorentzVector",&jet1);
	_tree->Branch("fatJet2","TLorentzVector",&jet2);
	_tree->Branch("fatJetTrimmed1","TLorentzVector",&trimmedJet1);
	_tree->Branch("fatJetTrimmed2","TLorentzVector",&trimmedJet2);
	_tree->Branch("smallJet1","TLorentzVector",&smalljet1);
	_tree->Branch("smallJet2","TLorentzVector",&smalljet2);
	_tree->Branch("smallJet3","TLorentzVector",&smalljet3);
	_tree->Branch("smallJet4","TLorentzVector",&smalljet4);

	

	_tree->Branch("mcQuark1","TLorentzVector",&quark1);
	_tree->Branch("mcQuark2","TLorentzVector",&quark2);
	_tree->Branch("mcQuark3","TLorentzVector",&quark3);
	_tree->Branch("mcQuark4","TLorentzVector",&quark4);
	
	_tree->Branch("smallJetComb1","TLorentzVector",&_smallJetComb1);
	_tree->Branch("smallJetComb2","TLorentzVector",&_smallJetComb2);

	_tree->Branch("smallJetMassW","std::vector<float>",&_smallJetMassW);
	_tree->Branch("smallJetMassZ","std::vector<float>",&_smallJetMassZ);
	_tree->Branch("fatJetMassW","std::vector<float>",&_fatJetMassW);
	_tree->Branch("fatJetMassZ","std::vector<float>",&_fatJetMassZ);

	_tree->Branch("deltaTheta","std::vector<float>",&_deltaTheta);
	_tree->Branch("deltaRtheta","std::vector<float>",&_deltaRtheta);
	_tree->Branch("WZtag",&_WZtag,"WZtag/I");
	_tree->Branch("singleWZtag",&_singleWZtag,"singleWZtag/I");
	_tree->Branch("particlesInEvent","std::vector<std::vector<float>>",&_particlesInEvent);

//  tdat.Branch("p4datrad","TLorentzVector",&p4datrad)

  //PDG values                                                                                                                
  _pdg_d = 1;
  _pdg_u = 2;
  _pdg_s = 3;
  _pdg_c = 4;
  _pdg_b = 5;
  _pdg_t = 6;

  _pdg_electron = 11;
  _pdg_velectron = 12;
  _pdg_muon = 13;
  _pdg_vmuon = 14;
  _pdg_tau = 15;
  _pdg_vtau = 16;

  _pdg_photon = 22;
  _pdg_Z = 23; 
  _pdg_W = 24;

  _t_mass = 173.21;
  _w_t_mass = 2.0;

  _w_mass = 80.385;
  _z_mass = 91.1876;
  _w_width = 2.085;
  _z_width = 2.4952;

} //end init

void TagWWstoringProcessor::processRunHeader(LCRunHeader*) {
  _nRun++ ;
} //end processRunHeader

void TagWWstoringProcessor::processEvent(LCEvent * evt)
{
  //if(_nEvt%100==0) streamlog_out(MESSAGE) << "ProcessEvent ----- _nEvt = " << _nEvt << std::endl;
	bool _tm_out = false;

  clearVectors();
  clearNumbers();
	//bool ZZflag = false;
	//bool WWflag = false;

  //General MC information
  LCCollection* mcpIn;
	try { mcpIn = evt->getCollection( _mcpColName);
	  if (mcpIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  _nMCP = mcpIn->getNumberOfElements();
  EVENT::MCParticle* mcParticle = NULL;
  for(int i=0; i<_nMCP ; i++){
    mcParticle = dynamic_cast<EVENT::MCParticle*>(mcpIn->getElementAt(i));
    if(mcParticle == NULL){ return; }
    int pdg = fabs(mcParticle->getPDG());
    if (isElectron(pdg) && mcParticle->getParents().size()!=0){ break; }
  }
  _daughters = mcParticle->getDaughters(); //four quarks
  _ev_inv_mass = getEventInvMass(_daughters);



	// ******************* RETRIEVE LARGE-R JET COLLECTION ******************* //
	LCCollection* jetInFat;
	try{ jetInFat = evt->getCollection( _jetColFat );
		if( jetInFat->getNumberOfElements() < 1){
			throw DataNotAvailableException("Collection is there, but its empty!");
		}
	}
	catch (DataNotAvailableException e){
		streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
		return;
	}
	EVENT::ReconstructedParticle* fatJet = NULL;
	for(int i=0;i<jetInFat->getNumberOfElements();i++){
		fatJet = dynamic_cast<EVENT::ReconstructedParticle*>(jetInFat->getElementAt(i));
		if(fatJet == NULL) { return; }
		_fatJetCol.push_back( fatJet ); 
	}

	// **************** RETRIEVE TRIMMED LARGE-R JET COLLECTION *************** //
	
	if(_useTrimming){

		LCCollection* jetInFatTrimmed;
		try{ jetInFatTrimmed = evt->getCollection( _jetColFatTrimmed );
			if( jetInFatTrimmed->getNumberOfElements() < 1){
				throw DataNotAvailableException("Collection is there, but its empty!");
			}
		}
		catch (DataNotAvailableException e){
			streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
			return;
		}
		EVENT::ReconstructedParticle* fatJetTrimmed = NULL;
		for(int i=0;i<jetInFatTrimmed->getNumberOfElements();i++){
			fatJetTrimmed = dynamic_cast<EVENT::ReconstructedParticle*>(jetInFatTrimmed->getElementAt(i));
			if(fatJetTrimmed == NULL) { return; }
			_fatJetColTrimmed.push_back( fatJetTrimmed ); 
		}

		trimmedJet1 = recoToTLV( _fatJetColTrimmed.at(0) );
		trimmedJet2 = recoToTLV( _fatJetColTrimmed.at(1) );

	}


	// ******************* RETRIEVE SMALL-R JET COLLECTION ******************* //

	if(_use_4jet){
		LCCollection* jetInSmall;
		try{ jetInSmall = evt->getCollection( _jetColSmall );
			if( jetInSmall->getNumberOfElements() < 1){
				throw DataNotAvailableException("Collection is there, but its empty!");
			}
		}
		catch (DataNotAvailableException e){
			streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
			return;
		}
		EVENT::ReconstructedParticle* smallJet = NULL;
		for(int i=0;i<jetInSmall->getNumberOfElements();i++){
			smallJet = dynamic_cast<EVENT::ReconstructedParticle*>(jetInSmall->getElementAt(i));
			if(smallJet == NULL) { return; }
			_smallJetCol.push_back( smallJet ); 
		} 
		for(unsigned int i=0;i<_smallJetCol.size();i++){
			_smallJets.push_back(recoToTLV(_smallJetCol.at(i)));
		}
		
	
		float bigR = 500000;
		int q1 = 10;
		int q2 = 10;
		//Combine nearest small jets (delta-R)
		for(unsigned int i=0;i<_smallJets.size();i++){
			for(unsigned int j=i+1;j<_smallJets.size();j++){
				float deltaRjj = _smallJets.at(i).DeltaR(_smallJets.at(j));
				if( deltaRjj < bigR ){
					bigR = deltaRjj;
					_smallJetComb1 = _smallJets.at(i) + _smallJets.at(j);
					q1=i;
					q2=j;
				}
			}
		}
		if( (q1==0) && (q2==1) ) _smallJetComb2 = _smallJets.at(2) + _smallJets.at(3);
		if( (q1==0) && (q2==2) ) _smallJetComb2 = _smallJets.at(1) + _smallJets.at(3);
		if( (q1==0) && (q2==3) ) _smallJetComb2 = _smallJets.at(1) + _smallJets.at(2);
		if( (q1==1) && (q2==2) ) _smallJetComb2 = _smallJets.at(0) + _smallJets.at(3);
		if( (q1==1) && (q2==3) ) _smallJetComb2 = _smallJets.at(0) + _smallJets.at(2);
		if( (q1==2) && (q2==3) ) _smallJetComb2 = _smallJets.at(0) + _smallJets.at(1);
	
		//save mass and pt of combined small jets
		_smallJetMassComb.push_back(_smallJetComb1.M());
		_smallJetMassComb.push_back(_smallJetComb2.M());

		_smallJetPtComb.push_back(_smallJetComb1.Pt());
		_smallJetPtComb.push_back(_smallJetComb2.Pt());
		
		for(unsigned int i=0;i<_smallJets.size();i++){
		_smallJetPt.push_back(_smallJets.at(i).Pt());
		_smallJetMass.push_back(_smallJets.at(i).M());
		}
		
		smalljet1 = _smallJets.at(0);
		smalljet2 = _smallJets.at(1);
		smalljet3 = _smallJets.at(2);
		smalljet4 = _smallJets.at(3);

	
	}//end if small jets


	// ******************* RETRIEVE LARGE-R JET COLLECTION ******************* //
	
	if(_useAllParticles){
		LCCollection* recPartIn;
		try{ recPartIn = evt->getCollection( _particleInCol );
			if( recPartIn->getNumberOfElements() < 1){
				throw DataNotAvailableException("Collection is there, but its empty!");
			}
		}
		catch (DataNotAvailableException e){
			streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
			return;
		}
		EVENT::ReconstructedParticle* recPart = NULL;
		for(int i=0;i<recPartIn->getNumberOfElements();i++){
			recPart = dynamic_cast<EVENT::ReconstructedParticle*>(recPartIn->getElementAt(i));
		
			TLorentzVector tempTLV = recoToTLV(recPart);
			std::vector<float> tempParticle;
			tempParticle.push_back( tempTLV.Px() );
			tempParticle.push_back( tempTLV.Py() );
			tempParticle.push_back( tempTLV.Pz() );
			tempParticle.push_back( tempTLV.E() );	
			_particlesInEvent.push_back(tempParticle);
		}
	}

	//convert to TLV
	jet1 = recoToTLV(_fatJetCol.at(0));
	jet2 = recoToTLV(_fatJetCol.at(1));

	//save jet pt
	_fatJetPt.push_back(jet1.Pt());	
	_fatJetPt.push_back(jet2.Pt());		
	//save jet mass
	_fatJetMass.push_back(jet1.M());
	_fatJetMass.push_back(jet2.M());



	if(_tm_out){
	  std::cout << "Mass of Jet 1: " << _fatJetMass.at(0) << std::endl;
	  std::cout << "Mass of Jet 2: " << _fatJetMass.at(1) << std::endl;
	}
		
	
	//save angular distances
	_deltaRjj = jet1.DeltaR(jet2);
	_deltaPhijj = jet1.DeltaPhi(jet2);

	// ******************* MONTE CARLO LEVEL ******************* //
	if(isQuark(_daughters)){
	
		quark1=mcToTLV(_daughters.at(0));
		quark2=mcToTLV(_daughters.at(1));
		quark3=mcToTLV(_daughters.at(2));
		quark4=mcToTLV(_daughters.at(3));

		//option 1:  QUARK 1+2. QUARK 3+4		
		quark12 = quark1 + quark2;
		quark34 = quark3 + quark4;
		//option 2: QUARK 1+3. QUARK 2+4
		quark13 = quark1 + quark3;  
		quark24 = quark2 + quark4;  
		//option 3: QUARK 1+4, QUARk 2+3
		quark14 = quark1 + quark4; 
		quark23 = quark2 + quark3; 
		
		//CHARGE
		charge1 = _daughters.at(0)->getCharge();
		charge2 = _daughters.at(1)->getCharge();
		charge3 = _daughters.at(2)->getCharge();
		charge4 = _daughters.at(3)->getCharge();

		//FLAVOUR
		flavour1 = _daughters.at(0)->getPDG();
		flavour2 = _daughters.at(1)->getPDG();
		flavour3 = _daughters.at(2)->getPDG();
		flavour4 = _daughters.at(3)->getPDG();
		
		float eventCharge = 3;
		float eventFlavour = -3;

		// ********  MASS -> CHARGE -> FLAVOUR -> FLAG ************** //
		//option 1
		if( (quark12.M() <= (_z_mass+10*_z_width)) && (quark12.M() >= (_w_mass-10*_w_width)) ){
			if( (quark34.M() <= (_z_mass+10*_z_width)) && (quark34.M() >= (_w_mass-10*_w_width)) ){
				if( (fabs(charge1+charge2) == 1) && (fabs(charge3+charge4) == 1) ) eventCharge = 1;
				if( (fabs(charge1+charge2) == 0) && (fabs(charge3+charge4) == 0) ) eventCharge = 0;
				if( (fabs(flavour1+flavour2) != 0) && (fabs(flavour3+flavour4) != 0) ) eventFlavour = 1;
				if( (fabs(flavour1+flavour2) == 0) && (fabs(flavour3+flavour4) == 0) ) eventFlavour = 0;
				if( (eventCharge == 1) && (eventFlavour == 1) ) _WZtag = 1;
				if( (eventCharge == 0) && (eventFlavour == 0) ) _WZtag = 0;
				if( _WZtag != 3){
					_deltaRqq.push_back(quark1.DeltaR(quark2)); //Distance between quarks in pairs
					_deltaRqq.push_back(quark3.DeltaR(quark4));
					_deltaRqqqq=quark12.DeltaR(quark34); //Distance between quark pairs
					_deltaPhiqqqq=quark12.DeltaPhi(quark34);
					float deltaPhi1= quark1.DeltaPhi(quark2);
					float deltaPhi2= quark3.DeltaPhi(quark4);
					float deltaTheta1 = abs(quark1.Theta()-quark2.Theta());
					float deltaTheta2 = abs(quark3.Theta()-quark4.Theta());
					_deltaPhiqq.push_back(deltaPhi1);
		      _deltaPhiqq.push_back(deltaPhi2);
					_deltaTheta.push_back( deltaTheta1 );
					_deltaTheta.push_back( deltaTheta2 );
					_deltaRtheta.push_back( TMath::Sqrt(deltaTheta1*deltaTheta1+deltaPhi1*deltaPhi1) );
					_deltaRtheta.push_back( TMath::Sqrt(deltaTheta2*deltaTheta2+deltaPhi2*deltaPhi2) );
					_mcFatJet1=quark12;
					_mcFatJet2=quark34;
					if(_WZtag == 0){		
						_fatJetMassZ.push_back(_fatJetMass.at(0));
						_fatJetMassZ.push_back(_fatJetMass.at(1));	
						if(_use_4jet){
							_smallJetMassZ.push_back(_smallJetMassComb.at(0));
							_smallJetMassZ.push_back(_smallJetMassComb.at(1));
						}
					}	
					if(_WZtag == 1){
						_fatJetMassW.push_back(_fatJetMass.at(0));
						_fatJetMassW.push_back(_fatJetMass.at(1));	
						if(_use_4jet){
							_smallJetMassW.push_back(_smallJetMassComb.at(0));
							_smallJetMassW.push_back(_smallJetMassComb.at(1));
						}					}
					eventCharge = 3;
					eventFlavour = -3;
					_WZtag = 3;		
					if(_tm_out) std::cout << "chose quark12 + quark34    " << "deltaRqq= " << _deltaRqq.at(0) << " " << _deltaRqq.at(1) <<  std::endl;
				}
			}
		}
		//option 2
		if( (quark13.M() <= (_z_mass+10*_z_width)) && (quark13.M() >= (_w_mass-10*_w_width)) ){
			if( (quark24.M() <= (_z_mass+10*_z_width)) && (quark24.M() >= (_w_mass-10*_w_width)) ){
				if( (fabs(charge1+charge3) == 1) && (fabs(charge2+charge4) == 1) ) eventCharge = 1;
				if( (fabs(charge1+charge3) == 0) && (fabs(charge2+charge4) == 0) ) eventCharge = 0;
				if( (fabs(flavour1+flavour3) != 0) && (fabs(flavour2+flavour4) != 0) ) eventFlavour = 1;
				if( (fabs(flavour1+flavour3) == 0) && (fabs(flavour2+flavour4) == 0) ) eventFlavour = 0;
				if( (eventCharge == 1) && (eventFlavour == 1) ) _WZtag = 1;
				if( (eventCharge == 0) && (eventFlavour == 0) ) _WZtag = 0;
				if( _WZtag != 3){
					_deltaRqq.push_back(quark1.DeltaR(quark3)); //Distance between quarks in pairs
					_deltaRqq.push_back(quark2.DeltaR(quark4));
					_deltaRqqqq=quark13.DeltaR(quark24); //Distance between quark pairs
					_deltaPhiqqqq=quark13.DeltaPhi(quark24);
					float deltaPhi1= quark1.DeltaPhi(quark3);
		      float deltaPhi2= quark2.DeltaPhi(quark4);
		      float deltaTheta1 = abs(quark1.Theta()-quark3.Theta());
		      float deltaTheta2 = abs(quark2.Theta()-quark4.Theta());
					_deltaPhiqq.push_back(deltaPhi1);
		      _deltaPhiqq.push_back(deltaPhi2);
					_deltaTheta.push_back( deltaTheta1 );
		      _deltaTheta.push_back( deltaTheta2 );
		      _deltaRtheta.push_back( TMath::Sqrt(deltaTheta1*deltaTheta1+deltaPhi1*deltaPhi1) );
		      _deltaRtheta.push_back( TMath::Sqrt(deltaTheta2*deltaTheta2+deltaPhi2*deltaPhi2) );
					_mcFatJet1=quark13;
					_mcFatJet2=quark24;
					if(_WZtag == 0){		
						_fatJetMassZ.push_back(_fatJetMass.at(0));
						_fatJetMassZ.push_back(_fatJetMass.at(1));	
						if(_use_4jet){
							_smallJetMassZ.push_back(_smallJetMassComb.at(0));
							_smallJetMassZ.push_back(_smallJetMassComb.at(1));
						}
					}	
					if(_WZtag == 1){
						_fatJetMassW.push_back(_fatJetMass.at(0));
						_fatJetMassW.push_back(_fatJetMass.at(1));	
						if(_use_4jet){
							_smallJetMassW.push_back(_smallJetMassComb.at(0));
							_smallJetMassW.push_back(_smallJetMassComb.at(1));
						}
					}
					eventCharge = 3;
					eventFlavour = -3;
					_WZtag = 3;		
					if(_tm_out) std::cout << "chose quark13 + quark24    " << "deltaRqq= " << _deltaRqq.at(0) << " " << _deltaRqq.at(1) <<  std::endl;
				}			
			}
		}
		//option 3
		if( (quark14.M() <= (_z_mass+10*_z_width)) && (quark14.M() >= (_w_mass-10*_w_width)) ){
			if( (quark23.M() <= (_z_mass+10*_z_width)) && (quark23.M() >= (_w_mass-10*_w_width)) ){
				if( (fabs(charge1+charge4) == 1) && (fabs(charge2+charge3) == 1) ) eventCharge = 1;
				if( (fabs(charge1+charge4) == 0) && (fabs(charge2+charge3) == 0) ) eventCharge = 0;
				if( (fabs(flavour1+flavour4) != 0) && (fabs(flavour2+flavour3) != 0) ) eventFlavour = 1;
				if( (fabs(flavour1+flavour4) == 0) && (fabs(flavour2+flavour3) == 0) ) eventFlavour = 0;
				if( (eventCharge == 1) && (eventFlavour == 1) ) _WZtag = 1;
				if( (eventCharge == 0) && (eventFlavour == 0) ) _WZtag = 0;
				if( _WZtag != 3){
					_deltaRqq.push_back(quark1.DeltaR(quark4)); //Distance between quarks in pairs
					_deltaRqq.push_back(quark2.DeltaR(quark3));
					_deltaRqqqq=quark14.DeltaR(quark23); //Distance between quark pairs
					_deltaPhiqqqq=quark14.DeltaPhi(quark23);
					float deltaPhi1= quark1.DeltaPhi(quark4);
		      float deltaPhi2= quark2.DeltaPhi(quark3);
		      float deltaTheta1 = abs(quark1.Theta()-quark4.Theta());
		      float deltaTheta2 = abs(quark2.Theta()-quark3.Theta());
					_deltaPhiqq.push_back(deltaPhi1);
				  _deltaPhiqq.push_back(deltaPhi2);
				  _deltaTheta.push_back( deltaTheta1 );
				  _deltaTheta.push_back( deltaTheta2 );
				  _deltaRtheta.push_back( TMath::Sqrt(deltaTheta1*deltaTheta1+deltaPhi1*deltaPhi1) );
				  _deltaRtheta.push_back( TMath::Sqrt(deltaTheta2*deltaTheta2+deltaPhi2*deltaPhi2) );
					_mcFatJet1=quark14;
					_mcFatJet2=quark23;
					if(_WZtag == 0){		
						_fatJetMassZ.push_back(_fatJetMass.at(0));
						_fatJetMassZ.push_back(_fatJetMass.at(1));	
						if(_use_4jet){
							_smallJetMassZ.push_back(_smallJetMassComb.at(0));
							_smallJetMassZ.push_back(_smallJetMassComb.at(1));
						}
					}	
					if(_WZtag == 1){
						_fatJetMassW.push_back(_fatJetMass.at(0));
						_fatJetMassW.push_back(_fatJetMass.at(1));	
						if(_use_4jet){
							_smallJetMassW.push_back(_smallJetMassComb.at(0));
							_smallJetMassW.push_back(_smallJetMassComb.at(1));
						}
					}
					eventCharge = 3;
					eventFlavour = -3;
					_WZtag = 3;		
					if(_tm_out) std::cout << "chose quark14 + quark23    " << "deltaRqq= " << _deltaRqq.at(0) << " " << _deltaRqq.at(1) <<  std::endl;
				}
			}
		}
	
		if(_tm_out){
		  std::cout << "mass12 " << ": " << quark12.M() << "     mass34 " << ": " << quark34.M()  << std::endl;
		  std::cout << "mass13 " << ": " << quark13.M() << "     mass24 " << ": " << quark24.M()  << std::endl;
		  std::cout << "mass14 " << ": " << quark14.M() << "     mass23 " << ": " << quark23.M()  << std::endl;
		
		}

		if(_tm_out){
		  std::cout << "charge1 = " << charge1 << "     " << "charge2 = " << charge2 << "     " << "charge3 = " << charge3 << "     " << "charge4 = " 			<< charge4 << "     " << std::endl;	

		  std::cout << "flavour1 = " << flavour1 << "     " << "flavour2 = " << flavour2 << "     " << "flavour3 = " << flavour3 << "     " << "flavour4 = " 			<< flavour4 << "     " << std::endl;	
		}				
	}//end monte carlo level

	std::vector<TLorentzVector> tempMCjets;
	std::vector<float> tempMCjetCharge;
	std::vector<float> tempMCjetFlavour;

	//single WZ tag
	if( (_fatJetMassW.size() == 0) && (_fatJetMassZ.size() == 0) ){

		if( (quark12.M() <= (_z_mass+10*_z_width)) && (quark12.M() >= (_w_mass-10*_w_width)) ){ 
			tempMCjets.push_back(quark12);
			tempMCjetCharge.push_back(charge1 + charge2);
			tempMCjetFlavour.push_back(flavour1 + flavour2);
		}
		if( (quark23.M() <= (_z_mass+10*_z_width)) && (quark23.M() >= (_w_mass-10*_w_width)) ){ 
			tempMCjets.push_back(quark23);
			tempMCjetCharge.push_back(charge2 + charge3);
			tempMCjetFlavour.push_back(flavour2 + flavour3);
		}
		if( (quark13.M() <= (_z_mass+10*_z_width)) && (quark13.M() >= (_w_mass-10*_w_width)) ){ 
			tempMCjets.push_back(quark13);
			tempMCjetCharge.push_back(charge1 + charge3);
			tempMCjetFlavour.push_back(flavour1 + flavour3);
		}
		if( (quark24.M() <= (_z_mass+10*_z_width)) && (quark24.M() >= (_w_mass-10*_w_width)) ){ 
			tempMCjets.push_back(quark24);
			tempMCjetCharge.push_back(charge2 + charge4);
			tempMCjetFlavour.push_back(flavour2 + flavour4);
		}
		if( (quark14.M() <= (_z_mass+10*_z_width)) && (quark14.M() >= (_w_mass-10*_w_width)) ){ 
			tempMCjets.push_back(quark14);
			tempMCjetCharge.push_back(charge1 + charge4);
			tempMCjetFlavour.push_back(flavour1 + flavour4);
		}
		if( (quark34.M() <= (_z_mass+10*_z_width)) && (quark34.M() >= (_w_mass-10*_w_width)) ){ 
			tempMCjets.push_back(quark34);
			tempMCjetCharge.push_back(charge3 + charge4);
			tempMCjetFlavour.push_back(flavour3 + flavour4);
		}	

		for(unsigned int i=0; i<tempMCjets.size(); i++){	
			if( (fabs(tempMCjetCharge.at(i)) == 1) && (fabs(tempMCjetFlavour.at(i)) != 0) ) _singleWZtag = 1;
			if( (fabs(tempMCjetCharge.at(i)) == 0) && (fabs(tempMCjetFlavour.at(i)) == 0) ) _singleWZtag = 0;
			if(jet1.DeltaR(tempMCjets.at(i)) < jet2.DeltaR(tempMCjets.at(i))){
				if( (_singleWZtag == 1) && (_fatJetMassW.size()==0) ) {_fatJetMassW.push_back(jet1.M()); _singleWZtag = 3;}
				if( (_singleWZtag == 0) && (_fatJetMassZ.size()==0) ) {_fatJetMassZ.push_back(jet1.M()); _singleWZtag = 3;}
			}
			else{
				if( (_singleWZtag == 1) && (_fatJetMassW.size()==0) ) {_fatJetMassW.push_back(jet2.M()); _singleWZtag = 3;}
			  if( (_singleWZtag == 0) && (_fatJetMassZ.size()==0) ) {_fatJetMassZ.push_back(jet2.M()); _singleWZtag = 3;}
			}
			if(_smallJetComb1.DeltaR(tempMCjets.at(i)) < _smallJetComb2.DeltaR(tempMCjets.at(i))){
				if( (_singleWZtag == 1) && (_fatJetMassW.size()==0) ) {_fatJetMassW.push_back(_smallJetComb1.M()); _singleWZtag = 3;}
				if( (_singleWZtag == 0) && (_fatJetMassZ.size()==0) ) {_fatJetMassZ.push_back(_smallJetComb1.M()); _singleWZtag = 3;}
			}
			else{
				if( (_singleWZtag == 1) && (_fatJetMassW.size()==0) ) {_fatJetMassW.push_back(_smallJetComb2.M()); _singleWZtag = 3;}
			  if( (_singleWZtag == 0) && (_fatJetMassZ.size()==0) ) {_fatJetMassZ.push_back(_smallJetComb2.M()); _singleWZtag = 3;}
			}
		} 
	}
	if(_tm_out) std::cout << "**************** End of event ****************" << std::endl;  

// _smallJetComb1
  _nEvt++ ;
  _tree->Fill(); 
} //{end processEvent

void TagWWstoringProcessor::end(){
  _out->Write();
  _out->Close();
  delete _out;
} //end end

void TagWWstoringProcessor::clearVectors(){

  _jetCharge.clear();
  _jetType.clear();
  _fatJetPt.clear();
  _jetEnergy.clear();
  _jetEta.clear();
  _jetMass.clear();
  _jetTheta.clear();
  _jetPhi.clear();
  _fatJetCol.clear();
  _fatJetColTrimmed.clear();
	_smallJetCol.clear();
  _fatJetMass.clear();
	
	_smallJetMass.clear();
	_smallJetPt.clear();
	_smallJetMassComb.clear();
	_smallJetPtComb.clear();
	
	_deltaPhiqq.clear();
	_deltaRqq.clear();

	_smallJets.clear();
	_vecJetQuark.clear();

	_smallJetMassW.clear();
  _smallJetMassZ.clear();

  _fatJetMassW.clear();
  _fatJetMassZ.clear();
 	_deltaTheta.clear();
  _deltaRtheta.clear();
	
	for(unsigned int i=0; i<_particlesInEvent.size(); i++){	
		_particlesInEvent.at(i).clear();	
	}
	_particlesInEvent.clear();

} //end clearVectors

void TagWWstoringProcessor::clearNumbers(){

  _nPFO = 0;

	_WZtag = 3;
	_singleWZtag = 3;
	
  _minPt = 0;
  _Et = 0;
  _ev_inv_mass = 0;

	charge1 = 0;
	charge2 = 0;
	charge3 = 0;
	charge4 = 0;

	jet1.SetPtEtaPhiM(0.,0.,0.,0.);
	jet2.SetPtEtaPhiM(0.,0.,0.,0.);
	trimmedJet1.SetPtEtaPhiM(0.,0.,0.,0.);
	trimmedJet2.SetPtEtaPhiM(0.,0.,0.,0.);
	smalljet1.SetPtEtaPhiM(0.,0.,0.,0.);
	smalljet2.SetPtEtaPhiM(0.,0.,0.,0.);
	smalljet3.SetPtEtaPhiM(0.,0.,0.,0.);
	smalljet4.SetPtEtaPhiM(0.,0.,0.,0.);
	
	quark1.SetPtEtaPhiM(0.,0.,0.,0.);
	quark2.SetPtEtaPhiM(0.,0.,0.,0.);
	quark3.SetPtEtaPhiM(0.,0.,0.,0.);
	quark4.SetPtEtaPhiM(0.,0.,0.,0.);

	quark12.SetPtEtaPhiM(0.,0.,0.,0.);
	quark34.SetPtEtaPhiM(0.,0.,0.,0.);
	quark13.SetPtEtaPhiM(0.,0.,0.,0.);
	quark24.SetPtEtaPhiM(0.,0.,0.,0.);
	quark14.SetPtEtaPhiM(0.,0.,0.,0.);
	quark34.SetPtEtaPhiM(0.,0.,0.,0.);

	_mcFatJet1.SetPtEtaPhiM(0.,0.,0.,0.);
	_mcFatJet2.SetPtEtaPhiM(0.,0.,0.,0.);


	_deltaRjj = 0;
	_deltaPhijj = 0;
	_deltaRqqqq = 0;
	_deltaPhiqqqq = 0;


} //end clearNumbers

template <class T>
  void TagWWstoringProcessor::getKinematics(T*& obj, std::vector<float>& objCharge, std::vector<float>& objType, std::vector<float>& objPt, std::vector<float>& objEnergy, std::vector<float>& objEta, std::vector<float>& objMass, std::vector<float>& objTheta, std::vector<float>& objPhi){
  objCharge.push_back(obj->getCharge());
  objType.push_back(obj->getType());
  double energy = obj->getEnergy();
  const double* mom = obj->getMomentum();
  TLorentzVector objtlv;
  objtlv.SetPxPyPzE(mom[0],mom[1],mom[2],energy);
  objPt.push_back(objtlv.Pt());
  objEnergy.push_back(objtlv.E());
  objEta.push_back(objtlv.Eta());
  objMass.push_back(objtlv.M());
  objTheta.push_back(objtlv.Theta());
  objPhi.push_back(objtlv.Phi());
} //end getKinematics

int TagWWstoringProcessor::getChoose(int n, int k) {
  if (k == 0) { return 1; }
  return (n * getChoose(n - 1, k - 1)) / k;
}//end getChoose

void TagWWstoringProcessor::getCombinations(int offset, int k, std::vector<int>& o, std::vector<int>& c, std::vector<std::vector<int> >& cs){
  if (k == 0) {
    cs.push_back(c);
    return;
  }
  for (size_t i=offset; i<=o.size()-k; ++i) {
    c.push_back(o[i]);
    getCombinations(i+1, k-1, o, c, cs);
    c.pop_back();
  }
}

bool TagWWstoringProcessor::isElectron(int& id){ return fabs(id) == _pdg_electron; } //end isElectron  

template <typename T>
double TagWWstoringProcessor::getEventInvMass(std::vector<T>& v){
  TLorentzVector tv;
  for (typename std::vector<T>::iterator iter=v.begin(); iter!=v.end(); ++iter){
    TLorentzVector tvi;
    const double* mom = (*iter)->getMomentum();
    tvi.SetPxPyPzE(mom[0], mom[1], mom[2], (*iter)->getEnergy());
    tv += tvi;
  }
  return tv.M();
}

bool TagWWstoringProcessor::isISR(std::vector<EVENT::MCParticle*>& mcp){
  std::vector<int> pdg;
  for(size_t i=0; i<mcp.size(); i++){
    MCParticle* p = dynamic_cast<EVENT::MCParticle*>(mcp.at(i));
    pdg.push_back(fabs(p->getPDG()));
  }
  int nPhotons = std::count(pdg.begin(), pdg.end(), _pdg_photon);
  if (nPhotons == 2){
    return 1;
  }
  return 0;
}//end isISR  

void TagWWstoringProcessor::convertFromRecParticle(LCCollection* recCol)
{
  // foreach RecoParticle in the LCCollection: convert it into a PseudoJet and and save it in our list
  for (int i = 0; i < recCol->getNumberOfElements(); ++i)
    {
      ReconstructedParticle* par = dynamic_cast<ReconstructedParticle*> (recCol->getElementAt(i));
      _pjList.push_back(
			fastjet::PseudoJet( par->getMomentum()[0],
					    par->getMomentum()[1],
					    par->getMomentum()[2],
					    par->getEnergy() ) );
      _pjList.back().set_user_index(i);	// save the id of this recParticle
    }
}


bool TagWWstoringProcessor::isQuark(std::vector<MCParticle*> daughters){
	for(unsigned int i=0;i<daughters.size();i++){
		if ( fabs( daughters.at(i)->getPDG() ) > 6 ) return 0;	
	}
	return 1;
}

TLorentzVector TagWWstoringProcessor::recoToTLV(ReconstructedParticle* particle){	
	TLorentzVector temp;
	temp.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
	return temp;
}

TLorentzVector TagWWstoringProcessor::mcToTLV(MCParticle* particle){	
	TLorentzVector temp;
	temp.SetPxPyPzE(particle->getMomentum()[0],particle->getMomentum()[1],particle->getMomentum()[2],particle->getEnergy());
	return temp;
}

template <class T>
void TagWWstoringProcessor::pretty_print(std::vector<T>& v){
  std::cout << " [ ";
  for (size_t i = 0; i < v.size(); ++i) { std::cout << v[i] << " "; }
  std::cout << "] " << std::endl;
}
