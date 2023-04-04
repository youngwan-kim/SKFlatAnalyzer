#ifndef WRStudy_h
#define WRStudy_h

#include "AnalyzerCore.h"

class WRStudy : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<Muon> AllMuons;
  vector<Electron> AllElectrons;
  vector<Tau> AllTaus;
  vector<Lepton *> AllLeptons;
  vector<Lepton *> Leptons;
  vector<Lepton *> LeptonsVeto;
  vector<Jet> AllJets;
  vector<Jet> BJets;
  vector<TString> triggers;
  vector<TString> singleEle_triggers;
  vector<TString> singleMu_triggers;
  

  vector<TString> etau_triggers;
  vector<TString> mutau_triggers;
  
  vector<Gen> AllGens;


  WRStudy();
  ~WRStudy();

};



#endif

