#ifndef NTUPLE_MAKER_H
#define NTUPLE_MAKER_H

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include <vector>
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

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
 * @version $Id: ntuple_maker.h,v 1.4 2005-10-11 12:57:39 gaede Exp $
 */

class ntuple_maker : public Processor {

 public:

  virtual Processor*  newProcessor() { return new ntuple_maker ; }


  ntuple_maker() ;

  // Initialisation - run at the beginning to start histograms, etc.
  virtual void init() ;

  // Called at the beginning of every run
  virtual void processRunHeader( LCRunHeader* run ) ;

  // Run over each event - the main algorithm
  virtual void processEvent( LCEvent * evt ) ;

  // Run at the end of each event
  virtual void check( LCEvent * evt ) ;

  // Called at the very end for cleanup, histogram saving, etc.
  virtual void end() ;

  void clearEventVariables() ;
  void getCollection(LCCollection*&, std::string, LCEvent*);


 protected:

  //parameters
  std::string m_treeName;
  std::string m_outfileName;

  //collections
  std::string m_inputParticleCollection ;
  std::string m_inputTrackCollection ;
  std::string m_inputTrackRelationCollection ;
  std::string m_outputPandoraPFOCollection ;
  std::string m_inputPFORelationCollection ;
  std::string m_outputJetkt7 ;

  TTree *tracktree ;
  TFile *trackfile ;

  //histos
  TH1F* h_res_mc_pfo_etot;
  TH1F* h_res_mc_jet_etot;
  TH1F* h_dR_jet_pfo;

  //branches
  TLorentzVector mc_particle;
  int mc_n;
  double mc_etot;
  std::vector<int> mc_pdg;
  std::vector<double> mc_pt;
  std::vector<double> mc_theta;
  std::vector<double> mc_phi;
  std::vector<double> mc_e;

  TLorentzVector trk_particle;
  int trk_n;
  double trk_etot;
  std::vector<int> trk_pdg;
  std::vector<double> trk_pt;
  std::vector<double> trk_theta;
  std::vector<double> trk_phi;
  std::vector<double> trk_e;
  std::vector<int> trk_hit_n;
  std::vector<double> trk_chi2;
  std::vector<double> trk_chi2ndf;
  std::vector<int> trk_ndf;

  TLorentzVector pfo_fourvec;
  int pfo_n;
  double pfo_etot;
  std::vector<int> pfo_pdg;
  std::vector<double> pfo_pt;
  std::vector<double> pfo_theta;
  std::vector<double> pfo_phi;
  std::vector<double> pfo_e;
  std::vector<int> pfo_trk_n;
  std::vector<int> pfo_trk_relation;
  std::vector<int> pfo_trk_pdg;
  std::vector<double> pfo_trk_phi;
  std::vector<double> pfo_trk_omega;
  std::vector<double> pfo_trk_dedx;

  TLorentzVector jet_fourvec;
  int jet_n;
  double jet_etot;
  std::vector<double> jet_pt;
  std::vector<double> jet_theta;
  std::vector<double> jet_phi;
  std::vector<double> jet_e;
  std::vector<std::vector<double>> jet_pfo_relation;

  int _nRun ;
  int _nEvt ;
} ;

#endif
