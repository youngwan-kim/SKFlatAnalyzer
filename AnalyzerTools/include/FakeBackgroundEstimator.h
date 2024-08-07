#ifndef FakeBackgroundEstimator_h
#define FakeBackgroundEstimator_h

#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include "TFile.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"

#include "Lepton.h"
#include "Muon.h"
#include "Electron.h"

#include "AnalyzerParameter.h"
#include "TDirectoryHelper.h"

class FakeBackgroundEstimator{

public:

  FakeBackgroundEstimator();
  ~FakeBackgroundEstimator();

  TDirectory *histDir;
  void ReadHistograms();
  void ReadTauHistograms();

  bool IgnoreNoHist;

  int DataYear;
  TString DataEra;
  void SetEra(TString era){
    DataEra=era;
    DataYear=TString(era(0,4)).Atoi();
  }
  TString GetEra() const { return DataEra; }
  int GetYear() const { return DataYear; }

  std::map< TString, TH2D* > map_hist_Electron;
  std::map< TString, TH2D* > map_hist_Muon;
  TH1D* Tau_PR_Resolved_El;
  TH1D* Tau_PR_Boosted_El;
  TH1D* Tau_PR_Resolved_Mu;
  TH1D* Tau_PR_Boosted_Mu;
  std::map< TString, TF1* >  map_TF1_tau;
  std::map< TString, TH1D* > map_FitErr_tau;
  std::map< TString, TH1D* > map_hist_tau;

  std::map< TString, TH1D* > map_TauFF_QCD;
  std::map< TString, TH1D* > map_TauFF_TT;
  std::map< TString, TH1D* > map_TauFF_Ratio;

  double GetElectronFakeRate(TString ID, TString key, double eta, double pt, int sys=0);
  double GetMuonFakeRate(TString ID, TString key, double eta, double pt, int sys=0);
  double GetTauPromptRate(TString region,TString channel,double pt, int sys=0);

  bool HasLooseLepton;
  double GetWeight(vector<Lepton *> lepptrs, AnalyzerParameter param, int sys=0);

};

#endif
