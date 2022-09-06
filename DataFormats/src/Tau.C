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

  if(ID=="HNVeto") {

    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11) {
      
      if (DecayModeNewDM() && passTIDvJet() && passTIDvEl()  && passTIDvMu() ) return true;
    }

  }

  if(ID=="TriLepLoose"){
    if(!DecayModeNewDM()) return false;
    if(!( fabs(dZ())<0.2 )) return false;
    if(!( passVVVLIDvJet() && passLIDvEl() && passLIDvMu() )) return false;
    return true;
  }

  if(ID=="TriLepTight"){
    if(!DecayModeNewDM()) return false;
    if(!( fabs(dZ())<0.2 )) return false;
    if(!( passMIDvJet() && passLIDvEl() && passLIDvMu() )) return false;
    return true;
  }
  
  return false;
}
