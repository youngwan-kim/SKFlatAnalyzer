#include "WRTau_TauFake.h"

void WRTau_TauFake::initializeAnalyzer(){

  vJet_vec.clear(); vEl_vec.clear(); vMu_vec.clear();
  vJet_vec = {3,4,5}; vEl_vec = {13}; vMu_vec = {21};
  
  GetTauIDSFTools(vJet_vec,vEl_vec,vMu_vec);

  if(DataYear==2017){

    SingleElectronTriggers = {"HLT_Ele35_WPTight_Gsf_v"};
    SingleMuonTriggers = {"HLT_Mu50_v"};
    SingleTauTriggers = {"HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"};
    SingleLeptonTriggers = {"HLT_Mu27_v","HLT_Ele35_WPTight_Gsf_v"};

  }

}

void WRTau_TauFake::executeEvent(){


  AnalyzerParameter param;

  param.Clear();
  TriggerList.clear();
  
  TriggerList = SingleLeptonTriggers;

  param.Name = "WRTauFake";
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

  AllGens = GetGens();
  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllTaus = GetAllTaus();
  AllJets = GetAllJets();
  AllFatJets = GetAllFatJets();

  executeEventFromParameter(param);

}

void WRTau_TauFake::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight(1.);

  if(!IsDATA){
    if(HasFlag("unweighted")) weight *= 1.;
    else weight *= MCweight(true,true) * ev.GetTriggerLumi("Full") * GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }

  vector<Gen> this_AllGens = AllGens;
  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;
  vector<Muon> this_AllMuons = AllMuons;
  vector<Electron> this_AllElectrons = AllElectrons;

  vector<Tau> this_AllTaus = SelectTaus(AllTaus,"NoCut",50.,2.4);
  //cout << this_AllTaus.size() << endl ;
  //vector<Tau> this_AllTaus = TauFakeOnly(this_AllTaus_tmp,AllGens);
  //vector<Tau> this_AllTaus_Prompt = TauPromptOnly(this_AllTaus_tmp,AllGens);

  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::comb);

  vector<Muon> muons_veto = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 50., 2.4) ;
  vector<Electron> electrons_veto = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 50., 2.4);
  vector<Lepton *> VetoLeps = CombineLeptonPointerVector(electrons_veto,muons_veto);

  vector<Electron> electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 50. , 2.4);
  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 50. , 2.4);
  vector<Lepton *> leptons = CombineLeptonPointerVector(electrons,muons);

  vector<Tau> taus = VetoLeptonsFromTaus(VetoLeps,this_AllTaus);
  vector<Jet> jets_tauVeto = VetoTauFromJets(this_AllJets,taus); 
  vector<FatJet> fatjets_tmp = VetoTauFromFatJets(this_AllFatJets,taus);
  vector<Jet> jets_lepVeto_tauVeto = JetsVetoLeptonInside(jets_tauVeto,electrons_veto,muons_veto,0.4);

  vector<Jet> jets = SelectJets(jets_lepVeto_tauVeto, param.Jet_ID, 40., 2.4);
  vector<Jet> bjets = SelectBJets(jets,param_jetsM);
  vector<FatJet> fatjets = SelectFatJets(fatjets_tmp,param.FatJet_ID,200.,2.4);

  //vector<Tau> taus_prompt = SelectTaus(taus_le)

  std::sort(taus.begin(),taus.end(),PtComparing);
  std::sort(jets.begin(),jets.end(),PtComparing);
  std::sort(fatjets.begin(),fatjets.end(),PtComparing);

  //map<WRTau_Core::SearchRegion,std::pair<bool,bool>> m_fakeregion = GetQCDFakeRegion(METv,taus,jets,fatjets);
  //FillPassingFakeRegions(m_fakeregion,param.Name,taus,AllGens,weight,true);
  map<WRTau_Core::SearchRegion,std::pair<bool,bool>> m_fakeTTDYCR = GetTTDYFakeRegion(METv,taus,leptons,bjets);
  FillPassingFakeRegions(m_fakeTTDYCR,param.Name,taus,AllGens,weight,true);

}

map<WRTau_Core::SearchRegion,std::pair<bool,bool>> WRTau_TauFake::GetQCDFakeRegion(Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets,const std::vector<FatJet>& fatjets){

  std::pair<bool,bool> _isAK4bpair = std::make_pair(false,false);
  std::pair<bool,bool> _isAK8bpair = std::make_pair(false,false);

  if(taus.size()==1){
    for(auto const &j : fatjets){
      if(j.DeltaR(taus.at(0))>1.2 && j.Pt()>200 && METv.Pt()<20){
        if(taus.at(0).PassID("LooseFakeStudyID")) _isAK8bpair.first = true;
        if(taus.at(0).PassID("TightFakeStudyID")) _isAK8bpair.second = true;
      }
    }
    for(auto const &j : jets){
      if(j.DeltaR(taus.at(0))>0.7 && j.Pt()>30 && METv.Pt()<20){
        if(taus.at(0).PassID("LooseFakeStudyID")) _isAK4bpair.first = true;
        if(taus.at(0).PassID("TightFakeStudyID")) _isAK4bpair.second = true;
      }
    }
  }

  map<WRTau_Core::SearchRegion,std::pair<bool,bool>> m_fakeregion = {
    {WRTau_Core::QCDEnrichedControlRegionAK4,_isAK4bpair},
    {WRTau_Core::QCDEnrichedControlRegionAK8,_isAK8bpair}
  };

  return m_fakeregion;

}

