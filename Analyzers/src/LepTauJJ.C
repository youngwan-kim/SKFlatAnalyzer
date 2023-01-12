#include "LepTauJJ.h"

void LepTauJJ::initializeAnalyzer(){

}

void LepTauJJ::executeEvent(){

  AllMuons = GetAllMuons();
  AllJets = GetAllJets();

  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void LepTauJJ::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

}

LepTauJJ::LepTauJJ(){

}

LepTauJJ::~LepTauJJ(){

}


