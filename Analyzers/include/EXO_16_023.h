#ifndef EXO_16_023_h
#define EXO_16_023_h

#include "AnalyzerCore.h"

class EXO_16_023 : public AnalyzerCore {

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
  vector<TString> TriggerList;

  EXO_16_023();
  ~EXO_16_023();

};



#endif

