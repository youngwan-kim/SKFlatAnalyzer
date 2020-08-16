#include "cr_study.h"

void cr_study::initializeAnalyzer(){

  Triggers_POGTight_Muon.clear();
  Triggers_POGHighPt_Muon.clear();

  //==== DATA 
  
  if(IsDATA){

    if(DataStream == "DoubleMuon"){

      if(HasFlag("BF") || HasFlag("G")) Triggers_POGTight_Muon = { 

        "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
        "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
        "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
        "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v" 

      };

      // DZ triggers were prescaled for period H
      if(HasFlag("H")) Triggers_POGTight_Muon = { 

        "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
        "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v" 

      };

//  TriggerNameForSF_POGTight_Muon = ""; 
    TriggerSafePt_POGTight_Muon = 26.;

    }

    if(DataStream == "SingleMuon"){

      Triggers_POGHighPt_Muon = { "HLT_Mu50_v", "HLT_TkMu50_v" };
      TriggerNameForSF_POGHighPt_Muon = "Mu50";
      TriggerSafePt_POGHighPt_Muon = 52.;

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
    param.Muon_Trigger_SF_Key = "";
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

  }

  if(param.Name=="POGHighPt"){

//  TriggerNameForSF_Muon = TriggerNameForSF_POGHighPt_Muon

    TriggerSafePt_Muon = TriggerSafePt_POGHighPt_Muon;

    if(!ev.PassTrigger(Triggers_POGHighPt_Muon)) return;

  }

  // Muons 
  std::vector<Muon> muons = GetMuons(param.Muon_Tight_ID, MinLeptonPt, 2.4); 

  // Jets ( DY needs no cleaning, TT needs lepton cleaning
  std::vector<Jet> jets;

  if(HasFlag("DY")) jets = GetJets(param.Jet_ID,20,2.7);
  
  if(HasFlag("TT")){ 

    std::vector<Muon> veto_muons = GetMuons("POGLoose", MinLeptonPt, 2.4);
    std::vector<Electron> veto_electrons = GetElectrons("passVetoID", MinLeptonPt, 2.5);
    jets = JetsVetoLeptonInside(GetJets("tight",20,2.7),veto_electrons,veto_muons);
  
  }

  std::vector<Jet> bjets;

  double HT=0;

  JetTagging::Parameters jtp_DeepCSV_Medium = JetTagging::Parameters(JetTagging::DeepCSV,JetTagging::Medium,JetTagging::incl, JetTagging::comb);
  double btagweight_1a = mcCorr->GetBTaggingReweight_1a(jets,jtp_DeepCSV_Medium);

/*  for(unsigned int i=0; i<jets.size(); i++){
  
    Jet this_jet = jets.at(i);
    HT += this_jet.Pt();

    double this_discr = this_jet.GetTaggerResult(JetTagging::DeepCSV);

    if( this_discr > mcCorr->GetJetTaggingCutValue(JetTagging::DeepCSV, JetTagging::Medium)){
      bjets.push_back(this_jet);
    }
  }*/
  cout<<"jetsize "<< jets.size() << "\n" ;

  std::sort(muons.begin(),muons.end(),PtComparing);
//std::sort(bjets.begin(),bjets.end(),PtComparing);
  std::sort(jets.begin(),jets.end(),PtComparing); 

  //==================
  //==== Cut Flow ====
  //==================

  FillHist("CutFlow",0.,1.,8,0,8); // No Cut , cut number is temporary

  //=========================
  //==== Event Selection ====
  // cms101.snu.ac.kr:8080/Heavy+Neutrino/200814_014357/HN_20200813.pdf
  //=========================  

  if(muons.size()!=2) return; // dimuon cut (exactly 2)
  FillHist("CutFlow",1,1.,8,0,8);

  Particle Cand = muons.at(0)+muons.at(1);

  if(muons.at(0).Charge()*muons.at(1).Charge()>0) return; // OS cut
  FillHist("CutFlow",2,1,8,0,8);

  if(Cand.M()<=50) return; // m(ll)>50GeV
  FillHist("CutFlow",3,1,8,0,8);
  
  if(param.Name=="POGTight"){

    if(muons.at(0).Pt()<=20 || muons.at(1).Pt()<=10) return;
    FillHist("CutFlow",4,1,8,0,8);

  }
  
  if(param.Name=="POGHighPt"){

    if(muons.at(0).Pt()<=50 || muons.at(1).Pt()<=50) return;
    FillHist("CutFlow",4,1,8,0,8);

  }

  if(HasFlag("DY")){

    if(bjets.size()>0) return;
    FillHist("CutFlow",5,1,8,0,8);

  // no bjet / m(ll) [80,100] / no jet cleaning required
  }

  if(HasFlag("TT")){

    if(METv.Pt()<=40) return;
    FillHist("CutFlow",5,1,8,0,8);

    if(jets.size()<2) return;
    FillHist("CutFlow",6,1,8,0,8);

    if(bjets.size()<1) return;
    FillHist("CutFlow",7,1,8,0,8);

  }     

  //======================
  //==== MC Weighting ====
  //======================

  double weight = 1.;
  if(!IsDATA){
    
    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full");
    weight *= ev.MCweight();
    weight *= weight_Prefire;

    for(unsigned int i=0; i<muons.size(); i++){    
  
      double this_pt = muons.at(i).MiniAODPt();
      double this_eta = muons.at(i).Eta();

      double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  this_eta, this_pt);
      double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, this_eta, this_pt);
      double this_trigsf = 1.;

      if(param.Name=="POGHighPt"){

        this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, muons);

      } 

      if(param.Name=="POGTight"){

        if(i==0) this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key,"Lead17", muons);

        if(i==1) this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, "Tail8", muons);

      }

      weight *= this_idsf*this_isosf*this_trigsf;

    }

  }

  //=========================
  //==== Fill Histograms ====
  //=========================

  // MET

  FillHist("METv_"+param.Name,METv.Pt(),weight,25,0,250);
  FillHist("MET_Phi_"+param.Name,METv.Phi(),weight,60,-3,3);
  
  // Jets

  FillHist("HT_"+param.Name,HT,weight,50,0,500);
  FillHist("NJets_"+param.Name,jets.size(),weight,10,0,10);
  FillHist("LeadingJet_Pt_"+param.Name,jets.at(0).Pt(),weight,30,0,450);
  FillHist("LeadingJet_Eta_"+param.Name,jets.at(0).Eta(),weight,30,-3,3);
  FillHist("NBJets_noSF_"+param.Name,bjets.size(),weight,10,0,10);
  FillHist("NBJets_SF1a_"+param.Name,bjets.size(),weight*btagweight_1a,10,0,10);
  FillHist("LeadingBJet_Pt_"+param.Name,bjets.at(0).Pt(),weight,30,0,450);
  
  // Muons ( index 0 leading muon )
 
  for(unsigned int i=0;i<muons.size();i++){

    TString index = TString::Itoa(i,10); 
    FillHist("Muon"+index+"_Pt_"+param.Name,muons.at(i).Pt(),weight,25,0,250);
    FillHist("Muon"+index+"_Eta_"+param.Name,muons.at(i).Eta(),weight,30,-3.,3.);
    FillHist("Muon"+index+"_Phi_"+param.Name,muons.at(i).Phi(),weight,35,-3.5,3.5);

  }

  // M(ll) plot
  FillHist("M(ll)_"+param.Name,Cand.M(),weight,20,50,250);

}

cr_study::cr_study(){

}

cr_study::~cr_study(){

}


