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

  static const int massbinnum=12;
  const double massrange[massbinnum+1]={60,70,78,84,87,89,91,93,95,98,104,112,120};
};



#endif

