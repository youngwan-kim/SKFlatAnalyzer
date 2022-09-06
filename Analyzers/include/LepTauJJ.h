#ifndef LepTauJJ_h
#define LepTauJJ_h

#include "AnalyzerCore.h"

class LepTauJJ : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();


  // Objects
  vector<Muon> AllMuons;
  vector<Electron> AllElectrons;
  vector<Tau> AllTaus;
  vector<Lepton *> Leptons;
  vector<Lepton *> LeptonsVeto;
  vector<Jet> AllJets;
  vector<Jet> BJets;

  LepTauJJ();
  ~LepTauJJ();

};



#endif