map<WRTau_Core::SearchRegion,std::pair<bool,bool>> WRTau_TauFake::GetTTDYFakeRegion(Particle METv, const std::vector<Tau>& taus, const std::vector<Lepton *> leptons, const std::vector<Jet>& bjets){

  std::pair<bool,bool> _isTTpair = std::make_pair(false,false);
  std::pair<bool,bool> _isDYpair = std::make_pair(false,false);
    
  int nEl(0); int nMu(0);
  for(const auto &l : leptons){
    if(l->IsElectron())   nEl += 1;
    else if(l->IsMuon())  nMu += 1;
  }

  if(taus.size()==1){
    if(leptons.size()==2 && leptons.at(0)->Charge() * leptons.at(1)->Charge() < 0){
      Particle ll = *leptons.at(0) + *leptons.at(1);
      if(nEl == 2 || nMu == 2){
        if(fabs(ll.M()-M_Z)<15 && METv.Pt()<50){
          if(taus.at(0).PassID("FakeBase") && taus.at(0).passLIDvJet() && !taus.at(0).passTIDvJet() ) _isDYpair.first = true;
          if(taus.at(0).PassID("FakeBase") && taus.at(0).passLIDvJet() &&  taus.at(0).passTIDvJet() ) _isDYpair.second = true;
        }
      }
      else if(nEl == 1 && nMu == 1){
        if(ll.M()>20 && METv.Pt()<50 && bjets.size()>0){
          if(taus.at(0).PassID("FakeBase") && taus.at(0).passLIDvJet() && !taus.at(0).passTIDvJet() ) _isTTpair.first = true;
          if(taus.at(0).PassID("FakeBase") && taus.at(0).passLIDvJet() &&  taus.at(0).passTIDvJet() ) _isTTpair.second = true;
        }
      }
    }
  }

  map<WRTau_Core::SearchRegion,std::pair<bool,bool>> m_fakeregion = {
    {WRTau_Core::FakeTTControlRegion,_isTTpair},
    {WRTau_Core::FakeDYControlRegion,_isDYpair}
  };

  return m_fakeregion;

}

void WRTau_TauFake::FillPassingFakeRegions(map<WRTau_Core::SearchRegion,std::pair<bool,bool>> m,TString fillpath,const std::vector<Tau>& taus,const std::vector<Gen>& gens,double MCweight, bool highpT){

  double ptbins[17] = {190,200,210,220,230,240,250,275,300,350,400,450,500,600,700,800,1000};
  double etabins[6] = {0.0,0.5,1.0,1.5,2.0,2.5};

  for(auto const& region : m){

  if(taus.size()>0){
      TString label = fillpath;
      TString tag;
      double weight = MCweight;
      double taupT = taus.at(0).Pt();
      double tauAbsEta = fabs(taus.at(0).Eta());
      if(taupT > 1000.) taupT = 999.;
      if(tauAbsEta > 2.5) tauAbsEta = 2.499;
      
      if(!IsDATA){
          if(GetTauType(taus.at(0),gens)==0){
            //cout << "========================" << endl;
            //cout << GetTauType(taus.at(0),gens) << endl;
            //cout << GetClosestTauGen(taus.at(0),gens).Index() << endl;
            //cout << GetClosestTauGen(taus.at(0),gens).PID() << endl;
            //cout << "========================" << endl;
          }
        if(GetTauType(taus.at(0),gens)>0) tag = "Prompt";
        else if(GetTauType(taus.at(0),gens)<0) tag = "Fake";
        //if(HasFlag("Debug")) cout << IsPromptTau(taus.at(0),gens) << " " << IsFakeTau(taus.at(0),gens) << endl;
        //if(HasFlag("Debug")) cout << tag  << endl;
        else return;
      }
      else tag = "Data";

      if(region.second.first){
        label += "/"+GetRegionString(region.first)+"_"+tag+"Loose";
        if(!IsDATA && IsPromptTau(taus.at(0),gens)){
          tuple<int,int,int> tauid(3,13,21);
          weight *= GetMatchedWeight(taus,gens,tauid,highpT); 
        }
        FillHist(label+"/TauPt_absEta",taupT,tauAbsEta,weight,10,ptbins,5,etabins);
        FillHist(label+"/TauPt",taus.at(0).Pt(),weight,2500,0.,2500.);
        FillHist(label+"/TauEta",taus.at(0).Eta(),weight,100,-5.,5.);
      }
      else if(region.second.second){
        label += "/"+GetRegionString(region.first)+"_"+tag+"Tight";
        if(!IsDATA && IsPromptTau(taus.at(0),gens)){
          tuple<int,int,int> tauid(5,13,21);
          weight *= GetMatchedWeight(taus,gens,tauid,highpT); 
        }
        FillHist(label+"/TauPt_absEta",taupT,tauAbsEta,weight,10,ptbins,5,etabins);
        FillHist(label+"/TauPt",taus.at(0).Pt(),weight,2500,0.,2500.);
        FillHist(label+"/TauEta",taus.at(0).Eta(),weight,100,-5.,5.);
      }
    }
  }
}

// bool WRTau_TauFake::UseEvent(std::vector<Tau>& taus,std::vector<Jet> jets, )

WRTau_TauFake::WRTau_TauFake(){

}

WRTau_TauFake::~WRTau_TauFake(){

}


