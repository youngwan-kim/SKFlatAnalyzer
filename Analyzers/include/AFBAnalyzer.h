#ifndef AFBAnalyzer_h
#define AFBAnalyzer_h

#include "SMPValidation.h"

class AFBAnalyzer : public SMPValidation {

public:

  void executeEventFromParameter(TString channelname,Event* ev);

  AFBAnalyzer();
  ~AFBAnalyzer();

  double GetCosThetaCS(const vector<Lepton*>& leps);
  void FillAFBHists(TString pre,TString suf,const vector<Lepton*>& leps,double w);
  void FillAFBSystematicHists(TString pre,TString suf,const vector<Lepton*>& leps,map<TString,double> map_systematic);
};



#endif

