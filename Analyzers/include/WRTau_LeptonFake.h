#ifndef WRTau_LeptonFake_h
#define WRTau_LeptonFake_h

#include "WRTau_Core.h"

class WRTau_LeptonFake : public WRTau_Core {

public:

  tuple<int,int,int> tauid_LTT;
  tuple<int,int,int> tauid_TTT;

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  void FillLeptonKinematics(map<WRTau_Core::SearchRegion,bool> map_regions, AnalyzerParameter param,
                                            Particle METv, const std::vector<Gen>& gens,const std::vector<Tau>& taus, 
                                            const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                                            const std::vector<Electron>& electrons, const std::vector<Electron>& electrons_loose, const std::vector<Electron>& electrons_veto,
                                            const std::vector<Muon>& muons, const std::vector<Muon>& muons_loose, const std::vector<Muon>& muons_veto,
                                            const std::vector<Jet>& jets, const std::vector<Jet>& bjets, const std::vector<FatJet>& fatjets, TString fillpath, double weight);

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

  WRTau_LeptonFake();
  ~WRTau_LeptonFake();

};



#endif

