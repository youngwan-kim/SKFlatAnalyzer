#ifndef TauQCDFakeStudy_h
#define TauQCDFakeStudy_h

#include "WRTau_Core.h"

class TauQCDFakeStudy : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<Muon> AllMuons;
  vector<Electron> AllElectrons;
  vector<Tau> AllTaus;
  vector<Lepton *> Leptons;
  vector<Gen> AllGens;

  TauIDSFTool *tauIDSFtool = NULL;
  std::string DeepTauVSjet = "DeepTau2017v2p1VSjet";
  std::string DeepTauVSe = "DeepTau2017v2p1VSe";
  std::string DeepTauVSmu = "DeepTau2017v2p1VSmu";


  vector<TString> TriggerList;
  vector<TString> SingleElectronTriggers;
  vector<TString> SingleMuonTriggers;
  vector<TString> SingleTauTriggers;

  TauQCDFakeStudy();
  ~TauQCDFakeStudy();

};



#endif

