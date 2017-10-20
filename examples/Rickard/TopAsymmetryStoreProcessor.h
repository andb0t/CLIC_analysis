/*
 * TopAsymmetryStoreProcessor.cpp
 *
 *  Created on: 22.11.2016
 *      Author: Rickard Stroem (CERN) - lars.rickad.stroem@cern.ch
 *           ROOT Booking Processor for Top Asymmetry Analysis
 */

#ifndef TOPASYMMETRYPROCESSOR_H_
#define TOPASYMMETRYPROCESSOR_H_

#include <marlin/Processor.h>
#include <marlin/VerbosityLevels.h>

#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/ReconstructedParticle.h>
#include <LCIOSTLTypes.h>

#include <fastjet/PseudoJet.hh>
#include <fastjet/JetDefinition.hh>
#include <fastjet/tools/JHTopTagger.hh>

#include <EVENT/MCParticle.h>

#include <TH1F.h>
#include <TFile.h>
#include <TLorentzVector.h>

//General functions

template <typename T>
double getEventInvMass(std::vector<T>& v);

template <class T>
void getKinematics(T*& obj, std::vector<float > &objPt, std::vector<float > &objEta, std::vector<float > &objPhi, std::vector<float > &objM, std::vector<float > &objTheta, std::vector<int > &objType, std::vector<float> &objCharge, std::vector<float> &objEnergy);

void getBeamInfo(LCCollection* coll, std::vector<float>& vCharge, std::vector<float>& vEnergy, std::vector<float>& vEta, std::vector<int>& vGenStatus, std::vector<float>& vM, std::vector<int>& vPDG, std::vector<float>& vPhi, std::vector<float>& vPt, std::vector<int>& vSimStatus, std::vector<float>& vTheta);

int getQuarkCombinations(int* got, int n_chosen, int len, int at, int max_types, const int* types, std::vector<std::vector<int> >& combs);
float getChargeSum(std::vector<int>& combination);
float getChargeFromPDG(int pdg);

bool wayToSort(std::vector<std::vector<EVENT::MCParticle*> > lhs, std::vector<std::vector<MCParticle*> > rhs);
bool wayToSortPt(TLorentzVector lhs, TLorentzVector rhs);
bool wayToSortWv(std::pair<TLorentzVector, TLorentzVector> lhs, std::pair<TLorentzVector, TLorentzVector> rhs);

class TTree;
class FastJetUtil;

class TopAsymmetryStoreProcessor : marlin::Processor {

 public:
  TopAsymmetryStoreProcessor();
  virtual ~TopAsymmetryStoreProcessor();
  
  virtual Processor* newProcessor(){ return new TopAsymmetryStoreProcessor();	}

  virtual void init();
  virtual void processRunHeader(LCRunHeader*);
  virtual void processEvent(LCEvent * evt);
  virtual void check(LCEvent * ) {};
  virtual void end();

  TopAsymmetryStoreProcessor(const TopAsymmetryStoreProcessor &); // copy constructor
  TopAsymmetryStoreProcessor & operator = (const TopAsymmetryStoreProcessor &);

  void clearVectors();
  void clearNumbers();
  
  void getDaughterCombinations(std::vector<MCParticle*>& daughters, std::vector<std::vector<MCParticle*> >& daughterCombinations);

  bool isElectron(int& id);
  bool isMuon(int& id);
  bool isTau(int& id);
  bool isPhoton(int& id);

  void isCorrectLepton(LCCollection* LinkerCol, ReconstructedParticle* lepton, std::vector<int>& vNRelObjMC, std::vector<bool>& vRelObjIsLep, std::vector<int>& vRecoIsCorrectLepton, int& nRelObjIsLep);
  void isCorrectPhoton(LCCollection* LinkerCol, ReconstructedParticle* photon, std::vector<int>& vNRelObjMC, std::vector<bool>& vRelObjIsPhoton, std::vector<int>& vRecoIsCorrectPhoton, int& nRelObjIsPhoton);

