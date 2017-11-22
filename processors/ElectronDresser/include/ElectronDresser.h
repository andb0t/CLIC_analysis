#ifndef ELECTRON_DRESSER_H
#define ELECTRON_DRESSER_H
#include <string>
#include <vector>

#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include "lcio.h"
#include "marlin/Processor.h"
#include "marlin/VerbosityLevels.h"
#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <EVENT/Track.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/ParticleID.h>
#include <UTIL/LCRelationNavigator.h>

using namespace lcio ;
using namespace marlin ;

//written by Andreas Alexander Maier in November 2017
//andreas.alexander.maier@cern.ch


class ElectronDresser : public Processor {

 public:

  virtual Processor*  newProcessor() { return new ElectronDresser ; }

  ElectronDresser() ;

  // Initialisation - run at the beginning to start histograms, etc.
  virtual void init() ;

  // Called at the beginning of every run
  virtual void processRunHeader( LCRunHeader* run ) ;

  // Run over each event - the main algorithm
  virtual void processEvent( LCEvent * evt ) ;

  // Called at the very end for cleanup, histogram saving, etc.
  virtual void end() ;


  // my methods
  void clearEventVariables() ;
  void fillMCInfo(LCEvent * evt) ;
  void fillPFOs(LCEvent * evt) ;
  void fillLeptons(LCEvent * evt) ;
  void dressLeptons(LCEvent * evt) ;
  void fillVectors(std::string collName, ReconstructedParticle* particle);
  void getCollection(LCCollection*&, std::string, LCEvent*);

 protected:

  std::string m_pfos;
  std::string m_mc_particles;
  std::string m_IsolatedLepton;
  std::string m_DressedLepton;

  TLorentzVector fourvec;

  int mc_n;
  std::vector<int> mc_gen_status;
  std::vector<int> mc_type;
  std::vector<double> mc_pt;
  std::vector<double> mc_theta;
  std::vector<double> mc_phi;
  std::vector<double> mc_e;
  std::vector<double> mc_charge;

  int pfo_n;
  std::vector<double> pfo_pt;
  std::vector<double> pfo_theta;
  std::vector<double> pfo_phi;
  std::vector<double> pfo_e;

  int lep_n;
  double lep_etot;
  std::vector<int> lep_type;
  std::vector<double> lep_pt;
  std::vector<double> lep_theta;
  std::vector<double> lep_phi;
  std::vector<double> lep_e;
  std::vector<double> lep_charge;

  int _nRun ;
  int _nEvt ;
} ;

#endif
