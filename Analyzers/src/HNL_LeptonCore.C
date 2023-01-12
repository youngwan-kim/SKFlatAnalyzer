#include "HNL_LeptonCore.h"

void HNL_LeptonCore::initializeAnalyzer(){


  SglMuon_Channel    =   (IsDATA && (this->DataStream == "SingleMuon"        )); //--> Needed when adding Mu50 + validation                 
  DblEG_Channel      =   (IsDATA && (this->DataStream == DoubleElectronPD()  ));
  DblMuon_Channel    =   (IsDATA && (this->DataStream == DoubleMuonPD()      ));
  SglEG_Channel      =   (IsDATA && (this->DataStream == SingleElectronPD()      ));
  MuonEG_Channel     =  (IsDATA && (this->DataStream == MuonEGPD()      ));
    
  //=== VERBOSE                                                                                                                                        
  run_Debug = HasFlag("DEBUG");
  

  //=== bkg flags                                                                                                                                      
  RunFake   = HasFlag("RunFake");
  RunCF     = HasFlag("RunCF");

  IncludeFakeLepton = HasFlag("IncludeFakeLepton");

  /// Other flags                                                                                                                                      
  RunSyst = HasFlag("RunSyst");
  HEM1516 = HasFlag("HEM1516");


  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Loose, JetTagging::incl, JetTagging::comb) );
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Tight, JetTagging::incl, JetTagging::comb) );
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Loose, JetTagging::incl, JetTagging::mujets));
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets));
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets));


  // clear Trig vect

  TrigList_HNL_HighPtMu.clear();
  TrigList_HNL_DblMu.clear();
  TrigList_HNL_Mu.clear();
  TrigList_Full_Mu.clear();
  TrigList_POG_Mu.clear();

  // EG                                                                                                                                                                              
  TrigList_HNL_DblEG.clear();
  TrigList_HNL_EG.clear();
  TrigList_POG_EG.clear();
  TrigList_HNL_HighPtEG.clear();
  TrigList_Full_EG.clear();

  // MUG                                                                                                                                                                             
  TrigList_HNL_MuEG.clear();
  TrigList_POG_MuEG.clear();
  TrigList_Full_MuEG.clear();

  
  if(GetEraShort()=="2016a"){

    // MU
    TrigList_HNL_DblMu = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v", 
		       "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v", 
		       "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v", 
		       "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"};
  }
  if(GetEraShort()=="2016b"){

    // MU                                                                                                                                                     
    TrigList_HNL_DblMu = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v", 
		       "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"};//, "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"};
    
    //https://twiki.cern.ch/twiki/bin/view/CMS/MuonHLT2016
    // Do we include HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v which is only in periodH
    // Do we include HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v in perfiod F + G
  }

  if(DataYear==2016){

    // Mu};
    
    TrigList_POG_Mu = {    "HLT_IsoMu24_v",   "HLT_IsoTkMu24_v",};
    TrigList_HNL_HighPtMu = {"HLT_Mu50_v",   "HLT_TkMu50_v"};
    TrigList_HNL_Mu = {    "HLT_IsoMu24_v",   "HLT_IsoTkMu24_v",};
    // EG                                                                                                                                                     
    TrigList_HNL_DblEG = {  "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"};
    TrigList_HNL_EG    = {  "HLT_Ele27_WPTight_Gsf_v" ,
			    "HLT_Ele25_eta2p1_WPTight_Gsf_v" ,
			    "HLT_Ele115_CaloIdVT_GsfTrkIdT_v", 
			    "HLT_Photon175_v",
			    "HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v",
			    "HLT_Photon175_v",
			    "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v",
			    "HLT_Photon175_v",
			    "HLT_DoubleEle33_CaloIdL_MW_v",
			    "HLT_DoubleEle37_Ele27_CaloIdL_GsfTrkIdVL_v"};
    //https://twiki.cern.ch/twiki/bin/view/CMS/EgHLTRunIISummary
    TrigList_HNL_HighPtEG = {"HLT_Photon175_v","HLT_DoublePhoton60_v"};
    TrigList_POG_EG    = {  "HLT_Ele27_WPTight_Gsf_v" };

    //https://twiki.cern.ch/twiki/bin/view/CMS/EgHLTPathDetails

    // MUG                                                                                                                                                    
    TrigList_HNL_MuEG  = {"HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v", "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v"};
    TrigList_POG_MuEG  = {"HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v", "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v"};
    
  }  // END OF 2016 Triggers                                                                                                                 

  else if(DataYear==2017){


    TrigList_HNL_DblMu = { "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v", 
			"HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
			"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v" };
    TrigList_HNL_Mu = {  "HLT_IsoMu27_v"};
    TrigList_POG_Mu = {  "HLT_IsoMu27_v"};
    TrigList_HNL_HighPtMu = {"HLT_Mu50_v"};


    TrigList_HNL_DblEG = { "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"};
    TrigList_HNL_EG = { "HLT_Ele32_WPTight_Gsf_v", 
			"HLT_Ele32_WPTight_Gsf_L1DoubleEG_v",
			"HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
			"HLT_Photon200_v",
			"HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v",
			"HLT_Photon200_v",
			"HLT_DoubleEle33_CaloIdL_MW_v"};
    TrigList_POG_EG = {"HLT_Ele35_WPTight_Gsf_v","HLT_Ele32_WPTight_Gsf_v", "HLT_Ele32_WPTight_Gsf_L1DoubleEG_v"};
    TrigList_HNL_HighPtEG = {"HLT_Photon200_v","HLT_DoublePhoton70_v"};

    TrigList_HNL_MuEG  = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v"};

  } // end of 2017 triggers                                                                                                                  

  else if(DataYear==2018){

    TrigList_HNL_DblMu = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v"};
    TrigList_HNL_Mu = {   "HLT_IsoMu24_v"};
    TrigList_HNL_HighPtMu = {"HLT_Mu50_v"};
    TrigList_POG_Mu = {   "HLT_IsoMu24_v"};
    
    // MAIN TRIGGER
    TrigList_HNL_DblEG = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"};
    
    TrigList_HNL_EG = { "HLT_Ele32_WPTight_Gsf_v",
			"HLT_DoubleEle25_CaloIdL_MW_v" ,
			"HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
			"HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v"};

    TrigList_POG_EG = { "HLT_Ele32_WPTight_Gsf_v"};

    TrigList_HNL_HighPtEG = { "HLT_Photon200_v", "HLT_DoublePhoton70_v"};

    TrigList_HNL_MuEG  = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v"};

  }   

  //--- Method 1d using JetTagging::iterativefit needs csv file changing in histmap to run                                                   
  // jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::iterativefit, JetTagging::iterativefit) );  
  mcCorr->SetJetTaggingParameters(jtps);

  
  TrigList_Full_Mu.clear();
  TrigList_Full_EG.clear();
  TrigList_Full_MuEG.clear();
  for(auto itrig : TrigList_HNL_DblMu) TrigList_Full_Mu.push_back(itrig);
  for(auto itrig : TrigList_HNL_Mu) TrigList_Full_Mu.push_back(itrig);
  for(auto itrig : TrigList_HNL_HighPtMu) TrigList_Full_Mu.push_back(itrig);
  
  for(auto itrig : TrigList_HNL_DblEG) TrigList_Full_EG.push_back(itrig);
  for(auto itrig : TrigList_HNL_EG) TrigList_Full_EG.push_back(itrig);
  for(auto itrig : TrigList_HNL_HighPtEG) TrigList_Full_EG.push_back(itrig);

  for(auto itrig : TrigList_HNL_MuEG) TrigList_Full_MuEG.push_back(itrig);


  for(auto itrig : TrigList_Full_Mu){
    cout << "TrigList_Full_Mu  : "<< itrig << endl;
  }
  for(auto itrig : TrigList_Full_EG){
    cout << "TrigList_Full_EG  : "<< itrig << endl;
  } 

}

void HNL_LeptonCore::TriggerPrintOut(Event ev){
  
  vector<TString> PDs = {"DoubleEG","DoubleMuon","MuonEG","SingleElectron","SingleMuon", "EGamma"};

  cout << "TriggerPrintOut: MuMu" << endl;
  for(auto itrig : TrigList_HNL_DblMu){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl; 
    }
  }
  
  cout << " " << endl;

  cout << "TriggerPrintOut: EE" << endl;
  for(auto itrig : TrigList_HNL_DblEG){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl;
    }
  }

  cout << " " << endl;


  cout << "TriggerPrintOut: EMu" << endl;
  for(auto itrig : TrigList_HNL_MuEG){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl;
    }
  }

  cout << " " << endl;

  return;

}

//====================================================/====================================================
//====================================================/====================================================
//====================================================/====================================================


bool HNL_LeptonCore::PassTriggerSelection(HNL_LeptonCore::Channel channel,Event ev, std::vector<Lepton *> leps, TString selection, bool apply_ptcut){

  bool PassTrigger(false);

  if (channel == MuMu){
    
    std::vector<Lepton *> leps_muon;
    for(auto ilep : leps) {
      if(ilep->LeptonFlavour() == Lepton::MUON) leps_muon.push_back(ilep);
    }
    
    if(selection == "Dilep"){
      
      // Check It passes DiMu Trigger
      // Check if for data we are running on correct data stream
      
      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_HNL_DblMu);

      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_HNL_DblMu, leps_muon)) PassTrigger=false;

    }
    
    else     if(selection == "lep"){
      
      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_HNL_Mu);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_HNL_Mu, leps_muon)) PassTrigger=false;
    }
    else     if(selection == "POG"){

      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_POG_Mu);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_POG_Mu, leps_muon)) PassTrigger=false;
    }

    else if(selection == "HighPt"){

      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_HNL_HighPtMu);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_HNL_HighPtMu, leps_muon)) PassTrigger=false;

    }
    else if(selection == "Full"){

      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_Full_Mu);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_Full_Mu, leps_muon)) PassTrigger=false;

    }
    else {
      cout << "[HNL_LeptonCore::PassTriggerSelection ] selection not found.." << endl;
      exit(EXIT_FAILURE);
    }
  }
  
  
  if (channel == EE){
    
    std::vector<Lepton *> leps_eg;
    
    for(auto ilep : leps) {
      if(ilep->LeptonFlavour() == Lepton::ELECTRON) leps_eg.push_back(ilep);
    }
    
    
    if(selection == "Dilep"){
      
      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_HNL_DblEG);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_HNL_DblEG, leps_eg)) PassTrigger=false;
    }

    else     if(selection == "POG"){

      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_POG_EG);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_POG_EG, leps_eg)) PassTrigger=false;
    }



    else if(selection == "HighPt"){

      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_HNL_HighPtEG);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_HNL_HighPtEG, leps_eg)) PassTrigger=false;
 
    }
    
    
    else if(selection == "Full"){
      
      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_Full_EG);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_Full_EG, leps_eg)) PassTrigger=false;

    }
    else {
      cout << "[HNL_LeptonCore::PassTriggerSelection ] selection not found.." << endl;
      exit(EXIT_FAILURE);
    }
  }



  if (channel == EMu){
    
    
    if(selection == "Dilep"){
      
      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_HNL_MuEG);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_HNL_MuEG, leps)) PassTrigger=false;

    }
    
    else if(selection == "HighPt"){


      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_HNL_HighPtMu);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_HNL_HighPtMu, leps)) PassTrigger=false;

    }
    
    else    if(selection == "Full"){
      
      PassTrigger = PassTriggerAndCheckStream(ev,TrigList_Full_MuEG);
      if(apply_ptcut  && !PassPtTrigger(ev, TrigList_Full_MuEG, leps)) PassTrigger=false;

    }
    
    else {
      cout << "[HNL_LeptonCore::PassTriggerSelection ] selection not found.." << endl;
      exit(EXIT_FAILURE);
    }
  }
  
       



  return PassTrigger;
}

AnalyzerParameter HNL_LeptonCore::InitialiseHNLParameter(TString s_setup, TString tag){
  
  AnalyzerParameter param  ;
  param.Clear();

  param.Name  = s_setup+tag;
  param.DefName  =s_setup+tag;


  // Default settings
  
  param.syst_ = AnalyzerParameter::Central;
  param.MCCorrrectionIgnoreNoHist = true;

  param.MuFakeMethod = "Data";
  param.ElFakeMethod = "DataNoConv";
  param.CFMethod   = "Data";

  param.Jet_ID                     = "tight";
  param.Jet_MinPt = 10.;
  param.Jet_MaxEta = 5.;

  param.FatJet_ID                  = "tight";
  param.FatJet_MinPt = 200.;
  param.FatJet_MaxEta = 5.;

  param.BJet_Method                = "2a";

  param.Muon_MinPt = 5.;
  param.Muon_MaxEta = 2.4;

  param.Electron_MinPt = 10.;
  param.Electron_MaxEta = 2.5;

  param.Muon_Veto_ID = "HNVeto2016";
  param.Electron_Veto_ID = "HNVeto2016";

  param.Tau_Veto_ID = "HNVeto";

  if (s_setup=="MuonCFStudy"){

    param.CFMethod   = "MC";

    param.Muon_Tight_ID = "HNTightMuonCF";
    
    param.Electron_Tight_ID = "HNTightV2";


    if(RunFake){
      param.Muon_Tight_ID = "HNLooseV1";
      param.Electron_Tight_ID = "HNLooseV4";
    }
    return param;

  }


  if (s_setup=="SignalStudy"){

    param.CFMethod   = "MC";

    param.Muon_Tight_ID = "HNTightV2";
    
    param.Electron_Tight_ID = "HNTightV2";


    if(RunFake){
      param.Muon_Tight_ID = "HNLooseV1";
      param.Electron_Tight_ID = "HNLooseV4";
    }
    return param;

  }
  else if (s_setup=="HNL"){


    param.CFMethod   = "MC";

    param.Muon_Tight_ID = "HNTightV2";
    param.Electron_Tight_ID = "HNTightV2";

    if(RunFake){
      param.Muon_Tight_ID = "HNLooseV4";
      param.Electron_Tight_ID = "HNLooseV1";

    }

    return param;

  }
  else if (s_setup=="EXO17028"){
    
    param.CFMethod   = "MC";
    
    param.Muon_Tight_ID = "HNTight_17028";
    param.Electron_Tight_ID = "HNTight_17028";
    
    if(RunFake){
      param.Muon_Tight_ID = "HNLoose_17028";
      param.Electron_Tight_ID = "HNLoose_17028";
    }
    
    return param;
  }

  else if (s_setup=="POGTight"){

    param.CFMethod   = "MC";

    param.Muon_Tight_ID = "POGTightWithTightIso";
    param.Electron_Tight_ID = "passPOGTight";

    if(RunFake){
      param.Muon_Tight_ID = "HNLooseV1";
      param.Electron_Tight_ID = "HNLoosePOG";
    }

    return param;
  }


  cout << "[HNL_LeptonCore::InitialiseHNLParameters ] ID not found.." << endl;
  exit(EXIT_FAILURE);
  
  
}

