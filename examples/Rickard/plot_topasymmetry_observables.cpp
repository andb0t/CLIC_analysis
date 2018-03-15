#ifndef __CINT__

#include "TMath.h"

#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"

#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TVector.h"
#include "TColor.h"
#include "TPaletteAxis.h"
#include "THStack.h"
#include "TProfile.h"
#include "TLorentzVector.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>

#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <boost/format.hpp>

#include "utils_split.h"

//Style stuff
#include "/afs/cern.ch/user/l/lstroem/clicdp/pubcom/Style/CLICdpStyle/rootstyle/CLICdpStyle.C" //CLIC Style
#include "/afs/cern.ch/user/l/lstroem/clicdp/analysis/tools/styles/HistStackStyle.C"
#include "/afs/cern.ch/user/l/lstroem/clicdp/analysis/tools/styles/HistStyle.C"

#endif

template <class T>
void pretty_print(std::vector<T>& v);
void doStuff();

void fillHist(const TString& fileName, const TString& treeName, double eventWeight, std::vector<TH1F*> hists, double boostCutMin, double boostCutMax, const TString& mvaFileName, const TString& mvaTreeName, float mvaCut = 0., bool isSignal = false, bool removeTau = false, bool useBkgBool = false);

bool wayToSortArray(std::vector<std::pair<TH1F*, TString> > lhs, std::vector<std::pair<TH1F*, TString> > rhs);
bool wayToSortReverseArray(std::vector<std::pair<TH1F*, TString> > lhs, std::vector<std::pair<TH1F*, TString> > rhs);
void sortChannels(std::vector<std::vector<std::pair<TH1F*, TString> > > &hChannels);
void sortReverseChannels(std::vector<std::vector<std::pair<TH1F*, TString> > > &hChannels);
bool wayToSortLepPt(std::pair<Double_t, Int_t> lhs, std::pair<Double_t, Int_t> rhs);
void calcAfb(TH1F* hist);

void correctUncertainty(TH1F* hist);

bool wayToSortSemiLepCandidate(std::pair<TLorentzVector, int> lhs, std::pair<TLorentzVector, int> rhs);
float getConeEnergy(float cosConeAngle, TLorentzVector& vTlvPhoton, std::vector<TLorentzVector>& vTlvPFOs);
std::vector<float> getRelConeEnergyVector(float cosConeAngle, std::vector<Double_t>* vPhotonPt, std::vector<Double_t>* vPhotonE, std::vector<Double_t>* vPhotonEta, std::vector<Double_t>* vPhotonPhi, std::vector<Double_t>* vRecoPt, std::vector<Double_t>* vRecoE, std::vector<Double_t>* vRecoEta, std::vector<Double_t>* vRecoPhi);
TLorentzVector getRecoSPrime(TLorentzVector visible, TLorentzVector lepton, std::vector<TLorentzVector> vTlvJet);

int myColor1 = TColor::GetColor("#EFB221");
int myColor2 = TColor::GetColor("#DC5318");
int myColor3 = TColor::GetColor("#0072BF");
int myColor4 = TColor::GetColor("#8C8C8C");
int myColor5 = TColor::GetColor("#FF0000");

double lumiTrue = 750.;
double lumiEff;
double lumiFactor;

//Setting up enum for variable names
enum variableNameIndex {
  TopThetaGenBoostedPart1 = 0,
  TopThetaGenBoostedPart2,
  TopThetaRecoBoostedPassedMVACutPart1,
  TopThetaRecoBoostedPassedMVACutPart2,
  TopThetaRecoBoostedTauPassedMVACutPart1,
  TopThetaRecoBoostedTauPassedMVACutPart2,
  TopThetaRecoBoostedSTPassedMVACutPart1,
  TopThetaRecoBoostedSTPassedMVACutPart2,
  TopThetaRecoBoostedSTTauPassedMVACutPart1,
  TopThetaRecoBoostedSTTauPassedMVACutPart2,
  TopThetaRecoBoostedCompPassedMVACutPart1,
  TopThetaRecoBoostedCompPassedMVACutPart2,
  TopThetaRecoBoostedTauCompPassedMVACutPart1,
  TopThetaRecoBoostedTauCompPassedMVACutPart2,
  TopThetaRecoBoostedSTCompPassedMVACutPart1,
  TopThetaRecoBoostedSTCompPassedMVACutPart2,
  TopThetaRecoBoostedSTTauCompPassedMVACutPart1,
  TopThetaRecoBoostedSTTauCompPassedMVACutPart2,
  TopThetaRecoBoostedPassedMVACutPart1SingleTop,
  TopThetaRecoBoostedPassedMVACutPart2SingleTop,
  TopThetaRecoBoostedTauPassedMVACutPart1SingleTop,
  TopThetaRecoBoostedTauPassedMVACutPart2SingleTop,
  TopThetaRecoBoostedSTPassedMVACutPart1SingleTop,
  TopThetaRecoBoostedSTPassedMVACutPart2SingleTop,
  TopThetaRecoBoostedSTTauPassedMVACutPart1SingleTop,
  TopThetaRecoBoostedSTTauPassedMVACutPart2SingleTop,
  TopThetaRecoBoostedCompPassedMVACutPart1SingleTop,
  TopThetaRecoBoostedCompPassedMVACutPart2SingleTop,
  TopThetaRecoBoostedTauCompPassedMVACutPart1SingleTop,
  TopThetaRecoBoostedTauCompPassedMVACutPart2SingleTop,
  TopThetaRecoBoostedSTCompPassedMVACutPart1SingleTop,
  TopThetaRecoBoostedSTCompPassedMVACutPart2SingleTop,
  TopThetaRecoBoostedSTTauCompPassedMVACutPart1SingleTop,
  TopThetaRecoBoostedSTTauCompPassedMVACutPart2SingleTop,
  varLAST
};

TString variableNames[varLAST] =
  {
  "TopThetaGenBoostedPart1",
  "TopThetaGenBoostedPart2",
  "TopThetaRecoBoostedPassedMVACutPart1",
  "TopThetaRecoBoostedPassedMVACutPart2",
  "TopThetaRecoBoostedTauPassedMVACutPart1",
  "TopThetaRecoBoostedTauPassedMVACutPart2",
  "TopThetaRecoBoostedSTPassedMVACutPart1",
  "TopThetaRecoBoostedSTPassedMVACutPart2",
  "TopThetaRecoBoostedSTTauPassedMVACutPart1",
  "TopThetaRecoBoostedSTTauPassedMVACutPart2",
  "TopThetaRecoBoostedCompPassedMVACutPart1",
  "TopThetaRecoBoostedCompPassedMVACutPart2",
  "TopThetaRecoBoostedTauCompPassedMVACutPart1",
  "TopThetaRecoBoostedTauCompPassedMVACutPart2",
  "TopThetaRecoBoostedSTCompPassedMVACutPart1",
  "TopThetaRecoBoostedSTCompPassedMVACutPart2",
  "TopThetaRecoBoostedSTTauCompPassedMVACutPart1",
  "TopThetaRecoBoostedSTTauCompPassedMVACutPart2",
  "TopThetaRecoBoostedPassedMVACutPart1SingleTop",
  "TopThetaRecoBoostedPassedMVACutPart2SingleTop",
  "TopThetaRecoBoostedTauPassedMVACutPart1SingleTop",
  "TopThetaRecoBoostedTauPassedMVACutPart2SingleTop",
  "TopThetaRecoBoostedSTPassedMVACutPart1SingleTop",
  "TopThetaRecoBoostedSTPassedMVACutPart2SingleTop",
  "TopThetaRecoBoostedSTTauPassedMVACutPart1SingleTop",
  "TopThetaRecoBoostedSTTauPassedMVACutPart2SingleTop",
  "TopThetaRecoBoostedCompPassedMVACutPart1SingleTop",
  "TopThetaRecoBoostedCompPassedMVACutPart2SingleTop",
  "TopThetaRecoBoostedTauCompPassedMVACutPart1SingleTop",
  "TopThetaRecoBoostedTauCompPassedMVACutPart2SingleTop",
  "TopThetaRecoBoostedSTCompPassedMVACutPart1SingleTop",
  "TopThetaRecoBoostedSTCompPassedMVACutPart2SingleTop",
  "TopThetaRecoBoostedSTTauCompPassedMVACutPart1SingleTop",
  "TopThetaRecoBoostedSTTauCompPassedMVACutPart2SingleTop",
  };

