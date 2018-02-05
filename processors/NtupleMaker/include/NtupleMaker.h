#ifndef NTUPLE_MAKER_H
#define NTUPLE_MAKER_H
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
#include "lcio.h"
#include "marlin/Processor.h"
#include "marlin/VerbosityLevels.h"

using namespace lcio;
using namespace marlin;

/** A class to write out ntuples for further offline analysis
*
*  @author Andreas A. Maier, CERN
*/

class NtupleMaker : public Processor {
public:
  virtual Processor* newProcessor() { return new NtupleMaker; }

  NtupleMaker();

  // silence warnings on pointer data members
  NtupleMaker(const NtupleMaker&);             // copy constructor
  NtupleMaker& operator=(const NtupleMaker&);  // override

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
  void fillMissingEnergy();
  void fillHistograms();
  void fillBeamEnergy();
  void fillOtherVars();
  void fillRecoParticles(std::string collName, LCEvent* evt);
  void fillVectors(std::string collName, ReconstructedParticle* particle);
  void             getCollection(LCCollection*&, std::string, LCEvent*);
  std::vector<int> orderByPt(LCCollection*);

protected:
  //parameters
  std::string _m_outfileName{};

  //collections
  std::string              _m_mc_particles{};
  std::string              _m_IsolatedLepton{};
  std::string              _m_DressedLepton{};
  std::string              _m_kt_R07{};
  std::string              _m_kt_R10{};
  std::string              _m_kt_R12{};
  std::string              _m_vlc_R06{};
  std::string              _m_vlc_R08{};
  std::string              _m_vlc_R10{};
  std::string              _m_vlc_R08_g05{};
  std::string              _m_vlc_R08_g10{};
  std::vector<std::string> _recoInputCollections{};

  TTree* _rawTree    = nullptr;
  TFile* _ntupleFile = nullptr;

  //control histograms
  TH1F* _hist_m_W_lep = nullptr;
  TH1F* _hist_m_W_had = nullptr;

  //branches
  TLorentzVector _fourvec{};
  TLorentzVector _tmp0vec{};
  TLorentzVector _tmp1vec{};
  TLorentzVector _tmp2vec{};
  TLorentzVector _tmp3vec{};

  double _miss_pt    = 0.0;
  double _miss_theta = 0.0;
  double _miss_phi   = 0.0;
  double _miss_e     = 0.0;

  double _qq_m   = 0.0;
  double _ln_m   = 0.0;
  double _beam_e = 0.0;
  double _beam_m = 0.0;

  double              _mc_beam_e = 0.0;
  double              _mc_beam_m = 0.0;
  double              _mc_qq_m   = 0.0;
  double              _mc_ln_m   = 0.0;
  int                 _mc_n      = 0;
  std::vector<int>    _mc_gen_status{};
  std::vector<int>    _mc_type{};
  std::vector<double> _mc_pt{};
  std::vector<double> _mc_theta{};
  std::vector<double> _mc_phi{};
  std::vector<double> _mc_e{};
  std::vector<double> _mc_charge{};

  int                 _lep_n    = 0;
  double              _lep_etot = 0.0;
  std::vector<int>    _lep_type{};
  std::vector<double> _lep_pt{};
  std::vector<double> _lep_theta{};
  std::vector<double> _lep_phi{};
  std::vector<double> _lep_e{};
  std::vector<double> _lep_charge{};

  int                 _lep_dressed_n    = 0;
  double              _lep_dressed_etot = 0.0;
  std::vector<int>    _lep_dressed_type{};
  std::vector<double> _lep_dressed_pt{};
  std::vector<double> _lep_dressed_theta{};
  std::vector<double> _lep_dressed_phi{};
  std::vector<double> _lep_dressed_e{};
  std::vector<double> _lep_dressed_charge{};

  int                 _jet_kt_R07_n    = 0;
  double              _jet_kt_R07_etot = 0.0;
  std::vector<double> _jet_kt_R07_pt{};
  std::vector<double> _jet_kt_R07_theta{};
  std::vector<double> _jet_kt_R07_phi{};
  std::vector<double> _jet_kt_R07_e{};
  std::vector<double> _jet_kt_R07_charge{};

  int                 _jet_kt_R10_n    = 0;
  double              _jet_kt_R10_etot = 0.0;
  std::vector<double> _jet_kt_R10_pt{};
  std::vector<double> _jet_kt_R10_theta{};
  std::vector<double> _jet_kt_R10_phi{};
  std::vector<double> _jet_kt_R10_e{};
  std::vector<double> _jet_kt_R10_charge{};

  int                 _jet_kt_R12_n    = 0;
  double              _jet_kt_R12_etot = 0.0;
  std::vector<double> _jet_kt_R12_pt{};
  std::vector<double> _jet_kt_R12_theta{};
  std::vector<double> _jet_kt_R12_phi{};
  std::vector<double> _jet_kt_R12_e{};
  std::vector<double> _jet_kt_R12_charge{};

  int                 _jet_vlc_R06_n    = 0;
  double              _jet_vlc_R06_etot = 0.0;
  std::vector<double> _jet_vlc_R06_pt{};
  std::vector<double> _jet_vlc_R06_theta{};
  std::vector<double> _jet_vlc_R06_phi{};
  std::vector<double> _jet_vlc_R06_e{};
  std::vector<double> _jet_vlc_R06_charge{};

  int                 _jet_vlc_R08_n    = 0;
  double              _jet_vlc_R08_etot = 0.0;
  std::vector<double> _jet_vlc_R08_pt{};
  std::vector<double> _jet_vlc_R08_theta{};
  std::vector<double> _jet_vlc_R08_phi{};
  std::vector<double> _jet_vlc_R08_e{};
  std::vector<double> _jet_vlc_R08_charge{};

  int                 _jet_vlc_R10_n    = 0;
  double              _jet_vlc_R10_etot = 0.0;
  std::vector<double> _jet_vlc_R10_pt{};
  std::vector<double> _jet_vlc_R10_theta{};
  std::vector<double> _jet_vlc_R10_phi{};
  std::vector<double> _jet_vlc_R10_e{};
  std::vector<double> _jet_vlc_R10_charge{};

  int                 _jet_vlc_R08_g05_n    = 0;
  double              _jet_vlc_R08_g05_etot = 0.0;
  std::vector<double> _jet_vlc_R08_g05_pt{};
  std::vector<double> _jet_vlc_R08_g05_theta{};
  std::vector<double> _jet_vlc_R08_g05_phi{};
  std::vector<double> _jet_vlc_R08_g05_e{};
  std::vector<double> _jet_vlc_R08_g05_charge{};

  int                 _jet_vlc_R08_g10_n    = 0;
  double              _jet_vlc_R08_g10_etot = 0.0;
  std::vector<double> _jet_vlc_R08_g10_pt{};
  std::vector<double> _jet_vlc_R08_g10_theta{};
  std::vector<double> _jet_vlc_R08_g10_phi{};
  std::vector<double> _jet_vlc_R08_g10_e{};
  std::vector<double> _jet_vlc_R08_g10_charge{};

  int _nRun = 0;
  int _nEvt = 0;
};

#endif
