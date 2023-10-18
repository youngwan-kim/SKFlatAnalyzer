#ifndef SkimTree_LRSMTau_h
#define SkimTree_LRSMTau_h

#include "AnalyzerCore.h"

class SkimTree_LRSMTau : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_LRSMTau();
  ~SkimTree_LRSMTau();

  TTree *newtree;

  vector<TString> triggers;
  void WriteHist();

  double LeptonPtCut, AK4JetPtCut, AK8JetPtCut;

};



#endif

