#ifndef WRTau_SignalStudy_h
#define WRTau_SignalStudy_h

#include "WRTau_Core.h"

class WRTau_SignalStudy : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  WRTau_SignalStudy();
  ~WRTau_SignalStudy();

  vector<Muon> AllMuons;
  vector<Electron> AllElectrons;
  vector<Tau> AllTaus;
  vector<Lepton *> AllLeptons;
  vector<Lepton *> Leptons;
  vector<Lepton *> LeptonsVeto;
  vector<Jet> AllJets;
  vector<Jet> BJets;
  vector<FatJet> AllFatJets;
  vector<Gen> AllGens;
  vector<LHE> AllLHEs;

  TString muon_IsoSF_loose_key = "";
  TString muon_IsoSF_tight_key = "";

  TString electron_looseID_SF_Key = "passLooseID";
  TString muon_recoSF_key = "Default";

  vector<TString> TriggerList;
  vector<TString> SingleLeptonTriggers;
  vector<TString> SingleElectronTriggers;
  vector<TString> SingleMuonTriggers;
  vector<TString> SingleTauTriggers;


  vector<TString> trigger;

};



#endif