  void getCalEnergy( ReconstructedParticle*& pfo , float& sumECal, float& sumHCal);
  float getEnergyCal( ReconstructedParticle*& pfo);
  void getImpactParametersAndTrackEnergy( ReconstructedParticle*& pfo, float& d0, float& z0, float& r0, int& ntrk);
  float getConeEnergy( LCCollection* pfoCol, ReconstructedParticle*& pfo, float cosConeAngle );
  float getRatioEnergyCal( ReconstructedParticle*& pfo);
  float getEnergyByMomentum( ReconstructedParticle*& pfo);

  void convertFromRecParticle(LCCollection* recCol);

  template <typename T>
    double getCombinationCharge(std::vector<T>& v);

  template <class T>
    void pretty_print(std::vector<T>& v);

 private:
  
  std::string _mcpColName;
  std::string _recoMCTruthLinkName;
  std::string _generalParticleInName;

  std::string _isolepParticleElectronInName;
  std::string _isolepParticleMuonInName;
  std::string _isolepParticleInName;

  std::string _toptaggerJetInName;
  std::string _toptaggerInName;
  std::string _toptaggerInName_W;
  std::string _toptaggerInName_nonW;
  std::string _toptaggerInName_W1;
  std::string _toptaggerInName_W2;
  std::string _toptaggerInName_cos_theta_W;

  std::string _toptaggerJetInName_v2;
  std::string _toptaggerInName_v2;
  std::string _toptaggerInName_v2_W;
  std::string _toptaggerInName_v2_nonW;
  std::string _toptaggerInName_v2_W1;
  std::string _toptaggerInName_v2_W2;
  std::string _toptaggerInName_v2_cos_theta_W;

  std::string _jetInNameJ4;
  std::string _jetInNameJ4_v2;
  std::string _jetInNameJ6;
  std::string _jetInNameJ6_v2;
  std::string _vertexJetsInName;
  std::string _vertexJets3InName;

  std::string _outFileName;
  std::string _treeName;

  bool _useSixFermionSample;

  //Root tree
  TFile* _out;
  TTree* _tree;

  //Event variabels
  int _nRun;
  int _nEvt;

  double _Et;
  double _minPt;
  int _nPFO;
  int _nPhotons;
  int _nPhotonswoCut;

  int _nMCP;
  double _eventInvMass;

  int _mcLepType;
  bool _singleTop;	
  bool _compatibleWith6f;	

  int _nTopSideLep;
  int _nTopSideNu;
  int _nTopSidebQuark;
  int _nTopSideLightQuark;
  float _topLeptonVSbAngle;
  float _topLeptonVSWAngle;
  float _topNeutrinoVSWAngle;
  float _topbQuarkVSWAngle;

  int _nAntiTopSideLep;
  int _nAntiTopSideNu;
  int _nAntiTopSidebQuark;
  int _nAntiTopSideLightQuark;
  float _antiTopLeptonVSbAngle;
  float _antiTopLeptonVSWAngle;
  float _antiTopNeutrinoVSWAngle;
  float _antiTopbQuarkVSWAngle;

  std::vector<float> _vGenBeamCharge;
  std::vector<float> _vGenBeamEnergy;
  std::vector<float> _vGenBeamEta;
  std::vector<int> _vGenBeamGenStatus;
  std::vector<float> _vGenBeamM;
  std::vector<int> _vGenBeamPDG;
  std::vector<float> _vGenBeamPhi;
  std::vector<float> _vGenBeamPt;
  std::vector<int> _vGenBeamSimStatus;
  std::vector<float> _vGenBeamTheta;
  
  std::vector<float> _vRecoPt;
  std::vector<float> _vRecoEta;
  std::vector<float> _vRecoPhi;
  std::vector<float> _vRecoMass;
  std::vector<float> _vRecoTheta;
  std::vector<int> _vRecoType;
  std::vector<float> _vRecoCharge;
  std::vector<float> _vRecoEnergy;

