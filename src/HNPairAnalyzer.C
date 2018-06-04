#include "HNPairAnalyzer.h"

void HNPairAnalyzer::executeEvent(){

  AnalyzerParameter param;

  param.Name = "SUSYTight_SUDYLooseNoIPMiniIso0p6";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "SUSYLoose";
  param.Electron_Loose_RelIso = 0.4;
  param.Electron_Veto_ID = "SUSYLoose";
  param.Electron_Veto_RelIso = 0.4;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "TEST";
  param.Muon_Loose_RelIso = 0.6;
  param.Muon_Veto_ID = "TEST";
  param.Muon_Veto_RelIso = 0.6;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  executeEventFromParameter(param);

}

void HNPairAnalyzer::executeEventFromParameter(AnalyzerParameter param){

  //==== Get userflags

  //bool IsEMuRun = HasFlag("EMuRun");

  //==== Event selecitions

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  bool PassDiPhoton = ev.PassTrigger("HLT_DoublePhoton70_v");
  bool PassSingleMuon = ev.PassTrigger("HLT_Mu50_v");

  double MinPt = 75;

  std::vector<Electron> Veto_electrons  = GetElectrons(param.Electron_Tight_ID, MinPt, 2.5);
  std::vector<Muon>     Veto_muons      = GetMuons(param.Muon_Tight_ID, MinPt, 2.4);
  std::vector<Electron> Loose_electrons = GetElectrons(param.Electron_Tight_ID, MinPt, 2.5);
  std::vector<Muon>     Loose_muons     = GetMuons(param.Muon_Tight_ID, MinPt, 2.4);

  std::vector<FatJet>   fatjets         = GetFatJets(param.FatJet_ID, 300, 2.7);

  std::vector<Jet>      alljets         = GetJets(param.Jet_ID, 40., 2.7);
  std::vector<Jet>      jets            = JetsVetoLeptonInside(JetsAwayFromFatJet(alljets, fatjets), Veto_electrons, Veto_muons);

  int NBJets=0;
  for(unsigned int i=0; i<alljets.size(); i++){
    if(alljets.at(i).IsTagged(Jet::CSVv2, Jet::Medium)) NBJets++;
  }
  double HT(0.);
  for(unsigned int i=0; i<jets.size(); i++){
    HT += jets.at(i).Pt();
  }
  for(unsigned int i=0; i<fatjets.size(); i++){
    HT += fatjets.at(i).Pt();
  }

  //==== Based on which trigger is fired
  std::vector< TString > Suffixs = {
    //"DiPhoton",
    "SingleMuon",
  };
  std::vector< bool > PassTriggers = {
    //PassDiPhoton && (Loose_electrons.size()==2) && (Loose_muons.size()==0),
    PassSingleMuon && (Loose_electrons.size()==0) && (Loose_muons.size()==2),
  };

  for(unsigned int it_Suffix=0; it_Suffix<Suffixs.size(); it_Suffix++){

    TString Suffix = Suffixs.at(it_Suffix);
    bool PassTrigger = PassTriggers.at(it_Suffix);

    if(!( PassTrigger )) continue;

    if(this->IsDATA){
      if(this->DataStream == "DoubleEG"){
        if(!( Suffix.Contains("DiPhoton") )) continue;
      }
      else if(this->DataStream == "SingleMuon"){
        if(!( Suffix.Contains("SingleMuon") )) continue;
      }
      else{

      }
    }

    std::vector<Lepton *> leps;
    if(Suffix.Contains("DiPhoton")){
      leps = MakeLeptonPointerVector(Loose_electrons);
    }
    else if(Suffix.Contains("SingleMuon")){
      leps = MakeLeptonPointerVector(Loose_muons);
    }
    else{

    }

    bool IsOS = false;
    Particle Z = (*leps[0])+(*leps[1]);;
    IsOS = (leps[0]->Charge() != leps[1]->Charge());

    double weight = 1.;
    if(!IsDATA){
      weight *= weight_norm_1invfb*ev.GetTriggerLumi("Full")*ev.MCweight();

      mccor.IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

      //==== FIXME add third lepton veto later
      for(unsigned int i=0; i<Loose_electrons.size(); i++){
        double this_recosf = mccor.ElectronReco_SF(Loose_electrons.at(i).scEta(),Loose_electrons.at(i).Pt());
        double this_idsf = mccor.ElectronID_SF(param.Electron_ID_SF_Key, Loose_electrons.at(i).scEta(), Loose_electrons.at(i).Pt());
        weight *= this_recosf*this_idsf;
      }
      for(unsigned int i=0; i<Loose_muons.size(); i++){
        double this_idsf  = mccor.MuonID_SF (param.Muon_ID_SF_Key,  Loose_muons.at(i).Eta(), Loose_muons.at(i).MiniAODPt());
        double this_isosf = mccor.MuonISO_SF(param.Muon_ISO_SF_Key, Loose_muons.at(i).Eta(), Loose_muons.at(i).MiniAODPt());
        double this_trigsf = mccor.MuonTrigger_SF(param.Muon_Trigger_SF_Key, "IsoMu27", Loose_muons);

        weight *= this_idsf*this_isosf*this_trigsf;
      }

    }

    std::map<TString, bool> map_bool_To_Region;

    //==== generic two OS lepton
    map_bool_To_Region["OS"] = IsOS;
    map_bool_To_Region["SS"] = !IsOS;

    if(fatjets.size()==0 && jets.size()>3){
      vector<Particle> Ns = RecoPairN(leps, fatjets, jets);
      FillHist(Suffix+"_N_Mass", Ns.at(0).M(), 1., 500, 0., 500);
      FillHist(Suffix+"_N_Mass", Ns.at(1).M(), 1., 500, 0., 500);
    }

  } // END Loop Suffix

}

vector<Particle> HNPairAnalyzer::RecoPairN(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets){

  vector<Particle> out;

  //==== check lepton size
  if(lepptrs.size()!=2) return out;;

  //==== Use the four leading jets
  if(fatjets.size()==0 && jets.size()>3){
    // (01)(23), (02)(13), (03)(12)

    Particle Dummy_AllJets = jets.at(0)+jets.at(1)+jets.at(2)+jets.at(3);

    double mindM = 999999999;
    Particle temp_N[2];

    //==== Loop over, which jet paired to jets.at(0)
    for(int i=1; i<=3; i++){

      Particle TwoJet[2];
      TwoJet[0] = jets.at(0)+jets.at(i);
      TwoJet[1] = Dummy_AllJets-TwoJet[0];


      //==== (TwoJet[0]+lep[0])(TwoJet[1]+lep[1])
      Particle N_00 = TwoJet[0]+*(lepptrs.at(0));
      Particle N_11 = TwoJet[1]+*(lepptrs.at(1));
      if( fabs(N_00.M()-N_11.M()) < mindM ){
        mindM = fabs(N_00.M()-N_11.M());
        temp_N[0] = N_00;
        temp_N[1] = N_11;
      }

      //==== (TwoJet[0]+lep[1])(TwoJet[1]+lep[0])
      Particle N_01 = TwoJet[0]+*(lepptrs.at(1));
      Particle N_10 = TwoJet[1]+*(lepptrs.at(0));
      if( fabs(N_01.M()-N_10.M()) < mindM ){
        mindM = fabs(N_01.M()-N_10.M());
        temp_N[0] = N_01;
        temp_N[1] = N_11;
      }

    }

    out.push_back(temp_N[0]);
    out.push_back(temp_N[1]);

  }
  else if(fatjets.size()==1 && jets.size()){

  }

  return out;

}