//Functions
void doStuff() {

  time_t begin, end;
  time (&begin);

  std::cout << "Setting things up..." << std::endl;
  CLICdpStyle();
  CLICdpLabel();

  //Enum Check for ProdID
  if ( checkEnum() == false ){
    std::cout << "WARNING: Enums do not match file names!" << std::endl;
    return;
  }

  //Setting cross-section weights
  double weights[pidLAST];
  setWeights(weights);

  //DEFINITIONS
  TString dataDirNameLocal = "/afs/cern.ch/user/l/lstroem/Whoopi/clicdp/data/";
  TString mvaFilePath = "/afs/cern.ch/user/l/lstroem/clicdp/analysis/scikit_learn/root/";
  TString fitFilePath = "/afs/cern.ch/user/l/lstroem/topa/fit/";

  //Creating plotting directory
  std::string plotDirName = "/afs/cern.ch/user/l/lstroem/clicdp/analysis/plotting/topasymmetry/plots/";
  gROOT->ProcessLine(Form(".! mkdir -p %s", plotDirName.c_str()));

  //Creating macro directory
  std::string macroDirName = "/afs/cern.ch/user/l/lstroem/clicdp/analysis/plotting/topasymmetry/macro/";
  gROOT->ProcessLine(Form(".! mkdir -p %s", macroDirName.c_str()));

  //Data files
  TString costumizedString = "23Jan2017";
  TString fileNameTemplate = dataDirNameLocal +
    "top/%i/topasymmetry/root/topasymmetry_chain_vlc_%.1f_%.1f_%.1f_excl2_Es_dR%.2f_dP%.2f_cthetaWmax%.2f_%s_merged_%i.root";
  TString fileNameTemplateHiggs = dataDirNameLocal +
    "higgs/%i/topasymmetry/root/topasymmetry_chain_vlc_%.1f_%.1f_%.1f_excl2_Es_dR%.2f_dP%.2f_cthetaWmax%.2f_%s_merged_%i.root";
  TString fileNameTemplateHiggsExtraStats = dataDirNameLocal +
    "higgs/%i/topasymmetry/root/topasymmetry_chain_vlc_%.1f_%.1f_%.1f_excl2_Es_dR%.2f_dP%.2f_cthetaWmax%.2f_%s_merged_%i_extrastats.root";
  TString treeName = "anatree";

  //mva
  TString mvaFileNameTemplate;
  TString mvaTreeName = "BDTOutput";

  //roots fit
  TString fitFileName;

  //SETTINGS
  TString roots = "1.4tev"; //"3tev"
  //double lumiEff = 0.;
  double _defCosThetaCut = 1.;
  double _defJetRadius = 0.;
  double _defJetBeta = 0.;
  double _defJetGamma = 0.;
  double _defDeltaR = 0.;
  double _defDeltaP = 0.;
  double _defCosThetaWMax = 0.;
  double _defBoostCutMin = 0.;
  double _defBoostCutMax = 0.;
  double _defMVACut = 0.;

  if (roots == "1.4tev"){
    std::cout << "Using settings for 1.4 TeV..." << std::endl;
    lumiEff = 750.;

    _defJetRadius = 1.4;
    _defJetBeta = 1.0;
    _defJetGamma = 1.0;

    _defDeltaR = 0.30;
    _defDeltaP = 0.04;
    _defCosThetaWMax = 1.0;

    _defBoostCutMin = 1200.;
    _defBoostCutMax = 1.e4;

    _defMVACut = -0.05;

    fitFileName = fitFilePath + "fitPol1_vlc_1.4_1.0_1.0.root";

  } else if (roots == "3tev"){
    std::cout << "Using settings for 3 TeV..." << std::endl;
    lumiEff = 1500.;

    _defJetRadius = 1.0;
    _defJetBeta = 1.0;
    _defJetGamma = 1.0;

    _defDeltaR = 0.12;
    _defDeltaP = 0.02;
    _defCosThetaWMax = 1.0;

    _defBoostCutMin = 2600.;
    _defBoostCutMax = 1.e4;

    _defMVACut = 0.;

    fitFileName = fitFilePath + "fitPol1_vlc_1.0_1.0_1.0_3TeV.root";

  } else {
    std::cout << "WARNING! Something wrong with the settings (roots not correct)!" << std::endl;
    return;
  }

  if (_defBoostCutMax == 1.e4){
    mvaFileNameTemplate = mvaFilePath + "prediction_%i_vlc_%.1f_%.1f_%.1f_excl2_Es_dR%.2f_dP%.2f_cthetaWmax%.2f_%s_sPrimeAbove%.0f_deactivatedTauInTraining_deactivateComp6f_20180131.root";
    //mvaFileNameTemplate = mvaFilePath + "prediction_%i_vlc_%.1f_%.1f_%.1f_excl2_Es_dR%.2f_dP%.2f_cthetaWmax%.2f_%s_sPrimeAbove%.0f_deactivatedTauInTraining_deactivateComp6f_polpos_20180131.root"; //use for 3 tev polpos sample only
  } else {
    mvaFileNameTemplate = mvaFilePath + "prediction_%i_vlc_%.1f_%.1f_%.1f_excl2_Es_dR%.2f_dP%.2f_cthetaWmax%.2f_%s_sPrimeAbove%.0fBelow%.0f_deactivatedTauInTraining_deactivateComp6f_20180131.root";
  }

  //Setting up plotting filename suffix
  TString suffix = Form("_s1.4TeV_%.0f-%.0fGeV_VLC_%.1f_%.1f_%.1f_deltaR%.2f_deltaP%.2f_cosThetaWMax1.0_cosThetaCut%.2f_%s_%s_estimatorSqrt_wTopWCut_test_20180306", _defBoostCutMin, _defBoostCutMax, _defJetRadius, _defJetBeta, _defJetGamma, _defDeltaR, _defDeltaP, _defCosThetaCut, costumizedString.Data(), "deactivatedTauInTraining_deactivateComp6f");

  //Settings for histograms
  int nBinsTheta = 20; double minTheta = -1.; double maxTheta = 1.;

  double limits[varLAST][3];
  for (int v = 0; v < varLAST; v++){
    if ( (variableNames[v].Contains("Pt")) ){
      limits[v][0] = 100;
      limits[v][1] = 0.;
      limits[v][2] = 1.e10;
    } else {
      limits[v][0] = nBinsTheta;
      limits[v][1] = minTheta;
      limits[v][2] = maxTheta;
    }
  }

  //Filling histograms
  std::vector<std::vector<TH1F*> > vHists;
  for (int i = 0; i < pidLAST; i++){
    std::vector<TH1F*> hVariables;
    for (int j = 0; j < varLAST; j++){
      double nBins = limits[j][0];
      double min = limits[j][1];
      double max = limits[j][2];
      TH1F* hist = new TH1F(variableNames[j]+Form("%i",fileNamePID[i]),"", nBins, min, max);
      hist->Sumw2();
      hVariables.push_back(hist);
    }
    if ( (i==pid6589) || (i==pid6592) || (i==pid6634) || (i==pid6637) || (i==pid6595) || (i==pid6598) || (i==pid6601) || (i==pid6604) || (i==pid6607) || (i==pid6610) || (i==pid6613) || (i==pid6616) || (i==pid6619) || (i==pid6622) || (i==pid8283) || (i==pid8254) || (i==pid7477) || (i==pid8244) || (i==pid8271) || (i==pid6586) || (i==pid6625) || (i==pid6628) || (i==pid6631) ){
      TString fileName, mvaFileName;
      fileName = Form(fileNameTemplate, fileNamePID[i], _defJetRadius, _defJetBeta, _defJetGamma, _defDeltaR, _defDeltaP, _defCosThetaWMax, costumizedString.Data(), fileNamePID[i]);
      if (_defBoostCutMax == 1.e4){
	mvaFileName = Form(mvaFileNameTemplate, fileNamePID[i], _defJetRadius, _defJetBeta, _defJetGamma, _defDeltaR, _defDeltaP, _defCosThetaWMax, costumizedString.Data(), _defBoostCutMin);
      } else {
	mvaFileName = Form(mvaFileNameTemplate, fileNamePID[i], _defJetRadius, _defJetBeta, _defJetGamma, _defDeltaR, _defDeltaP, _defCosThetaWMax, costumizedString.Data(), _defBoostCutMin, _defBoostCutMax);
      }
      if ( (i==pid6589) || (i==pid6592) || (i == pid6634) || (i == pid6637) ){
	fillHist(fileName, treeName, weights[i], hVariables, _defBoostCutMin, _defBoostCutMax, mvaFileName, mvaTreeName, _defMVACut, true);
      } else {
	fillHist(fileName, treeName, weights[i], hVariables, _defBoostCutMin, _defBoostCutMax, mvaFileName, mvaTreeName, _defMVACut);
      }
    }
    for (int j = 0; j < varLAST; j++){ correctUncertainty(hVariables.at(j)); }
    vHists.push_back(hVariables);
  }

  ///////////////////////////////////////////
  ///////////////////////////////////////////
  ///////////////////////////////////////////
  ///////////////////////////////////////////
  ///////////////////////////////////////////

  //Defining histograms
  TH1F* histTopThetaGenPart1 = (TH1F*) vHists.at(pid6589).at(TopThetaGenBoostedPart1)->Clone();
  histTopThetaGenPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaGenBoostedPart1)->Clone());
  histTopThetaGenPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaGenBoostedPart1)->Clone());
  histTopThetaGenPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaGenBoostedPart1)->Clone());

  TH1F* histTopThetaRecoPassedMVACutPart1 = (TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  histTopThetaRecoPassedMVACutPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());

  //Define background histograms
  TH1F* histTopThetaRecoNonFidPassedMVACutPart1 = (TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone();
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedSTCompPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedSTCompPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedSTCompPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedSTCompPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedCompPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedCompPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedCompPassedMVACutPart1)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedCompPassedMVACutPart1)->Clone());

  TH1F* histTopThetaRecoTauPassedMVACutPart1 = (TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedTauPassedMVACutPart1)->Clone();
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedTauPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedTauPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedTauPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedTauCompPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedTauCompPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedTauCompPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedTauCompPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedSTTauCompPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedSTTauCompPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedSTTauCompPassedMVACutPart1)->Clone());
  histTopThetaRecoTauPassedMVACutPart1->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedSTTauCompPassedMVACutPart1)->Clone());

  TH1F* histTopThetaRecoPassedMVACutHadPart1 = (TH1F*) vHists.at(pid6595).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6598).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6601).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6604).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6607).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6610).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6613).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6616).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6619).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6622).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6595).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6598).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6601).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6604).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6607).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6610).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6613).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6616).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6619).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutHadPart1->Add((TH1F*) vHists.at(pid6622).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());

  TH1F* histTopThetaRecoPassedMVACutLepPart1 = (TH1F*) vHists.at(pid6586).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6625).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6628).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6631).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6586).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6625).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6628).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6631).at(TopThetaRecoBoostedSTPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6586).at(TopThetaRecoBoostedTauPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6625).at(TopThetaRecoBoostedTauPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6628).at(TopThetaRecoBoostedTauPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6631).at(TopThetaRecoBoostedTauPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6586).at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6625).at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6628).at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Clone());
  histTopThetaRecoPassedMVACutLepPart1->Add((TH1F*) vHists.at(pid6631).at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Clone());

  TH1F* histTopThetaRecoPassedMVACutBkgqqPart1 = (TH1F*) vHists.at(pid8283).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  TH1F* histTopThetaRecoPassedMVACutBkgqqqqPart1 = (TH1F*) vHists.at(pid8254).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  TH1F* histTopThetaRecoPassedMVACutBkgqqlvPart1 = (TH1F*) vHists.at(pid7477).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  TH1F* histTopThetaRecoPassedMVACutBkgqqllPart1 = (TH1F*) vHists.at(pid8244).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  TH1F* histTopThetaRecoPassedMVACutBkgqqvvPart1 = (TH1F*) vHists.at(pid8271).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  //TH1F* histTopThetaRecoPassedMVACutBkgqqqqllPart1 = (TH1F*) vHists.at(pid5572).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  //TH1F* histTopThetaRecoPassedMVACutBkgqqqqvvPart1 = (TH1F*) vHists.at(pid5527).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();
  //TH1F* histTopThetaRecoPassedMVACutBkgqqqqlvPart1 = (TH1F*) vHists.at(pid5594).at(TopThetaRecoBoostedPassedMVACutPart1)->Clone();

  //Defining signal, background, tot, etc.
  TH1F* histTopThetaRecoPassedMVACutSignalPart1 = (TH1F*) histTopThetaRecoPassedMVACutPart1->Clone();

  TH1F* histTopThetaRecoPassedMVACutBkgPart1 = (TH1F*) histTopThetaRecoNonFidPassedMVACutPart1->Clone();
  histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoTauPassedMVACutPart1->Clone());
  histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutHadPart1->Clone());
  histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutLepPart1->Clone());
  histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqPart1->Clone());
  histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqPart1->Clone());
  histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqlvPart1->Clone());
  histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqllPart1->Clone());
  histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqvvPart1->Clone());
  //histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqllPart1->Clone());
  //histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqvvPart1->Clone());
  //histTopThetaRecoPassedMVACutBkgPart1->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqlvPart1->Clone());

  for (int bin=0; bin<=histTopThetaRecoPassedMVACutSignalPart1->GetNbinsX()+1; bin++){ histTopThetaRecoPassedMVACutSignalPart1->SetBinError(bin, TMath::Sqrt( histTopThetaRecoPassedMVACutSignalPart1->GetBinContent(bin) + histTopThetaRecoPassedMVACutBkgPart1->GetBinContent(bin) )); }

  //Efficiency definition
  TH1F* histScalePart1 = (TH1F*) histTopThetaGenPart1->Clone();
  histScalePart1->Divide(histTopThetaRecoPassedMVACutSignalPart1);
  for (int bin=0; bin<=histScalePart1->GetNbinsX()+1; bin++){ histScalePart1->SetBinError(bin, 0.); }

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  //Defining histograms
  TH1F* histTopThetaGenPart2 = (TH1F*) vHists.at(pid6589).at(TopThetaGenBoostedPart2)->Clone();
  histTopThetaGenPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaGenBoostedPart2)->Clone());
  histTopThetaGenPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaGenBoostedPart2)->Clone());
  histTopThetaGenPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaGenBoostedPart2)->Clone());

  TH1F* histTopThetaRecoPassedMVACutPart2 = (TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  histTopThetaRecoPassedMVACutPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());

  //Define background histograms
  TH1F* histTopThetaRecoNonFidPassedMVACutPart2 = (TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone();
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedSTCompPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedSTCompPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedSTCompPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedSTCompPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedCompPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedCompPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedCompPassedMVACutPart2)->Clone());
  histTopThetaRecoNonFidPassedMVACutPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedCompPassedMVACutPart2)->Clone());

  TH1F* histTopThetaRecoTauPassedMVACutPart2 = (TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedTauPassedMVACutPart2)->Clone();
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedTauPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedTauPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedTauPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedTauCompPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedTauCompPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedTauCompPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedTauCompPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6589).at(TopThetaRecoBoostedSTTauCompPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6592).at(TopThetaRecoBoostedSTTauCompPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6634).at(TopThetaRecoBoostedSTTauCompPassedMVACutPart2)->Clone());
  histTopThetaRecoTauPassedMVACutPart2->Add((TH1F*) vHists.at(pid6637).at(TopThetaRecoBoostedSTTauCompPassedMVACutPart2)->Clone());

  TH1F* histTopThetaRecoPassedMVACutHadPart2 = (TH1F*) vHists.at(pid6595).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6598).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6601).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6604).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6607).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6610).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6613).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6616).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6619).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6622).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6595).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6598).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6601).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6604).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6607).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6610).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6613).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6616).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6619).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutHadPart2->Add((TH1F*) vHists.at(pid6622).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());

  TH1F* histTopThetaRecoPassedMVACutLepPart2 = (TH1F*) vHists.at(pid6586).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6625).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6628).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6631).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6586).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6625).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6628).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6631).at(TopThetaRecoBoostedSTPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6586).at(TopThetaRecoBoostedTauPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6625).at(TopThetaRecoBoostedTauPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6628).at(TopThetaRecoBoostedTauPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6631).at(TopThetaRecoBoostedTauPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6586).at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6625).at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6628).at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Clone());
  histTopThetaRecoPassedMVACutLepPart2->Add((TH1F*) vHists.at(pid6631).at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Clone());

  TH1F* histTopThetaRecoPassedMVACutBkgqqPart2 = (TH1F*) vHists.at(pid8283).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  TH1F* histTopThetaRecoPassedMVACutBkgqqqqPart2 = (TH1F*) vHists.at(pid8254).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  TH1F* histTopThetaRecoPassedMVACutBkgqqlvPart2 = (TH1F*) vHists.at(pid7477).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  TH1F* histTopThetaRecoPassedMVACutBkgqqllPart2 = (TH1F*) vHists.at(pid8244).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  TH1F* histTopThetaRecoPassedMVACutBkgqqvvPart2 = (TH1F*) vHists.at(pid8271).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  //TH1F* histTopThetaRecoPassedMVACutBkgqqqqllPart2 = (TH1F*) vHists.at(pid5572).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  //TH1F* histTopThetaRecoPassedMVACutBkgqqqqvvPart2 = (TH1F*) vHists.at(pid5527).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();
  //TH1F* histTopThetaRecoPassedMVACutBkgqqqqlvPart2 = (TH1F*) vHists.at(pid5594).at(TopThetaRecoBoostedPassedMVACutPart2)->Clone();

  //Defining signal, background, tot, etc.
  TH1F* histTopThetaRecoPassedMVACutSignalPart2 = (TH1F*) histTopThetaRecoPassedMVACutPart2->Clone();

  TH1F* histTopThetaRecoPassedMVACutBkgPart2 = (TH1F*) histTopThetaRecoNonFidPassedMVACutPart2->Clone();
  histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoTauPassedMVACutPart2->Clone());
  histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutHadPart2->Clone());
  histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutLepPart2->Clone());
  histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqPart2->Clone());
  histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqPart2->Clone());
  histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqlvPart2->Clone());
  histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqllPart2->Clone());
  histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqvvPart2->Clone());
  //histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqllPart2->Clone());
  //histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqvvPart2->Clone());
  //histTopThetaRecoPassedMVACutBkgPart2->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqlvPart2->Clone());

  for (int bin=0; bin<=histTopThetaRecoPassedMVACutSignalPart2->GetNbinsX()+1; bin++){ histTopThetaRecoPassedMVACutSignalPart2->SetBinError(bin, TMath::Sqrt( histTopThetaRecoPassedMVACutSignalPart2->GetBinContent(bin) + histTopThetaRecoPassedMVACutBkgPart2->GetBinContent(bin) )); }

  //Efficiency definition
  TH1F* histScalePart2 = (TH1F*) histTopThetaGenPart2->Clone();
  histScalePart2->Divide(histTopThetaRecoPassedMVACutSignalPart2);
  for (int bin=0; bin<=histScalePart2->GetNbinsX()+1; bin++){ histScalePart2->SetBinError(bin, 0.); }

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  lumiFactor = TMath::Sqrt(lumiEff/lumiTrue);

  //Afb from generator level
  TH1F* histTopThetaGen = (TH1F*) histTopThetaGenPart1->Clone();
  histTopThetaGen->Add((TH1F*) histTopThetaGenPart2->Clone());
  std::cout << "Simple calculation for histTopThetaGen: " << std::endl;
  calcAfb(histTopThetaGen);

  TH1F* histTopThetaRecoPassedMVACutSignal = (TH1F*) histTopThetaRecoPassedMVACutSignalPart1->Clone();
  histTopThetaRecoPassedMVACutSignal->Add((TH1F*) histTopThetaRecoPassedMVACutSignalPart2->Clone());
  std::cout << "Total number of signal events at final level: " << histTopThetaRecoPassedMVACutSignal->GetSumOfWeights() << std::endl;
  TH1F* histTopThetaRecoPassedMVACutBkg = (TH1F*) histTopThetaRecoPassedMVACutBkgPart1->Clone();
  histTopThetaRecoPassedMVACutBkg->Add((TH1F*) histTopThetaRecoPassedMVACutBkgPart2->Clone());
  std::cout << "Total number of background events at final level: " << histTopThetaRecoPassedMVACutBkg->GetSumOfWeights() << std::endl;
  TH1F* histTopThetaRecoPassedMVACutTot = (TH1F*) histTopThetaRecoPassedMVACutSignal->Clone();
  histTopThetaRecoPassedMVACutTot->Add((TH1F*) histTopThetaRecoPassedMVACutBkg->Clone());
  double csCalc = lumiFactor * TMath::Sqrt(histTopThetaRecoPassedMVACutSignal->GetSumOfWeights()+histTopThetaRecoPassedMVACutBkg->GetSumOfWeights())/histTopThetaRecoPassedMVACutSignal->GetSumOfWeights();
  std::cout << "Cross section rel. uncertainty (sqrt(S+B)/S): " << csCalc << std::endl;

  //Check cross-sections
  TH1F* histTopThetaRecoNonFidPassedMVACut = (TH1F*) histTopThetaRecoNonFidPassedMVACutPart1->Clone();
  histTopThetaRecoNonFidPassedMVACut->Add((TH1F*) histTopThetaRecoNonFidPassedMVACutPart2->Clone());
  std::cout << "histTopThetaRecoNonFidPassedMVACut: " << histTopThetaRecoNonFidPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  TH1F* histTopThetaRecoTauPassedMVACut = (TH1F*) histTopThetaRecoTauPassedMVACutPart1->Clone();
  histTopThetaRecoTauPassedMVACut->Add((TH1F*) histTopThetaRecoTauPassedMVACutPart2->Clone());
  std::cout << "histTopThetaRecoTauPassedMVACut: " << histTopThetaRecoTauPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  TH1F* histTopThetaRecoHadPassedMVACut = (TH1F*) histTopThetaRecoPassedMVACutHadPart1->Clone();
  histTopThetaRecoHadPassedMVACut->Add((TH1F*) histTopThetaRecoPassedMVACutHadPart2->Clone());
  std::cout << "histTopThetaRecoHadPassedMVACut: " << histTopThetaRecoHadPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  TH1F* histTopThetaRecoLepPassedMVACut = (TH1F*) histTopThetaRecoPassedMVACutLepPart1->Clone();
  histTopThetaRecoLepPassedMVACut->Add((TH1F*) histTopThetaRecoPassedMVACutLepPart2->Clone());
  std::cout << "histTopThetaRecoLepPassedMVACut: " << histTopThetaRecoLepPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  TH1F* histTopThetaRecoBkgqqPassedMVACut = (TH1F*) histTopThetaRecoPassedMVACutBkgqqPart1->Clone();
  histTopThetaRecoBkgqqPassedMVACut->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqPart2->Clone());
  std::cout << "histTopThetaRecoBkgqqPassedMVACut: " << histTopThetaRecoBkgqqPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  TH1F* histTopThetaRecoBkgqqqqPassedMVACut = (TH1F*) histTopThetaRecoPassedMVACutBkgqqqqPart1->Clone();
  histTopThetaRecoBkgqqqqPassedMVACut->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqqqPart2->Clone());
  std::cout << "histTopThetaRecoBkgqqqqPassedMVACut: " << histTopThetaRecoBkgqqqqPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  TH1F* histTopThetaRecoBkgqqlvPassedMVACut = (TH1F*) histTopThetaRecoPassedMVACutBkgqqlvPart1->Clone();
  histTopThetaRecoBkgqqlvPassedMVACut->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqlvPart2->Clone());
  std::cout << "histTopThetaRecoBkgqqlvPassedMVACut: " << histTopThetaRecoBkgqqlvPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  TH1F* histTopThetaRecoBkgqqllPassedMVACut = (TH1F*) histTopThetaRecoPassedMVACutBkgqqllPart1->Clone();
  histTopThetaRecoBkgqqllPassedMVACut->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqllPart2->Clone());
  std::cout << "histTopThetaRecoBkgqqllPassedMVACut: " << histTopThetaRecoBkgqqllPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  TH1F* histTopThetaRecoBkgqqvvPassedMVACut = (TH1F*) histTopThetaRecoPassedMVACutBkgqqvvPart1->Clone();
  histTopThetaRecoBkgqqvvPassedMVACut->Add((TH1F*) histTopThetaRecoPassedMVACutBkgqqvvPart2->Clone());
  std::cout << "histTopThetaRecoBkgqqvvPassedMVACut: " << histTopThetaRecoBkgqqvvPassedMVACut->GetSumOfWeights()/lumiEff << std::endl;

  //Merging and scaling (part 1 and part 2) - 2 fold
  TH1F* histTopThetaRecoPassedMVACutScaledPart1 = (TH1F*) histTopThetaRecoPassedMVACutSignalPart1->Clone();
  histTopThetaRecoPassedMVACutScaledPart1->Multiply(histScalePart2);
  TH1F* histTopThetaRecoPassedMVACutScaledPart2 = (TH1F*) histTopThetaRecoPassedMVACutSignalPart2->Clone();
  histTopThetaRecoPassedMVACutScaledPart2->Multiply(histScalePart1);
  TH1F* histTopThetaRecoPassedMVACutScaled = (TH1F*) histTopThetaRecoPassedMVACutScaledPart1->Clone();
  histTopThetaRecoPassedMVACutScaled->Add((TH1F*) histTopThetaRecoPassedMVACutScaledPart2->Clone());
  std::cout << "Simple calculation for histTopThetaRecoPassedMVACutScaled: " << std::endl;
  calcAfb(histTopThetaRecoPassedMVACutScaled);

  //Reconstruction level
  TF1* afbFitNew = new TF1("afbFitNew","[0]*(1+x)*(1+x) + [1]*(1-x)*(1-x) + [2]*(1-x*x)", -1,1);
  afbFitNew->SetParName(0,"A1");
  afbFitNew->SetParName(1,"A2");
  afbFitNew->SetParName(2,"A3");
  TFitResultPtr afbFitNewResult = histTopThetaRecoPassedMVACutScaled->Fit(afbFitNew,"RS");
  double sigmaA = afbFitNew->GetParameter(0); double uSigmaA = afbFitNew->GetParError(0);
  double sigmaB = afbFitNew->GetParameter(1); double uSigmaB = afbFitNew->GetParError(1);
  double sigmaC = afbFitNew->GetParameter(2); double uSigmaC = afbFitNew->GetParError(2);
  TMatrixDSym cov = afbFitNewResult->GetCovarianceMatrix();
  cov.Print();
  //CS - derive value and error
  double cs = (8/3.)*sigmaA + (8/3.)*sigmaB + (4/3.)*sigmaC; //2.
  double derAcs = (8/3.);
  double derBcs = (8/3.);
  double derCcs = (4/3.); //2.
  TMatrix mCov(cov);
  TMatrix mDercs(1,3); mDercs(0,0) = derAcs; mDercs(0,1) = derBcs; mDercs(0,2) = derCcs;
  TMatrix mDercsT(3,1); mDercsT(0,0) = derAcs; mDercsT(1,0) = derBcs; mDercsT(2,0) = derCcs;
  TMatrix mMultiPartcs(1,3);
  mMultiPartcs = mDercs;
  mMultiPartcs *= mCov;
  double csError = lumiFactor * TMath::Sqrt(mMultiPartcs(0,0)*mDercsT(0,0) + mMultiPartcs(0,1)*mDercsT(1,0) + mMultiPartcs(0,2)*mDercsT(2,0));
  double csErrorOld = lumiFactor * TMath::Sqrt( TMath::Power(derAcs, 2.)*TMath::Power(uSigmaA, 2.) + TMath::Power(derBcs, 2.)*TMath::Power(uSigmaB, 2.) + TMath::Power(derCcs, 2.)*TMath::Power(uSigmaC, 2.) );
  std::cout << "FIT RESULTS!" << std::endl;
  std::cout << "cs = " << cs/lumiEff/0.1 << std::endl;
  std::cout << "cs uncertainty = " << csError/lumiEff/0.1 << " (old: " << csErrorOld/lumiEff/0.1 << ")" << std::endl;
  std::cout << "cs rel. uncertainty = " << csError/cs << std::endl;
  //AFB - derive value and error
  double afb = 2.*(sigmaA - sigmaB)/cs;
  double derA = (3*(4*sigmaB+sigmaC))/(2.*TMath::Power((2*sigmaA+2*sigmaB+sigmaC), 2.));
  double derB = -1.*(3*(4*sigmaA+sigmaC))/(2.*TMath::Power((2*sigmaA+2*sigmaB+sigmaC), 2.));
  double derC = -1.*(3*(sigmaA-sigmaB))/(2.*TMath::Power((2*sigmaA+2*sigmaB+sigmaC), 2.));
  TMatrix mDer(1,3); mDer(0,0) = derA; mDer(0,1) = derB; mDer(0,2) = derC;
  TMatrix mDerT(3,1); mDerT(0,0) = derA; mDerT(1,0) = derB; mDerT(2,0) = derC;
  TMatrix mMultiPart(1,3);
  mMultiPart = mDer;
  mMultiPart *= mCov;
  double afbError = lumiFactor * TMath::Sqrt(mMultiPart(0,0)*mDerT(0,0) + mMultiPart(0,1)*mDerT(1,0) + mMultiPart(0,2)*mDerT(2,0));
  double afbErrorOld = lumiFactor * TMath::Sqrt( TMath::Power(derA, 2.)*TMath::Power(uSigmaA, 2.) + TMath::Power(derB, 2.)*TMath::Power(uSigmaB, 2.) + TMath::Power(derC, 2.)*TMath::Power(uSigmaC, 2.) );
  std::cout << "afb = " << afb << std::endl;
  std::cout << "afb uncertainty = " << afbError << " (old: " << afbErrorOld << ")" << std::endl;
  std::cout << "afb rel. uncertainty = " << afbError/afb << std::endl;

  //Generator level
  TF1* afbFitNewGen = new TF1("afbFitNewGen","[0]*(1+x)*(1+x) + [1]*(1-x)*(1-x) + [2]*(1-x*x)", -1,1);
  afbFitNewGen->SetParName(0,"A1");
  afbFitNewGen->SetParName(1,"A2");
  afbFitNewGen->SetParName(2,"A3");
  TFitResultPtr afbFitNewResultGen = histTopThetaGen->Fit(afbFitNewGen,"RS");
  double sigmaAGen = afbFitNewGen->GetParameter(0); double uSigmaAGen = afbFitNewGen->GetParError(0);
  double sigmaBGen = afbFitNewGen->GetParameter(1); double uSigmaBGen = afbFitNewGen->GetParError(1);
  double sigmaCGen = afbFitNewGen->GetParameter(2); double uSigmaCGen = afbFitNewGen->GetParError(2);
  TMatrixDSym covGen = afbFitNewResultGen->GetCovarianceMatrix();
  covGen.Print();
  //CS - derive value and error
  double csGen = (8/3.)*sigmaAGen + (8/3.)*sigmaBGen + (4/3.)*sigmaCGen; //2.
  double derCsAGen = (8/3.);
  double derCsBGen = (8/3.);
  double derCsCGen = (4/3.); //2.
  TMatrix mCovGen(covGen);
  TMatrix mDerCsGen(1,3); mDerCsGen(0,0) = derCsAGen; mDerCsGen(0,1) = derCsBGen; mDerCsGen(0,2) = derCsCGen;
  TMatrix mDerCsGenT(3,1); mDerCsGenT(0,0) = derCsAGen; mDerCsGenT(1,0) = derCsBGen; mDerCsGenT(2,0) = derCsCGen;
  TMatrix mMultiPartCsGen(1,3);
  mMultiPartCsGen = mDerCsGen;
  mMultiPartCsGen *= mCovGen;
  double csErrorGen = lumiFactor * TMath::Sqrt(mMultiPartCsGen(0,0)*mDerCsGenT(0,0) + mMultiPartCsGen(0,1)*mDerCsGenT(1,0) + mMultiPartCsGen(0,2)*mDerCsGenT(2,0));
  double csErrorGenOld = lumiFactor * TMath::Sqrt( TMath::Power(derCsAGen, 2.)*TMath::Power(uSigmaAGen, 2.) + TMath::Power(derCsBGen, 2.)*TMath::Power(uSigmaBGen, 2.) + TMath::Power(derCsCGen, 2.)*TMath::Power(uSigmaCGen, 2.) );
  std::cout << "FIT RESULTS - GENERATOR!" << std::endl;
  std::cout << "cs = " << csGen/lumiEff/0.1 << std::endl;
  std::cout << "cs uncertainty = " << csErrorGen/lumiEff/0.1 << " (old: " << csErrorGenOld/lumiEff/0.1 << ")" << std::endl;
  std::cout << "cs rel. uncertainty = " << csErrorGen/csGen << std::endl;
  //AFB - derive value and error
  double afbGen = 2.*(sigmaAGen - sigmaBGen)/csGen;
  double derAGen = (3*(4*sigmaBGen+sigmaCGen))/(2.*TMath::Power((2*sigmaAGen+2*sigmaBGen+sigmaCGen), 2.));
  double derBGen = -1.*(3*(4*sigmaAGen+sigmaCGen))/(2.*TMath::Power((2*sigmaAGen+2*sigmaBGen+sigmaCGen), 2.));
  double derCGen = -1.*(3*(sigmaAGen-sigmaBGen))/(2.*TMath::Power((2*sigmaAGen+2*sigmaBGen+sigmaCGen), 2.));
  TMatrix mDerGen(1,3); mDerGen(0,0) = derAGen; mDerGen(0,1) = derBGen; mDerGen(0,2) = derCGen;
  TMatrix mDerGenT(3,1); mDerGenT(0,0) = derAGen; mDerGenT(1,0) = derBGen; mDerGenT(2,0) = derCGen;
  TMatrix mMultiPartGen(1,3);
  mMultiPartGen = mDerGen;
  mMultiPartGen *= mCovGen;
  double afbGenError = lumiFactor * TMath::Sqrt(mMultiPartGen(0,0)*mDerGenT(0,0) + mMultiPartGen(0,1)*mDerGenT(1,0) + mMultiPartGen(0,2)*mDerGenT(2,0));
  double afbGenErrorOld = lumiFactor * TMath::Sqrt( TMath::Power(derAGen, 2.)*TMath::Power(uSigmaAGen, 2.) + TMath::Power(derBGen, 2.)*TMath::Power(uSigmaBGen, 2.) + TMath::Power(derCGen, 2.)*TMath::Power(uSigmaCGen, 2.) );
  std::cout << "afb = " << afbGen << std::endl;
  std::cout << "afb uncertainty = " << afbGenError << " (old: " << afbGenErrorOld << ")" << std::endl;
  std::cout << "afb rel. uncertainty = " << afbGenError/afbGen << std::endl;


  // //OLD PARAMETRISATION
  // TF1* afbFit = new TF1("afbFit","(3./8.)*(1+x*x)*[0]+(3./4.)*(1-x*x)*[1]+([0]+[1])*x*[2]",-1,1);
  // afbFit->SetParName(0,"sigmaU");
  // afbFit->SetParName(1,"sigmaL");
  // afbFit->SetParName(2,"AFB");
  // TFitResultPtr afbFitResult = histTopThetaRecoPassedMVACutScaled->Fit(afbFit,"RS");
  // double sigmaAstd = afbFit->GetParameter(0); double uSigmaAstd = afbFit->GetParError(0);
  // double sigmaBstd = afbFit->GetParameter(1); double uSigmaBstd = afbFit->GetParError(1);
  // double sigmaCstd = afbFit->GetParameter(2); double uSigmaCstd = afbFit->GetParError(2);
  // TMatrixDSym covstd = afbFitResult->GetCovarianceMatrix();
  // covstd.Print();
  // //CS - derive value and error
  // double csstd = sigmaAstd + sigmaBstd;
  // double derAcsstd = 1.;
  // double derBcsstd = 1.;
  // TMatrix mCovstd(2,2); mCovstd(0,0) = covstd(0,0); mCovstd(0,1) = covstd(0,1); mCovstd(1,1) = covstd(1,1); mCovstd(1,0) = covstd(1,0);
  // TMatrix mDercsstd(1,2); mDercsstd(0,0) = derAcsstd; mDercsstd(0,1) = derBcsstd;
  // TMatrix mDercsstdT(2,1); mDercsstdT(0,0) = derAcsstd; mDercsstdT(1,0) = derBcsstd;
  // TMatrix mMultiPartcsstd(1,2);
  // mMultiPartcsstd = mDercsstd;
  // mMultiPartcsstd *= mCovstd;
  // double csErrorstd = lumiFactor * TMath::Sqrt( mMultiPartcsstd(0,0)*mDercsstdT(0,0) + mMultiPartcsstd(0,1)*mDercsstdT(1,0) );
  // double csErrorstdOld = lumiFactor * TMath::Sqrt( TMath::Power(derAcsstd, 2.)*TMath::Power(uSigmaAstd, 2.) + TMath::Power(derBcsstd, 2.)*TMath::Power(uSigmaBstd, 2.) );
  // std::cout << "Standard FIT RESULTS!" << std::endl;
  // std::cout << "cs = " << csstd/lumiEff/0.1 << std::endl;
  // std::cout << "cs uncertainty = " << csErrorstd/lumiEff/0.1 << std::endl;
  // std::cout << "OLD cs uncertainty = " << csErrorstdOld/lumiEff/0.1 << std::endl;
  // std::cout << "cs rel. uncertainty = " << csErrorstd/csstd << std::endl;
  // //AFB - derive value and error
  // double afbstd = sigmaCstd;
  // double afbErrorstd = lumiFactor * TMath::Sqrt( covstd(2,2) );
  // double afbErrorstdOld = lumiFactor * uSigmaCstd;
  // std::cout << "afb = " << afbstd << std::endl;
  // std::cout << "afb uncertainty = " << afbErrorstd << std::endl;
  // std::cout << "OLD afb uncertainty = " << afbErrorstdOld << std::endl;
  // std::cout << "afb rel. uncertainty = " << afbErrorstd/afbstd << std::endl;


  //Plot distribution with fit
  TCanvas *cTopThetaDistributionwFit = new TCanvas("TopThetaDistributionwFit"+suffix, "JetMass", 900, 700);
  cTopThetaDistributionwFit->cd();
  //Legend setup
  TLegend *leggyTopThetawFit = new TLegend(0.21, 0.60, 0.57, 0.89, NULL, "brNDC");
  leggyTopThetawFit->SetFillColor(0);
  leggyTopThetawFit->SetFillStyle(0);
  leggyTopThetawFit->SetNColumns(1);
  //Draw scaled signal histogram as graph
  double cosTheta[20], crossSection[20], xError[20], yError[20];
  for (int bin=0;bin<histTopThetaRecoPassedMVACutScaled->GetNbinsX();bin++){
    cosTheta[bin] = histTopThetaRecoPassedMVACutScaled->GetXaxis()->GetBinCenter(bin+1);
    crossSection[bin] = histTopThetaRecoPassedMVACutScaled->GetBinContent(bin+1);
    xError[bin] = 0.;
    yError[bin] = histTopThetaRecoPassedMVACutScaled->GetBinError(bin+1);
  }
  TGraphAsymmErrors* grCosTheta = new TGraphAsymmErrors(20, cosTheta, crossSection, xError, xError, yError, yError);
  grCosTheta->SetLineColor(myColor3);
  grCosTheta->SetLineWidth(3);
  grCosTheta->SetMarkerStyle(24);
  grCosTheta->SetMarkerSize(1.4);
  grCosTheta->SetMarkerColor(myColor3);
  grCosTheta->Draw("AP");
  auto axis = grCosTheta->GetXaxis();
  axis->SetLimits(-1.,1.);
  grCosTheta->GetHistogram()->SetMinimum(0.);
  //Generated
  histTopThetaGen->SetLineColor(myColor5);
  histTopThetaGen->SetLineWidth(3);
  histTopThetaGen->DrawCopy("HIST SAME");
  //Draw
  afbFitNew->SetLineColor(myColor3);
  afbFitNew->SetLineStyle(3);
  afbFitNew->SetLineWidth(3);
  afbFitNew->Draw("SAME");
  //Total
  histTopThetaRecoPassedMVACutTot->SetLineColor(myColor4);
  histTopThetaRecoPassedMVACutTot->SetLineWidth(3);
  histTopThetaRecoPassedMVACutTot->SetLineStyle(2);
  histTopThetaRecoPassedMVACutTot->DrawCopy("HIST SAME");
  //Background level
  histTopThetaRecoPassedMVACutBkg->SetFillColorAlpha(myColor4,0.3);
  histTopThetaRecoPassedMVACutBkg->SetLineWidth(0);
  histTopThetaRecoPassedMVACutBkg->DrawCopy("HIST SAME");
  //final
  leggyTopThetawFit->AddEntry(histTopThetaGen,"Generated", "l");
  leggyTopThetawFit->AddEntry(grCosTheta,"Reco. corrected", "ep");
  leggyTopThetawFit->AddEntry(afbFitNew,"Fit", "l");
  leggyTopThetawFit->AddEntry(histTopThetaRecoPassedMVACutTot,"Reco.", "l");
  leggyTopThetawFit->AddEntry(histTopThetaRecoPassedMVACutBkg,"Background", "f");
  leggyTopThetawFit->Draw();
  grCosTheta->SetTitle(";cos #theta_{t};Events [750 fb^{-1}]");
  cTopThetaDistributionwFit->SaveAs((plotDirName+cTopThetaDistributionwFit->GetName()+".pdf").c_str());
  cTopThetaDistributionwFit->SaveAs((macroDirName+cTopThetaDistributionwFit->GetName()+".C").c_str());
  //end plot

  time (&end);
  double difference = difftime(end,begin);
  printf ("Time taken for function() %.2lf seconds.\n", difference );

}

