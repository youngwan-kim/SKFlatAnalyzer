#include "WRTau_SR_Test.h"

void WRTau_SR_Test::initializeAnalyzer(){


  if(DataYear==2017){

    SingleElectronTriggers = {"HLT_Ele35_WPTight_Gsf_v"};
    SingleMuonTriggers = {"HLT_Mu50_v"};
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
    SingleLeptonTriggers = {"HLT_Mu27_v","HLT_Ele35_WPTight_Gsf_v"};

  }

}

void WRTau_SR_Test::executeEvent(){


  AnalyzerParameter param;

  // SingleLepton Trigger for bkg study
  /*param.Clear();
  TriggerList.clear();
  TriggerList = SingleLeptonTriggers;

  param.Name = "WRTau_BkgSingleLeptonTrg";
  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "passVetoID";
  param.Electron_ID_SF_Key = "HEEP";
  electron_looseID_SF_Key = "passLooseID";

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPt";
  param.Muon_Veto_ID = "POGLoose";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_TrackerMuons";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
  muon_recoSF_key = "HighPtMuonRecoSF";

  param.Jet_ID = "tightLepVeto";
  param.FatJet_ID = "tight";
  param.syst_ = AnalyzerParameter::Central;

  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  AllFatJets = GetAllFatJets();
  AllGens = GetGens();

  executeEventFromParameter(param);*/
 
  param.Clear();

  TriggerList.clear();
  TriggerList = SingleTauTriggers;

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

  executeEventFromParameter(param);
  
}

