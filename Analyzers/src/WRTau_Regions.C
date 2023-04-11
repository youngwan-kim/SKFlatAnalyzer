#include "WRTau_Regions.h"

void WRTau_Regions::initializeAnalyzer(){

}

void WRTau_Regions::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void WRTau_Regions::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

}

WRTau_Regions::WRTau_Regions(){

}

WRTau_Regions::~WRTau_Regions(){

}


