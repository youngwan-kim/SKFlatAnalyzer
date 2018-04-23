#include "SKFlatValidation.h"

void SKFlatValidation::executeEvent(){

  if(muon_pt->size()==2){

    //cout << "[SKFlatValidation::executeEvent] " << muon_pt->size() << endl;
    TLorentzVector mu[2];
    for(int i=0;i<2;i++){
      mu[i].SetPtEtaPhiM(muon_pt->at(i), muon_eta->at(i), muon_phi->at(i), 0.1057);
    }

    FillHist("DiMuon_mass", (mu[0]+mu[1]).M(), 1., 500, 0., 500.);

  }


}