AnalyzerParameter HNL_LeptonCore::InitialiseHNLParameters( TString param_name, vector<vector<TString> >  hnl_run_param){

  AnalyzerParameter param  ;
  param.Clear();

  param.syst_ = AnalyzerParameter::Central;
  param.Name  = param_name;
  param.DefName  = param_name;

  param.MCCorrrectionIgnoreNoHist = false;

  
  //==== Muon ID                                                                                                                                                                                                                                                                              
  param.Muon_Tight_ID             = hnl_run_param[1][1];
  param.Muon_Loose_ID             = hnl_run_param[1][2];
  param.Muon_Veto_ID              = hnl_run_param[1][3];
  param.Muon_FR_ID                = hnl_run_param[1][2];
  param.Muon_UsePtCone            = false;


  param.Muon_ID_SF_Key             = hnl_run_param[3][1];
  param.Muon_ISO_SF_Key            = hnl_run_param[3][2];
  param.Muon_Trigger_SF_Key        = hnl_run_param[3][3];
  param.Muon_Trigger_NameForSF     = hnl_run_param[3][4];
  param.Muon_RECO_SF_Key           = hnl_run_param[3][5];
  param.Muon_FR_Key                = hnl_run_param[3][6];
  //==== Electron ID                                                                                                                                                                                                                                                                          
  param.Electron_Tight_ID          = hnl_run_param[0][1];
  param.Electron_Loose_ID          = hnl_run_param[0][2];
  param.Electron_Veto_ID           = hnl_run_param[0][3];
  param.Electron_FR_ID             = hnl_run_param[0][2];

  param.Electron_ID_SF_Key         = hnl_run_param[2][1];
  param.Electron_Trigger_SF_Key    = hnl_run_param[2][2];
  param.Electron_Trigger_NameForSF = hnl_run_param[2][3];
  param.Electron_FR_Key            = hnl_run_param[2][4];

  param.Electron_UsePtCone         = false;

  //==== Jet ID                                                                                                                                                                                                                                                                               
  param.Jet_ID                     = hnl_run_param[4][1];
  param.FatJet_ID                  = hnl_run_param[4][2];
  param.BJet_Method                = "2a";

  return param;
}



  
void HNL_LeptonCore::PrintParam(AnalyzerParameter param){
  
  cout << "param name = " << param.Name << endl;
  cout << "param Default name = " << param.DefName << endl;
  cout << "Electron_Tight_ID = " << param.Electron_Tight_ID << endl;
  cout << "Electron_Loose_ID = " << param.Electron_Loose_ID << endl;
  cout << "Electron_Veto_ID = " << param.Electron_Veto_ID << endl;
  cout << "Electron_ID_SF_Key = " << param.Electron_ID_SF_Key << endl;
  cout << "Electron_Trigger_SF_Key = " << param.Electron_Trigger_SF_Key << endl;
  cout << "Electron_Trigger_NameForSF = " << param.Electron_Trigger_NameForSF << endl;
  cout << "Electron_FR_ID = " << param.Electron_FR_ID << endl;
  cout << "Electron_FR_Key = " << param.Electron_FR_Key << endl;

  cout << "Muon_Tight_ID = " << param.Muon_Tight_ID << endl;
  cout << "Muon_Loose_ID = " << param.Muon_Loose_ID << endl;
  cout << "Muon_Veto_ID = " << param.Muon_Veto_ID << endl;
  cout << "Muon_RECO_SF_Key = " << param.Muon_RECO_SF_Key << endl;
  cout << "Muon_ID_SF_Key = " << param.Muon_ID_SF_Key << endl;
  cout << "Muon_ISO_SF_Key = " << param.Muon_ISO_SF_Key << endl;
  cout << "Muon_Trigger_SF_Key = " << param.Muon_Trigger_SF_Key << endl;
  cout << "Muon_Trigger_NameForSF = " << param.Muon_Trigger_NameForSF << endl;
  cout << "Muon_FR_ID = " << param.Muon_FR_ID << endl;
  cout << "Jet_ID = " << param.Jet_ID << endl;
  cout << "FatJet_ID = " << param.FatJet_ID << endl;
  
  return;
  
}

double HNL_LeptonCore::SetupWeight(Event ev, AnalyzerParameter param){

  //=== Apply MC weight                                                                                                                                                                                    
  if(IsDATA) return 1.;

  double prefire_weight = 1.;
  
  if(param.syst_ == AnalyzerParameter::PrefireUp) prefire_weight = GetPrefireWeight(1);
  else if(param.syst_ == AnalyzerParameter::PrefireDown)  prefire_weight = GetPrefireWeight(-1);
  else  prefire_weight = GetPrefireWeight(0);


  double pileup_weight(1.);
  if(param.syst_ == AnalyzerParameter::PUUp) pileup_weight= GetPileUpWeight(nPileUp,1);
  else if(param.syst_ == AnalyzerParameter::PUDown) pileup_weight= GetPileUpWeight(nPileUp,-1);
  else pileup_weight= GetPileUpWeight(nPileUp,0);
  
  double this_mc_weight = ev.GetTriggerLumi("Full") * MCweight(true, true) * pileup_weight * GetKFactor()*prefire_weight;
 

  FillWeightHist("PrefireWeight_" ,GetPrefireWeight(0));
  FillWeightHist("MCWeight_" ,MCweight(true, true));
  FillWeightHist("MCFullWeight_" , this_mc_weight);
  FillWeightHist("PileupWeight_" , GetPileUpWeight(nPileUp,0));

  return this_mc_weight;
  
}


