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


/**  Example processor for marlin.
 *
 *  <h4>Input - Prerequisites</h4>
 *  Needs the collection of MCParticles.
 *
 *  <h4>Output</h4>
 *  A histogram.
 *
 * @param CollectionName Name of the MCParticle collection
 *
 * @author F. Gaede, DESY
 * @version $Id: ElectronDresser.h,v 1.4 2005-10-11 12:57:39 gaede Exp $
 */

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

  void clear_event_variables() ;
  void fill_mc_info(LCEvent * evt) ;
  void fill_missing_energy(LCEvent * evt) ;
  void fill_reco_particles(std::string collName, LCEvent * evt) ;
  void fill_vectors(std::string collName, ReconstructedParticle* particle);
  void get_collection(LCCollection*&, std::string, LCEvent*);
  std::vector<int> order_by_pt(LCCollection*);

 protected:

  //parameters
  std::string m_outfileName;

  //collections
  std::string m_pfos;
  std::string m_mc_particles;
  std::string m_IsolatedLepton;
  std::string m_kt_R07;
  std::string m_kt_R10;
  std::string m_kt_R12;
  std::string m_vlc_R06;
  std::string m_vlc_R08;
  std::string m_vlc_R10;
  std::string m_vlc_R08_g05;
  std::string m_vlc_R08_g10;
  std::vector<std::string> recoInputCollections;
  std::vector<std::string> mcInputCollections;

  TTree *rawTree ;
  TFile *ntupleFile ;

  //histos

  //branches
  TLorentzVector fourvec;
  TLorentzVector tmp0vec;
  TLorentzVector tmp1vec;
  TLorentzVector tmp2vec;
  TLorentzVector tmp3vec;

  double beam_e;
  double beam_m;
  double mc_qq_m;
  double mc_ln_m;

  double miss_pt;
  double miss_theta;
  double miss_phi;
  double miss_e;

  int mc_n;
  std::vector<int> mc_gen_status;
  std::vector<int> mc_type;
  std::vector<double> mc_pt;
  std::vector<double> mc_theta;
  std::vector<double> mc_phi;
  std::vector<double> mc_e;
  std::vector<double> mc_charge;

  int lep_n;
  double lep_etot;
  std::vector<int> lep_type;
  std::vector<double> lep_pt;
  std::vector<double> lep_theta;
  std::vector<double> lep_phi;
  std::vector<double> lep_e;
  std::vector<double> lep_charge;

  int jet_kt_R07_n;
  double jet_kt_R07_etot;
  std::vector<double> jet_kt_R07_pt;
  std::vector<double> jet_kt_R07_theta;
  std::vector<double> jet_kt_R07_phi;
  std::vector<double> jet_kt_R07_e;
  std::vector<double> jet_kt_R07_charge;

  int jet_kt_R10_n;
  double jet_kt_R10_etot;
  std::vector<double> jet_kt_R10_pt;
  std::vector<double> jet_kt_R10_theta;
  std::vector<double> jet_kt_R10_phi;
  std::vector<double> jet_kt_R10_e;
  std::vector<double> jet_kt_R10_charge;

  int jet_kt_R12_n;
  double jet_kt_R12_etot;
  std::vector<double> jet_kt_R12_pt;
  std::vector<double> jet_kt_R12_theta;
  std::vector<double> jet_kt_R12_phi;
  std::vector<double> jet_kt_R12_e;
  std::vector<double> jet_kt_R12_charge;

  int jet_vlc_R06_n;
  double jet_vlc_R06_etot;
  std::vector<double> jet_vlc_R06_pt;
  std::vector<double> jet_vlc_R06_theta;
  std::vector<double> jet_vlc_R06_phi;
  std::vector<double> jet_vlc_R06_e;
  std::vector<double> jet_vlc_R06_charge;

  int jet_vlc_R08_n;
  double jet_vlc_R08_etot;
  std::vector<double> jet_vlc_R08_pt;
  std::vector<double> jet_vlc_R08_theta;
  std::vector<double> jet_vlc_R08_phi;
  std::vector<double> jet_vlc_R08_e;
  std::vector<double> jet_vlc_R08_charge;

  int jet_vlc_R10_n;
  double jet_vlc_R10_etot;
  std::vector<double> jet_vlc_R10_pt;
  std::vector<double> jet_vlc_R10_theta;
  std::vector<double> jet_vlc_R10_phi;
  std::vector<double> jet_vlc_R10_e;
  std::vector<double> jet_vlc_R10_charge;

  int jet_vlc_R08_g05_n;
  double jet_vlc_R08_g05_etot;
  std::vector<double> jet_vlc_R08_g05_pt;
  std::vector<double> jet_vlc_R08_g05_theta;
  std::vector<double> jet_vlc_R08_g05_phi;
  std::vector<double> jet_vlc_R08_g05_e;
  std::vector<double> jet_vlc_R08_g05_charge;

  int jet_vlc_R08_g10_n;
  double jet_vlc_R08_g10_etot;
  std::vector<double> jet_vlc_R08_g10_pt;
  std::vector<double> jet_vlc_R08_g10_theta;
  std::vector<double> jet_vlc_R08_g10_phi;
  std::vector<double> jet_vlc_R08_g10_e;
  std::vector<double> jet_vlc_R08_g10_charge;

  int _nRun ;
  int _nEvt ;
} ;

#endif
