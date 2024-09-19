#include "WRTau_Analyzer.h"

void WRTau_Analyzer::initializeAnalyzer(){

  if(HasFlag("RunSyst") && !IsDATA) runSystematics = true;
  else runSystematics = false;

  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {5}; vEl_vec = {13}; vMu_vec = {21};
  if(HasFlag("TauFake")){
    vJet_vec = {0};
    runTauFake = true;
  }
  if(HasFlag("LooseTauPrompt")) vJet_vec = {0};
  if(HasFlag("RunApplicationRegion")) vJet_vec = {0};

  GetTauIDSFTools(vJet_vec,vEl_vec,vMu_vec);

  if(DataYear==2016){
    SingleTauTriggers = { "HLT_VLooseIsoPFTau140_Trk50_eta2p1_v" };
    TriggerSafeTauPtCut = 150.;
  }

  if(DataYear==2017){
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
    TriggerSafeTauPtCut = 190.;
  }

  if(DataYear==2018){
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
    TriggerSafeTauPtCut = 190.;
  }

  RegionOfInterest = {
                      WRTau_Core::BoostedPreselection,
                      WRTau_Core::ResolvedPreselection,
                      WRTau_Core::BoostedLowMassControlRegion,
                      WRTau_Core::ResolvedLowMassControlRegion,
                      WRTau_Core::BoostedSignalRegion,
                      WRTau_Core::ResolvedSignalRegion,
                      WRTau_Core::ResolvedSignalRegionMETInvert,
                      WRTau_Core::BoostedSignalRegionMETInvert,
                      //WRTau_Core::BenchmarkResolvedPreselection,
                      //WRTau_Core::BenchmarkBoostedPreselection,
                      WRTau_Core::ResolvedSignalRegionMETInvertMTSame,
                      WRTau_Core::BoostedSignalRegionMETInvertMTSame
                      };

  if(HasFlag("RunApplicationRegion")){
    RegionOfInterest = { 
                      WRTau_Core::BoostedLowMassControlRegion,
                      WRTau_Core::ResolvedLowMassControlRegion,
                      WRTau_Core::ResolvedSignalRegionMETInvertMTSame,
                      WRTau_Core::BoostedSignalRegionMETInvertMTSame,
                      WRTau_Core::BoostedSignalRegion,
                      WRTau_Core::ResolvedSignalRegion};
  }

  if(HasFlag("TauFake")){
      RegionOfInterest = {
                      WRTau_Core::BoostedSignalRegion,
                      WRTau_Core::ResolvedSignalRegion,
                      WRTau_Core::BoostedLowMassControlRegion,
                      WRTau_Core::ResolvedLowMassControlRegion,
                      WRTau_Core::ResolvedSignalRegionMETInvertMTSame,
                      WRTau_Core::BoostedSignalRegionMETInvertMTSame
                      };
  }

  if(HasFlag("2DScan")){

    RegionOfInterest.clear();
    RegionOfInterest = {
      WRTau_Core::BenchmarkResolvedPreselection,
      WRTau_Core::BenchmarkBoostedPreselection
    };

    if(HasFlag("TauEnDebug")){
      RegionOfInterest.clear();
        RegionOfInterest = {
                      WRTau_Core::BoostedPreselection,
                      WRTau_Core::ResolvedPreselection,
                      WRTau_Core::BoostedLowMassControlRegion,
                      WRTau_Core::ResolvedLowMassControlRegion,
                      WRTau_Core::BoostedSignalRegion,
                      WRTau_Core::ResolvedSignalRegion,
                      WRTau_Core::ResolvedSignalRegionMETInvertMTSame,
                      WRTau_Core::BoostedSignalRegionMETInvertMTSame
                      };
    }

  }

  if(HasFlag("RunTTEnrichedRegion")){
    RegionOfInterest = {WRTau_Core::TTEnrichedRegion};
  }


  if(HasFlag("EXO_16_023")){

    RegionOfInterest.clear();
    RegionOfInterest = {WRTau_Core::EXO_16_023_Preselection,
                        WRTau_Core::BenchmarkBoostedPreselection,
                        WRTau_Core::BenchmarkResolvedPreselection,
                        WRTau_Core::BoostedSignalRegion};

  }

}

