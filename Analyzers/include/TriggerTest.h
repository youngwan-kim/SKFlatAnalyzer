#ifndef TriggerTest_h
#define TriggerTest_h

#include "WRTau_Core.h"

class TriggerTest : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<Muon> AllMuons;
  vector<Electron> AllElectrons;
  vector<Tau> AllTaus;
  vector<Lepton *> AllLeptons;
  vector<Lepton *> Leptons;
  vector<Lepton *> LeptonsVeto;
  vector<Jet> AllJets;
  vector<Jet> BJets;
  vector<FatJet> AllFatJets;
  vector<Gen> AllGens;
  vector<LHE> AllLHEs;

  vector<TString> TriggerList;
  vector<WRTau_Core::SearchRegion> RegionOfInterest;


  TriggerTest();
  ~TriggerTest();

};



#endif

