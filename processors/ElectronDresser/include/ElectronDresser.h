#ifndef ELECTRON_DRESSER_H
#define ELECTRON_DRESSER_H
#include <string>
#include <vector>

#include "TFile.h"
#include "TH1F.h"
#include "TLorentzVector.h"
#include "TTree.h"

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <EVENT/ParticleID.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Track.h>
#include <UTIL/LCRelationNavigator.h>
#include "IMPL/LCCollectionVec.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "lcio.h"
#include "marlin/Processor.h"
#include "marlin/VerbosityLevels.h"

using namespace lcio;
using namespace marlin;

//written by Andreas Alexander Maier in November 2017
//andreas.alexander.maier@cern.ch

class ElectronDresser : public Processor {
public:
  virtual Processor* newProcessor() { return new ElectronDresser; }

  ElectronDresser();

  // Initialisation - run at the beginning to start histograms, etc.
  virtual void init();

  // Called at the beginning of every run
  virtual void processRunHeader(LCRunHeader* run);

  // Run over each event - the main algorithm
  virtual void processEvent(LCEvent* evt);

  // Called at the very end for cleanup, histogram saving, etc.
  virtual void end();

  // my methods
  void clearEventVariables();
  void fillMCInfo(LCEvent* evt);
  void fillPFOs(LCEvent* evt);
  void fillLeptons(LCEvent* evt);
  void dressLeptons(LCEvent* evt);
  void fillVectors(std::string collName, ReconstructedParticle* particle);
  void getCollection(LCCollection*&, std::string, LCEvent*);

  const double ELECTRON_DRESS_MAX_DR = 0.2;

protected:
  std::string _m_pfos{};
  std::string _m_mc_particles{};
  std::string _m_IsolatedLepton{};
  std::string _m_DressedLepton{};

  TLorentzVector _fourvec{};
  TLorentzVector _tmpvec{};

  int                 _mc_n = 0;
  std::vector<int>    _mc_gen_status{};
  std::vector<int>    _mc_type{};
  std::vector<double> _mc_pt{};
  std::vector<double> _mc_theta{};
  std::vector<double> _mc_eta{};
  std::vector<double> _mc_phi{};
  std::vector<double> _mc_e{};
  std::vector<double> _mc_charge{};

  int                 _pfo_n = 0;
  std::vector<int>    _pfo_type{};
  std::vector<double> _pfo_pt{};
  std::vector<double> _pfo_theta{};
  std::vector<double> _pfo_eta{};
  std::vector<double> _pfo_phi{};
  std::vector<double> _pfo_e{};
  std::vector<double> _pfo_charge{};

  int                 _lep_n = 0;
  double              _lep_etot{};
  std::vector<int>    _lep_type{};
  std::vector<double> _lep_pt{};
  std::vector<double> _lep_theta{};
  std::vector<double> _lep_eta{};
  std::vector<double> _lep_phi{};
  std::vector<double> _lep_e{};
  std::vector<double> _lep_charge{};

  int _nRun = 0;
  int _nEvt = 0;
};

#endif
