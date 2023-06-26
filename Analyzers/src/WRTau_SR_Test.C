#include "WRTau_SR_Test.h"

void WRTau_SR_Test::initializeAnalyzer(){

  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {3,4,5}; vEl_vec = {9,13}; vMu_vec = {18,21};
  

  for(const auto &vjet : vJet_vec){
    for(const auto &vel : vEl_vec){
      std::pair<int,int> idpair = std::make_pair(vjet,vel);
      tauidsftool_map[idpair] = new TauIDSFTool("UL"+std::to_string(DataYear),DeepTauVSjet,idname_map_str[vjet]);
    }
  }

  if(DataYear==2017){

    SingleElectronTriggers = {"HLT_Ele35_WPTight_Gsf_v"};
    SingleMuonTriggers = {"HLT_Mu50_v"};
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
    SingleLeptonTriggers = {"HLT_Mu27_v","HLT_Ele35_WPTight_Gsf_v"};

  }

}

void WRTau_SR_Test::executeEvent(){


  //FillTimer("START_EV");

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
  AllLHEs = GetLHEs();

  executeEventFromParameter(param);

  //FillTimer("END_EV");
  
}

void WRTau_SR_Test::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);

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
  vector<Muon> this_AllMuons_ = UseTunePMuon(AllMuons);
  vector<Muon> this_AllMuons;
  vector<Electron> this_AllElectrons;


  // b-tagging
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

  //cout << "====" << endl; 
  //cout << "this_AllMuons , this_AllElectrons " << this_AllMuons.size() << " " << this_AllElectrons.size() << endl ; 

  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.4) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 50., 2.4) ;
  vector<Muon> muons_loose = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 50., 2.4) ;

  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50., 2.4);
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 50., 2.4);
  vector<Electron> electrons_loose = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 50.,2.4);

  vector<Lepton *> leptons_tmp = CombineLeptonPointerVector(electrons,muons);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);
  vector<Lepton *> LooseLeps_tmp = CombineLeptonPointerVector(electrons_loose,muons_loose);
  
  //cout << "AllMuons , AllEles : " << AllMuons.size() << " " << AllElectrons.size() <<  endl;
  //cout << "TightMuons , VetoMuons , LooseMuons : " << muons.size() << " " << muons_veto.size() << " " << muons_loose.size() << endl;
  //cout << "TightEles , VetoEles , LooseEles : " << electrons.size() << " " << electrons_veto.size() << " " << electrons_loose.size() << endl;


  vector<Tau> taus_lepVeto = VetoLeptonsFromTaus(VetoLeps,this_AllTaus);

  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){

        TString idname = "vJet"+idname_map[vJet_vec.at(i)]+"_vEl"+idname_map[vEl_vec.at(j)]+"_vMu"+idname_map[vMu_vec.at(k)];
        TString path = param.Name+"/"+idname;
        vector<Tau> taus_temp = SelectTaus_varWP(taus_lepVeto,vJet_vec[i],vEl_vec[j],vMu_vec[k],50,2.4);
        
        vector<Tau> taus;

        if(taus_temp.size()>0){
          for(unsigned int i =0 ; i < taus_temp.size() ; i++){
            FillHist(path+"/TauType"+TString::Itoa(i,10),GetTauType(taus_temp.at(i),AllGens),weight,20,-10.,10.);
          }
        }
        if(HasFlag("NonpromptTau")) taus = TauFakeOnly(taus_temp,AllGens);
        else if(HasFlag("PromptTau")) taus = TauPromptOnly(taus_temp,AllGens);
        std::sort(taus.begin(),taus.end(),PtComparing);

        if(HasFlag("GenMatchDebug") && taus.size()>0){
          continue;
          vector<Gen> debugtaugens;
          for(const auto &tau : taus) debugtaugens.push_back(GetClosestTauGen(tau,AllGens));
          //PrintPartialGen(debugtaugens,AllGens);
          for(const auto &gen : debugtaugens){
            continue;
            int currentidx = gen.Index();
            int motheridx = gen.MotherIndex();
            int counter(0);
            cout << "===========================================================" << endl;
            cout << "RunNumber:EventNumber = " << run << ":" << event << endl;
            cout << "index\tPID\tStatus\tMIdx\tMPID\tPt\tEta\tPhi\tM" << endl;
            cout <<   counter << "\t"   << currentidx << "\t" << gen.PID() << "\t" << gen.Status() << "\t" << gen.MotherIndex() <<  endl;
            while(motheridx>1){
              counter++;
              cout << "daughter,mother : " << currentidx << " " << motheridx << endl;
              currentidx = motheridx;
              motheridx = AllGens.at(motheridx).MotherIndex();
              Gen updatedgen = AllGens.at(motheridx);
              cout << counter << "\t" << currentidx << "\t" << updatedgen.PID() << "\t" << updatedgen.Status() << "\t" << updatedgen.MotherIndex() << "\t";
              printf("%.2f\t%.2f\t%.2f\t%.2f\n",updatedgen.Pt(), updatedgen.Eta(), updatedgen.Phi(), updatedgen.M());
              if(motheridx<2 || updatedgen.PID() != 2212 ) continue;
            }
          }
          continue;
        }

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
        std::pair<std::vector<Lepton *>,std::vector<Lepton *>> test = std::make_pair(LooseLeps,leptons);

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

        FillHist(path+"/nLooseLep",LooseLeps.size(),weight,10,0.,10.);
        FillHist(path+"/nLooseMu",muons_loose.size(),weight,10,0.,10.);
        FillHist(path+"/nLooseEl",electrons_loose.size(),weight,10,0.,10.);
        if(HasFlag("wjdebug")) continue;

        FillClosestTauLHE(taus.at(0),AllLHEs,path+"/BeforeLooseLepSel",weight);
        FillClosestTauGen(taus.at(0),AllGens,path+"/BeforeLooseLepSel",weight);

        if(LooseLeps.size()!=1) continue;
        FillHist(path+"/Cutflow",4.,weight,10,0.,10.);

        FillClosestTauLHE(taus.at(0),AllLHEs,path,weight);
        FillClosestTauGen(taus.at(0),AllGens,path,weight);
        
        double cut = 4.;

        if(HasFlag("METcut")){
          cut += 1;
          if(METv.Pt()<100) continue;
          FillHist(path+"/Cutflow",cut,weight,10,0.,10.);
        }

        if(HasFlag("inverseMETcut")){
          cut += 1;
          if(METv.Pt()>100) continue;
          FillHist(path+"/Cutflow",cut,weight,10,0.,10.);
        }

        if(HasFlag("Nb0")){
          cut += 1;
          if(!bjets.size()==0) continue;
          FillHist(path+"/Cutflow",cut,weight,10,0.,10.);
        }

        if(HasFlag("NbGt0")){
          cut += 1;
          if(bjets.size()==0) continue;
          FillHist(path+"/Cutflow",cut,weight,10,0.,10.);
        }

        //PrintGen(AllGens);

        TString region = "Preselection";
        TString region_path = path+"/"+region;

        double tau_weight(1.);

        if(!IsDATA){
          
          // TAU POG ID SF 
          // https://github.com/cms-tau-pog/TauIDSFs/tree/master#c
          
          if(!HasFlag("unweighted")){

            std::pair<int,int> idpair = std::make_pair(vJet_vec.at(i),vEl_vec.at(j));

            tau_weight *= tauidsftool_map[idpair]->getSFvsPT(taus.at(0).Pt());
            FillTimer("GetTauWeight"+idname);
            if(HasFlag("NonpromptTau")) tau_weight = 1.0;

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
        bool _isWJetsCR(false);
        bool _isQCDrichCR(false);

        //isResolvedPreselection = hasAtLeast2AK4Jets && (leptons.size()>0);
        //isBoostedPreselection = hasAtLeast1AK8Jets && (LooseLeps.size()>0);
        _isResolvedPreselection = hasAtLeast2AK4Jets && hasAtLeast1TightLeptons;
        _isStrictResolvedPreselection = hasAtLeast2AK4Jets && leptons.size()==1;
        _isBoostedPreselection = !_isResolvedPreselection && hasAtLeast1AK8Jets;
        _isBoostedPreselectionTest = !hasAtLeast2AK4Jets && hasAtLeast1AK8Jets;
        _isWJetsCR = leptons.size()==1 ;


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
          else if(HasFlag("unweighted") || HasFlag("NonpromptLepton")){
            weight_loose = 1.; weight_tight = 1.;
          }
        }

        if(_isWJetsCR){

          region = "WJetsCR";
          region_path = path+"/"+region;
          double weight_WJets = weight * weight_loose;
          if(HasFlag("debug")) cout << region << endl;

          CopyHist(path+"/Cutflow",region_path+"/Cutflow");
          FillHist(region_path+"/Cutflow",cut+1.,weight_WJets,10,0.,10.);
          FillHist(region_path+"/MET",METv.Pt(),weight_WJets,2500,0.,2500.);
          FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_WJets);
          FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_WJets);

          double mt = MT(*LooseLeps.at(0),METv);

          if(LooseLeps.at(0)->IsElectron()){
            CopyHist(path+"/Cutflow",region_path+"_ElTau/Cutflow");
            FillHist(region_path+"_ElTau/Cutflow",cut+1.,weight_WJets,10,0.,10.);
            FillHist(region_path+"_ElTau/MET",METv.Pt(),weight_WJets,2500,0.,2500.);
            FillHist(region_path+"_ElTau/mt",mt,weight_WJets,500,0.,500.);
            FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_WJets);
            FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_WJets);
          }
          
          else if(LooseLeps.at(0)->IsMuon()){
            CopyHist(path+"/Cutflow",region_path+"_MuTau/Cutflow");
            FillHist(region_path+"_MuTau/Cutflow",cut+1.,weight_WJets,10,0.,10.);
            FillHist(region_path+"_MuTau/MET",METv.Pt(),weight_WJets,2500,0.,2500.);
            FillHist(region_path+"_MuTau/mt",mt,weight_WJets,500,0.,500.);
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_WJets);
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_WJets);
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
            CopyHist(path+"/Cutflow",region_path+"_ElTau/Cutflow");
            FillHist(region_path+"_ElTau/Cutflow",cut+1.,weight_ResPresel,10,0.,10.);
            FillHist(region_path+"_ElTau/MET",METv.Pt(),weight_ResPresel,2500,0.,2500.);
            FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_ResPresel);
            FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_ResPresel);
          }
          
          else if(leptons.at(0)->IsMuon()){
            CopyHist(path+"/Cutflow",region_path+"_MuTau/Cutflow");
            FillHist(region_path+"_MuTau/Cutflow",cut+1.,weight_ResPresel,10,0.,10.);
            FillHist(region_path+"_MuTau/MET",METv.Pt(),weight_ResPresel,2500,0.,2500.);
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_ResPresel);
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_ResPresel);
          }

        }

        /*if(_isStrictResolvedPreselection){

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

        }*/

        std::vector<string> METcuts = {"0"};

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
              CopyHist(path+"/Cutflow",region_path+"_ElTau/Cutflow");
              FillHist(region_path+"_ElTau/Cutflow",cut+1.,weight_BstPresel,10,0.,10.);
              FillHist(region_path+"_ElTau/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
              FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
              FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
              for(unsigned int i=0;i<LooseLeps.size();i++){
                FillHist(region_path+"_ElTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
              }
              FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_ElTau",weight_BstPresel);
            }

            else if(LooseLeps.at(0)->IsMuon()){
              CopyHist(path+"/Cutflow",region_path+"_MuTau/Cutflow");
              FillHist(region_path+"_MuTau/Cutflow",cut+1.,weight_BstPresel,10,0.,10.);
              FillHist(region_path+"_MuTau/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
              for(unsigned int i=0;i<LooseLeps.size();i++){
                FillHist(region_path+"_MuTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
              }
              FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_MuTau",weight_BstPresel);
            }
          
          }

        }

        /*if(_isBoostedPreselectionTest){

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
            FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_ElTau",weight_BstPresel);
          }

          else if(LooseLeps.at(0)->IsMuon()){
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"_MuTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }
            FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_MuTau",weight_BstPresel);
          }

        }*/

        /*if(_isBoostedNonisoLepPreselection){

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
            FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_ElTau",weight_BstPresel);
          }

          else if(LooseLeps.at(0)->IsMuon()){
            FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstPresel);
            FillHist(region_path+"_MuTau/MET",METv.Pt(),weight_BstPresel,2500,0.,2500.);
            for(unsigned int i=0;i<LooseLeps.size();i++){
              FillHist(region_path+"_MuTau/dRl"+TString::Itoa(i,10)+"tau",fatjets.at(0).DeltaR(*LooseLeps.at(0)),weight_BstPresel,60,0.,6.);
            }
            FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_MuTau",weight_BstPresel);
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
        }*/


        // Blinding SRs
        if(true){ 

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
                Particle ditau = taus.at(0) + *leptons.at(0);
                Particle wr = ditau + jets.at(0) + jets.at(1) ;
                double MT_ditau = MT(ditau,METv);
                double MT_wr = MT(wr,METv);
                if(MT_ditau>200 && MT_wr<800) isResolvedLowMassCR = true ;
                else if(MT_ditau>400 && MT_wr>800) isResolved = true;
                lepton_ResolvedSR = leptons.at(0);
              }
            }  
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
                Particle ditau = taus.at(0) + *LooseLeps.at(0);
                Particle wr = taus.at(0) + AddFatJetAndLepton(fatjet_BoostedSR,*LooseLeps.at(0));
                double MT_ditau = MT(ditau,METv);
                double MT_wr = MT(wr,METv);
                if(MT_ditau>200 && MT_wr<800) isBoostedLowMassCR = true ;
                else if(MT_ditau>200 && MT_wr>800) isBoosted = true;
                lepton_BoostedSR = leptons_BoostedSR.at(0);
              }
            }
          }

          // Fill CRs
          if(isBoostedLowMassCR){
            region = "BoostedLowMassCR";
            region_path = path+"/"+region; 
            double weight_BstLmCR = weight * weight_loose;
            CopyHist(path+"/isBoostedPreselection/Cutflow",region_path+"/Cutflow");
            FillHist(region_path+"/MET",METv.Pt(),weight_BstLmCR,2500,0.,2500.);
            FillHist(region_path+"/Cutflow",cut+2.,weight_BstLmCR,10,0.,10.);
            FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstLmCR);
            FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstLmCR);

            if(LooseLeps.at(0)->IsElectron()){
              CopyHist(path+"/isBoostedPreselection/Cutflow",region_path+"_ElTau/Cutflow");
              FillHist(region_path+"_ElTau/Cutflow",cut+2.,weight_BstLmCR,10,0.,10.);
              FillHist(region_path+"_ElTau/MET",METv.Pt(),weight_BstLmCR,2500,0.,2500.);
              FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstLmCR);
              FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstLmCR);
              FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_ElTau",weight_BstLmCR);
            }
          
            else if(LooseLeps.at(0)->IsMuon()){
              CopyHist(path+"/isBoostedPreselection/Cutflow",region_path+"_MuTau/Cutflow");
              FillHist(region_path+"_MuTau/Cutflow",cut+2.,weight_BstLmCR,10,0.,10.);
              FillHist(region_path+"_MuTau/MET",METv.Pt(),weight_BstLmCR,2500,0.,2500.);
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_BstLmCR);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_BstLmCR);
              FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_MuTau",weight_BstLmCR);
            }
          }

          if(isResolvedLowMassCR){
            region = "ResolvedLowMassCR";
            region_path = path+"/"+region; 
            double weight_RsvLmCR = weight * weight_tight;
            CopyHist(path+"/isResolvedPreselection/Cutflow",region_path+"/Cutflow");
            FillHist(region_path+"/MET",METv.Pt(),weight_RsvLmCR,2500,0.,2500.);
            FillHist(region_path+"/Cutflow",cut+2.,weight_RsvLmCR,10,0.,10.);
            FillPreselHists(region_path,taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
            FillMassHists(region_path,METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvLmCR);

            if(LooseLeps.at(0)->IsElectron()){
              CopyHist(path+"/isResolvedPreselection/Cutflow",region_path+"_ElTau/Cutflow");
              FillHist(region_path+"_ElTau/Cutflow",cut+2.,weight_RsvLmCR,10,0.,10.);
              FillHist(region_path+"_ElTau/MET",METv.Pt(),weight_RsvLmCR,2500,0.,2500.);
              FillPreselHists(region_path+"_ElTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
              FillMassHists(region_path+"_ElTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
              FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_ElTau",weight_RsvLmCR);
            }
          
            else if(LooseLeps.at(0)->IsMuon()){
              CopyHist(path+"/isResolvedPreselection/Cutflow",region_path+"_MuTau/Cutflow");
              FillHist(region_path+"_MuTau/Cutflow",cut+2.,weight_RsvLmCR,10,0.,10.);
              FillHist(region_path+"_MuTau/MET",METv.Pt(),weight_RsvLmCR,2500,0.,2500.);
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvLmCR);
              FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_MuTau",weight_RsvLmCR);
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
              FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_ElTau",weight_RsvSR);
            }
          
            else if(leptons.at(0)->IsMuon()){
              FillPreselHists(region_path+"_MuTau",taus,jets,bjets,fatjets,LooseLeps,leptons,weight_RsvSR);
              FillMassHists(region_path+"_MuTau",METv,taus,jets,fatjets,LooseLeps,leptons,weight_RsvSR);
              FillClosestTauLHE(taus.at(0),AllLHEs,region_path+"_MuTau",weight_RsvSR);
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