void calcAfb(TH1F* hist){
  //get number of bins
  Int_t nBins = hist->GetNbinsX();
  double sumLeft = 0.;
  double sumRight = 0.;
  for(Int_t i=1; i<=nBins/2.; i++){
    sumLeft += hist->GetBinContent(i);
    sumRight += hist->GetBinContent(i+nBins/2.);
  }
  double sum = sumLeft+sumRight;

  double cs = sum/lumiEff;
  double sigmaCs = 1/TMath::Sqrt(cs)*lumiFactor;

  double afb = TMath::Abs(sumLeft-sumRight)/sum;
  double sigmaAfbRel = TMath::Sqrt(1-(afb*afb))/(afb*TMath::Sqrt(sum))*lumiFactor;

  std::cout << "Cross-section: " << cs << std::endl;
  std::cout << "Relative uncertainty: " << sigmaCs << std::endl;
  std::cout << "Asymmetry: " << afb << std::endl;
  std::cout << "Relative uncertainty (eq. error propagation): " << sigmaAfbRel << std::endl;
}

void fillHist(const TString& fileName, const TString& treeName, double eventWeight, std::vector<TH1F*> hists, double boostCutMin, double boostCutMax, const TString& mvaFileName, const TString& mvaTreeName, float mvaCut, bool isSignal, bool removeTau, bool useBkgBool){

  std::cout << "INPUT: " << fileName << std::endl;
  TFile* file = new TFile(fileName);
  if (file->IsZombie()) {
    std::cout << "Missing file: " << fileName << std::endl;
    return;
  }

  std::cout << "removeTau: " << removeTau << std::endl;

  TTree* tree = (TTree*)file->Get(treeName);
  int nEntries = tree->GetEntries();

  //Activate MC branches
  Bool_t singleTop;
  Bool_t compatibleWith6f;
  Int_t mcLepType;
  Double_t eventInvMass;
  tree->SetBranchAddress("singleTop", &singleTop);
  tree->SetBranchAddress("compatibleWith6f", &compatibleWith6f);
  tree->SetBranchAddress("mcLepType", &mcLepType);
  tree->SetBranchAddress("eventInvMass", &eventInvMass);

  //Activate reconstruction branches
  std::vector<Double_t>* vPhotonPt = 0;
  std::vector<Double_t>* vPhotonEta = 0;
  std::vector<Double_t>* vPhotonPhi = 0;
  std::vector<Double_t>* vPhotonE = 0;
  std::vector<Double_t>* vPhotonTheta = 0;
  tree->SetBranchAddress("vPhotonPt", &vPhotonPt);
  tree->SetBranchAddress("vPhotonEta", &vPhotonEta);
  tree->SetBranchAddress("vPhotonPhi", &vPhotonPhi);
  tree->SetBranchAddress("vPhotonEnergy", &vPhotonE);
  tree->SetBranchAddress("vPhotonTheta", &vPhotonTheta);

  std::vector<Double_t>* vRecoPt = 0;
  std::vector<Double_t>* vRecoEta = 0;
  std::vector<Double_t>* vRecoPhi = 0;
  std::vector<Double_t>* vRecoE = 0;
  tree->SetBranchAddress("vRecoPt", &vRecoPt);
  tree->SetBranchAddress("vRecoEta", &vRecoEta);
  tree->SetBranchAddress("vRecoPhi", &vRecoPhi);
  tree->SetBranchAddress("vRecoEnergy", &vRecoE);

  std::vector<Double_t>* vGenLepCharge = 0;
  tree->SetBranchAddress("vGenLepCharge", &vGenLepCharge);

  std::vector<Double_t>* vLepElectronCharge = 0;
  std::vector<Double_t>* vLepElectronPt = 0;
  std::vector<Double_t>* vLepElectronEta = 0;
  std::vector<Double_t>* vLepElectronPhi = 0;
  std::vector<Double_t>* vLepElectronE = 0;
  tree->SetBranchAddress("vLepElectronCharge", &vLepElectronCharge);
  tree->SetBranchAddress("vLepElectronPt", &vLepElectronPt);
  tree->SetBranchAddress("vLepElectronEta", &vLepElectronEta);
  tree->SetBranchAddress("vLepElectronPhi", &vLepElectronPhi);
  tree->SetBranchAddress("vLepElectronEnergy", &vLepElectronE);

  std::vector<Double_t>* vLepMuonCharge = 0;
  std::vector<Double_t>* vLepMuonPt = 0;
  std::vector<Double_t>* vLepMuonEta = 0;
  std::vector<Double_t>* vLepMuonPhi = 0;
  std::vector<Double_t>* vLepMuonE = 0;
  tree->SetBranchAddress("vLepMuonCharge", &vLepMuonCharge);
  tree->SetBranchAddress("vLepMuonPt", &vLepMuonPt);
  tree->SetBranchAddress("vLepMuonEta", &vLepMuonEta);
  tree->SetBranchAddress("vLepMuonPhi", &vLepMuonPhi);
  tree->SetBranchAddress("vLepMuonEnergy", &vLepMuonE);

  std::vector<Double_t>* vJetPt = 0;
  std::vector<Double_t>* vJetE = 0;
  std::vector<Double_t>* vJetEta = 0;
  std::vector<Double_t>* vJetPhi = 0;
  tree->SetBranchAddress("vJetPt", &vJetPt);
  tree->SetBranchAddress("vJetEnergy", &vJetE);
  tree->SetBranchAddress("vJetEta", &vJetEta);
  tree->SetBranchAddress("vJetPhi", &vJetPhi);

  std::vector<Double_t>* vGenTopPt = 0;
  std::vector<Double_t>* vGenTopE = 0;
  std::vector<Double_t>* vGenTopEta = 0;
  std::vector<Double_t>* vGenTopPhi = 0;
  std::vector<Double_t>* vGenTopTheta = 0;
  std::vector<Double_t>* vGenTopCharge = 0;
  tree->SetBranchAddress("vGenTopPt", &vGenTopPt);
  tree->SetBranchAddress("vGenTopEnergy", &vGenTopE);
  tree->SetBranchAddress("vGenTopEta", &vGenTopEta);
  tree->SetBranchAddress("vGenTopPhi", &vGenTopPhi);
  tree->SetBranchAddress("vGenTopTheta", &vGenTopTheta);
  tree->SetBranchAddress("vGenTopCharge", &vGenTopCharge);

  std::vector<Double_t>* vTopPt = 0;
  std::vector<Double_t>* vTopE = 0;
  std::vector<Double_t>* vTopEta = 0;
  std::vector<Double_t>* vTopPhi = 0;
  std::vector<Double_t>* vTopTheta = 0;
  tree->SetBranchAddress("vTopPt", &vTopPt);
  tree->SetBranchAddress("vTopEnergy", &vTopE);
  tree->SetBranchAddress("vTopEta", &vTopEta);
  tree->SetBranchAddress("vTopPhi", &vTopPhi);
  tree->SetBranchAddress("vTopTheta", &vTopTheta);

  std::vector<Int_t>* vTopTagged = 0;
  std::vector<Double_t>* vTopMass = 0;
  std::vector<Double_t>* vTopMassW = 0;
  std::vector<float>* vTopTaggedWCosTheta = 0;
  tree->SetBranchAddress("vTopTagged", &vTopTagged);
  tree->SetBranchAddress("vTopMass", &vTopMass);
  tree->SetBranchAddress("vTopMassW", &vTopMassW);
  tree->SetBranchAddress("vTopTaggedWCosTheta", &vTopTaggedWCosTheta);

  //Fit
  TString fitFileName = "/afs/cern.ch/user/l/lstroem/topa/fit/fitPol1_vlc_1.4_1.0_1.0.root";
  TFile *fitFile = new TFile(fitFileName,"read"); fitFile->ls();
  TF1* fitResults = (TF1*) fitFile->Get("pol1");
  double p0 = fitResults->GetParameter(0);
  double p1 = fitResults->GetParameter(1);

  //MVA prediction
  //std::cout << mvaFileName << " " << mvaTreeName << std::endl;
  TFile *mvaFile = new TFile(mvaFileName, "read");
  mvaFile->ls();
  TTree* mvaTree = (TTree*) mvaFile->Get(mvaTreeName);
  Long64_t nPFO;
  Double_t score;
  mvaTree->SetBranchAddress("nPFO",&nPFO);
  mvaTree->SetBranchAddress("score",&score);
  int iMVA = 0;

  //Loop settings

  //Fixed number of events for each sample
  //double maxEntries = nEntries;
  double maxEntries = 100;
  double eventWeightScaled = eventWeight*lumiEff/(double)maxEntries;

  //Fixed luminosity for each sample
  //double maxEntries = eventWeight*lumiEff;
  //double eventWeightScaled = 1.;

  if(maxEntries > nEntries){
    std::cout << Form("WARNING! Too many events requested (%.2f), using (%i)!", maxEntries, nEntries) << std::endl;
    maxEntries = nEntries;
    std::cout << Form("Recalculating weights: before (%.2f), after (%.2f)", 1., eventWeightScaled) << std::endl;
    eventWeightScaled = eventWeight*lumiEff/(double)maxEntries;
  }

  int nEvents = 0;
  double sumTestTot = 0.;
  double sumTest0 = 0.;
  double sumTest1 = 0.;
  double sumTest2 = 0.;
  double sumTest3 = 0.;

  std::cout << "Passed cuts: " << std::endl;
  std::cout << "nIsoPhotons == 0" << std::endl;
  std::cout << "nLeptonsChosen == 1" << std::endl;
  std::cout << "nTopTaggedJets == 1" << std::endl;
  std::cout << "comCorr >= " << boostCutMin << std::endl;
  std::cout << "comCorr < " << boostCutMax << std::endl;

  //Loop over all
  std::cout << "Initializing " << maxEntries << " events!" << std::endl;
  std::cout << "Event weight = " << eventWeightScaled << std::endl;
  for(int i=0; i<maxEntries; ++i){

    tree->GetEntry(i);
    if(i>=maxEntries){
      std::cout << "Limit on maximum number of events loaded has been reached, breaking!" << std::endl;
      break;
    }

    nEvents += 1;

    //Count energetic jets
    int nLoop = 2;

    //Get COM frame for gen.
    TLorentzVector tlvGenTopCOM;
    for (unsigned int j=0; j<vGenTopPt->size(); j++){
      TLorentzVector tlv;
      tlv.SetPtEtaPhiE( vGenTopPt->at(j), vGenTopEta->at(j), vGenTopPhi->at(j), vGenTopE->at(j));
      tlvGenTopCOM += tlv;
    }
    TVector3 comBoostVectorGen = tlvGenTopCOM.BoostVector();

    //adding leptons
    int nLeptons = vLepElectronCharge->size() + vLepMuonCharge->size();
    std::vector<Double_t>* vLepCharge = vLepElectronCharge;
    vLepCharge->insert(vLepCharge->end(), vLepMuonCharge->begin(), vLepMuonCharge->end());
    std::vector<Double_t>* vLepPt = vLepElectronPt;
    vLepPt->insert(vLepPt->end(), vLepMuonPt->begin(), vLepMuonPt->end());
    std::vector<Double_t>* vLepPhi = vLepElectronPhi;
    vLepPhi->insert(vLepPhi->end(), vLepMuonPhi->begin(), vLepMuonPhi->end());
    std::vector<Double_t>* vLepEta = vLepElectronEta;
    vLepEta->insert(vLepEta->end(), vLepMuonEta->begin(), vLepMuonEta->end());
    std::vector<Double_t>* vLepE = vLepElectronE;
    vLepE->insert(vLepE->end(), vLepMuonE->begin(), vLepMuonE->end());

    //sorting and picking lepton
    std::vector<Double_t> vLepChargeChosen;
    std::vector<Double_t> vLepPtChosen;
    std::vector<Double_t> vLepPhiChosen;
    std::vector<Double_t> vLepEtaChosen;
    std::vector<Double_t> vLepEChosen;

    if (nLeptons > 1){
      std::vector<std::pair<Double_t, Int_t> > vPairLep;
      for(unsigned int r=0; r<vLepPt->size(); ++r){
	vPairLep.push_back(std::make_pair(vLepPt->at(r), r));
      }
      std::sort(vPairLep.begin(), vPairLep.end(),  wayToSortLepPt);
      int indexChosen = vPairLep.at(0).second;
      vLepChargeChosen.push_back((*vLepCharge).at(indexChosen));
      vLepPtChosen.push_back((*vLepPt).at(indexChosen));
      vLepPhiChosen.push_back((*vLepPhi).at(indexChosen));
      vLepEtaChosen.push_back((*vLepEta).at(indexChosen));
      vLepEChosen.push_back((*vLepE).at(indexChosen));
    } else if (nLeptons == 1){
      vLepChargeChosen.push_back((*vLepCharge).at(0));
      vLepPtChosen.push_back((*vLepPt).at(0));
      vLepPhiChosen.push_back((*vLepPhi).at(0));
      vLepEtaChosen.push_back((*vLepEta).at(0));
      vLepEChosen.push_back((*vLepE).at(0));
    }
    int nLeptonsChosen = vLepChargeChosen.size();

    //Isolated photon
    int nIsoPhotons = 0;
    double eCut = 15.;
    std::vector<float> vRelConeE3 = getRelConeEnergyVector(0.9950, vPhotonPt, vPhotonE, vPhotonEta, vPhotonPhi, vRecoPt, vRecoE, vRecoEta, vRecoPhi);
    nIsoPhotons = 0;
    for (unsigned int j=0; j<vPhotonTheta->size(); j++){
      if ( (fabs(TMath::Cos(vPhotonTheta->at(j))) <= fabs(TMath::Cos(TMath::DegToRad()*10.))) && (vPhotonE->at(j)>=eCut) ){
	if ( vRelConeE3.at(j) <= 1.2 ){
	  if ( vPhotonPt->at(j) >= 75. ){
	    nIsoPhotons++;
	  }
	}
      }
    }

    //Reconstruction of s'
    double comCorr = 0.;
    TLorentzVector tlvTotal;
    if (nLeptonsChosen == 1){
      TLorentzVector tlvVisible;
      TLorentzVector tlvLepton; tlvLepton.SetPtEtaPhiE((vLepPtChosen).at(0), (vLepEtaChosen).at(0), (vLepPhiChosen).at(0), (vLepEChosen).at(0));
      tlvVisible += tlvLepton;
      std::vector<TLorentzVector> vTlvJet;
      for (int r = 0; r < nLoop; r++){
    	TLorentzVector tlvJet; tlvJet.SetPtEtaPhiE(vJetPt->at(r), vJetEta->at(r), vJetPhi->at(r), vJetE->at(r));
    	vTlvJet.push_back(tlvJet);
    	tlvVisible += tlvJet;
      }
      tlvTotal = getRecoSPrime(tlvVisible, tlvLepton, vTlvJet);
      double com = tlvTotal.M();
      //Bias correction
      comCorr =  (com-p0)/p1;
    }
    TVector3 comBoostVector = tlvTotal.BoostVector();

    //Top tagger setup
    double _defMinTopMass = 145.;
    double _defMaxTopMass = 205.;
    double _defMinWMass = 65.;
    double _defMaxWMass = 95.;
    int nTopTaggedCandJets = 0;
    for(int k=0; k<nLoop; ++k){
      if( ((*vTopTagged).at(k) == 1) ){ //sub-structure test only
    	nTopTaggedCandJets++;
      }
    }
    int nTopTaggedJets = 0;
    std::vector<bool> vTopTaggedPassed;
    if ( nTopTaggedCandJets == 2 ){
      for(int k=0; k<nLoop; ++k){
    	if ( ((*vTopMass).at(k) <= _defMaxTopMass) && ((*vTopMass).at(k) >= _defMinTopMass) && ((*vTopMassW).at(k) <= _defMaxWMass) && ((*vTopMassW).at(k) >= _defMinWMass) && (abs(vTopTaggedWCosTheta->at(k)) <= 0.8) ){
    	  nTopTaggedJets++;
	  vTopTaggedPassed.push_back(true);
    	} else {
	  vTopTaggedPassed.push_back(false);
	}
      }
    } else if ( nTopTaggedCandJets == 1 ){
      for(int k=0; k<nLoop; ++k){
    	if ( ((*vTopTagged).at(k) == 1) ){
    	  if ( ((*vTopMass).at(0) <= _defMaxTopMass) && ((*vTopMass).at(0) >= _defMinTopMass) && ((*vTopMassW).at(0) <= _defMaxWMass) && ((*vTopMassW).at(0) >= _defMinWMass) && (abs(vTopTaggedWCosTheta->at(k)) <= 0.8) ){
    	    nTopTaggedJets++;
	    vTopTaggedPassed.push_back(true);
    	  } else {
	    vTopTaggedPassed.push_back(false);
	  }
    	} else {
	  vTopTaggedPassed.push_back(false);
	}
      }
    } else {
      for(int k=0; k<nLoop; ++k){
	vTopTaggedPassed.push_back(false);
      }
    }

    //Top tagged jet
    int indexTopTagged = 0;
    for (int r = 0; r < nLoop; r++){
      if ( vTopTaggedPassed.at(r) ){
	if (nTopTaggedCandJets==2){
	  indexTopTagged = r;
	} else if (nTopTaggedCandJets==1){
	  indexTopTagged = 0;
	}
      }
    }

    //Event selection
    bool passedCuts = false;
    if ( (nIsoPhotons == 0) && (nLeptonsChosen == 1) && (nTopTaggedJets == 1) && (comCorr >= boostCutMin) && (comCorr < boostCutMax) ){
      passedCuts = true;
    }
    // std::cout << "passedCuts: " << passedCuts << std::endl;

    if( useBkgBool && compatibleWith6f ){
      if (passedCuts == true){
	mvaTree->GetEntry(iMVA); iMVA++;
      }
      continue;
    }

    //Signal selection
    if( removeTau && (TMath::Abs(mcLepType) == 15) ){
      if (passedCuts == true){
	mvaTree->GetEntry(iMVA); iMVA++;
      }
      continue;
    } else {

      if ( (eventInvMass >= 0.) && (eventInvMass < 1.e4) ){
	sumTestTot+=eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType) != 15);
      }

      if ( (eventInvMass < 400.) ){
	//sumTest0+=eventWeightScaled*(TMath::Abs(mcLepType) != 15);
	sumTest0+=eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType) != 15);
      } else if ( (eventInvMass >= 400.) && (eventInvMass < 900.) ){
	//sumTest1+=eventWeightScaled*(TMath::Abs(mcLepType) != 15);
	sumTest1+=eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType) != 15);
      } else if ( (eventInvMass >= 900.) && (eventInvMass < 1200.) ){
	//sumTest2+=eventWeightScaled*(TMath::Abs(mcLepType) != 15);
	sumTest2+=eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType) != 15);
      } else if ( (eventInvMass >= 1200.) && (eventInvMass < 1.e4) ){
	//sumTest3+=eventWeightScaled*(TMath::Abs(mcLepType) != 15);
	sumTest3+=eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType) != 15);
      }

      if (vGenLepCharge->size() == 1){
	for(size_t j=0; j<vGenTopTheta->size(); ++j){
	  if( ((-vGenLepCharge->at(0))*(vGenTopCharge->at(j))) > 0. ){ //fill hadronic top (i.e. one entry per event)
	    TLorentzVector tlv;
	    tlv.SetPtEtaPhiE( vGenTopPt->at(j), vGenTopEta->at(j), vGenTopPhi->at(j), vGenTopE->at(j) );
	    std::cout << "Before boost: " << std::endl;
	    std::cout << "px: " << tlv.Px() << std::endl;
	    std::cout << "py: " << tlv.Py() << std::endl;
	    std::cout << "pz: " << tlv.Pz() << std::endl;
	    tlv.Boost( -comBoostVectorGen);
	    std::cout << "After boost: " << std::endl;
	    std::cout << "px: " << tlv.Px() << std::endl;
	    std::cout << "py: " << tlv.Py() << std::endl;
	    std::cout << "pz: " << tlv.Pz() << std::endl;
	    hists.at(TopThetaGenBoostedPart1)->Fill((-vGenLepCharge->at(0))*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==0)*(TMath::Abs(mcLepType)!=15)*(i%2==0));
	    hists.at(TopThetaGenBoostedPart2)->Fill((-vGenLepCharge->at(0))*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==0)*(TMath::Abs(mcLepType)!=15)*(i%2!=0));
	  }
	}
      }

      int topsign = 0;
      if( nLeptonsChosen == 1 ){
	if( vLepChargeChosen.at(0) > 0. ){ //if pos lep found, top tagger must have catched antitop
	  topsign = -1;
	} else if( vLepChargeChosen.at(0) < 0. ){
	  topsign = 1;
	}
      }

      if (passedCuts == true){
	mvaTree->GetEntry(iMVA); iMVA++;
	TLorentzVector tlv;
	tlv.SetPtEtaPhiE( vTopPt->at(indexTopTagged), vTopEta->at(indexTopTagged), vTopPhi->at(indexTopTagged), vTopE->at(indexTopTagged) );
	tlv.Boost( -comBoostVector);
	if (score >= mvaCut){
	  if (isSignal){
	    hists.at(TopThetaRecoBoostedPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==0)*(TMath::Abs(mcLepType)!=15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedTauPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==0)*(TMath::Abs(mcLepType)==15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedSTPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==1)*(TMath::Abs(mcLepType)!=15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==1)*(TMath::Abs(mcLepType)==15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedCompPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass<boostCutMin)*(singleTop==0)*(TMath::Abs(mcLepType)!=15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedTauCompPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass<boostCutMin)*(singleTop==0)*(TMath::Abs(mcLepType)==15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedSTCompPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass<boostCutMin)*(singleTop==1)*(TMath::Abs(mcLepType)!=15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedSTTauCompPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass<boostCutMin)*(singleTop==1)*(TMath::Abs(mcLepType)==15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==0)*(TMath::Abs(mcLepType)!=15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedTauPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==0)*(TMath::Abs(mcLepType)==15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedSTPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==1)*(TMath::Abs(mcLepType)!=15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass>=boostCutMin)*(eventInvMass<boostCutMax)*(singleTop==1)*(TMath::Abs(mcLepType)==15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedCompPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass<boostCutMin)*(singleTop==0)*(TMath::Abs(mcLepType)!=15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedTauCompPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass<boostCutMin)*(singleTop==0)*(TMath::Abs(mcLepType)==15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedSTCompPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass<boostCutMin)*(singleTop==1)*(TMath::Abs(mcLepType)!=15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedSTTauCompPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(eventInvMass<boostCutMin)*(singleTop==1)*(TMath::Abs(mcLepType)==15)*(i%2!=0));
	  } else {
	    hists.at(TopThetaRecoBoostedPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType)!=15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedTauPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType)==15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedSTPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(singleTop==1)*(TMath::Abs(mcLepType)!=15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedSTTauPassedMVACutPart1)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(singleTop==1)*(TMath::Abs(mcLepType)==15)*(i%2==0));
	    hists.at(TopThetaRecoBoostedPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType)!=15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedTauPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(singleTop==0)*(TMath::Abs(mcLepType)==15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedSTPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(singleTop==1)*(TMath::Abs(mcLepType)!=15)*(i%2!=0));
	    hists.at(TopThetaRecoBoostedSTTauPassedMVACutPart2)->Fill(topsign*TMath::Cos(tlv.Theta()), eventWeightScaled*(singleTop==1)*(TMath::Abs(mcLepType)==15)*(i%2!=0));
	  }
	}
      }

    } //end else tau

  } //end event loop

  std::cout << "[0,400]: " << sumTest0/lumiEff << std::endl;
  std::cout << "[400,900]: " << sumTest1/lumiEff << std::endl;
  std::cout << "[900,1200]: " << sumTest2/lumiEff << std::endl;
  std::cout << "[1200, 1e4]: " << sumTest3/lumiEff << std::endl;
  std::cout << "Total cross-section: " << (sumTest0+sumTest1+sumTest2+sumTest3)/lumiEff << " fb" << std::endl;
  std::cout << "Total cross-section (validation): " << sumTestTot/lumiEff << " fb" << std::endl;

} //end function

