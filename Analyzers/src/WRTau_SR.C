#include "WRTau_SR.h"

void WRTau_SR::initializeAnalyzer(){


  if(DataYear==2017){

    singlelepton_trigger = {"HLT_IsoMu24_v10","HLT_Ele35_WPTight_Gsf_v7"};

    singletau_highpt_trigger = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v8"};

    singletau_trigger = {"HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_v8"};

    tau_met_trigger = {"HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET90_v8"};

    etau_trigger = {
      "HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTau30_eta2p1_CrossL1_v9"
    };

    mutau_trigger = {
      "HLT_IsoMu24_eta2p1_LooseChargedIsoPFTau20_SingleL1_v8"
    };

  }

}

void WRTau_SR::executeEvent(){

  AnalyzerParameter param;
  /*param.Clear();

  param.Name = "WRTauStudy";
  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "passVetoID";

  trigger = singletau_highpt_trigger;
  safeptcut = 190;
  triggerlist = {param.Name+"/SingleTauTrigger"};

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPt";
  param.Muon_Veto_ID = "POGLoose";

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
  triggerlist.clear();

  param.Name = "WRTauStudy";
  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "passVetoID";

  trigger = singlelepton_trigger;
  safeptcut = 50;
  triggerlist = {param.Name+"/SingleLeptonTrigger"};

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPt";
  param.Muon_Veto_ID = "POGLoose";

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

  /*param.Clear();

  param.Name = "WRTauStudy_IDTest_POGMedium";
  param.Electron_Tight_ID = "passMediumID";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "passVetoID";

  param.Muon_Tight_ID = "POGMedium";
  param.Muon_Loose_ID = "POGLoose";
  param.Muon_Veto_ID = "POGLoose";

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

}

void WRTau_SR::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);

  //vector<TString> triggerlist = {"SingleTauTrigger_Old_PlaceHolder","SingleTauTrigger"};
  //vector<TString> triggerlist = {param.Name+"/SingleTauTrigger"};

  vector<int> vJet_vec = {5,6};   //vL,L,M,T,vT
  vector<int> vEl_vec = {11,12,13};     //L,M,T 
  vector<int> vMu_vec = {19,20,21};     //L,M,T

  if(HasFlag("PreselOnly")){
    vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
    vJet_vec = {5}; vEl_vec = {11,13}; vMu_vec = {19,21};
  }

  map<int,TString> idname_map = {{2,"vJetVLoose"},{3,"vJetLoose"},{4,"vJetMedium"},{5,"vJetTight"},{6,"vJetVTight"},
                                 {11,"vElLoose"},{12,"vElMedium"},{13,"vElTight"},
                                 {19,"vMuLoose"},{20,"vMuMedium"},{21,"vMuTight"}};

  vector<Muon> this_AllMuons = AllMuons;
  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Lepton *> allleptons = CombineLeptonPointerVector(this_AllElectrons,this_AllMuons);
  
  // Sample has all flavours, get only events with gen pid 9900016
  // Also filter out events with #taul > 0  
  bool isTauChannel = false;
  int nLeptonicTau = 0;
  for(const auto &gen : AllGens){
    if(gen.PID()==9900016){
      isTauChannel = true;
      break;
    }
  }

  bool isChannelOfInterest = false;
  bool GenFilter;
  int nHadTauFromWR = 0;
  int nLepTauFromN = 0;
  int nLepTauFromWR = 0;

  Gen GenLepTauFromN;
  Gen GenLepTauFromWR;

  vector<Gen> NDecayProduct;

  if(isTauChannel){
    for(unsigned int i=2; i<AllGens.size();i++){
      Gen gen = AllGens.at(i);
      if(fabs(gen.PID())==11 || fabs(gen.PID())==13){
        //cout << "gen #" << i << endl;
        //cout << "Midx : " << gen.MotherIndex() << endl;
        //cout << "grMidx : " << AllGens.at(gen.MotherIndex()).MotherIndex() << endl;
        if(AllGens.at(gen.MotherIndex()).MotherIndex() > 0){
          Gen grandmother = AllGens.at(AllGens.at(gen.MotherIndex()).MotherIndex());
          if(fabs(AllGens.at(gen.MotherIndex()).PID())==15){
            //cout << "grMidx : " << AllGens.at(gen.MotherIndex()).MotherIndex() << endl;
            if(fabs(grandmother.PID())==9900016){
              nLepTauFromN += 1;
              GenLepTauFromN = AllGens.at(i);
            }
            if(fabs(grandmother.PID())==34){
              nLepTauFromWR += 1;
              GenLepTauFromWR = AllGens.at(i);
            }
          }
        }
      }
      if(AllGens.at(gen.MotherIndex()).PID()==9900016){
        if(fabs(gen.PID())!=15 || fabs(gen.PID())!=9900016) NDecayProduct.push_back(gen);
      }
    }
  }

  isChannelOfInterest = (nLepTauFromWR == 0) && (nLepTauFromN == 1);

  if(HasFlag("GenFilter_TauOnly")) GenFilter = isTauChannel;
  else if(HasFlag("GenFilter_DiHadronicVeto")) GenFilter = isTauChannel && isChannelOfInterest;
  else GenFilter = isTauChannel && ((nLepTauFromN + nLepTauFromWR)>0);
  
  if(MCSample.Contains("WRTau")){
    if(!GenFilter) return;
  }

  //PrintGen(AllGens);

  /*if(NDecayProduct.size()>1){
    FillHist(param.Name+"/Gen/dRqq",NDecayProduct.at(0).DeltaR(NDecayProduct.at(1)),weight,60,0.,6.);
    for(unsigned int i=0; i<NDecayProduct.size(); i++){
      FillHist(param.Name+"/Gen/dRq"+TString::Itoa(i,10)+"lTau",NDecayProduct.at(i).DeltaR(GenLepTauFromN),weight,60,0.,6.);
    }
  }*/


  bool PassOneTrigger = false;
  TString trglumi_key;
  TString triggerflag = "";

  if(ev.PassTrigger(trigger)){
    triggerflag = triggerlist.at(0);
    if(ev.PassTrigger("HLT_IsoMu24_v10") && !ev.PassTrigger("HLT_Ele35_WPTight_Gsf_v7")){
      trglumi_key = "HLT_IsoMu24_v10";
      PassOneTrigger = true;
    }
    else if(!ev.PassTrigger("HLT_IsoMu24_v10") && ev.PassTrigger("HLT_Ele35_WPTight_Gsf_v7")){
      trglumi_key = "HLT_Ele35_WPTight_Gsf_v7";
      PassOneTrigger = true;
    }
    else if(ev.PassTrigger("HLT_IsoMu24_v10") && ev.PassTrigger("HLT_Ele35_WPTight_Gsf_v7")) return;
  }

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0);
  }

  int GenRegion = 0;
  if(isChannelOfInterest) GenRegion = 1;
  FillHist(param.Name+"/GenMatched_Region",GenRegion,weight,2,0.,2.);

  for(const auto &trg : triggerlist){
    for(const auto &vJ : vJet_vec){
      for(const auto &vE : vEl_vec){
        for(const auto &vM : vMu_vec){
          TString idname = idname_map[vJ]+"_"+idname_map[vE]+"_"+idname_map[vM];
          TString path = trg+"/"+idname;
          FillHist(path+"/Cutflow",0.,weight,10,0.,10.);
        }
      }
    }
  }

  if(!PassOneTrigger) return;


  for(const auto &trg : triggerlist){
    for(const auto &vJ : vJet_vec){
      for(const auto &vE : vEl_vec){
        for(const auto &vM : vMu_vec){
          TString idname = idname_map[vJ]+"_"+idname_map[vE]+"_"+idname_map[vM];
          TString path = trg+"/"+idname;
          FillHist(path+"/Cutflow",1.,weight,10,0.,10.);
        }
      }
    }
  }

  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;
  vector<Tau> this_AllTaus = AllTaus;

  // Muons
  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.1) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 50., 2.1) ;
  vector<Muon> muons_loose = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 50., 2.1) ;

  // Electrons
  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50., 2.1);
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 50., 2.1);
  vector<Electron> electrons_loose = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 50.,2.1);

  // CB POG ID TEST
  vector<Electron> electrons_medium = SelectElectrons(this_AllElectrons,"passMediumID", 50., 2.1);
  vector<Muon> muons_medium = SelectMuons(this_AllMuons,"POGMedium", 50., 2.1);
  vector<Electron> electrons_tight = SelectElectrons(this_AllElectrons,"passTightID", 50., 2.1);
  vector<Muon> muons_tight = SelectMuons(this_AllMuons,"POGTight", 50., 2.1);

  // Lepton Combination
  vector<Lepton *> leptons = CombineLeptonPointerVector(electrons,muons);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);
  vector<Lepton *> LooseLeps_tmp = CombineLeptonPointerVector(electrons_loose,muons_loose);
  vector<Lepton *> MediumLeps = CombineLeptonPointerVector(electrons_medium,muons_medium);
  vector<Lepton *> TightLeps = CombineLeptonPointerVector(electrons_tight,muons_tight);

  // Tau removal with VetoLeps
  vector<Tau> taus_lepVeto;
  for(const auto &th : this_AllTaus){
    bool HasVetoLepInside = false;
    for(const auto &lep : VetoLeps){
      if(th.DeltaR(*lep)<0.4){
        HasVetoLepInside = true;
        break;
      }
    }
    if(HasVetoLepInside) continue;
    taus_lepVeto.push_back(th);
  }

  FillHist(triggerflag+"/nAK4_noSel",this_AllJets.size(),weight,10,0.,10.);
  FillHist(triggerflag+"/nAK8_noSel",this_AllFatJets.size(),weight,10,0.,10.);

  // Pt Sorting
  std::sort(muons.begin(),muons.end(),PtComparing); 
  std::sort(electrons.begin(),electrons.end(),PtComparing);
  std::sort(allleptons.begin(),allleptons.end(),PtComparingPtr);

  // Define leptons of interest - userflags used to vary IDs to test
  vector<Lepton *> AnalysisLeptons = leptons;
  if(HasFlag("ElectronIDTest")) AnalysisLeptons = LooseLeps_tmp;
  if(HasFlag("ElectronIDMediumTest")) AnalysisLeptons = MediumLeps;
  if(HasFlag("ElectronIDTightTest")) AnalysisLeptons = TightLeps; 
  std::sort(AnalysisLeptons.begin(),AnalysisLeptons.end(),PtComparingPtr);

  // Loop over all tau ID combinations
  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){

        TString idname = idname_map[vJet_vec.at(i)]+"_"+idname_map[vEl_vec.at(j)]+"_"+idname_map[vMu_vec.at(k)];
        vector<Tau> tempTau = SelectTaus_varWP(this_AllTaus,i,j,k,50,2.1);
        std::sort(tempTau.begin(),tempTau.end(),PtComparing);

        // AK4 removal with vetolep && alltau overlap
        vector<Jet> jets_tauVeto;
        for(const auto &j : this_AllJets){
          bool HasTauInside = false;
          for(const auto &th : tempTau){
            if(j.DeltaR(th)<0.4){
              HasTauInside = true;
              break;
            }
          }
          if(HasTauInside) continue;
          jets_tauVeto.push_back(j);
        }
      
        vector<Jet> jets_lepVeto_tauVeto = JetsVetoLeptonInside(jets_tauVeto,electrons_veto,muons_veto,0.4);
        vector<Jet> jets = SelectJets(jets_lepVeto_tauVeto, param.Jet_ID, 40., 2.1) ;
        std::sort(jets.begin(),jets.end(),PtComparing);

        TString path = triggerflag+"/"+idname;
        FillHist(path+"/nHadTau_noID_beforePresel",this_AllTaus.size(),weight,10,0.,10.);
        FillHist(path+"/nHadTau_beforePresel",tempTau.size(),weight,10,0.,10.);

        // Cut1(Preselection) : At least 1 hadronic tau
        if(tempTau.size()<1) continue;
        FillHist(path+"/Cutflow",2.,weight,10,0.,10.);

        // Cut2(Preselection) : Leading hadronic tau pT > 190 GeV
        if(tempTau.at(0).Pt()<190) continue;
        FillHist(path+"/Cutflow",3.,weight,10,0.,10.);
        FillHist(path+"/nLep_beforePresel",AnalysisLeptons.size(),weight,10,0.,10.);
        FillHist(path+"/nLep_all_beforePresel",allleptons.size(),weight,10,0.,10.);
        FillHist(path+"/nElTight_beforePresel",electrons.size(),weight,10,0.,10.);
        FillHist(path+"/nMuTight_beforePresel",muons.size(),weight,10,0.,10.);
        //FillLeptonPlots(leptons,path+"/Preselection/Leptons",weight);
        
        if(leptons.size()>0) FillHist(path+"/Preselection/dRtautau_Tight",tempTau.at(0).DeltaR(*leptons.at(0)),weight,60,0.,6.);

        // GenRegion Test
        FillHist(path+"/Preselection/GenMatched_Region",GenRegion,weight,2,0.,2.);

        // FatJet removal should be done with signal taus (temptau)
        // AK8 removal with vetolep && tau overlap
        vector<FatJet> fatjets_tmp;
        for(const auto &J : this_AllFatJets){
          bool HasTauInside = false;
          for(const auto &th : tempTau){
            if(J.DeltaR(th)<0.8){
              HasTauInside = true;
              break;
            }
          }
          if(HasTauInside) continue;
          fatjets_tmp.push_back(J);
        }

        // Remove LooseLeptons around signal hadronic tau
        vector<Lepton *> LooseLeps;
        for(const auto &lep : LooseLeps_tmp){
          bool HasHadronicTauInside = false;
          if(tempTau.at(0).DeltaR(*lep)<0.3){
            HasHadronicTauInside = true;
            break;
          }
          if(HasHadronicTauInside) continue;
          LooseLeps.push_back(lep);
        }

        //vector<FatJet> fatjets_lepveto = FatJetsVetoLeptonInside(fatjets_tmp,electrons_veto,muons_veto,0.8);
        //vector<FatJet> fatjets_lepveto = FatJetsVetoLeptonInside(fatjets_tmp,AnalysisLeptons,0.8);
        vector<FatJet> fatjets = SelectFatJets(fatjets_tmp,param.FatJet_ID,200.,2.1);


        // Preselection level GenMatching test
        /*for(unsigned int i=0;i<NDecayProduct.size();i++){
          for(unsigned int j=0; j<jets.size(); j++){
            FillHist(path+"/Preselection/Gen/dRq"+TString::Itoa(i,10)+"RecoAK4j"+TString::Itoa(j,10),NDecayProduct.at(i).DeltaR(jets.at(j)),weight,60,0.,6.);
          }
          for(unsigned int j=0; j<fatjets.size(); j++){
            FillHist(path+"/Preselection/Gen/dRq"+TString::Itoa(i,10)+"RecoAK8J"+TString::Itoa(j,10),NDecayProduct.at(i).DeltaR(fatjets.at(j)),weight,60,0.,6.);
          }
          for(unsigned int j=0; j<LooseLeps.size(); j++){
            FillHist(path+"/Preselection/Gen/dRq"+TString::Itoa(i,10)+"RecoLooselep"+TString::Itoa(j,10),NDecayProduct.at(i).DeltaR(*LooseLeps.at(j)),weight,60,0.,6.);
          }
        }*/

        std::sort(fatjets.begin(),fatjets.end(),PtComparing);
        std::sort(LooseLeps.begin(),LooseLeps.end(),PtComparingPtr);

        if(HasFlag("PreselOnly")){
          if(!(fatjets.size()>0)) continue;
          FillHist(path+"/Cutflow",4.,weight,10,0.,10.);
        }
        FillHist(path+"/Preselection/MET",METv.Pt(),weight,2500,0.,2500.);
        FillHist(path+"/Preselection/Tauh_pT",tempTau.at(0).Pt(),weight,5000,0.,5000.);
        FillLeptonPlots(AnalysisLeptons,path+"/Preselection/HighPtTight",weight);
        FillLeptonPlots(LooseLeps,path+"/Preselection/HighPtLoose_TauVeto",weight);
        FillLeptonPlots(LooseLeps_tmp,path+"/Preselection/HighPtLoose",weight);
        FillJetPlots(jets,fatjets,path+"/Preselection/Jets",weight);
        FillHist(path+"/Preselection/nFatJet",fatjets.size(),weight,10,0.,10.);
        FillHist(path+"/Preselection/nJets",jets.size(),weight,10,0.,10.);
        FillHist(path+"/Preselection/nLooseLeps",LooseLeps_tmp.size(),weight,10,0.,10.);
        FillHist(path+"/Preselection/nLooseLeps_TauVeto",LooseLeps.size(),weight,10,0.,10.);
        if(fatjets.size()>0) FillHist(path+"/Preselection/dRJ0Tau",fatjets.at(0).DeltaR(tempTau.at(0)),weight,60,0.,6.);
        if(LooseLeps.size()>0) FillHist(path+"/Preselection/dRtautau_Loose",tempTau.at(0).DeltaR(*LooseLeps.at(0)),weight,60,0.,6.);

        if(HasFlag("PreselOnly")) continue;

        bool isBoosted = false;
        bool isResolved = false;

        
        //Boosted
        vector<double> mll;
        vector<FatJet> fatjets_BoostedSR;
        vector<Lepton *> leptons_BoostedSR;
        FatJet fatjet_BoostedSR;

        FillHist(path+"/CutflowBoostedSR",0.,weight,10,0.,10.);
        if(HasFlag("debug")) cout << "Boosted SR Selection" << endl;
        for(const auto &looselep : LooseLeps){
          Particle ll = *looselep+tempTau.at(0);
          if(!(60 < ll.M() && ll.M() < 150)) mll.push_back(ll.M());
        }

        //If none of the loose lepton satisfies 60 < m(lTightlLoose) < 150 GeV:

        if(mll.size() == LooseLeps.size()){
          FillHist(path+"/CutflowBoostedSR",1.,weight,10,0.,10.);
          for(const auto &J : fatjets){
            FillHist(path+"/BoostedSRSelection/dRJhTau",J.DeltaPhi(tempTau.at(0)),weight,60,0.,6.);
            if(J.DeltaPhi(tempTau.at(0))>2.0){
              fatjets_BoostedSR.push_back(J);
            }
          }
          if(fatjets_BoostedSR.size()>0){
            FillHist(path+"/CutflowBoostedSR",2.,weight,10,0.,10.);
            fatjet_BoostedSR = fatjets_BoostedSR.at(0);
            FillHist(path+"/BoostedSRSelection/nLooseLeptons_BeforeSelected",LooseLeps.size(),weight,10,0.,10.);
            for(const auto &looselep : LooseLeps){
              FillHist(path+"/BoostedSRSelection/dRhTaul",tempTau.at(0).DeltaR(*looselep),weight,60,0.,6.);
              FillHist(path+"/BoostedSRSelection/dRJl",fatjet_BoostedSR.DeltaR(*looselep),weight,60,0.,6.);
              if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
            }
            FillHist(path+"/BoostedSRSelection/nLooseLeptons_Selected",leptons_BoostedSR.size(),weight,10,0.,10.);
            if(leptons_BoostedSR.size()>0){
              FillHist(path+"/CutflowBoostedSR",3.,weight,10,0.,10.);
              isBoosted = true;
            }  
          }
        }

        vector<Lepton *> AnalysisLeptons_LepVeto;
        for(const auto &lep : AnalysisLeptons){
          bool HasHadronicTauInside = false;
          if(tempTau.at(0).DeltaR(*lep)<0.4){
            HasHadronicTauInside = true;
            break;
          }
          if(HasHadronicTauInside) continue;
          AnalysisLeptons_LepVeto.push_back(lep);
        }

        AnalysisLeptons.clear();
        std::sort(AnalysisLeptons_LepVeto.begin(),AnalysisLeptons_LepVeto.end(),PtComparingPtr);
        AnalysisLeptons = AnalysisLeptons_LepVeto;

        //Resolved
        vector<Jet> jets_ResolvedSR;
        vector<Particle> leptons_ResolvedSR;
        Lepton * lepton_ResolvedSR;

        if(isBoosted == false && jets.size()>1 && AnalysisLeptons.size()>0 ){
          if(HasFlag("debug")) cout << "Resolved SR Selection" << endl;
          if(AnalysisLeptons.at(0)->Pt() > 53){
            jets_ResolvedSR = {jets.at(0),jets.at(1)};
            leptons_ResolvedSR = {(Particle)tempTau.at(0), (Particle)*AnalysisLeptons.at(0)};
            vector<double> dRlj;
            for(const auto &lep : leptons_ResolvedSR){
              for(const auto &j : jets_ResolvedSR){
                dRlj.push_back(j.DeltaR(lep));
              }
              if(*min_element(dRlj.begin(),dRlj.end())<0.4) isResolved = false;
              else{
                isResolved = true;
                lepton_ResolvedSR = AnalysisLeptons.at(0);
              }
            }
          }  
        }

        vector<double> LSFCuts = {0,0.2,0.4,0.6,0.7,0.8};
        map<double,TString> LSFCut_path = {{0,""},{0.2,"_LSF0p2"},{0.4,"_LSF0p4"},{0.6,"_LSF0p6"},{0.7,"_LSF0p7"},{0.8,"_LSF0p8"}};

        // fatjets_BoostedSR , leptons_BoostedSR
        if(isBoosted && !isResolved){
          FillHist(path+"/Cutflow",5.,weight,10,0.,10.);
          FillHist(path+"/Region",0.,weight,2,0.,2.);
          for(unsigned int i=0;i<LSFCuts.size();i++){
            if(fatjet_BoostedSR.LSF()>LSFCuts.at(i)){  
              if(HasFlag("debug")) cout << "Boosted SR FillHists" << endl;
              Particle WR = AddFatJetAndLepton(fatjets_BoostedSR.at(0),*leptons_BoostedSR.at(0))+tempTau.at(0)+METv;
              FillJetPlots(jets,fatjets_BoostedSR,path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/Jets",weight);
              FillLeptonPlots(leptons_BoostedSR,path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/Leptons",weight);
              FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/GenMatched_Region",GenRegion,weight,2,0.,2.);
              //FillHist(path+"/Cutflow",5.,weight,10,0.,10.);
              //FillHist(path+"/Region",0.,weight,2,0.,2.);
              FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/EffectiveWRMass",WR.M(),weight,1000,0.,5000.);
              FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/MET",METv.Pt(),weight,1000,0.,1000.);
              //FillHist(path+"/Boosted/FatJet0/LSF",fatjets.at(0).LSF(),weight,100,0.,1.);
              //FillHist(path+"/Boosted/FatJet0/SDMass",fatjets.at(0).SDMass(),weight,1000,0.,1000.);
              //FillHist(path+"/Boosted/FatJet0/pT",fatjets.at(0).Pt(),weight,1000,0.,1000.);
              FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/dRJl",fatjets_BoostedSR.at(0).DeltaR(*leptons_BoostedSR.at(0)),weight,100,0.,1.);
              FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/Taul_pT",leptons_BoostedSR.at(0)->Pt(),weight,1000,0.,1000.);
              FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/Tauh_pT",tempTau.at(0).Pt(),weight,1000,0.,1000.);
              FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/Taul_RelIso",leptons_BoostedSR.at(0)->RelIso(),weight,100,0.,0.5);
              FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/dRtautau",tempTau.at(0).DeltaR(*leptons_BoostedSR.at(0)),weight,60,0.,6.);
              for(unsigned int i=0; i<fatjets_BoostedSR.size();i++){
                FillHist(path+"/Boosted"+LSFCut_path[LSFCuts.at(i)]+"/dRJ"+TString::Itoa(i,10)+"tau",tempTau.at(0).DeltaR(fatjets_BoostedSR.at(i)),weight,60,0.,6.);
              }
            }
          }
        }

        // jets_ResolvedSR , lepton_ResolvedSR
        if(isResolved && !isBoosted){
          if(HasFlag("debug")) cout << "Resolved SR FillHists" << endl;
          Particle WR = jets_ResolvedSR.at(0)+jets_ResolvedSR.at(1)+tempTau.at(0)+*lepton_ResolvedSR+METv;
          FillHist(path+"/Cutflow",6.,weight,10,0.,10.);
          FillHist(path+"/Region",weight,1.,2,0.,2.);
          vector<Lepton *> lepton = {lepton_ResolvedSR};
          FillHist(path+"/Resolved/GenMatched_Region",GenRegion,weight,2,0.,2.);
          FillJetPlots(jets_ResolvedSR,fatjets,path+"/Resolved/Jets",weight);
          FillLeptonPlots(lepton,path+"/Resolved/Leptons",weight);
          FillHist(path+"/Resolved/MET",METv.Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Resolved/EffectiveWRMass",WR.M(),weight,1000,0.,5000.);
          FillHist(path+"/Resolved/Taul_pT",lepton_ResolvedSR->Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Resolved/Tauh_pT",tempTau.at(0).Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Resolved/Taul_RelIso",lepton_ResolvedSR->RelIso(),weight,100,0.,0.5);
          FillHist(path+"/Resolved/dRtautau",tempTau.at(0).DeltaR(*lepton_ResolvedSR),weight,60,0.,6.);
        }
        else continue;
      }
    }
  }


}

WRTau_SR::WRTau_SR(){

}

WRTau_SR::~WRTau_SR(){

}


