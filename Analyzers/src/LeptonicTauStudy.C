#include "LeptonicTauStudy.h"

void LeptonicTauStudy::initializeAnalyzer(){

}

void LeptonicTauStudy::executeEvent(){

  AnalyzerParameter param;

  param.Name = "LeptonicTauIDStudy";
  param.Electron_Tight_ID = "passTightID";
  param.Muon_Tight_ID = "POGTight";
  param.Electron_Veto_ID = "passVetoID";
  param.Muon_Veto_ID = "POGLoose";
  param.Jet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllGens = GetGens();

  executeEventFromParameter(param);

}

void LeptonicTauStudy::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

  vector<Lepton*> LeptonicTauDecayLeptons;
  vector<Lepton*> PromptLeptons;
  vector<Lepton*> AllLeptons = CombineLeptonPointerVector(AllElectrons,AllMuons);
  

  for(unsigned int i=0;i<AllLeptons.size();i++){
    if(GetLeptonType(*AllLeptons.at(i),AllGens) == 3){
      FillHist("LeptonicTauDecayLeptons/dXY",AllLeptons.at(i)->dXY(),1.,2000,-0.5,0.5);
      FillHist("LeptonicTauDecayLeptons/dZ",AllLeptons.at(i)->dZ(),1.,2000,-0.5,0.5);
      FillHist("LeptonicTauDecayLeptons/RelIso",AllLeptons.at(i)->RelIso(),1.,2000,0.,1.);
      FillHist("LeptonicTauDecayLeptons/IP3D",AllLeptons.at(i)->IP3D(),1.,2000,0,0.5);
    } 
    else if(GetLeptonType(*AllLeptons.at(i),AllGens) == 1){
      FillHist("PromptLeptons/dXY",AllLeptons.at(i)->dXY(),1.,2000,-.5,.5);
      FillHist("PromptLeptons/dZ",AllLeptons.at(i)->dZ(),1.,2000,-.5,.5);
      FillHist("PromptLeptons/RelIso",AllLeptons.at(i)->RelIso(),1.,2000,0.,1.);
      FillHist("PromptLeptons/IP3D",AllLeptons.at(i)->IP3D(),1.,2000,0,.5);
    }
  }



  // GetLeptonType = 3 : EWTau

}

LeptonicTauStudy::LeptonicTauStudy(){

}

LeptonicTauStudy::~LeptonicTauStudy(){

}


