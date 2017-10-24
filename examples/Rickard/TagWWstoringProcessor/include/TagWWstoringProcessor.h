 /*
 * TagWWstoringProcessor.cpp
 *
 *  Created on: 22.11.2016
 *      Author: Rickard Stroem (CERN) - lars.rickad.stroem@cern.ch
 *           ROOT Booking Processor for Top Asymmetry Analysis
 */

#ifndef TagWWstoringProcessor_H_
#define TagWWstoringProcessor_H_

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

class TTree;
class FastJetUtil;

class TagWWstoringProcessor : marlin::Processor {

 public:
  TagWWstoringProcessor();
  virtual ~TagWWstoringProcessor();
  
  virtual Processor* newProcessor(){ return new TagWWstoringProcessor();	}

  TagWWstoringProcessor(const TagWWstoringProcessor &); // copy constructor
  TagWWstoringProcessor & operator = (const TagWWstoringProcessor &);

  void clearVectors();
  void clearNumbers();
  
  template <class T>
    void getKinematics(T*& obj, std::vector<float>& objCharge, std::vector<float>& objType, std::vector<float>& objPt, std::vector<float>& objEnergy, std::vector<float>& objEta, std::vector<float>& objMass, std::vector<float>& objTheta, std::vector<float>& objPhi);

  int getChoose(int n, int k);
  void getCombinations(int offset, int k, std::vector<int>& o, std::vector<int>& c, std::vector<std::vector<int> >& cs);
  bool isElectron(int& id);

  template <typename T>
    double getEventInvMass(std::vector<T>& v);

  bool isISR(std::vector<EVENT::MCParticle*>& mcp);
  void convertFromRecParticle(LCCollection* recCol);
	bool isQuark(std::vector<MCParticle*> _daughters);
	TLorentzVector mcToTLV(MCParticle* particle);
	TLorentzVector recoToTLV(ReconstructedParticle* particle);

  template <class T>
    void pretty_print(std::vector<T>& v);

  virtual void init();
  virtual void processRunHeader(LCRunHeader*);
  virtual void processEvent(LCEvent * evt);
  virtual void check(LCEvent * ) {};
  virtual void end();

 private:
  
  std::string _mcpColName;
  std::string _recoColName;
	std::string _jetColFat;
	std::string _jetColFatTrimmed;
	std::string _jetColSmall;
	std::string _particleInCol;

  std::string _outFileName;
  std::string _treeName;


  //Event variabels
  int _nRun;
  int _nEvt;
  

  //Root tree
  TFile* _out;
  TTree* _tree;

  int _nPFO;
  int _nMCP;

  double _minPt;
  double _Et;
  double _ev_inv_mass;

	std::vector<MCParticle*> _daughters;
	std::vector<ReconstructedParticle*> _fatJetCol;
	std::vector<ReconstructedParticle*> _fatJetColTrimmed;
	std::vector<ReconstructedParticle*> _smallJetCol;
	
	std::vector<TLorentzVector> _smallJets;

	std::vector<ReconstructedParticle*> _particleCol;
	std::vector<std::vector<float>> _particlesInEvent;

  std::vector<float> _jetCharge;
  std::vector<float> _jetType;
  std::vector<float> _fatJetPt;
  std::vector<float> _jetEnergy;
  std::vector<float> _jetEta;
  std::vector<float> _jetMass;
  std::vector<float> _jetTheta;
  std::vector<float> _jetPhi;

	std::vector<float> _smallJetPt;
	std::vector<float> _smallJetMass;
	std::vector<float> _smallJetPtComb;
	std::vector<float> _smallJetMassComb;

	std::vector<float> _smallJetMassW;
	std::vector<float> _smallJetMassZ;

	std::vector<float> _fatJetMassW;
	std::vector<float> _fatJetMassZ;

	std::vector<float> _vecJetQuark;

	int _WZtag;
	int _singleWZtag;

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

  double _t_mass;
  double _w_t_mass;
  double _w_mass;
  double _z_mass;
  double _w_width;
  double _z_width;

	TLorentzVector jet1;
	TLorentzVector jet2;
	TLorentzVector trimmedJet1;
	TLorentzVector trimmedJet2;
	TLorentzVector smalljet1;
	TLorentzVector smalljet2;
	TLorentzVector smalljet3;
	TLorentzVector smalljet4;

	TLorentzVector _smallJetComb1;
	TLorentzVector _smallJetComb2;

	TLorentzVector _mcFatJet1;
	TLorentzVector _mcFatJet2;

	TLorentzVector quark1;
	TLorentzVector quark2;
	TLorentzVector quark3;
	TLorentzVector quark4;

	TLorentzVector quark12;
	TLorentzVector quark34;
	TLorentzVector quark13;
	TLorentzVector quark24;
	TLorentzVector quark14;
	TLorentzVector quark23;


	float charge1;
	float charge2;
	float charge3;
	float charge4;

	float flavour1;
	float flavour2;
	float flavour3;
	float flavour4;
	
	std::vector<float> _fatJetMass;

	std::vector<float> _deltaRqq;
	float _deltaRqqqq;
	float _deltaRjj;
	std::vector<float> _deltaPhiqq;
	float _deltaPhiqqqq;
	float _deltaPhijj;
	std::vector<float> _deltaTheta;
	std::vector<float> _deltaRtheta;



  std::vector< fastjet::PseudoJet > _pjList;

	bool _use_4jet;
	bool _useTrimming;
	bool _useAllParticles;

};

#endif /* TOPASYMMETRYPROCESSOR_H_ */