  std::vector<float > _vRecoECal;
  std::vector<float > _vRecoRCal;
  std::vector<float > _vRecoEnergyByMom;  

  std::vector<float > _vRecoD0;
  std::vector<float > _vRecoZ0;
  std::vector<float > _vRecoR0;
  std::vector<int > _vRecoNTrkObj;

  std::vector<float > _vRecoECone900;
  std::vector<float > _vRecoECone950;
  std::vector<float > _vRecoECone990;
  std::vector<float > _vRecoECone995;
  std::vector<float > _vRecoECone999;
  std::vector<float > _vRecoECone9995;

  int _nRelObjIsLep;
  std::vector<int> _vNRelObjMC;
  std::vector<bool> _vRelObjIsLep;
  std::vector<int> _vRecoIsCorrectLepton;

  int _nRelObjIsPhoton;
  std::vector<int> _vNRelObjMCPhoton;
  std::vector<bool> _vRelObjIsPhoton;
  std::vector<int> _vRecoIsCorrectPhoton;

  int _nLeptons;
  int _nElectron;
  int _nMuon;
  int _nJets;
  int _nJets_v2;
  int _nJetsJ4;
  int _nJetsJ4_v2;
  int _nJetsJ6;
  int _nJetsJ6_v2;
  int _nTopTagged;
  int _nTopTagged_v2;

  double _jetMassTot;
  double _jetMassHigh2Pt;
  double _jetMassTot_v2;
  double _jetMassHigh2Pt_v2;

  double _jetMassJ4Tot;
  double _jetMassJ4High2Pt;
  double _jetMassJ4Tot_v2;
  double _jetMassJ4High2Pt_v2;

  double _jetMassJ6Tot;
  double _jetMassJ6High2Pt;
  double _jetMassJ6Tot_v2;
  double _jetMassJ6High2Pt_v2;

  double _sumBTag;
  double _sumCTag;
  double _sumBCTag;
  double _y23;
  double _y34;
  double _y45;
  double _y56;

  double _sumBTagJ3;
  double _sumCTagJ3;
  double _sumBCTagJ3;
  double _y23J3;
  double _y34J3;
  double _y45J3;
  double _y56J3;

  double _invMassFlavourTag;
  double _invMassFlavourTagwCut050;
  double _invMassFlavourTagwCut075;
  double _invMassFlavourTagwCut090;

  double _invMassFlavourTagJ3;
  double _invMassFlavourTagJ3wCut050;
  double _invMassFlavourTagJ3wCut075;
  double _invMassFlavourTagJ3wCut090;
  
  std::vector<fastjet::PseudoJet> _pjList;
  std::vector<MCParticle*> _daughters;
  std::vector<std::vector<MCParticle*> > _daughterCombinations;  
  std::vector<MCParticle*> _topFermions;
  std::vector<MCParticle*> _antitopFermions;

  //Generator info - top
  std::vector<float> _vGenTopCharge;
  std::vector<int> _vGenTopType;
  std::vector<float> _vGenTopPt;
  std::vector<float> _vGenTopEnergy;
  std::vector<float> _vGenTopEta;
  std::vector<float> _vGenTopMass;
  std::vector<float> _vGenTopTheta;
  std::vector<float> _vGenTopPhi;

  //Generator info - lep
  std::vector<float> _vGenLepPt;
  std::vector<float> _vGenLepEta;
  std::vector<float> _vGenLepPhi;
  std::vector<float> _vGenLepMass;
  std::vector<float> _vGenLepTheta;
  std::vector<int> _vGenLepType;
  std::vector<float> _vGenLepCharge;
  std::vector<float> _vGenLepEnergy;
  
  //Lepton isolation info
  std::vector<float> _vLepCharge;
  std::vector<int> _vLepType;
  std::vector<float> _vLepPt;
  std::vector<float> _vLepEnergy;
  std::vector<float> _vLepEta;
  std::vector<float> _vLepMass;
  std::vector<float> _vLepTheta;
  std::vector<float> _vLepPhi;