void WRTau_Analyzer::executeEvent(){

  Syst_MuonRecoSF = 0.; Syst_ElectronRecoSF = 0. ; Syst_MuonIDSF = 0.; Syst_ElectronIDSF = 0. ;
  Syst_MuonISOSF = 0. ; Syst_TauIDSF = "" ; Syst_TauTriggerSF = 0. ; Syst_LSFSF = 0.; Syst_PU = 0.;
  Syst_Prefire = 0.; Syst_TauES = 0.;

  AnalyzerParameter param;

  param.Clear();
  TriggerList.clear();
  TriggerList = SingleTauTriggers;
  param.Name = "Central";
  param.Electron_Tight_ID = "LRSMTight";
  if(DataYear==2018) param.Electron_Tight_ID = "LRSMTight2018";

  param.Electron_Loose_ID = "LRSMLoose";
  param.Electron_Veto_ID = "LRSMVeto";
  param.Electron_ID_SF_Key = "HEEP";

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPt";
  param.Muon_Veto_ID = "POGLoose";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_TrackerMuons";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";

  param.Jet_ID = "tightLepVeto";
  param.FatJet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  AllFatJets = GetAllFatJets();
  AllGens = GetGens();
  AllLHEs = GetLHEs();
  TauFRErr = 0 ;
  executeEventFromParameter(param);
  if(HasFlag("TauEnDebug")){
    param.syst_ = AnalyzerParameter::TauEnUp;
    param.Name = "Syst_"+param.GetSystType();
    executeEventFromParameter(param);
    param.syst_ = AnalyzerParameter::TauEnDown;
    param.Name = "Syst_"+param.GetSystType();
    executeEventFromParameter(param);
    param.syst_ = AnalyzerParameter::JetEnUp;
    param.Name = "Syst_"+param.GetSystType();
    executeEventFromParameter(param);
    param.syst_ = AnalyzerParameter::JetEnDown;
    param.Name = "Syst_"+param.GetSystType();
    executeEventFromParameter(param);
    
  }
  if(runSystematics){
    
    RegionOfInterest = {
                      WRTau_Core::BoostedLowMassControlRegion,
                      WRTau_Core::ResolvedLowMassControlRegion,
                      WRTau_Core::BoostedSignalRegion,
                      WRTau_Core::ResolvedSignalRegion,
                      WRTau_Core::ResolvedSignalRegionMETInvertMTSame,
                      WRTau_Core::BoostedSignalRegionMETInvertMTSame };
    
    for(unsigned int i=0; i<AnalyzerParameter::NSyst; i++){
      param.syst_ = AnalyzerParameter::Syst(i);
      // cout << "[WRTau_Analyzer::Debug] Syst : " << i << endl;
      param.Name = "Syst_"+ param.GetSystType();
      auto it = std::find(whiteSysts.begin(), whiteSysts.end(), param.syst_);
      if (it != whiteSysts.end()) executeEventFromParameter(param);
    }
  }
  if(runTauFake){
    param.Name = "Central_TauFRErrUp";
    TauFRErr = 1 ;
    executeEventFromParameter(param);
    param.Name = "Central_TauFRErrDown";
    TauFRErr = -1 ;
    executeEventFromParameter(param);
    
  }

}

