#ifndef WMassChargeAsymmetry_h
#define WMassChargeAsymmetry_h

#include "AnalyzerCore.h"

class WMassChargeAsymmetry : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<TString> Triggers;
  double TriggerSafePtCut;

  vector<Muon> AllMuons;
  double weight_Prefire;

  WMassChargeAsymmetry();
  ~WMassChargeAsymmetry();

};



#endif

