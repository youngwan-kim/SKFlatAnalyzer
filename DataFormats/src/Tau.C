#include "Tau.h"

ClassImp(Tau)

Tau::Tau(){

  j_IDBit = 0;
  j_decaymode=-1;
  j_idDecayModeNewDMs=false;

  this->SetLeptonFlavour(TAU);
}

Tau::~Tau(){

}

void Tau::SetIDBit(unsigned int idbit){
  j_IDBit = idbit;
}


void Tau::SetDecayMode(int decaymode){

  j_decaymode= decaymode;
}


void Tau::SetDecayModeNewDM(bool DecayModeNewDMs){

  j_idDecayModeNewDMs= DecayModeNewDMs;
}


   
bool Tau::PassID(TString ID) const{

  // === list of IDs for analyis
  if(ID=="NoCut") return true;

  if(ID=="EXO_16_023"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!( fabs(dXY())<0.05 )) return false;
      if(!(passLIDvJet() && passTIDvEl() && passLIDvMu())) return false;
      return true;
    }
  }

  if(ID=="Baseline"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      return true;
    }
  }

  if(ID=="LRSMLoose"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!(passVVVLIDvJet() && !passTIDvJet())) return false;
      if(!(passTIDvMu() && passTIDvEl())) return false;
      return true;
    }
  }

  if(ID=="BaselineWithvJetLoose"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!passLIDvJet()) return false;
      return true;
    }
  }

  if(ID=="BaselineWithvJetTight"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!passTIDvJet()) return false;
      return true;
    }
  }

  if(ID=="BaselineWithvJetTightvElLoose"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!(passTIDvJet() && passLIDvEl())) return false;
      return true;
    }
  }

  if(ID=="BaselineWithvJetTightvElTight"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!(passTIDvJet() && passTIDvEl())) return false;
      return true;
    }
  }

  if(ID=="BaselineWithvJetTightvElTightvMuLoose"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!(passTIDvJet() && passTIDvEl() && passLIDvMu())) return false;
      return true;
    }
  }

  if(ID=="BaselineWithvJetTightvElLoosevMuLoose"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!(passTIDvJet() && passLIDvEl() && passLIDvMu())) return false;
      return true;
    }
  }

  if(ID=="BaselineWithvJetTightvElLoosevMuTight"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!(passTIDvJet() && passLIDvEl() && passTIDvMu())) return false;
      return true;
    }
  }

  if(ID=="BaselineWithvJetTightvElTightvMuTight"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!(passTIDvJet() && passTIDvEl() && passTIDvMu())) return false;
      return true;
    }
  }

  if(ID=="HNVeto") {

    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11) {
      
      if (DecayModeNewDM() && passTIDvJet() && passTIDvEl()  && passTIDvMu() ) return true;
    }

  }

  if(ID=="TriLepLoose"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!( passVVVLIDvJet() && passLIDvEl() && passLIDvMu() )) return false;
      return true;
    }
  }

  if(ID=="TriLepTight"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!( passMIDvJet() && passLIDvEl() && passLIDvMu() )) return false;
      return true;
    }
  }

  if(ID=="WRTauTight"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 2 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!( passTIDvJet() && passTIDvEl() && passTIDvMu() )) return false;
      return true;
    }
  }

  
  if(ID=="FakeBase"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      //if(!( passTIDvEl() && passTIDvMu() )) return false;
      return true;
    }
  }

  if(ID=="TightFakeStudyID"){
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!( passLIDvJet() && passTIDvJet() && passTIDvEl() && passTIDvMu() )) return false;
      return true;
    }
  }

  if(ID=="LooseFakeStudyID"){ //loose but not tight
    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
      if(!DecayModeNewDM()) return false;
      if(!( fabs(dZ())<0.2 )) return false;
      if(!( passLIDvJet() && !passTIDvJet() && passTIDvEl() && passTIDvMu() )) return false;
      return true;
    }
  }
  
  return false;
}

/*
  DeepTau2017v2p1VSJet_VVVLoose = 1<< 0,0
  DeepTau2017v2p1VSJet_VVLoose = 1<< 1,2
  DeepTau2017v2p1VSJet_VLoose = 1<< 2,4
  DeepTau2017v2p1VSJet_Loose = 1<< 3,8
  DeepTau2017v2p1VSJet_Medium  = 1<< 4,16
  DeepTau2017v2p1VSJet_Tight = 1<< 5,32
  DeepTau2017v2p1VSJet_VTight = 1<< 6,64
  DeepTau2017v2p1VSJet_VVTight= 1<< 7,128

  DeepTau2017v2p1VSEl_VVVLoose = 1<< 8,
  DeepTau2017v2p1VSEl_VVLoose = 1<< 9,
  DeepTau2017v2p1VSEl_VLoose = 1<< 10,
  DeepTau2017v2p1VSEl_Loose = 1<< 11,
  DeepTau2017v2p1VSEl_Medium  = 1<< 12,
  DeepTau2017v2p1VSEl_Tight = 1<< 13,
  DeepTau2017v2p1VSEl_VTight = 1<< 14,
  DeepTau2017v2p1VSEl_VVTight= 1<< 15,
  
  DeepTau2017v2p1VSMu_VVVLoose = 1<< 16,
  DeepTau2017v2p1VSMu_VVLoose = 1<< 17,
  DeepTau2017v2p1VSMu_VLoose = 1<< 18,
  DeepTau2017v2p1VSMu_Loose = 1<< 19,
  DeepTau2017v2p1VSMu_Medium  = 1<< 20,
  DeepTau2017v2p1VSMu_Tight = 1<< 21,
  DeepTau2017v2p1VSMu_VTight = 1<< 22,
  DeepTau2017v2p1VSMu_VVTight= 1<< 23,

*/

bool Tau::PassWRID(int vJet) const{

  if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 2 || j_decaymode == 10 || j_decaymode ==11){
    if(!DecayModeNewDM()) return false;
    if(!( fabs(dZ())<0.2 )) return false;
    if(!(passTIDvEl() && passTIDvMu() )) return false;
    if(!PassSelector(1<<vJet)) return false;
    return true;
  }

  else return false;

}

// implement also not passing certain WP or several WP
// i.e if(!ignoreVSel) passselector for vElectron

bool Tau::PassID_varWP(int vJet, int vEl, int vMu) const{

  if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11){
    if(!DecayModeNewDM()) return false;
    if(!( fabs(dZ())<0.2 )) return false;
    /*if(!PassSelector(1<<vJet)) return false;
    if(!(PassSelector(1<<vEl))) return false;
    if(!(PassSelector(1<<vMu))) return false;*/
    if(!(PassSelector(1<<vJet) && PassSelector(1<<vEl) && PassSelector(1<<vMu))) return false;
    return true;
  }

  else return false;

}