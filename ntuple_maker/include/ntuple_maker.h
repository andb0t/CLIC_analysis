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
  std::string m_IsolatedLepton_collection ;
  std::string m_kt_4jets_radius07_collection ;
  std::string m_kt_4jets_radius10_collection ;
  std::string m_kt_4jets_radius12_collection ;
  std::string m_vlc_4jets_radius06_collection ;
  std::string m_vlc_4jets_radius08_collection ;
  std::string m_vlc_4jets_radius10_collection ;
  std::string m_vlc_4jets_radius08_gamma10_collection ;
  std::string m_vlc_4jets_radius08_gamma05_collection ;
  std::vector<std::string> inputCollections;

  TTree *rawTree ;
  TFile *ntupleFile ;

  //histos

  //branches
  TLorentzVector fourvec;

  int jet_KT_R07_n;
  double jet_KT_R07_etot;
  std::vector<double> jet_KT_R07_pt;
  std::vector<double> jet_KT_R07_theta;
  std::vector<double> jet_KT_R07_phi;
  std::vector<double> jet_KT_R07_e;

  int jet_KT_R10_n;
  double jet_KT_R10_etot;
  std::vector<double> jet_KT_R10_pt;
  std::vector<double> jet_KT_R10_theta;
  std::vector<double> jet_KT_R10_phi;
  std::vector<double> jet_KT_R10_e;

  int jet_KT_R12_n;
  double jet_KT_R12_etot;
  std::vector<double> jet_KT_R12_pt;
  std::vector<double> jet_KT_R12_theta;
  std::vector<double> jet_KT_R12_phi;
  std::vector<double> jet_KT_R12_e;

  int lep_n;
  double lep_etot;
  std::vector<int> lep_type;
  std::vector<double> lep_pt;
  std::vector<double> lep_theta;
  std::vector<double> lep_phi;
  std::vector<double> lep_e;

  int _nRun ;
  int _nEvt ;
} ;

#endif
