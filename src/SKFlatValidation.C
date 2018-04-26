#include "SKFlatValidation.h"

void SKFlatValidation::executeEvent(){

  Event ev = GetEvent();

  bool PassDiMuon = ev.PassTrigger("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v");
  bool PassDiElectron = ev.PassTrigger("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");

  std::vector<Muon> muons = GetMuons("TEST", 10., 2.4);
  std::vector<Electron> electrons = GetElectrons("passMVAID_iso_WP80", 10., 2.5);
 
  std::map<TString, bool> map_bool_To_Region;
  map_bool_To_Region["DiMuon"] = PassDiMuon && (muons.size()==2);
  map_bool_To_Region["DiElectron"] = PassDiElectron && (electrons.size()==2);

  Particle METv = ev.GetMETVector();

  double weight = 1.;
  if(!IsThisDataFile){
    weight *= weight_norm_1invfb*ev.GetTriggerLumi("Full");
  }

  for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

/*
    if(this->DataStream == "DoubleMuon"){
      if(!( this_region.Contains("Muon") )) continue;
    }
    if(this->DataStream == "DoubleEG"){
      if(!( this_region.Contains("Electron") )) continue;
    }
*/

    TString this_region = it_map->first;
    if(it_map->second){

      Lepton lep[2];
      if(this_region.Contains("Muon")){
        lep[0] = muons.at(0);
        lep[1] = muons.at(1);
      }
      if(this_region.Contains("Electron")){
        lep[0] = electrons.at(0);
        lep[1] = electrons.at(1);
      }

      if(lep[0].Charge()!=lep[1].Charge()){

        Particle Z = lep[0]+lep[1];

        FillHist("MET_"+this_region, METv.Pt(), weight, 500, 0., 500.);
        FillHist("METphi_"+this_region, METv.Phi(), weight, 60, -3., 3.);

        FillHist("Lepton_0_Pt_"+this_region, lep[0].Pt(), weight, 500, 0., 500.);
        FillHist("Lepton_0_Eta_"+this_region, lep[0].Eta(), weight, 60, -3., 3.);
        FillHist("Lepton_0_RelIso_"+this_region, lep[0].RelIso(), weight, 100, 0., 1.);

        FillHist("Lepton_1_Pt_"+this_region, lep[1].Pt(), weight, 500, 0., 500.);
        FillHist("Lepton_1_Eta_"+this_region, lep[1].Eta(), weight, 60, -3., 3.);
        FillHist("Lepton_1_RelIso_"+this_region, lep[1].RelIso(), weight, 100, 0., 1.);

        FillHist("Z_Mass_"+this_region, Z.M(), weight, 500, 0., 500.);
        FillHist("Z_Pt_"+this_region, Z.Pt(), weight, 500, 0., 500.);
        FillHist("Z_Eta_"+this_region, Z.Eta(), weight, 60, -3., 3.);


      }
    }

  }

}