void HNL_LeptonCore::Fill_All_SignalRegion3(HNL_LeptonCore::Channel channel, TString signal_region3, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps,   Particle _MET,int _npv  , double w    , bool FullAnalysis ){

  Fill_SigRegionPlots3(channel,signal_region3 + "_highmass","", label, jets,  fatjets,  leps,  _MET, _npv, w, 9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.);
  
  if(!FullAnalysis)  return;
  if (channel == EE){
    
    // EE SR1/3
    
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn100", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 3.1, 25., 15., 50.,120., 120., 50., 110., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn125", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 3.1, 30., 25., 50.,120., 120., 90., 140., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn200", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 3.1, 55., 40., 50.,120., 220.,160., 225.,  6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn250", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999, 70., 60., 50.,120., 310.,220., 270.,  6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn300", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 80., 60., 50.,120., 370.,235., 335.,  6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn400", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 100., 65., 50.,120., 450.,335., 450.,  6.);

    
    Fill_SigRegionPlots3(channel, signal_region3 , "_mn500", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 65., 50.,120., 560.,400., 555., 6.);
    Fill_SigRegionPlots3(channel, signal_region3 , "_mn600", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 690., 6.);
    Fill_SigRegionPlots3(channel, signal_region3 , "_mn700", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 966., 6.);
    Fill_SigRegionPlots3(channel, signal_region3 , "_mn800", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1130., 6.);
    Fill_SigRegionPlots3(channel, signal_region3 , "_mn900", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1300., 6.);
    Fill_SigRegionPlots3(channel, signal_region3 , "_mn1000", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1490., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn1100", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1490., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn1200", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1600., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn1300", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn1400", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn1500", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn1700", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 2130., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn2000", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 2530., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn2500", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 9999., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn5000", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 9999., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "_mn20000", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 999999., 6.);

    
  }
  if (channel == MuMu){
    
    // CC limits                                                                                                                                                                                                                                                                                                                                             
    Fill_SigRegionPlots3(channel,signal_region3 , "mn100", label, jets,  fatjets ,  leps,  _MET, _npv, w, 4, 25., 3.1, 25., 15., 50.,120., 110., 55., 115., 9.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn125", label, jets,  fatjets ,  leps,  _MET, _npv, w, 4, 25., 3.1, 25., 25., 50.,120., 140., 85., 140., 7.);      
    Fill_SigRegionPlots3(channel,signal_region3 , "mn200", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 3.1, 50., 40., 50.,120., 250.,160., 215.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn250", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 3.1, 85., 45., 50.,120., 310.,215., 270.,  7.);      
    Fill_SigRegionPlots3(channel,signal_region3 , "mn300", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 100., 50., 50.,120., 370.,225., 340.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn400", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 60., 50.,120., 490.,296., 490.,  7.);
    Fill_SigRegionPlots3(channel, signal_region3 , "mn500", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 110., 60., 50.,120., 610.,370., 550., 7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn600", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 0., 50.,120., 680.,370., 630.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn700", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 0., 50.,120., 800.,370., 885.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn800", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 0., 50.,120., 800.,370., 890.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn900", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 0., 50.,120., 800.,370., 1225.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn1000", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 0., 50.,120., 800.,370., 1230.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn1100", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 1245., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn1200", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 0., 50.,120., 800.,370., 1690.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn1300", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 0., 50.,120., 800.,370., 1890.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn1400", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 999., 110., 0., 50.,120., 800.,370., 1940.,  7.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn1500", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2220., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn1700", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2520., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn2000", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2720., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn2500", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 3220., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn5000", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 99999., 6.);
    Fill_SigRegionPlots3(channel,signal_region3 , "mn20000", label, jets,  fatjets,  leps,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 9999999., 6.);
    
  } // MM


  return;
}

void HNL_LeptonCore::Fill_All_SignalRegion1(HNL_LeptonCore::Channel channel, TString signal_region1, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps, Particle _MET,int _npv  , double w, bool FullAnalysis){
  
  // get loop of systs + one nominal                                                                                                                         
  
  Fill_SigRegionPlots1(channel,signal_region1 + "_highmass","", label, jets,  fatjets,  leps,  _MET, _npv, w,  9999.,  9999.,  9999.,  9999.,  9999.,  9999.,  9999.);

  if(!FullAnalysis)  return;
  
  if(channel == EE){
    // EE SR2/4
    Fill_SigRegionPlots1(channel,signal_region1 , "mn100", label, jets,  fatjets,  leps,  _MET, _npv, w, 25.,15., 40.,130., 90., 220., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn125", label, jets,  fatjets,  leps,  _MET, _npv, w, 60.,15., 40.,130., 123., 145., 15.);////////////////
    Fill_SigRegionPlots1(channel,signal_region1 , "mn200", label, jets,  fatjets,  leps,  _MET, _npv, w, 100., 20., 40., 130., 173., 220., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn250", label, jets,  fatjets,  leps,  _MET, _npv, w, 100., 25., 40., 130., 220., 305., 15.);//////////////
    Fill_SigRegionPlots1(channel,signal_region1 , "mn300", label, jets,  fatjets,  leps,  _MET, _npv, w, 100., 30., 40., 130., 270, 330., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn400", label, jets,  fatjets,  leps,  _MET, _npv, w, 100., 35., 40., 130., 330., 440., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn500", label, jets,  fatjets,  leps,  _MET, _npv, w, 120., 35., 40., 130., 440., 565., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn600", label, jets,  fatjets,  leps,  _MET, _npv, w, 120., 0., 40., 130., 565., 675., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn700", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 635., 775., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn800", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 740., 1005., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn900", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 865., 1030., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1000", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 890., 1185., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1100", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1035., 1395., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1200", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1085., 1460., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1300", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1140., 1590., 15.);///////////
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1400", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1245., 1700., 15.);///////
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1500", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1300., 1800., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1700", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1300., 2000., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn2000", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1300., 2800., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn2500", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1300., 3800., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn5000", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1300., 5800., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn20000", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1300., 99999., 15.);
    
  }
  if(channel == MuMu){
    // fill mass binned selections                                                                                                                                                                                                                                                                                                                           
    // MM SR2/4
    Fill_SigRegionPlots1(channel,signal_region1 , "mn100", label, jets,  fatjets,  leps,  _MET, _npv, w, 25.,  15., 40.,130., 98., 145., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn125", label, jets,  fatjets,  leps,  _MET, _npv, w, 60.,  15., 40.,130., 110., 150., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn200", label, jets,  fatjets,  leps,  _MET, _npv, w, 100., 20., 40., 130., 175., 235., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn250", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 25., 40., 130., 226., 280., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn300", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 20., 40., 130., 280., 340., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn400", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 65., 40., 130., 340., 445., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn500", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 65., 40., 130., 445., 560., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn600", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 560., 685., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn700", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 635., 825., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn800", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 755., 960., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn900", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 840., 1055., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1000", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0., 40., 130., 900., 1205., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1100", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 990., 1250., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1200", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1035., 1430., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1300", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1110., 1595., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1400", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1285., 1700., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1500", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1330., 1800., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn1700", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1330., 2200., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn2000", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1330., 99999., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn2500", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1330., 99999., 15.);
    Fill_SigRegionPlots1(channel,signal_region1 , "mn5000", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1330., 9999., 15.);    
    Fill_SigRegionPlots1(channel,signal_region1 , "mn20000", label, jets,  fatjets,  leps,  _MET, _npv, w, 140., 0.,40., 130., 1330., 999999., 15.);


  }

  if(channel == EMu){

  }
    
  return;
}



bool HNL_LeptonCore::CorrectChannelStream(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps){
  
  int n_el(0);
  int n_mu(0);
  for(auto ilep : leps)  {
    if(ilep->LeptonFlavour() == Lepton::MUON) n_mu++;
    if(ilep->LeptonFlavour() == Lepton::ELECTRON) n_el++;
  }
  
  if(channel==MuMu  || channel==EE || channel== EMu  ){
    
    if (leps.size() != 2) return false;

    if (channel==EE     && !(leps[0]->LeptonFlavour() == Lepton::ELECTRON && leps[1]->LeptonFlavour() == Lepton::ELECTRON)) return false;
    if (channel==MuMu   && !(leps[0]->LeptonFlavour() == Lepton::MUON     && leps[1]->LeptonFlavour() == Lepton::MUON))    return false;
    if (channel==EMu  &&
	!( (leps[0]->LeptonFlavour() == Lepton::ELECTRON && leps[1]->LeptonFlavour() == Lepton::MUON) ||
	   (leps[0]->LeptonFlavour() == Lepton::MUON && leps[1]->LeptonFlavour() == Lepton::ELECTRON) ))  return false;
  
    double lep1_ptcut= (channel==MuMu) ?   20. : 25.;
    double lep2_ptcut= (channel==MuMu) ?   10. : 15.;

    if(!(leps[0]->Pt() > lep1_ptcut && leps[1]->Pt()  > lep2_ptcut)) return false;
    
    return true;

  }

  
  if(channel==MuMuMu  || channel==EEE || channel==EMuL ){
    
    if( leps.size() != 3) return false;
    if(channel==MuMuMu && n_mu != 3) return false;
    if(channel==EEE && n_el != 3) return false;
    if(channel==EMuL&&  (n_el == 3  || n_mu == 3)) return false;
    
    
    double lep1_ptcut= (channel==MuMuMu) ?   20. : 25.;
    double lep2_ptcut= (channel==MuMuMu) ?   10. : 15.;
    double lep3_ptcut= (channel==MuMuMu) ?   10. : 10.;

    if(DataEra=="2017" ||DataEra=="2018"){
      lep2_ptcut=17;
      lep3_ptcut= 17;
    }

    if(!(leps[0]->Pt() > lep1_ptcut && leps[1]->Pt()  > lep2_ptcut && leps[2]->Pt()  > lep3_ptcut)) return false;

    return true;
  }

  if(channel == MuMuMuMu || channel == EEEE || channel == EMuLL ) {

    if( leps.size() != 4) return false;
    if( channel==MuMuMuMu && n_mu != 4) return false;
    if( channel==EEEE && n_el != 4) return false;
    if( channel == EMuLL && (n_mu == 0 || n_mu == 4)) return false;
    
    double lep1_ptcut= (channel==MuMuMuMu) ?   20. : 25.;
    double lep2_ptcut= (channel==MuMuMuMu) ?   10. : 15.;
    double lep3_ptcut= (channel==MuMuMuMu) ?   10. : 10.;
    double lep4_ptcut= (channel==MuMuMuMu) ?   10. : 10.;

    if(!(leps[0]->Pt() > lep1_ptcut && leps[1]->Pt()  > lep2_ptcut && leps[2]->Pt()  > lep3_ptcut && leps[3]->Pt()  > lep4_ptcut)) return false;

    return true;
  }

  return true;

}

double HNL_LeptonCore::GetPtCutTrigger(TString trigname, int nlep){
  
  if(trigname.Contains("HLT_Mu17") ||  trigname.Contains("HLT_TkMu17")) {
    if(trigname.Contains("Mu8") || trigname.Contains("TkMu8")) {
    if(nlep==0) return 20;
    if(nlep==1) return 10;
    if(nlep>1) return 5.;
    }  
  }

  else if(trigname.Contains("HLT_Mu50") && trigname.Contains("HLT_TkMu50")) {
    if(nlep==0) return 55;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Iso") && trigname.Contains("Mu24")) {
    if(nlep==0) return 26;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Iso") && trigname.Contains("Mu27")) {
    if(nlep==0) return 30;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Ele23_Ele12")){
    if(nlep==0) return 25;
    if(nlep==1) return 15.;
    if(nlep>1) return 10.;
  }

  else if(trigname.Contains("HLT_Ele27_WPTight")){
    if(nlep==0) return 30;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Ele25_eta2p1")){
    if(nlep==0) return 27;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Ele115")){
    if(nlep==0) return 120;
    if(nlep>0) return 5.;
  }
  
  else if(trigname.Contains("HLT_Photon175")){
    if(nlep==0) return 180;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Ele45_CaloIdVT")){
    if(nlep==0) return 50;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Ele50_CaloIdVT")){
    if(nlep==0) return 55;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_DoubleEle33")){
    if(nlep==0) return 35;
    if(nlep==1) return 35;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_DoubleEle37_Ele27")){
    if(nlep==0) return 40;
    if(nlep==1) return 30;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_DoublePhoton60")){
    if(nlep==0) return 65;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Mu8_TrkIsoVVL_Ele23")){
    if(nlep==0) return 25;
    if(nlep==1) return 10;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Mu23_TrkIsoVVL_Ele8")){
    if(nlep==0) return 25;
    if(nlep==1) return 10;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_IsoMu27")){
    if(nlep==0) return 30;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_Ele27_")){
    if(nlep==0) return 30;
    if(nlep>0) return 5.;
  }
  else if(trigname.Contains("HLT_Ele32_")){
    if(nlep==0) return 35;
    if(nlep>0) return 5.;
  }
  else if(trigname.Contains("HLT_Ele35")){
    if(nlep==0) return 37;
    if(nlep>0) return 5.;
  }
  else if(trigname.Contains("HLT_Photon200")){
    if(nlep==0) return 210;
    if(nlep>0) return 5.;
  }
  else if(trigname.Contains("HLT_DoublePhoton70")){
    if(nlep==0) return 80;
    if(nlep==1) return 80;
    if(nlep>0) return 5.;
  }

  else if(trigname.Contains("HLT_DoubleEle25")){
    if(nlep==0) return 28;
    if(nlep==1) return 28;
    if(nlep>0) return 5.;
  }
  else{
    cout << "[HNL_LeptonCore::GetPtTrigger ] ID not found.. " << trigname << endl;
    exit(EXIT_FAILURE);

  }

  return 0.;
}
bool HNL_LeptonCore::PassPtTrigger(Event ev, vector<TString> triglist,std::vector<Lepton *> leps){

  /// Code to check if event passes trigger/ Trigger is in PD running on for DAta and pt cuts are applied
  

  for(auto itrig : triglist){

    bool pass_trig_pt=true;    
    if(IsData&&!ev.IsPDForTrigger(itrig, this->DataStream)) continue;

    double lep1_ptcut = GetPtCutTrigger(itrig,0);
    double lep2_ptcut = GetPtCutTrigger(itrig,1);
    
    if(leps.size() >0 && lep1_ptcut >= 0){
      if(leps[0]->Pt() < lep1_ptcut) pass_trig_pt=false;
    } 
    if(leps.size() >1 && lep2_ptcut >= 0){
      if(leps[1]->Pt() < lep2_ptcut) pass_trig_pt= false;
    }
    
    if(pass_trig_pt) return true;
  }

  return false;
}

bool HNL_LeptonCore::CheckSRStream(Event ev,HNL_LeptonCore::Channel channel_ID){

  if(!IsData) return true;
  //  if(channel_ID==MuMu) return CheckStream(ev, TrigList_HNL_DblMu);
  //if(channel_ID==EE) return CheckStream(ev, TrigList_HNL_DblEG);
  //if(channel_ID==EMu) return CheckStream(ev, TrigList_HNL_MuEG);
  
  return true;
}

bool HNL_LeptonCore::PassTriggerAndCheckStream(Event ev, vector<TString> triglist){

  bool trig_passed=false;
  for(auto itrig : triglist){
    if(!IsData) {
      if(ev.PassTrigger(itrig))  trig_passed=true;
    }
    else if(ev.PassTrigger(itrig) && ev.IsPDForTrigger(itrig, this->DataStream)) trig_passed=true;
  }
  
  return trig_passed;
}


TString HNL_LeptonCore::MuonEGPD(){

  /// --- return PD name for EE channel data                                                                                                 
  return "MuonEG";
  return "";


}



TString HNL_LeptonCore::DoubleElectronPD(){

  /// --- return PD name for EE channel data                                                                                                 
  if(DataYear==2016) return "DoubleEG";
  if(DataYear==2017) return "SingleElectron";
  if(DataYear==2018) return "EGamma";

  return "";


}


TString HNL_LeptonCore::SingleElectronPD(){

  /// --- return PD name for EE channel data
  if(DataYear==2016) return "SingleElectron";
  if(DataYear==2017) return "SingleElectron";
  if(DataYear==2018) return "EGamma";

  return "";
  

}

TString HNL_LeptonCore::DoubleMuonPD(){

  /// --- return PD name for EE channel data                                                                                                                                      
  if(DataYear==2016) return "DoubleMuon";
  if(DataYear==2017) return "DoubleMuon";
  if(DataYear==2018) return "DoubleMuon";

  return "";

}

HNL_LeptonCore::HNL_LeptonCore(){
      
  rand_ = new TRandom3(1234);

  RunFake = false;
  RunCF= false;
  IsCentral=true;
  RunFullAnalysis=true;
  run_Debug=false;

}
 
 HNL_LeptonCore::~HNL_LeptonCore(){
   delete rand_;

}



void HNL_LeptonCore::Fill_SigRegionPlots4(HNL_LeptonCore::Channel channel, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Lepton *> leps, Particle  met, double nvtx,  double w){


  if(!CorrectChannelStream(channel, leps)) return;

  double HT(0.);
  for(auto ij : jets) HT += ij.Pt();
  
  //double ST += lep1.Pt() + lep2.Pt() + met.Pt();

  int NJet = jets.size();
  int bin = 0;
  double mass_lll = (*leps[0]+ *leps[1]+ *leps[2]).M();
  double LToHT=HT/(leps[0]->Pt()+leps[1]->Pt()+leps[2]->Pt());

  if(LToHT < 10.){
    if(mass_lll < 50){
      if(NJet<=0) bin=1;
      else bin=2;
    }
    else{
      if(NJet<=0) bin=3;
      else bin=4;
    }
  }
  else {
    if(mass_lll < 50){
      if(NJet<=0) bin=5;
      else bin=6;
    }
    else{
      if(NJet<=0) bin=7;
      else bin=8;
    }

  }
  FillHist( label + "/LimitInput_HNL_SR4/signalbin_"+label+"_HNL_SR4",  bin,  w, 8, 0.,8., "SRs");


}

void HNL_LeptonCore::Fill_SigRegionPlots3(HNL_LeptonCore::Channel channel, TString label_sr, TString label_mass, TString label_anid,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Lepton *> leps , Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11){
  
  bool pass = Fill_DefSigRegionPlots3(channel, label_sr, label_mass, label_anid, jets, fatjets, leps, met, nvtx, w, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11 );
  
  //Fill_RegionPlots      (channel,true, "FailSR3/"+label_mass , label_anid, jets, fatjets, els, mus,  met, nvtx, w);

  return;
}

bool  HNL_LeptonCore::Fill_DefSigRegionPlots3(HNL_LeptonCore::Channel channel, TString label_sr, TString label_mass, TString label_anid,  std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11){
  
  
  if(!CorrectChannelStream(channel, leps)) return false;
  
  if(jets.size() < 2) return false;
  
  
  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  int n=-999;
  double ST(0.);
  for(UInt_t emme=0; emme<jets.size(); emme++){
    ST += jets[emme].Pt();
    for(UInt_t enne=1; enne<jets.size(); enne++) {

      dijetmass_tmp = (jets[emme]+jets[enne]).M();
      if(emme == enne) continue;

      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
	dijetmass = dijetmass_tmp;
	m = emme;
	n = enne;
      }
    }
  }


  ST += leps[0]->Pt() + leps[1]->Pt() + met.Pt();
  double met2 = pow(met.Pt(),2.);
  Particle Wcand = jets[m] + jets[n];
  Particle W1cand = jets[m] + jets[n]+ *leps[0] + *leps[1];
  Particle N1cand = jets[m] + jets[n]+ *leps[0];
  Particle N2cand = jets[m] + jets[n]+ *leps[1];


  if(N1cand.M() < var9 || N1cand.M() > var10)     FillHist( label_anid+"/"+label_sr+ "/"+label_mass +  "_FailN1_mN2" ,  N2cand.M(),  w, 2000, 0.,20000. );
  if(N2cand.M() < var9 || N2cand.M() > var10)     FillHist( label_anid+"/"+label_sr+ "/"+label_mass +  "_FailN2_mN1" ,  N1cand.M(),  w, 2000, 0.,20000. );
 

  if(!label_sr.Contains("highmass")){

    HNL_LeptonCore::SearchRegion cutflow_SR_index = SR3;
    //if  (label_anid.Contains("OS")) label_mass+="OS";
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_default","SR3_"+label_mass);
    
    if(jets.size() >= var1) return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_jetsize","SR3_"+label_mass);

    if(jets[0].Pt() < var2)return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_jetpt","SR3_"+label_mass);

    if(Wcand.DeltaR(*leps[1]) > var3) return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_W_DR_lep2","SR3_"+label_mass);

    if(leps[0]->Pt() < var4) return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_lepPt1","SR3_"+label_mass);

    if(leps[1]->Pt() < var5) return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_lepPt2","SR3_"+label_mass);

    if(Wcand.M() < var6) return false;

    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_Wmass_Low","SR3_"+label_mass);

    if(Wcand.M() > var7) return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_Wmass_High","SR3_"+label_mass);
    if(W1cand.M() < var8) return false;

    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_lljj","SR3_"+label_mass);
    if(N1cand.M() < var9 && N2cand.M()  < var9) return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_Nmass_Low","SR3_"+label_mass);
    if(N1cand.M()  > var10 && N2cand.M()  > var10) return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_Nmass_High","SR3_"+label_mass);
    if(met2/ST > var11) return false;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR3_MET","SR3_"+label_mass);

    if(IsCentral)
      FillEventCutflowSR(label_anid,cutflow_SR_index,w, label_sr+"_"+label_mass);

    FillHist( label_anid+"/"+label_sr+ "/"+label_mass +  "_nevent" ,  1.,  w, 2, 0.,2. );
    return true;
    
  }
  

  return true;
}

void HNL_LeptonCore::Fill_SigRegionPlots1(HNL_LeptonCore::Channel channel,TString label_sr, TString label_mass, TString label_anid,  std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7){

  if(!CorrectChannelStream(channel, leps)) return;
  
  if(fatjets.size() == 0) return;

  
  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  double ST(0.);
  for(UInt_t emme=0; emme<jets.size(); emme++){
    ST += jets[emme].Pt();
  }
  for(UInt_t emme=0; emme<fatjets.size(); emme++){
    ST += fatjets[emme].Pt();
    dijetmass_tmp = fatjets[emme].SDMass();
    if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
    }
  }
  ST += leps[0]->Pt() + leps[1]->Pt() + met.Pt();

  double met2 = met.Pt()*met.Pt();
  Particle Wcand  = fatjets[m];
  Particle W1cand = fatjets[m] + *leps[0] + *leps[1];
  Particle N1cand = fatjets[m] + *leps[0];
  Particle N2cand = fatjets[m] + *leps[1];

  if(!label_sr.Contains("highmass")){
    
    HNL_LeptonCore::SearchRegion  cutflow_SR_index = SR1;
    TString label_mass_tmp=label_mass;
    //if  (label_anid.Contains("OS")) label_mass_tmp+="OS";

    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR1_default","SR1_"+label_mass_tmp);                                                                                                              
    if(leps[0]->Pt() < var1) return; 
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR1_lepPt1","SR1_"+label_mass_tmp);                                                                                                               
    if(leps[1]->Pt() < var2) return;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR1_lepPt2","SR1_"+label_mass_tmp);                                                                                                               
    if(fatjets[m].SDMass() < var3) return;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR1_SDmass_Low","SR1_"+label_mass_tmp);                                                                                                              
    if(fatjets[m].SDMass() > var4) return;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR1_SDmass_High","SR1_"+label_mass_tmp);                                                                                                         
    
    if(N1cand.M()  < var5 && N2cand.M()  < var5) return;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR1_Nmass_Low","SR1_"+label_mass_tmp);

    if(N1cand.M()  > var6 && N2cand.M()  > var6) return;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR1_Nmass_High","SR1_"+label_mass_tmp);

    if(met2/ST > var7) return;
    FillEventCutflowPerMass(label_anid,cutflow_SR_index, w, "SR1_MET","SR1_"+label_mass_tmp);
    
    FillHist( label_anid+"/"+label_sr+ "/"+ label_mass +  "_nevent"  , 1.,  w, 2, 0.,2. , "N cut");

    //HNL_LeptonCore::ChargeType charge_i = (leps[0]->Charge() ==  leps[1]->Charge() )  ?  SS : OS;

    //if(RunCF) charge_i = (leps[0]->Charge() ==  leps[1]->Charge() )  ?  OS : SS;

    FillEventCutflowSR(label_anid,cutflow_SR_index,w, label_sr+"_"+label_mass);

    return;
  }


  return;

}


void HNL_LeptonCore::Fill_RegionPlots(HNL_LeptonCore::Channel channel, int plotCR, TString plot_dir, TString region,   std::vector<Jet> jets,   std::vector<FatJet> fatjets, std::vector<Lepton *> Leps, Particle  met, double nvtx,  double w){
  
  std::vector<Jet> jets_vbf;
  Fill_RegionPlots(channel, plotCR,plot_dir ,region, jets, jets_vbf,fatjets, Leps, met, nvtx, w );

}

void HNL_LeptonCore::Fill_RegionPlots(HNL_LeptonCore::Channel channel, int plot_flag, TString plot_dir, TString region, std::vector<Jet> jets, std::vector<Jet> jets_vbf,   std::vector<FatJet> fatjets, std::vector<Lepton *> leps , Particle  met, double nvtx,  double w){
  

  bool debug(false);

  bool threelep = (leps.size()  == 3);
  bool fourlep  = (leps.size()  == 4);
  
  if(threelep && !(channel == EEE  || channel == EMuL || channel == MuMuMu )) return;
  if(fourlep  && !(channel == EEEE  || channel == MuMuMuMu || channel == EMuLL)) return;
     

  int nel(0), nmu(0);
  for(auto ilep: leps) {
    if(ilep->LeptonFlavour() == Lepton::ELECTRON) nel++;
    if(ilep->LeptonFlavour() == Lepton::MUON) nmu++;
  }

  bool DrawAll(false), DrawSyst(false);
  if (plot_dir.Contains("Syst")){
    DrawSyst=true;
  }
  else{
    DrawAll=true;
    DrawSyst=true;
  }

  /// Draw N leptons
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/" +  "N_El", nel,  w, 5, 0, 5, "El size");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/" +  "N_Mu", nmu,  w, 5, 0, 5, "Mu size");
  // Draw N jets
  if(DrawSyst) FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_AK4Jets", jets.size() , w, 10, 0., 10., "N_{AK4 jets}");
  if(DrawAll) FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_AK4VBFJets", jets_vbf.size() , w, 10, 0., 10., "N_{AK4 jets}");
  if(DrawSyst) FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_AK8Jets", fatjets.size() , w, 10, 0., 10., "N_{AK8 jets}");


  int NBJetsL=GetNBJets2a("tight","Loose");
  int NBJetsM=GetNBJets2a("tight","Medium");
  int NBJetsT=GetNBJets2a("tight","Tight");
  int NBJetsDJL=GetNBJets2a("tight","DeepJet_Loose");
  int NBJetsDJM=GetNBJets2a("tight","DeepJet_Medium");
  int NBJetsDJT=GetNBJets2a("tight","DeepJet_Tight");

  if(leps.size() < 2) return;

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/" +  "SumQ", leps[0]->Charge() + leps[1]->Charge(),  w, 10, -5, 5, "Q size");

  Particle llCand = *leps[0] + *leps[1];
  Particle lllCand = (threelep) ? *leps[0] + *leps[1] + *leps[2] :  Particle();
  Particle lljjCand;

  if(fatjets.size() > 0){
    Particle llJCand =  *leps[0] + *leps[1]+ fatjets[0];
    Particle l1JCand = *leps[0] +  fatjets[0];
    Particle l2JCand = *leps[1] +  fatjets[0];

    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/" +  "AK8Jet_Mass_llJ",  llJCand.M(),  w, 2000, 0, 20000, "Reco M_{llJ}");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/" +  "AK8Jet_dR_W_lep1",   fatjets[0].DeltaR(*leps[0]),  w, 10, 0, 5, "#DeltaR (Wj1,lep1)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/" +  "AK8Jet_dR_W_lep2",   fatjets[0].DeltaR(*leps[1]),  w, 10, 0, 5, "#DeltaR (Wj2,lep2)");

    if(threelep){
      Particle l3JCand = *leps[2]  +  fatjets[0];
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "AK8Jet_dR_W_lep3",   fatjets[0].DeltaR(*leps[2]),  w, 10, 0, 5, "#DeltaR (Wj1,lep3)");
    }
    if(fourlep){
      Particle l4JCand = *leps[3]  +  fatjets[0];
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "AK8Jet_dR_W_lep4",   fatjets[0].DeltaR(*leps[3]),  w, 10, 0, 5, "#DeltaR (Wj1,lep4)");
    }
    if(fatjets[0].DeltaR(*leps[0] ) < fatjets[0].DeltaR(*leps[1] )){
      Particle lJcloseCand = *leps[0]  +  fatjets[0];
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "AK8Jet_Mass_W_close_lep",  lJcloseCand.M(),  w, 250, 0, 2000, "Reco M_{llJ}");
    }
  }

  if(jets.size() ==1 ) {
   
    
    lljjCand += jets[0];

    Particle WCand  = jets[0];
    Particle N1Cand = jets[0]+ *leps[0] ;
    Particle N2Cand = jets[0]+ *leps[1] ;

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Single_AK4Jet_M_l1W_M_l2W",  N1Cand.M(), N2Cand.M(), w, 200, 0., 20000., 200, 0., 20000.);
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Single_AK4Jet_dR_W_lep1",   WCand.DeltaR(*leps[0] ),  w, 10, 0, 5, "#DeltaR (W,lep1)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Single_AK4Jet_dR_W_lep2",   WCand.DeltaR(*leps[1] ),  w, 10, 0, 5, "#DeltaR (W,lep2)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Single_AK4Jet_mW",   WCand.M(),  w, 50, 0, 500, "Reco_Onejet M_{jj}");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Single_AK4Jet_M_l1W",  N1Cand.M(),  w, 1000, 0., 20000., "Reco_Onejet M_{l1jj}");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Single_AK4Jet_M_l2W",  N2Cand.M(),  w,  1000, 0., 20000., "Reco_Onejet M_{l2jj} ");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Single_AK4Jet_M_llW",  lljjCand.M(),  w,  1000, 0., 20000., "Reco_Onejet M_{lljj}");

  }
  
  if(jets.size() > 1){
    
    float dijetmass_tmp=999.;
    float dijetmass=9990000.;
    int m=-999;
    int n=-999;

    for(UInt_t emme=0; emme<jets.size(); emme++){
      for(UInt_t enne=1; enne<jets.size(); enne++) {

        dijetmass_tmp = (jets[emme]+jets[enne]).M();
        if(emme == enne) continue;

        if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
	  dijetmass = dijetmass_tmp;
	  m = emme;
          n = enne;
	}
      }
    }
    lljjCand = *leps[0]  + *leps[1]  + jets[m]+jets[n];

  
    Particle WCand  = jets[m]+jets[n];
    Particle N1Cand  = jets[m]+jets[n]+ *leps[0] ;
    Particle N2Cand  = jets[m]+jets[n]+ *leps[1] ;
   
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_M_l1W_M_l2W",  N1Cand.M(), N2Cand.M(), w, 100, 0., 20000., 100, 0., 20000.);
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_dR_W_lep1",   WCand.DeltaR(*leps[0] ),  w, 10, 0, 5, "#DeltaR (W,lep1)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_dR_W_lep2",   WCand.DeltaR(*leps[1] ),  w, 10, 0, 5, "#DeltaR (W,lep2)");
    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_M_W",   WCand.M(),  w, 50, 0, 500, "Reco M_{jj}");
    
    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_M_l1W",   N1Cand.M(),  w, 1000, 0., 20000., "Reco M_{l1jj}");
    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_M_l2W",   N2Cand.M(),  w,  1000, 0., 20000., "Reco M_{l2jj} ");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_M_lAv12W", (N1Cand.M()+  N2Cand.M())/2.,  w,  1000, 0., 20000., "Reco M_{l1_2jj} ");
    
    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_M_llW",  lljjCand.M(),  w,  1000, 0., 20000., "Reco M_{lljj}");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_dR_Wj1_lep2",   jets[m].DeltaR(*leps[1] ),  w, 10, 0, 5, "#DeltaR (Wj1,lep2)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "DiJet_dR_Wj2_lep2",   jets[n].DeltaR(*leps[1] ),  w, 10, 0, 5, "#DeltaR (Wj2,lep2)");
  }// end dijet loop


  double LT(0.);
  for(auto i : leps){
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Leps_pt", i->Pt()  , w, 200, 0., 1000.,"1_{2} p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Leps_eta",i->Eta()  , w, 30, -3., 3,"l_{1} #eta");
    LT += i->Pt();
  }
  
  if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Lep_1_pt", leps[0]->Pt()  , w, 400, 0., 2000.,"l_{1} p_{T} GeV");
  if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Lep_2_pt", leps[1]->Pt()  , w, 200, 0., 1000.,"1_{2} p_{T} GeV");
  if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Lep_1_eta", leps[0]->Eta()  , w, 60, -3., 3,"l_{1} #eta");
  if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Lep_2_eta", leps[1]->Eta()  , w, 60, -3., 3.,"l_{2} #eta");

  if(threelep) {
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Lep_3_pt", leps[2]->Pt()  , w, 200, 0., 1000.,"l_{3} p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Lep_3_eta", leps[2]->Eta()  , w, 60, -3., 3.,"l_{3} #eta");
  }
  if(fourlep) {
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Lep_4_pt", leps[3]->Pt()  , w, 200, 0., 1000.,"l_{4} p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Lep_4_eta", leps[3]->Eta()  , w, 60, -3., 3.,"l_{4} #eta");
  }

  double HT = GetHT(jets, fatjets);
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Ev_LT", LT  , w, 200, 0., 2000.,"L_{T} GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Ev_HT", HT  , w, 250, 0., 5000.,"H_{T} GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "HToLepPt1", HT/ leps[0]->Pt()  , w, 100, 0., 20.,"HT/PT(1)");
  if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "HToLepPt2", HT/ leps[1]->Pt()  , w, 100, 0., 20.,"HT/PT(2)");
  if(threelep&&DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "HToLepPt3", HT/ leps[2]->Pt()  , w, 100, 0., 20.,"HT/PT(3)");
  if(fourlep&&DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "HToLepPt4", HT/ leps[3]->Pt()  , w, 100, 0., 20.,"HT/PT(4)");

  
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "nPV",  nvtx , w, 60, 0., 60.);
  
  double ST = GetST(leps, jets, fatjets, met);
  double met2_st = pow(met.Pt(),2.)/ ST;  

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Ev_ST", ST  , w, 500, 0., 10000.,"ST GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Ev_MET2_ST", met2_st  , w, 60, 0., 30.,"MET2/ST GeV");

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Ev_MET", met.Pt()  , w, 200, 0., 400.,"MET GeV");

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Mt_lep1", MT(*leps[0] ,met)  , w, 200, 0., 400.,"MT GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Mt_lep2", MT(*leps[1] ,met)  , w, 200, 0., 400.,"MT GeV");
 
  if(threelep) {
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "M_lll"+region, lllCand.M() , w, 200, 0., 800.,"M(lll) GeV");

    if(GetIndexNonMinOSSF(leps)> 0 && DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Mt_minOSSF", MT(*leps[GetIndexNonMinOSSF(leps)], met) , w, 200, 0., 800.,"M(T) non MinOS GeV");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "M_minOSSF", GetMassMinOSSF(leps), w, 200, 0., 800.,"M non MinOS GeV");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "M_minSSSF", GetMassMinSSSF(leps), w, 200, 0., 800.,"M(T) non MinSS GeV");
    if(GetIndexNonMinSSSF(leps) > 0 && DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "Mt_minSSSF", MT(*leps[GetIndexNonMinSSSF(leps)], met) , w, 200, 0., 800.,"M non MinSS GeV");

  }

  if(fourlep) {
    Particle llllCand = *leps[0] + *leps[1] + *leps[2] + *leps[3] ;
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "M_llll", llllCand.M() , w, 200, 0., 800.,"M(llll) GeV");
  }
  
  Event ev = GetEvent();
  Particle METunsmearedv = ev.GetMETVector();
  

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "MET_uncorr", METunsmearedv.Pt()  , w, 200, 0., 400.,"MET GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_bjetsL", NBJetsL , w, 5, 0., 5., "N_{b jets}");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_bjetsM", NBJetsM , w, 5, 0., 5., "N_{b jets}");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_bjetsT", NBJetsT , w, 5, 0., 5., "N_{b jets}");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_DeepJet_bjetsL", NBJetsDJL , w, 5, 0., 5., "N_{b jets}");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_DeepJet_bjetsM", NBJetsDJM , w, 5, 0., 5., "N_{b jets}");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_DeepJet_bjetsT", NBJetsDJT , w, 5, 0., 5., "N_{b jets}");

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "M_ll",  llCand.M(), w, 400, 0., 2000., "M_{ll} GeV");
  
  //  return;

  float mindRlepj1(99999.);
  float mindRlepj2(99999.);

  for(unsigned int i=0; i < jets.size(); i++){
    if(i == 0)     if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Jet_0_pt",  jets[i].Pt() , w, 400, 0., 2000., "AK4 Jet p_{T} GeV");
    

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DeepCSV_score", jets[i].GetTaggerResult(JetTagging::DeepCSV), w, 100, -1, 1., "DeepCSV_score");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DeepJet_score", jets[i].GetTaggerResult(JetTagging::DeepJet), w, 100, -1, 1., "DeepCSV_score");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Jet_pt",  jets[i].Pt() , w, 400, 0., 2000., "AK4 Jet p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Jet_eta",  jets[i].Eta() , w, 100, -5., 5., "AK4 Jet #eta ");

    if(jets[i].DeltaR(*leps[0] ) < mindRlepj1) mindRlepj1=jets[i].DeltaR(*leps[0] );
    if(jets[i].DeltaR(*leps[1] ) < mindRlepj2) mindRlepj2=jets[i].DeltaR(*leps[1] );
    
    if(i == 0){
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_jet1_lep1",  jets[i].DeltaR(*leps[0] ) , w, 50, 0., 5., "#Delta R(l1,j)") ;
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_jet1_lep2",  jets[i].DeltaR(*leps[1] ) , w, 50, 0., 5., "#Delta R(l2,j)"); 
    }
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_jet_lep1",  jets[i].DeltaR(*leps[0] ) , w, 50, 0., 5., "#Delta R(l1,j)") ;
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_jet_lep2",  jets[i].DeltaR(*leps[1] ) , w, 50, 0., 5., "#Delta R(l2,j)");
    
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_jet_MET",  jets[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(j,met)");
  }


  for(unsigned int i=0; i < jets_vbf.size(); i++){
    if(i == 0)     if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "VBFJet_0_pt",  jets_vbf[i].Pt() , w, 400, 0., 2000., "AK4 Jet p_{T} GeV");


    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DeepCSV_score_vbf", jets_vbf[i].GetTaggerResult(JetTagging::DeepCSV), w, 100, -1, 1., "DeepCSV_score");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "VBFJet_pt",  jets_vbf[i].Pt() , w, 400, 0., 2000., "AK4 Jet p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "VBFJet_eta",  jets_vbf[i].Eta() , w, 100, -5., 5., "AK4 Jet #eta ");

    if(i == 0){
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "VBF_dR_jet1_lep1",  jets_vbf[i].DeltaR(*leps[0] ) , w, 50, 0., 5., "#Delta R(l1,j)") ;
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "VBF_dR_jet1_lep2",  jets_vbf[i].DeltaR(*leps[1] ) , w, 50, 0., 5., "#Delta R(l2,j)");
    }
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "VBF_dR_jet_lep1",  jets_vbf[i].DeltaR(*leps[0] ) , w, 50, 0., 5., "#Delta R(l1,j)") ;
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "VBF_dR_jet_lep2",  jets_vbf[i].DeltaR(*leps[1] ) , w, 50, 0., 5., "#Delta R(l2,j)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"VBF_dR_jet_MET",  jets_vbf[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(j,met)");

    for(unsigned int j=0; j < jets_vbf.size(); j++){
      if(i==j)continue;
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"VBF_dR_jj",  jets_vbf[i].DeltaR(jets_vbf[i]) ,w, 50,  0., 5,"#DeltaR(j,j)");
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"VBF_M_jj",  (jets_vbf[i]+jets_vbf[i]).M() ,w, 200,  0., 2000,"M(j,j)");
    }
  }
 

  if(jets_vbf.size() > 1){

    double maxDiJetDeta=0.;
    int ijet1(-1), ijet2(-1);
    for(unsigned int ij = 0; ij < jets_vbf.size()-1; ij++){
      for(unsigned int ij2 = ij+1; ij2 < jets_vbf.size(); ij2++){
	
	double deta = fabs(jets_vbf[ij].Eta() - jets_vbf[ij2].Eta());
	if(deta > maxDiJetDeta) {
	  maxDiJetDeta=deta;
	  ijet1=ij;
	  ijet2=ij2;
	}
      }
    }
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "MaxDEta_jet1_jet2", maxDiJetDeta  , w, 200, 0., 10., "Max DEta");
    Particle JJMEta = jets_vbf[ijet1] + jets_vbf[ijet2];
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "MaxDEtaJets_MJJ",JJMEta.M()   , w, 200, 0., 2000., "MaxDEta MJJ");
    
    double Av_JetEta= 0.5*(jets_vbf[ijet1].Eta()+ jets_vbf[ijet2].Eta());
    double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "MaxDEtaJets_zeppenfeld", zeppenfeld  , w, 200, 0., 2., "zeppenfeld");
  }

  if(jets_vbf.size()>1){

    Particle JJLead = jets_vbf[0] + jets_vbf[1];
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Lead_MJJ",JJLead.M()   , w, 200, 0., 2000., "Lead MJJ");
    
    double maxDiJetDeta=fabs(jets_vbf[0].Eta() - jets_vbf[1].Eta());

    double Av_JetEta= 0.5*(jets_vbf[0].Eta()+ jets_vbf[1].Eta());
    double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Lead_zeppenfeld", zeppenfeld  , w, 200, 0., 2., "zeppenfeld");    
  }



  //////

  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  double ll_deta = fabs((*leps[0]).Eta() - (*leps[1]).Eta());
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dPhi_lep1_lep2", ll_dphi  , w, 200, -5., 5., "#Delta #Phi(l1,l2)") ;
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dEta_lep1_lep2", ll_deta  , w, 200, -5., 5., "#Delta #Phi(l1,l2)") ;

  
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dRmin_jet_lep1", mindRlepj1  , w, 50, 0., 5., "#Delta R(l1,j)") ;
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dRmin_jet_lep2", mindRlepj2  , w, 50, 0., 5., "#Delta R(l1,j)") ;

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Sum_lep_eta", (fabs(leps[0]->Eta())+  fabs(leps[1]->Eta())) , w, 50, 0., 5.);
  if(fabs(leps[0]->Eta()) > fabs(leps[1]->Eta()))    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Max_lep_eta", fabs(leps[0]->Eta()) , w, 50, 0., 2.5);
  else  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Max_lep_eta", fabs(leps[1]->Eta()) , w, 50, 0., 2.5);
  
  

  for(unsigned int i=0; i < fatjets.size(); i++){
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "AK8Jet_pt",  fatjets[i].Pt() , w, 100, 0., 2000., "AK8 Jet p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "AK8Jet_sdmass",  fatjets[i].SDMass() , w, 100, 0., 500., "Mass_{softdrop} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "AK8Jet_tau21",  fatjets[i].PuppiTau2()/ fatjets[i].PuppiTau1() , w, 400, 0., 2., "#tau_{21}");
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_AK8Jet_lep1",  fatjets[i].DeltaR(*leps[0] ) , w, 50, 0., 5., "#Delta R(l1,fj)") ;
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_AK8Jet_lep2",  fatjets[i].DeltaR(*leps[1] ) , w, 50, 0., 5., "#Delta R(l2,fj)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_AK8Jet_MET",  fatjets[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(FJ,met)");

  }
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_l1_MET",  leps[0]->DeltaR(met) ,w, 50,  0., 5,"#DeltaR(l1,met)");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_l2_MET",  leps[1]->DeltaR(met) ,w, 50,  0., 5,"#DeltaR(l2,met)");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_ll", leps[0]->DeltaR(*leps[1] ) ,w, 50,  0., 5,"#DeltaR(l,l)");
  
  
  return;
}



