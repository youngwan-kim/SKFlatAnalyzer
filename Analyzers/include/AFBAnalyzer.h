#ifndef AFBAnalyzer_h
#define AFBAnalyzer_h

#include "AnalyzerCore.h"
#include "SkimTree_AFB.h"
class AFBAnalyzer : public SkimTree_AFB {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(TString channelname,Event* ev);
  void executeEvent();

  AFBAnalyzer();
  ~AFBAnalyzer();

};



#endif

