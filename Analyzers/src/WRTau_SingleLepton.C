#include "WRTau_SingleLepton.h"

void WRTau_SingleLepton::initializeAnalyzer(){

}

void WRTau_SingleLepton::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void WRTau_SingleLepton::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

}

WRTau_SingleLepton::WRTau_SingleLepton(){

}

WRTau_SingleLepton::~WRTau_SingleLepton(){

}