void HNL_LeptonCore::FillEventCutflowAll(TString analysis_dir_name,TString histname, double weight, vector<TString> lables, TString label){
  FillEventCutflowDef(analysis_dir_name,histname, weight,lables, label);

  if(!IsData)FillEventCutflowDef(analysis_dir_name,histname+"_no_weight",1.,lables, label);

}
void HNL_LeptonCore::FillEventCutflowDef(TString analysis_dir_name,TString histname, double weight, vector<TString> lables, TString label){

  TH1D *this_hist = GetHist1D(analysis_dir_name+"/"+histname);


  cout << analysis_dir_name+"/"+histname << endl;

  if( !this_hist ){
    TString cf_name="FillEventCutflow";
    //if(histname.Contains("Syst")) cf_name= "FillEventCutflow_Syst";
    //if(histname.Contains("SR")) cf_name="FillEventCutflow_SR";
    if(histname.Contains("_massbinned")) cf_name="FillEventCutflow_MassBinned";
    
    if(!analysis_dir_name.Contains("ChannelCutFlow"))  cf_name = analysis_dir_name + "/"+cf_name;
    else cf_name = analysis_dir_name;

    this_hist = new TH1D(cf_name+"/"+histname, "", lables.size(), 0, lables.size());
    //    this_hist->SetDirectory(NULL);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,lables[i]);
    this_hist->SetDirectory(NULL);

    maphist_TH1D[analysis_dir_name+"/"+histname] = this_hist;
  }
  this_hist->Fill(label, weight);

}



