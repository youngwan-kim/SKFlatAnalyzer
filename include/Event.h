#ifndef Event_h
#define Event_h

#include "Particle.h"

class Event{
public:

  Event();
  ~Event();

  void SetMCweight(double genw);
  inline double MCweight() const { return j_MCweight; }

  inline void SetnPV(double n){ j_nPV = n; }
  inline int nPV() const { return j_nPV; }

  void SetTrigger(vector<string> HLT_TriggerName, vector<bool> HLT_TriggerFired);
  bool PassTrigger(std::string trig);
  bool PassTrigger(std::vector<std::string> trigs);
  double GetTriggerLumi(TString trig);

  void SetMET(double pt, double px, double py);
  inline Particle GetMETVector() const {return j_METVector;}

private:
  double j_MCweight;
  int j_nPV;
  vector<string> j_HLT_TriggerName;
  vector<bool> j_HLT_TriggerFired;
  Particle j_METVector;

};

#include "Event.h"

Event::Event(){
  j_MCweight = 1.;
  j_nPV = -999;
}

Event::~Event(){

}

void Event::SetMCweight(double genw){
  if(genw>0) j_MCweight=1.;
  else j_MCweight=-1.;
}

void Event::SetTrigger(vector<string> HLT_TriggerName, vector<bool> HLT_TriggerFired){
  j_HLT_TriggerName = HLT_TriggerName;
  j_HLT_TriggerFired = HLT_TriggerFired;
}

bool Event::PassTrigger(std::string trig){
  std::vector<std::string> tmp_vec;
  tmp_vec.push_back(trig);
  return PassTrigger(tmp_vec);
}

bool Event::PassTrigger(std::vector<std::string> trigs){

  for(unsigned int i=0; i<trigs.size(); i++){
    std::string this_check_trig = trigs.at(i);
    bool this_pass=false;

    //cout << this_check_trig << endl;

    for(unsigned int j=0; j<j_HLT_TriggerName.size(); j++){

      //cout << j_HLT_TriggerName.at(j) << endl;

      if( j_HLT_TriggerName.at(j).find(this_check_trig)!=std::string::npos ){
        this_pass = j_HLT_TriggerFired.at(j);
        break;
      }
    }
    if(this_pass) return true;

  }
  return false;
}

double Event::GetTriggerLumi(TString trig){

  if(trig=="Full"){
    return 41527.540;
  }

  return 41527.540;

}

void Event::SetMET(double pt, double px, double py){
  j_METVector.SetPxPyPzE(px,py,0,pt);
}


#endif
