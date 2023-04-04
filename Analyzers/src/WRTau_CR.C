#include "WRTau_CR.h"

void WRTau_CR::initializeAnalyzer(){

}

void WRTau_CR::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void WRTau_CR::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

}

WRTau_CR::WRTau_CR(){

}

WRTau_CR::~WRTau_CR(){

}