void WRTau_Analyzer::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  bool isRunXsecSyst = HasFlag("RunXsecSyst") && param.syst_ == AnalyzerParameter::Central;

  Event ev = GetEvent();
  double weight(1.);

  //if(HasFlag("debugFRfromBins")) cout << param.Name << "with TauFRErr = " << TauFRErr << endl;

  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

  vector<Tau> this_AllTaus = ScaleTaus(AllTaus,0);
  vector<Muon> this_AllMuons = UseTunePMuon(AllMuons);
  vector<Electron> this_AllElectrons = AllElectrons;



  if(param.syst_ == AnalyzerParameter::JetResUp){
    this_AllJets = SmearJets( this_AllJets, +1 );
    this_AllFatJets = SmearFatJets( this_AllFatJets, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetResDown){
    this_AllJets = SmearJets( this_AllJets, -1 );
    this_AllFatJets = SmearFatJets( this_AllFatJets, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetEnUp){
    this_AllJets = ScaleJets( this_AllJets, +1 );
    this_AllFatJets = ScaleFatJets( this_AllFatJets, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetEnDown){
    this_AllJets = ScaleJets( this_AllJets, -1 );
    this_AllFatJets = ScaleFatJets( this_AllFatJets, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonEnUp)                this_AllMuons = ScaleMuons(this_AllMuons,+1);
  else if(param.syst_ == AnalyzerParameter::MuonEnDown)         this_AllMuons = ScaleMuons(this_AllMuons,-1);
  else if(param.syst_ == AnalyzerParameter::ElectronResUp)      this_AllElectrons = SmearElectrons(this_AllElectrons, +1); 
  else if(param.syst_ == AnalyzerParameter::ElectronResDown)    this_AllElectrons = SmearElectrons(this_AllElectrons, -1);
  else if(param.syst_ == AnalyzerParameter::ElectronEnUp)       this_AllElectrons = ScaleElectrons(this_AllElectrons, +1);
  else if(param.syst_ == AnalyzerParameter::ElectronEnDown)     this_AllElectrons = ScaleElectrons(this_AllElectrons, -1);
  else if(param.syst_ == AnalyzerParameter::TauEnUp)            this_AllTaus = ScaleTaus(this_AllTaus,+1);
  else if(param.syst_ == AnalyzerParameter::TauEnDown)          this_AllTaus = ScaleTaus(this_AllTaus,-1);
  else if(param.syst_ == AnalyzerParameter::MuonIDSFUp)         Syst_MuonIDSF   = +1;
  else if(param.syst_ == AnalyzerParameter::MuonIDSFDown)       Syst_MuonIDSF   = -1;
  else if(param.syst_ == AnalyzerParameter::MuonISOSFUp)        Syst_MuonISOSF  = +1;
  else if(param.syst_ == AnalyzerParameter::MuonISOSFDown)      Syst_MuonISOSF  = -1;
  else if(param.syst_ == AnalyzerParameter::ElectronRecoSFUp)   Syst_ElectronRecoSF = +1;
  else if(param.syst_ == AnalyzerParameter::ElectronRecoSFDown) Syst_ElectronRecoSF = -1;
  else if(param.syst_ == AnalyzerParameter::ElectronIDSFUp)     Syst_ElectronIDSF   = +1;
  else if(param.syst_ == AnalyzerParameter::ElectronIDSFDown)   Syst_ElectronIDSF   = -1;
  else if(param.syst_ == AnalyzerParameter::TauIDSFSystUp)      Syst_TauIDSF = "syst_up";
  else if(param.syst_ == AnalyzerParameter::TauIDSFSystDown)    Syst_TauIDSF = "syst_down";
  else if(param.syst_ == AnalyzerParameter::TauIDSFStatUp)      Syst_TauIDSF = "stat_up";
  else if(param.syst_ == AnalyzerParameter::TauIDSFStatDown)    Syst_TauIDSF = "stat_down";
  else if(param.syst_ == AnalyzerParameter::TauIDSFExtUp)       Syst_TauIDSF = "extrap_up";
  else if(param.syst_ == AnalyzerParameter::TauIDSFExtDown)     Syst_TauIDSF = "extrap_down";
  else if(param.syst_ == AnalyzerParameter::TauTriggerSFUp)     Syst_TauTriggerSF = +1;
  else if(param.syst_ == AnalyzerParameter::TauTriggerSFDown)   Syst_TauTriggerSF = -1;
  else if(param.syst_ == AnalyzerParameter::PUUp)               Syst_PU = +1;
  else if(param.syst_ == AnalyzerParameter::PUDown)             Syst_PU = -1;
  else if(param.syst_ == AnalyzerParameter::PrefireUp)          Syst_Prefire = +1;
  else if(param.syst_ == AnalyzerParameter::PrefireDown)        Syst_Prefire = -1;
  
  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else{
      weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(Syst_Prefire) * GetPileUpWeight(nPileUp,Syst_PU) * GetTauTriggerSF(Syst_TauTriggerSF);
      //cout << "weight = " << weight << "@(" << run << " , " << lumi << " , " << event << "), " << param.Name << endl;
      //cout << "\t" << weight << " = " <<  MCweight(true,true) << "*" << ev.GetTriggerLumi("Full") << "*" << GetPrefireWeight(Syst_Prefire) << "*" << GetPileUpWeight(nPileUp,Syst_PU) << "*" << GetTauTriggerSF(Syst_TauTriggerSF) << "@(" << run << " , " << lumi << " , " << event << "), " << param.Name << endl;
      //cout << "\t\t Syst_TauTriggerSF = " <<  Syst_TauTriggerSF << endl;
    }
  }
  FillHist(param.Name+"/Weight",1,weight,4000,0.8,1.2);
  

  // GetvMET function incorporates each systematic variations for every case 
  Particle METv       = GetvMET("Puppi",param,this_AllJets,this_AllFatJets,this_AllMuons,this_AllElectrons,this_AllTaus,false);
  Particle METv_Puppi = GetvMET("Puppi",param,this_AllJets,this_AllFatJets,this_AllMuons,this_AllElectrons,this_AllTaus,false);

  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::comb);

  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.4) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 50., 2.4) ;
  vector<Muon> muons_loose  = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 50., 2.4);

  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50., 2.4);
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 50., 2.4);
  vector<Electron> electrons_loose = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 50.,2.4);

  vector<Lepton *> leptons_tmp = CombineLeptonPointerVector(electrons,muons);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);
  vector<Lepton *> LooseLeptons_tmp = CombineLeptonPointerVector(electrons_loose,muons_loose);


  vector<Tau> taus_lepVeto = VetoLeptonsFromTaus(VetoLeps,this_AllTaus);

  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){


        std::tuple<int,int,int> IDtuple = std::make_tuple(vJet_vec[i],vEl_vec[j],vMu_vec[k]);
        TString idname = "vJet"+idname_map[vJet_vec.at(i)]+"_vEl"+idname_map[vEl_vec.at(j)]+"_vMu"+idname_map[vMu_vec.at(k)];
        vector<Tau> taus_temp = SelectTaus_varWP(taus_lepVeto,vJet_vec[i],vEl_vec[j],vMu_vec[k],50,2.1);
        vector<Tau> taus;

        TString path = param.Name ; //+"/"+idname;
        //TString path = param.Name+TauPromptString+LeptonPromptString"/"+idname;
        /*if(taus_temp.size()>0){
          for(unsigned int i =0 ; i < taus_temp.size() ; i++){
            FillHist(path+"/TauType"+TString::Itoa(i,10),GetTauType(taus_temp.at(i),AllGens),weight,20,-10.,10.);
          }
        }*/
        if(HasFlag("NonpromptTau")) taus = TauFakeOnly(taus_temp,AllGens);
        else if(HasFlag("PromptTau")) taus = TauPromptOnly(taus_temp,AllGens);
        else if(HasFlag("RunApplicationRegion")) taus = SelectTaus(taus_lepVeto,"LRSMLoose",50,2.1);
        else taus = taus_temp;
        std::sort(taus.begin(),taus.end(),PtComparing);

        vector<Jet> jets_tauVeto = VetoTauFromJets(this_AllJets,taus); 
        vector<FatJet> fatjets_tmp = VetoTauFromFatJets(this_AllFatJets,taus);
        //vector<Lepton *> LooseLeptons = VetoTauFromLeptons(LooseLeptons_tmp,taus);
        //vector<Lepton *> TightLeptons = VetoTauFromLeptons(leptons_tmp,taus);
        vector<Lepton *> LooseLeptons = LooseLeptons_tmp;
        vector<Lepton *> TightLeptons = leptons_tmp;

        vector<Jet> jets_lepVeto_tauVeto = JetsVetoLeptonInside(jets_tauVeto,electrons_veto,muons_veto,0.4);
        vector<Jet> jets = SelectJets(jets_lepVeto_tauVeto, param.Jet_ID, 40., 2.4);
        vector<FatJet> fatjets = SelectFatJets(fatjets_tmp,param.FatJet_ID,200.,2.4);
        if(HasFlag("AK8PuppiCorr")){ 
          vector<FatJet> fatjets_puppi = puppiCorr->Correct(fatjets);
          fatjets = fatjets_puppi;
        }
        vector<Jet> bjets = SelectBJets(jets,param_jetsM);

        std::sort(bjets.begin(),bjets.end(),PtComparing);
        std::sort(jets.begin(),jets.end(),PtComparing);
        std::sort(fatjets.begin(),fatjets.end(),PtComparing);
        std::sort(LooseLeptons.begin(),LooseLeptons.end(),PtComparingPtr);
        std::sort(TightLeptons.begin(),TightLeptons.end(),PtComparingPtr);

        //cout << "[WRTau_Analyzer::Debug] Object vector sorted" << endl;

        FillHist(path+"/Cutflow",0.,weight,10,0.,10.);
        /*
        if(isRunXsecSyst){
           
          for(unsigned int i=0; i<weight_PDF->size(); i++){
            FillHist("XsecSyst/PDFWeights_"+TString::Itoa(i,10),0.,weight_PDF->at(i)*MCweight(false,true)*normweight,1,0.,1.);
          }
          for(unsigned int i=0; i<weight_AlphaS->size(); i++){
            FillHist("XsecSyst/PDFAlphaS_"+TString::Itoa(i,10),0.,weight_AlphaS->at(i)*MCweight(false,true)*normweight,1,0.,1.);
          }
          for(unsigned int i=0; i<weight_Scale->size(); i++){
            if(i==5 || i==7) continue;
            FillHist("XsecSyst/Scale_"+TString::Itoa(i,10),0.,weight_Scale->at(i)*MCweight(false,true)*normweight,1,0.,1.);
          }
          
        }*/

        if(isSignalSample() && HasFlag("SignalDebug")){

          vector<Tau> baselineTau = SelectTaus(AllTaus,"Baseline",50,2.4);
          vector<Tau> baseline_L_Tau = SelectTaus(AllTaus,"BaselineWithvJetLoose",50,2.4);
          vector<Tau> baseline_T_Tau = SelectTaus(AllTaus,"BaselineWithvJetTight",50,2.4);
          vector<Tau> baseline_TL_Tau = SelectTaus(AllTaus,"BaselineWithvJetTightvElLoose",50,2.4);
          vector<Tau> baseline_TT_Tau = SelectTaus(AllTaus,"BaselineWithvJetTightvElTight",50,2.4);
          vector<Tau> baseline_TLL_Tau = SelectTaus(AllTaus,"BaselineWithvJetTightvElLoosevMuLoose",50,2.4);
          vector<Tau> baseline_TTL_Tau = SelectTaus(AllTaus,"BaselineWithvJetTightvElTightvMuLoose",50,2.4);
          vector<Tau> baseline_TLT_Tau = SelectTaus(AllTaus,"BaselineWithvJetTightvElLoosevMuTight",50,2.4);
          vector<Tau> baseline_TTT_Tau = SelectTaus(AllTaus,"BaselineWithvJetTightvElTightvMuTight",50,2.4);

          FillHist(path+"/nAllTaus",AllTaus.size(),weight,10,0.,10.);
          FillHist(path+"/nBaselineTaus",baselineTau.size(),weight,10,0.,10.);
          FillHist(path+"/nBaseline_L_Taus",baseline_L_Tau.size(),weight,10,0.,10.);
          FillHist(path+"/nBaseline_T_Taus",baseline_T_Tau.size(),weight,10,0.,10.);
          FillHist(path+"/nBaseline_TL_Taus",baseline_TL_Tau.size(),weight,10,0.,10.);
          FillHist(path+"/nBaseline_TT_Taus",baseline_TT_Tau.size(),weight,10,0.,10.);
          FillHist(path+"/nBaseline_TLL_Taus",baseline_TLL_Tau.size(),weight,10,0.,10.);
          FillHist(path+"/nBaseline_TTL_Taus",baseline_TTL_Tau.size(),weight,10,0.,10.);
          FillHist(path+"/nBaseline_TLT_Taus",baseline_TLT_Tau.size(),weight,10,0.,10.);
          FillHist(path+"/nBaseline_TTT_Taus",baseline_TTT_Tau.size(),weight,10,0.,10.);
          FillHist(path+"/nTaus",taus.size(),weight,10,0.,10.);
          std::map<WRTau_Core::SearchRegion,bool> genChannelMap = getGenLevelChannelMap(AllGens);
          for(auto const& region : genChannelMap){

            if(region.second){
              FillHist(path+"/GenLevelChannel",region.first-100,weight,10,0.,10.);
              FillHist(path+"/GenLevelChannel_"+TString::Itoa(region.first-100,10)+"_nAllTaus",AllTaus.size(),weight,10,0.,10.);
              FillHist(path+"/GenLevelChannel_"+TString::Itoa(region.first-100,10)+"_nTaus",taus.size(),weight,10,0.,10.);
              FillHist(path+"/GenLevelChannel_MCWeight",region.first-100,MCweight(true,true),10,0.,10.);
              FillHist(path+"/GenLevelChannel_MCWeight_"+TString::Itoa(region.first-100,10)+"_nAllTaus",AllTaus.size(),MCweight(true,true),10,0.,10.);
              FillHist(path+"/GenLevelChannel_MCWeight_"+TString::Itoa(region.first-100,10)+"_nTaus",taus.size(),MCweight(true,true),10,0.,10.);
            }

          }
          continue;
        }

        if(isSignalSample()){
          std::map<WRTau_Core::SearchRegion,bool> genChannelMap = getGenLevelChannelMap(AllGens);
          for(auto const& region : genChannelMap){

            if(region.second){
              FillHist(path+"/GenLevelChannel",region.first-100,weight,10,0.,10.);
              FillHist(path+"/GenLevelChannel_"+TString::Itoa(region.first-100,10)+"_nAllTaus",AllTaus.size(),weight,10,0.,10.);
              FillHist(path+"/GenLevelChannel_"+TString::Itoa(region.first-100,10)+"_nTaus",taus.size(),weight,10,0.,10.);
            }

          }
        }

        /*bool PassTrg(false);
        if(param.Name == "WRTau_SignalSingleTauTrg" || HasFlag("MuIsoCutOpt") ){
          if(ev.PassTrigger(TriggerList)) PassTrg = true;
        }*/

        if(!ev.PassTrigger(TriggerList)) continue;
        FillHist(path+"/Cutflow",1.,weight,10,0.,10.);

        if(HasFlag("debug")) cout << "Pass Cut 1" << endl;

        if(taus.size()<1) continue;
        FillHist(path+"/Cutflow",2.,weight,10,0.,10.);

        if(HasFlag("debug")) cout << "Pass Cut 2" << endl;

        if(taus.at(0).Pt()<190) continue;
        FillHist(path+"/Cutflow",3.,weight,10,0.,10.);

        //if(HasFlag("debug")) cout << "Pass Cut 3" << endl;
        /*if(HasFlag("ResolvedElectronChannelFake")){
          if()
        }*/

        if(LooseLeptons.size()!=1) continue;
        
        //if(LooseLeptons.at(0)->IsMuon()) METv = METv_Puppi;
        FillHist(path+"/Cutflow",4.,weight,10,0.,10.);

        if(HasFlag("debug")) cout << "Pass Cut 4" << endl;

        if(HasFlag("DeltaTest")){
          if(LooseLeptons.at(0)->IsMuon()){
            FillHist(path+"/Baseline_MuTau/dRMETmu",LooseLeptons.at(0)->DeltaR(METv),weight,650,0,6.5);
            FillHist(path+"/Baseline_MuTau/dPhiMETmu",LooseLeptons.at(0)->DeltaPhi(METv),weight,650,0,6.5);
            FillHist(path+"/Baseline_MuTau/dRMETtau",taus.at(0).DeltaR(METv),weight,650,0,6.5);
            FillHist(path+"/Baseline_MuTau/dPhiMETtau",taus.at(0).DeltaPhi(METv),weight,650,0,6.5);
            FillHist(path+"/Baseline_MuTau/dRmutau",taus.at(0).DeltaR(*LooseLeptons.at(0)),weight,650,0,6.5);
            FillHist(path+"/Baseline_MuTau/dPhimutau",taus.at(0).DeltaPhi(*LooseLeptons.at(0)),weight,650,0,6.5);
          }
          continue;
        }
        
        map<WRTau_Core::SearchRegion,bool> map_regions = GetRegion(param,weight,METv,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons);
        if(HasFlag("debug")) cout << "Pass get map_region" << endl;

        if(HasFlag("LSFOpt")){
          map<pair<WRTau_Core::SearchRegion,double>, bool> LSFOptCutMap = LSFCutter(map_regions,{0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85},fatjets);
          FillPassingRegions(LSFOptCutMap,"LSF",METv,AllGens,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,path,weight,IDtuple,true);
        }

        if(HasFlag("MassOpt")){
          
          //cout << "[WRTauAnalyzer] Start MassCutter_Meff" << endl;
          map<pair<WRTau_Core::SearchRegion,double>, bool> MassOptCutMap_eff = MassCutter(map_regions,{500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500},METv,taus,jets,fatjets,LooseLeptons,TightLeptons,true);
          map<pair<WRTau_Core::SearchRegion,double>, bool> MassOptCutMap_reco = MassCutter(map_regions,{500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500},METv,taus,jets,fatjets,LooseLeptons,TightLeptons);
          map<pair<WRTau_Core::SearchRegion,double>, bool> MassOptCutMap_mt = MassCutter(map_regions,{0,25,50,75,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,475,500,525,550,575,600,625,650,675,700},METv,taus,jets,fatjets,LooseLeptons,TightLeptons,false);

          FillPassingRegions(MassOptCutMap_eff,"EffMass",METv,AllGens,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,path,weight,IDtuple,true);
          FillPassingRegions(MassOptCutMap_reco,"RecoNuMass",METv,AllGens,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,path,weight,IDtuple,true);
          FillPassingRegions(MassOptCutMap_mt,"TransMass",METv,AllGens,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,path,weight,IDtuple,true);

        }

        if(isRunXsecSyst){
          //double normweight = 1./sumW/weight_PDF->at(0); // k-factor
          for(unsigned int l=0; l<RegionOfInterest.size() ; l++){
            FillPassingRegions_XsecVar(map_regions,RegionOfInterest.at(l),METv,AllGens,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,path,weight,IDtuple,true);
          }
        }

        else{
          for(unsigned int l=0; l<RegionOfInterest.size() ; l++){
            if(HasFlag("2DScan")) FillPassingRegions_2DScan(map_regions,RegionOfInterest.at(l),METv,AllGens,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,path,weight,IDtuple,true);
            else FillPassingRegions(map_regions,RegionOfInterest.at(l),METv,AllGens,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,path,weight,IDtuple,true);
          }
          /*if(MCSample.Contains("WRtoTauNtoTauTauJets")){
            if(map_regions[WRTau_Core::BoostedSignalRegion]) cout << "(run,lumi,event) = " << run << ", " << lumi << ", " << event << endl;
            if(map_regions[WRTau_Core::ResolvedSignalRegion]) cout << "Resolved (run,lumi,event) = " << run << ", " << lumi << ", " << event << endl;
          }*/
        }

      }
    }
  }

}

WRTau_Analyzer::WRTau_Analyzer(){

}

WRTau_Analyzer::~WRTau_Analyzer(){

}


