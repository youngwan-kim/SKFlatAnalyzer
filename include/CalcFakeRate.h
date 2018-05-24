#ifndef CalcFakeRate_h
#define CalcFakeRate_h

#include "AnalyzerCore.C"

class CalcFakeRate : public AnalyzerCore {

public:

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  void FillFakeRatePlots(TString name, Lepton *lep, bool IsTight, double weight);

};



#endif

