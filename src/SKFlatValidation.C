#include "SKFlatValidation.h"

void SKFlatValidation::executeEvent(){

  Event ev = GetEvent();

  bool PassDiMuon = ev.PassTrigger("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v");
  bool PassDiElectron = ev.PassTrigger("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");

  std::vector<Muon> muons = GetMuons("TEST", 10., 2.4);
  std::vector<Electron> electrons = GetElectrons("passMVAID_iso_WP80", 10., 2.5);

  std::vector<Jet> alljets = GetAllJets();
  std::vector<Jet> myjets;
  int NBJets=0;
  for(unsigned int i=0; i<alljets.size(); i++){
    Jet this_jet = alljets.at(i);
    if(this_jet.Pt() > 30. && fabs(this_jet.Eta())<2.5){
      myjets.push_back(this_jet);
      if(this_jet.IsTagged(Jet::CSVv2, Jet::Medium)) NBJets++;
    }
  }

  std::vector< TString > Suffixs = {
    "DiMuon",
    "DiElectron",
  };
  std::vector< bool > PassTriggers = {
    PassDiMuon && (muons.size()==2),
    PassDiElectron && (electrons.size()==2),
  };

  for(unsigned int i=0; i<Suffixs.size(); i++){

    TString Suffix = Suffixs.at(i);
    if( !PassTriggers.at(i) ) continue;

    if(this->DataStream == "DoubleMuon"){
      if(!( Suffix.Contains("Muon") )) continue;
    }
    if(this->DataStream == "DoubleEG"){
      if(!( Suffix.Contains("Electron") )) continue;
    }

    std::vector<Lepton *> lep;
    if(Suffix.Contains("Muon")){
      lep = MakeLeptonPointerVector(muons);
    }
    if(Suffix.Contains("Electron")){
      lep = MakeLeptonPointerVector(electrons);
    }

    bool IsOS = (lep[0]->Charge() != lep[1]->Charge());
    Particle Z = (*lep[0])+(*lep[1]);

    Particle METv = ev.GetMETVector();

    double weight = 1.;
    if(!IsThisDataFile){
      weight *= weight_norm_1invfb*ev.GetTriggerLumi("Full");
    }

    std::map<TString, bool> map_bool_To_Region;
    map_bool_To_Region["OS"] = IsOS;
    map_bool_To_Region["OnZ_OS"] = IsOS && IsOnZ(Z.M(), 15.);
    map_bool_To_Region["WithBJet_METgt30_OS"] = IsOS && (NBJets>0) && (METv.Pt()>30.);

    for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

      TString this_region = it_map->first;
      if(it_map->second){

        if(IsOS){

          JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 500, 0., 500.);
          JSFillHist(this_region, "METphi_"+this_region, METv.Phi(), weight, 60, -3., 3.);

          JSFillHist(this_region, "Lepton_0_Pt_"+this_region, lep[0]->Pt(), weight, 500, 0., 500.);
          JSFillHist(this_region, "Lepton_0_Eta_"+this_region, lep[0]->Eta(), weight, 60, -3., 3.);
          JSFillHist(this_region, "Lepton_0_RelIso_"+this_region, lep[0]->RelIso(), weight, 100, 0., 1.);

          JSFillHist(this_region, "Lepton_1_Pt_"+this_region, lep[1]->Pt(), weight, 500, 0., 500.);
          JSFillHist(this_region, "Lepton_1_Eta_"+this_region, lep[1]->Eta(), weight, 60, -3., 3.);
          JSFillHist(this_region, "Lepton_1_RelIso_"+this_region, lep[1]->RelIso(), weight, 100, 0., 1.);

          JSFillHist(this_region, "Z_Mass_"+this_region, Z.M(), weight, 500, 0., 500.);
          JSFillHist(this_region, "Z_Pt_"+this_region, Z.Pt(), weight, 500, 0., 500.);
          JSFillHist(this_region, "Z_Eta_"+this_region, Z.Eta(), weight, 60, -3., 3.);

          JSFillHist(this_region, "Jet_Size_"+this_region, myjets.size(), weight, 10, 0., 10.);
          JSFillHist(this_region, "NBJets_"+this_region, NBJets, weight, 10, 0., 10.);


        } // END if(OS)

      } // END if(pass Region)

    } // END Loop mapRegionBool

  } // END Loop Suffixs

}
