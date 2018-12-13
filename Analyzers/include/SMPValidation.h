#ifndef SMPValidation_h
#define SMPValidation_h

#include "AnalyzerCore.h"

class SMPValidation : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(TString channelname,Event* ev,TString trigger);
  void executeEvent();
  void FillBasicHists(TString prefix,TString suffix,const vector<Lepton*>& leps,double weight);
  void FillSystematicHists(TString prefix,TString suffix,const vector<Lepton*>& leps,map<TString,double> map_systematic);
  double DileptonTrigger_SF(TString SFhistkey0,TString SFhistkey1,const vector<Lepton*>& leps,int sys);
 
  SMPValidation();
  ~SMPValidation();

};



#endif

