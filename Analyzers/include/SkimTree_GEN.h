#ifndef SkimTree_GEN_h
#define SkimTree_GEN_h

#include "AnalyzerCore.h"

class SkimTree_GEN : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_GEN();
  ~SkimTree_GEN();

  TTree *newtree;

  void WriteHist();

};



#endif

