#include "SKFlatValidation.h"

void SKFlatValidation::executeEvent(){

  Event ev = GetEvent();
  if(!ev.PassTrigger("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v")) return;

  std::vector<Muon> muons = GetMuons("TEST",10.,2.4);
  //cout << muons.size() << endl;
 
  if(muons.size()==2){
    Muon mu[2] = {muons.at(0), muons.at(1)};
    if(mu[0].Charge()!=mu[1].Charge()){

      Particle METv = ev.GetMETVector();

      FillHist("DiMuon_mass_DiMuon_OS", (mu[0]+mu[1]).M(), 1., 500, 0., 500.);
      FillHist("MET_DiMuon_OS", METv.Pt(), 1., 500, 0., 500.);
      FillHist("METphi_DiMuon_OS", METv.Phi(), 1., 60, -3., 3.);
    }
  }

}
