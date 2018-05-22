#include "TString.h"

class AnalyzerParameter{

public:

  TString Electron_Tight_ID, Electron_Loose_ID, Electron_Veto_ID;
  TString Electron_ID_SF_Key;

  TString Muon_Tight_ID, Muon_Loose_ID, Muon_Veto_ID;
  TString Muon_ID_SF_Key, Muon_ISO_SF_Key, Muon_Trigger_SF_Key;
  TString Jet_ID;

  AnalyzerParameter();
  ~AnalyzerParameter();
};


AnalyzerParameter::AnalyzerParameter(){

  Electron_Tight_ID = "passTightID";
  Electron_Loose_ID = "passLooseID";
  Electron_Veto_ID = "passVetoID";
  Electron_ID_SF_Key = "passTightID";

  Muon_Tight_ID = "POGTightWithTightIso";
  Muon_Loose_ID = "POGLoose";
  Muon_Veto_ID = "POGLoose";
  Muon_ID_SF_Key = "NUM_TightID_DEN_genTracks";
  Muon_ISO_SF_Key = "NUM_TightRelIso_DEN_TightIDandIPCut";
  Muon_Trigger_SF_Key = "POGTight";

  Jet_ID = "HN";

}

AnalyzerParameter::~AnalyzerParameter(){
}
