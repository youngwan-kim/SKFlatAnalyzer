#ifndef HNL_TriLep_h
#define HNL_TriLep_h

#include "AnalyzerCore.h"

class HNL_TriLep : public AnalyzerCore {

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

  vector<TString> trilepton_triggers;
  vector<TString> dilepton_triggers;
  vector<TString> lepton_triggers;

  HNL_TriLep();
  ~HNL_TriLep();

};



#endif

