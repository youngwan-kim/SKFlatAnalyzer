#ifndef WRTau_Analyzer_h
#define WRTau_Analyzer_h

#include "WRTau_Core.h"

class WRTau_Analyzer : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  bool runSystematics = false;
  bool runTauFake = false;
  vector<AnalyzerParameter::Syst> skipSysts = { 
                                                AnalyzerParameter::JetMassSmearUp, AnalyzerParameter::JetMassSmearDown,
                                                AnalyzerParameter::JetResUp, AnalyzerParameter::JetResDown,
                                                AnalyzerParameter::JetMassUp, AnalyzerParameter::JetMassDown,
                                                AnalyzerParameter::BTagSFHTagUp, AnalyzerParameter::BTagSFHTagDown,
                                                AnalyzerParameter::BTagSFLTagUp, AnalyzerParameter::BTagSFLTagDown,
                                                AnalyzerParameter::JetPUIDUp , AnalyzerParameter::JetPUIDDown,
                                                AnalyzerParameter::MuonRecoSFUp, AnalyzerParameter::MuonRecoSFDown,
                                                AnalyzerParameter::MuonEnUp, AnalyzerParameter::MuonEnDown,
                                                AnalyzerParameter::MuonIDSFUp, AnalyzerParameter::MuonIDSFDown,
                                                AnalyzerParameter::FRUp , AnalyzerParameter::FRDown,
                                                AnalyzerParameter::JetMassSmearUp,AnalyzerParameter::JetMassSmearDown,
                                                
                                              };

  vector<AnalyzerParameter::Syst> whiteSysts = {
                                                AnalyzerParameter::JetEnUp, AnalyzerParameter::JetEnDown,
                                                AnalyzerParameter::JetResUp, AnalyzerParameter::JetResDown,
                                                AnalyzerParameter::JetMassUp, AnalyzerParameter::JetMassDown,
                                                
                                                AnalyzerParameter::MuonEnUp, AnalyzerParameter::MuonEnDown,
                                                AnalyzerParameter::MuonIDSFUp, AnalyzerParameter::MuonIDSFDown,
                                                AnalyzerParameter::MuonISOSFUp, AnalyzerParameter::MuonISOSFDown,         

                                                AnalyzerParameter::ElectronEnUp, AnalyzerParameter::ElectronEnDown,       //
                                                AnalyzerParameter::ElectronIDSFUp, AnalyzerParameter::ElectronIDSFDown,   //
                                                AnalyzerParameter::ElectronResUp, AnalyzerParameter::ElectronResDown,     // ? 
                                                
                                                AnalyzerParameter::TauIDSFSystUp, AnalyzerParameter::TauIDSFSystDown,     //
                                                AnalyzerParameter::TauIDSFStatUp, AnalyzerParameter::TauIDSFStatDown,     //
                                                AnalyzerParameter::TauIDSFExtUp, AnalyzerParameter::TauIDSFExtDown,       //
                                                AnalyzerParameter::TauTriggerSFUp, AnalyzerParameter::TauTriggerSFDown,   //
                                                AnalyzerParameter::TauEnUp,AnalyzerParameter::TauEnDown,                  //

                                                AnalyzerParameter::METUnclUp, AnalyzerParameter::METUnclDown,             //
                                                AnalyzerParameter::PrefireUp, AnalyzerParameter::PrefireDown,             //
                                                AnalyzerParameter::PUUp, AnalyzerParameter::PUDown                        //
  };

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

  double MuIsoCut = -999.;

  TString muon_IsoSF_loose_key = "";
  TString muon_IsoSF_tight_key = "";

  TString electron_looseID_SF_Key = "passLooseID";
  TString muon_recoSF_key = "Default";

  vector<TString> TriggerList;
  vector<TString> SingleLeptonTriggers;
  vector<TString> SingleElectronTriggers;
  vector<TString> SingleMuonTriggers;
  vector<TString> SingleTauTriggers;

  vector<WRTau_Core::SearchRegion> RegionOfInterest;

  vector<TString> trigger;

  WRTau_Analyzer();
  ~WRTau_Analyzer();

};



#endif

