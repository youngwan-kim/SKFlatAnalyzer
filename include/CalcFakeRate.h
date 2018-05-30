#ifndef CalcFakeRate_h
#define CalcFakeRate_h

#include "AnalyzerCore.C"

class TriggerWithPtRange{

public:

  vector<double> PtValues;
  vector<TString> Triggers;
  vector<double> TriggerSafePtCuts;

  void Validate(){

    if( PtValues.size() != Triggers.size()+1 ){
      cout << "[TriggerWithPtRange::Validate()] PtValues.size() != Triggers.size()+1" << endl;
      exit(EXIT_FAILURE);
    }

    double prev_pt = PtValues.at(0);
    bool PtOrderingOkay = true;
    for(unsigned int i=1; i<PtValues.size(); i++){
      if(!( prev_pt < PtValues.at(i) )){
        PtOrderingOkay = false;
        break;
      }
    }

    if(!PtOrderingOkay){
      cout << "[TriggerWithPtRange::Validate()] PtValues = {";
      for(unsigned int i=0; i<PtValues.size(); i++) cout << PtValues.at(i) << ", ";
      cout << "} --> Pt binning failed" << endl;
      exit(EXIT_FAILURE);
    }
  }

  TString GetTriggerFromPt(double pt){
    TString out = "NULL";
    if(pt < PtValues.at(0)) return "PTFAIL"; // min pt is important
    if(pt >= PtValues.at( PtValues.size()-1 )) pt = PtValues.at( PtValues.size()-1 )-0.1; //max pt is not important..
    for(unsigned int i=0; i<Triggers.size(); i++){
      if( PtValues.at(i) < pt && pt <= PtValues.at(i+1) ){
        return Triggers.at(i);
      }
    }

    cout << "[TriggerWithPtRange::GetTriggerFromPt] input pt = " << pt << endl;
    cout << "[TriggerWithPtRange::GetTriggerFromPt] not in the ptrage : " << endl;
    cout << "[TriggerWithPtRange::GetTriggerFromPt] PtValues = {";
    for(unsigned int i=0; i<PtValues.size(); i++) cout << PtValues.at(i) << ", ";
    cout << "} --> Pt binning failed" << endl;
    exit(EXIT_FAILURE);

    return out;

  }

  TriggerWithPtRange(){}
  ~TriggerWithPtRange(){}

};

class CalcFakeRate : public AnalyzerCore {

public:

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  void FillFakeRatePlots(TString name, TString frtype, Lepton *lep, bool IsTight, double weight);

  TriggerWithPtRange Muon_TrigWithPtRange, Electron_TrigWithPtRange;


};

#endif