  std::vector<float> _vLepElectronCharge;
  std::vector<int> _vLepElectronType;
  std::vector<float> _vLepElectronPt;
  std::vector<float> _vLepElectronEnergy;
  std::vector<float> _vLepElectronEta;
  std::vector<float> _vLepElectronMass;
  std::vector<float> _vLepElectronTheta;
  std::vector<float> _vLepElectronPhi;

  std::vector<float> _vLepMuonCharge;
  std::vector<int> _vLepMuonType;
  std::vector<float> _vLepMuonPt;
  std::vector<float> _vLepMuonEnergy;
  std::vector<float> _vLepMuonEta;
  std::vector<float> _vLepMuonMass;
  std::vector<float> _vLepMuonTheta;
  std::vector<float> _vLepMuonPhi;

  //Photon info
  std::vector<float> _vPhotonCharge;
  std::vector<int> _vPhotonType;
  std::vector<float> _vPhotonPt;
  std::vector<float> _vPhotonEnergy;
  std::vector<float> _vPhotonEta;
  std::vector<float> _vPhotonMass;
  std::vector<float> _vPhotonTheta;
  std::vector<float> _vPhotonPhi;

  //Jet info
  std::vector<float> _vJetCharge;
  std::vector<int> _vJetType;
  std::vector<float> _vJetPt;
  std::vector<float> _vJetEnergy;
  std::vector<float> _vJetEta;
  std::vector<float> _vJetMass;
  std::vector<float> _vJetTheta;
  std::vector<float> _vJetPhi;

  std::vector<float> _vJetCharge_v2;
  std::vector<int> _vJetType_v2;
  std::vector<float> _vJetPt_v2;
  std::vector<float> _vJetEnergy_v2;
  std::vector<float> _vJetEta_v2;
  std::vector<float> _vJetMass_v2;
  std::vector<float> _vJetTheta_v2;
  std::vector<float> _vJetPhi_v2;

  std::vector<float> _vJetChargeJ4;
  std::vector<int> _vJetTypeJ4;
  std::vector<float> _vJetPtJ4;
  std::vector<float> _vJetEnergyJ4;
  std::vector<float> _vJetEtaJ4;
  std::vector<float> _vJetMassJ4;
  std::vector<float> _vJetThetaJ4;
  std::vector<float> _vJetPhiJ4;
  std::vector<float> _vJetMassJ4Comb2;
  std::vector<float> _vJetMassJ4Comb3;

  std::vector<float> _vJetChargeJ4_v2;
  std::vector<int> _vJetTypeJ4_v2;
  std::vector<float> _vJetPtJ4_v2;
  std::vector<float> _vJetEnergyJ4_v2;
  std::vector<float> _vJetEtaJ4_v2;
  std::vector<float> _vJetMassJ4_v2;
  std::vector<float> _vJetThetaJ4_v2;
  std::vector<float> _vJetPhiJ4_v2;
  std::vector<float> _vJetMassJ4Comb2_v2;
  std::vector<float> _vJetMassJ4Comb3_v2;

  std::vector<float> _vJetChargeJ6;
  std::vector<int> _vJetTypeJ6;
  std::vector<float> _vJetPtJ6;
  std::vector<float> _vJetEnergyJ6;
  std::vector<float> _vJetEtaJ6;
  std::vector<float> _vJetMassJ6;
  std::vector<float> _vJetThetaJ6;
  std::vector<float> _vJetPhiJ6;
  std::vector<float> _vJetMassJ6Comb2;
  std::vector<float> _vJetMassJ6Comb3;

