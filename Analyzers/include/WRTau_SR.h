#ifndef WRTau_SR_h
#define WRTau_SR_h

#include "AnalyzerCore.h"

class WRTau_SR : public AnalyzerCore {

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

  WRTau_SR();
  ~WRTau_SR();

  double safeptcut;

  vector<TString> triggerlist;

  vector<TString> singlelepton_trigger;
  vector<TString> singletau_highpt_trigger;
  vector<TString> singletau_trigger;
  vector<TString> tau_met_trigger;
  vector<TString> etau_trigger;
  vector<TString> mutau_trigger;
  vector<TString> trigger;

};



#endif

