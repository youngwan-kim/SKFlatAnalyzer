#ifndef WRTau_TauFake_h
#define WRTau_TauFake_h

#include "WRTau_Core.h"

class WRTau_TauFake : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  map<WRTau_Core::SearchRegion,bool> GetQCDFakeRegion(Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets,const std::vector<FatJet>& fatjets);
  void FillPassingFakeRegions(map<WRTau_Core::SearchRegion,bool> m,TString fillpath,Particle METv, const std::vector<Tau>& taus,
                                          double MCweight, std::tuple<int,int,int> idtuple, bool highpT);



  vector<TString> TriggerList;
  vector<TString> SingleLeptonTriggers;
  vector<TString> SingleElectronTriggers;
  vector<TString> SingleMuonTriggers;
  vector<TString> SingleTauTriggers;

  vector<Muon> AllMuons;
  vector<Tau> AllTaus;
  vector<Jet> AllJets;
  vector<FatJet> AllFatJets;

  WRTau_TauFake();
  ~WRTau_TauFake();

};



#endif

