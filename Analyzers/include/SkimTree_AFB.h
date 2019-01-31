#ifndef SkimTree_AFB_h
#define SkimTree_AFB_h

#include "AnalyzerCore.h"
#include "SMPValidation.h"

class SkimTree_AFB : public SMPValidation {

public:

  virtual void initializeAnalyzer();
  virtual void executeEvent();

  SkimTree_AFB();
  ~SkimTree_AFB();

  TTree *newtree;
  bool isTauTau;
  double genZ_Px,genZ_Py,genZ_Pz,genZ_E;
  int genl0_PID;
  double genl0_Px,genl0_Py,genl0_Pz,genl0_E;
  int genl1_PID;
  double genl1_Px,genl1_Py,genl1_Pz,genl1_E;

  vector<TString> triggers;
  void WriteHist();
  void SetupSkimTree_AFB();
};



#endif