void HNL_LeptonCore::FillEventCutflow(HNL_LeptonCore::SearchRegion sr, float event_weight, TString label,   TString analysis_dir_name){
  
  if (run_Debug) cout << "FillEventCutflow " << label << " " << analysis_dir_name <<  endl;

  vector<TString> labels;
  TString EVhitname("");
  
  
  if(sr==SR1 ){
    labels = {  "Presel", "AK8Jet", "SR1_Init",   "SR1_lep_charge",  "SR1_lep_pt", "SR1_dilep_mass" , "SR1_Wmass",  "SR1_MET" , "SR1_bveto" };
    EVhitname= "SR1";
  }
  
  if(sr==SR2){
    labels = {   "Presel", "NoAK8Jet", "PassVBF",       "SR2_lep_charge",       "SR2_lep_pt", "SR2_VBF",     "SR2_ht_lt1",      "SR2_bveto"};
    EVhitname= "SR2";
  }
  
  if( sr==SR3){
    labels = {  "Presel", "NoAK8Jet", "FailVBF",    "SR3_lep_charge" ,     "SR3_lep_pt",      "SR3_dilep_mass",      "SR3_jet",       "SR3_dijet",
		"SR3_Wmass", "SR3_J1Pt",     "SR3_MET",      "SR3_bveto"};
    EVhitname= "SR3";
  }
  if(sr==SR4){
    labels = {      "SR4_3lep",             "SR4_3lep_veto", "SR4_3lep_chargereq",   "SR4_3lep_bjet",      "SR4_3lep_Zmlll",       "SR4_3lep_Zmll_os",      "SR4_lll_mu"};
    EVhitname= "SR4";
  }
  if(sr==PreselSS || sr==Presel ){
    labels = {"NoCut", "METFilter", "Trigger", "Dilepton",
	      "SS_Dilep" ,"SS_lep_veto", "SS_Dilep_mass", "SS_Presel"};
    EVhitname = "SS_Presel";
  }
  if(sr==PreselOS || sr==Presel ){
    labels = {"NoCut", "METFilter", "Trigger", "Dilepton",
	      "OS_Dilep" ,"OS_lep_veto", "OS_Dilep_mass", "OS_Presel"};
    EVhitname ="OS_Presel";
  }
  if(sr==ChannelDepPresel){
    labels = {"MuMu_Presel","EE_Presel","EMu_Presel"};
    EVhitname ="ChannelDependantPresel";
  }
  if(sr==ChannelDepSR1){
    labels = {"MuMu_SR1","EE_SR1","EMu_SR1"};
    EVhitname ="ChannelDependantSR1";
  }
  if(sr==ChannelDepSR2){
    labels = {"MuMu_SR2","EE_SR2","EMu_SR2"};
    EVhitname ="ChannelDependantSR2";
  }

  if(sr==ChannelDepSR3){
    labels = {"MuMu_SR3","EE_SR3","EMu_SR3"};
    EVhitname ="ChannelDependantSR3";
  }

  if(sr==ChannelDepSR3){
    labels = {"MuMu_SR3Fail_0j","MuMu_SR3Fail_1j","MuMu_SR3Fail_2j", "EE_SR3Fail_0j","EE_SR3Fail_1j","EE_SR3Fail_2j","EMu_SR3Fail_0j","EMu_SR3Fail_1j","EMu_SR3Fail_2j"};
    EVhitname ="ChannelDependantSR3Fail";
  }

  if(sr==SR){
    labels = {"SR1Pass","SR1Fail","SR2Pass","SR2Fail","SR3Pass","SR3Fail","SR4Pass","SR4Fail"
    };
    EVhitname ="SR_Summary";
  }

  if(sr==sigmm){
    labels = {"SSNoCut", "SSGen", "SSGen2", "SSMuMuTrig", "SSMuMuTrig2", "SSMuMuTrig2L", "SSMuMu", "SSMuMu_Pt", "SSMuMu_HEMVeto","SSMuMu_LepVeto", "SSMuMu_LLMass",  "SSMuMu_vTau","SSMuMu_Jet","SSMuMu_BJet", "SSMuMu_DiJet",  "SSMuMu_SR1","SSMuMu_SR1Fail", "SSMuMu_SR2","SSMuMu_SR3","SSMuMu_SR4","SSMuMu_SR5","SSMuMu_SR3Fail"};
    EVhitname ="SR_Summary";
  }

  if(sr==sigmm_17028){
    labels = {"SSNoCut", "SSGen","SSGen2", "SSMuMuTrig", "SSMuMuTrig2","SSMuMuTrig2L" , "SSMuMu","SSMuMu_Pt", "SSMuMu_HEMVeto","SSMuMu_LepVeto", "SSMuMu_LLMass", "SSMuMu_Jet", "SSMuMu_BJet", "SSMuMu_DiJet",  "SSMuMu_SR1","SSMuMu_SR1Fail","SSMuMu_SR3","SSMuMu_SR3Fail"};
    EVhitname ="SR_Summary_17028";
  }
  
  if(sr==sigee){
    labels = {"SSNoCut", "SSGen", "SSGen2", "SSEETrig", "SSEETrig2", "SSEETrig2L","SSEE", "SSEE_Pt","SSEE_LepVeto", "SSEE_HEMVeto","SSEE_LLMass",  "SSEE_vTau","SSEE_Jet","SSEE_BJet", "SSEE_DiJet",  "SSEE_SR1","SSEE_SR1Fail", "SSEE_SR2","SSEE_SR3","SSEE_SR4","SSEE_SR5","SSEE_SR3Fail"};
    EVhitname ="SR_Summary";
  }

  if(sr==sigee_17028){
    labels = {"SSNoCut", "SSGen","SSGen2", "SSEETrig", "SSEETrig2", "SSEETrig2L", "SSEE","SSEE_Pt", "SSEE_HEMVeto", "SSEE_LepVeto", "SSEE_LLMass", "SSEE_Jet", "SSEE_BJet", "SSEE_DiJet",  "SSEE_SR1","SSEE_SR1Fail","SSEE_SR3","SSEE_SR3Fail"};
    EVhitname ="SR_Summary_17028";
  }



  if(sr==sigem){
    labels = {"SSNoCut", "SSGen", "SSGen2", "SSEMuTrig", "SSEMuTrig2","SSEMuTrig2L","SSEMu", "SSEMu_Pt","SSEMu_LepVeto", "SSEMu_LLMass",  "SSEMu_vTau","SSEMu_Jet","SSEMu_BJet", "SSEMu_DiJet",  "SSEMu_SR1","SSEMu_SR1Fail", "SSEMu_SR2","SSEMu_SR3","SSEMu_SR4","SSEMu_SR5","SSEMu_SR3Fail"};
    EVhitname ="SR_Summary";
  }

  if(sr==sigem_17028){
    labels = {"SSNoCut", "SSGen","SSGen2", "SSEMuTrig", "SSEMuTrig2", "SSEMuTrig2L", "SSEMu","SSEMu_Pt", "SSEMu_LepVeto", "SSEMu_LLMass", "SSEMu_Jet", "SSEMu_BJet", "SSEMu_DiJet",  "SSEMu_SR1","SSEMu_SR1Fail","SSEMu_SR3","SSEMu_SR3Fail"};
    EVhitname ="SR_Summary_17028";
  }





  HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,EVhitname, event_weight, labels, label);


  return;
}      
 
