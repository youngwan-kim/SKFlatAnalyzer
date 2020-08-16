#ifndef ttbar_test_h
#define ttbar_test_h

#include "AnalyzerCore.h"

class ttbar_test : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<Electron> AllElectrons;
  vector<Muon> AllMuons;
  vector<Jet> AllJets;
  vector<Gen> Gens;

  vector<TString> MuonIDs, MuonIDSFKeys, MuonTriggers;
//  vector<TString> ElectronIDs 
 
  TString TriggerSafePtCut;

  double weight_Prefire;
  
  ttbar_test();
  ~ttbar_test();

};



#endif

