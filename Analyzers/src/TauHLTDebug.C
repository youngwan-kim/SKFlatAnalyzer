#include "TauHLTDebug.h"

void TauHLTDebug::initializeAnalyzer(){

}

void TauHLTDebug::executeEvent(){

  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void TauHLTDebug::executeEventFromParameter(AnalyzerParameter param){

  Event ev;
  /*cout << "----" << endl;
  vector<string> passingtriggers = ev.GetHLTName();
  cout << passingtriggers.size() << endl;
  for(unsigned int i=0; i<passingtriggers.size(); i++){
    cout << passingtriggers.at(i) << endl;
  } */

  bool pass2016trigger = ev.PassTrigger("HLT_VLooseIsoPFTau120_Trk50_eta2p1_v"); 
  bool testtrigger = ev.PassTrigger("HLT_IsoMu24_v");
  cout << "pass2016trigger = " << pass2016trigger << endl;
  cout << "testtrigger = " << testtrigger << endl;

}

TauHLTDebug::TauHLTDebug(){

}

TauHLTDebug::~TauHLTDebug(){

}


