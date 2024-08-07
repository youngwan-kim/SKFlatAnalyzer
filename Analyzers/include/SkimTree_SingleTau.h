#ifndef SkimTree_SingleTau_h
#define SkimTree_SingleTau_h

#include "AnalyzerCore.h"

class SkimTree_SingleTau : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_SingleTau();
  ~SkimTree_SingleTau();

  TTree *newtree;

  vector<TString> triggers;
  void WriteHist();

  double LeptonPtCut, AK4JetPtCut, AK8JetPtCut;
  double TriggerSafePtCut;
};



#endif