template <class T>
void pretty_print(std::vector<T>& v){
  std::cout << " [ ";
  for (size_t i = 0; i < v.size(); ++i) { std::cout << v[i] << " "; }
  std::cout << "] " << std::endl;
}

bool wayToSortArray(std::vector<std::pair<TH1F*, TString> > lhs, std::vector<std::pair<TH1F*, TString> > rhs){
  return (lhs.at(0).first)->GetSumOfWeights() > (rhs.at(0).first)->GetSumOfWeights();
}

bool wayToSortReverseArray(std::vector<std::pair<TH1F*, TString> > lhs, std::vector<std::pair<TH1F*, TString> > rhs){
  return (lhs.at(0).first)->GetSumOfWeights() < (rhs.at(0).first)->GetSumOfWeights();
}

void sortChannels(std::vector<std::vector<std::pair<TH1F*, TString> > > &hChannels){
  std::sort(hChannels.begin(), hChannels.end(), wayToSortArray);
  return;
}

void sortReverseChannels(std::vector<std::vector<std::pair<TH1F*, TString> > > &hChannels){
  std::sort(hChannels.begin(), hChannels.end(), wayToSortReverseArray);
  return;
}

bool wayToSortLepPt(std::pair<Double_t, Int_t> lhs, std::pair<Double_t, Int_t> rhs){
  return (lhs.first) > (rhs.first);
}

