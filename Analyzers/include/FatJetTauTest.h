#ifndef FatJetTauTest_h
#define FatJetTauTest_h

#include "WRTau_Core.h"

class FatJetTauTest : public WRTau_Core {

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
  vector<FatJet> AllFatJets;
  vector<Gen> AllGens;
  vector<LHE> AllLHEs;

  FatJetTauTest();
  ~FatJetTauTest();

  std::string DeepTauVSjet = "DeepTau2017v2p1VSjet";
  std::string DeepTauVSe = "DeepTau2017v2p1VSe";
  std::string DeepTauVSmu = "DeepTau2017v2p1VSmu";

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

