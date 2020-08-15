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

  cr_study();
  ~cr_study();

};



#endif