bool wayToSortSemiLepCandidate(std::pair<TLorentzVector, int> lhs, std::pair<TLorentzVector, int> rhs){
  double _mt = 174.0;
  return (lhs.first.M() - _mt) < (rhs.first.M() - _mt);
}

float getConeEnergy(float cosConeAngle, TLorentzVector& tlvPhoton, std::vector<TLorentzVector>& vTlvPFOs){
  float coneE = 0;
  int nPFO = vTlvPFOs.size();
  for (int i=0; i<nPFO; i++){
    if ( vTlvPFOs.at(i).Angle(tlvPhoton.Vect()) <= TMath::ACos(cosConeAngle) ){
      coneE += vTlvPFOs.at(i).E();
    }
  }
  return coneE;
} //end function getConeEnergy

void correctUncertainty(TH1F* hist){
  for (int bin=0;bin<=hist->GetNbinsX()+1;bin++){
    hist->SetBinError(bin, TMath::Sqrt(hist->GetBinContent(bin)));
  }
}

std::vector<float> getRelConeEnergyVector(float cosConeAngle, std::vector<Double_t>* vPhotonPt, std::vector<Double_t>* vPhotonE, std::vector<Double_t>* vPhotonEta, std::vector<Double_t>* vPhotonPhi, std::vector<Double_t>* vRecoPt, std::vector<Double_t>* vRecoE, std::vector<Double_t>* vRecoEta, std::vector<Double_t>* vRecoPhi){
  std::vector<TLorentzVector> vTlvPFO;
  for (unsigned int j=0; j<vRecoE->size(); j++){
    TLorentzVector tlvPFO;
    tlvPFO.SetPtEtaPhiE( vRecoPt->at(j), vRecoEta->at(j), vRecoPhi->at(j), vRecoE->at(j));
    vTlvPFO.push_back(tlvPFO);
  }
  std::vector<float> vConeE;
  for (unsigned int j=0; j<vPhotonE->size(); j++){
    TLorentzVector tlvPhoton;
    tlvPhoton.SetPtEtaPhiE( vPhotonPt->at(j), vPhotonEta->at(j), vPhotonPhi->at(j), vPhotonE->at(j));
    vConeE.push_back( getConeEnergy(cosConeAngle, tlvPhoton, vTlvPFO)/tlvPhoton.E() );
  }
  return vConeE;
} //end funton getConeEnergyVector

