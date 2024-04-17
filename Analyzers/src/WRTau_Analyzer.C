#include "WRTau_Analyzer.h"

void WRTau_Analyzer::initializeAnalyzer(){

  if(HasFlag("RunSyst") && !IsDATA) runSystematics = true;
  else runSystematics = false;

  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {5}; vEl_vec = {13}; vMu_vec = {21};
  if(HasFlag("TauFake")) vJet_vec = {0};

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
                      WRTau_Core::BoostedSignalRegionMETInvert
                      };

}

void WRTau_Analyzer::executeEvent(){

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

  executeEventFromParameter(param);
  if(runSystematics){
    for(unsigned int i=0; i<AnalyzerParameter::NSyst; i++){
      param.syst_ = AnalyzerParameter::Syst(i);
      param.Name = "Syst_"+ param.GetSystType();
      auto it = std::find(skipSysts.begin(), skipSysts.end(), param.syst_);
      executeEventFromParameter(param);
    }
  }

}

void WRTau_Analyzer::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  double weight(1.);

  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

  vector<Tau> this_AllTaus = AllTaus;
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
  /* For TuneP muons, MomentumShift() are set correctly from AnalyzerCore::UseTunePMuon()
     So we can just use MomentumShift() */
  else if(param.syst_ == AnalyzerParameter::MuonEnUp)           this_AllMuons = ScaleMuons(this_AllMuons,+1);
  else if(param.syst_ == AnalyzerParameter::MuonEnDown)         this_AllMuons = ScaleMuons(this_AllMuons,-1);
  else if(param.syst_ == AnalyzerParameter::ElectronResUp)      this_AllElectrons = SmearElectrons(this_AllElectrons, +1); 
  else if(param.syst_ == AnalyzerParameter::ElectronResDown)    this_AllElectrons = SmearElectrons(this_AllElectrons, -1);
  else if(param.syst_ == AnalyzerParameter::ElectronEnUp)       this_AllElectrons = ScaleElectrons(this_AllElectrons, +1);
  else if(param.syst_ == AnalyzerParameter::ElectronEnDown)     this_AllElectrons = ScaleElectrons(this_AllElectrons, -1);
  // TODO
  //else if(param.syst_ == AnalyzerParameter::MuonRecoSFUp)       Syst_MuonRecoSF = +1;
  //else if(param.syst_ == AnalyzerParameter::MuonRecoSFDown)     Syst_MuonRecoSF = -1; 
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
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(Syst_Prefire) * GetPileUpWeight(nPileUp,Syst_PU) * GetTauTriggerSF(Syst_TauTriggerSF);
  }

  // GetvMET function incorporates each systematic variations for every case 
  Particle METv       = GetvMET("T1",param,this_AllJets,this_AllFatJets,this_AllMuons,this_AllElectrons,false);
  Particle METv_Puppi = GetvMET("Puppi",param,this_AllJets,this_AllFatJets,this_AllMuons,this_AllElectrons,false);

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
        vector<Tau> taus_temp = SelectTaus_varWP(taus_lepVeto,vJet_vec[i],vEl_vec[j],vMu_vec[k],50,2.4);
        vector<Tau> taus;

        TString path = param.Name+"/"+idname;
        //TString path = param.Name+TauPromptString+LeptonPromptString"/"+idname;
        if(taus_temp.size()>0){
          for(unsigned int i =0 ; i < taus_temp.size() ; i++){
            FillHist(path+"/TauType"+TString::Itoa(i,10),GetTauType(taus_temp.at(i),AllGens),weight,20,-10.,10.);
          }
        }
        if(HasFlag("NonpromptTau")) taus = TauFakeOnly(taus_temp,AllGens);
        else if(HasFlag("PromptTau")) taus = TauPromptOnly(taus_temp,AllGens);
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

        if(MCSample.Contains("WRtoTauNtoTauTauJets")){

          std::map<WRTau_Core::SearchRegion,bool> genChannelMap = getGenLevelChannelMap(AllGens);
          for(auto const& region : genChannelMap){

            if(region.second) FillHist(path+"/GenLevelChannel",region.first-100,weight,10,0.,10.);

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
        
        if(LooseLeptons.at(0)->IsMuon()) METv = METv_Puppi;
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
        
        map<WRTau_Core::SearchRegion,bool> map_regions = GetRegion(METv,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons);
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

        else{
          for(unsigned int l=0; l<RegionOfInterest.size() ; l++){
            FillPassingRegions(map_regions,RegionOfInterest.at(l),METv,AllGens,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,path,weight,IDtuple,true);
          }
        }

      }
    }
  }

}

WRTau_Analyzer::WRTau_Analyzer(){

}

WRTau_Analyzer::~WRTau_Analyzer(){

}


