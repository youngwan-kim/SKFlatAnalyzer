#include "SKFlatValidation.h"

void SKFlatValidation::executeEvent(){

  Event ev = GetEvent();

  //bool PassDiMuon = ev.PassTrigger("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v");
  bool PassSingleMuon = ev.PassTrigger("HLT_IsoMu27_v");
  bool PassDiElectron = ev.PassTrigger("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");
  //bool PassSingleElectron = ev.PassTrigger("HLT_Ele35_WPTight_Gsf_v");

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

  //==== Based on which trigger is fired
  std::vector< TString > Suffixs = {
    "SingleMuon",
    "DiElectron",
  };
  std::vector< bool > PassTriggers = {
    PassSingleMuon && (muons.size()>=1) && (electrons.size()==0),
    PassDiElectron && (electrons.size()==2) && (muons.size()==0),
  };

  for(unsigned int i=0; i<Suffixs.size(); i++){

    TString Suffix = Suffixs.at(i);
    if( !PassTriggers.at(i) ) continue;

    if(Suffix.Contains("SingleMuon")){
      if( muons.at(0).Pt() < 29. ) continue;
    }
    else if(Suffix.Contains("DiElectron")){
      if( electrons.at(0).Pt() < 25. || electrons.at(1).Pt() < 15. ) continue;
    }
    else{

    }

    if(this->IsThisDataFile){
      if(this->DataStream == "SingleMuon"){
        if(!( Suffix.Contains("SingleMuon") )) continue;
      }
      else if(this->DataStream == "DoubleEG"){
        if(!( Suffix.Contains("DiElectron") )) continue;
      }
      else{

      }
    }
    
    std::vector<Lepton *> lep;
    if(Suffix.Contains("SingleMuon")){
      lep = MakeLeptonPointerVector(muons);
    }
    else if(Suffix.Contains("DiElectron")){
      lep = MakeLeptonPointerVector(electrons);
    }
    else{

    }

    Particle METv = ev.GetMETVector();

    int n_lepton = lep.size();
    //==== DiLepton variables
    bool IsOS = false;
    Particle Z;
    if(n_lepton==2){
      IsOS = (lep[0]->Charge() != lep[1]->Charge());
      Z = (*lep[0])+(*lep[1]);
    }
    //==== SingleLepton variables
    double this_MT=-999.;
    if(n_lepton==1){
      this_MT = MT( METv, (*lep[0]) );
    }

    double weight = 1.;
    if(!IsThisDataFile){
      //cout << "weight_norm_1invfb = " << weight_norm_1invfb << endl;
      //cout << "GetTriggerLumi = " << ev.GetTriggerLumi("Full") << endl;
      weight *= weight_norm_1invfb*ev.GetTriggerLumi("Full")*ev.MCweight();

      //==== FIXME add third lepton veto later
      if(Suffix.Contains("SingleMuon")){
        for(unsigned int i=0; i<muons.size(); i++){
          double this_idsf  = mccor.MuonID_SF ("NUM_TightID_DEN_genTracks",          muons.at(i).Eta(),muons.at(i).MiniAODPt());
          double this_isosf = mccor.MuonISO_SF("NUM_TightRelIso_DEN_TightIDandIPCut",muons.at(i).Eta(),muons.at(i).MiniAODPt());
          double this_trigsf = mccor.MuonTrigger_SF("POGTight", "IsoMu27", muons);

          //weight *= this_idsf*this_isosf*this_trigsf;

        }
      }
      if(Suffix.Contains("DiElectron")){
        for(unsigned int i=0; i<electrons.size(); i++){
          double this_recosf = mccor.ElectronReco_SF(electrons.at(i).scEta(),electrons.at(i).Pt());
          double this_idsf = mccor.ElectronID_SF("passMVAID_iso_WP80",electrons.at(i).scEta(),electrons.at(i).Pt());
          //weight *= this_recosf*this_idsf;
        }
      }


    }

    std::map<TString, bool> map_bool_To_Region;
    if(n_lepton==2){
      //==== generic two OS lepton
      map_bool_To_Region["OS"] = IsOS;
      //==== OnZ event
      map_bool_To_Region["OnZ_OS"] = IsOS && IsOnZ(Z.M(), 15.);
      //==== With B-jet, MET > 30 for dilepton ttbar
      map_bool_To_Region["WithBJet_METgt30_OS"] = IsOS && (NBJets>0) && (METv.Pt()>30.);
    }
    if(n_lepton==1){
      map_bool_To_Region["W_CR"] = ( METv.Pt() > 30. ) && ( this_MT > 30. );
    }

    for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

      TString this_region = it_map->first;
      this_region = Suffix+"_"+this_region;

      if(it_map->second){

        JSFillHist(this_region, "NEvent_"+this_region, 0., weight, 1, 0., 1.);

        JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 500, 0., 500.);
        JSFillHist(this_region, "METphi_"+this_region, METv.Phi(), weight, 60, -3., 3.);

        JSFillHist(this_region, "Jet_Size_"+this_region, myjets.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "NBJets_"+this_region, NBJets, weight, 10, 0., 10.);

        JSFillHist(this_region, "Lepton_0_Pt_"+this_region, lep[0]->Pt(), weight, 500, 0., 500.);
        JSFillHist(this_region, "Lepton_0_Eta_"+this_region, lep[0]->Eta(), weight, 60, -3., 3.);
        JSFillHist(this_region, "Lepton_0_RelIso_"+this_region, lep[0]->RelIso(), weight, 100, 0., 1.);

        if(n_lepton==1){
          JSFillHist(this_region, "MT_"+this_region, this_MT, weight, 500, 0., 500.);
        }

        if(n_lepton>=2){

          JSFillHist(this_region, "Lepton_1_Pt_"+this_region, lep[1]->Pt(), weight, 500, 0., 500.);
          JSFillHist(this_region, "Lepton_1_Eta_"+this_region, lep[1]->Eta(), weight, 60, -3., 3.);
          JSFillHist(this_region, "Lepton_1_RelIso_"+this_region, lep[1]->RelIso(), weight, 100, 0., 1.);

          JSFillHist(this_region, "Z_Mass_"+this_region, Z.M(), weight, 500, 0., 500.);
          JSFillHist(this_region, "Z_Pt_"+this_region, Z.Pt(), weight, 500, 0., 500.);
          JSFillHist(this_region, "Z_Eta_"+this_region, Z.Eta(), weight, 60, -3., 3.);

        }



      } // END if(pass Region)

    } // END Loop mapRegionBool

  } // END Loop Suffixs

}
