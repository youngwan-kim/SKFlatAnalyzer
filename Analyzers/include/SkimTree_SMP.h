#ifndef SkimTree_SMP_h
#define SkimTree_SMP_h

#include "AnalyzerCore.h"

class SkimTree_SMP : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_SMP();
  ~SkimTree_SMP();

  TTree *newtree;

  vector<TString> triggers;
  void WriteHist();

};



#endif

