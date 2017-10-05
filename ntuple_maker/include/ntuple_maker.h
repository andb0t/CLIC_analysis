#ifndef NTUPLE_MAKER_H
#define NTUPLE_MAKER_H
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

  // Called at the very end for cleanup, histogram saving, etc.
  virtual void end() ;

  void clearEventVariables() ;
  void fillRecoParticleEventVariables(std::string collName, LCEvent * evt) ;
  void fillVectors(std::string collName, ReconstructedParticle* particle);
  void getCollection(LCCollection*&, std::string, LCEvent*);


 protected:

  //parameters
  std::string m_outfileName;

  //collections
  std::string m_IsolatedLepton;
  std::string m_kt_4j_R07;
  std::string m_kt_4j_R10;
  std::string m_kt_4j_R12;
  std::string m_vlc_4j_R06;
  std::string m_vlc_4j_R08;
  std::string m_vlc_4j_R10;
  std::string m_vlc_4j_R08_g10;
  std::string m_vlc_4j_R08_g05;
  std::vector<std::string> inputCollections;

  TTree *rawTree ;
  TFile *ntupleFile ;

  //histos

  //branches
  TLorentzVector fourvec;

  int lep_n;
  double lep_etot;
  std::vector<int> lep_type;
  std::vector<double> lep_pt;
  std::vector<double> lep_theta;
  std::vector<double> lep_phi;
  std::vector<double> lep_e;


  int jet_kt_4j_R07_n;
  double jet_kt_4j_R07_etot;
  std::vector<double> jet_kt_4j_R07_pt;
  std::vector<double> jet_kt_4j_R07_theta;
  std::vector<double> jet_kt_4j_R07_phi;
  std::vector<double> jet_kt_4j_R07_e;

  int jet_kt_4j_R10_n;
  double jet_kt_4j_R10_etot;
  std::vector<double> jet_kt_4j_R10_pt;
  std::vector<double> jet_kt_4j_R10_theta;
  std::vector<double> jet_kt_4j_R10_phi;
  std::vector<double> jet_kt_4j_R10_e;

  int jet_kt_4j_R12_n;
  double jet_kt_4j_R12_etot;
  std::vector<double> jet_kt_4j_R12_pt;
  std::vector<double> jet_kt_4j_R12_theta;
  std::vector<double> jet_kt_4j_R12_phi;
  std::vector<double> jet_kt_4j_R12_e;


  int jet_vlc_4j_R06_n;
  double jet_vlc_4j_R06_etot;
  std::vector<double> jet_vlc_4j_R06_pt;
  std::vector<double> jet_vlc_4j_R06_theta;
  std::vector<double> jet_vlc_4j_R06_phi;
  std::vector<double> jet_vlc_4j_R06_e;

  int jet_vlc_4j_R08_n;
  double jet_vlc_4j_R08_etot;
  std::vector<double> jet_vlc_4j_R08_pt;
  std::vector<double> jet_vlc_4j_R08_theta;
  std::vector<double> jet_vlc_4j_R08_phi;
  std::vector<double> jet_vlc_4j_R08_e;

  int jet_vlc_4j_R10_n;
  double jet_vlc_4j_R10_etot;
  std::vector<double> jet_vlc_4j_R10_pt;
  std::vector<double> jet_vlc_4j_R10_theta;
  std::vector<double> jet_vlc_4j_R10_phi;
  std::vector<double> jet_vlc_4j_R10_e;

  int jet_vlc_4j_R08_g10_n;
  double jet_vlc_4j_R08_g10_etot;
  std::vector<double> jet_vlc_4j_R08_g10_pt;
  std::vector<double> jet_vlc_4j_R08_g10_theta;
  std::vector<double> jet_vlc_4j_R08_g10_phi;
  std::vector<double> jet_vlc_4j_R08_g10_e;

  int jet_vlc_4j_R08_g05_n;
  double jet_vlc_4j_R08_g05_etot;
  std::vector<double> jet_vlc_4j_R08_g05_pt;
  std::vector<double> jet_vlc_4j_R08_g05_theta;
  std::vector<double> jet_vlc_4j_R08_g05_phi;
  std::vector<double> jet_vlc_4j_R08_g05_e;

  int _nRun ;
  int _nEvt ;
} ;

#endif
