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

  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;


}

map<WRTau_Core::SearchRegion,bool> WRTau_TauFake::GetQCDFakeRegion(Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets,const std::vector<FatJet>& fatjets){

  bool _isAK4(false);
  bool _isAK8(false);

  if(taus.size()==1){
    for(auto const &j : fatjets){
      if(j.DeltaR(taus.at(0))>1.2 && j.Pt()>200 && METv.Pt()<20) _isAK8 = true;
    }
    for(auto const &j : jets){
        if(j.DeltaR(taus.at(0))>0.7 && j.Pt()>30 && METv.Pt()<20) _isAK4 = true;
    }
  }

  map<WRTau_Core::SearchRegion,bool> m_fakeregion = {
    {WRTau_Core::QCDEnrichedControlRegionAK4,_isAK4},
    {WRTau_Core::QCDEnrichedControlRegionAK8,_isAK8},
  };

  return m_fakeregion;

}

void WRTau_TauFake::FillPassingFakeRegions(map<WRTau_Core::SearchRegion,bool> m,TString fillpath,Particle METv, const std::vector<Tau>& taus,
                                          double MCweight, std::tuple<int,int,int> idtuple, bool highpT){

  double ptbins[11] = {190,200,250,300,400,500,600,700,800,900,1000};
  double etabins[6] = {0.0,0.5,1.0,1.5,2.0,2.5};

  for(auto const& region : m){
    if(region.second){

      TString label = fillpath+"/"+GetRegionString(region.first)+"_FakeRate";
      double weight = GetMatchedWeight(taus,idtuple,highpT)*MCweight;

      double taupT = taus.at(0).Pt();
      double tauAbsEta = fabs(taus.at(0).Eta());
      if(taupT > 1000.) taupT = 999.;
      if(tauAbsEta > 2.5) tauAbsEta = 2.499;
      FillHist(label+"/TauPt_absEta",taupT,tauAbsEta,weight,11,ptbins,5,etabins);

    }

  }

}

// bool WRTau_TauFake::UseEvent(std::vector<Tau>& taus,std::vector<Jet> jets, )

WRTau_TauFake::WRTau_TauFake(){

}

WRTau_TauFake::~WRTau_TauFake(){

}


