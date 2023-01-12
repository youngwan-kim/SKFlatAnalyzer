#ifndef LepTauJJ_h
#define LepTauJJ_h

#include "AnalyzerCore.h"

class LepTauJJ : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<Muon> AllMuons;
  vector<Electron> AllElectrons;
  vector<Jet> AllJets;
  vector<Tau> AllTaus;

  LepTauJJ();
  ~LepTauJJ();

};



#endif