void WRTau_SR_Test::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);

  vector<int> vJet_vec = {5,6};   //vL,L,M,T,vT
  vector<int> vEl_vec = {11,12,13};     //L,M,T 
  vector<int> vMu_vec = {19,20,21};     //L,M,T

  // if(param.Name == "WRTau_BkgSingleLeptonTrg")
  
  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  //vJet_vec = {4,5}; vEl_vec = {9,13}; vMu_vec = {17,20,21};
  vJet_vec = {3,4,5}; vEl_vec = {9,13}; vMu_vec = {18,21};
  

  map<int,TString> idname_map = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  map<int,std::string> idname_map_str = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                         {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                         {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};


  bool PassTrg(false);

  if(param.Name == "WRTau_BkgSingleLeptonTrg"){
    bool PassOneTrigger(false);
    if(ev.PassTrigger(TriggerList)){
      if(ev.PassTrigger(SingleMuonTriggers) && !ev.PassTrigger(SingleElectronTriggers)){
        PassOneTrigger = true;
      }
      else if(!ev.PassTrigger(SingleMuonTriggers) && ev.PassTrigger(SingleElectronTriggers)){
        PassOneTrigger = true;
      }
      else if(ev.PassTrigger(SingleMuonTriggers) && ev.PassTrigger(SingleElectronTriggers)) return;
    }
    if(PassOneTrigger) PassTrg = true;
  }

  if(param.Name == "WRTau_SignalSingleTauTrg"){
    if(ev.PassTrigger(TriggerList)) PassTrg = true;
  }

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  /*if(!IsDATA && MCSample.Contains("QCD")){
    PassTrg = true;
  }*/

  
  // Object Selection
  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

  vector<Tau> this_AllTaus = AllTaus;
  vector<Muon> this_AllMuons = UseTunePMuon(AllMuons);
  vector<Electron> this_AllElectrons = AllElectrons;

  // b-tagging
  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::comb);

  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.4) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 50., 2.4) ;
  vector<Muon> muons_loose = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 50., 2.4) ;

  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50., 2.4);
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 50., 2.4);
  vector<Electron> electrons_loose = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 50.,2.4);

  vector<Lepton *> leptons_tmp = CombineLeptonPointerVector(electrons,muons);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);
  vector<Lepton *> LooseLeps_tmp = CombineLeptonPointerVector(electrons_loose,muons_loose);;
  
  vector<Tau> taus_lepVeto = VetoLeptonsFromTaus(VetoLeps,this_AllTaus);

  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){

        TString idname = "vJet"+idname_map[vJet_vec.at(i)]+"_vEl"+idname_map[vEl_vec.at(j)]+"_vMu"+idname_map[vMu_vec.at(k)];
        TString path = param.Name+"/"+idname;
        vector<Tau> taus = SelectTaus_varWP(taus_lepVeto,vJet_vec[i],vEl_vec[j],vMu_vec[k],50,2.4);
        std::sort(taus.begin(),taus.end(),PtComparing);

        vector<Jet> jets_tauVeto = VetoTauFromJets(this_AllJets,taus); 
        vector<FatJet> fatjets_tmp = VetoTauFromFatJets(this_AllFatJets,taus);
        vector<Lepton *> LooseLeps = VetoTauFromLeptons(LooseLeps_tmp,taus);
        vector<Lepton *> leptons = VetoTauFromLeptons(leptons_tmp,taus);

        vector<Jet> jets_lepVeto_tauVeto = JetsVetoLeptonInside(jets_tauVeto,electrons_veto,muons_veto,0.4);
        vector<Jet> jets = SelectJets(jets_lepVeto_tauVeto, param.Jet_ID, 40., 2.4) ;
        vector<FatJet> fatjets = SelectFatJets(fatjets_tmp,param.FatJet_ID,200.,2.4);

        //double btagWeight = mcCorr->GetBTaggingReweight_1a(jets, param_jetsM);
        
        int NBJets(0);
        vector<Jet> bjets = SelectBJets(jets,param_jetsM);

        //cout << NBJets << endl;

        std::sort(bjets.begin(),bjets.end(),PtComparing);
        std::sort(jets.begin(),jets.end(),PtComparing);
        std::sort(fatjets.begin(),fatjets.end(),PtComparing);
        std::sort(LooseLeps.begin(),LooseLeps.end(),PtComparingPtr);
        std::sort(leptons.begin(),leptons.end(),PtComparingPtr);

        FillHist(path+"/Cutflow",0.,weight,10,0.,10.);
        
        if(!PassTrg) continue;
        FillHist(path+"/Cutflow",1.,weight,10,0.,10.);

        if(taus.size()<1) continue;
        FillHist(path+"/Cutflow",2.,weight,10,0.,10.);

        if(taus.at(0).Pt()<190) continue;
        FillHist(path+"/Cutflow",3.,weight,10,0.,10.);

        if(LooseLeps.size()!=1) continue;
        FillHist(path+"/Cutflow",4.,weight,10,0.,10.);

        double cut = 4.

        if(HasFlag("METcut")){
          cut += 1;
          if(METv.Pt()<100) continue;
          FillHist(path+"/Cutflow",cut,weight,10,0.,10.);
        }

        TString region = "Preselection";
        TString region_path = path+"/"+region;

        double tau_weight(1.);

        if(!IsDATA){
          
          // TAU POG ID SF 
          // https://github.com/cms-tau-pog/TauIDSFs/tree/master#c
          
          if(!HasFlag("unweighted")){

            std::string str_Era = "UL"+std::to_string(DataYear);

            TauIDSFTool *tauSFTool_VSJet = new TauIDSFTool(str_Era,DeepTauVSjet,idname_map_str[vJet_vec.at(i)]);
            TauIDSFTool *tauSFTool_VSJet_DM = new TauIDSFTool(str_Era,DeepTauVSjet,idname_map_str[vJet_vec.at(i)],true);
            double tau_weight_DM = tauSFTool_VSJet_DM->getSFvsDM(taus.at(0).Pt(),taus.at(0).DecayMode());

            tau_weight *= tauSFTool_VSJet->getSFvsPT(taus.at(0).Pt());
            
            if(HasFlag("ApplyTauDMSF")) tau_weight *= tau_weight_DM;

            /*tau_weight *= GetTauIDSF(idname_map[vJet_vec.at(i)],idname_map[vEl_vec.at(j)],taus.at(0).DecayMode(),taus.at(0).Pt());
            cout << tau_weight << endl;*/

            // cout << "[TauSF] (vJet,vEl,vMu)" << tauSFTool_VSJet->getSFvsPT(taus.at(0).Pt()) << "," << tau_weight_DM << endl;
          
          }

          else if(HasFlag("unweighted")) tau_weight *= 1.;
          
          if(HasFlag("noTauWeight")) tau_weight = 1.;

        }

        weight *= tau_weight;

        // Tau Only Preselection
        CopyHist(path+"/Cutflow",region_path+"/Cutflow");
        FillHist(region_path+"/Cutflow",5.,weight,10,0.,10.);
        FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight);
        FillHist(region_path+"/MET",METv.Pt(),weight,2500,0.,2500.);

        bool _isBoostedPreselection(false);
        bool _isBoostedPreselectionTest(false);
        bool _isBoostedNonisoLepPreselection(false);
        bool _isBoostedNonisoLepPreselectionTest(false);
        bool _isResolvedPreselection(false);
        bool _isStrictResolvedPreselection(false);
        bool hasAtLeast2AK4Jets = jets.size()>1;
        bool hasAtLeast1AK8Jets = fatjets.size()>0;
        bool hasAtLeast1TightLeptons = leptons.size()>0;
        bool hasAtLeast1LooseLeptons = LooseLeps.size()>0;

        //isResolvedPreselection = hasAtLeast2AK4Jets && (leptons.size()>0);
        //isBoostedPreselection = hasAtLeast1AK8Jets && (LooseLeps.size()>0);
        _isResolvedPreselection = hasAtLeast2AK4Jets && hasAtLeast1TightLeptons;
        _isStrictResolvedPreselection = hasAtLeast2AK4Jets && leptons.size()==1;
        _isBoostedPreselection = !_isResolvedPreselection && hasAtLeast1AK8Jets;
        _isBoostedPreselectionTest = !hasAtLeast2AK4Jets && hasAtLeast1AK8Jets;
        
        if(_isBoostedPreselection){
          for(const auto &lep : LooseLeps){
            if(fatjets.at(0).DeltaR(*lep)<0.8){
              _isBoostedNonisoLepPreselection = true;
              break;
            }
          }
        }

        if(_isBoostedPreselectionTest){
          for(const auto &lep : LooseLeps){
            if(fatjets.at(0).DeltaR(*lep)<0.8){
              _isBoostedNonisoLepPreselectionTest = true;
              break;
            }
          }
        }

        double weight_loose(1.);
        double weight_tight(1.);

        if(!IsDATA){
          if(!HasFlag("unweighted")){  
            if(hasAtLeast1TightLeptons){
              if(leptons.at(0)->IsElectron()){
                weight_tight *= mcCorr->ElectronID_SF(param.Electron_ID_SF_Key,electrons.at(0).Eta(),electrons.at(0).Pt());
                weight_tight *= mcCorr->ElectronReco_SF(electrons.at(0).Eta(),electrons.at(0).Pt());
              }
              else if(leptons.at(0)->IsMuon()){
                weight_tight *= mcCorr->MuonID_SF(param.Muon_ID_SF_Key,muons.at(0).Eta(),muons.at(0).Pt());
              }
            }
            if(hasAtLeast1LooseLeptons){
              if(LooseLeps.at(0)->IsElectron()){
                weight_loose *= mcCorr->ElectronID_SF(electron_looseID_SF_Key,electrons_loose.at(0).Eta(),electrons_loose.at(0).Pt());
                weight_loose *= mcCorr->ElectronReco_SF(electrons_loose.at(0).Eta(),electrons_loose.at(0).Pt());
              }
              else if(LooseLeps.at(0)->IsMuon()){
                weight_loose *= mcCorr->MuonID_SF(param.Muon_ID_SF_Key,muons_loose.at(0).Eta(),muons_loose.at(0).Pt());
              }
            }
          }
          else if(HasFlag("unweighted")){
            weight_loose = 1.; weight_tight = 1.;
          }
        }

        if(_isResolvedPreselection){

          region = "Preselection_isResolvedPreselection";
          region_path = path+"/"+region;
          double weight_ResPresel = weight * weight_tight;
          if(HasFlag("debug")) cout << region << endl;

          CopyHist(path+"/Cutflow",region_path+"/Cutflow");
          FillHist(region_path+"/Cutflow",cut+1.,weight_ResPresel,10,0.,10.);
          FillHist(region_path+"/MET",METv.Pt(),weight_ResPresel,2500,0.,2500.);
          FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_ResPresel);
          FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_ResPresel);

          if(leptons.at(0)->IsElectron()){
            FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_ResPresel);
            FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_ResPresel);
          }
          
          else if(leptons.at(0)->IsMuon()){
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_ResPresel);
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_ResPresel);
          }

        }

        if(_isStrictResolvedPreselection){

          region = "Preselection_isStrictResolvedPreselection";
          region_path = path+"/"+region;
          double weight_ResPresel = weight * weight_tight;
          if(HasFlag("debug")) cout << region << endl;

          CopyHist(path+"/Cutflow",region_path+"/Cutflow");
          FillHist(region_path+"/Cutflow",cut+1.,weight_ResPresel,10,0.,10.);
          FillHist(region_path+"/MET",METv.Pt(),weight_ResPresel,2500,0.,2500.);
          FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_ResPresel);
          FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_ResPresel);

          if(leptons.at(0)->IsElectron()){
            FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_ResPresel);
            FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_ResPresel);
          }
          
          else if(leptons.at(0)->IsMuon()){
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_ResPresel);
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_ResPresel);
          }

        }

        std::vector<string> METcuts = {"0","50","100","150","200"};

        if(_isBoostedPreselection){

          for(const auto METcut : METcuts){

            region = "Preselection_isBoostedPreselection";
            double d_METcut = std::stod(METcut);

            if(d_METcut > 0.) region += "_MET"+METcut;
            else if(METcut == "0") region = "Preselection_isBoostedPreselection";

            if(METv.Pt()<d_METcut) continue;
            region_path = path+"/"+region;
            double weight_BstPresel = weight * weight_loose;
            if(HasFlag("debug")) cout << region << endl;

            CopyHist(path+"/Cutflow",region_path+"/Cutflow");
            FillHist(region_path+"/Cutflow",cut+1.,weight_BstPresel,10,0.,10.);
            FillHist(region_path+"/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);

            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }

            FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);

            if(LooseLeps.at(0)->IsElectron()){
              FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
              FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
              for(unsigned int i=0;i<LooseLeps.size();i++){
                FillHist(region_path+"_ElTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
              }
            }

            else if(LooseLeps.at(0)->IsMuon()){
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
              for(unsigned int i=0;i<LooseLeps.size();i++){
                FillHist(region_path+"_MuTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
              }
            }
          
          }

        }

        if(_isBoostedPreselectionTest){

          region = "Preselection_isBoostedPreselectionTest";
          region_path = path+"/"+region;
          double weight_BstPresel = weight * weight_loose;
          if(HasFlag("debug")) cout << region << endl;

          CopyHist(path+"/Cutflow",region_path+"/Cutflow");
          FillHist(region_path+"/Cutflow",cut+1.,weight_BstPresel,10,0.,10.);
          FillHist(region_path+"/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
          for(unsigned int i=0;i<LooseLeps.size();i++){
            FillHist(region_path+"/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
          }
          FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
          FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);

          if(LooseLeps.at(0)->IsElectron()){
            FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"_ElTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }
          }

          else if(LooseLeps.at(0)->IsMuon()){
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"_MuTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }
          }

        }

        if(_isBoostedNonisoLepPreselection){

          region = "Preselection_isBoostedNonisoLepPreselection";
          region_path = path+"/"+region;
          double weight_BstPresel = weight * weight_loose;
          if(HasFlag("debug")) cout << region << endl;

          CopyHist(path+"/Cutflow",region_path+"/Cutflow");
          FillHist(region_path+"/Cutflow",cut+1.,weight_BstPresel,10,0.,10.);
          FillHist(region_path+"/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
          for(unsigned int i=0;i<LooseLeps.size();i++){
            FillHist(region_path+"/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
          }
          FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
          FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);

          if(LooseLeps.at(0)->IsElectron()){
            FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillHist(region_path+"_ElTau/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"_ElTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }
          }

          else if(LooseLeps.at(0)->IsMuon()){
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillHist(region_path+"_MuTau/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"_MuTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }
          }
        }

        if(_isBoostedNonisoLepPreselectionTest){

          region = "Preselection_isBoostedNonisoLepPreselectionTest";
          region_path = path+"/"+region;
          double weight_BstPresel = weight * weight_loose;
          if(HasFlag("debug")) cout << region << endl;

          CopyHist(path+"/Cutflow",region_path+"/Cutflow");
          FillHist(region_path+"/Cutflow",cut+1.,weight_BstPresel,10,0.,10.);
          FillHist(region_path+"/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
          for(unsigned int i=0;i<LooseLeps.size();i++){
            FillHist(region_path+"/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
          }
          FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
          FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);

          if(LooseLeps.at(0)->IsElectron()){
            FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillHist(region_path+"_ElTau/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"_ElTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }
            FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
          }

          else if(LooseLeps.at(0)->IsMuon()){
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillHist(region_path+"_MuTau/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"_MuTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
          }
        }


        // Blinding SRs
        if(!IsDATA){ 

          // SRs
          bool isBoosted(false); 
          bool isResolved(false);

          // CRs
          bool isResolvedLowMassCR(false);
          bool isBoostedLowMassCR(false);

          // Resolved SR 
          vector<Jet> jets_ResolvedSR;
          vector<Particle> leptons_ResolvedSR;
          Lepton * lepton_ResolvedSR;

          if(_isResolvedPreselection){
            jets_ResolvedSR = {jets.at(0),jets.at(1)};
            if(leptons.at(0)->Pt()>53){
              leptons_ResolvedSR = {(Particle)taus.at(0),(Particle)*leptons.at(0)};
              vector<double> dRlj;
              for(const auto &lep : leptons_ResolvedSR){
                for(const auto &j : jets_ResolvedSR){
                  dRlj.push_back(j.DeltaR(lep));
                }
              }
              if(*min_element(dRlj.begin(),dRlj.end())<0.4) isResolved = false;
              else{
                isResolved = true;
                lepton_ResolvedSR = leptons.at(0);
              }
            }  
            Particle ditau = taus.at(0) + *leptons.at(0) + METv ;
            Particle wr = ditau + jets.at(0) + jets.at(1) ;
            if(ditau.M()>200 && wr.M()<800) isResolvedLowMassCR = true ;
          }

          // Boosted SR
          vector<double> mll;
          vector<FatJet> fatjets_BoostedSR;
          vector<Lepton *> leptons_BoostedSR;
          FatJet fatjet_BoostedSR;
          Lepton * lepton_BoostedSR;
          Particle ll;

          if(!isResolved && _isBoostedPreselection){
            for(const auto &looselep : LooseLeps){
              ll = *looselep + taus.at(0);
              mll.push_back(ll.M());
            }
            for(const auto &J : fatjets){
              if(J.DeltaPhi(taus.at(0))>2.0) fatjets_BoostedSR.push_back(J);
            }
            if(fatjets_BoostedSR.size()>0){
              fatjet_BoostedSR = fatjets_BoostedSR.at(0);
              for(const auto &looselep : LooseLeps){
                if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
              }
              if(leptons_BoostedSR.size()>0){
                isBoosted = true;
                lepton_BoostedSR = leptons_BoostedSR.at(0);
              }
            }
            Particle ditau = taus.at(0) + *LooseLeps.at(0) + METv;
            Particle wr = taus.at(0) + AddFatJetAndLepton(fatjet_BoostedSR.at(0),*LooseLeps.at(0)) + METv;
            if(ditau.M()>200 && wr.M()<800) isBoostedLowMassCR = true;
          }

          // Fill CRs
          if(isBoostedLowMassCR){
            region = "BoostedLowMassCR"
            region_path = path+"/"+region; 
            double weight_BstLmCR = weight * weight_loose;
            CopyHist(path+"/isBoostedPreselection/Cutflow",region_path+"/Cutflow");
            FillHist(region_path+"/Cutflow",cut+2.,weight_BstSR,10,0.,10.);
            FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstLmCR);
            FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstLmCR);

            if(LooseLeps.at(0)->IsElectron()){
              FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstLmCR);
              FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstLmCR);
            }
          
            else if(LooseLeps.at(0)->IsMuon()){
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstLmCR);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstLmCR);
            }
          }

          if(isResolvedLowMassCR){
            region = "ResolvedLowMassCR"
            region_path = path+"/"+region; 
            double weight_RsvLmCR = weight * weight_tight;
            CopyHist(path+"/isResolvedPreselection/Cutflow",region_path+"/Cutflow");
            FillHist(region_path+"/Cutflow",cut+2.,weight_BstSR,10,0.,10.);
            FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
            FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvLmCR);

            if(LooseLeps.at(0)->IsElectron()){
              FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
              FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
            }
          
            else if(LooseLeps.at(0)->IsMuon()){
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
            }
          }

          // Fill SRs
          if(isBoosted && !isResolved){
            region = "BoostedSR";
            region_path = path+"/"+region;

            if(HasFlag("debug")) cout << region << endl;
            double weight_BstSR = weight*weight_loose;
            FillHist(path+"/Region",0,weight_BstSR,2,0.,2.);
            CopyHist(path+"/isBoostedPreselection/Cutflow",region_path+"/Cutflow");
            FillHist(region_path+"/Cutflow",cut+2.,weight_BstSR,10,0.,10.);
            FillPreselHists(region_path,taus,jets,bjets,fatjets_BoostedSR,leptons_BoostedSR,leptons,weight_BstSR);
            FillMassHists(region_path,METv,taus,jets,fatjets_BoostedSR,leptons_BoostedSR,leptons,weight_BstSR);

            if(leptons_BoostedSR.at(0)->IsElectron()){
              FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons_BoostedSR,weight_BstSR);
              FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstSR);
            }
          
            else if(leptons_BoostedSR.at(0)->IsMuon()){
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons_BoostedSR,weight_BstSR);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstSR);
            }

          }

          if(!isBoosted && isResolved){
            region = "ResolvedSR";
            region_path = path+"/"+region;

            if(HasFlag("debug")) cout << region << endl;
            double weight_RsvSR = weight*weight_tight;
            FillHist(path+"/Region",1,weight_RsvSR,2,0.,2.);
            CopyHist(path+"/isResolvedPreselection/Cutflow",region_path+"/Cutflow");
            FillHist(region_path+"/Cutflow",cut+2.,weight_RsvSR,10,0.,10.);
            FillHist(region_path+"/mll",ll.M(),weight_RsvSR,5000,0.,5000.);
            FillPreselHists(region_path,taus,jets_ResolvedSR,bjets,fatjets,LooseLeps,leptons,weight_RsvSR);
            FillMassHists(region_path,METv,taus,jets_ResolvedSR,fatjets,LooseLeps,leptons,weight_RsvSR);

            if(leptons.at(0)->IsElectron()){
              FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvSR);
              FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvSR);
            }
          
            else if(leptons.at(0)->IsMuon()){
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvSR);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvSR);
            }

          }

          else continue;

        }

      }
    }
  }

}

WRTau_SR_Test::WRTau_SR_Test(){

}

WRTau_SR_Test::~WRTau_SR_Test(){

}


