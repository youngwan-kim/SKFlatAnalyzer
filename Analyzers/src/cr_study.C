#include "cr_study.h"

void cr_study::initializeAnalyzer(){

  Triggers_POGTight_Muon.clear();
  Triggers_POGHighPt_Muon.clear();

  //==== DATA 
  
  if(IsDATA){

    if(DataStream == "DoubleMuon"){

      if(HasFlag("BF") || HasFlag("G")) Triggers_POGTight_Muon = dimuon_trigger_BG;
      if(HasFlag("H")) Triggers_POGTight_Muon = dimuon_trigger_H;

//  TriggerNameForSF_POGTight_Muon = ""; 
    TriggerSafePt_POGTight_Muon = 26.;

    }

    if(DataStream == "SingleMuon"){

      Triggers_POGHighPt_Muon = { "HLT_Mu50_v", "HLT_TkMu50_v" };
      TriggerNameForSF_POGHighPt_Muon = "Mu50";
      TriggerSafePt_POGHighPt_Muon = 52.;

//      cout << "[init] SingleMuon, Data, POGHighPt \n" ;

    } 

  }

  //==== MC

  if(!IsDATA){

    Triggers_POGTight_Muon = {

      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"

    };
    TriggerNameForSF_POGTight_Muon = ""; // TODO MC trigger sf... 
    TriggerSafePt_POGTight_Muon = 26.;

    Triggers_POGHighPt_Muon = { "HLT_Mu50_v", "HLT_TkMu50_v" };
    TriggerNameForSF_POGHighPt_Muon = "Mu50";
    TriggerSafePt_POGHighPt_Muon = 52.;

  }

  //==== B-Tagging 
  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb));
  mcCorr->SetJetTaggingParameters(jtps);

 //  cout << "[init] B Tagging init \n";
}

void cr_study::executeEvent(){

  weight_Prefire = GetPrefireWeight(0);

  AnalyzerParameter param;
 
  //==== POG Tight ==== ( dimuon )

  if(DataStream=="DoubleMuon"){

    param.Clear();
    param.Name = "POGTight";

    if(HasFlag("BF")) param.Muon_Trigger_SF_Key = "POGTight_BCDEF";
    if(HasFlag("G")||HasFlag("H")) param.Muon_Trigger_SF_Key = "POGTight_GH";

    param.Muon_Tight_ID =  "POGTightWithTightIso";
    param.Muon_ID_SF_Key = "NUM_TightID_DEN_genTracks";
    param.Muon_ISO_SF_Key = "NUM_TightRelIso_DEN_TightIDandIPCut";

    param.Jet_ID = "tight";  

    executeEventFromParameter(param);

  }

  //==== POG HighPt ==== ( single muon )

  if(DataStream=="SingleMuon"){

    param.Clear();
    param.Name = "POGHighPt";

    param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
    param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
    param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
    param.Muon_Trigger_SF_Key = "POGHighPtLooseTrkIso";

    param.Jet_ID = "tight";

    executeEventFromParameter(param);

  }

  if(!IsDATA){

    param.Clear();
    param.Name = "POGTight";
    param.Muon_Tight_ID =  "POGTightWithTightIso";
    param.Muon_ID_SF_Key = "NUM_TightID_DEN_genTracks";
    param.Muon_ISO_SF_Key = "NUM_TightRelIso_DEN_TightIDandIPCut";
    param.Muon_Trigger_SF_Key = "POGTight_BCDEF"; //TODO ??
    param.Jet_ID = "tight";

    executeEventFromParameter(param);

    param.Clear();
    param.Name = "POGHighPt";

    param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
    param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
    param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
    param.Muon_Trigger_SF_Key = "POGHighPtLooseTrkIso";
    param.Jet_ID = "tight";

    executeEventFromParameter(param);

  }


}

