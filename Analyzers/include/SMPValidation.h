#ifndef SMPValidation_h
#define SMPValidation_h

#include "AnalyzerCore.h"

class SMPValidation : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(TString channelname,Event* ev);
  void executeEvent();
  void FillBasicHists(TString prefix,TString suffix,const vector<Lepton*>& leps,double weight);
  void FillSystematicHists(TString prefix,TString suffix,const vector<Lepton*>& leps,map<TString,double> map_systematic);
  double GetZPtWeight(double zpt,double zrap,Lepton::Flavour flavour);
  double DileptonTrigger_SF(TString SFhistkey0,TString SFhistkey1,const vector<Lepton*>& leps,int sys);
  void PrintGens(const vector<Gen>& gens);

  const double zptcor_ptbinning[47]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,24,26,28,30,32,34,36,38,40,44,48,52,56,60,70,80,90,100,120,140,160,180,200,250,400};
  const double zptcor_ybinning[4]={0,0.5,1,3};

  TH2D *hzpt_muon,*hzpt_electron;

  SMPValidation();
  ~SMPValidation();

};



#endif

