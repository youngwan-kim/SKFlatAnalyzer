#ifndef WRTau_SR_Debug_h
#define WRTau_SR_Debug_h

#include "AnalyzerCore.h"

class WRTau_SR_Debug : public AnalyzerCore {

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


  vector<TString> singletau_highpt_trigger;
  vector<TString> singletau_trigger;
  vector<TString> tau_met_trigger;
  vector<TString> etau_trigger;
  vector<TString> mutau_trigger;

  WRTau_SR_Debug();
  ~WRTau_SR_Debug();

};



#endif