void HNL_LeptonCore::FillEventCutflowPerMass(TString analysis_dir_name,SearchRegion sr, float event_weight, TString label,   TString massname){

  if(sr==SR3 ){

    vector<TString> labels = {
      "SR3_default",
      "SR3_jetsize",
      "SR3_jetpt",
      "SR3_W_DR_lep2",
      "SR3_lepPt1",
      "SR3_lepPt2",
      "SR3_Wmass_Low",
      "SR3_Wmass_High",
      "SR3_lljj",
      "SR3_Nmass_Low",
      "SR3_Nmass_High",
      "SR3_MET"};

    if(run_Debug)  cout << "FillEventCutflowAll : " << analysis_dir_name+"_SR3" << " " << label << endl;
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,"Massopt_"+massname, event_weight, labels, label);
  }



  if(sr== SR1 ){

    vector<TString> labels = {
      "SR1_default",
      "SR1_lepPt1",
      "SR1_lepPt2",
      "SR1_SDmass_Low",
      "SR1_SDmass_High",
      "SR1_Nmass_Low",
      "SR1_Nmass_High",
      "SR1_MET"};

    if(run_Debug)  cout << "FillEventCutflowAll : " << analysis_dir_name+"_SR1" << " " << label << endl;
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,"Massopt_"+massname, event_weight, labels, label);
  }


  return;

}

 
void HNL_LeptonCore::FillEventCutflowSR(TString analysis_dir_name,HNL_LeptonCore::SearchRegion sr, float event_weight, TString label){                                                            

  vector<TString> masses = {"100","125","200","250","300","400","500","600","700","800","900","1000","1100","1200","1300","1400","1500","1700","2000","2500","5000","20000"};
  
  

  if(sr== SR1){
    vector<TString> labels;
    for (unsigned int x=0; x < masses.size(); x++) labels.push_back("HNL_SR1_mn"+masses[x] );
    TString histname = "SR1_massbinned";  
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,histname, event_weight, labels, label);
  }

  if(sr== SR2){
    vector<TString> labels;
    for (unsigned int x=0; x < masses.size(); x++) labels.push_back("HNL_SR2_mn"+masses[x] );
    TString histname = "SR2_massbinned";
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,histname, event_weight, labels, label);
  }
  if(sr == SR3){
    vector<TString> labels;
    for (unsigned int x=0; x < masses.size(); x++) labels.push_back("HNL_SR3_mn"+masses[x] );
    TString histname = "SR3_massbinned";
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,histname, event_weight, labels, label);
    
  }
  
  return;
  
}  

void HNL_LeptonCore::FillFullTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){

  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D("FillEventFullType/"+histname, "", lables.size(), 0, lables.size(),  lables.size(), 0, lables.size());
    this_hist->SetDirectory(NULL);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,lables[i]);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetYaxis()->SetBinLabel(i+1,lables[i]);

    maphist_TH2D[histname] = this_hist;
  }

  this_hist->Fill(label1, label2, fabs(weight));

}


void HNL_LeptonCore::FillTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){

  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D("CR2DCutflow/"+histname, "", lables.size(), 0, lables.size(),  lables.size(), 0, lables.size());
    this_hist->SetDirectory(NULL);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,lables[i]);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetYaxis()->SetBinLabel(i+1,lables[i]);

    maphist_TH2D[histname] = this_hist;
  }

  this_hist->Fill(label1, label2, fabs(weight));

}


void HNL_LeptonCore::FillCutFlow(bool IsCentral, TString suffix, TString histname, double ev_weight){
  
  if(IsCentral){
    
    FillHist(suffix+"/"+histname, 0., ev_weight, 1, 0., 1.);
    
  }
  
}


TString HNL_LeptonCore::GetChannelString(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType q){

  TString channel_string="";
  if (channel == EE) channel_string="EE";
  if (channel == MuMu) channel_string="MuMu";
  if (channel == EMu) channel_string="EMu";
  
  if (channel == EEE) channel_string="EEE";
  if (channel == EMuL) channel_string="EMuL";
  if (channel == MuMuMu) channel_string="MuMuMu";

  
  if (channel == EEEE) channel_string="EEEE";
  if (channel == MuMuMuMu) channel_string="MuMuMuMu";
  if (channel == EMuLL) channel_string="EMuLL";


  if (q == Plus) channel_string+="_+";
  else if (q == Minus) channel_string+="_-";
  else   return channel_string;

  return channel_string;
}

TString HNL_LeptonCore::QToString(HNL_LeptonCore::ChargeType q){

  if (q==Plus) return "Plus";
  if (q==Minus) return "Minus";
  else return "";
  
}

vector<Gen> HNL_LeptonCore::GetGenLepronsSignal(){

  bool isDYVBF=false;
  vector<Gen> gens= GetGens();
  vector<Gen> gen_lep;
  int N_Mother(0);

  for (auto i : gens){ Gen gen = i; if(gen.PID() == 9900012 || gen.PID() == 9900014) isDYVBF=true; }

  if(isDYVBF){
    
    for(unsigned int i=2; i<gens.size(); i++){

      Gen gen = gens.at(i);

      if((gen.PID() == 9900012 || gen.PID() == 9900014)  && gen.Status()==22) {
        N_Mother= gen.MotherIndex();
      }
    }

    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);     TString lep_ch="";

      if((gens.at(gen.MotherIndex()).PID() == 9900012|| gens.at(gen.MotherIndex()).PID() == 9900014) && !(gens.at(i).PID() == 9900012 || gens.at(i).PID() == 9900014)){

        if(fabs(gen.PID()) == 15)     gen_lep.push_back(gen);

        if(fabs(gen.PID()) == 13)     gen_lep.push_back(gen);
        if(fabs(gen.PID()) == 11)     gen_lep.push_back(gen);

      }
      else if(gen.MotherIndex() == N_Mother&& !(gens.at(i).PID() == 9900012 || gens.at(i).PID() == 9900014)){


        if(fabs(gen.PID()) == 15) gen_lep.push_back(gen);

        if(fabs(gen.PID()) == 13) gen_lep.push_back(gen);
        if(fabs(gen.PID()) == 11) gen_lep.push_back(gen);
      }
    }
  }
  
  
  else{

    
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      if (fabs(gen.PID()) == 13 && gen.Status() == 23) gen_lep.push_back(gen);
      if (fabs(gen.PID()) == 11 && gen.Status() == 23) gen_lep.push_back(gen);
      if (fabs(gen.PID()) == 15 && gen.Status() == 23)gen_lep.push_back(gen);
    }
  }
  
  return gen_lep;
}

TString HNL_LeptonCore::GetProcess(){

  int N_Mother(0);

  //  cout << "index\tPID\tStatus\tMIdx\tMPID\tStart\tPt\tEta\tPhi\tM" << endl;                                                                                                                             

  bool isDYVBF=false;
  vector<Gen> gens= GetGens();

  for (auto i : gens){ Gen gen = i; if(gen.PID() == 9900012 || gen.PID() == 9900014) isDYVBF=true; }

  if(isDYVBF){

    bool isVBF=false;
    
    for (auto i : gens){
      Gen gen = i;
      if (gen.PID() == 22 && gen.Status() == 21) isVBF=true;
    }

    

    for(unsigned int i=2; i<gens.size(); i++){

      Gen gen = gens.at(i);

      if((gen.PID() == 9900012 || gen.PID() == 9900014)  && gen.Status()==22) {
        N_Mother= gen.MotherIndex();
      }
    }

    int lep_1_ch=-999;
    int lep_2_ch=-999;
    int lep_3_ch=-999;

    TString lep1_s="", lep2_s="", lep3_s="";
    TString lep1_ss="", lep2_ss="", lep3_ss="";

    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);     TString lep_ch="";

      if((gens.at(gen.MotherIndex()).PID() == 9900012|| gens.at(gen.MotherIndex()).PID() == 9900014) && !(gens.at(i).PID() == 9900012 || gens.at(i).PID() == 9900014)){
        //if(fabs(gen.PID())  < 16 && fabs(gen.PID())  > 10) lep_2_ch = (gen.PID() < 0) ? 1 : -1;

        if(fabs(gen.PID()) == 15) {
	  lep2_s="Tau";
	  if (gen.PID() <0) lep2_ss=lep2_s+"+";
	  else lep2_ss=lep2_s+"-";
	  lep_2_ch = (gen.PID() < 0) ? 1 : -1;
	}
        if(fabs(gen.PID()) == 13) {
	  lep2_s="Mu";
	  if (gen.PID() <0) lep2_ss=lep2_s+"+";
          else lep2_ss=lep2_s+"-";
	  lep_2_ch = (gen.PID() < 0) ? 1 : -1;
	}
        //if(fabs(gen.PID()) == 14) {lep2_s="Mu";lep2_s+=lep_ch;}
        if(fabs(gen.PID()) == 11) {
	  lep2_s="El";
          if (gen.PID() <0) lep2_ss=lep2_s+"+";
          else lep2_ss=lep2_s+"-";
	  lep_2_ch = (gen.PID() < 0) ? 1 : -1;
	}
      }
      else if(gen.MotherIndex() == N_Mother&& !(gens.at(i).PID() == 9900012 || gens.at(i).PID() == 9900014)){

        //if(fabs(gen.PID())  < 16 && fabs(gen.PID())  > 10)  lep_1_ch = (gen.PID() < 0) ? 1 : -1;

	
        if(fabs(gen.PID()) == 15) {
	  lep1_s="Tau"; 
	  if (gen.PID() < 0) lep1_ss=lep1_s+"+";
	  else lep1_ss=lep1_s+"-";
	  lep_1_ch = (gen.PID() < 0) ? 1 : -1;    
	}
        if(fabs(gen.PID()) == 13) {
	  lep1_s="Mu";
	  if (gen.PID() < 0) lep1_ss=lep1_s+"+";
          else lep1_ss=lep1_s+"-";
	  lep_1_ch = (gen.PID() < 0) ? 1 : -1;    
	}
        if(fabs(gen.PID()) == 11) {
	  lep1_s="El";
	  if (gen.PID() < 0) lep1_ss=lep1_s+"+";
          else lep1_ss=lep1_s+"-";
	  lep_1_ch = (gen.PID() < 0) ? 1 : -1;    
	}
      }
    }
    if(lep_1_ch == lep_2_ch) return ("SS_"+ lep1_ss+lep2_ss);
    else return ("OS_"+ lep1_s + lep2_s);

  }

  else{
    
    
    TString pr="SS_";
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      if (fabs(gen.PID()) == 13 && gen.Status() == 23) {
	pr=pr+"Mu";
	if (gen.PID() < 0) pr = pr+"+";
	else pr = pr+"-";
      }
      if (fabs(gen.PID()) == 11 && gen.Status() == 23) {
	pr=pr+"El";
	if (gen.PID() < 0) pr = pr+"+";
	else pr = pr+"-";
      }
      if (fabs(gen.PID()) == 15 && gen.Status() == 23){
	pr=pr+"Tau";
	if (gen.PID() < 0) pr = pr+"+";
	else pr = pr+"-";
      }
    }
    return pr;
  }
  return "TMP";

}

