#ifndef CalcCFRate_h
#define CalcCFRate_h

#include "AnalyzerCore.C"

class CalcCFRate : public AnalyzerCore {

public:

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  void FillCFRatePlots(TString name, Lepton *lep, Gen genlep, bool IsCF, double weight);

};



#endif

