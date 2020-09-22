#ifndef cr_study_h
#define cr_study_h

#include "AnalyzerCore.h"

class cr_study : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

/*  vector<Electron> AllElectrons;
  vector<Muon> AllMuons;
  vector<Jet> AllJets;
  vector<Gen> Gens;

  vector<TString> MuonIDs, MuonIDSFKeys, MuonTriggers;

  TString TriggerSafePtCut;*/
  
  vector<TString> Triggers_POGTight_Muon;
  TString TriggerNameForSF_POGTight_Muon;
  double TriggerSafePt_POGTight_Muon;

  vector<TString> Triggers_POGHighPt_Muon;
  TString TriggerNameForSF_POGHighPt_Muon;
  double TriggerSafePt_POGHighPt_Muon;

  double weight_Prefire;

  std::vector<TString> dimuon_trigger = {

    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"

  };

  std::vector<TString> dimuon_trigger_BG = {

    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
  //  "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
  //  "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"

  };

  std::vector<TString> dimuon_trigger_H = {

    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"

  };

  std::vector<Muon> muons, muons_tight, muons_loose, muons_veto;
  std::vector<Electron> electrons, electrons_veto;
  std::vector<Jet> jets,bjets;


  void DrawHists(TString path, const vector<Muon>& muons, const double& weight);
//  void cr_study::DrawHists(TString path, const vector<Electron>& electrons, const double& weight);
  void DrawHists(TString path, const vector<Jet>& jets, const double& weight);
  void DrawHists(TString path, const Particle& METv, const double& weight);

  cr_study();
  ~cr_study();

};


#endif