bool HNL_LeptonCore::HasLowMassMeson(std::vector<Lepton *> leps){

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() == leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if(fabs(ll.M() - 3.10) < 0.08) return true;
      if(fabs(ll.M() - 3.69) < 0.08) return true;
      if(fabs(ll.M() - 0.78) < 0.08) return true;
      if(fabs(ll.M() - 1.02) < 0.08) return true;
      if(fabs(ll.M() - 3.10) < 0.08) return true;
      if(fabs(ll.M() - 9.46) < 0.08) return true;
      if(fabs(ll.M() - 10.02) < 0.08) return true;
      if(fabs(ll.M() - 10.36) < 0.08) return true;
    }
  }


  return false;

}



std::vector<Jet> HNL_LeptonCore::SelBJets(std::vector<Jet>& jetColl, JetTagging::Parameters jtp){

  std::vector<Jet> bjetColl;
  for(unsigned int i=0; i<jetColl.size(); i++){
    if( jetColl.at(i).GetTaggerResult(jtp.j_Tagger) <= mcCorr->GetJetTaggingCutValue(jtp.j_Tagger, jtp.j_WP) ) continue;
    bjetColl.push_back( jetColl.at(i) );
  }
  return bjetColl;
}


Particle HNL_LeptonCore::GetvMET(TString METType){

  bool IsType1   = METType.Contains("T1");
  bool IsxyCorr  = METType.Contains("xyCorr");
  bool UsePuppi  = METType.Contains("Puppi");

  Particle vMET;
  if(UsePuppi){
    if(IsType1){
      if(IsxyCorr) vMET.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.);
      else         vMET.SetPtEtaPhiM(PuppiMET_Type1_pt, 0., PuppiMET_Type1_phi, 0.);
    }
  }
  else{
    if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt, 0., pfMET_Type1_PhiCor_phi, 0.); 
    else         vMET.SetPtEtaPhiM(pfMET_Type1_pt, 0., pfMET_Type1_phi, 0.); 
  }
  return vMET;

}

Particle HNL_LeptonCore::GetvMET(TString METType, AnalyzerParameter param){

  bool IsType1   = METType.Contains("T1");
  bool IsxyCorr  = METType.Contains("xyCorr");
  bool UsePuppi  = METType.Contains("Puppi");

  bool IsJetSmear  = METType.Contains("JetSmear");

  int IdxSyst = -1;

  if(param.syst_ == AnalyzerParameter::METUnclUp)   IdxSyst = 10;
  if(param.syst_ == AnalyzerParameter::METUnclDown)   IdxSyst = 11;

  // Use CMSSW MET SYSTa
  if(METType.Contains("CMSSW")){
    if(param.syst_ == AnalyzerParameter::JetEnUp)  IdxSyst = 2;
    if(param.syst_ == AnalyzerParameter::JetEnDown)  IdxSyst = 3;
    if(param.syst_ == AnalyzerParameter::JetResUp)  IdxSyst = 0;
    if(param.syst_ == AnalyzerParameter::JetResDown)  IdxSyst = 1;
  }
  bool ApplySyst = (!IsDATA) && IdxSyst >= 0;

  Particle vMET;

  if(UsePuppi){
    if(IsType1){
      if( (!ApplySyst) ){
	if(IsxyCorr) vMET.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.); 
	else         vMET.SetPtEtaPhiM(PuppiMET_Type1_pt, 0., PuppiMET_Type1_phi, 0.); 
      }
      else{
	if(IsxyCorr)  vMET.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.);
	else         vMET.SetPtEtaPhiM(PuppiMET_Type1_pt_shifts->at(IdxSyst), 0.,  PuppiMET_Type1_phi_shifts->at(IdxSyst), 0.); 
      }
    }
    
  }
  else{
    if(IsType1){
      if( (!ApplySyst) or ( IdxSyst>=0 && (!isfinite(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst))) ) ){
	if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt, 0., pfMET_Type1_PhiCor_phi, 0.); 
	else         vMET.SetPtEtaPhiM(pfMET_Type1_pt, 0., pfMET_Type1_phi, 0.); 
      }
      else{
	if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst), 0., pfMET_Type1_PhiCor_phi_shifts->at(IdxSyst), 0.); 
	else         vMET.SetPtEtaPhiM(pfMET_Type1_pt_shifts->at(IdxSyst), 0., pfMET_Type1_phi_shifts->at(IdxSyst), 0.); 
      }
    }

  }

  if (!IsJetSmear && param.syst_ == AnalyzerParameter::Central) return vMET;
  
  Particle vMETSmeared;
  if (IsJetSmear) vMETSmeared = UpdateMETSmearedJet(vMET, GetJets(param, param.Jet_ID, 10., 5.));

  if(param.syst_ == AnalyzerParameter::Central) return vMETSmeared;
  double MET = vMETSmeared.Pt();
  double METPhi = vMETSmeared.Pt();
  //if(param.syst_ == AnalyzerParameter::JetResUp ) CorrectedMETJER(1, GetJets(param, 10., 5.), GetFatJets(param, 200., 5.), MET,METPhi);
  //if(param.syst_ == AnalyzerParameter::JetResDown) CorrectedMETJER(-11, GetJets(param, 10., 5.), GetFatJets(param, 200., 5.), MET,METPhi);
  // Write CorrectedMETXXhttps://github.com/jedori0228/LQanalyzer/blob/CatAnalyzer_13TeV_v8-0-7.36_HNAnalyzer/LQAnalysis/Analyzers/src/AnalyzerCore.cc#L4826

  Particle vMETSyst;
  vMETSyst.SetPtEtaPhiM(MET, 0., METPhi, 0.);

  
  return vMETSyst;
}





int HNL_LeptonCore::GetIndexNonMinOSSF(std::vector<Lepton *> leps){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]) + (*leps[1]);
    Particle ll2 = (*leps[0]) + (*leps[2]);
    Particle ll3 = (*leps[1]) + (*leps[2]);

    float minOS=99999999999.;
    if(ll1.Charge() == 0) {
      if(ll1.M()  < minOS) { minOS=ll1.M(); index=2;}
    }
    if(ll2.Charge() == 0) {
      if(ll2.M()  < minOS) { minOS=ll2.M(); index=1;}
    }
    if(ll3.Charge() == 0) {
      if(ll3.M()  < minOS) { minOS=ll3.M(); index=0;}
    }

  }

  return index;

}



double  HNL_LeptonCore::GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets){

  if (type=="HNL_SR3"){

    if(jets.size() < 2) return 0.;
    float dijetmass_tmp=9999.;
    float dijetmass=99990000.;
    int m=-999;
    int n=-999;
    double ST(0.);
    for(UInt_t emme=0; emme<jets.size(); emme++){
      ST += jets[emme].Pt();
      for(UInt_t enne=1; enne<jets.size(); enne++) {

        dijetmass_tmp = (jets[emme]+jets[enne]).M();
        if(emme == enne) continue;

        if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
          dijetmass = dijetmass_tmp;
          m = emme;
          n = enne;
        }
      }
    }
    Particle Wcand = jets[m] + jets[n];
    return Wcand.M();
  }

  if (type=="HNL_SR1"){
    if(fatjets.size() ==0 )  return 0.;
    float dijetmass_tmp=999.;
    float dijetmass=9990000;
    int m=-999;
    for(UInt_t emme=0; emme<fatjets.size(); emme++){
      dijetmass_tmp= fatjets[emme].SDMass();
      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
      }
    }
    return fatjets[m].SDMass();
  }
  cout << "[HNL_LeptonCore::GetMass ] ID not found.." << endl;
  exit(EXIT_FAILURE);

  return -9999.;
}



float HNL_LeptonCore::GetLT(std::vector<Lepton *> leps){
  
  float lt(0.);
  for(auto ilep : leps) lt +=  ilep->Pt();

  return lt;

}

float HNL_LeptonCore::GetLLMass(std::vector<Muon> leps){
  
  return GetLLMass(MakeLeptonPointerVector(leps));
}

float HNL_LeptonCore::GetLLMass(std::vector<Electron> leps){

  return GetLLMass(MakeLeptonPointerVector(leps));
}

float HNL_LeptonCore::GetLLMass(std::vector<Lepton *> leps){

  if(leps.size() != 2) return -1.;
  
  Particle ll = (*leps[0]) + (*leps[1]);
  return ll.M();
}

float HNL_LeptonCore::GetMassMinOSSF(std::vector<Lepton *> leps){


  float minOS=99999999999.;

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() == leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if(ll.M() < minOS) minOS=ll.M();
    }
  }



  return minOS;

}

float  HNL_LeptonCore::GetMassMinSSSF(std::vector<Lepton *> leps){

  float minSS=99999999999.;

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() != leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if(ll.M() < minSS) minSS=ll.M();
    }
  }
  if(minSS < 0 ) cout << "minSS = " << minSS << " " << endl;
  return minSS;

}



bool  HNL_LeptonCore::ZmasslllWindowCheck(std::vector<Lepton *> leps){

  if (leps.size() == 3){

    Particle lll = *leps[0] + *leps[1]+ *leps[2];

    bool passZmass_lll_Window = (fabs(lll.M() - 90.1) < 15.);
    return passZmass_lll_Window;

  }

  return true;
}


int HNL_LeptonCore::GetIndexNonMinSSSF(std::vector<Lepton *> leps){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]) + (*leps[1]);
    Particle ll2 = (*leps[0]) + (*leps[2]);
    Particle ll3 = (*leps[1]) + (*leps[2]);

    float minOS=99999999999.;
    if(fabs(ll1.Charge()) == 2) {
      if(ll1.M()  < minOS) { minOS=ll1.M(); index=2;}
    }
    if(fabs(ll2.Charge()) == 2) {
      if(ll2.M()  < minOS) { minOS=ll2.M(); index=1;}
    }
    if(fabs(ll3.Charge()) == 2) {
      if(ll3.M()  < minOS) { minOS=ll3.M(); index=0;}
    }

  }

  return index;

}

int HNL_LeptonCore::GetIndexNonBestZ(std::vector<Lepton *> leps,double mass_diff){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]) + (*leps[1]);
    Particle ll2 = (*leps[0]) + (*leps[2]);
    Particle ll3 = (*leps[1]) + (*leps[2]);

    float minOSZ=99999999999.;
    if(fabs(ll1.Charge()) == 0) {
      if(fabs(ll1.M() - 90.1) < minOSZ) {minOSZ = fabs(ll1.M() - 90.1); index=2;}
    }
    if(fabs(ll2.Charge()) == 0) {
      if(fabs(ll2.M() - 90.1) < minOSZ) {minOSZ = fabs(ll2.M() - 90.1);index=1;}

    }
    if(fabs(ll3.Charge()) == 0) {
      if(fabs(ll3.M() - 90.1) < minOSZ) {minOSZ = fabs(ll3.M() - 90.1);index=0;}
    }


    if(minOSZ < mass_diff) return index;
  }
  return -1;

}



bool  HNL_LeptonCore::ZmassOSSFWindowCheck(std::vector<Lepton *> leps, double mass_diff){

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() == leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if((fabs(ll.M() - 90.1) < mass_diff)) return true;
    }
  }

  return false;
}

float HNL_LeptonCore::GetMassBestZ(std::vector<Lepton *> leps,  bool bestZ){

  float minMZ = 99999999.;
  float massNonBestZ(-9999.), massBestZ(-9999);
  if(leps.size()==4){

    Particle Z1Cand;
    Particle Z2Cand;


    for(unsigned int iel =0; iel < leps.size() ; iel++){
      for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
        if(iel== iel2) continue;
        Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
        if(leps[iel]->Charge() != leps[iel2]->Charge()){

          int zel1(-9), zel2(-9);
          if(iel ==0 && iel2==1){ zel1=2; zel2=3;    Z2Cand = (*leps[2]) + (*leps[3]);}
          if(iel ==0 && iel2==2){ zel1=1; zel2=3;    Z2Cand = (*leps[1]) + (*leps[3]);}
          if(iel ==0 && iel2==3){ zel1=1; zel2=2;    Z2Cand = (*leps[1]) + (*leps[2]);}
          if(iel ==1 && iel2==2){ zel1=0; zel2=3;    Z2Cand = (*leps[0]) + (*leps[3]);}
          if(iel ==1 && iel2==3){ zel1=0; zel2=2;    Z2Cand = (*leps[0]) + (*leps[2]);}
          if(iel ==2 && iel2==3){ zel1=0; zel2=1;    Z2Cand = (*leps[0]) + (*leps[1]);}

          if(leps[zel1]->Charge() != leps[zel2]->Charge()){
            if(( fabs(Z1Cand.M()- 90.1) + fabs(Z2Cand.M()- 90.1)) < minMZ) {
              minMZ = fabs(Z1Cand.M()- 90.1) + fabs(Z2Cand.M()- 90.1);
              if(fabs(Z1Cand.M()- 90.1)< fabs(Z2Cand.M()- 90.1) ){ massBestZ=Z1Cand.M(); massNonBestZ=Z2Cand.M();}
              else{massBestZ=Z2Cand.M(); massNonBestZ=Z1Cand.M();}
            }
          }
        }
      }
    }


    if(bestZ) return massBestZ;
    return massNonBestZ;
  }
  Particle Z1Cand;

  for(unsigned int iel =0; iel < leps.size() ; iel++){

    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
      if(leps[iel]->Charge() != leps[iel2]->Charge()){
        if(fabs(Z1Cand.M()- 90.1) < minMZ) {minMZ = fabs(Z1Cand.M()- 90.1) ; massBestZ=Z1Cand.M();}
      }
    }
  }
  return massBestZ;

}
bool  HNL_LeptonCore::ZmassOSWindowCheck(std::vector<Lepton *> leps){

  bool passZmass_os_Window=false;

  if (leps.size() == 3){


    Particle ll1 = *leps[0] + *leps[1];
    Particle ll2 = *leps[0] + *leps[2];
    Particle ll3 = *leps[1] + *leps[2];

    if(ll1.Charge() == 0 && (fabs(ll1.M() - 90.1) < 15.)) passZmass_os_Window=true;
    if(ll2.Charge() == 0 && (fabs(ll2.M() - 90.1) < 15.)) passZmass_os_Window=true;
    if(ll3.Charge() == 0 && (fabs(ll3.M() - 90.1) < 15.)) passZmass_os_Window=true;

  }

  return passZmass_os_Window;
}