void cr_study::executeEventFromParameter(AnalyzerParameter param){

  FillHist(param.Name+"/CutFlow", 0., 1., 10, 0., 10.);
  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  TString TriggerNameForSF_Muon;
  double TriggerSafePt_Muon;
  double MinLeptonPt = 20 ;

  if(param.Name=="POGTight"){

    TriggerNameForSF_Muon = TriggerNameForSF_POGTight_Muon;
    TriggerSafePt_Muon = TriggerSafePt_POGTight_Muon;
    
    if(!ev.PassTrigger(Triggers_POGTight_Muon)) return;
    FillHist(param.Name+"/CutFlow", 1., 1., 10, 0., 10.);

  }

  if(param.Name=="POGHighPt"){

    TriggerNameForSF_Muon = TriggerNameForSF_POGHighPt_Muon;
    TriggerSafePt_Muon = TriggerSafePt_POGHighPt_Muon;

    if(!ev.PassTrigger(Triggers_POGHighPt_Muon)) return;
    FillHist(param.Name+"/CutFlow", 1., 1., 10, 0., 10.);

  }

  // Muons 
  std::vector<Muon> muons = GetMuons(param.Muon_Tight_ID, MinLeptonPt, 2.4); 

  muons = GetAllMuons();
  electrons = GetAllElectrons();
  electrons_veto = SelectElectrons(electrons, "HNType1_CutBasedVeto", 10, 2.5);

  if (param.Name=="POGTight") {

    muons_veto = SelectMuons(muons, "HNType1_POGVeto", 5., 2.4);
    muons_loose = SelectMuons(muons_veto, "HNType1_POGLoose", 10., 2.4);
    muons_tight = SelectMuons(muons_loose, "HNType1_POGTight", 10, 2.4);

  }

  if (param.Name=="POGHighPt") {

    muons_veto = SelectMuons(muons, "HNType1_HighPtVeto", 5., 2.4);
    muons_loose = SelectMuons(muons_veto, "HNType1_HighPtLoose", 10., 2.4);
    muons_tight = SelectMuons(muons_loose, "HNType1_HighPtTight", 10., 2.4);

  }

  // Jets ( DY needs no cleaning, TT needs lepton cleaning

  if(HasFlag("DY")) jets = GetJets(param.Jet_ID,20,2.7);  
  if(HasFlag("TT")) jets = JetsVetoLeptonInside(GetJets("tight",20,2.7),electrons_veto,muons_veto);

  for(unsigned int i=0; i<jets.size(); i++){
  
    Jet this_jet = jets.at(i);

    double this_discr = this_jet.GetTaggerResult(JetTagging::DeepCSV);

    if( this_discr > mcCorr->GetJetTaggingCutValue(JetTagging::DeepCSV, JetTagging::Medium)){
      bjets.push_back(this_jet);
    }

  }

  muons = muons_tight;
  std::sort(muons.begin(),muons.end(),PtComparing);
  std::sort(bjets.begin(),bjets.end(),PtComparing);
  std::sort(jets.begin(),jets.end(),PtComparing); 

  //==================
  //==== Cut Flow ====
  //==================

  FillHist(param.Name+"/CutFlow",2.,1.,10,0,10); // No Cut , cut number is temporary

  //=========================
  //==== Event Selection ====
  // cms101.snu.ac.kr:8080/Heavy+Neutrino/200814_014357/HN_20200813.pdf
  //=========================  

  if(muons.size()!=2) return; // dimuon cut (exactly 2)
  FillHist(param.Name+"/CutFlow", 3., 1., 10, 0., 10.);
  FillHist("CutFlow",1,1.,8,0,8);

  Particle Cand = muons.at(0)+muons.at(1);

  if(muons.at(0).Charge()*muons.at(1).Charge()>0) return; // OS cut
  FillHist(param.Name+"/CutFlow",4,1,10,0,10);

  if(Cand.M()<=50) return; // m(ll)>50GeV
  FillHist(param.Name+"/CutFlow",5,1,10,0,10);
  
  if(param.Name=="POGTight"){

    if(muons.at(0).Pt()<=20 || muons.at(1).Pt()<=10) return;
    FillHist(param.Name+"/CutFlow",6,1,10,0,10);

  }
  
  if(param.Name=="POGHighPt"){

    if(muons.at(0).Pt()<=50 || muons.at(1).Pt()<=50) return;
    FillHist(param.Name+"/CutFlow",6,1,10,0,10);

  }

  if(HasFlag("DY")){

    if(bjets.size()>0) return;
    FillHist(param.Name+"/CutFlow",7,1,10,0,10);

  // no bjet / m(ll) [80,100] / no jet cleaning required
  }

  if(HasFlag("TT")){

    if(METv.Pt()<=40) return;
    FillHist(param.Name+"/CutFlow",7,1,10,0,10);

    if(jets.size()<2) return;
    FillHist(param.Name+"/CutFlow",8,1,10,0,10);

    if(bjets.size()<1) return;
    FillHist(param.Name+"/CutFlow",9,1,10,0,10);

  }     

  //======================
  //==== MC Weighting ====
  //======================

  double weight = 1.;
  if(!IsDATA){
    
    weight *= weight_norm_1invpb;
    weight *= ev.MCweight();
    weight *= weight_Prefire;

    double trigger_lumi = 0.;
 
    if(ev.PassTrigger(dimuon_trigger_BG)) trigger_lumi += 27267.591;
    if(ev.PassTrigger(dimuon_trigger_H)) trigger_lumi += 8650.628;
    else trigger_lumi *= ev.GetTriggerLumi("Full");

    weight *= trigger_lumi;

    double this_idsf=1; double this_isosf=1; double this_trigsf=1;

    for(unsigned int i=0; i<muons.size(); i++){    
  
      double this_pt = muons.at(i).MiniAODPt();
      double this_eta = muons.at(i).Eta();

      this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  this_eta, this_pt);
      this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, this_eta, this_pt);

      if(param.Name=="POGHighPt"){

        this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, muons);

      } 

      if(param.Name=="POGTight"){

        if(i==0) this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key,"Lead17", muons);

        if(i==1) this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, "Tail8", muons);

      }

      JetTagging::Parameters jtp_DeepCSV_Medium = JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb);
      double btagweight_1a = mcCorr->GetBTaggingReweight_1a(jets, jtp_DeepCSV_Medium);
      weight *= this_idsf*this_isosf*this_trigsf*btagweight_1a;

    }

  }

  //=========================
  //==== Fill Histograms ====
  //=========================
  TString path = param.Name+"/";
  DrawHists(path + "muons_tight/", muons_tight, weight);
  DrawHists(path + "muons_loose/", muons_loose, weight);
  DrawHists(path + "jets", jets, weight);
  DrawHists(path + "bjets/", bjets, weight);
  DrawHists(path + "METv_xyCorr/", METv, weight);
  FillHist(path + "m(ll)",Cand.M(),weight,20,50,250);

}
/*  // MET

  FillHist("METv_"+param.Name,METv.Pt(),weight,25,0,250);
  FillHist("MET_Phi_"+param.Name,METv.Phi(),weight,60,-3,3);
  
  
  
  // Jets

  FillHist("HT_"+param.Name,HT,weight,50,0,500);
  FillHist("NJets_"+param.Name,jets.size(),weight,10,0,10);

  if(jets.size()!=0){

    FillHist("LeadingJet_Pt_"+param.Name,jets.at(0).Pt(),weight,30,0,450);
    FillHist("LeadingJet_Eta_"+param.Name,jets.at(0).Eta(),weight,30,-3,3);

  }

  FillHist("NBJets_noSF_"+param.Name,bjets.size(),weight,10,0,10);
  FillHist("NBJets_SF1a_"+param.Name,bjets.size(),weight*btagweight_1a,10,0,10);
  if(HasFlag("TT")) FillHist("LeadingBJet_Pt_"+param.Name,bjets.at(0).Pt(),weight,30,0,450);
  
  // Muons ( index 0 leading muon )
 
  for(unsigned int i=0;i<muons.size();i++){

    TString index = TString::Itoa(i,10); 
    FillHist("Muon"+index+"_Pt_"+param.Name,muons.at(i).Pt(),weight,25,0,250);
    FillHist("Muon"+index+"_Eta_"+param.Name,muons.at(i).Eta(),weight,30,-3.,3.);
    FillHist("Muon"+index+"_Phi_"+param.Name,muons.at(i).Phi(),weight,35,-3.5,3.5);

  }

  // M(ll) plot
  FillHist("M(ll)_"+param.Name,Cand.M(),weight,20,50,250);

}*/