  std::vector<float> _vJetChargeJ6_v2;
  std::vector<int> _vJetTypeJ6_v2;
  std::vector<float> _vJetPtJ6_v2;
  std::vector<float> _vJetEnergyJ6_v2;
  std::vector<float> _vJetEtaJ6_v2;
  std::vector<float> _vJetMassJ6_v2;
  std::vector<float> _vJetThetaJ6_v2;
  std::vector<float> _vJetPhiJ6_v2;
  std::vector<float> _vJetMassJ6Comb2_v2;
  std::vector<float> _vJetMassJ6Comb3_v2;
  
  //Top tagging
  std::vector<int> _vTopTagged;
  std::vector<float> _vTopTaggedWCosTheta;
  
  std::vector<float> _vTopCharge;
  std::vector<int> _vTopType;
  std::vector<float> _vTopPt;
  std::vector<float> _vTopEnergy;
  std::vector<float> _vTopEta;
  std::vector<float> _vTopMass;
  std::vector<float> _vTopTheta;
  std::vector<float> _vTopPhi;

  std::vector<float> _vTopChargeW;
  std::vector<int> _vTopTypeW;
  std::vector<float> _vTopPtW;
  std::vector<float> _vTopEnergyW;
  std::vector<float> _vTopEtaW;
  std::vector<float> _vTopMassW;
  std::vector<float> _vTopThetaW;
  std::vector<float> _vTopPhiW;

  std::vector<int> _vTopTagged_v2;
  std::vector<float> _vTopTaggedWCosTheta_v2;
  
  std::vector<float> _vTopCharge_v2;
  std::vector<int> _vTopType_v2;
  std::vector<float> _vTopPt_v2;
  std::vector<float> _vTopEnergy_v2;
  std::vector<float> _vTopEta_v2;
  std::vector<float> _vTopMass_v2;
  std::vector<float> _vTopTheta_v2;
  std::vector<float> _vTopPhi_v2;

  std::vector<float> _vTopChargeW_v2;
  std::vector<int> _vTopTypeW_v2;
  std::vector<float> _vTopPtW_v2;
  std::vector<float> _vTopEnergyW_v2;
  std::vector<float> _vTopEtaW_v2;
  std::vector<float> _vTopMassW_v2;
  std::vector<float> _vTopThetaW_v2;
  std::vector<float> _vTopPhiW_v2;

  //Flavour Tagging
  std::vector<float> _vBTag;
  std::vector<float> _vCTag;
  std::vector<float> _vBCTag;

  std::vector<float> _vBTagJ3;
  std::vector<float> _vCTagJ3;
  std::vector<float> _vBCTagJ3;

  std::vector<float> _vJetChargeFlavourTag;
  std::vector<int> _vJetTypeFlavourTag;
  std::vector<float> _vJetPtFlavourTag;
  std::vector<float> _vJetEnergyFlavourTag;
  std::vector<float> _vJetEtaFlavourTag;
  std::vector<float> _vJetMassFlavourTag;
  std::vector<float> _vJetThetaFlavourTag;
  std::vector<float> _vJetPhiFlavourTag;

  std::vector<float> _vJetChargeFlavourTagJ3;
  std::vector<int> _vJetTypeFlavourTagJ3;
  std::vector<float> _vJetPtFlavourTagJ3;
  std::vector<float> _vJetEnergyFlavourTagJ3;
  std::vector<float> _vJetEtaFlavourTagJ3;
  std::vector<float> _vJetMassFlavourTagJ3;
  std::vector<float> _vJetThetaFlavourTagJ3;
  std::vector<float> _vJetPhiFlavourTagJ3;

  //General definitions
  int _pdg_d;
  int _pdg_u;
  int _pdg_s;
  int _pdg_c;
  int _pdg_b;
  int _pdg_t;
  
  int _pdg_electron;
  int _pdg_velectron;
  int _pdg_muon;
  int _pdg_vmuon;
  int _pdg_tau;
  int _pdg_vtau;
  
  int _pdg_photon;
  int _pdg_Z;
  int _pdg_W;

  double _topMass;
  double _sigmaTopMass;

  std::vector<std::vector<int> > _finalStates;
};

#endif /* TOPASYMMETRYPROCESSOR_H_ */