double HNL_LeptonCore::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){


  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}

double HNL_LeptonCore::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev){

  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  Particle METUnsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  _st += METv.Pt();

  return _st;
}

double HNL_LeptonCore::GetST( std::vector<Lepton *> leps, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){


  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(auto ilep : leps) _st +=  ilep->Pt();
  _st += met.Pt();
  return _st;

}

double HNL_LeptonCore::GetST( std::vector<Lepton *> leps, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev){

  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(auto ilep: leps) _st +=ilep->Pt();

  Particle METUnsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  _st += METv.Pt();

  return _st;
}



double HNL_LeptonCore::M_T(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);

  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );

}

double HNL_LeptonCore::GetHT(std::vector<Jet> jets, std::vector<FatJet> fatjets){

  double _ht(0.);
  for(unsigned int i=0; i<jets.size(); i++)_ht += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_ht += fatjets.at(i).Pt();
  return _ht;

}



TString HNL_LeptonCore::GetPtLabel(Muon mu){


  if (mu.Pt() < 10.) return "pt_5_10";
  if (mu.Pt() < 15.) return "pt_10_15";
  if (mu.Pt() < 20.) return "pt_15_20";
  if (mu.Pt() < 30.) return "pt_20_30";
  if (mu.Pt() < 40.) return "pt_30_40";
  if (mu.Pt() < 50.) return "pt_40_50";
  if (mu.Pt() < 100.) return "pt_50_100";
  if (mu.Pt() < 2000.) return "pt_100_2000";

  return "";


}


TString HNL_LeptonCore::GetEtaLabel(Muon mu){
  if(fabs(mu.Eta()) < 0.8 ) return "eta1";
  if(fabs(mu.Eta()) < 1.5 ) return "eta2";
  if(fabs(mu.Eta()) < 2.5 ) return "eta3";
  return "";
}


TString HNL_LeptonCore::GetElType(Electron el, const std::vector<Gen>& gens){
  if(IsDATA) return "";
  TString tag = "";
  if(GetLeptonType(el, gens) > 0) tag += "NF";
  else tag += "F";

  if(fabs(GetLeptonType(el, gens))  >=4)  tag += "_Conv";
  if(IsCF(el, gens)) tag += "_CF";

  return tag;
}
TString HNL_LeptonCore::GetElTypeString(Electron el, const std::vector<Gen>& gens){
  if(IsDATA) return "";
  TString tag = "";
  if(GetLeptonType(el, gens) > 0) tag += "NonFake";
  else tag += "Fake";

  if(fabs(GetLeptonType(el, gens))  >=4)  tag += "_Conv";
  if(IsCF(el, gens)) tag += "_CF";

  return tag;
}

TString HNL_LeptonCore::GetElTypeTString(Electron el, const std::vector<Gen>& gens){
  if(IsDATA) return "";
  TString tag = "";

  if(GetLeptonType(el, gens) < 0) tag = "Minus";
  tag+=TString::Itoa(fabs(GetLeptonType(el, gens)), 10);
  return tag;
}

TString HNL_LeptonCore::GetLepTypeTString(const Lepton& lep, const std::vector<Gen>& gens){
  if(IsDATA) return "";
  TString tag = "";
  if(GetLeptonType(lep, gens) < 0) tag = "Minus";
  tag+=TString::Itoa(fabs(GetLeptonType(lep, gens)), 10);
  return tag;
}

bool HNL_LeptonCore::SameCharge(vector<Muon> mus, int ch){

  if(mus.size() != 2) return false;
 
  int sumQ=mus[0].Charge()+mus[1].Charge();
  if(ch==0){
    if(fabs(sumQ) == 2) return true;
    return false;
  }
  else if(ch==1){
    if(sumQ == 2) return true;
    return false;

  }
  else if(ch==-1){
    if(sumQ == -2) return true;
    return false;

  }
  return false;

}

bool HNL_LeptonCore::SameCharge(vector<Electron> els, int ch){

  if(els.size() != 2) return false;

  int sumQ=els[0].Charge()+els[1].Charge();
  if(ch==0){
    if(fabs(sumQ) == 2) return true;
    return false;
  }
  else if(ch==1){
    if(sumQ == 2) return true;
    return false;

  }
  else if(ch==-1){
    if(sumQ == -2) return true;
    return false;

  }
  return false;
}


bool HNL_LeptonCore::SameCharge(vector<Electron> els, vector<Muon> mus,int ch){

  if(els.size() != 1) return false;
  if(mus.size() != 1) return false;

  int sumQ=els[0].Charge()+mus[1].Charge();
  if(ch==0){
    if(fabs(sumQ) == 2) return true;
    return false;
  }
  else if(ch==1){
    if(sumQ == 2) return true;
    return false;

  }
  else if(ch==-1){
    if(sumQ == -2) return true;
    return false;

  }
  return false;
}


bool HNL_LeptonCore::SameCharge(std::vector<Lepton *> leps, int ch){

  if(leps.size() != 2) return false;

  int sumQ=leps[0]->Charge()+leps[1]->Charge();
  if(ch==0){
    if(fabs(sumQ) == 2) return true;
    return false;
  }
  else if(ch==1){
    if(sumQ == 2) return true;
    return false;

  }
  else if(ch==-1){
    if(sumQ == -2) return true;
    return false;

  }

  return false;
}


void HNL_LeptonCore::FillAllMuonPlots(TString label , TString cut,  std::vector<Muon> muons, float w){

  for(unsigned int i=0; i <  muons.size(); i++){

    TString mu_lab="muon1";
    if(i==1) mu_lab="muon2";
    if(i==2) mu_lab="muon3";

    TString eta_label="";
    if(fabs(muons.at(i).Eta()) < 1.5) eta_label = "_barrel";
    else eta_label = "_endcap";


    FillAllMuonPlots(mu_lab+label, cut, muons.at(i), w);
    FillAllMuonPlots(mu_lab+label+eta_label, cut, muons.at(i), w);

    FillAllMuonPlots("muon"+label, cut, muons.at(i), w);
    FillAllMuonPlots("muon"+label+eta_label, cut, muons.at(i), w);

    if(muons.at(i).MVA() < 0.5)     FillAllMuonPlots("muon"+label+eta_label+"_lowmva", cut, muons.at(i), w);
    else FillAllMuonPlots("muon"+label+eta_label+"_highmva", cut, muons.at(i), w);


  }

  return;
}


void HNL_LeptonCore::FillAllMuonPlots(TString label , TString cut,  Muon mu, float w){

  vector<Jet> JetAllColl = GetJets("NoID", 10., 3.0);
  int IdxMatchJet=-1;
  double mindR(999.);

  double PtRelv0(0.);
  double PtRelv1(0.);
  double PtRatio(1.);
  double jet_disc(-1);
  for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
    float dR=mu.DeltaR(JetAllColl.at(ij));
    if(dR>0.4) continue;
    if(dR<mindR){ mindR=dR; IdxMatchJet=ij; }
  }
  if(IdxMatchJet!=-1){
    PtRatio = mu.Pt()/JetAllColl.at(IdxMatchJet).Pt();
    TLorentzVector JetNoLep(JetAllColl.at(IdxMatchJet));
    JetNoLep -= mu;
    PtRelv0 = mu.Perp(JetAllColl.at(IdxMatchJet).Vect());
    PtRelv1 = mu.Perp(JetNoLep.Vect());
    jet_disc = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepCSV);

  }
  FillHist( cut+ "/ID_POGLoose_"+label, mu.PassID("POGLoose"), w, 2, 0., 2.);
  FillHist( cut+ "/ID_POGMedium_"+label, mu.PassID("POGMedium"), w, 2, 0., 2.);
  FillHist( cut+ "/ID_POGTight_"+label, mu.PassID("POGTight"), w, 2, 0., 2.);
  FillHist( cut+ "/ID_POGHighPt_"+label, mu.PassID("POGHighPt"), w, 2, 0., 2.);

  FillHist( cut+ "/ptrel_0_"+label , PtRelv0 , w, 200, 0., 20., "");
  FillHist( cut+ "/ptrel_1_"+label , PtRelv1 , w, 200, 0., 20., "");
  FillHist( cut+ "/ptrel_def_"+label , mu.lep_jet_ptrel() , w, 200, 0., 20., "");
  FillHist( cut+ "/ptratio_def_"+label , mu.lep_jet_ptratio() , w, 100, 0., 2., "");
  FillHist( cut+ "/ptratio_"+label , PtRatio , w, 100, 0., 2., "");
  FillHist( cut+ "/jet_disc_"+label , jet_disc , w, 400, -2., 2., "");

  double new_iso = mu.RelIso() / (PtRatio * PtRelv1);
  double new_miso = mu.MiniRelIso() / (PtRatio * PtRelv1);
  FillHist( cut+ "/new_iso_"+label , new_iso , fabs(w), 100000, 0., 1., "");
  FillHist( cut+ "/new_miso_"+label , new_miso , fabs(w), 100000, 0., 1., "");
  FillHist( cut+ "/ptratio_rel", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);

  if(mu.MiniRelIso() < 0.2)   FillHist( cut+ "/ptratio_rel_miso02", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);
  if(mu.MiniRelIso() < 0.1)   FillHist( cut+ "/ptratio_rel_miso01", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);
  if(mu.MiniRelIso() < 0.08)   FillHist( cut+ "/ptratio_rel_miso008", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);
  if(mu.MiniRelIso() < 0.05)   FillHist( cut+ "/ptratio_rel_miso005", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);
  if(mu.RelIso() < 0.2)   FillHist( cut+ "/ptratio_rel_iso02", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);
  if(mu.RelIso() < 0.1)   FillHist( cut+ "/ptratio_rel_iso01", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);
  if(mu.RelIso() < 0.08)   FillHist( cut+ "/ptratio_rel_iso008", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);
  if(mu.RelIso() < 0.05)   FillHist( cut+ "/ptratio_rel_iso005", PtRelv1, PtRatio, fabs(w), 200, 0., 20., 100, 0., 2.);

  FillHist( cut+ "/pt_mu_"+label , mu.Pt() , w, 500, 0., 1000., "muon p_{T} GeV");
  FillHist( cut+ "/dxy_mu_"+label , mu.dXY() , w, 500, -0.2, 0.2, "dXY");
  FillHist( cut+ "/dz_mu_"+label , mu.dZ() , w, 500, -0.5, 0.5, "dZ");
  FillHist( cut+ "/reliso_mu_"+label , mu.RelIso() , w, 50, 0., 1., "R_{ISO} GeV");
  FillHist( cut+ "/eta_"+label  , mu.Eta() , w, 60, -3., 3.,"muon #eta");
  FillHist( cut+ "/IP3D_"+label  , mu.IP3D()/mu.IP3Derr(), w, 400, -20., 20., "IP3D");

  FillHist( cut+ "/mva_"+label  , mu.MVA(), w, 400, -1., 1., "MVA");
  FillHist( cut+ "/pt_mva_"+label , mu.Pt() , mu.MVA(), fabs(w), 200, 0., 1000., 400, -1., 1.);

  FillHist( cut+ "/chi2_"+label  , mu.Chi2(), w, 1000,0., 100., "chi2");
  FillHist( cut+ "/validhits_"+label  , mu.ValidMuonHits(), w, 100,0., 100., "");
  FillHist( cut+ "/matched_stations_"+label  , mu.MatchedStations(), w, 10,0., 10., "");
  FillHist( cut+ "/pixel_hits_"+label  , mu.PixelHits(), w, 10,0., 10., "");
  FillHist( cut+ "/minireliso_mu_"+label , mu.MiniRelIso() , w, 50, 0., 1., "R_{ISO} GeV");
  FillHist( cut+ "/tracker_layers_"+label  , mu.TrackerLayers(), w, 50,0., 50., "");

  return;
}


void HNL_LeptonCore::FillAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, float w){

  FillHist( cut+ "/nelectrons"+label , size(els) , w, 5, 0., 5., "n_{el}");

  for(unsigned int i=0; i < els.size(); i++){
    TString el_lab="el1";
    if(i==1) el_lab="el2";
    if(i==2) el_lab="el3";
    if(!els[i].IsGsfCtfScPixChargeConsistent()){
      FillHist( cut+ "/pt_vetoel_cc_"+el_lab+label , els.at(i).Pt() , w, 500, 0., 1000., "el p_{T} GeV");

    }

    //FillAllElectronPlots(el_label+label, cut, el, w);                                                                                         

  }
  return;

}
