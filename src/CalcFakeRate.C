#include "CalcFakeRate.h"

void CalcFakeRate::executeEvent(){

  //==============================
  //==== Trigger Pt(cone) ranges
  //==============================

  //Muon_TrigWithPtRange.PtValues = {10,              15,           30,           35,           45,           80,        999999};
  //Muon_TrigWithPtRange.Triggers = {"HLT_Mu3_PFJet40_v","HLT_Mu8_v", "HLT_Mu17_v", "HLT_Mu20_v", "HLT_Mu27_v","HLT_Mu50_v"};
  Muon_TrigWithPtRange.PtValues          = {35,           45,           80,        999999};
  Muon_TrigWithPtRange.Triggers          = {"HLT_Mu20_v", "HLT_Mu27_v","HLT_Mu50_v"};
  Muon_TrigWithPtRange.TriggerSafePtCuts = {23,            30,          55};
  Muon_TrigWithPtRange.Validate();

  Electron_TrigWithPtRange.PtValues          = {45,            55,               85,              150,              200,              250,               300,                350,              400,               999999};
  Electron_TrigWithPtRange.Triggers          = {"HLT_Photon25_v", "HLT_Photon33_v", "HLT_Photon50_v", "HLT_Photon75_v", "HLT_Photon90_v", "HLT_Photon120_v", "HLT_Photon150_v", "HLT_Photon175_v", "HLT_Photon200_v"};
  Electron_TrigWithPtRange.TriggerSafePtCuts = {28,                35,               55,               80,               100,              140,               170,               200,               250};
  Electron_TrigWithPtRange.Validate();

  //======================
  //==== AnalyzerParamer
  //======================

  AnalyzerParameter param;

  //==== To Check Denominator distribution, remove all selections for loose lepton

  param.Name = "SUSYTight_VeryLoose";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "NOCUT";
  param.Electron_Loose_RelIso = 999.;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "POGLoose";
  param.Muon_Loose_RelIso = 999.;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== SUS-17-009 ID

  param.Name = "SUSY";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "SUSYLoose";
  param.Electron_Loose_RelIso = 0.4;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "SUSYLoose";
  param.Muon_Loose_RelIso = 0.4;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== Test Loose Muon (0.6)

  param.Name = "SUSYTight_SUDYLooseNoIPMiniIso0p6";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "SUSYLoose";
  param.Electron_Loose_RelIso = 0.4;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "TEST";
  param.Muon_Loose_RelIso = 0.6;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== Test Loose Muon (0.4)

  param.Name = "SUSYTight_SUDYLooseNoIPMiniIso0p4";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "SUSYTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "SUSYLoose";
  param.Electron_Loose_RelIso = 0.4;

  param.Muon_Tight_ID = "SUSYTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "TEST";
  param.Muon_Loose_RelIso = 0.4;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

}

