#include "WRTau_Analyzer.h"

void WRTau_Analyzer::initializeAnalyzer(){

  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {5}; vEl_vec = {13}; vMu_vec = {21};
  
  GetTauIDSFTools(vJet_vec,vEl_vec,vMu_vec);

  if(DataYear==2017){

    SingleElectronTriggers = {"HLT_Ele35_WPTight_Gsf_v"};
    SingleMuonTriggers = {"HLT_Mu50_v"};
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
    SingleLeptonTriggers = {"HLT_Mu27_v","HLT_Ele35_WPTight_Gsf_v"};

  }

  RegionOfInterest = {WRTau_Core::BoostedPreselection,
                      WRTau_Core::BoostedLowMassControlRegion,
                      WRTau_Core::ResolvedLowMassControlRegion,
                      //WRTau_Core::BoostedLowMassControlRegionMass1,
                      WRTau_Core::BoostedSignalRegion,
                      //WRTau_Core::BoostedSignalRegionMass1,
                      WRTau_Core::ResolvedSignalRegionMETInvert,
                      WRTau_Core::BoostedSignalRegionMETInvert
                      };

}

void WRTau_Analyzer::executeEvent(){


  AnalyzerParameter param;

  param.Clear();
  TriggerList.clear();
  
  TriggerList = SingleTauTriggers;
  TriggerSafeTauPtCut = 190.;

  param.Name = "WRTau_SignalSingleTauTrg";
  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "passVetoID";
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
/*
  param.Clear();
  TriggerList.clear();
  
  TriggerList = {"HLT_IsoMu20_eta2p1_LooseChargedIsoPFTau27_eta2p1_CrossL1_v"};
  TriggerSafeTauPtCut = 40.;

  param.Name = "WRTau_Tau27IsoMu20Trg";
  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "passVetoID";
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
*/
}

void WRTau_Analyzer::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  FillHist("MCWeight",MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0),1.,2e5,-1e5,1e5);

  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

  vector<Tau> this_AllTaus = AllTaus;
  vector<Muon> this_AllMuons_ = UseTunePMuon(AllMuons);
  vector<Muon> this_AllMuons;
  vector<Electron> this_AllElectrons;

  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::comb);

  if(HasFlag("PromptLepton")){
    this_AllMuons = MuonPromptOnly(this_AllMuons_,AllGens);
    this_AllElectrons = ElectronPromptOnly(AllElectrons,AllGens);
    //cout << "prompt leptons init" << endl;
  }

  else if(HasFlag("NonpromptLepton")){
    this_AllMuons = MuonNonPromptOnly(this_AllMuons_,AllGens);
    this_AllElectrons = ElectronNonPromptOnly(AllElectrons,AllGens);
    //cout << "nonprompt leptons init" << endl;
    //cout << "this_AllMuons , this_AllElectrons " << this_AllMuons.size() << " " << this_AllElectrons.size() << endl ; 
  }

  else{
    this_AllMuons = this_AllMuons_;
    this_AllElectrons = AllElectrons;
  }


  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.4) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 50., 2.4) ;
  vector<Muon> muons_loose = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 50., 2.4) ;

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
        
        /*FillHist(path+"/AllTaus",AllTaus.size(),weight,10,0.,10.);
        FillHist(path+"/LepVetoTaus",taus_lepVeto.size(),weight,10,0.,10.);
        FillHist(path+"/TausTMP",taus_temp.size(),weight,10,0.,10.);

        std::sort(AllTaus.begin(),AllTaus.end(),PtComparing);
        std::sort(taus_lepVeto.begin(),taus_lepVeto.end(),PtComparing);
        std::sort(taus_temp.begin(),taus_temp.end(),PtComparing);

        for(unsigned int i=0;i<AllTaus.size();i++){
          FillHist(path+"/AllTaus"+TString::Itoa(i,10)+"_Pt",AllTaus.at(i).Pt(),weight,1000,0.,1000.);
          FillHist(path+"/AllTaus_Pt",AllTaus.at(i).Pt(),weight,1000,0.,1000.);
        }

        for(unsigned int i=0;i<taus_lepVeto.size();i++){
          FillHist(path+"/LepVetoTaus"+TString::Itoa(i,10)+"_Pt",taus_lepVeto.at(i).Pt(),weight,1000,0.,1000.);
          FillHist(path+"/LepVetoTaus_Pt",taus_lepVeto.at(i).Pt(),weight,1000,0.,1000.);
        }

        for(unsigned int i=0;i<taus_temp.size();i++){
          FillHist(path+"/TausTMP"+TString::Itoa(i,10)+"_Pt",taus_temp.at(i).Pt(),weight,1000,0.,1000.);
          FillHist(path+"/TausTMP_Pt",taus_temp.at(i).Pt(),weight,1000,0.,1000.);
        }*/

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

        FillHist(path+"/Cutflow",0.,weight,10,0.,10.);

        if(MCSample.Contains("WRtoTauNtoTauTauJets")){

          std::map<WRTau_Core::SearchRegion,bool> genChannelMap = getGenLevelChannelMap(AllGens);
          for(auto const& region : genChannelMap){

            if(region.second) FillHist(path+"/GenLevelChannel",region.first-100,weight,10,0.,10.);

          }

        }
        
        bool PassTrg(false);
        if(param.Name == "WRTau_SignalSingleTauTrg"){
          if(ev.PassTrigger(TriggerList)) PassTrg = true;
        }

        if(!PassTrg) continue;
        FillHist(path+"/Cutflow",1.,weight,10,0.,10.);

        if(taus.size()<1) continue;
        FillHist(path+"/Cutflow",2.,weight,10,0.,10.);

        if(taus.at(0).Pt()<190) continue;
        FillHist(path+"/Cutflow",3.,weight,10,0.,10.);

        if(LooseLeptons.size()!=1) continue;
        if(!LooseLeptons.at(0)->IsMuon()) continue; // concentrate on muon channel first 
        FillHist(path+"/Cutflow",4.,weight,10,0.,10.);

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


