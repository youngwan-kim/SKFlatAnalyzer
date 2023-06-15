#include "FatJetTauTest.h"

void FatJetTauTest::initializeAnalyzer(){

}

void FatJetTauTest::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void FatJetTauTest::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

}

FatJetTauTest::FatJetTauTest(){

}

FatJetTauTest::~FatJetTauTest(){

}