TLorentzVector getRecoSPrime(TLorentzVector visible, TLorentzVector lepton, std::vector<TLorentzVector> vTlvJet){

  double _mW = 80.419;

  //Initial assumptions
  double pXnu = -visible.Px();
  double pYnu = -visible.Py();
  double pXlep = lepton.Px();
  double pYlep = lepton.Py();
  double pZllep = lepton.Pz();
  double elep = lepton.E();
  double mlep = lepton.M();

  //Useful defintions
  double MET = sqrt(pow(pXnu, 2) + pow(pYnu, 2));
  double solutionPart = pow(elep,2)*(pow(pow(_mW,2) - pow(mlep,2) + 2*(pXlep*pXnu + pYlep*pYnu), 2) + 4*pow(MET, 2)*(-pow(elep,2) + pow(pZllep,2)));

  //Solutions to the equation
  TLorentzVector tlvWSolution1;
  TLorentzVector tlvWSolution2;

  if ( solutionPart >= 0. ){

    //Solution will give mW by definition
    long double pZnuSolution1 = (pZllep*pow(mlep,2) - pZllep*pow(_mW,2) - 2*pXlep*pZllep*pXnu - 2*pYlep*pZllep*pYnu + sqrt(solutionPart))/(2.*(pow(pZllep,2) - pow(elep,2)));
    long double pZnuSolution2 = -(-(pZllep*pow(mlep,2)) + pZllep*pow(_mW,2) + 2*pXlep*pZllep*pXnu + 2*pYlep*pZllep*pYnu + sqrt(solutionPart))/(2.*(pow(pZllep,2) - pow(elep,2)));
    TLorentzVector tlvNeutrinoSolution1; tlvNeutrinoSolution1.SetPxPyPzE( pXnu, pYnu, pZnuSolution1, sqrt(pow(pXnu,2)+pow(pYnu,2)+pow(pZnuSolution1,2)));
    TLorentzVector tlvNeutrinoSolution2; tlvNeutrinoSolution2.SetPxPyPzE( pXnu, pYnu, pZnuSolution2, sqrt(pow(pXnu,2)+pow(pYnu,2)+pow(pZnuSolution2,2)));
    tlvWSolution1 += tlvNeutrinoSolution1+lepton;
    tlvWSolution2 += tlvNeutrinoSolution2+lepton;

  } else {

    //Solution will NOT give mW since MET has to be scaled

    //Determine MET so that the func(MET) = 0
    TLorentzVector tlvNeutrino; tlvNeutrino.SetPxPyPzE( pXnu, pYnu, 0., sqrt(pow(pXnu, 2) + pow(pYnu, 2)));
    long double METSolution1 = (-sqrt(-( (pow(pZllep,2) - pow(elep,2))*pow(pow(mlep,2) - pow(_mW,2),2) )) + pXlep*(pow(mlep,2) - pow(_mW,2))*cos(tlvNeutrino.Phi()) + pYlep*(pow(mlep,2) - pow(_mW,2))*sin(tlvNeutrino.Phi()))\
      /(2.*(pow(pZllep,2) - pow(elep,2) + pow(pXlep,2)*pow(cos(tlvNeutrino.Phi()),2) + pow(pYlep,2)*pow(sin(tlvNeutrino.Phi()),2) + pXlep*pYlep*sin(2.*tlvNeutrino.Phi())));
    long double METSolution2 = (sqrt(-((pow(pZllep,2) - pow(elep,2))*pow(pow(mlep,2) - pow(_mW,2),2))) + pXlep*(pow(mlep,2) - pow(_mW,2))*cos(tlvNeutrino.Phi()) + pYlep*(pow(mlep,2) - pow(_mW,2))*sin(tlvNeutrino.Phi()))\
      /(2.*(pow(pZllep,2) - pow(elep,2) + pow(pXlep,2)*pow(cos(tlvNeutrino.Phi()),2) + pow(pYlep,2)*pow(sin(tlvNeutrino.Phi()),2) + pXlep*pYlep*sin(2.*tlvNeutrino.Phi())));

    //Scale the MET
    long double pZnuSolution1 = (pZllep*pow(mlep,2) - pZllep*pow(_mW,2) - 2*pXlep*pZllep*METSolution1*cos(tlvNeutrino.Phi()) - 2*pYlep*pZllep*METSolution1*sin(tlvNeutrino.Phi()))/(2.*(pow(pZllep,2) - pow(elep,2)));
    long double pZnuSolution2 = -(-(pZllep*pow(mlep,2)) + pZllep*pow(_mW,2) + 2*pXlep*pZllep*METSolution2*cos(tlvNeutrino.Phi()) + 2*pYlep*pZllep*METSolution2*cos(tlvNeutrino.Phi()))/(2.*(pow(pZllep,2) - pow(elep,2)));
    TLorentzVector tlvNeutrinoSolution1; tlvNeutrinoSolution1.SetPxPyPzE( pXnu, pYnu, pZnuSolution1, sqrt(pow(pXnu,2)+pow(pYnu,2)+pow(pZnuSolution1,2)));
    TLorentzVector tlvNeutrinoSolution2; tlvNeutrinoSolution2.SetPxPyPzE( pXnu, pYnu, pZnuSolution2, sqrt(pow(pXnu,2)+pow(pYnu,2)+pow(pZnuSolution2,2)));
    tlvWSolution1 += tlvNeutrinoSolution1+lepton;
    tlvWSolution2 += tlvNeutrinoSolution2+lepton;
  }

  //Select solution that gives results closest to mTop
  int nLoop = vTlvJet.size();
  std::vector<std::pair<TLorentzVector, int> > vSolutionPair;
  for (int r = 0; r < nLoop; r++){
    TLorentzVector tlvSolution1;
    tlvSolution1 += tlvWSolution1;
    tlvSolution1 += vTlvJet.at(r);
    vSolutionPair.push_back(std::make_pair(tlvSolution1, r));
    TLorentzVector tlvSolution2;
    tlvSolution2 += tlvWSolution2;
    tlvSolution2 += vTlvJet.at(r);
    vSolutionPair.push_back(std::make_pair(tlvSolution2, r));
  }
  std::sort(vSolutionPair.begin(), vSolutionPair.end(),  wayToSortSemiLepCandidate);
  int indexSemiLepSelect = vSolutionPair.at(0).second;

  //Calculate the collision energy
  TLorentzVector tlvTotal;
  tlvTotal += vSolutionPair.at(0).first; //semi-lep side
  //TLorentzVector tlvJet;

  if (indexSemiLepSelect > 0){
    tlvTotal += vTlvJet.at(0);
  } else {
    tlvTotal += vTlvJet.at(1);
  }
  //double mass = tlvTotal.M();
  return tlvTotal;
 }

# ifndef __CNT__
int main(){
  doStuff();
  return 0;
}
# endif
