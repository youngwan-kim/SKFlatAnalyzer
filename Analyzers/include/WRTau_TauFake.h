#ifndef WRTau_TauFake_h
#define WRTau_TauFake_h

#include "WRTau_Core.h"

class WRTau_TauFake : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  map<WRTau_Core::SearchRegion,std::pair<bool,bool>> GetQCDFakeRegion(Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets,const std::vector<FatJet>& fatjets);
  map<WRTau_Core::SearchRegion,std::pair<bool,bool>> GetTTDYFakeRegion(Particle METv, const std::vector<Tau>& taus,  const std::vector<Lepton *> leptons, const std::vector<Jet>& bjets);
  void FillPassingFakeRegions(map<WRTau_Core::SearchRegion,std::pair<bool,bool>> m,TString fillpath,const std::vector<Tau>& taus,const std::vector<Gen>& gens,double MCweight, bool highpT);

  vector<TString> TriggerList;
  vector<TString> SingleLeptonTriggers;
  vector<TString> SingleElectronTriggers;
  vector<TString> SingleMuonTriggers;
  vector<TString> SingleTauTriggers;

  vector<Muon> AllMuons;
  vector<Tau> AllTaus;
  vector<Jet> AllJets;
  vector<Gen> AllGens;
  vector<FatJet> AllFatJets;
  vector<Electron> AllElectrons;

  WRTau_TauFake();
  ~WRTau_TauFake();

};



#endif