void CalcFakeRate::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  std::vector<Electron> Loose_electrons = GetElectrons(param.Electron_Loose_ID, 10., 2.5);
  std::vector<Muon> Loose_muons = GetMuons(param.Muon_Loose_ID, 10., 2.4);
  vector<Gen> gens = GetGens();

  vector<Lepton *> lepptrs_electron = MakeLeptonPointerVector(Loose_electrons);
  vector<Lepton *> lepptrs_muon = MakeLeptonPointerVector(Loose_muons);
  vector<Lepton *> lepptrs;
  for(unsigned int i=0;i<lepptrs_electron.size();i++){
    lepptrs.push_back( lepptrs_electron.at(i) );
  }
  for(unsigned int i=0;i<lepptrs_muon.size();i++){
    lepptrs.push_back( lepptrs_muon.at(i) );
  }

  vector<Jet> jets = GetJets("HN", 20., 2.7);
  vector<double> AwayJetMinPts = {20, 30, 40, 60, 100, 500};

  const int n_eta = 3;
  float etaarray[n_eta+1] = {0.0, 0.8, 1.479, 2.5};
  const int n_pt = 8;
  float ptarray[n_pt+1] = {5., 10., 15., 20., 30., 40., 50., 60., 70.};

  bool LooseMuonRelIso0p4 = param.Name.Contains("MiniIso0p4");

  //==== Find prompt and fake leptons
  //==== Also save IsTight

  vector<Lepton *> lepptrs_prompt, lepptrs_fake;
  vector<bool>     IsTight_prompt, IsTight_fake;
  int n_TightElectron(0), n_TightMuon(0);

  for(unsigned int i=0; i<lepptrs.size(); i++){

    Lepton *lepptr = lepptrs.at(i);
    Lepton lep = *lepptr;
    bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);
    double this_Tight_RelIso = param.Electron_Tight_RelIso;

    if(IsElectron){

    }
    else{
      this_Tight_RelIso = param.Muon_Tight_RelIso;
      if(LooseMuonRelIso0p4 && lep.MiniRelIso()>=0.4) continue;
    }

    double ptcone = lep.CalcPtCone(lep.MiniRelIso(), this_Tight_RelIso);
    lepptr->SetPtCone( ptcone );

    int lep_tyep=0;
    if(IsDATA){
      lep_tyep = 1;
    }
    else{
      lep_tyep = GetLeptonType(lep, gens);
    }

    bool PassTight = false;
    if(IsElectron){
      Electron *el = (Electron *)lepptr;
      PassTight = el->PassID(param.Electron_Tight_ID);
      if(PassTight) n_TightElectron++;
    }
    else{
      Muon *mu = (Muon *)lepptr;
      PassTight = mu->PassID(param.Muon_Tight_ID);
      if(PassTight) n_TightMuon++;
    }

    if(lep_tyep>0){
      lepptrs_prompt.push_back( lepptr );
      IsTight_prompt.push_back( PassTight );
    }
    else if(lep_tyep<0){
      lepptrs_fake.push_back( lepptr );
      IsTight_fake.push_back( PassTight );
    }

  }

  //==== TODO Add SingleLepton Trigger Norm Plots at Z-peak

  bool RunElectronSingleLeptonNormPlots = false;
  bool RunMuonSingleLeptonNormPlots = false;
  if(IsDATA){
    if(DataStream=="SingleMuon")   RunMuonSingleLeptonNormPlots = true;
    if(DataStream=="SinglePhoton") RunElectronSingleLeptonNormPlots = true;
  }
  else{
    RunElectronSingleLeptonNormPlots = true;
    RunMuonSingleLeptonNormPlots = true;
  }

  if(RunElectronSingleLeptonNormPlots){

    bool OneLeptonEvent = (n_TightElectron==1) && (Loose_electrons.size()==1);
    bool TwoLeptonEvent = (n_TightElectron==2) && (Loose_electrons.size()==2);

    for(unsigned int i=0; i<Electron_TrigWithPtRange.Triggers.size(); i++){
      TString this_trigger = Electron_TrigWithPtRange.Triggers.at(i);

      if(ev.PassTrigger(this_trigger)){

        //==== TODO Add SFs
        double weight = 1.;
        if(!IsDATA){
          weight *= weight_norm_1invfb * ev.GetTriggerLumi(this_trigger) * ev.MCweight();
        }

        if(OneLeptonEvent){
          double thisMT = MT( METv, Loose_electrons.at(0) );
          if( (METv.Pt() > 40.) && (Loose_electrons.at(0).Pt() > Electron_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_electrons.at(0).Pt() > 20.) ){
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MET", METv.Pt(), weight, 500, 0., 500.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MT", thisMT, weight, 500, 0., 500.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"Lepton_0_Pt", Loose_electrons.at(0).Pt(), weight, 1000, 0., 1000.);
          }
        }
        if(TwoLeptonEvent){
          double dilepmass = (Loose_electrons.at(0)+Loose_electrons.at(1)).M();
          bool OnZEvent = IsOnZ( dilepmass, 15. );
          if( OnZEvent && (Loose_electrons.at(1).Pt() > Electron_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_electrons.at(1).Pt() > 20.) ){
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Z_Mass", dilepmass, weight, 40, 70., 110.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_0_Pt", Loose_electrons.at(0).Pt(), weight, 1000, 0., 1000.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_1_Pt", Loose_electrons.at(1).Pt(), weight, 1000, 0., 1000.);
          }
        }


      }

    } // END Loop Trigger

  }
  if(RunMuonSingleLeptonNormPlots){

    bool OneLeptonEvent = (n_TightMuon==1) && (Loose_muons.size()==1);
    bool TwoLeptonEvent = (n_TightMuon==2) && (Loose_muons.size()==2);

    for(unsigned int i=0; i<Muon_TrigWithPtRange.Triggers.size(); i++){
      TString this_trigger = Muon_TrigWithPtRange.Triggers.at(i);

      if(ev.PassTrigger(this_trigger)){

        //==== TODO Add SFs
        double weight = 1.;
        if(!IsDATA){
          weight *= weight_norm_1invfb * ev.GetTriggerLumi(this_trigger) * ev.MCweight();
        }

        if(OneLeptonEvent){
          double thisMT = MT( METv, Loose_muons.at(0) );
          if( (METv.Pt() > 40.) && (Loose_muons.at(0).Pt() > Muon_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_muons.at(0).Pt() > 20.) ){
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MET", METv.Pt(), weight, 500, 0., 500.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MT", thisMT, weight, 500, 0., 500.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"Lepton_0_Pt", Loose_muons.at(0).Pt(), weight, 1000, 0., 1000.);
          }
        }
        if(TwoLeptonEvent){
          double dilepmass = (Loose_muons.at(0)+Loose_muons.at(1)).M();
          bool OnZEvent = IsOnZ( dilepmass, 15. );
          if( OnZEvent && (Loose_muons.at(1).Pt() > Muon_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_muons.at(1).Pt() > 20.) ){
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Z_Mass", dilepmass, weight, 40, 70., 110.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_0_Pt", Loose_muons.at(0).Pt(), weight, 1000, 0., 1000.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_1_Pt", Loose_muons.at(1).Pt(), weight, 1000, 0., 1000.);
          }
        }


      }

    } // END Loop Trigger

  }

  //==== MC Fake rate

  if(!IsDATA){

    for(unsigned int i=0; i<lepptrs_fake.size(); i++){

      Lepton *lepptr_fake = lepptrs_fake.at(i);
      Lepton lep = *lepptr_fake;
      TString str_lepflv = "Electron";
      bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);

      if(IsElectron){

      }
      else{
        str_lepflv = "Muon";
      }

      //==== MC Fake Rate
      FillFakeRatePlots(str_lepflv+"_"+param.Name, "MCFR", lepptrs_fake.at(i), IsTight_fake.at(i), 1.);

    }

  }

  //==== FR from Data
  //==== For Data, use all leptons
  //==== For MC, pick up prompt leptons 

  double weight = 1.;

  if(lepptrs_prompt.size()==1){ //TODO add veto

    Lepton *lepptr_prompt = lepptrs_prompt.at(0);
    Lepton lep = *lepptr_prompt;
    TString str_lepflv = "Electron";

    double ptcone = lepptr_prompt->PtCone();

    bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);
    if(!IsElectron) str_lepflv = "Muon";

    //===============================
    //==== Prepare event selections
    //===============================

    //==== 1) Trigger Pass

    bool PassTriggerByPt = false;
    TString ThisPtTrigger = "";
    if(IsElectron){
      ThisPtTrigger = Electron_TrigWithPtRange.GetTriggerFromPt(ptcone);
      if(ThisPtTrigger!="PTFAIL"){
        PassTriggerByPt = ev.PassTrigger(ThisPtTrigger);
      }
    }
    else{
      ThisPtTrigger = Muon_TrigWithPtRange.GetTriggerFromPt(ptcone);
      if(ThisPtTrigger!="PTFAIL"){
        PassTriggerByPt = ev.PassTrigger(ThisPtTrigger);
      }
    }

    if(PassTriggerByPt){

      double weight = 1.;
      if(!IsDATA){
        weight *= weight_norm_1invfb * ev.GetTriggerLumi(ThisPtTrigger) * ev.MCweight();
      }

      //TODO add Scale factors

      //==== 2) Loop over AwayJet pT here

      double thisMT = MT( METv, lep );

      for(unsigned int it_jpt=0; it_jpt<AwayJetMinPts.size(); it_jpt++){
        double ThisMinJetPt = AwayJetMinPts.at(it_jpt);

        for(unsigned int it_jet=0; it_jet<jets.size(); it_jet++){

          Jet jet = jets.at(it_jet);
          if(!( jet.Pt() > ThisMinJetPt )) continue;
          double dPhi = lep.DeltaR(jet);

          bool UseEvent = (dPhi > 2.5) && (jet.Pt()/lep.Pt() > 1.0) && (METv.Pt() < 80.) && (thisMT < 25.);
/*
          cout << "dPhi = " << dPhi << endl;
          cout << "jet.Pt()/lep.Pt() = " << jet.Pt()/lep.Pt() << endl;
          cout << "METv.Pt() = " << METv.Pt() << endl;
          cout << "thisMT = " << thisMT << endl;
          cout << "--> UseEvent = " << UseEvent << endl;
*/
          if(UseEvent){
            FillFakeRatePlots(str_lepflv+"_"+param.Name, "DATA", lepptr_prompt, IsTight_prompt.at(0), weight);
            break;
          }

        }

      }


    }

  }

}

