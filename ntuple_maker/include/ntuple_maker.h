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

  // Run at the end of each event
  virtual void check( LCEvent * evt ) ;

  // Called at the very end for cleanup, histogram saving, etc.
  virtual void end() ;

  void clearEventVariables() ;
  void getCollection(LCCollection*&, std::string, LCEvent*);


 protected:

  //parameters
  std::string m_outfileName;

  //collections
  std::string m_IsolatedLepton_collection ;
  std::string m_durham_4jets_collection ;
  std::string m_kt_4jets_radius05_collection ;
  std::string m_kt_4jets_radius07_collection ;
  std::string m_kt_4jets_radius10_collection ;
  std::string m_kt_4jets_radius12_collection ;
  std::string m_kt_4jets_radius15_collection ;
  std::vector<std::string> inputCollections;

  TTree *rawTree ;
  TFile *ntupleFile ;

  //histos

  //branches
  TLorentzVector jet_fourvec;
  int jet_n;
  double jet_etot;
  std::vector<double> jet_pt;
  std::vector<double> jet_theta;
  std::vector<double> jet_phi;
  std::vector<double> jet_e;

  int _nRun ;
  int _nEvt ;
} ;

#endif
