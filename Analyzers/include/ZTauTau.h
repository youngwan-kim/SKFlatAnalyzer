#ifndef ZTauTau_h
#define ZTauTau_h

#include "WRTau_Core.h"

class ZTauTau : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<Tau> AllTaus;
  vector<Electron> AllElectrons;
  vector<Muon> AllMuons;

  vector<TString> Triggers;

  ZTauTau();
  ~ZTauTau();

  TauIDSFTool *tauIDSFtool = NULL;

  std::string DeepTauVSjet = "DeepTau2017v2p1VSjet";
  std::string DeepTauVSe = "DeepTau2017v2p1VSe";
  std::string DeepTauVSmu = "DeepTau2017v2p1VSmu";

};



#endif

