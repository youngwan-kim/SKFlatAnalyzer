#ifndef LeptonicTauStudy_h
#define LeptonicTauStudy_h

#include "AnalyzerCore.h"

class LeptonicTauStudy : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<Muon> AllMuons;
  vector<Electron> AllElectrons;
  vector<Gen> AllGens;

  LeptonicTauStudy();
  ~LeptonicTauStudy();

};



#endif

