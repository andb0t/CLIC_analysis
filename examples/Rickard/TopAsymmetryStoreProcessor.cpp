/*
 * TopAsymmetryStoreProcessor.cpp
 *
 *  Created on: 22.11.2016
 *      Author: Rickard Stroem (CERN) - lars.rickad.stroem@cern.ch
 *           ROOT Booking Processor for Top Asymmetry Analysis
 */

#include <TopAsymmetryStoreProcessor.h>

#include <IMPL/LCCollectionVec.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/MCParticle.h>
#include <EVENT/LCRelation.h>
#include "UTIL/LCRelationNavigator.h"
#include <EVENT/LCGenericObject.h>
#include <UTIL/PIDHandler.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <algorithm>

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>

typedef std::pair<TLorentzVector, TLorentzVector> pairTlv2Tlv;

TopAsymmetryStoreProcessor aTopAsymmetryStoreProcessor;

TopAsymmetryStoreProcessor::TopAsymmetryStoreProcessor() : Processor("TopAsymmetryStoreProcessor"),
							   _mcpColName( "" ),
							   _recoMCTruthLinkName( "" ),
							   _generalParticleInName( "" ),

							   _isolepParticleElectronInName( "" ),
							   _isolepParticleMuonInName( "" ),
							   _isolepParticleInName( "" ),

							   _toptaggerJetInName( "" ),
							   _toptaggerInName( "" ),
							   _toptaggerInName_W( "" ),
							   _toptaggerInName_nonW( "" ),
							   _toptaggerInName_W1( "" ),
							   _toptaggerInName_W2( "" ),
							   _toptaggerInName_cos_theta_W( "" ),

							   _toptaggerJetInName_v2( "" ),
							   _toptaggerInName_v2( "" ),
							   _toptaggerInName_v2_W( "" ),
							   _toptaggerInName_v2_nonW( "" ),
							   _toptaggerInName_v2_W1( "" ),
							   _toptaggerInName_v2_W2( "" ),
							   _toptaggerInName_v2_cos_theta_W( "" ),

							   _jetInNameJ4( "" ),
							   _jetInNameJ4_v2( "" ),
							   _jetInNameJ6( "" ),
							   _jetInNameJ6_v2( "" ),

							   _vertexJetsInName( "" ),
							   _vertexJets3InName( "" ),

							   _outFileName( "" ),
							   _treeName( "" ),

							   _useSixFermionSample( false ),

							   _out( NULL ),
							   _tree( NULL ),
							   
							   _nRun( 0 ),
							   _nEvt( 0 ),  

							   _Et( 0. ), 
							   _minPt( 0. ), 							   
							   _nPFO( 0 ),
							   _nPhotons( 0 ),
							   _nPhotonswoCut( 0 ),

							   _nMCP( 0 ),
							   _eventInvMass( 0. ), 

							   _mcLepType( 0 ),
							   _singleTop( false ),  
							   _compatibleWith6f( false ),  

							   _nTopSideLep( 0 ),
							   _nTopSideNu( 0 ),
							   _nTopSidebQuark( 0 ),
							   _nTopSideLightQuark( 0 ),
							   _topLeptonVSbAngle( 0. ),
							   _topLeptonVSWAngle( 0. ),
							   _topNeutrinoVSWAngle( 0. ),
							   _topbQuarkVSWAngle( 0. ),
							   
							   _nAntiTopSideLep( 0 ),
							   _nAntiTopSideNu( 0 ),
							   _nAntiTopSidebQuark( 0 ),
							   _nAntiTopSideLightQuark( 0 ),
							   _antiTopLeptonVSbAngle( 0. ),
							   _antiTopLeptonVSWAngle( 0. ),
							   _antiTopNeutrinoVSWAngle( 0. ),
							   _antiTopbQuarkVSWAngle( 0. ),

							   _vGenBeamCharge( 0. ),
							   _vGenBeamEnergy( 0. ),
							   _vGenBeamEta( 0. ),
							   _vGenBeamGenStatus( 0 ),
							   _vGenBeamM( 0. ),
							   _vGenBeamPDG( 0 ),
							   _vGenBeamPhi( 0. ),
							   _vGenBeamPt( 0. ),
							   _vGenBeamSimStatus( 0 ),
							   _vGenBeamTheta( 0. ),

							   _vRecoPt(0),
							   _vRecoEta( 0 ),
							   _vRecoPhi( 0 ),
							   _vRecoMass( 0 ),
							   _vRecoTheta( 0 ),
							   _vRecoType( 0 ),
							   _vRecoCharge( 0 ),
							   _vRecoEnergy( 0 ),

							   _vRecoECal( 0 ),
							   _vRecoRCal( 0 ),
							   _vRecoEnergyByMom( 0 ),

							   _vRecoD0( 0 ),
							   _vRecoZ0( 0 ),
							   _vRecoR0( 0 ),
							   _vRecoNTrkObj( 0 ),

							   _vRecoECone900( 0 ),
							   _vRecoECone950( 0 ),
							   _vRecoECone990( 0 ),
							   _vRecoECone995( 0 ),
							   _vRecoECone999( 0 ),
							   _vRecoECone9995( 0 ),

							   _nRelObjIsLep( 0 ),
							   _vNRelObjMC( 0 ),
							   _vRelObjIsLep( 0 ),
							   _vRecoIsCorrectLepton( 0 ),

							   _nRelObjIsPhoton( 0 ),
							   _vNRelObjMCPhoton( 0 ),
							   _vRelObjIsPhoton( 0 ),
							   _vRecoIsCorrectPhoton( 0 ),

							   _nLeptons( 0 ),
							   _nElectron( 0 ),
							   _nMuon( 0 ),
							   _nJets( 0 ),
							   _nJets_v2( 0 ),
							   _nJetsJ4( 0 ),							   
							   _nJetsJ4_v2( 0 ),							   
							   _nJetsJ6( 0 ),
							   _nJetsJ6_v2( 0 ),
							   _nTopTagged( 0 ),
							   _nTopTagged_v2( 0 ),

							   _jetMassTot( 0. ),
							   _jetMassHigh2Pt( 0. ),
							   _jetMassTot_v2( 0. ),
							   _jetMassHigh2Pt_v2( 0. ),
							   _jetMassJ4Tot( 0. ),
							   _jetMassJ4High2Pt( 0. ),
							   _jetMassJ4Tot_v2( 0. ),
							   _jetMassJ4High2Pt_v2( 0. ),
							   _jetMassJ6Tot( 0. ),
							   _jetMassJ6High2Pt( 0. ),
							   _jetMassJ6Tot_v2( 0. ),
							   _jetMassJ6High2Pt_v2( 0. ),

							   _sumBTag( 0. ),
							   _sumCTag( 0. ),
							   _sumBCTag( 0. ),
							   _y23( 0. ),
							   _y34( 0. ),
							   _y45( 0. ),
							   _y56( 0. ),

							   _sumBTagJ3( 0. ),
							   _sumCTagJ3( 0. ),
							   _sumBCTagJ3( 0. ),
							   _y23J3( 0. ),
							   _y34J3( 0. ),
							   _y45J3( 0. ),
							   _y56J3( 0. ),

							   _invMassFlavourTag( 0. ),
							   _invMassFlavourTagwCut050( 0. ),
							   _invMassFlavourTagwCut075( 0. ),
							   _invMassFlavourTagwCut090( 0. ),

							   _invMassFlavourTagJ3( 0. ),
							   _invMassFlavourTagJ3wCut050( 0. ),
							   _invMassFlavourTagJ3wCut075( 0. ),
							   _invMassFlavourTagJ3wCut090( 0. ),

							   _pjList( 0 ),
							   _daughters( 0 ),
							   _daughterCombinations( 0 ),
							   _topFermions( 0 ),
							   _antitopFermions( 0 ),

							   _vGenTopCharge( 0 ),
							   _vGenTopType( 0 ),
							   _vGenTopPt( 0 ),
							   _vGenTopEnergy( 0 ),
							   _vGenTopEta( 0 ),
							   _vGenTopMass( 0 ),
							   _vGenTopTheta( 0 ),
							   _vGenTopPhi( 0 ),

							   _vGenLepPt( 0 ),
							   _vGenLepEta( 0 ),
							   _vGenLepPhi( 0 ),
							   _vGenLepMass( 0 ),
							   _vGenLepTheta( 0 ),
							   _vGenLepType( 0 ),
							   _vGenLepCharge( 0 ),
							   _vGenLepEnergy( 0 ),
							   
							   _vLepCharge( 0 ),
							   _vLepType( 0 ),
							   _vLepPt( 0 ),
							   _vLepEnergy( 0 ),
							   _vLepEta( 0 ),
							   _vLepMass( 0 ),
							   _vLepTheta( 0 ),
							   _vLepPhi( 0 ),

							   _vLepElectronCharge( 0 ),
							   _vLepElectronType( 0 ),
							   _vLepElectronPt( 0 ),
							   _vLepElectronEnergy( 0 ),
							   _vLepElectronEta( 0 ),
							   _vLepElectronMass( 0 ),
							   _vLepElectronTheta( 0 ),
							   _vLepElectronPhi( 0 ),

							   _vLepMuonCharge( 0 ),
							   _vLepMuonType( 0 ),
							   _vLepMuonPt( 0 ),
							   _vLepMuonEnergy( 0 ),
							   _vLepMuonEta( 0 ),
							   _vLepMuonMass( 0 ),
							   _vLepMuonTheta( 0 ),
							   _vLepMuonPhi( 0 ),

							   _vPhotonCharge( 0 ),
							   _vPhotonType( 0 ),
							   _vPhotonPt( 0 ),
							   _vPhotonEnergy( 0 ),
							   _vPhotonEta( 0 ),
							   _vPhotonMass( 0 ),
							   _vPhotonTheta( 0 ),
							   _vPhotonPhi( 0 ),

							   _vJetCharge( 0 ),
							   _vJetType( 0 ),
							   _vJetPt( 0 ),
							   _vJetEnergy( 0 ),
							   _vJetEta( 0 ),
							   _vJetMass( 0 ),
							   _vJetTheta( 0 ),
							   _vJetPhi( 0 ),

							   _vJetCharge_v2( 0 ),
							   _vJetType_v2( 0 ),
							   _vJetPt_v2( 0 ),
							   _vJetEnergy_v2( 0 ),
							   _vJetEta_v2( 0 ),
							   _vJetMass_v2( 0 ),
							   _vJetTheta_v2( 0 ),
							   _vJetPhi_v2( 0 ),

							   _vJetChargeJ4( 0 ),
							   _vJetTypeJ4( 0 ),
							   _vJetPtJ4( 0 ),
							   _vJetEnergyJ4( 0 ),
							   _vJetEtaJ4( 0 ),
							   _vJetMassJ4( 0 ),
							   _vJetThetaJ4( 0 ),
							   _vJetPhiJ4( 0 ),
							   _vJetMassJ4Comb2( 0 ),
							   _vJetMassJ4Comb3( 0 ),

							   _vJetChargeJ4_v2( 0 ),
							   _vJetTypeJ4_v2( 0 ),
							   _vJetPtJ4_v2( 0 ),
							   _vJetEnergyJ4_v2( 0 ),
							   _vJetEtaJ4_v2( 0 ),
							   _vJetMassJ4_v2( 0 ),
							   _vJetThetaJ4_v2( 0 ),
							   _vJetPhiJ4_v2( 0 ),
							   _vJetMassJ4Comb2_v2( 0 ),
							   _vJetMassJ4Comb3_v2( 0 ),

							   _vJetChargeJ6( 0 ),
							   _vJetTypeJ6( 0 ),
							   _vJetPtJ6( 0 ),
							   _vJetEnergyJ6( 0 ),
							   _vJetEtaJ6( 0 ),
							   _vJetMassJ6( 0 ),
							   _vJetThetaJ6( 0 ),
							   _vJetPhiJ6( 0 ),							   
							   _vJetMassJ6Comb2( 0 ),
							   _vJetMassJ6Comb3( 0 ),

							   _vJetChargeJ6_v2( 0 ),
							   _vJetTypeJ6_v2( 0 ),
							   _vJetPtJ6_v2( 0 ),
							   _vJetEnergyJ6_v2( 0 ),
							   _vJetEtaJ6_v2( 0 ),
							   _vJetMassJ6_v2( 0 ),
							   _vJetThetaJ6_v2( 0 ),
							   _vJetPhiJ6_v2( 0 ),							   
							   _vJetMassJ6Comb2_v2( 0 ),
							   _vJetMassJ6Comb3_v2( 0 ),

							   _vTopTagged( 0 ),
							   _vTopTaggedWCosTheta( 0 ),

							   _vTopCharge( 0 ),
							   _vTopType( 0 ),
							   _vTopPt( 0 ),
							   _vTopEnergy( 0 ),
							   _vTopEta( 0 ),
							   _vTopMass( 0 ),
							   _vTopTheta( 0 ),
							   _vTopPhi( 0 ),

							   _vTopChargeW( 0 ),
							   _vTopTypeW( 0 ),
							   _vTopPtW( 0 ),
							   _vTopEnergyW( 0 ),
							   _vTopEtaW( 0 ),
							   _vTopMassW( 0 ),
							   _vTopThetaW( 0 ),
							   _vTopPhiW( 0 ),

							   _vTopTagged_v2( 0 ),
							   _vTopTaggedWCosTheta_v2( 0 ),

							   _vTopCharge_v2( 0 ),
							   _vTopType_v2( 0 ),
							   _vTopPt_v2( 0 ),
							   _vTopEnergy_v2( 0 ),
							   _vTopEta_v2( 0 ),
							   _vTopMass_v2( 0 ),
							   _vTopTheta_v2( 0 ),
							   _vTopPhi_v2( 0 ),

							   _vTopChargeW_v2( 0 ),
							   _vTopTypeW_v2( 0 ),
							   _vTopPtW_v2( 0 ),
							   _vTopEnergyW_v2( 0 ),
							   _vTopEtaW_v2( 0 ),
							   _vTopMassW_v2( 0 ),
							   _vTopThetaW_v2( 0 ),
							   _vTopPhiW_v2( 0 ),

							   _vBTag( 0 ),
							   _vCTag( 0 ),
							   _vBCTag( 0 ),

							   _vBTagJ3( 0 ),
							   _vCTagJ3( 0 ),
							   _vBCTagJ3( 0 ),

							   _vJetChargeFlavourTag( 0 ),
							   _vJetTypeFlavourTag( 0 ),
							   _vJetPtFlavourTag( 0 ),
							   _vJetEnergyFlavourTag( 0 ),
							   _vJetEtaFlavourTag( 0 ),
							   _vJetMassFlavourTag( 0 ),
							   _vJetThetaFlavourTag( 0 ),
							   _vJetPhiFlavourTag( 0 ),

							   _vJetChargeFlavourTagJ3( 0 ),
							   _vJetTypeFlavourTagJ3( 0 ),
							   _vJetPtFlavourTagJ3( 0 ),
							   _vJetEnergyFlavourTagJ3( 0 ),
							   _vJetEtaFlavourTagJ3( 0 ),
							   _vJetMassFlavourTagJ3( 0 ),
							   _vJetThetaFlavourTagJ3( 0 ),
							   _vJetPhiFlavourTagJ3( 0 ),

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

							   _topMass( 0. ),
							   _sigmaTopMass( 0. ),
							   
							   _finalStates( 0 )
{
  _description = "Using the FastJet library to identify jets using _";
  
  // the input & output collections
  registerInputCollection(LCIO::MCPARTICLE, "mcParticles",
			  "Name of the MCParticle collection",
			  _mcpColName, std::string("MCParticlesSkimmed"));
  registerInputCollection(LCIO::LCRELATION, "truthlink", "Name of the TruthLink Collection",
			  _recoMCTruthLinkName, std::string("RecoMCTruthLink"));

  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "generalParticleIn", 
			  "A list of all PFO objects",
			  _generalParticleInName, std::string("SelectedPandoraPFANewPFOs"));

  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "isolepParticleElectronIn", 
			  "A list of all leptons found in the isolated lepton finder with electron settings",
			  _isolepParticleElectronInName, std::string("IsoLep"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "isolepParticleMuonIn", 
			  "A list of all leptons found in the isolated lepton finder with muon settings",
			  _isolepParticleMuonInName, std::string("IsoLep"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "isolepParticleIn", 
			  "A list of all leptons found in the isolated lepton finder",
			  _isolepParticleInName, std::string("IsoLep"));

  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerJetIn",
			   "A list of all reconstructed jets",
			   _toptaggerJetInName, std::string("JetOut"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn", 
			   "The top tagger output for each jet",
			   _toptaggerInName, std::string("topTaggerOut"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_W", 
			   "The top tagger output for each jet",
			   _toptaggerInName_W, std::string("topTaggerOut_W"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_nonW", 
			   "The top tagger output for each jet",
			   _toptaggerInName_nonW, std::string("topTaggerOut_nonW"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_W1", 
			   "The top tagger output for each jet",
			   _toptaggerInName_W1, std::string("topTaggerOut_W1"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_W2", 
			   "The top tagger output for each jet",
			   _toptaggerInName_W2, std::string("topTaggerOut_W2"));
  registerInputCollection(LCIO::LCGENERICOBJECT, "topTaggerIn_cos_theta_W", 
			  "The top tagger output for each jet",
			  _toptaggerInName_cos_theta_W, std::string("topTaggerOut_cos_theta_W"));

  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerJetIn_v2",
			   "A list of all reconstructed jets",
			   _toptaggerJetInName_v2, std::string("JetOut"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_v2", 
			   "The top tagger output for each jet",
			   _toptaggerInName_v2, std::string("topTaggerOut"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_v2_W", 
			   "The top tagger output for each jet",
			   _toptaggerInName_v2_W, std::string("topTaggerOut_W"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_v2_nonW", 
			   "The top tagger output for each jet",
			   _toptaggerInName_v2_nonW, std::string("topTaggerOut_nonW"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_v2_W1", 
			   "The top tagger output for each jet",
			   _toptaggerInName_v2_W1, std::string("topTaggerOut_W1"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "topTaggerIn_v2_W2", 
			   "The top tagger output for each jet",
			   _toptaggerInName_v2_W2, std::string("topTaggerOut_W2"));
  registerInputCollection(LCIO::LCGENERICOBJECT, "topTaggerIn_v2_cos_theta_W", 
			  "The top tagger output for each jet",
			  _toptaggerInName_v2_cos_theta_W, std::string("topTaggerOut_cos_theta_W"));


  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "jetInJ4", 
			  "Jet input using 4 excl. jets",
			  _jetInNameJ4, std::string("JetOutJ4"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "jetInJ4_v2", 
			  "Jet input using 4 excl. jets",
			  _jetInNameJ4_v2, std::string("JetOutJ4wTrimming"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "jetInJ6", 
			  "Jet input using 6 excl. jets",
			  _jetInNameJ6, std::string("JetOutJ6"));
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "jetInJ6_v2", 
			  "Jet input using 6 excl. jets",
			  _jetInNameJ6_v2, std::string("JetOutJ6wTrimming"));
  
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "VertexJetCollection",
			  "Name of the Jet Vertex collection",
			  _vertexJetsInName, std::string("RefinedJets"));  
  registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "VertexJet2Collection",
			  "Name of the Jet Vertex collection",
			  _vertexJets3InName, std::string("RefinedJets2"));  

  //Output parameter
  registerProcessorParameter("outFileName", "Name of the output root file",
			     _outFileName, std::string("ana.root"));
  registerProcessorParameter("treeName", "Name of the tree",
			     _treeName, std::string("anatree"));
  registerProcessorParameter("useSixFermionSample", "Set to true if MC is a 6 fermion sample, e.g. 6601",
			     _useSixFermionSample, false);  
}

TopAsymmetryStoreProcessor::~TopAsymmetryStoreProcessor(){
}

void TopAsymmetryStoreProcessor::init(){

  printParameters();

  //Initiate event variables
  _nRun = 0;
  _nEvt = 0;

  //Initate root file
  _out = new TFile(_outFileName.c_str(),"RECREATE");
  
  //Initiate root tree
  _tree = new TTree(_treeName.c_str(),_treeName.c_str());

  //Set up general event branches
  _tree->Branch("Et",&_Et,"Et/D");
  _tree->Branch("minPt",&_minPt,"minPt/D");
  _tree->Branch("nPFO",&_nPFO,"nPFO/I");
  _tree->Branch("nMCP",&_nMCP,"nMCP/I");
  _tree->Branch("eventInvMass",&_eventInvMass,"eventInvMass/D");

  _tree->Branch("singleTop",&_singleTop,"singleTop/O");
  _tree->Branch("compatibleWith6f",&_compatibleWith6f,"compatibleWith6f/O");
  _tree->Branch("mcLepType",&_mcLepType,"mcLepType/I");

  //Generator level information
  _tree->Branch("vGenTopCharge", "std::vector<float>",&_vGenTopCharge);
  _tree->Branch("vGenTopPt", "std::vector<float>",&_vGenTopPt);
  _tree->Branch("vGenTopEnergy", "std::vector<float>",&_vGenTopEnergy);
  _tree->Branch("vGenTopEta", "std::vector<float>",&_vGenTopEta);
  _tree->Branch("vGenTopMass", "std::vector<float>",&_vGenTopMass);
  _tree->Branch("vGenTopTheta", "std::vector<float>",&_vGenTopTheta);
  _tree->Branch("vGenTopPhi", "std::vector<float>",&_vGenTopPhi);

  _tree->Branch("vGenLepCharge","std::vector<float>",&_vGenLepCharge);
  _tree->Branch("vGenLepType","std::vector<int>",&_vGenLepType);
  _tree->Branch("vGenLepPt","std::vector<float>",&_vGenLepPt);
  _tree->Branch("vGenLepEnergy","std::vector<float>",&_vGenLepEnergy);
  _tree->Branch("vGenLepEta","std::vector<float>",&_vGenLepEta);
  _tree->Branch("vGenLepTheta","std::vector<float>",&_vGenLepTheta);
  _tree->Branch("vGenLepPhi","std::vector<float>",&_vGenLepPhi);

  _tree->Branch("nTopSideLep",&_nTopSideLep,"nTopSideLep/I");
  _tree->Branch("nTopSideNu",&_nTopSideNu,"nTopSideNu/I");
  _tree->Branch("nTopSidebQuark",&_nTopSidebQuark,"nTopSidebQuark/I");
  _tree->Branch("nTopSideLightQuark",&_nTopSideLightQuark,"nTopSideLightQuark/I");
  _tree->Branch("topLeptonVSbAngle",&_topLeptonVSbAngle,"topLeptonVSbAngle/F");
  _tree->Branch("topLeptonVSWAngle",&_topLeptonVSWAngle,"topLeptonVSWAngle/F");
  _tree->Branch("topNeutrinoVSWAngle",&_topNeutrinoVSWAngle,"topNeutrinoVSWAngle/F");
  _tree->Branch("topbQuarkVSWAngle",&_topbQuarkVSWAngle,"topbQuarkVSWAngle/F");

  _tree->Branch("nAntiTopSideLep",&_nAntiTopSideLep,"nAntiTopSideLep/I");
  _tree->Branch("nAntiTopSideNu",&_nAntiTopSideNu,"nAntiTopSideNu/I");
  _tree->Branch("nAntiTopSidebQuark",&_nAntiTopSidebQuark,"nAntiTopSidebQuark/I");
  _tree->Branch("nAntiTopSideLightQuark",&_nAntiTopSideLightQuark,"nAntiTopSideLightQuark/I");
  _tree->Branch("antiTopLeptonVSbAngle",&_antiTopLeptonVSbAngle,"antiTopLeptonVSbAngle/F");
  _tree->Branch("antiTopLeptonVSWAngle",&_antiTopLeptonVSWAngle,"antiTopLeptonVSWAngle/F");
  _tree->Branch("antiTopNeutrinoVSWAngle",&_antiTopNeutrinoVSWAngle,"antiTopNeutrinoVSWAngle/F");
  _tree->Branch("antiTopbQuarkVSWAngle",&_antiTopbQuarkVSWAngle,"antiTopbQuarkVSWAngle/F");

  _tree->Branch("vGenBeamCharge","std::vector<float>",&_vGenBeamCharge);
  _tree->Branch("vGenBeamEnergy","std::vector<float>",&_vGenBeamEnergy);
  _tree->Branch("vGenBeamEta","std::vector<float>",&_vGenBeamEta);
  _tree->Branch("vGenBeamGenStatus","std::vector<int>",&_vGenBeamGenStatus);
  _tree->Branch("vGenBeamM","std::vector<float>",&_vGenBeamM);
  _tree->Branch("vGenBeamPDG","std::vector<int>",&_vGenBeamPDG);
  _tree->Branch("vGenBeamPhi","std::vector<float>",&_vGenBeamPhi);
  _tree->Branch("vGenBeamPt","std::vector<float>",&_vGenBeamPt);
  _tree->Branch("vGenBeamSimStatus","std::vector<int>",&_vGenBeamSimStatus);
  _tree->Branch("vGenBeamTheta","std::vector<float>",&_vGenBeamTheta);

  _tree->Branch("vRecoPt","std::vector<float>",&_vRecoPt);
  _tree->Branch("vRecoEta","std::vector<float>",&_vRecoEta);
  _tree->Branch("vRecoPhi","std::vector<float>",&_vRecoPhi);
  _tree->Branch("vRecoMass","std::vector<float>",&_vRecoMass);
  _tree->Branch("vRecoTheta","std::vector<float>",&_vRecoTheta); 
  _tree->Branch("vRecoType","std::vector<int>",&_vRecoType);
  _tree->Branch("vRecoCharge","std::vector<float>",&_vRecoCharge);
  _tree->Branch("vRecoEnergy","std::vector<float>",&_vRecoEnergy);

  _tree->Branch("vRecoECal","std::vector<float>",&_vRecoECal);
  _tree->Branch("vRecoRCal","std::vector<float>",&_vRecoRCal);
  _tree->Branch("vRecoEnergyByMom","std::vector<float>",&_vRecoEnergyByMom);

  _tree->Branch("vRecoD0","std::vector<float>",&_vRecoR0);
  _tree->Branch("vRecoZ0","std::vector<float>",&_vRecoZ0);
  _tree->Branch("vRecoR0","std::vector<float>",&_vRecoR0);
  _tree->Branch("vRecoNTrkObj","std::vector<int>",&_vRecoNTrkObj);

  _tree->Branch("vRecoECone900","std::vector<float>",&_vRecoECone900);
  _tree->Branch("vRecoECone950","std::vector<float>",&_vRecoECone950);
  _tree->Branch("vRecoECone990","std::vector<float>",&_vRecoECone990);
  _tree->Branch("vRecoECone999","std::vector<float>",&_vRecoECone999);
  _tree->Branch("vRecoECone9995","std::vector<float>",&_vRecoECone9995);

  _tree->Branch("nRelObjIsLep",&_nRelObjIsLep,"nRelObjIsLep/I");
  _tree->Branch("vNRelObjMC","std::vector<int>",&_vNRelObjMC);
  _tree->Branch("vRelObjIsLep","std::vector<bool>",&_vRelObjIsLep);
  _tree->Branch("vRecoIsCorrectLepton","std::vector<int>",&_vRecoIsCorrectLepton);

  _tree->Branch("nRelObjIsPhoton",&_nRelObjIsPhoton,"nRelObjIsPhoton/I");
  _tree->Branch("vNRelObjMCPhoton","std::vector<int>",&_vNRelObjMCPhoton);
  _tree->Branch("vRelObjIsPhoton","std::vector<bool>",&_vRelObjIsPhoton);
  _tree->Branch("vRecoIsCorrectPhoton","std::vector<int>",&_vRecoIsCorrectPhoton);
    
  //Set up isolated lepton finder branches
  _tree->Branch("nLeptons",&_nLeptons,"nLeptons/I");
  _tree->Branch("nElectron",&_nElectron,"nElectron/I");
  _tree->Branch("nMuon",&_nMuon,"nMuon/I");
  _tree->Branch("vLepCharge","std::vector<float>",&_vLepCharge);
  _tree->Branch("vLepType","std::vector<int>",&_vLepType);
  _tree->Branch("vLepPt","std::vector<float>",&_vLepPt);
  _tree->Branch("vLepEnergy","std::vector<float>",&_vLepEnergy);
  _tree->Branch("vLepEta","std::vector<float>",&_vLepEta);
  _tree->Branch("vLepTheta","std::vector<float>",&_vLepTheta);
  _tree->Branch("vLepPhi","std::vector<float>",&_vLepPhi);
  _tree->Branch("vLepElectronCharge","std::vector<float>",&_vLepElectronCharge);
  _tree->Branch("vLepElectronType","std::vector<int>",&_vLepElectronType);
  _tree->Branch("vLepElectronPt","std::vector<float>",&_vLepElectronPt);
  _tree->Branch("vLepElectronEnergy","std::vector<float>",&_vLepElectronEnergy);
  _tree->Branch("vLepElectronEta","std::vector<float>",&_vLepElectronEta);
  _tree->Branch("vLepElectronTheta","std::vector<float>",&_vLepElectronTheta);
  _tree->Branch("vLepElectronPhi","std::vector<float>",&_vLepElectronPhi);
  _tree->Branch("vLepMuonCharge","std::vector<float>",&_vLepMuonCharge);
  _tree->Branch("vLepMuonType","std::vector<int>",&_vLepMuonType);
  _tree->Branch("vLepMuonPt","std::vector<float>",&_vLepMuonPt);
  _tree->Branch("vLepMuonEnergy","std::vector<float>",&_vLepMuonEnergy);
  _tree->Branch("vLepMuonEta","std::vector<float>",&_vLepMuonEta);
  _tree->Branch("vLepMuonTheta","std::vector<float>",&_vLepMuonTheta);
  _tree->Branch("vLepMuonPhi","std::vector<float>",&_vLepMuonPhi);

  //Set up photon finder branches
  _tree->Branch("nPhotons",&_nPhotons,"nPhotons/I");
  _tree->Branch("nPhotonswoCut",&_nPhotonswoCut,"nPhotonswoCut/I");
  _tree->Branch("vPhotonCharge","std::vector<float>",&_vPhotonCharge);
  _tree->Branch("vPhotonType","std::vector<int>",&_vPhotonType);
  _tree->Branch("vPhotonPt","std::vector<float>",&_vPhotonPt);
  _tree->Branch("vPhotonEnergy","std::vector<float>",&_vPhotonEnergy);
  _tree->Branch("vPhotonEta","std::vector<float>",&_vPhotonEta);
  _tree->Branch("vPhotonTheta","std::vector<float>",&_vPhotonTheta);
  _tree->Branch("vPhotonPhi","std::vector<float>",&_vPhotonPhi);

  //Set up jet branches
  _tree->Branch("nJets",&_nJets,"nJets/I");
  _tree->Branch("vJetCharge","std::vector<float>",&_vJetCharge);
  _tree->Branch("vJetType","std::vector<int>",&_vJetType);
  _tree->Branch("vJetPt","std::vector<float>",&_vJetPt);
  _tree->Branch("vJetMass","std::vector<float>",&_vJetMass);
  _tree->Branch("vJetEta","std::vector<float>",&_vJetEta);
  _tree->Branch("vJetEnergy","std::vector<float>",&_vJetEnergy);
  _tree->Branch("vJetTheta","std::vector<float>",&_vJetTheta);
  _tree->Branch("vJetPhi","std::vector<float>",&_vJetPhi);
  _tree->Branch("jetMassTot",&_jetMassTot,"jetMassTot/D");
  _tree->Branch("jetMassHigh2Pt",&_jetMassHigh2Pt,"jetMassHigh2Pt/D");

  _tree->Branch("nJets_v2",&_nJets_v2,"nJets_v2/I");
  _tree->Branch("vJetCharge_v2","std::vector<float>",&_vJetCharge_v2);
  _tree->Branch("vJetType_v2","std::vector<int>",&_vJetType_v2);
  _tree->Branch("vJetPt_v2","std::vector<float>",&_vJetPt_v2);
  _tree->Branch("vJetMass_v2","std::vector<float>",&_vJetMass_v2);
  _tree->Branch("vJetEta_v2","std::vector<float>",&_vJetEta_v2);
  _tree->Branch("vJetEnergy_v2","std::vector<float>",&_vJetEnergy_v2);
  _tree->Branch("vJetTheta_v2","std::vector<float>",&_vJetTheta_v2);
  _tree->Branch("vJetPhi_v2","std::vector<float>",&_vJetPhi_v2);
  _tree->Branch("jetMassTot_v2",&_jetMassTot_v2,"jetMassTot_v2/D");
  _tree->Branch("jetMassHigh2Pt_v2",&_jetMassHigh2Pt_v2,"jetMassHigh2Pt_v2/D");

  _tree->Branch("nJetsJ4",&_nJetsJ4,"nJetsJ4/I");
  _tree->Branch("vJetPtJ4","std::vector<float>",&_vJetPtJ4);
  _tree->Branch("vJetMassJ4","std::vector<float>",&_vJetMassJ4);
  _tree->Branch("vJetEtaJ4","std::vector<float>",&_vJetEtaJ4);
  _tree->Branch("vJetEnergyJ4","std::vector<float>",&_vJetEnergyJ4);
  _tree->Branch("vJetThetaJ4","std::vector<float>",&_vJetThetaJ4);
  _tree->Branch("vJetPhiJ4","std::vector<float>",&_vJetPhiJ4);
  _tree->Branch("jetMassJ4Tot",&_jetMassJ4Tot,"jetMassJ4Tot/D");
  _tree->Branch("jetMassJ4High2Pt",&_jetMassJ4High2Pt,"jetMassJ4High2Pt/D");
  _tree->Branch("vJetMassJ4Comb2","std::vector<float>",&_vJetMassJ4Comb2);
  _tree->Branch("vJetMassJ4Comb3","std::vector<float>",&_vJetMassJ4Comb3);

  _tree->Branch("nJetsJ4_v2",&_nJetsJ4_v2,"nJetsJ4_v2/I");
  _tree->Branch("vJetPtJ4_v2","std::vector<float>",&_vJetPtJ4_v2);
  _tree->Branch("vJetMassJ4_v2","std::vector<float>",&_vJetMassJ4_v2);
  _tree->Branch("vJetEtaJ4_v2","std::vector<float>",&_vJetEtaJ4_v2);
  _tree->Branch("vJetEnergyJ4_v2","std::vector<float>",&_vJetEnergyJ4_v2);
  _tree->Branch("vJetThetaJ4_v2","std::vector<float>",&_vJetThetaJ4_v2);
  _tree->Branch("vJetPhiJ4_v2","std::vector<float>",&_vJetPhiJ4_v2);
  _tree->Branch("jetMassJ4Tot_v2",&_jetMassJ4Tot_v2,"jetMassJ4Tot_v2/D");
  _tree->Branch("jetMassJ4High2Pt_v2",&_jetMassJ4High2Pt_v2,"jetMassJ4High2Pt_v2/D");
  _tree->Branch("vJetMassJ4Comb2_v2","std::vector<float>",&_vJetMassJ4Comb2_v2);
  _tree->Branch("vJetMassJ4Comb3_v2","std::vector<float>",&_vJetMassJ4Comb3_v2);

  _tree->Branch("nJetsJ6",&_nJetsJ6,"nJetsJ6/I");
  _tree->Branch("vJetPtJ6","std::vector<float>",&_vJetPtJ6);
  _tree->Branch("vJetMassJ6","std::vector<float>",&_vJetMassJ6);
  _tree->Branch("vJetEtaJ6","std::vector<float>",&_vJetEtaJ6);
  _tree->Branch("vJetEnergyJ6","std::vector<float>",&_vJetEnergyJ6);
  _tree->Branch("vJetThetaJ6","std::vector<float>",&_vJetThetaJ6);
  _tree->Branch("vJetPhiJ6","std::vector<float>",&_vJetPhiJ6);
  _tree->Branch("jetMassJ6Tot",&_jetMassJ6Tot,"jetMassJ6Tot/D");
  _tree->Branch("jetMassJ6High2Pt",&_jetMassJ6High2Pt,"jetMassJ6High2Pt/D");
  _tree->Branch("vJetMassJ6Comb2","std::vector<float>",&_vJetMassJ6Comb2);
  _tree->Branch("vJetMassJ6Comb3","std::vector<float>",&_vJetMassJ6Comb3);

  _tree->Branch("nJetsJ6_v2",&_nJetsJ6_v2,"nJetsJ6_v2/I");
  _tree->Branch("vJetPtJ6_v2","std::vector<float>",&_vJetPtJ6_v2);
  _tree->Branch("vJetMassJ6_v2","std::vector<float>",&_vJetMassJ6_v2);
  _tree->Branch("vJetEtaJ6_v2","std::vector<float>",&_vJetEtaJ6_v2);
  _tree->Branch("vJetEnergyJ6_v2","std::vector<float>",&_vJetEnergyJ6_v2);
  _tree->Branch("vJetThetaJ6_v2","std::vector<float>",&_vJetThetaJ6_v2);
  _tree->Branch("vJetPhiJ6_v2","std::vector<float>",&_vJetPhiJ6_v2);
  _tree->Branch("jetMassJ6Tot_v2",&_jetMassJ6Tot_v2,"jetMassJ6Tot_v2/D");
  _tree->Branch("jetMassJ6High2Pt_v2",&_jetMassJ6High2Pt_v2,"jetMassJ6High2Pt_v2/D");
  _tree->Branch("vJetMassJ6Comb2_v2","std::vector<float>",&_vJetMassJ6Comb2_v2);
  _tree->Branch("vJetMassJ6Comb3_v2","std::vector<float>",&_vJetMassJ6Comb3_v2);

  //Set up top tagger branches
  _tree->Branch("nTopTagged",&_nTopTagged,"nTopTagged/I");
  _tree->Branch("vTopTagged","std::vector<int>",&_vTopTagged);
  _tree->Branch("vTopTaggedWCosTheta","std::vector<float>",&_vTopTaggedWCosTheta);

  _tree->Branch("vTopCharge","std::vector<float>",&_vTopCharge);
  _tree->Branch("vTopPt","std::vector<float>",&_vTopPt);
  _tree->Branch("vTopEnergy","std::vector<float>",&_vTopEnergy);
  _tree->Branch("vTopEta","std::vector<float>",&_vTopEta);
  _tree->Branch("vTopMass","std::vector<float>",&_vTopMass);
  _tree->Branch("vTopTheta","std::vector<float>",&_vTopTheta);
  _tree->Branch("vTopPhi","std::vector<float>",&_vTopPhi);

  _tree->Branch("vTopChargeW","std::vector<float>",&_vTopChargeW);
  _tree->Branch("vTopPtW","std::vector<float>",&_vTopPtW);
  _tree->Branch("vTopEnergyW","std::vector<float>",&_vTopEnergyW);
  _tree->Branch("vTopEtaW","std::vector<float>",&_vTopEtaW);
  _tree->Branch("vTopMassW","std::vector<float>",&_vTopMassW);
  _tree->Branch("vTopThetaW","std::vector<float>",&_vTopThetaW);
  _tree->Branch("vTopPhiW","std::vector<float>",&_vTopPhiW);

  _tree->Branch("nTopTagged_v2",&_nTopTagged_v2,"nTopTagged_v2/I");
  _tree->Branch("vTopTagged_v2","std::vector<int>",&_vTopTagged_v2);
  _tree->Branch("vTopTaggedWCosTheta_v2","std::vector<float>",&_vTopTaggedWCosTheta_v2);

  _tree->Branch("vTopCharge_v2","std::vector<float>",&_vTopCharge_v2);
  _tree->Branch("vTopPt_v2","std::vector<float>",&_vTopPt_v2);
  _tree->Branch("vTopEnergy_v2","std::vector<float>",&_vTopEnergy_v2);
  _tree->Branch("vTopEta_v2","std::vector<float>",&_vTopEta_v2);
  _tree->Branch("vTopMass_v2","std::vector<float>",&_vTopMass_v2);
  _tree->Branch("vTopTheta_v2","std::vector<float>",&_vTopTheta_v2);
  _tree->Branch("vTopPhi_v2","std::vector<float>",&_vTopPhi_v2);

  _tree->Branch("vTopChargeW_v2","std::vector<float>",&_vTopChargeW_v2);
  _tree->Branch("vTopPtW_v2","std::vector<float>",&_vTopPtW_v2);
  _tree->Branch("vTopEnergyW_v2","std::vector<float>",&_vTopEnergyW_v2);
  _tree->Branch("vTopEtaW_v2","std::vector<float>",&_vTopEtaW_v2);
  _tree->Branch("vTopMassW_v2","std::vector<float>",&_vTopMassW_v2);
  _tree->Branch("vTopThetaW_v2","std::vector<float>",&_vTopThetaW_v2);
  _tree->Branch("vTopPhiW_v2","std::vector<float>",&_vTopPhiW_v2);

  //Set up flavour tagger branches
  _tree->Branch("vBTag","std::vector<float>",&_vBTag);
  _tree->Branch("vCTag","std::vector<float>",&_vCTag);
  _tree->Branch("vBCTag","std::vector<float>",&_vBCTag);
  _tree->Branch("sumBTag",&_sumBTag,"sumBTag/D");   
  _tree->Branch("sumCTag",&_sumCTag,"sumCTag/D");   
  _tree->Branch("sumBCTag",&_sumBCTag,"sumBCTag/D");   

  _tree->Branch("y23",&_y23,"y23/D");   
  _tree->Branch("y34",&_y34,"y34/D");   
  _tree->Branch("y45",&_y45,"y45/D");   
  _tree->Branch("y56",&_y56,"y56/D");   

  _tree->Branch("vBTagJ3","std::vector<float>",&_vBTagJ3);
  _tree->Branch("vCTagJ3","std::vector<float>",&_vCTagJ3);
  _tree->Branch("vBCTagJ3","std::vector<float>",&_vBCTagJ3);
  _tree->Branch("sumBTagJ3",&_sumBTagJ3,"sumBTagJ3/D");   
  _tree->Branch("sumCTagJ3",&_sumCTagJ3,"sumCTagJ3/D");   
  _tree->Branch("sumBCTagJ3",&_sumBCTagJ3,"sumBCTagJ3/D");   

  _tree->Branch("y23J3",&_y23J3,"y23J3/D");   
  _tree->Branch("y34J3",&_y34J3,"y34J3/D");   
  _tree->Branch("y45J3",&_y45J3,"y45J3/D");   
  _tree->Branch("y56J3",&_y56J3,"y56J3/D");   

  _tree->Branch("vJetChargeFlavourTag","std::vector<float>",&_vJetChargeFlavourTag);
  _tree->Branch("vJetTypeFlavourTag","std::vector<int>",&_vJetTypeFlavourTag);
  _tree->Branch("vJetPtFlavourTag","std::vector<float>",&_vJetPtFlavourTag);
  _tree->Branch("vJetMassFlavourTag","std::vector<float>",&_vJetMassFlavourTag);
  _tree->Branch("vJetEtaFlavourTag","std::vector<float>",&_vJetEtaFlavourTag);
  _tree->Branch("vJetEnergyFlavourTag","std::vector<float>",&_vJetEnergyFlavourTag);
  _tree->Branch("vJetThetaFlavourTag","std::vector<float>",&_vJetThetaFlavourTag);
  _tree->Branch("vJetPhiFlavourTag","std::vector<float>",&_vJetPhiFlavourTag);
  _tree->Branch("invMassFlavourTag",&_invMassFlavourTag,"invMassFlavourTag/D");
  _tree->Branch("invMassFlavourTagwCut050",&_invMassFlavourTagwCut050,"invMassFlavourTagwCut050/D");
  _tree->Branch("invMassFlavourTagwCut075",&_invMassFlavourTagwCut075,"invMassFlavourTagwCut075/D");
  _tree->Branch("invMassFlavourTagwCut090",&_invMassFlavourTagwCut090,"invMassFlavourTagwCut090/D");

  _tree->Branch("vJetChargeFlavourTagJ3","std::vector<float>",&_vJetChargeFlavourTagJ3);
  _tree->Branch("vJetTypeFlavourTagJ3","std::vector<int>",&_vJetTypeFlavourTagJ3);
  _tree->Branch("vJetPtFlavourTagJ3","std::vector<float>",&_vJetPtFlavourTagJ3);
  _tree->Branch("vJetMassFlavourTagJ3","std::vector<float>",&_vJetMassFlavourTagJ3);
  _tree->Branch("vJetEtaFlavourTagJ3","std::vector<float>",&_vJetEtaFlavourTagJ3);
  _tree->Branch("vJetEnergyFlavourTagJ3","std::vector<float>",&_vJetEnergyFlavourTagJ3);
  _tree->Branch("vJetThetaFlavourTagJ3","std::vector<float>",&_vJetThetaFlavourTagJ3);
  _tree->Branch("vJetPhiFlavourTagJ3","std::vector<float>",&_vJetPhiFlavourTagJ3);
  _tree->Branch("invMassFlavourTagJ3",&_invMassFlavourTagJ3,"invMassFlavourTagJ3/D");
  _tree->Branch("invMassFlavourTagJ3wCut050",&_invMassFlavourTagJ3wCut050,"invMassFlavourTagJ3wCut050/D");
  _tree->Branch("invMassFlavourTagJ3wCut075",&_invMassFlavourTagJ3wCut075,"invMassFlavourTagJ3wCut075/D");
  _tree->Branch("invMassFlavourTagJ3wCut090",&_invMassFlavourTagJ3wCut090,"invMassFlavourTagJ3wCut090/D");

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

  _topMass = 173.21;
  _sigmaTopMass = 1.41;

  //Setting up possible final states

  //Get all y combinations
  std::vector<std::vector<int> > yQuarkCombinations;
  int yChosen[3];
  const int yQuarks[] = {1, -1, 3, -3, 5, -5};
  getQuarkCombinations(yChosen, 0, 3, 0, 6, yQuarks, yQuarkCombinations);

  //Get all x combinations
  std::vector<std::vector<int> > xQuarkCombinations;
  int xChosen[1];
  const int xQuarks[] = {2, -2, 4, -4};  
  getQuarkCombinations(xChosen, 0, 1, 0, 4, xQuarks, xQuarkCombinations);

  //Get all lepton combinations
  std::vector<std::vector<int> > lepCombinations;
  int lepChosen[1];
  const int leptons[] = {11, -11, 13, -13, 15, -15};  
  getQuarkCombinations(lepChosen, 0, 1, 0, 6, leptons, lepCombinations);

  for (std::vector<std::vector<int> >::iterator xQuarkComb = xQuarkCombinations.begin(); xQuarkComb != xQuarkCombinations.end(); ++xQuarkComb){
    for (std::vector<std::vector<int> >::iterator lepComb = lepCombinations.begin(); lepComb != lepCombinations.end(); ++lepComb){
      for (std::vector<std::vector<int> >::iterator yQuarkComb = yQuarkCombinations.begin(); yQuarkComb != yQuarkCombinations.end(); ++yQuarkComb){ 
	std::vector<int> comb6f = *xQuarkComb;
	comb6f.insert(comb6f.end(), (*lepComb).begin(), (*lepComb).end());
	comb6f.insert(comb6f.end(), (*yQuarkComb).begin(), (*yQuarkComb).end());
	float comb6fCharge = getChargeSum(comb6f);
	if ( TMath::Abs(comb6fCharge) <= 0.1 ){
	  //Add neutrinos
	  if ( comb6f.at(1) == 11 ){ comb6f.push_back(-12);
	  } else if ( comb6f.at(1) == -11 ){ comb6f.push_back(12); 
	  } else if ( comb6f.at(1) == 13 ){ comb6f.push_back(-14); 
	  } else if ( comb6f.at(1) == -13 ){ comb6f.push_back(14); 
	  } else if ( comb6f.at(1) == 15 ){ comb6f.push_back(-16); 
	  } else if ( comb6f.at(1) == -15 ){ comb6f.push_back(16); }
	  std::sort(comb6f.begin(), comb6f.end());
	  _finalStates.push_back(comb6f);
	}
      }
    }
  }
  std::cout << "Number of ttbar compatible final states: " << _finalStates.size() << std::endl;
  
} //end init

void TopAsymmetryStoreProcessor::processRunHeader(LCRunHeader*) {
  _nRun++ ;
} //end processRunHeader

void TopAsymmetryStoreProcessor::processEvent(LCEvent * evt)
{

  if(_nEvt%100 == 0) std::cout << "*************  Event " << _nEvt << "  *******************" << std::endl;

  clearVectors();
  clearNumbers();
  
  //General event information
  LCCollection* generalIn = NULL;
  try {
    generalIn = evt->getCollection(_generalParticleInName);
    if (generalIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
    // convert to pseudojet list
    convertFromRecParticle(generalIn);
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  //Calculate minimum pt
  for (unsigned int i=0; i<_pjList.size(); i++)
    _Et += _pjList[i].perp();
  _minPt = std::min(500.0, 0.7*_Et/2);
  
  //Check if we have a photon, save bool and energy
  _nPFO = generalIn->getNumberOfElements();
  for(int i=0; i<_nPFO ; i++){
    ReconstructedParticle* pfo = dynamic_cast<EVENT::ReconstructedParticle*>(generalIn->getElementAt(i));
    if(pfo == NULL){ continue; }
    int pdg = pfo->getType();
    if ( isPhoton(pdg) ){
      _nPhotonswoCut++;
      if ( pfo->getEnergy() >= 5.0 ){ 
	//Save truth link information for each reco photon
	LCCollection* recoMCTruthLink = evt->getCollection(_recoMCTruthLinkName);
	isCorrectPhoton(recoMCTruthLink, pfo, _vNRelObjMCPhoton, _vRelObjIsPhoton, _vRecoIsCorrectPhoton, _nRelObjIsPhoton);
	getKinematics<ReconstructedParticle>(pfo, _vPhotonPt, _vPhotonEta, _vPhotonPhi, _vPhotonMass, _vPhotonTheta, _vPhotonType, _vPhotonCharge, _vPhotonEnergy); 
      }
    }
  }
  _nPhotons = _vPhotonCharge.size();

  //General MC information
  LCCollection* mcpIn = NULL;
  try{
    mcpIn = evt->getCollection( _mcpColName);
    if (mcpIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }

  //Save beam info
  getBeamInfo(mcpIn, _vGenBeamCharge, _vGenBeamEnergy, _vGenBeamEta, _vGenBeamGenStatus, _vGenBeamM, _vGenBeamPDG, _vGenBeamPhi, _vGenBeamPt, _vGenBeamSimStatus, _vGenBeamTheta);
  //std::cout << _vGenBeamTheta.at(4)/3.14*180 << " "  << _vGenBeamTheta.at(5)/3.14*180 << std::endl;
  //Find daughters
  _nMCP = mcpIn->getNumberOfElements();
  EVENT::MCParticle* mcParticle = NULL;
  for(int i=0; i<_nMCP ; i++){
    mcParticle = dynamic_cast<EVENT::MCParticle*>(mcpIn->getElementAt(i));
    if(mcParticle == NULL){ continue; }
    int pdg = fabs(mcParticle->getPDG());
    if (isElectron(pdg) && mcParticle->getParents().size()!=0){ break; }
  }
  _daughters = mcParticle->getDaughters();
  _eventInvMass = getEventInvMass(_daughters);
  //std::cout << "_eventInvMass: " << _eventInvMass << std::endl;

  //Find leptons type
  int nDaughters = _daughters.size();
  for (int j=0; j<nDaughters; j++){
    MCParticle* daughter = _daughters.at(j);
    int pdg = daughter->getPDG();
    if ( isElectron(pdg) || isMuon(pdg) || isTau(pdg) ){
      _mcLepType = pdg;
      //Save true lepton info
      //ReconstructedParticle* genLepton = dynamic_cast<EVENT::ReconstructedParticle*>( daughter );
      getKinematics<MCParticle>(daughter, _vGenLepPt, _vGenLepEta, _vGenLepPhi, _vGenLepMass, _vGenLepTheta, _vGenLepType, _vGenLepCharge, _vGenLepEnergy);
      break; //The first parent candidate is the one we want since ordered in energy
    }
  }

  std::vector<int> daughtersPDG;
  for (std::vector<MCParticle*>::iterator daughterMCP = _daughters.begin(); daughterMCP != _daughters.end(); ++daughterMCP){
    daughtersPDG.push_back((*daughterMCP)->getPDG());
  }
  std::sort(daughtersPDG.begin(), daughtersPDG.end());
  for (std::vector<std::vector<int> >::iterator finalState = _finalStates.begin(); finalState != _finalStates.end(); ++finalState){
    //compare daughtersPDG with finalState
    if ( daughtersPDG == *finalState ){ 
      //std::cout << "Matched found, breaking loop!" << std::endl;
      _compatibleWith6f = true; 
      break;
    }
  }
  
  //TruthMatching - ttbar selection
  if (_useSixFermionSample){
    getDaughterCombinations(_daughters, _daughterCombinations);
    std::vector<std::vector<std::vector<MCParticle*> > > combinationCandidates;
    for (std::vector<std::vector<EVENT::MCParticle*> >::iterator combination = _daughterCombinations.begin(); combination != _daughterCombinations.end(); ++combination){
      double combinationCharge = TMath::Abs(getCombinationCharge(*combination));
      if( (combinationCharge < 0.6) || (combinationCharge > 0.7) ){ continue; }
      if( TMath::Abs(getEventInvMass(*combination) - _topMass) > (5.*_sigmaTopMass) ){ continue; }
      std::vector<EVENT::MCParticle*> otherCombination;
      for (std::vector<EVENT::MCParticle*>::iterator iter = _daughters.begin(); iter!=_daughters.end(); ++iter){ 
	if (std::find(combination->begin(), combination->end(), *iter) == combination->end()) { otherCombination.push_back(*iter); } 
      }      
      if( TMath::Abs(getEventInvMass(otherCombination) - _topMass) > (5.*_sigmaTopMass) ){ continue; }
      std::vector<std::vector<MCParticle*> > combinationCandidate;
      combinationCandidate.push_back(*combination);
      combinationCandidate.push_back(otherCombination);
      combinationCandidates.push_back(combinationCandidate);
    }
    int nCombinationCandidates = combinationCandidates.size();
    if( nCombinationCandidates == 0){ 
      _singleTop = true;
    } else if (nCombinationCandidates == 1){ 
      _singleTop = false;
      if ( getCombinationCharge(combinationCandidates.at(0).at(0)) > 0. ){ 
	_topFermions = combinationCandidates.at(0).at(0); 
	_antitopFermions = combinationCandidates.at(0).at(1);
      } else {
	_topFermions = combinationCandidates.at(0).at(1);
	_antitopFermions = combinationCandidates.at(0).at(0);
      }
    } else {
      std::sort(combinationCandidates.begin(), combinationCandidates.end(), wayToSort);
      _singleTop = false;
      if ( getCombinationCharge(combinationCandidates.at(0).at(0)) > 0. ){ 
	_topFermions = combinationCandidates.at(0).at(0); 
	_antitopFermions = combinationCandidates.at(0).at(1);
      } else {
	_topFermions = combinationCandidates.at(0).at(1);
	_antitopFermions = combinationCandidates.at(0).at(0);
      }
    }
  }
  else { _singleTop = false; }

  TLorentzVector topQuark;
  double topQuarkCharge = getCombinationCharge(_topFermions);
  //std::cout << "topQuarkCharge: " << topQuarkCharge << std::endl;
  for (std::vector<MCParticle*>::iterator iter=_topFermions.begin(); iter!=_topFermions.end(); ++iter){
    TLorentzVector topQuarkPart;
    const double* momTopQuarkPart = (*iter)->getMomentum();
    topQuarkPart.SetPxPyPzE(momTopQuarkPart[0], momTopQuarkPart[1], momTopQuarkPart[2], (*iter)->getEnergy());
    topQuark += topQuarkPart;
  }
  _vGenTopCharge.push_back(topQuarkCharge);
  _vGenTopPt.push_back(topQuark.Pt());
  _vGenTopEnergy.push_back(topQuark.E());
  _vGenTopEta.push_back(topQuark.Eta());
  _vGenTopMass.push_back(topQuark.M());
  _vGenTopTheta.push_back(topQuark.Theta());
  _vGenTopPhi.push_back(topQuark.Phi());

  TLorentzVector antiTopQuark;
  double antiTopQuarkCharge = getCombinationCharge(_antitopFermions);
  for (std::vector<MCParticle*>::iterator iter=_antitopFermions.begin(); iter!=_antitopFermions.end(); ++iter){
    TLorentzVector antiTopQuarkPart;
    const double* momAntiTopQuarkPart = (*iter)->getMomentum();
    antiTopQuarkPart.SetPxPyPzE(momAntiTopQuarkPart[0], momAntiTopQuarkPart[1], momAntiTopQuarkPart[2], (*iter)->getEnergy());
    antiTopQuark += antiTopQuarkPart;
  }
  _vGenTopCharge.push_back(antiTopQuarkCharge);
  _vGenTopPt.push_back(antiTopQuark.Pt());
  _vGenTopEnergy.push_back(antiTopQuark.E());
  _vGenTopEta.push_back(antiTopQuark.Eta());
  _vGenTopMass.push_back(antiTopQuark.M());
  _vGenTopTheta.push_back(antiTopQuark.Theta());
  _vGenTopPhi.push_back(antiTopQuark.Phi());

  //Save angles for top side
  std::vector<TLorentzVector> vTlvTopLepton;
  std::vector<TLorentzVector> vTlvTopNeutrino;
  std::vector<TLorentzVector> vTlvTopbQuark;
  std::vector<TLorentzVector> vTlvTopLightQuark;
  for (std::vector<MCParticle*>::iterator iter=_topFermions.begin(); iter!=_topFermions.end(); ++iter){
    const double* mom = (*iter)->getMomentum();
    TLorentzVector tlv;
    tlv.SetPxPyPzE(mom[0],mom[1],mom[2],(*iter)->getEnergy());
    if ( (TMath::Abs((*iter)->getPDG()) == 11) || (TMath::Abs((*iter)->getPDG()) == 13) || (TMath::Abs((*iter)->getPDG()) == 15) ){
      vTlvTopLepton.push_back(tlv);
    } else if ( (TMath::Abs((*iter)->getPDG()) == 12) || (TMath::Abs((*iter)->getPDG()) == 14) || (TMath::Abs((*iter)->getPDG()) == 16) ){
      vTlvTopNeutrino.push_back(tlv);
    } else if ( (TMath::Abs((*iter)->getPDG()) == 5) ){
      vTlvTopbQuark.push_back(tlv);
    } else {
      vTlvTopLightQuark.push_back(tlv);
    }
  }
  _nTopSideLep = vTlvTopLepton.size();
  _nTopSideNu = vTlvTopNeutrino.size();
  _nTopSidebQuark = vTlvTopbQuark.size();
  _nTopSideLightQuark = vTlvTopLightQuark.size();

  if ( (_nTopSideLep == 1) && (_nTopSideNu == 1) && (_nTopSidebQuark == 1) ){ //semi-leptonic
    TLorentzVector tlvW = vTlvTopLepton.at(0) + vTlvTopNeutrino.at(0);
    _topLeptonVSbAngle = vTlvTopLepton.at(0).Angle(vTlvTopbQuark.at(0).Vect()); //angle between lepton and b quark
    _topLeptonVSWAngle = vTlvTopLepton.at(0).Angle(tlvW.Vect()); //angle between lepton and W
    _topNeutrinoVSWAngle = vTlvTopNeutrino.at(0).Angle(tlvW.Vect()); //angle between lepton and W
    _topbQuarkVSWAngle = vTlvTopbQuark.at(0).Angle(tlvW.Vect()); //angle between b and W
    
  } else if ( (_nTopSideLep == 1) && (_nTopSideNu == 1) && (_nTopSidebQuark == 0) && (_nTopSideLightQuark == 1) ){ //semi-leptonic
    TLorentzVector tlvW = vTlvTopLepton.at(0) + vTlvTopNeutrino.at(0);
    _topLeptonVSbAngle = vTlvTopLepton.at(0).Angle(vTlvTopLightQuark.at(0).Vect()); //angle between lepton and b quark
    _topLeptonVSWAngle = vTlvTopLepton.at(0).Angle(tlvW.Vect()); //angle between lepton and W
    _topNeutrinoVSWAngle = vTlvTopNeutrino.at(0).Angle(tlvW.Vect()); //angle between lepton and W
    _topbQuarkVSWAngle = vTlvTopLightQuark.at(0).Angle(tlvW.Vect()); //angle between b and W
    
  } else if ( (_nTopSidebQuark == 1) && (_nTopSideLightQuark == 2) ){ //hadronic w. 1 b-quark
    TLorentzVector tlvW = vTlvTopLightQuark.at(0) + vTlvTopLightQuark.at(1);
    _topbQuarkVSWAngle = vTlvTopbQuark.at(0).Angle(tlvW.Vect()); 
    
  } else if ( _nTopSidebQuark == 3 ){ //hadronic w. 3 b-quark
    std::vector<pairTlv2Tlv> vPairTlv2TlvW;
    TLorentzVector tlvWv01 = vTlvTopLightQuark.at(0) + vTlvTopLightQuark.at(1); vPairTlv2TlvW.push_back(std::make_pair(tlvWv01, vTlvTopLightQuark.at(2)));
    TLorentzVector tlvWv02 = vTlvTopLightQuark.at(0) + vTlvTopLightQuark.at(2); vPairTlv2TlvW.push_back(std::make_pair(tlvWv02, vTlvTopLightQuark.at(1)));
    TLorentzVector tlvWv12 = vTlvTopLightQuark.at(1) + vTlvTopLightQuark.at(2); vPairTlv2TlvW.push_back(std::make_pair(tlvWv12, vTlvTopLightQuark.at(0)));
    //for (std::vector<pairTlv2Tlv>::iterator iter1=vPairTlv2TlvW.begin(); iter1!=vPairTlv2TlvW.end(); ++iter1){
    //std::cout << "Mass of pair: " << (*iter1).first.M() << std::endl;
    //}
    std::sort(vPairTlv2TlvW.begin(), vPairTlv2TlvW.end(),  wayToSortWv);
    TLorentzVector tlvW = vPairTlv2TlvW.at(0).first;
    //std::cout << "Mass of selected W: " << tlvW.M() << std::endl;
     _topbQuarkVSWAngle = vPairTlv2TlvW.at(0).second.Angle(tlvW.Vect()); 
  }	  
  
  //Save angles for anti-top side
  std::vector<TLorentzVector> vTlvAntiTopLepton;
  std::vector<TLorentzVector> vTlvAntiTopNeutrino;
  std::vector<TLorentzVector> vTlvAntiTopbQuark;
  std::vector<TLorentzVector> vTlvAntiTopLightQuark;
  for (std::vector<MCParticle*>::iterator iter=_antitopFermions.begin(); iter!=_antitopFermions.end(); ++iter){
    const double* mom = (*iter)->getMomentum();
    TLorentzVector tlv;
    tlv.SetPxPyPzE(mom[0],mom[1],mom[2],(*iter)->getEnergy());
    if ( (TMath::Abs((*iter)->getPDG()) == 11) || (TMath::Abs((*iter)->getPDG()) == 13) || (TMath::Abs((*iter)->getPDG()) == 15) ){
      vTlvAntiTopLepton.push_back(tlv);
    } else if ( (TMath::Abs((*iter)->getPDG()) == 12) || (TMath::Abs((*iter)->getPDG()) == 14) || (TMath::Abs((*iter)->getPDG()) == 16) ){
      vTlvAntiTopNeutrino.push_back(tlv);
    } else if ( (TMath::Abs((*iter)->getPDG()) == 5) ){
      vTlvAntiTopbQuark.push_back(tlv);
    } else {
      vTlvAntiTopLightQuark.push_back(tlv);
    }
  }
  _nAntiTopSideLep = vTlvAntiTopLepton.size();
  _nAntiTopSideNu = vTlvAntiTopNeutrino.size();
  _nAntiTopSidebQuark = vTlvAntiTopbQuark.size();
  _nAntiTopSideLightQuark = vTlvAntiTopLightQuark.size();

  if ( (_nAntiTopSideLep == 1) && (_nAntiTopSideNu == 1) && (_nAntiTopSidebQuark == 1) ){ //semi-leptonic
    TLorentzVector tlvW = vTlvAntiTopLepton.at(0) + vTlvAntiTopNeutrino.at(0);
    _antiTopLeptonVSbAngle = vTlvAntiTopLepton.at(0).Angle(vTlvAntiTopbQuark.at(0).Vect()); //angle between lepton and b quark
    _antiTopLeptonVSWAngle = vTlvAntiTopLepton.at(0).Angle(tlvW.Vect()); //angle between lepton and W
    _antiTopNeutrinoVSWAngle = vTlvAntiTopNeutrino.at(0).Angle(tlvW.Vect()); //angle between lepton and W
    _antiTopbQuarkVSWAngle = vTlvAntiTopbQuark.at(0).Angle(tlvW.Vect()); //angle between b and W
    
  } else if ( (_nAntiTopSideLep == 1) && (_nAntiTopSideNu == 1) && (_nAntiTopSidebQuark == 0) && (_nAntiTopSideLightQuark == 1) ){ //semi-leptonic
    TLorentzVector tlvW = vTlvAntiTopLepton.at(0) + vTlvAntiTopNeutrino.at(0);
    _antiTopLeptonVSbAngle = vTlvAntiTopLepton.at(0).Angle(vTlvAntiTopLightQuark.at(0).Vect()); //angle between lepton and b quark
    _antiTopLeptonVSWAngle = vTlvAntiTopLepton.at(0).Angle(tlvW.Vect()); //angle between lepton and W
    _antiTopNeutrinoVSWAngle = vTlvAntiTopNeutrino.at(0).Angle(tlvW.Vect()); //angle between lepton and W
    _antiTopbQuarkVSWAngle = vTlvAntiTopLightQuark.at(0).Angle(tlvW.Vect()); //angle between b and W
    
  } else if ( (_nAntiTopSidebQuark == 1) && (_nAntiTopSideLightQuark == 2) ){ //hadronic w. 1 b-quark
    TLorentzVector tlvW = vTlvAntiTopLightQuark.at(0) + vTlvAntiTopLightQuark.at(1);
    _antiTopbQuarkVSWAngle = vTlvAntiTopbQuark.at(0).Angle(tlvW.Vect()); 
    
  } else if ( _nAntiTopSidebQuark == 3 ){ //hadronic w. 3 b-quark
    std::vector<pairTlv2Tlv> vPairTlv2TlvW;
    TLorentzVector tlvWv01 = vTlvAntiTopLightQuark.at(0) + vTlvAntiTopLightQuark.at(1); vPairTlv2TlvW.push_back(std::make_pair(tlvWv01, vTlvAntiTopLightQuark.at(2)));
    TLorentzVector tlvWv02 = vTlvAntiTopLightQuark.at(0) + vTlvAntiTopLightQuark.at(2); vPairTlv2TlvW.push_back(std::make_pair(tlvWv02, vTlvAntiTopLightQuark.at(1)));
    TLorentzVector tlvWv12 = vTlvAntiTopLightQuark.at(1) + vTlvAntiTopLightQuark.at(2); vPairTlv2TlvW.push_back(std::make_pair(tlvWv12, vTlvAntiTopLightQuark.at(0)));
    //for (std::vector<pairTlv2Tlv>::iterator iter1=vPairTlv2TlvW.begin(); iter1!=vPairTlv2TlvW.end(); ++iter1){
    //  std::cout << "Mass of pair: " << (*iter1).first.M() << std::endl;
    //}
    std::sort(vPairTlv2TlvW.begin(), vPairTlv2TlvW.end(),  wayToSortWv);
    TLorentzVector tlvW = vPairTlv2TlvW.at(0).first;
    //std::cout << "Mass of selected W: " << tlvW.M() << std::endl;
     _antiTopbQuarkVSWAngle = vPairTlv2TlvW.at(0).second.Angle(tlvW.Vect()); 
  }	  

  //Save PFO information
  for(int j=0; j< _nPFO ; j++){ 
    ReconstructedParticle* pfo = dynamic_cast<ReconstructedParticle*>( generalIn->getElementAt(j) ) ;
    if( pfo == NULL ){ continue; }  
    //Get PFO info
    getKinematics<ReconstructedParticle>(pfo, _vRecoPt, _vRecoEta, _vRecoPhi, _vRecoMass, _vRecoTheta, _vRecoType, _vRecoCharge, _vRecoEnergy);
    
    //Sum of all energy is calo
    _vRecoECal.push_back(getEnergyCal(pfo));

    //Impact parameters
    float d0, z0, r0;
    int nTrkObj;
    getImpactParametersAndTrackEnergy(pfo,d0,z0,r0,nTrkObj);
    _vRecoD0.push_back(d0);
    _vRecoZ0.push_back(z0);
    _vRecoR0.push_back(r0);
    _vRecoNTrkObj.push_back(nTrkObj);
    
    //Cone energy for different defintions (loop through collection again for each pfo)
    _vRecoECone900.push_back(getConeEnergy(generalIn, pfo, 0.900));
    _vRecoECone950.push_back(getConeEnergy(generalIn, pfo, 0.950));
    _vRecoECone990.push_back(getConeEnergy(generalIn, pfo, 0.990));
    _vRecoECone995.push_back(getConeEnergy(generalIn, pfo, 0.995));
    _vRecoECone999.push_back(getConeEnergy(generalIn, pfo, 0.999));
    _vRecoECone9995.push_back(getConeEnergy(generalIn, pfo, 0.9995));
    
    //Ecal fractions
    _vRecoRCal.push_back(getRatioEnergyCal(pfo));
    _vRecoEnergyByMom.push_back(getEnergyByMomentum(pfo));

    //Truthlinking information
    LCCollection* recoMCTruthLink = evt->getCollection(_recoMCTruthLinkName);
    isCorrectLepton(recoMCTruthLink, pfo, _vNRelObjMC, _vRelObjIsLep, _vRecoIsCorrectLepton, _nRelObjIsLep);
  }

  //IsoLep
  bool doLep = true;
  LCCollection* isoLepIn = NULL;
  try{
    isoLepIn = evt->getCollection(_isolepParticleInName);
    if (isoLepIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    //streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    doLep = false;
    //return;
  }
  if(doLep){
    _nLeptons = isoLepIn->getNumberOfElements();
    for (int i = 0; i < _nLeptons; i++){
      ReconstructedParticle* lepton = dynamic_cast<ReconstructedParticle*>( isoLepIn->getElementAt(i) );
      getKinematics<ReconstructedParticle>(lepton, _vLepPt, _vLepEta, _vLepPhi, _vLepMass, _vLepTheta, _vLepType, _vLepCharge, _vLepEnergy); 
    }
  }

  //IsoLep Electron
  bool doLepE = true;
  LCCollection* isoLepElectronIn = NULL;
  try{
    isoLepElectronIn = evt->getCollection(_isolepParticleElectronInName);
    if (isoLepElectronIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    //streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    doLepE = false;
    //return;
  }
  if(doLepE){
    _nElectron = isoLepElectronIn->getNumberOfElements();
    for (int i = 0; i < _nElectron; i++){
      ReconstructedParticle* lepton = dynamic_cast<ReconstructedParticle*>( isoLepElectronIn->getElementAt(i) );
      getKinematics<ReconstructedParticle>(lepton, _vLepElectronPt, _vLepElectronEta, _vLepElectronPhi, _vLepElectronMass, _vLepElectronTheta, _vLepElectronType, _vLepElectronCharge, _vLepElectronEnergy); 
    }
  }

  //IsoLep Muon
  bool doLepMu = true;
  LCCollection* isoLepMuonIn = NULL;
  try{
    isoLepMuonIn = evt->getCollection(_isolepParticleMuonInName);
    if (isoLepMuonIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    //streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    doLepMu = false;
    //return;
  }
  if(doLepMu){
    _nMuon = isoLepMuonIn->getNumberOfElements();
    for (int i = 0; i < _nMuon; i++){
      ReconstructedParticle* lepton = dynamic_cast<ReconstructedParticle*>( isoLepMuonIn->getElementAt(i) );
      getKinematics<ReconstructedParticle>(lepton, _vLepMuonPt, _vLepMuonEta, _vLepMuonPhi, _vLepMuonMass, _vLepMuonTheta, _vLepMuonType, _vLepMuonCharge, _vLepMuonEnergy); 
    }
  }

  //Jet saver, clustering into 2 jets
  LCCollection* toptaggerJetIn = NULL;
  try{
    toptaggerJetIn = evt->getCollection(_toptaggerJetInName);
    if (toptaggerJetIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  _nJets = toptaggerJetIn->getNumberOfElements();
  std::vector<TLorentzVector> vTlvTotJet;
  TLorentzVector tlvTotJet;
  for (int i = 0; i < _nJets; i++){
    ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( toptaggerJetIn->getElementAt(i) ); 
    getKinematics<ReconstructedParticle>(jet, _vJetPt, _vJetEta, _vJetPhi, _vJetMass, _vJetTheta, _vJetType, _vJetCharge, _vJetEnergy); 
    TLorentzVector tlvJet;
    const double* jetMom = jet->getMomentum();
    tlvJet.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());
    vTlvTotJet.push_back(tlvJet);
    tlvTotJet += tlvJet;
  }
  _jetMassTot = tlvTotJet.M();
  std::sort(vTlvTotJet.begin(), vTlvTotJet.end(), wayToSortPt);
  TLorentzVector tlvJetHigh2;   
  for (unsigned int i = 0; i < 2; i++){
    tlvJetHigh2 += vTlvTotJet.at(i);
  }
  _jetMassHigh2Pt = tlvJetHigh2.M();

  //Jet saver, clustering into 2 jets (with trimming)
  LCCollection* toptaggerJetIn_v2 = NULL;
  try{
    toptaggerJetIn_v2 = evt->getCollection(_toptaggerJetInName_v2);
    if (toptaggerJetIn_v2->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  _nJets_v2 = toptaggerJetIn_v2->getNumberOfElements();
  std::vector<TLorentzVector> vTlvTotJet_v2;
  TLorentzVector tlvTotJet_v2;
  for (int i = 0; i < _nJets_v2; i++){
    ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( toptaggerJetIn_v2->getElementAt(i) ); 
    getKinematics<ReconstructedParticle>(jet, _vJetPt_v2, _vJetEta_v2, _vJetPhi_v2, _vJetMass_v2, _vJetTheta_v2, _vJetType_v2, _vJetCharge_v2, _vJetEnergy_v2); 
    TLorentzVector tlvJet;
    const double* jetMom = jet->getMomentum();
    tlvJet.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());
    vTlvTotJet_v2.push_back(tlvJet);
    tlvTotJet_v2 += tlvJet;
  }
  _jetMassTot_v2 = tlvTotJet_v2.M();
  std::sort(vTlvTotJet_v2.begin(), vTlvTotJet_v2.end(), wayToSortPt);
  TLorentzVector tlvJetHigh2_v2;   
  for (unsigned int i = 0; i < 2; i++){
    tlvJetHigh2_v2 += vTlvTotJet_v2.at(i);
  }
  _jetMassHigh2Pt_v2 = tlvJetHigh2_v2.M();

  //Jet saver, clustering into 4 jets
  LCCollection* jetInJ4 = NULL;
  try{
    jetInJ4 = evt->getCollection(_jetInNameJ4);
    if (jetInJ4->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  _nJetsJ4 = jetInJ4->getNumberOfElements();
  std::vector<TLorentzVector> vTlvJetJ4Tot;
  TLorentzVector tlvJetJ4Tot;
  for (int i = 0; i < _nJetsJ4; i++){
    ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( jetInJ4->getElementAt(i) ); 
    getKinematics<ReconstructedParticle>(jet, _vJetPtJ4, _vJetEtaJ4, _vJetPhiJ4, _vJetMassJ4, _vJetThetaJ4, _vJetTypeJ4, _vJetChargeJ4, _vJetEnergyJ4); 
    TLorentzVector tlvJet;
    const double* jetMom = jet->getMomentum();
    tlvJet.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());
    vTlvJetJ4Tot.push_back(tlvJet);
    tlvJetJ4Tot += tlvJet;
  }
  _jetMassJ4Tot = tlvJetJ4Tot.M();
  std::sort(vTlvJetJ4Tot.begin(), vTlvJetJ4Tot.end(), wayToSortPt);
  TLorentzVector tlvJetJ4High2;
  for (unsigned int i = 0; i < 2; i++){
    tlvJetJ4High2 += vTlvJetJ4Tot.at(i);
  }
  _jetMassJ4High2Pt = tlvJetJ4High2.M();
  //--Get jet combinations (2 out of 4)
  for (int i = 0; i < _nJetsJ4; i++){
    for (int j = (i+1); j < _nJetsJ4; j++){
      TLorentzVector tlvJet;
      tlvJet += vTlvJetJ4Tot.at(i) + vTlvJetJ4Tot.at(j);
      _vJetMassJ4Comb2.push_back(tlvJet.M());
    }
  }
  //--Get jet combinations (3 out of 4)
  for (int i = 0; i < _nJetsJ4; i++){
    for (int j = (i+1); j < _nJetsJ4; j++){
      for (int k = (j+1); k < _nJetsJ4; k++){
	TLorentzVector tlvJet;
	tlvJet += vTlvJetJ4Tot.at(i) + vTlvJetJ4Tot.at(j) + vTlvJetJ4Tot.at(k);
	_vJetMassJ4Comb3.push_back(tlvJet.M());
      }
    }
  }

  //Jet saver, clustering into 4 jets
  LCCollection* jetInJ4_v2 = NULL;
  try{
    jetInJ4_v2 = evt->getCollection(_jetInNameJ4_v2);
    if (jetInJ4_v2->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  _nJetsJ4_v2 = jetInJ4_v2->getNumberOfElements();
  std::vector<TLorentzVector> vTlvJetJ4Tot_v2;
  TLorentzVector tlvJetJ4Tot_v2;
  for (int i = 0; i < _nJetsJ4_v2; i++){
    ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( jetInJ4_v2->getElementAt(i) ); 
    getKinematics<ReconstructedParticle>(jet, _vJetPtJ4_v2, _vJetEtaJ4_v2, _vJetPhiJ4_v2, _vJetMassJ4_v2, _vJetThetaJ4_v2, _vJetTypeJ4_v2, _vJetChargeJ4_v2, _vJetEnergyJ4_v2); 
    TLorentzVector tlvJet;
    const double* jetMom = jet->getMomentum();
    tlvJet.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());
    vTlvJetJ4Tot_v2.push_back(tlvJet);
    tlvJetJ4Tot_v2 += tlvJet;
  }
  _jetMassJ4Tot_v2 = tlvJetJ4Tot_v2.M();
  std::sort(vTlvJetJ4Tot_v2.begin(), vTlvJetJ4Tot_v2.end(), wayToSortPt);
  TLorentzVector tlvJetJ4High2_v2;
  for (unsigned int i = 0; i < 2; i++){
    tlvJetJ4High2_v2 += vTlvJetJ4Tot_v2.at(i);
  }
  _jetMassJ4High2Pt_v2 = tlvJetJ4High2_v2.M();
  //--Get jet combinations (2 out of 4)
  for (int i = 0; i < _nJetsJ4_v2; i++){
    for (int j = (i+1); j < _nJetsJ4_v2; j++){
      TLorentzVector tlvJet;
      tlvJet += vTlvJetJ4Tot_v2.at(i) + vTlvJetJ4Tot_v2.at(j);
      _vJetMassJ4Comb2_v2.push_back(tlvJet.M());
    }
  }
  //--Get jet combinations (3 out of 4)
  for (int i = 0; i < _nJetsJ4_v2; i++){
    for (int j = (i+1); j < _nJetsJ4_v2; j++){
      for (int k = (j+1); k < _nJetsJ4_v2; k++){
	TLorentzVector tlvJet;
	tlvJet += vTlvJetJ4Tot_v2.at(i) + vTlvJetJ4Tot_v2.at(j) + vTlvJetJ4Tot_v2.at(k);
	_vJetMassJ4Comb3_v2.push_back(tlvJet.M());
      }
    }
  }

  //Jet saver, clustering into 6 jets
  LCCollection* jetInJ6 = NULL;
  try{
    jetInJ6 = evt->getCollection(_jetInNameJ6);
    if (jetInJ6->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  _nJetsJ6 = jetInJ6->getNumberOfElements();
  std::vector<TLorentzVector> vTlvJetJ6Tot;
  TLorentzVector tlvJetJ6Tot;
  for (int i = 0; i < _nJetsJ6; i++){
    ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( jetInJ6->getElementAt(i) ); 
    getKinematics<ReconstructedParticle>(jet, _vJetPtJ6, _vJetEtaJ6, _vJetPhiJ6, _vJetMassJ6, _vJetThetaJ6, _vJetTypeJ6, _vJetChargeJ6, _vJetEnergyJ6); 
    TLorentzVector tlvJet;
    const double* jetMom = jet->getMomentum();
    tlvJet.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());
    vTlvJetJ6Tot.push_back(tlvJet);
    tlvJetJ6Tot += tlvJet;
  }
  _jetMassJ6Tot = tlvJetJ6Tot.M();
  std::sort(vTlvJetJ6Tot.begin(), vTlvJetJ6Tot.end(), wayToSortPt);
  TLorentzVector tlvJetJ6High2;
  for (unsigned int i = 0; i < 2; i++){
    tlvJetJ6High2 += vTlvJetJ6Tot.at(i);
  }
  _jetMassJ6High2Pt = tlvJetJ6High2.M();
  //--Get jet combinations (2 out of 6)
  for (int i = 0; i < _nJetsJ6; i++){
    for (int j = (i+1); j < _nJetsJ6; j++){
      TLorentzVector tlvJet;
      tlvJet += vTlvJetJ6Tot.at(i) + vTlvJetJ6Tot.at(j);
      _vJetMassJ6Comb2.push_back(tlvJet.M());
    }
  }
  //--Get jet combinations (3 out of 6)
  for (int i = 0; i < _nJetsJ6; i++){
    for (int j = (i+1); j < _nJetsJ6; j++){
      for (int k = (j+1); k < _nJetsJ6; k++){
	TLorentzVector tlvJet;
	tlvJet += vTlvJetJ6Tot.at(i) + vTlvJetJ6Tot.at(j) + vTlvJetJ6Tot.at(k);
	_vJetMassJ6Comb3.push_back(tlvJet.M());
      }
    }
  }

  //Jet saver, clustering into 6 jets
  LCCollection* jetInJ6_v2 = NULL;
  try{
    jetInJ6_v2 = evt->getCollection(_jetInNameJ6_v2);
    if (jetInJ6_v2->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  _nJetsJ6_v2 = jetInJ6_v2->getNumberOfElements();
  std::vector<TLorentzVector> vTlvJetJ6Tot_v2;
  TLorentzVector tlvJetJ6Tot_v2;
  for (int i = 0; i < _nJetsJ6_v2; i++){
    ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( jetInJ6_v2->getElementAt(i) ); 
    getKinematics<ReconstructedParticle>(jet, _vJetPtJ6_v2, _vJetEtaJ6_v2, _vJetPhiJ6_v2, _vJetMassJ6_v2, _vJetThetaJ6_v2, _vJetTypeJ6_v2, _vJetChargeJ6_v2, _vJetEnergyJ6_v2); 
    TLorentzVector tlvJet;
    const double* jetMom = jet->getMomentum();
    tlvJet.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());
    vTlvJetJ6Tot_v2.push_back(tlvJet);
    tlvJetJ6Tot_v2 += tlvJet;
  }
  _jetMassJ6Tot_v2 = tlvJetJ6Tot_v2.M();
  std::sort(vTlvJetJ6Tot_v2.begin(), vTlvJetJ6Tot_v2.end(), wayToSortPt);
  TLorentzVector tlvJetJ6High2_v2;
  for (unsigned int i = 0; i < 2; i++){
    tlvJetJ6High2_v2 += vTlvJetJ6Tot_v2.at(i);
  }
  _jetMassJ6High2Pt_v2 = tlvJetJ6High2_v2.M();
  //--Get jet combinations (2 out of 6)
  for (int i = 0; i < _nJetsJ6_v2; i++){
    for (int j = (i+1); j < _nJetsJ6_v2; j++){
      TLorentzVector tlvJet;
      tlvJet += vTlvJetJ6Tot_v2.at(i) + vTlvJetJ6Tot_v2.at(j);
      _vJetMassJ6Comb2_v2.push_back(tlvJet.M());
    }
  }
  //--Get jet combinations (3 out of 6)
  for (int i = 0; i < _nJetsJ6_v2; i++){
    for (int j = (i+1); j < _nJetsJ6_v2; j++){
      for (int k = (j+1); k < _nJetsJ6_v2; k++){
	TLorentzVector tlvJet;
	tlvJet += vTlvJetJ6Tot_v2.at(i) + vTlvJetJ6Tot_v2.at(j) + vTlvJetJ6Tot_v2.at(k);
	_vJetMassJ6Comb3_v2.push_back(tlvJet.M());
      }
    }
  }

  //Top tagger
  LCCollection* toptaggerIn = NULL;
  try{
    toptaggerIn = evt->getCollection(_toptaggerInName);
    if (toptaggerIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  int nTagged = toptaggerIn->getNumberOfElements();
  for (int i = 0; i < nTagged; i++){
    ReconstructedParticle* top = dynamic_cast<ReconstructedParticle*>( toptaggerIn->getElementAt(i) );
    if( (top->getEnergy() != 0.) ){ 
      _vTopTagged.push_back(1);
      _nTopTagged += 1;
      getKinematics<ReconstructedParticle>(top, _vTopPt, _vTopEta, _vTopPhi, _vTopMass, _vTopTheta, _vTopType, _vTopCharge, _vTopEnergy); 
      LCCollection* toptaggerIn_W = evt->getCollection(_toptaggerInName_W); 
      if( toptaggerIn_W == NULL){ continue; } 
      ReconstructedParticle* topW = dynamic_cast<ReconstructedParticle*>( toptaggerIn_W->getElementAt(i) );
      getKinematics<ReconstructedParticle>(topW, _vTopPtW, _vTopEtaW, _vTopPhiW, _vTopMassW, _vTopThetaW, _vTopTypeW, _vTopChargeW, _vTopEnergyW); 
    } else {
      _vTopTagged.push_back(0);
    }
  }
  
  //Top tagger helicity angle
  LCCollection* toptaggerCosThetaWIn = NULL;
  try{
    toptaggerCosThetaWIn = evt->getCollection( _toptaggerInName_cos_theta_W );
    if (toptaggerCosThetaWIn->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }

  LCGenericObject* lcgCosThetaW = dynamic_cast<LCGenericObject*>( toptaggerCosThetaWIn->getElementAt(0) ); //only 1 object
  int nCosThetaW = lcgCosThetaW->getNDouble();
  for (int i = 0; i < nCosThetaW; i++){ 
    _vTopTaggedWCosTheta.push_back( lcgCosThetaW->getDoubleVal(i) );
  }

  //Top tagger
  LCCollection* toptaggerIn_v2 = NULL;
  try{
    toptaggerIn_v2 = evt->getCollection(_toptaggerInName_v2);
    if (toptaggerIn_v2->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  int nTagged_v2 = toptaggerIn_v2->getNumberOfElements();
  for (int i = 0; i < nTagged_v2; i++){
    ReconstructedParticle* top = dynamic_cast<ReconstructedParticle*>( toptaggerIn_v2->getElementAt(i) );
    if( (top->getEnergy() != 0.) ){ 
      _vTopTagged_v2.push_back(1);
      _nTopTagged_v2 += 1;
      getKinematics<ReconstructedParticle>(top, _vTopPt_v2, _vTopEta_v2, _vTopPhi_v2, _vTopMass_v2, _vTopTheta_v2, _vTopType_v2, _vTopCharge_v2, _vTopEnergy_v2); 
      LCCollection* toptaggerIn_v2_W = evt->getCollection(_toptaggerInName_v2_W); 
      if( toptaggerIn_v2_W == NULL){ continue; } 
      ReconstructedParticle* topW = dynamic_cast<ReconstructedParticle*>( toptaggerIn_v2_W->getElementAt(i) );
      getKinematics<ReconstructedParticle>(topW, _vTopPtW_v2, _vTopEtaW_v2, _vTopPhiW_v2, _vTopMassW_v2, _vTopThetaW_v2, _vTopTypeW_v2, _vTopChargeW_v2, _vTopEnergyW_v2); 
    } else {
      _vTopTagged_v2.push_back(0);
    }
  }
  
  //Top tagger helicity angle
  LCCollection* toptaggerCosThetaWIn_v2 = NULL;
  try{
    toptaggerCosThetaWIn_v2 = evt->getCollection( _toptaggerInName_v2_cos_theta_W );
    if (toptaggerCosThetaWIn_v2->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }

  LCGenericObject* lcgCosThetaW_v2 = dynamic_cast<LCGenericObject*>( toptaggerCosThetaWIn_v2->getElementAt(0) ); //only 1 object
  int nCosThetaW_v2 = lcgCosThetaW_v2->getNDouble();
  for (int i = 0; i < nCosThetaW_v2; i++){ 
    _vTopTaggedWCosTheta_v2.push_back( lcgCosThetaW_v2->getDoubleVal(i) );
  }

  //Move these variables to plotting side
  /*
  //Semi-leptonic side, missing energy, etc.
  TLorentzVector tlvVisible;
  TLorentzVector tlvVisibleExclTopTagged;
  TLorentzVector tlvVisiblewTopTaggedMass;

  //if( (_nLeptons >= 1) && (_nTopTagged >= 1) ){ //we only do this for cases with at least 1 lepton

  //Lepton (use the first in list)
  TLorentzVector lepTlv;
  ReconstructedParticle* lepton = dynamic_cast<ReconstructedParticle*>( isoLepIn->getElementAt(0) );
  if (lepton != NULL){
    const double* lepMom = lepton->getMomentum(); 
    lepTlv.SetPxPyPzE(lepMom[0], lepMom[1], lepMom[2], lepton->getEnergy());
    tlvVisible += lepTlv;    
    tlvVisibleExclTopTagged += lepTlv;    
    tlvVisiblewTopTaggedMass += lepTlv;
  }
   
  //Jets (typically JetOut)
  for (int i = 0; i < _nJets; i++){
    TLorentzVector jetTlv;
    ReconstructedParticle* jet = dynamic_cast<ReconstructedParticle*>( toptaggerJetIn->getElementAt(i) ); 
    const double* jetMom = jet->getMomentum(); 
    jetTlv.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());
    tlvVisible += jetTlv;
    if(_vTopTagged.at(i) == 0){
      tlvVisibleExclTopTagged += jetTlv;
      tlvVisiblewTopTaggedMass += jetTlv;
    } else {
      ReconstructedParticle* toppen = dynamic_cast<ReconstructedParticle*>( toptaggerIn->getElementAt(i) );
      TLorentzVector toppenTlv;
      const double* toppenMom = toppen->getMomentum(); 
      toppenTlv.SetPxPyPzE(toppenMom[0], toppenMom[1], toppenMom[2], toppen->getEnergy());
      tlvVisiblewTopTaggedMass += toppenTlv;
    }	
    //std::cout << "NOTE: Add the part of the top jet that was not top tagged, i.e. some stuff cleaned away!" << std::endl;
  }

  //Cut to remove events with photons (ISR) + beamstrahlung
  _missingPt = tlvVisible.Pt(); //if no photon detector in detector, this should be neutrino pT
  _missingPz = -tlvVisible.Pz();
  _missingMass = tlvVisible.M(); //if no photon detector in detector, this should be neutrino invariant mass
  _missingE = _eventInvMass - tlvVisible.E();
  
  _missingPtExclTopTagged = tlvVisibleExclTopTagged.Pt(); 
  _missingPzExclTopTagged = -tlvVisibleExclTopTagged.Pz(); 
  _missingMassExclTopTagged = tlvVisibleExclTopTagged.M();
  _missingEExclTopTagged = _eventInvMass - tlvVisibleExclTopTagged.E();
  
  _missingPtwTopTaggedMass = tlvVisiblewTopTaggedMass.Pt();
  _missingPzwTopTaggedMass = -tlvVisiblewTopTaggedMass.Pz();
  _missingMasswTopTaggedMass = tlvVisiblewTopTaggedMass.M();
  _missingEwTopTaggedMass = _eventInvMass - tlvVisiblewTopTaggedMass.E();
  
  //Cut to remove single top events, if this is not top mass remove
  TLorentzVector tlvSemiLep = -tlvVisible + tlvVisibleExclTopTagged;
  _semiLepPt = tlvSemiLep.Pt();
  _semiLepPz = tlvSemiLep.Pz();
  _semiLepMass = tlvSemiLep.M();
  _semiLepE = _eventInvMass - tlvSemiLep.E();
  */
  //}

  //Flavour-tagging information - 2 jets
  LCCollection* vertexJets = NULL;
  try{
    vertexJets = evt->getCollection(_vertexJetsInName);
    if (vertexJets->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  
  PIDHandler hPID(vertexJets); 
  int algo = hPID.getAlgorithmID("lcfiplus");
  int iBTag = hPID.getParameterIndex(algo, "BTag");
  int iCTag = hPID.getParameterIndex(algo, "CTag");
  
  TLorentzVector tlvJetFlavourTagged;
  TLorentzVector tlvJetFlavourTaggedwCut050;
  TLorentzVector tlvJetFlavourTaggedwCut075;
  TLorentzVector tlvJetFlavourTaggedwCut090;
  for (int i = 0; i < vertexJets->getNumberOfElements(); i++) {
    ReconstructedParticle *jet = dynamic_cast<ReconstructedParticle*>(vertexJets->getElementAt(i));
    getKinematics<ReconstructedParticle>(jet, _vJetPtFlavourTag, _vJetEtaFlavourTag, _vJetPhiFlavourTag, _vJetMassFlavourTag, _vJetThetaFlavourTag, _vJetTypeFlavourTag, _vJetChargeFlavourTag, _vJetEnergyFlavourTag); 
    TLorentzVector jetTlv; 
    const double* jetMom = jet->getMomentum(); 
    jetTlv.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());  
    tlvJetFlavourTagged += jetTlv;
    const ParticleID &pid = hPID.getParticleID(jet, algo);
    double bTag_helper = pid.getParameters()[iBTag];
    double cTag_helper = pid.getParameters()[iCTag];
    _vBTag.push_back(bTag_helper);
    if (bTag_helper >= 0.50){ tlvJetFlavourTaggedwCut050 += jetTlv; }
    if (bTag_helper >= 0.75){ tlvJetFlavourTaggedwCut075 += jetTlv; }
    if (bTag_helper >= 0.90){ tlvJetFlavourTaggedwCut090 += jetTlv; }
    _vCTag.push_back(cTag_helper);
    double bcTag_helper = 0.; 
    if (bTag_helper>0. || cTag_helper>0.) bcTag_helper = cTag_helper/(bTag_helper+cTag_helper);
    _vBCTag.push_back(bcTag_helper);
    _sumBTag = _sumBTag + bTag_helper;
    _sumCTag = _sumCTag + cTag_helper;
    _sumBCTag = _sumBCTag + bcTag_helper;
  }// end loop on vertex jets

  int algoYth = hPID.getAlgorithmID("yth");
  int iy23 = hPID.getParameterIndex(algoYth, "y23");
  int iy34 = hPID.getParameterIndex(algoYth, "y34");
  int iy45 = hPID.getParameterIndex(algoYth, "y45");
  int iy56 = hPID.getParameterIndex(algoYth, "y56");
  const ParticleID &jet_distances = hPID.getParticleID(vertexJets->getElementAt(0), algoYth);
  _y23 = jet_distances.getParameters()[iy23];
  _y34 = jet_distances.getParameters()[iy34];
  _y45 = jet_distances.getParameters()[iy45];
  _y56 = jet_distances.getParameters()[iy56];
  
  _invMassFlavourTag = tlvJetFlavourTagged.M();
  _invMassFlavourTagwCut050 = tlvJetFlavourTaggedwCut050.M();
  _invMassFlavourTagwCut075 = tlvJetFlavourTaggedwCut075.M();
  _invMassFlavourTagwCut090 = tlvJetFlavourTaggedwCut090.M();

  //Flavour-tagging information - processor 2, called J3 for other reasons
  LCCollection* vertexJetsJ3 = NULL;
  try{
    vertexJetsJ3 = evt->getCollection(_vertexJets3InName);
    if (vertexJetsJ3->getNumberOfElements() < 1){
      throw DataNotAvailableException("Collection is there, but its empty!");
    }
  }
  catch (DataNotAvailableException e){
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;
    return;
  }
  
  PIDHandler hPIDJ3(vertexJetsJ3); 
  int algoJ3 = hPIDJ3.getAlgorithmID("lcfiplus");
  int iBTagJ3 = hPIDJ3.getParameterIndex(algoJ3, "BTag");
  int iCTagJ3 = hPIDJ3.getParameterIndex(algoJ3, "CTag");
  
  TLorentzVector tlvJetFlavourTaggedJ3;
  TLorentzVector tlvJetFlavourTaggedJ3wCut050;
  TLorentzVector tlvJetFlavourTaggedJ3wCut075;
  TLorentzVector tlvJetFlavourTaggedJ3wCut090;
  for (int i = 0; i < vertexJetsJ3->getNumberOfElements(); i++) {
    ReconstructedParticle *jet = dynamic_cast<ReconstructedParticle*>(vertexJetsJ3->getElementAt(i));
    getKinematics<ReconstructedParticle>(jet, _vJetPtFlavourTagJ3, _vJetEtaFlavourTagJ3, _vJetPhiFlavourTagJ3, _vJetMassFlavourTagJ3, _vJetThetaFlavourTagJ3, _vJetTypeFlavourTagJ3, _vJetChargeFlavourTagJ3, _vJetEnergyFlavourTagJ3); 
    TLorentzVector jetTlv; 
    const double* jetMom = jet->getMomentum(); 
    jetTlv.SetPxPyPzE(jetMom[0], jetMom[1], jetMom[2], jet->getEnergy());  
    tlvJetFlavourTaggedJ3 += jetTlv;
    const ParticleID &pid = hPIDJ3.getParticleID(jet, algoJ3);
    double bTag_helper = pid.getParameters()[iBTagJ3];
    double cTag_helper = pid.getParameters()[iCTagJ3];
    _vBTagJ3.push_back(bTag_helper);
    if (bTag_helper >= 0.50){ tlvJetFlavourTaggedJ3wCut050 += jetTlv; }
    if (bTag_helper >= 0.75){ tlvJetFlavourTaggedJ3wCut075 += jetTlv; }
    if (bTag_helper >= 0.90){ tlvJetFlavourTaggedJ3wCut090 += jetTlv; }
    _vCTagJ3.push_back(cTag_helper);
    double bcTag_helper = 0.; 
    if (bTag_helper>0. || cTag_helper>0.) bcTag_helper = cTag_helper/(bTag_helper+cTag_helper);
    _vBCTagJ3.push_back(bcTag_helper);
    _sumBTagJ3 = _sumBTagJ3 + bTag_helper;
    _sumCTagJ3 = _sumCTagJ3 + cTag_helper;
    _sumBCTagJ3 = _sumBCTagJ3 + bcTag_helper;
  }// end loop on vertex jets

  int algoYthJ3 = hPIDJ3.getAlgorithmID("yth");
  int iy23J3 = hPIDJ3.getParameterIndex(algoYthJ3, "y23");
  int iy34J3 = hPIDJ3.getParameterIndex(algoYthJ3, "y34");
  int iy45J3 = hPIDJ3.getParameterIndex(algoYthJ3, "y45");
  int iy56J3 = hPIDJ3.getParameterIndex(algoYthJ3, "y56");
  const ParticleID &jet_distances_j3 = hPIDJ3.getParticleID(vertexJetsJ3->getElementAt(0), algoYthJ3);
  _y23J3 = jet_distances_j3.getParameters()[iy23J3];
  _y34J3 = jet_distances_j3.getParameters()[iy34J3];
  _y45J3 = jet_distances_j3.getParameters()[iy45J3];
  _y56J3 = jet_distances_j3.getParameters()[iy56J3];
  
  _invMassFlavourTagJ3 = tlvJetFlavourTaggedJ3.M();
  _invMassFlavourTagJ3wCut050 = tlvJetFlavourTaggedJ3wCut050.M();
  _invMassFlavourTagJ3wCut075 = tlvJetFlavourTaggedJ3wCut075.M();
  _invMassFlavourTagJ3wCut090 = tlvJetFlavourTaggedJ3wCut090.M();

  _nEvt++ ;
  _tree->Fill(); 
} //{end processEvent

void TopAsymmetryStoreProcessor::end(){
  _out->Write();
  _out->Close();
  delete _out;
} //end end

void TopAsymmetryStoreProcessor::clearVectors(){

  _pjList.clear();
  _daughters.clear();
  _daughterCombinations.clear();
  _topFermions.clear();
  _antitopFermions.clear();

  _vGenBeamCharge.clear();
  _vGenBeamEnergy.clear();
  _vGenBeamEta.clear();
  _vGenBeamGenStatus.clear();
  _vGenBeamM.clear();
  _vGenBeamPDG.clear();
  _vGenBeamPhi.clear();
  _vGenBeamPt.clear();
  _vGenBeamSimStatus.clear();
  _vGenBeamTheta.clear();

  _vGenTopCharge.clear();
  _vGenTopType.clear();
  _vGenTopPt.clear();
  _vGenTopEnergy.clear();
  _vGenTopEta.clear();
  _vGenTopMass.clear();
  _vGenTopTheta.clear();
  _vGenTopPhi.clear();

  _vGenLepCharge.clear();
  _vGenLepType.clear();
  _vGenLepPt.clear();
  _vGenLepEnergy.clear();
  _vGenLepEta.clear();
  _vGenLepMass.clear();
  _vGenLepTheta.clear();
  _vGenLepPhi.clear();

  _vLepCharge.clear();
  _vLepType.clear();
  _vLepPt.clear();
  _vLepEnergy.clear();
  _vLepEta.clear();
  _vLepMass.clear();
  _vLepTheta.clear();
  _vLepPhi.clear();

  _vLepElectronCharge.clear();
  _vLepElectronType.clear();
  _vLepElectronPt.clear();
  _vLepElectronEnergy.clear();
  _vLepElectronEta.clear();
  _vLepElectronMass.clear();
  _vLepElectronTheta.clear();
  _vLepElectronPhi.clear();

  _vLepMuonCharge.clear();
  _vLepMuonType.clear();
  _vLepMuonPt.clear();
  _vLepMuonEnergy.clear();
  _vLepMuonEta.clear();
  _vLepMuonMass.clear();
  _vLepMuonTheta.clear();
  _vLepMuonPhi.clear();

  _vRecoCharge.clear();
  _vRecoType.clear();
  _vRecoPt.clear();
  _vRecoEnergy.clear();
  _vRecoEta.clear();
  _vRecoMass.clear();
  _vRecoTheta.clear();
  _vRecoPhi.clear();

  _vRecoECal.clear();
  _vRecoRCal.clear();
  _vRecoEnergyByMom.clear();

  _vRecoD0.clear();
  _vRecoZ0.clear();
  _vRecoR0.clear();
  _vRecoNTrkObj.clear();
 
  _vRecoECone900.clear();
  _vRecoECone950.clear();
  _vRecoECone990.clear();
  _vRecoECone995.clear();
  _vRecoECone999.clear();
  _vRecoECone9995.clear();

  _vNRelObjMC.clear();
  _vRelObjIsLep.clear();
  _vRecoIsCorrectLepton.clear();

  _vNRelObjMCPhoton.clear();
  _vRelObjIsPhoton.clear();
  _vRecoIsCorrectPhoton.clear();

  _vPhotonCharge.clear();
  _vPhotonType.clear();
  _vPhotonPt.clear();
  _vPhotonEnergy.clear();
  _vPhotonEta.clear();
  _vPhotonMass.clear();
  _vPhotonTheta.clear();
  _vPhotonPhi.clear();

  _vJetCharge.clear();
  _vJetType.clear();
  _vJetPt.clear();
  _vJetEnergy.clear();
  _vJetEta.clear();
  _vJetMass.clear();
  _vJetTheta.clear();
  _vJetPhi.clear();

  _vJetCharge_v2.clear();
  _vJetType_v2.clear();
  _vJetPt_v2.clear();
  _vJetEnergy_v2.clear();
  _vJetEta_v2.clear();
  _vJetMass_v2.clear();
  _vJetTheta_v2.clear();
  _vJetPhi_v2.clear();

  _vJetChargeJ4.clear();
  _vJetTypeJ4.clear();
  _vJetPtJ4.clear();
  _vJetEnergyJ4.clear();
  _vJetEtaJ4.clear();
  _vJetMassJ4.clear();
  _vJetThetaJ4.clear();
  _vJetPhiJ4.clear();
  _vJetMassJ4Comb2.clear();
  _vJetMassJ4Comb3.clear();

  _vJetChargeJ4_v2.clear();
  _vJetTypeJ4_v2.clear();
  _vJetPtJ4_v2.clear();
  _vJetEnergyJ4_v2.clear();
  _vJetEtaJ4_v2.clear();
  _vJetMassJ4_v2.clear();
  _vJetThetaJ4_v2.clear();
  _vJetPhiJ4_v2.clear();
  _vJetMassJ4Comb2_v2.clear();
  _vJetMassJ4Comb3_v2.clear();

  _vJetChargeJ6.clear();
  _vJetTypeJ6.clear();
  _vJetPtJ6.clear();
  _vJetEnergyJ6.clear();
  _vJetEtaJ6.clear();
  _vJetMassJ6.clear();
  _vJetThetaJ6.clear();
  _vJetPhiJ6.clear();
  _vJetMassJ6Comb2.clear();
  _vJetMassJ6Comb3.clear();

  _vJetChargeJ6_v2.clear();
  _vJetTypeJ6_v2.clear();
  _vJetPtJ6_v2.clear();
  _vJetEnergyJ6_v2.clear();
  _vJetEtaJ6_v2.clear();
  _vJetMassJ6_v2.clear();
  _vJetThetaJ6_v2.clear();
  _vJetPhiJ6_v2.clear();
  _vJetMassJ6Comb2_v2.clear();
  _vJetMassJ6Comb3_v2.clear();

  _vTopTagged.clear();
  _vTopTaggedWCosTheta.clear();

  _vTopCharge.clear();
  _vTopType.clear();
  _vTopPt.clear();
  _vTopEnergy.clear();
  _vTopEta.clear();
  _vTopMass.clear();
  _vTopTheta.clear();
  _vTopPhi.clear();

  _vTopChargeW.clear();
  _vTopTypeW.clear();
  _vTopPtW.clear();
  _vTopEnergyW.clear();
  _vTopEtaW.clear();
  _vTopMassW.clear();
  _vTopThetaW.clear();
  _vTopPhiW.clear();

  _vTopTagged_v2.clear();
  _vTopTaggedWCosTheta_v2.clear();

  _vTopCharge_v2.clear();
  _vTopType_v2.clear();
  _vTopPt_v2.clear();
  _vTopEnergy_v2.clear();
  _vTopEta_v2.clear();
  _vTopMass_v2.clear();
  _vTopTheta_v2.clear();
  _vTopPhi_v2.clear();

  _vTopChargeW_v2.clear();
  _vTopTypeW_v2.clear();
  _vTopPtW_v2.clear();
  _vTopEnergyW_v2.clear();
  _vTopEtaW_v2.clear();
  _vTopMassW_v2.clear();
  _vTopThetaW_v2.clear();
  _vTopPhiW_v2.clear();

  _vBTag.clear();
  _vCTag.clear();
  _vBCTag.clear();

  _vBTagJ3.clear();
  _vCTagJ3.clear();
  _vBCTagJ3.clear();
  
  _vJetChargeFlavourTag.clear();
  _vJetTypeFlavourTag.clear();
  _vJetPtFlavourTag.clear();
  _vJetEnergyFlavourTag.clear();
  _vJetEtaFlavourTag.clear();
  _vJetMassFlavourTag.clear();
  _vJetThetaFlavourTag.clear();
  _vJetPhiFlavourTag.clear();

  _vJetChargeFlavourTagJ3.clear();
  _vJetTypeFlavourTagJ3.clear();
  _vJetPtFlavourTagJ3.clear();
  _vJetEnergyFlavourTagJ3.clear();
  _vJetEtaFlavourTagJ3.clear();
  _vJetMassFlavourTagJ3.clear();
  _vJetThetaFlavourTagJ3.clear();
  _vJetPhiFlavourTagJ3.clear();

} //end clearVectors

void TopAsymmetryStoreProcessor::clearNumbers(){

  _Et = 0;
  _minPt = 0;
  _nPFO = 0;
  _nPhotons = 0;
  _nPhotonswoCut = 0;

  _nMCP = 0;
  _eventInvMass = 0;

  _mcLepType = 0;
  _singleTop = false;
  _compatibleWith6f = false;

  _nTopSideLep = 0;
  _nTopSideNu = 0;
  _nTopSidebQuark = 0;
  _nTopSideLightQuark = 0;
  _topLeptonVSbAngle = 0.;
  _topLeptonVSWAngle = 0.;
  _topNeutrinoVSWAngle = 0.;
  _topbQuarkVSWAngle = 0.;
  
  _nAntiTopSideLep = 0;
  _nAntiTopSideNu = 0;
  _nAntiTopSidebQuark = 0;
  _nAntiTopSideLightQuark = 0;
  _antiTopLeptonVSbAngle = 0.;
  _antiTopLeptonVSWAngle = 0.;
  _antiTopNeutrinoVSWAngle = 0.;
  _antiTopbQuarkVSWAngle = 0.;

  _nRelObjIsLep = 0;
  _nRelObjIsPhoton = 0;

  _nLeptons = 0;
  _nElectron = 0; //missing in last processing of data
  _nMuon = 0; //missing in last processing of data
  _nJets = 0;
  _nJets_v2 = 0;
  _nJetsJ4 = 0;
  _nJetsJ4_v2 = 0;
  _nJetsJ6 = 0;
  _nJetsJ6_v2 = 0;
  _nTopTagged = 0;
  _nTopTagged_v2 = 0;

  _jetMassTot = 0;
  _jetMassHigh2Pt = 0;
  _jetMassTot_v2 = 0;
  _jetMassHigh2Pt_v2 = 0;

  _jetMassJ4Tot = 0;
  _jetMassJ4High2Pt = 0;
  _jetMassJ4Tot_v2 = 0;
  _jetMassJ4High2Pt_v2 = 0;

  _jetMassJ6Tot = 0;
  _jetMassJ6High2Pt = 0;
  _jetMassJ6Tot_v2 = 0;
  _jetMassJ6High2Pt_v2 = 0;
  /*
  _missingPt = 0.;
  _missingPz = 0.;
  _missingMass = 0.;
  _missingE = 0.;
  
  _missingPtExclTopTagged = 0.;
  _missingPzExclTopTagged = 0.;
  _missingMassExclTopTagged = 0.;
  _missingEExclTopTagged = 0.;

  _missingPtwTopTaggedMass = 0.;
  _missingPzwTopTaggedMass = 0.;
  _missingMasswTopTaggedMass = 0.;
  _missingEwTopTaggedMass = 0.;

  _semiLepPt = 0.;
  _semiLepPz = 0.;
  _semiLepMass = 0.;
  _semiLepE = 0.;
  */
  _sumBTag = 0.;
  _sumCTag = 0.;
  _sumBCTag = 0.;
  _y23 = 0.;
  _y34 = 0.;
  _y45 = 0.;
  _y56 = 0.;

  _sumBTagJ3 = 0.;
  _sumCTagJ3 = 0.;
  _sumBCTagJ3 = 0.;
  _y23J3 = 0.;
  _y34J3 = 0.;
  _y45J3 = 0.;
  _y56J3 = 0.;

  _invMassFlavourTag = 0.;
  _invMassFlavourTagwCut050 = 0.;
  _invMassFlavourTagwCut075 = 0.;
  _invMassFlavourTagwCut090 = 0.;

  _invMassFlavourTagJ3 = 0;
  _invMassFlavourTagJ3wCut050 = 0;
  _invMassFlavourTagJ3wCut075 = 0;
  _invMassFlavourTagJ3wCut090 = 0;

} //end clearNumbers

void TopAsymmetryStoreProcessor::convertFromRecParticle(LCCollection* recCol)
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

bool TopAsymmetryStoreProcessor::isElectron(int& id){ return fabs(id) == _pdg_electron; } //end isElectron  
bool TopAsymmetryStoreProcessor::isMuon(int& id){ return fabs(id) == _pdg_muon; } //end isElectron  
bool TopAsymmetryStoreProcessor::isTau(int& id){ return fabs(id) == _pdg_tau; } //end isElectron  
bool TopAsymmetryStoreProcessor::isPhoton(int& id){ return fabs(id) == _pdg_photon; } //end isPhoton

template <class T>
  void getKinematics(T*& obj, std::vector<float > &objPt, std::vector<float > &objEta, std::vector<float > &objPhi, std::vector<float > &objM, std::vector<float > &objTheta, std::vector<int > &objType, std::vector<float> &objCharge, std::vector<float> &objEnergy){
  ReconstructedParticle* recoObj = dynamic_cast<EVENT::ReconstructedParticle*> ( obj );
  if ( recoObj != NULL ){
    //std::cout << "obj->getType(): " << recoObj->getType() << std::endl;   
    objType.push_back(recoObj->getType());
  }
  const double* objMom = obj->getMomentum(); 
  float e = obj->getEnergy();
  TLorentzVector elobj;
  elobj.SetPxPyPzE(objMom[0],objMom[1],objMom[2],e); 
  
  objPt.push_back(elobj.Pt());
  objEta.push_back(elobj.Eta());
  objPhi.push_back(elobj.Phi());
  objM.push_back(elobj.M());
  objTheta.push_back(elobj.Theta());
  
  objCharge.push_back(obj->getCharge());
  objEnergy.push_back(obj->getEnergy());
  
  streamlog_out(DEBUG) << "getKinematics - px = " << objMom[0] << std::endl;
  streamlog_out(DEBUG) << "getKinematics - py = " << objMom[1] << std::endl;
  streamlog_out(DEBUG) << "getKinematics - pz = " << objMom[2] << std::endl;
  streamlog_out(DEBUG) << "getKinematics - m = " << objM.back() << std::endl;
  streamlog_out(DEBUG) << "getKinematics - pt = " << objPt.back() << std::endl;

}//end getKinematics

void getBeamInfo(LCCollection* coll, std::vector<float>& vCharge, std::vector<float>& vEnergy, std::vector<float>& vEta, std::vector<int>& vGenStatus, std::vector<float>& vM, std::vector<int>& vPDG, std::vector<float>& vPhi, std::vector<float>& vPt, std::vector<int>& vSimStatus, std::vector<float>& vTheta){

  for(int i=0; i<coll->getNumberOfElements(); i++){
    if (vCharge.size() >= 6){ break; } //Store the 6 first MC particles (for 6f incl. e/p before and after ISR + ISR photons
    EVENT::MCParticle* mcParticle = dynamic_cast<EVENT::MCParticle*>(coll->getElementAt(i));
    if(mcParticle == NULL){ continue; }
    float recoEnergy = mcParticle->getEnergy();
    const double* recoMom = mcParticle->getMomentum(); 
    TLorentzVector tlvReco;
    tlvReco.SetPxPyPzE(recoMom[0], recoMom[1], recoMom[2], recoEnergy); 

    vCharge.push_back(mcParticle->getCharge());
    vEnergy.push_back(recoEnergy);
    vEta.push_back(tlvReco.Eta());
    vGenStatus.push_back(mcParticle->getGeneratorStatus());
    vM.push_back(tlvReco.M());
    vPDG.push_back(mcParticle->getPDG());
    vPhi.push_back(tlvReco.Phi());
    vPt.push_back(tlvReco.Pt());
    vSimStatus.push_back(mcParticle->getSimulatorStatus());
    vTheta.push_back(tlvReco.Theta());
  }
  
}//end getBeamInfo

void TopAsymmetryStoreProcessor::getDaughterCombinations(std::vector<MCParticle*>& daughters, std::vector<std::vector<MCParticle*> >& daughterCombinations){
  int K = 3;
  int N = daughters.size(); //6 in 6-fermion sample

  std::string bitmask(K, 1); // K leading 1's
  bitmask.resize(N, 0); // N-K trailing 0's

  do{
    std::vector<MCParticle*> combination;
    //std::vector<int> combinationInt;
    for (int i = 0; i < N; ++i){ // [0..N-1] integers
      if (bitmask[i]){
	combination.push_back(daughters.at(i));
	//combinationInt.push_back(i);
      }
    }
    daughterCombinations.push_back(combination);
    //pretty_print(combinationInt);
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

}//end getDaughterCombinations

int getQuarkCombinations(int* got, int n_chosen, int len, int at, int max_types, const int* types, std::vector<std::vector<int> >& combs){
  //Source: https://rosettacode.org/wiki/Combinations_with_repetitions#C
  int i;
  int count = 0;
  if (n_chosen == len) {
    if (!got) return 1;
    std::vector<int> comb;
    for (i = 0; i < len; i++){ comb.push_back(types[got[i]]); }
    combs.push_back(comb);
    return 1;
  }
  for (i = at; i < max_types; i++) {
    if (got) got[n_chosen] = i;
    count += getQuarkCombinations(got, n_chosen + 1, len, i, max_types, types, combs);
  }
  return count;
} //end getQuarkCombinations

float getChargeFromPDG(int pdg){
  std::map<int, float> mapPDG2Charge;
  mapPDG2Charge[1] = -1./3.; //d
  mapPDG2Charge[-1] = 1./3.;
  mapPDG2Charge[2] = 2./3.; //u
  mapPDG2Charge[-2] = -2./3.;
  mapPDG2Charge[3] = -1./3.; //s
  mapPDG2Charge[-3] = 1./3.;
  mapPDG2Charge[4] = 2./3.; //c
  mapPDG2Charge[-4] = -2./3.;
  mapPDG2Charge[5] = -1./3.; //b
  mapPDG2Charge[-5] = 1./3.;
  mapPDG2Charge[6] = 2./3.; //top
  mapPDG2Charge[-6] = -2./3.;
  mapPDG2Charge[11] = -1.; //electron
  mapPDG2Charge[-11] = 1.;
  mapPDG2Charge[12] = 0.; //electron neutrino
  mapPDG2Charge[-12] = 0.;
  mapPDG2Charge[13] = -1.; //muon
  mapPDG2Charge[-13] = 1.;
  mapPDG2Charge[14] = 0.; //muon neutrino
  mapPDG2Charge[-14] = 0.;
  mapPDG2Charge[15] = -1.; //tau
  mapPDG2Charge[-15] = 1.;
  mapPDG2Charge[16] = 0.; //tau neutrino
  mapPDG2Charge[-16] = 0.;
  return mapPDG2Charge[pdg];
}

float getChargeSum(std::vector<int>& combination){
  float sumCharge = 0.;
  for (std::vector<int>::iterator pdg = combination.begin(); pdg != combination.end(); ++pdg){ sumCharge += getChargeFromPDG(*pdg); }
  return sumCharge;
}

//Sort will not work if this is a member function
template <typename T>
double getEventInvMass(std::vector<T>& v){
  TLorentzVector tv;
  for (typename std::vector<T>::iterator iter=v.begin(); iter!=v.end(); ++iter){
    TLorentzVector tvi;
    const double* mom = (*iter)->getMomentum();
    tvi.SetPxPyPzE(mom[0], mom[1], mom[2], (*iter)->getEnergy());
    tv += tvi;
  }
  return tv.M();
}

template <typename T>
double TopAsymmetryStoreProcessor::getCombinationCharge(std::vector<T>& v){
  double totCharge = 0.;
  for (typename std::vector<T>::iterator iter=v.begin(); iter!=v.end(); ++iter){
    double charge = (*iter)->getCharge();
    totCharge += charge;
  }
  return totCharge;
}

void TopAsymmetryStoreProcessor::getImpactParametersAndTrackEnergy( ReconstructedParticle*& pfo, float& d0, float& z0, float& r0, int& ntrk) {
  const EVENT::TrackVec & trkvec = pfo->getTracks();
  ntrk = trkvec.size();
  if (trkvec.size()==0) {
    d0=-9999.;
    z0=-9999.;
    r0=-9999.;
    return;
  } 

  // TODO: more sophisticated pfo/track matching
  d0 = fabs(trkvec[0]->getD0());
  z0 = fabs(trkvec[0]->getZ0());
  r0 = sqrt( d0*d0 + z0*z0 );
    
  return;
}

float TopAsymmetryStoreProcessor::getConeEnergy(LCCollection* pfoCol, ReconstructedParticle*& pfo, float cosConeAngle ) {
  float coneE = 0;
  TVector3 P( pfo->getMomentum() );
  int npfo = pfoCol->getNumberOfElements();
  for ( int i = 0; i < npfo; i++ ) {
    ReconstructedParticle* pfo_i = dynamic_cast<ReconstructedParticle*>( pfoCol->getElementAt(i) );
    // don't add itself to the cone energy
    if ( pfo == pfo_i ) continue; 
    TVector3 P_i( pfo_i->getMomentum() );
    float cosTheta = P.Dot( P_i )/(P.Mag()*P_i.Mag());
    if ( cosTheta >= cosConeAngle )
      coneE += pfo_i->getEnergy(); 
  }//end loop on pfo

  return coneE;
}

void TopAsymmetryStoreProcessor::getCalEnergy( ReconstructedParticle*& pfo , float& sumECal, float& sumHCal) {
  sumECal = 0.;
  sumHCal = 0.;
  std::vector<lcio::Cluster*> clusters = pfo->getClusters();
  for ( std::vector<lcio::Cluster*>::const_iterator iCluster=clusters.begin();
	iCluster!=clusters.end();
	++iCluster) {
    sumECal += (*iCluster)->getSubdetectorEnergies()[0];
    sumHCal += (*iCluster)->getSubdetectorEnergies()[1];
  }
  return;
}

float TopAsymmetryStoreProcessor::getRatioEnergyCal( ReconstructedParticle*& pfo) {
  float sumECal = 0.;
  float sumHCal = 0.; 
  this->getCalEnergy(pfo, sumECal, sumHCal);
  return sumECal/(sumECal+sumHCal);
}

float TopAsymmetryStoreProcessor::getEnergyByMomentum( ReconstructedParticle*& pfo) {
  float p = TVector3(pfo->getMomentum()).Mag();
  float sumECal = 0.;
  float sumHCal = 0.;
  this->getCalEnergy(pfo, sumECal, sumHCal);
  return (sumECal+sumHCal)/p;
}

float TopAsymmetryStoreProcessor::getEnergyCal( ReconstructedParticle*& pfo) {
  float sumECal = 0.;
  float sumHCal = 0.; 
  this->getCalEnergy(pfo, sumECal, sumHCal);
  return sumECal+sumHCal;
}

void TopAsymmetryStoreProcessor::isCorrectLepton(LCCollection* LinkerCol, ReconstructedParticle* lepton, std::vector<int>& vNRelObjMC, std::vector<bool>& vRelObjIsLep, std::vector<int>& vRecoIsCorrectLepton, int& nRelObjIsLep){
  //Implementation from Alasadair Winter, University of Birmingham
  int passedTest = 0;
  
  LCRelationNavigator *relationNavigatorPFOMC = new LCRelationNavigator( LinkerCol );
  EVENT::LCObjectVec relobjMC = relationNavigatorPFOMC->getRelatedToObjects(lepton);
  vNRelObjMC.push_back(relobjMC.size());
  for(unsigned int i=0; i<relobjMC.size(); i++){
    MCParticle* mcp = dynamic_cast<MCParticle*>(relobjMC.at(i));
    if (mcp!=NULL){
      if( (abs(mcp->getPDG())==11 || abs(mcp->getPDG())==13) ){
	//std::cout << "PFO pdg: " << mcp->getPDG() << " status: " << mcp->getGeneratorStatus() << std::endl;
	if (mcp->getGeneratorStatus()==1){ //if is really a lepton
	  vRelObjIsLep.push_back(true);
	  nRelObjIsLep += 1;

	  //here on it becomes complicated to define the true lepton- MC not very consistant. Here have used prefix g to denote generator level particles
	  for (MCParticle* testMCP = mcp; testMCP->getParents().size()>0; testMCP=testMCP->getParents()[0]){
	    if(abs(testMCP->getPDG())==5 || abs(testMCP->getPDG())==22){ break; } // Isolated lepton should never have come from a photon or b jet
	    
	    //Case 1:  Initial Electron + Positron -> gPositron+gElectron -> final electron (selects the second 6f sample)
	    if( ( (abs(testMCP->getParents()[0]->getPDG())==11) || (abs(testMCP->getParents()[0]->getPDG())==13) ) // parent is a lepton
		&& testMCP->getParents()[0]->getGeneratorStatus()==102 // parent is generator level particle
		&& testMCP->getParents()[0]->getParents().size()==2 //generator particle produced directly from the e+e- collision
		&& abs(testMCP->getParents()[0]->getParents()[0]->getPDG())==11
		&& abs(testMCP->getParents()[0]->getParents()[1]->getPDG())==11
		&& testMCP->getParents()[0]->getParents()[0]->getParents().size()==2
		&& ( (abs(testMCP->getPDG())==11) || (abs(testMCP->getPDG())==13) )){
	      //std::cout << "PFO passed cat1!" << std::endl;
	      passedTest = 1;
	      break;
	    }
	    
	    //Case 2: gW -> final electron
	    else if ( (abs(testMCP->getParents()[0]->getPDG())==24) //came from W decay
		      && ( (testMCP->getParents()[0]->getGeneratorStatus()==102) || (testMCP->getParents()[0]->getGeneratorStatus()==2) ) // W present at generator level
		      && ( (abs(testMCP->getPDG())==11) || (abs(testMCP->getPDG())==13) )){ // W decayed leptonically
	      //std::cout << "PFO passed cat2!" << std::endl;
	      passedTest = 2;
	      break;
	    }
	    
	    //Case 3: gElectron->g94->final electron
	    else if ( (abs(testMCP->getParents()[0]->getPDG())==94) //came from W like cluster
		      && ( (abs(testMCP->getParents()[0]->getParents()[0]->getPDG())==11) || (abs(testMCP->getParents()[0]->getParents()[0]->getPDG())==13) ) // W appeared from nowhere....
		      && ( (testMCP->getParents()[0]->getGeneratorStatus()==102) || (testMCP->getParents()[0]->getGeneratorStatus()==2) ) // ...but was produced by the generator
		      && ( (abs(testMCP->getPDG())==11) || (abs(testMCP->getPDG())==13) )){ // W decayed leptonically
	      //std::cout << "PFO passed cat3!" << std::endl;
	      passedTest = 3;
	      break;
	    }
	    
	    //else { std::cout << "Event did not pass!" << std::endl; }
	  }
	} else {
	  vRelObjIsLep.push_back(false);
	}
      }
    }
  }
  delete relationNavigatorPFOMC;
  //std::cout << "PassedTest: " << passedTest << std::endl;
  vRecoIsCorrectLepton.push_back(passedTest);
  return;
}

void TopAsymmetryStoreProcessor::isCorrectPhoton(LCCollection* LinkerCol, ReconstructedParticle* photon, std::vector<int>& vNRelObjMC, std::vector<bool>& vRelObjIsPhoton, std::vector<int>& vRecoIsCorrectPhoton, int& nRelObjIsPhoton){

  int passedTest = 0;
  //bool objIsPhoton = false;

  LCRelationNavigator *relationNavigatorPFOMC = new LCRelationNavigator( LinkerCol );
  EVENT::LCObjectVec relobjMC = relationNavigatorPFOMC->getRelatedToObjects(photon);
  vNRelObjMC.push_back(relobjMC.size()); //Number of related objects
  for(unsigned int i=0; i<relobjMC.size(); i++){
    MCParticle* mcp = dynamic_cast<MCParticle*>(relobjMC.at(i));
    if (mcp!=NULL){
      if( abs(mcp->getPDG())==22 ){
	if (mcp->getGeneratorStatus()==1){ //if is really a photon
	  vRelObjIsPhoton.push_back(true);
	  nRelObjIsPhoton += 1;
	  
	  for (MCParticle* testMCP = mcp; testMCP->getParents().size()>0; testMCP=testMCP->getParents()[0]){
	    //std::cout << "PDG: " << testMCP->getParents()[0]->getPDG() << std::endl;
	    //std::cout << "Status: " << testMCP->getParents()[0]->getGeneratorStatus() << std::endl;

	    //Case 1: ee->ee+gamma+gamma->ee+gamma+gamma
	    if( (abs(testMCP->getParents()[0]->getPDG())==22) && (testMCP->getParents()[0]->getGeneratorStatus()==102) ){
	      //std::cout << "PFO passed case 1!" << std::endl;
	      passedTest = 1;
	      break;
	    }
	  }
	} else {
	  vRelObjIsPhoton.push_back(false);
	}
      }
    }
  }
  delete relationNavigatorPFOMC;
  //std::cout << "PassedTest: " << passedTest << std::endl;
  //vRelObjIsPhoton.push_back(objIsPhoton);
  vRecoIsCorrectPhoton.push_back(passedTest);
  return;
}

bool wayToSort(std::vector<std::vector<EVENT::MCParticle*> > lhs, std::vector<std::vector<MCParticle*> > rhs){
  double _topMass = 173.21;
  
  double massLHS0 = TMath::Abs(getEventInvMass(lhs.at(0)) - _topMass);
  double massLHS1 = TMath::Abs(getEventInvMass(lhs.at(1)) - _topMass);
  double meanMassDiffLHS = (massLHS0 + massLHS1)/2.;
  
  double massRHS0 = TMath::Abs(getEventInvMass(rhs.at(0)) - _topMass);
  double massRHS1 = TMath::Abs(getEventInvMass(rhs.at(1)) - _topMass);
  double meanMassDiffRHS = (massRHS0 + massRHS1)/2.;

  return meanMassDiffLHS < meanMassDiffRHS;
}

bool wayToSortPt(TLorentzVector lhs, TLorentzVector rhs){
  return lhs.Pt() > rhs.Pt();
}

bool wayToSortWv(std::pair<TLorentzVector, TLorentzVector> lhs, std::pair<TLorentzVector, TLorentzVector> rhs){
  float _WMass = 80.385;
  
  float massLHS = TMath::Abs(lhs.first.M() - _WMass);
  float massRHS = TMath::Abs(rhs.first.M() - _WMass);
  
  return massLHS < massRHS;
}

template <class T>
void TopAsymmetryStoreProcessor::pretty_print(std::vector<T>& v){
  std::cout << " [ ";
  for (size_t i = 0; i < v.size(); ++i) { std::cout << v[i] << " "; }
  std::cout << "] " << std::endl;
}