void cr_study::DrawHists(TString path, const vector<Muon>& muons, const double& weight) {
  
  TString obj_path;

  for (unsigned int i = 0; i < muons.size(); i++) {

  obj_path = path + TString::Itoa(i+1, 10) + "/";
  FillHist(obj_path + "pt", muons.at(i).Pt(), weight, 300, 0., 300.);
  FillHist(obj_path + "eta", muons.at(i).Eta(), weight, 48, -2.4, 2.4);
  FillHist(obj_path + "phi", muons.at(i).Phi(), weight, 70, -3.5, 3.5);
  FillHist(obj_path + "RelIso", muons.at(i).RelIso(), weight, 50, 0., 0.5);
  FillHist(obj_path + "TrkIso", muons.at(i).TrkIso(), weight, 80, 0., 0.8);

  }
}

void cr_study::DrawHists(TString path, const vector<Jet>& jets, const double& weight) {
TString obj_path;

  FillHist(path + "size", jets.size(), weight, 14, 0., 14.);

  for (unsigned int i = 0; i < jets.size(); i++) {

    obj_path = path + TString::Itoa(i+1, 10) + "/";
    FillHist(obj_path + "pt", jets.at(i).Pt(), weight, 300, 0., 300.);
    FillHist(obj_path + "eta", jets.at(i).Eta(), weight, 48, -2.4, 2.4);
    FillHist(obj_path + "phi", jets.at(i).Phi(), weight, 70, -3.5, 3.5);

  }	
}

void cr_study::DrawHists(TString path, const Particle& METv, const double& weight) {
  
  FillHist(path + "pt", METv.Pt(), weight, 300, 0., 300.);
  FillHist(path + "eta", METv.Eta(), weight, 48, -2.4, 2.4); 
  FillHist(path + "phi", METv.Phi(), weight, 70, -3.5, 3.5);

}


cr_study::cr_study(){

}

cr_study::~cr_study(){

}


