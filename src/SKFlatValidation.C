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

      Particle Z = mu[0]+mu[1];

      FillHist("MET_DiMuon_OS", METv.Pt(), 1., 500, 0., 500.);
      FillHist("METphi_DiMuon_OS", METv.Phi(), 1., 60, -3., 3.);

      FillHist("Lepton_0_Pt_DiMuon_OS", mu[0].Pt(), 1., 500, 0., 500.);
      FillHist("Lepton_0_Eta_DiMuon_OS", mu[0].Eta(), 1., 60, -3., 3.);
      FillHist("Lepton_0_RelIso_DiMuon_OS", mu[0].RelIso(), 1., 100, 0., 1.);

      FillHist("Lepton_1_Pt_DiMuon_OS", mu[1].Pt(), 1., 500, 0., 500.);
      FillHist("Lepton_1_Eta_DiMuon_OS", mu[1].Eta(), 1., 60, -3., 3.);
      FillHist("Lepton_1_RelIso_DiMuon_OS", mu[1].RelIso(), 1., 100, 0., 1.);

      FillHist("Z_Mass_DiMuon_OS", Z.M(), 1., 500, 0., 500.);
      FillHist("Z_Pt_DiMuon_OS", Z.Pt(), 1., 500, 0., 500.);
      FillHist("Z_Eta_DiMuon_OS", Z.Eta(), 1., 60, -3., 3.);


    }
  }

}
