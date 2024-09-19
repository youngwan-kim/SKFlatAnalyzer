#include "TriggerTest.h"

void TriggerTest::initializeAnalyzer(){

}

void TriggerTest::executeEvent(){

AnalyzerParameter param;

  param.Clear();
  TriggerList.clear();
  TriggerList = {"HLT_Mu50_v"};
  param.Name = "Mu50";
  param.Electron_Tight_ID = "LRSMTight";
  if(DataYear==2018) param.Electron_Tight_ID = "LRSMTight2018";

  param.Electron_Loose_ID = "LRSMLoose";
  param.Electron_Veto_ID = "LRSMVeto";
  param.Electron_ID_SF_Key = "HEEP";

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPt";
  param.Muon_Veto_ID = "POGLoose";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_TrackerMuons";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";

  param.Jet_ID = "tightLepVeto";
  param.FatJet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  AllFatJets = GetAllFatJets();
  AllGens = GetGens();
  AllLHEs = GetLHEs();

  executeEventFromParameter(param);
  TriggerList.clear()


}

void TriggerTest::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

}

TriggerTest::TriggerTest(){

}

TriggerTest::~TriggerTest(){

}