void CalcFakeRate::FillFakeRatePlots(TString name, TString frtype, Lepton *lep, bool IsTight, double weight){

  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"PtCone", lep->PtCone(), weight, 2000, 0., 2000.);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"Eta", lep->Eta(), weight, 60, -3, 3.);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"RelIso", lep->RelIso(), weight, 100, 0., 1.0);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"MiniRelIso", lep->MiniRelIso(), weight, 100, 0., 1.0);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"dXY", fabs(lep->dXY()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"dXYSig", fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"dZ", fabs(lep->dZ()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"dZSig", fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"IP3D", fabs(lep->IP3D()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"IP3DSig", fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"NEvent", 0., weight, 1, 0., 1.);

  if(IsTight){

    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"PtCone", lep->PtCone(), weight, 2000, 0., 2000.);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"Eta", lep->Eta(), weight, 60, -3, 3.);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"RelIso", lep->RelIso(), weight, 100, 0., 1.0);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"MiniRelIso", lep->MiniRelIso(), weight, 100, 0., 1.0);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"dXY", fabs(lep->dXY()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"dXYSig", fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"dZ", fabs(lep->dZ()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"dZSig", fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"IP3D", fabs(lep->IP3D()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"IP3DSig", fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"NEvent", 0., weight, 1, 0., 1.);

  }

  if(lep->LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)lep;

    JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    if(IsTight){
      JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    }
  }
  else{
    Muon *mu = (Muon *)lep;

    JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"Chi2", mu->Chi2(), weight, 500, 0., 50.);
    if(IsTight){
      JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"Chi2", mu->Chi2(), weight, 500, 0., 50.);
    }

  }


}














