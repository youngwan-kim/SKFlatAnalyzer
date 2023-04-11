#include "WRTau_SR_Orthogonal.h"

void WRTau_SR_Orthogonal::initializeAnalyzer(){

}

void WRTau_SR_Orthogonal::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void WRTau_SR_Orthogonal::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

}

WRTau_SR_Orthogonal::WRTau_SR_Orthogonal(){

}

WRTau_SR_Orthogonal::~WRTau_SR_Orthogonal(){

}


