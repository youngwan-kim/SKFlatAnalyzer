#include "WRTau_SR_Debug.h"

void WRTau_SR_Debug::initializeAnalyzer(){


  if(DataYear==2017){

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

void WRTau_SR_Debug::executeEvent(){

  AnalyzerParameter param;
  param.Clear();

  param.Name = "TauStudy";
  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "passVetoID";

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

}

void WRTau_SR_Debug::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);
  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight() * ev.GetTriggerLumi("Full") * GetPrefireWeight(0);
  }
  //vector<TString> triggerlist = {"SingleTauTrigger_Old_PlaceHolder","SingleTauTrigger"};
  vector<TString> triggerlist = {"SingleTauTrigger"};

  vector<int> vJet_vec = {2,3,4,5,6};   //vL,L,M,T,vT
  vector<int> vEl_vec = {11,12,13};     //L,M,T 
  vector<int> vMu_vec = {19,20,21};     //L,M,T
  map<int,TString> idname_map = {{2,"vJetVLoose"},{3,"vJetLoose"},{4,"vJetMedium"},{5,"vJetTight"},{6,"vJetVTight"},
                                 {11,"vElLoose"},{12,"vElMedium"},{13,"vElTight"},
                                 {19,"vMuLoose"},{20,"vMuMedium"},{21,"vMuTight"}};

  vector<Muon> this_AllMuons = AllMuons;
  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Lepton *> allleptons = CombineLeptonPointerVector(this_AllElectrons,this_AllMuons);
  
  // TODO : Gen match and include(exclude) events of (dis)interest
  /*vector<Lepton *> GenSignalLepton;
  for(const auto &lep : allleptons){
    int leptontype = GetLeptonType(*lep,AllGens);
    if(leptontype==3) GenSignalLepton.push_back(lep);
    FillHist("Gen/LeptonType",leptontype,weight,16,-8.,8.);
  }*/

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

  for(const auto &gen : AllGens){
    if(fabs(gen.PID())==11 || fabs(gen.PID())==13){
      if(fabs(AllGens.at(gen.MotherIndex()).PID())==15) nLeptonicTau += 1;
    }
  }

  if(!(isTauChannel && nLeptonicTau > 0)) return;
  

  vector<Tau> this_AllTaus = AllTaus;
  //std::sort(this_AllTaus.begin(),this_AllTaus.end(),PtComparing);
  for(const auto &trg : triggerlist){
    for(const auto &vJ : vJet_vec){
      for(const auto &vE : vEl_vec){
        for(const auto &vM : vMu_vec){
          TString idname = idname_map[vJ]+"_"+idname_map[vE]+"_"+idname_map[vM];
          TString path = trg+"/"+idname;
          FillHist(path+"/Cutflow",0.,weight,10,0.,10.);
          //if(this_AllTaus.size()>0) FillHist(path+"/TestHadTauPt_beforeTrigger",this_AllTaus.at(0).Pt(),weight,1000,0.,500.);
        }
      }
    }
  }

  TString triggerflag = "";
  double safeptcut = 0.;
  /*if(ev.PassTrigger(singletau_trigger)  && !ev.PassTrigger(singletau_highpt_trigger)){
    triggerflag = "SingleTauTrigger_Old_PlaceHolder";
    safeptcut = 60;
  }*/

  if(ev.PassTrigger(singletau_highpt_trigger)){
    triggerflag = "SingleTauTrigger";
    safeptcut = 190; // Refer to EXO-22-018 LQ analysis (offline pttauh > 200 GeV)
  }

  else return;

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

  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50., 2.1) ;
  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 50., 2.1) ;
  vector<Muon> muons_loose = SelectMuons(this_AllMuons, param.Muon_Loose_ID, 50., 2.1) ;

  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50., 2.1);
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 50., 2.1);
  vector<Electron> electrons_loose = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 50.,2.1);

  // Electron ID debugging
  vector<Electron> electrons_medium = SelectElectrons(this_AllElectrons,"passMediumID", 50., 2.1);
  vector<Muon> muons_medium = SelectMuons(this_AllMuons,"POGMedium", 50., 2.1);

  vector<Electron> electrons_tight = SelectElectrons(this_AllElectrons,"passTightID", 50., 2.1);
  vector<Muon> muons_tight = SelectMuons(this_AllMuons,"POGTight", 50., 2.1);


  vector<Lepton *> leptons = CombineLeptonPointerVector(electrons,muons);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);
  vector<Lepton *> LooseLeps = CombineLeptonPointerVector(electrons_loose,muons_loose);
  vector<Lepton *> MediumLeps = CombineLeptonPointerVector(electrons_medium,muons_medium);
  vector<Lepton *> TightLeps = CombineLeptonPointerVector(electrons_tight,muons_tight);

  // Pilot Study : remove leptons with no requirements from all hadronic tau collection
  for(unsigned int i=0; i<this_AllTaus.size(); i++){
    for(unsigned int j=0; j<VetoLeps.size(); j++){
      if(this_AllTaus.at(i).DeltaR(*VetoLeps.at(j))<0.4){
        this_AllTaus.erase(this_AllTaus.begin()+i);
        break;
      }
    }
  }

  FillHist(triggerflag+"/nAK4_noSel",this_AllJets.size(),weight,10,0.,10.);
  FillHist(triggerflag+"/nAK8_noSel",this_AllFatJets.size(),weight,10,0.,10.);

  vector<FatJet> this_AllFatJets_test = FatJetsVetoLeptonInside(this_AllFatJets,electrons_veto,muons_veto,0.8);
  FillHist(triggerflag+"/nAK8_VetoLeptonCleaned",this_AllFatJets_test.size(),weight,10,0.,10.);

  for(const auto &th : this_AllTaus){
    for(const auto &j : this_AllJets){
      if(j.DeltaR(th)<0.4) this_AllJets.erase(this_AllJets.begin()+1);
    }
    for(const auto &J : this_AllFatJets){
      if(J.DeltaR(th)<0.8 && !HasFlag("AK8VetoTest")) this_AllFatJets.erase(this_AllFatJets.begin()+1);
    }
  }

  // Gen-level test for AK8 jets
  for(const auto &J : this_AllFatJets){
    vector<Gen> GensInAK8;
    vector<double> dRJGen;
    for(const auto &gen : AllGens){
      if(gen.DeltaR(J)<0.8){
        GensInAK8.push_back(gen);
        dRJGen.push_back(gen.DeltaR(J));
      }
    }
    for(const auto &th : this_AllTaus){
      if(J.DeltaR(th)<0.8){
        cout << "!!! Reco Hadronic Tau inside AK8 !!!" << endl;
        printf("%.2f\t%.2f\t%.2f\t%.2f\n",th.Pt(), th.Eta(), th.Phi(), th.M());
      }
    }
    for(const auto &lep : leptons){
      if(J.DeltaR(*lep)<0.8) {
        cout << "!!! Tight Lepton inside AK8 !!!" << endl;
        printf("%.2f\t%.2f\t%.2f\t%.2f\n",lep->Pt(),lep->Eta(),lep->Phi(),lep->M());
      }
    }
    cout << "== AK8 GenReport dR(gen,J) ==" << endl;
    for(const auto &dR : dRJGen) cout << dR << endl;
    cout << "== AK8 GenReport GenHist ==" << endl;
    PrintPartialGen(GensInAK8,AllGens);
  }

  FillHist(triggerflag+"/nAK4_TauCleaned",this_AllJets.size(),weight,10,0.,10.);
  FillHist(triggerflag+"/nAK8_TauCleaned",this_AllFatJets.size(),weight,10,0.,10.);

  vector<FatJet> this_AllFatJets_tmp = FatJetsVetoLeptonInside(this_AllFatJets,electrons_veto,muons_veto,0.8);
  FillHist(triggerflag+"/nAK8_TauCleaned_VetoLeptonCleaned",this_AllFatJets_tmp.size(),weight,10,0.,10.);

  FillHist(triggerflag+"/nHadTau_noID_noLepVeto_beforePresel",this_AllTaus.size(),weight,10,0.,10.);

  vector<Jet> this_AllJets_tmp = JetsVetoLeptonInside(this_AllJets,electrons_veto,muons_veto,0.4);
  vector<Jet> jets = SelectJets(this_AllJets_tmp, param.Jet_ID, 40., 2.1) ;
  vector<FatJet> fatjets = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200., 2.1) ;
  if(HasFlag("AK8AddVeto")){
    fatjets.clear();
    fatjets = SelectFatJets(this_AllFatJets_tmp, param.FatJet_ID, 200., 2.1);
  }


  std::sort(muons.begin(),muons.end(),PtComparing);
  std::sort(jets.begin(),jets.end(),PtComparing);
  std::sort(fatjets.begin(),fatjets.end(),PtComparing);
  std::sort(electrons.begin(),electrons.end(),PtComparing);
  std::sort(allleptons.begin(),allleptons.end(),PtComparingPtr);

  vector<Lepton *> AnalysisLeptons = leptons;
  if(HasFlag("ElectronIDTest")) AnalysisLeptons = LooseLeps;
  if(HasFlag("ElectronIDMediumTest")) AnalysisLeptons = MediumLeps;
  if(HasFlag("ElectronIDTightTest")) AnalysisLeptons = TightLeps; 
  std::sort(AnalysisLeptons.begin(),AnalysisLeptons.end(),PtComparingPtr);

  for(unsigned int i=0; i<vJet_vec.size(); i++){
    for(unsigned int j=0; j<vEl_vec.size(); j++){
      for(unsigned int k=0; k<vMu_vec.size(); k++){

        TString idname = idname_map[vJet_vec.at(i)]+"_"+idname_map[vEl_vec.at(j)]+"_"+idname_map[vMu_vec.at(k)];
        vector<Tau> tempTau = SelectTaus_varWP(this_AllTaus,i,j,k,50,2.1);
        std::sort(tempTau.begin(),tempTau.end(),PtComparing);

        TString path = triggerflag+"/"+idname;
        //if(this_AllTaus.size()>0) FillHist(path+"/TestHadTauPt_afterTrigger",this_AllTaus.at(0).Pt(),weight,1000,0.,500.);
        FillHist(path+"/nHadTau_noID_beforePresel",this_AllTaus.size(),weight,10,0.,10.);
        FillHist(path+"/nHadTau_beforePresel",tempTau.size(),weight,10,0.,10.);
        if(tempTau.size()<1) continue;
        FillHist(path+"/Cutflow",2.,weight,10,0.,10.);
        if(tempTau.at(0).Pt()<safeptcut) continue;
        FillHist(path+"/Cutflow",3.,weight,10,0.,10.);

        FillHist(path+"/nLep_beforePresel",AnalysisLeptons.size(),weight,10,0.,10.);
        FillHist(path+"/nLep_all_beforePresel",allleptons.size(),weight,10,0.,10.);
        FillHist(path+"/nElTight_beforePresel",electrons.size(),weight,10,0.,10.);
        FillHist(path+"/nMuTight_beforePresel",muons.size(),weight,10,0.,10.);
        
        if(AnalysisLeptons.size()<1) continue;
        FillHist(path+"/Cutflow",4.,weight,10,0.,10.);
        FillHist(path+"/Preselection/Taul_pT",AnalysisLeptons.at(0)->Pt(),weight,1000,0.,1000.);
        FillHist(path+"/Preselection/Tauh_pT",tempTau.at(0).Pt(),weight,1000,0.,1000.);
        FillHist(path+"/Preselection/Taul_RelIso",AnalysisLeptons.at(0)->RelIso(),weight,100,0.,0.5);
        FillHist(path+"/Preselection/dRtautau",tempTau.at(0).DeltaR(*AnalysisLeptons.at(0)),weight,60,0.,6.);
        FillHist(path+"/Preselection/nFatJet",fatjets.size(),weight,10,0.,10.);
        if(fatjets.size()>0) FillHist(path+"/Preselection/dRJ0Tau",fatjets.at(0).DeltaR(tempTau.at(0)),weight,60,0.,6.);

        bool isBoosted = false;
        bool isResolved = false;

        //Boosted
        if(fatjets.size()>0){
          for(const auto &lep : AnalysisLeptons){
            for(const auto &J : fatjets){
              if(J.DeltaR(*lep)<0.8){
                isBoosted = true;
                break;
              } 
            }
          }
        }
        
        if(isBoosted == false && jets.size()>1){
          vector<double> dRlj;
          for(const auto &lep : AnalysisLeptons){
            for(const auto &j : jets){
              dRlj.push_back(j.DeltaR(*lep));
            }
            if(*min_element(dRlj.begin(),dRlj.end())<0.4) isResolved = false;
            else isResolved = true;
          }
        }

        if(isBoosted && !isResolved){
          Particle WR = AddFatJetAndLepton(fatjets.at(0),*AnalysisLeptons.at(0))+tempTau.at(0)+METv;
          //Particle chain1 = AddFatJetAndLepton(fatjets.at(0),*AnalysisLeptons.at(0));
          //Particle chain2 = tempTau.at(0);
          //double MT2_WR = MT2(chain1,chain2,METv,0.01);
          FillHist(path+"/Cutflow",5.,weight,10,0.,10.);
          FillHist(path+"/Region",0.,weight,2,0.,2.);
          FillHist(path+"/Boosted/EffectiveWRMass",WR.M(),weight,1000,0.,5000.);
          //FillHist(path+"/Boosted/MT2_WR",MT2_WR,weight,1000,0.,5000.);
          FillHist(path+"/Boosted/MET",METv.Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Boosted/FatJet0/LSF",fatjets.at(0).LSF(),weight,100,0.,1.);
          FillHist(path+"/Boosted/FatJet0/SDMass",fatjets.at(0).SDMass(),weight,1000,0.,1000.);
          FillHist(path+"/Boosted/FatJet0/pT",fatjets.at(0).Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Boosted/dRJl",fatjets.at(0).DeltaR(*AnalysisLeptons.at(0)),weight,100,0.,1.);
          FillHist(path+"/Boosted/Taul_pT",AnalysisLeptons.at(0)->Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Boosted/Tauh_pT",tempTau.at(0).Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Boosted/Taul_RelIso",AnalysisLeptons.at(0)->RelIso(),weight,100,0.,0.5);
          FillHist(path+"/Boosted/dRtautau",tempTau.at(0).DeltaR(*AnalysisLeptons.at(0)),weight,60,0.,6.);
          for(unsigned int i=0; i<fatjets.size();i++){
            FillHist(path+"/Boosted/dRJ"+TString::Itoa(i,10)+"tau",tempTau.at(0).DeltaR(fatjets.at(i)),weight,60,0.,6.);
          }
          //FillHist(path+"/Boosted/dRJtau",tempTau.at(0).DeltaR(fatjets.at(0)),weight,60,0.,6.);
        }
        if(isResolved && !isBoosted){
          Particle WR = jets.at(0)+jets.at(1)+tempTau.at(0)+*AnalysisLeptons.at(0)+METv;
          //Particle chain1 = jets.at(0)+jets.at(1)+*AnalysisLeptons.at(0);
          //Particle chain2 = tempTau.at(0);
          //double MT2_WR = MT2(chain1,chain2,METv,0.01);
          FillHist(path+"/Cutflow",6.,weight,10,0.,10.);
          FillHist(path+"/Region",weight,1.,2,0.,2.);
          FillHist(path+"/Resolved/MET",METv.Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Resolved/EffectiveWRMass",WR.M(),weight,1000,0.,5000.);
          //FillHist(path+"/Resolved/MT2_WR",MT2_WR,weight,1000,0.,5000.);
          FillHist(path+"/Resolved/Taul_pT",AnalysisLeptons.at(0)->Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Resolved/Tauh_pT",tempTau.at(0).Pt(),weight,1000,0.,1000.);
          FillHist(path+"/Resolved/Taul_RelIso",AnalysisLeptons.at(0)->RelIso(),weight,100,0.,0.5);
          FillHist(path+"/Resolved/dRtautau",tempTau.at(0).DeltaR(*AnalysisLeptons.at(0)),weight,60,0.,6.);
        }
        else continue;
      }
      //continue;
    }
  }


}

WRTau_SR_Debug::WRTau_SR_Debug(){

}

WRTau_SR_Debug::~WRTau_SR_Debug(){

}



