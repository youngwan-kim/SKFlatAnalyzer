#ifndef TauStudy_h
#define TauStudy_h

#include "AnalyzerCore.h"

class TauStudy : public AnalyzerCore {

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

  vector<TString> lepton_triggers;

  TauStudy();
  ~TauStudy();

};



#endif

