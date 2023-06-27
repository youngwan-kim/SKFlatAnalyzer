#include "WRTau_Core.h"

bool WRTau_Core::isBoostedPreselection(const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons){
  return ( (isPreselection(taus) && hasAtLeast1Leptons(LooseLeptons)) && !isResolvedPreselection(taus,jets,fatjets,LooseLeptons,TightLeptons) && hasAtLeast1AK8Jets(fatjets));
}

bool WRTau_Core::isBoostedPreselectionTest(const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons){
  return ( (isPreselection(taus) && hasAtLeast1Leptons(LooseLeptons))  && !hasAtLeast2AK4Jets(jets) && hasAtLeast1AK8Jets(fatjets));
}

bool WRTau_Core::isResolvedPreselection(const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons){
  return ( (isPreselection(taus) && hasAtLeast1Leptons(LooseLeptons)) && hasAtLeast2AK4Jets(jets) && hasAtLeast1Leptons(TightLeptons));
}

void WRTau_Core::GetTauIDSFTools(const std::vector<int> vJet_vec,const std::vector<int> vEl_vec,const std::vector<int> vMu_vec){

  for(const auto &vjet : vJet_vec){
    for(const auto &vel : vEl_vec){
      for(const auto &vmu : vMu_vec){
        std::tuple<int,int,int> IDtuple = std::make_tuple(vjet,vel,vmu);
        tauidsftool_map[IDtuple]     = new TauIDSFTool("UL"+std::to_string(DataYear),DeepTauVSjet,idname_map_str[vjet],idname_map_str[vel],false,false,false,true);
        tauidsftool_vEl_map[IDtuple] = new TauIDSFTool("UL"+std::to_string(DataYear),DeepTauVSe,idname_map_str[vel],idname_map_str[vel]);
        tauidsftool_vMu_map[IDtuple] = new TauIDSFTool("UL"+std::to_string(DataYear),DeepTauVSmu,idname_map_str[vmu],idname_map_str[vel]);
      }
    }
  }

  return;

}

double WRTau_Core::GetTauIDSF(TString vsJetWP, TString vsEleWP, int DM, double pt,bool GetFromDM){

  double sf(1.);

  if(pt<=20.) pt=20.1;
  else if(pt>=200.) pt=199.9;

  if(!(DM==0 || DM==1 || DM==10 || DM==11)){
    cerr << "[WRTauCore::GetTauIDSF] Wrong DM for tau selected, only 1 or 3 prong DMs are allowed for SF" << endl;
    exit(EXIT_FAILURE);
  }

  std::vector<TString> vsJetWPs = {"Loose","Medium","Tight","VTight"};
  std::vector<TString> vsEleWPs = {"Tight","VVLoose"};

  if(std::find(vsJetWPs.begin(),vsJetWPs.end(),vsJetWP)==vsJetWPs.end()){
    cerr << "[WRTauCore::GetTauIDSF] DeepTau vsJet WP "<< vsJetWP << " is unsupported, refer to TAU POG documentation" << endl;
    exit(EXIT_FAILURE);
  }

  if(std::find(vsEleWPs.begin(),vsEleWPs.end(),vsEleWP)==vsEleWPs.end()){
    cerr << "[WRTauCore::GetTauIDSF] DeepTau vsEle WP "<< vsEleWP << " is unsupported, refer to TAU POG documentation" << endl;
    exit(EXIT_FAILURE);
  }

  TString datapath = getenv("DATA_DIR");
  
  TDirectory* origDir = gDirectory;
  TString TauPath = datapath+"/Tau/";
  TString vsVar = "pt";
  TString DMVar = "DMinclusive_";

  if(GetFromDM){
    vsVar = "dm";
    DMVar = "DM"+std::to_string(DM)+"_";
  }

  // TauID_SF_dm_DeepTau2017v2p1VSjet_VSjetLoose_VSeleVVLoose_Mar07
  TString FileName = "TauID_SF_"+vsVar+"_DeepTau2017v2p1VSjet_VSjet"+vsJetWP+"_VSele"+vsEleWP+"_Mar07.root";
  TString EraString = "";
  if(DataEra=="2016preVFP") EraString = "2016_preVFP";
  else if(DataEra=="2016postVFP") EraString = "2017_postVFP";
  else EraString = DataEra;

  //TString SFhistname = "DM"+std::to_string(DM)+"_"+EraString+"_hist";
  TString SFhistname = DMVar+EraString+"_hist";
  TFile *TauSFfile = new TFile(TauPath+FileName);
  histDir->cd();
  if(!TauSFfile){
    cerr << "[WRTauCore::GetTauIDSF] File failed to be called :" << FileName << endl;
    exit(ENODATA);
  }
  TauIDSFHist = (TH1D *)TauSFfile->Get(SFhistname)->Clone();
  if(!TauIDSFHist){
    cerr << "[WRTauCore::GetTauIDSF] Hist failed to be called :" << SFhistname << endl;
    exit(ENODATA);
  }

  TauSFfile->Close();
  delete TauSFfile;
  origDir->cd();

  int this_bin = TauIDSFHist->GetXaxis()->FindBin(pt);
  //cout << pt << " " << this_bin << endl;
  sf = TauIDSFHist->GetBinContent(this_bin);

  return sf;

}


std::vector<Jet> WRTau_Core::VetoTauFromJets(const std::vector<Jet>& jets, const std::vector<Tau>& taus,double dR){

  vector<Jet> output;
  for(const auto &j : jets){
    bool HasTauInside(false);
    for(const auto &t : taus){
      if(j.DeltaR(t)<dR){
        HasTauInside = true;
        break;
      }
    }
    if(HasTauInside) continue;
    output.push_back(j);
  }

  return output;

}

std::vector<FatJet> WRTau_Core::VetoTauFromFatJets(const std::vector<FatJet>& fatjets, const std::vector<Tau>& taus,double dR){

  vector<FatJet> output;
  for(const auto &j : fatjets){
    bool HasTauInside(false);
    for(const auto &t : taus){
      if(j.DeltaR(t)<dR){
        HasTauInside = true;
        break;
      }
    }
    if(HasTauInside) continue;
    output.push_back(j);
  }

  return output;


}

std::vector<Lepton*> WRTau_Core::VetoTauFromLeptons(const std::vector<Lepton *> leptons, const std::vector<Tau>& taus,double dR){

  vector<Lepton *> output;
  for(const auto &l : leptons){
    bool HasTauInside(false);
    for(const auto &t : taus){
      if(t.DeltaR(*l)<dR){
        HasTauInside = true;
        break;
      }
    }
    if(HasTauInside) continue;
    output.push_back(l);
  }

  return output;

}

std::vector<Tau> WRTau_Core::VetoLeptonsFromTaus(const std::vector<Lepton *> leptons, const std::vector<Tau>& taus, double dR){

  vector<Tau> output;
  for(const auto &t : taus){
    bool HasLeptonInside(false);
    for(const auto &l : leptons){
      if(t.DeltaR(*l)<dR){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;
    output.push_back(t);
  }

  return output;

}

std::pair<Particle,Particle> WRTau_Core::GetNeutrinos(const Particle METv,const Particle p1,const Particle p2){

  Particle m1,m2;
  std::pair<Particle,Particle> out; 
  double pT1(0.), pT2(0.);

  // split MET pT 
  pT1 = METv.Pt() * TMath::Sin(METv.Phi()-p1.Phi()) / TMath::Sin(p1.Phi()-p2.Phi());
  pT2 = METv.Pt() * TMath::Sin(METv.Phi()-p2.Phi()) / TMath::Sin(p1.Phi()-p2.Phi());

  m1.SetPtEtaPhiM(pT1,p1.Eta(),p1.Phi(),0.);
  m2.SetPtEtaPhiM(pT2,p2.Eta(),p2.Phi(),0.);

  out.first = m1;
  out.second = m2;

  return out;

}


void WRTau_Core::FillPreselHists(TString region,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets, const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons, double weight){

  if(!isPreselection(taus)){
    cout << "[WRTauCore::FillPreselHists] Wrong preselection region, no hadronic taus" << endl;
    return; 
  }

  else{
    //FillHist(region+"/MET",METv.Pt(),weight,2500,0.,2500.);
    FillHist(region+"/Tauh_pT",taus.at(0).Pt(),weight,5000,0.,5000.);
    FillLeptonPlots(TightLeptons,region+"/HighPtTight",weight);
    FillLeptonPlots(LooseLeptons,region+"/HighPtLoose",weight);
    FillJetPlots(jets,fatjets,region+"/Jets",weight);
    FillJetPlots(bjets,fatjets,region+"/BJets",weight);

    FillHist(region+"/nTightLeptons",TightLeptons.size(),weight,10,0.,10.);
    FillHist(region+"/nLooseLeptons",LooseLeptons.size(),weight,10,0.,10.);
    FillHist(region+"/nFatJet",fatjets.size(),weight,10,0.,10.);
    FillHist(region+"/nJets",jets.size(),weight,10,0.,10.);
    FillHist(region+"/nTaus",taus.size(),weight,10,0.,10.);
    FillHist(region+"/nBJets",bjets.size(),weight,10,0.,10.);
    

    double dRtJ0 = fatjets.size()<1? -1.:fatjets.at(0).DeltaR(taus.at(0));
    double dRtj0 = jets.size()<1? -1.:jets.at(0).DeltaR(taus.at(0));
    double dRtj1 = jets.size()<2? -1.:jets.at(1).DeltaR(taus.at(0));
    double dRtl0_loose =  LooseLeptons.size()<1? -1.:LooseLeptons.at(0)->DeltaR(taus.at(0));
    double dRtl0_tight =  TightLeptons.size()<1? -1.:TightLeptons.at(0)->DeltaR(taus.at(0));

    if(dRtJ0>0) FillHist(region+"/dRtJ0",dRtJ0,weight,60,0.,6.);
    if(dRtj0>0) FillHist(region+"/dRtj0",dRtj0,weight,60,0.,6.);
    if(dRtj1>0) FillHist(region+"/dRtj1",dRtj1,weight,60,0.,6.);
    if(dRtl0_loose>0) FillHist(region+"/dRtl0_loose",dRtl0_loose,weight,60,0.,6.);
    if(dRtl0_tight>0) FillHist(region+"/dRtl0_tight",dRtl0_tight,weight,60,0.,6.);

  }

}

void WRTau_Core::FillMassHists(TString region,const Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons, double weight){

  bool isRightSelection = isBoostedPreselection(taus,jets,fatjets,LooseLeptons,TightLeptons) || isResolvedPreselection(taus,jets,fatjets,LooseLeptons,TightLeptons) || isBoostedPreselectionTest(taus,jets,fatjets,LooseLeptons,TightLeptons);

  if(!isRightSelection){
    cout << "[WRTauCore::FillMassHists] Skipping region " << region << endl;
    return; 
  }

  else{

    if(isBoostedPreselection(taus,jets,fatjets,LooseLeptons,TightLeptons)){ 
      Particle BstWR = taus.at(0) + AddFatJetAndLepton(fatjets.at(0),*LooseLeptons.at(0));
      Particle BstWR_MET = BstWR + METv; 
      double BstWR_MT = MT(BstWR,METv);
      FillHist(region+"/BoostedWR",BstWR.M(),weight,5000,0.,5000.);
      FillHist(region+"/MTBoostedWR",BstWR_MT,weight,5000,0.,5000.);
      FillHist(region+"/BoostedWR_withMET",BstWR_MET.M(),weight,5000,0.,5000.);
    }

    if(isResolvedPreselection(taus,jets,fatjets,LooseLeptons,TightLeptons)){ 
      Particle RsvWR = taus.at(0) + jets.at(0) + jets.at(1) + *TightLeptons.at(0);
      Particle RsvWR_MET = RsvWR + METv; 
      double RsvWR_MT = MT(RsvWR,METv);
      FillHist(region+"/ResolvedWR",RsvWR.M(),weight,5000,0.,5000.);
      FillHist(region+"/MTResolvedWR",RsvWR_MT,weight,5000,0.,5000.);
      FillHist(region+"/ResolvedWR_withMET",RsvWR_MET.M(),weight,5000,0.,5000.);
    }

  }

  if(taus.size()>0 && (LooseLeptons.size()>0 || TightLeptons.size()>0)){
    if(LooseLeptons.size()>0){
      Particle thll = taus.at(0) + *LooseLeptons.at(0);
      Particle thll_MET = thll + METv;
      double thll_MT = MT(thll,METv);
      double MET2ST_loose = (METv.Pt()*METv.Pt())/GetST(LooseLeptons,taus,jets,fatjets, METv);
      FillHist(region+"/Mthll",thll.M(),weight,5000,0.,5000.);
      FillHist(region+"/Mthllmet",thll_MET.M(),weight,5000,0.,5000.);
      FillHist(region+"/MTtauhlooselep",thll_MT,weight,5000,0.,5000.);
      FillHist(region+"/MET2ST_loose",MET2ST_loose,weight,5000,0.,5000.);
      FillHist(region+"/ST_loose",GetST(LooseLeptons,taus,jets,fatjets, METv),weight,5000,0.,5000.);
    }
    if(TightLeptons.size()>0){
      Particle thlt = taus.at(0) + *TightLeptons.at(0);
      Particle thlt_MET = thlt+METv;
      double thlt_MT = MT(thlt,METv);
      double MET2ST_tight = (METv.Pt()*METv.Pt())/GetST(TightLeptons,taus,jets,fatjets, METv);
      FillHist(region+"/Mthlt",thlt.M(),weight,5000,0.,5000.);
      FillHist(region+"/Mthltmet",thlt_MET.M(),weight,5000,0.,5000.);
      FillHist(region+"/MTtauhtightlep",thlt_MT,weight,5000,0.,5000.);
      FillHist(region+"/MET2ST_tight",MET2ST_tight,weight,5000,0.,5000.);
      FillHist(region+"/ST_tight",GetST(TightLeptons,taus,jets,fatjets, METv),weight,5000,0.,5000.);
    }
  }

}

/*void WRTau_Core::FillChannelHists(TString region,const Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons, double weight){

  TString channelregion = region;
  std::vector<Lepton*> leptons_region;
  if(isBoostedPreselection(taus,jets,fatjets,LooseLeptons,TightLeptons) || isBoostedPreselectionTest(taus,jets,fatjets,LooseLeptons,TightLeptons);){
    leptons_region = LooseLeptons;
    if(leptons.at(0)->IsElectron()){
      FillPreselHists(region+"_ElTau",taus,jets,fatjets,)
    }
  }
  elif()

}*/

void WRTau_Core::CopyHist(TString histname0, TString histname1){

  TH1D *h_tmp = GetHist1D(histname0);
  
  if(!h_tmp){
    cout << "[WRTauCore::CopyHist] Wrong original histogram : " << histname0 << endl;
    return;
  }

  TH1D *h = (TH1D*)h_tmp->Clone(histname1);
  h->SetDirectory(NULL);
  maphist_TH1D[histname1] = h;

}

double WRTau_Core::GetST(std::vector<Electron> electrons, std::vector<Muon> muons,  std::vector<Tau> taus, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle METv){

  double _st(0.);
  for(const auto &el : electrons) _st += el.Pt();
  for(const auto &mu : muons) _st += mu.Pt();
  for(const auto &ta : taus) _st += ta.Pt();
  for(const auto &j : jets) _st += j.Pt();
  for(const auto &J : fatjets) _st += J.Pt();
  _st += METv.Pt();

  return _st;

}

double WRTau_Core::GetST(std::vector<Lepton *> leptons,  std::vector<Tau> taus, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle METv){

  double _st(0.);
  for(const auto &j : jets) _st += j.Pt();
  for(const auto &J : fatjets) _st += J.Pt();
  for(const auto &ta : taus) _st += ta.Pt();
  for(const auto lep : leptons) _st += lep->Pt();
  _st += METv.Pt();

  return _st;

}

LHE WRTau_Core::GetClosestTauLHE(const Tau tau, const std::vector<LHE>& LHEs){

  double min_dR = 0.1;
  LHE lhe_closest;
  //cout << "[WRTau_Core::GetClosestTauLHE] Start" << endl;
  for(unsigned int i=0; i<LHEs.size(); i++){

    LHE lhe = LHEs.at(i);
    //==== Status 1
    if( lhe.Status() != 1 ) continue;
    //==== dR matching
    if( lhe.DeltaR( tau ) < min_dR ){
      min_dR = lhe.DeltaR( tau ) ;
      lhe_closest = lhe;
    }

  }
  //cout << "[WRTau_Core::GetClosestTauLHE] End" << endl;
  return lhe_closest;
}

Gen WRTau_Core::GetClosestTauGen(const Tau tau, const std::vector<Gen>& gens){

  double min_dR = 0.1;
  Gen gen_closest;
  //cout << "[WRTau_Core::GetClosestTauGen] Start" << endl;
  for(unsigned int i=2; i<gens.size(); i++){

    Gen gen = gens.at(i);
    // no requirements for gen status to be 1 as taus are unstable ; they have status!=1
    //if(abs(gen.PID())!=15) continue;
    if(gen.Status()!=1) continue;
    if(gen.MotherIndex()<0) continue;
    if(gen.DeltaR(tau)<min_dR){
      min_dR = gen.DeltaR(tau);
      gen_closest = gen;
    }

  }

  //cout << "[WRTau_Core::GetClosestTauGen] End" << endl;
  return gen_closest;

}

void WRTau_Core::FillClosestTauLHE(const Tau tau, const std::vector<LHE>& LHEs, TString region,double weight){

  LHE lhe_closest = GetClosestTauLHE(tau,LHEs);
  if(abs(lhe_closest.ID())==15){
    FillHist(region+"/ClosestTauLHEpID_isTau",0.,weight,2,0.,2.);
  }
  else FillHist(region+"/ClosestTauLHEpID_isTau",1.,weight,2,0.,2.);
  FillHist(region+"/ClosestTauLHEpID",lhe_closest.ID(),weight,100,-50.,50.);

}

void WRTau_Core::FillClosestTauGen(const Tau tau, const std::vector<Gen>& gens, TString region,double weight){

  Gen gen_closest = GetClosestTauGen(tau,gens);
  if(abs(gen_closest.PID())==15){
    FillHist(region+"/ClosestTauGenID_isTau",0.,weight,2,0.,2.);
  }
  else FillHist(region+"/ClosestTauGenID_isTau",1.,weight,2,0.,2.);
  FillHist(region+"/ClosestTauGenID",gen_closest.PID(),weight,100,-50.,50.);

  //cout << "[WRTau_Core::FillClosestTauGen] Start" << endl;
  int TauType(0);
  TauType = GetTauType(tau,gens);
  FillHist(region+"/TauType",TauType,weight,14,-7,7);
  //cout << "[WRTau_Core::FillClosestTauGen] End" << endl;

}


int WRTau_Core::GetTauType(const Tau tau, const std::vector<Gen>& gens){

  // ==== Type
  // ==== 1 : EWPrompt Tau
  // ==== 2 : Hadronic Prompt Tau 
  // ==== 2 : Signal Daughter Tau (BSM)
  // ==== 3 : EW Nonprompt Daughter Tau 
  // ==== 4 : Hadronic Nonprompt Daughter Tau 
  // ==== -1 : Unmatched (Both Gen & LHE)
  // ==== -2 : Unmatched but has matched LHE (debugging)
  // ==== -3 : Hadronic Misid (Light flavor jet fakes)
  // ==== -4 : Leptonic Misid (Light lepton fakes)
  // ==== -5 : Debug flag
  // ==== ----------------------------------------------------
  // ==== 0 : Error
  // ==== >0 : Non-fake
  // ==== <0 : Fakes (Unmatched or misid.)

  //LHE lhe_closest = GetClosestTauLHE(tau,LHEs);
  Gen gen_closest = GetClosestTauGen(tau,gens);

  //cout << "[WRTau_Core::GetTauType] Start" << endl;
  int TauType = 0, MatchedTruthIdx = -1;
  if(gen_closest.IsEmpty()) TauType = -1;
  else{
    MatchedTruthIdx = gen_closest.Index();
    //cout << "[WRTau_Core::GetTauType] MatchedTruthIdx : " << MatchedTruthIdx << endl;
    TauType = GetTauType_Public(MatchedTruthIdx, gens);
  }
  //cout << "[WRTau_Core::GetTauType] End : " << TauType << endl;

  return TauType;

}

int WRTau_Core::GetTauType_Public(int TruthIdx, const std::vector<Gen>& TruthColl){

  // ==== Type
  // ==== 1 : EWPrompt Tau
  // ==== 2 : Hadronic Prompt Tau 
  // ==== 2 : Signal Daughter Tau (BSM)
  // ==== 3 : EW Nonprompt Daughter Tau 
  // ==== 4 : Hadronic Nonprompt Daughter Tau 
  // ==== -1 : Unmatched (Both Gen & LHE)
  // ==== -2 : Unmatched but has matched LHE (debugging)
  // ==== -3 : Hadronic Misid (Light flavor jet fakes)
  // ==== -4 : Leptonic Misid (Light lepton fakes)
  // ==== -5 : Debug flag
  // ==== ----------------------------------------------------
  // ==== 0 : Error
  // ==== >0 : Non-fake
  // ==== <0 : Fakes (Unmatched or misid.)

  if(TruthIdx<2) return 0;

  //cout << "[WRTau_Core::GetTauType_Public] Point1, TruthIdx " <<  TruthIdx  <<  endl;
  int TauType = 0;
  int PID = 0, MPID = 0, GrMPID = 0;
  vector<int> my_history, mom_history, grmom_history;
  int LastSelfIdx=-1, MotherIdx=-1, LastSelfMIdx=-1, GrMotherIdx=-1, LastSelfGrMIdx=-1;
  int Status_orig=0, MStatus_orig=0, MStatus_last=0, GrMStatus_orig=0, GrMStatus_last=0;
  bool HadronicOrigin = false;

  my_history = TrackGenSelfHistory(TruthColl.at(TruthIdx), TruthColl);
  LastSelfIdx = my_history[0];
  MotherIdx = my_history[1];

  //cout << "[WRTau_Core::GetTauType_Public] Point1, LastSelfIdx " <<  LastSelfIdx  <<  endl;
  //Status_orig = TruthColl.at(LastSelfIdx).Status();
  //HadronicOrigin = IsFromHadron(TruthColl.at(TruthIdx), TruthColl);

  //cout << "[WRTau_Core::GetTauType_Public] Point2" << endl;

  /*if(   MotherIdx!=-1   ){ mom_history  = TrackGenSelfHistory(TruthColl.at(MotherIdx), TruthColl);
                           LastSelfMIdx = mom_history[0];
                           GrMotherIdx  = mom_history[1];
                           MPID         = TruthColl.at(MotherIdx).PID();
                           MStatus_orig = TruthColl.at(LastSelfMIdx).Status();
                           MStatus_last = TruthColl.at(MotherIdx).Status();
                         }*/

  //cout << "[WRTau_Core::GetTauType_Public] Point3" << endl;

  /*if(  GrMotherIdx!=-1  ){ grmom_history  = TrackGenSelfHistory(TruthColl.at(GrMotherIdx), TruthColl);
                           LastSelfGrMIdx = grmom_history[0];
                           GrMPID         = TruthColl.at(GrMotherIdx).PID();
                           GrMStatus_orig = TruthColl.at(LastSelfGrMIdx).Status();
                           GrMStatus_last = TruthColl.at(GrMotherIdx).Status();
                         }*/
  
  //cout << "[WRTau_Core::GetTauType_Public] Point4" << endl;

  bool HasTauAncestor = false;
  int TauAncestorIndex = -1;

  //cout << "[WRTau_Core::GetTauType_Public] Point5" << endl;
  vector<int> FirstTauAncestorHistory = GetTauAncestor(TruthColl.at(TruthIdx),TruthColl);
  //cout << "[WRTau_Core::GetTauType_Public] Point6" << endl;
  if(FirstTauAncestorHistory[1]!=-1){
    HasTauAncestor = true;
    TauAncestorIndex = FirstTauAncestorHistory[1];
  }

  //cout << "[WRTau_Core::GetTauType_Public] Point7" << endl;
  if(TruthIdx == -1)                                                                                  TauType = 0;
  else if(HasTauAncestor){ 
    
    // Has a tau ancestor ; track tau again for radiation correction etc.
    //PID = TruthColl.at(TruthIdx).PID();
    vector<int> OriginTauHistory = TrackGenSelfHistory(TruthColl.at(TauAncestorIndex), TruthColl);
    int TauMotherIndex = OriginTauHistory[1];
    
    if(TauMotherIndex == -1)                                                                          TauType = -5; // Case study debug flag
    else{

      int TauMotherPID = TruthColl.at(TauMotherIndex).PID();
      int TauMotherStatus = TruthColl.at(TauMotherIndex).Status();

      if(fabs(TauMotherPID) == 23 || fabs(TauMotherPID) == 24 || fabs(TauMotherPID) == 25)            TauType = 1;
      else if(IsSignalPID(TauMotherPID))                                                              TauType = 3;
      else if(TauMotherStatus>20 && TauMotherStatus<30)                                               TauType = 1;
      else if(fabs(TauMotherPID)>50)                                                                  TauType = 4; // 4)
      
    }

  }
  else{ // Doesn't even have tau ancestor : Misid section
    
    PID = TruthColl.at(TruthIdx).PID();
    if( fabs(PID) == 11 || fabs(PID) == 13 )                                                          TauType = -4;
    else if( (fabs(PID)>=1 && fabs(PID)<=5) || fabs(PID) == 21 )                                      TauType = -3;
    else if( fabs(PID)==22 ){
      vector<int> GenMatchPhotonHistory = TrackGenSelfHistory(TruthColl.at(TruthIdx), TruthColl);
      int PhotonMotherIndex = GenMatchPhotonHistory[1];
      if(PhotonMotherIndex == -1)                                                                     TauType = -5;
      else{
        if(IsFromHadron(TruthColl.at(PhotonMotherIndex),TruthColl))                                   TauType = -3;
      }
    }
  }

  return TauType;
  
  // 1) Offshell EW mediators are not tracked ;
  // 2) Prompt Onshell EW mediators 
  // 3) Nonprompt onshell EW mediators
  // 4) had > ta + X , ta > had + v

}


vector<int> WRTau_Core::GetTauAncestor(const Gen& me, const std::vector<Gen>& gens){

  int myindex = me.Index();

  //cout << "[WRTau_Core::GetTauAncestor] Init (currentindex) : " << myindex << endl;

  if(myindex<2){
    //cout << "[WRTau_Core::GetTauAncestor] Exception (currentindex) : " << myindex << endl;
    vector<int> out = {myindex,-1};
    return out;
  }

  int currentidx = myindex;
  int motherindex = me.MotherIndex();

  //cout << "[WRTau_Core::GetTauAncestor] Start" << endl;
  while(fabs(gens.at(motherindex).PID()) != 15 ){

    currentidx = motherindex;
    motherindex = gens.at(motherindex).MotherIndex();

    //cout << "[WRTau_Core::GetTauAncestor] Loop (currentidx,motheridx)=" << currentidx << "," << motherindex << endl;
    //cout << "[WRTau_Core::GetTauAncestor] Loop (currentidx_PID,motheridx_PID)=" << gens.at(currentidx).PID() << "," << gens.at(motherindex).PID() << endl;

    if(currentidx<=2 || motherindex <= 0){
      motherindex = -1;
      break;
    }
    else if(fabs(gens.at(motherindex).PID())==15) break;
  }

  //cout << "[WRTau_Core::GetTauAncestor] Final (currentidx,motheridx)=" << currentidx << "," << motherindex << endl;
  //cout << "[WRTau_Core::GetTauAncestor] Final (currentidx_PID,motheridx_PID)=" << gens.at(currentidx).PID() << "," << gens.at(motherindex).PID() << endl;
  //cout << "[WRTau_Core::GetTauAncestor] End" << endl;

  vector<int> out = {currentidx, motherindex};

  return out;

}

vector<Tau> WRTau_Core::TauPromptOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens){

  if(IsDATA) return taus;

  std::vector<Tau> out;

  for(unsigned int i=0; i<taus.size(); i++){
    if(GetTauType(taus.at(i), gens)<=0) continue;
    out.push_back(taus.at(i));
  }

  return out;

}

vector<Tau> WRTau_Core::TauFakeOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens){

  if(IsDATA) return taus;

  std::vector<Tau> out;

  for(unsigned int i=0; i<taus.size(); i++){
    if(GetTauType(taus.at(i), gens)>=0) continue;
    out.push_back(taus.at(i));
  }

  return out;

}

vector<Tau> WRTau_Core::TauErrorOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens){

  if(IsDATA) return taus;

  std::vector<Tau> out;

  for(unsigned int i=0; i<taus.size(); i++){
    if(GetTauType(taus.at(i), gens)!=0) continue;
    out.push_back(taus.at(i));
  }

  return out;

}


TDirectory* WRTau_Core::GetTempDir_WRTauCore(){

    gROOT->cd(); 
    TDirectory* histDir = NULL;
    int counter = 0;
    while (!histDir) {
      //==== First, let's find a directory name that doesn't exist yet
      std::stringstream dirname;
      dirname << "WRTauCore" << counter;
      if (gROOT->GetDirectory((dirname.str()).c_str())) {
        ++counter;
        continue;
      }
      //==== Let's try to make this directory
      histDir = gROOT->mkdir((dirname.str()).c_str());
    }
    std::cout << "[TDirectoryHelper::GetTempDirectory()] histDir name = " << histDir->GetName() << std::endl;
    return histDir;

}

std::vector<Electron> WRTau_Core::ElectronPromptOnly_Tau(const std::vector<Electron>& electrons, const std::vector<Gen>& gens){

  if(IsDATA) return electrons;

  std::vector<Electron> out;

  for(unsigned int i=0; i<electrons.size(); i++){
    if(GetLeptonType(electrons.at(i), gens)<=0 && GetLeptonType(electrons.at(i), gens) != -3) continue;
    out.push_back( electrons.at(i) );
  }

  return out;

}

std::vector<Electron> WRTau_Core::ElectronNonPromptOnly_Tau(const std::vector<Electron>& electrons, const std::vector<Gen>& gens){

  if(IsDATA) return electrons;

  std::vector<Electron> out;

  for(unsigned int i=0; i<electrons.size(); i++){
    if(GetLeptonType(electrons.at(i), gens)>=0 || GetLeptonType(electrons.at(i), gens) != -3) continue;
    out.push_back( electrons.at(i) );
  }

  return out;

}

std::vector<Muon> WRTau_Core::MuonPromptOnly_Tau(const std::vector<Muon>& muons, const std::vector<Gen>& gens){

  if(IsDATA) return muons;

  std::vector<Muon> out;

  for(unsigned int i=0; i<muons.size(); i++){ // continue if not ( prompt(<0) OR -3 ) 
    if(!(GetLeptonType(muons.at(i), gens)>0 || GetLeptonType(muons.at(i), gens) == -3)) continue;
    out.push_back( muons.at(i) );
  }

  return out;

}

std::vector<Muon> WRTau_Core::MuonNonPromptOnly_Tau(const std::vector<Muon>& muons, const std::vector<Gen>& gens){

  if(IsDATA) return muons;

  std::vector<Muon> out;

  for(unsigned int i=0; i<muons.size(); i++){
    if(GetLeptonType(muons.at(i), gens)>0 || GetLeptonType(muons.at(i), gens) == -3) continue;
    out.push_back( muons.at(i) );
  }

  return out;

}

TString WRTau_Core::GetChannelString(WRTau_Core::Channel channel){

  TString channel_string="";
  if (channel == WRTau_Core::TauE)  channel_string="ElTau";
  if (channel == WRTau_Core::TauMu) channel_string="MuTau";

  return channel_string;
}

std::string WRTau_Core::GetRegionString(WRTau_Core::SearchRegion region){

  std::string region_string="";
  if (region == WRTau_Core::BaselinePreselection)               region_string="BaselinePreselection";
  if (region == WRTau_Core::ResolvedPreselection)               region_string="ResolvedPreselection";
  if (region == WRTau_Core::BoostedPreselection)                region_string="BoostedPreselection";
  if (region == WRTau_Core::ResolvedLowMassControlRegion)       region_string="ResolvedLowMassControlRegionMass1";
  if (region == WRTau_Core::BoostedLowMassControlRegion)        region_string="BoostedLowMassControlRegionMass1";
  if (region == WRTau_Core::ResolvedLowMassControlRegionMass1)  region_string="ResolvedLowMassControlRegion";
  if (region == WRTau_Core::BoostedLowMassControlRegionMass1)   region_string="BoostedLowMassControlRegion";
  if (region == WRTau_Core::ResolvedSignalRegion)               region_string="ResolvedSignalRegion";
  if (region == WRTau_Core::BoostedSignalRegion)                region_string="BoostedSignalRegion";
  if (region == WRTau_Core::ResolvedSignalRegionMass1)          region_string="ResolvedSignalRegionMass1";
  if (region == WRTau_Core::BoostedSignalRegionMass1)           region_string="BoostedSignalRegionMass1";
  if (region == WRTau_Core::WJetsControlRegion)                 region_string="WJetsControlRegion";
  if (region == WRTau_Core::QCDEnrichedControlRegion)           region_string="QCDEnrichedControlRegion";

  return region_string;
}

WRTau_Core::Channel WRTau_Core::GetChannel(const std::vector<Lepton *> leps){

  if(leps.at(0)->IsElectron()) return WRTau_Core::TauE;
  if(leps.at(0)->IsMuon()) return WRTau_Core::TauMu;

}


double WRTau_Core::GetResolvedSRMass(Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons,bool ignoreMET){

  double mass = -999;

  vector<Jet> jets_ResolvedSR;
  vector<Particle> leptons_ResolvedSR;

  jets_ResolvedSR = {jets.at(0),jets.at(1)};
  if(TightLeptons.at(0)->Pt()>53){
    leptons_ResolvedSR = {(Particle)taus.at(0),(Particle)*TightLeptons.at(0)};
    vector<double> dRlj;
    for(const auto &lep : leptons_ResolvedSR){
      for(const auto &j : jets_ResolvedSR){
        dRlj.push_back(j.DeltaR(lep));
      }
    }
    if(*min_element(dRlj.begin(),dRlj.end())>0.4){
      Particle wr = taus.at(0) + *TightLeptons.at(0) + jets.at(0) + jets.at(1) ;
      if(!ignoreMET) mass = MT(wr,METv);
      else mass = wr.M();
    }
  }  

  return mass;

}

double WRTau_Core::GetResolvedSRMass_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

  double mass = -999;

  vector<Jet> jets_ResolvedSR;
  vector<Particle> leptons_ResolvedSR;

  jets_ResolvedSR = {jets.at(0),jets.at(1)};
  if(TightLeptons.at(0)->Pt()>53){
    leptons_ResolvedSR = {(Particle)taus.at(0),(Particle)*TightLeptons.at(0)};
    vector<double> dRlj;
    for(const auto &lep : leptons_ResolvedSR){
      for(const auto &j : jets_ResolvedSR){
        dRlj.push_back(j.DeltaR(lep));
      }
    }
    if(*min_element(dRlj.begin(),dRlj.end())>0.4){
      std::pair<Particle,Particle> neutrinos = GetNeutrinos(METv,(Particle)taus.at(0),(Particle)*TightLeptons.at(0));
      Particle wr = taus.at(0) + *TightLeptons.at(0) + jets.at(0) + jets.at(1) + neutrinos.first + neutrinos.second;
      mass = wr.M();
    }
  }  

  return mass;

}

double WRTau_Core::GetResolvedSRMassN_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

  double mass = -999;

  vector<Jet> jets_ResolvedSR;
  vector<Particle> leptons_ResolvedSR;

  jets_ResolvedSR = {jets.at(0),jets.at(1)};
  if(TightLeptons.at(0)->Pt()>53){
    leptons_ResolvedSR = {(Particle)taus.at(0),(Particle)*TightLeptons.at(0)};
    vector<double> dRlj;
    for(const auto &lep : leptons_ResolvedSR){
      for(const auto &j : jets_ResolvedSR){
        dRlj.push_back(j.DeltaR(lep));
      }
    }
    if(*min_element(dRlj.begin(),dRlj.end())>0.4){
      std::pair<Particle,Particle> neutrinos = GetNeutrinos(METv,(Particle)taus.at(0),(Particle)*TightLeptons.at(0));
      Particle N = *TightLeptons.at(0) + jets.at(0) + jets.at(1)  + neutrinos.second;
      mass = N.M();
    }
  }  

  return mass;

}


double WRTau_Core::GetBoostedSRMass(Particle METv,const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons,bool ignoreMET){

  double mass = -999;

  vector<FatJet> fatjets_BoostedSR;
  vector<Lepton *> leptons_BoostedSR;
  FatJet fatjet_BoostedSR;

  for(const auto &J : fatjets){
    if(J.DeltaPhi(taus.at(0))>2.0) fatjets_BoostedSR.push_back(J);
  }

  if(fatjets_BoostedSR.size()>0){
    fatjet_BoostedSR = fatjets_BoostedSR.at(0);
    for(const auto &looselep : LooseLeptons){
      if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
    }
    if(leptons_BoostedSR.size()>0){
      Particle wr = taus.at(0) + AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0));
      if(!ignoreMET) mass = MT(wr,METv);
      else mass = wr.M();
    }
  }

  return mass;

}

double WRTau_Core::GetBoostedSRMass_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double mass = -999;

  vector<FatJet> fatjets_BoostedSR;
  vector<Lepton *> leptons_BoostedSR;
  FatJet fatjet_BoostedSR;

  for(const auto &J : fatjets){
    if(J.DeltaPhi(taus.at(0))>2.0) fatjets_BoostedSR.push_back(J);
  }

  if(fatjets_BoostedSR.size()>0){
    fatjet_BoostedSR = fatjets_BoostedSR.at(0);
    for(const auto &looselep : LooseLeptons){
      if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
    }
    if(leptons_BoostedSR.size()>0){
      std::pair<Particle,Particle> neutrinos = GetNeutrinos(METv,(Particle)taus.at(0),(Particle)*leptons_BoostedSR.at(0));
      Particle wr = taus.at(0) + AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0)) + neutrinos.first + neutrinos.second ;
      mass = wr.M();
    }
  }

  return mass;

}

double WRTau_Core::GetBoostedSRMassN_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double mass = -999;

  vector<FatJet> fatjets_BoostedSR;
  vector<Lepton *> leptons_BoostedSR;
  FatJet fatjet_BoostedSR;

  for(const auto &J : fatjets){
    if(J.DeltaPhi(taus.at(0))>2.0) fatjets_BoostedSR.push_back(J);
  }

  if(fatjets_BoostedSR.size()>0){
    fatjet_BoostedSR = fatjets_BoostedSR.at(0);
    for(const auto &looselep : LooseLeptons){
      if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
    }
    if(leptons_BoostedSR.size()>0){
      std::pair<Particle,Particle> neutrinos = GetNeutrinos(METv,(Particle)taus.at(0),(Particle)*leptons_BoostedSR.at(0));
      Particle N =  AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0)) + neutrinos.second ;
      mass = N.M();
    }
  }

  return mass;

}


map<WRTau_Core::SearchRegion,bool> WRTau_Core::GetRegion(Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                                   const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons){

  bool _isBaselinePreselection(false);
  bool _isResolvedPreselection(false);
  bool _isBoostedPreselection(false);
  bool _isResolvedLowMassControlRegion(false);
  bool _isBoostedLowMassControlRegion(false);
  bool _isResolvedLowMassControlRegionMass1(false);
  bool _isBoostedLowMassControlRegionMass1(false);
  bool _isResolvedSignalRegion(false);
  bool _isBoostedSignalRegion(false);
  bool _isResolvedSignalRegionMass1(false);
  bool _isBoostedSignalRegionMass1(false);
  bool _isWJetsControlRegion(false);
  bool _isQCDEnrichedControlRegion(false);

  bool hasAtLeast2AK4Jets = jets.size()>1;
  bool hasAtLeast1AK8Jets = fatjets.size()>0;
  bool hasAtLeast1TightLeptons = TightLeptons.size()>0;
  bool hasAtLeast1LooseLeptons = LooseLeptons.size()>0;

  _isBaselinePreselection = taus.size()>0 && taus.at(0).Pt()>190 && LooseLeptons.size()==0;
  _isResolvedPreselection = hasAtLeast2AK4Jets && TightLeptons.size()==0;
  _isBoostedPreselection = !_isResolvedPreselection && hasAtLeast1AK8Jets;


  if(_isResolvedPreselection){

    vector<Jet> jets_ResolvedSR;
    vector<Particle> leptons_ResolvedSR;

    jets_ResolvedSR = {jets.at(0),jets.at(1)};
    if(TightLeptons.at(0)->Pt()>53){
      leptons_ResolvedSR = {(Particle)taus.at(0),(Particle)*TightLeptons.at(0)};
      vector<double> dRlj;
      for(const auto &lep : leptons_ResolvedSR){
        for(const auto &j : jets_ResolvedSR){
          dRlj.push_back(j.DeltaR(lep));
        }
      }
      if(*min_element(dRlj.begin(),dRlj.end())<0.4) _isResolvedSignalRegion = false;
      else{
        std::pair<Particle,Particle> neutrinos = GetNeutrinos(METv,(Particle)taus.at(0),(Particle)*TightLeptons.at(0));
        Particle wr1 = taus.at(0) + *TightLeptons.at(0) + jets.at(0) + jets.at(1) + neutrinos.first + neutrinos.second;
        Particle wr = taus.at(0) + *TightLeptons.at(0) + jets.at(0) + jets.at(1);
        if(wr.M()<800) _isResolvedLowMassControlRegion = true ; // TODO : study mass cut optimization ; make a submethod to vary mass cuts and check significance in 2D
        else if(wr.M()>800) _isResolvedSignalRegion = true;

        if(wr1.M()<800) _isResolvedLowMassControlRegionMass1 = true;
        else if(wr1.M()>800) _isResolvedSignalRegionMass1 = true;
      }
    }  
  }

  if(!_isResolvedSignalRegion && _isBoostedPreselection){

    vector<double> mll;
    vector<FatJet> fatjets_BoostedSR;
    vector<Lepton *> leptons_BoostedSR;
    FatJet fatjet_BoostedSR;
    Particle ll;

    for(const auto &looselep : LooseLeptons){
      ll = *looselep + taus.at(0);
      mll.push_back(ll.M());
    }
    for(const auto &J : fatjets){
      if(J.DeltaPhi(taus.at(0))>2.0) fatjets_BoostedSR.push_back(J);
    }
    if(fatjets_BoostedSR.size()>0){
      fatjet_BoostedSR = fatjets_BoostedSR.at(0);
      for(const auto &looselep : LooseLeptons){
        if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
      }
      if(leptons_BoostedSR.size()>0){
        std::pair<Particle,Particle> neutrinos = GetNeutrinos(METv,(Particle)taus.at(0),(Particle)*leptons_BoostedSR.at(0));
        Particle wr = taus.at(0) + AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0));
        Particle wr1 = taus.at(0) + AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0)) + neutrinos.first + neutrinos.second ;
        double MT_wr = MT(wr,METv);
        if(wr.M()<800) _isBoostedLowMassControlRegion = true ;
        else if(wr.M()>800) _isBoostedSignalRegion = true;

        if(wr1.M()<800) _isBoostedLowMassControlRegionMass1 = true ;
        else if(wr1.M()>800) _isBoostedSignalRegionMass1 = true;
      }
    }
  }



  map<WRTau_Core::SearchRegion,bool> m_region = {
    {WRTau_Core::BaselinePreselection,_isBaselinePreselection},
    {WRTau_Core::ResolvedPreselection,_isResolvedPreselection},
    {WRTau_Core::BoostedPreselection,_isBoostedPreselection},
    {WRTau_Core::ResolvedLowMassControlRegion,_isResolvedLowMassControlRegion},
    {WRTau_Core::BoostedLowMassControlRegion,_isBoostedLowMassControlRegion},
    {WRTau_Core::ResolvedLowMassControlRegionMass1,_isResolvedLowMassControlRegionMass1},
    {WRTau_Core::BoostedLowMassControlRegionMass1,_isBoostedLowMassControlRegionMass1},
    {WRTau_Core::ResolvedSignalRegion,_isResolvedSignalRegion},
    {WRTau_Core::BoostedSignalRegionMass1,_isBoostedSignalRegionMass1},
    {WRTau_Core::ResolvedSignalRegionMass1,_isResolvedSignalRegionMass1},
    {WRTau_Core::BoostedSignalRegion,_isBoostedSignalRegion},
    {WRTau_Core::WJetsControlRegion,_isWJetsControlRegion},
    {WRTau_Core::QCDEnrichedControlRegion,_isQCDEnrichedControlRegion}
  };

  return m_region;

}



void WRTau_Core::FillPassingRegions(map<WRTau_Core::SearchRegion,bool> m_region,Particle METv, const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight,std::tuple<int,int,int> idtuple, bool highpT){
  
  for(auto const& region : m_region){

    if(region.second){

      std::pair<std::vector<Lepton *>,std::vector<Lepton *>> PairVecLeps = std::make_pair(LooseLeptons,TightLeptons);
      vector<Lepton *> leptons = ChooseLeptonColl(region.first,PairVecLeps);

      WRTau_Core::Channel ch = GetChannel(leptons);
      TString label = fillpath+"/"+GetRegionString(region.first); 
      TString label_channel = label + "_"+GetChannelString(ch);

      std::vector<TString> fillstr = {label,label_channel};
      
      double weight = GetMatchedWeight(taus,leptons,idtuple,highpT) * MCweight;
      weight *= GetTauIDLeptonFakeSF(idtuple,leptons,gens);
      if(HasFlag("unweighted")) weight = 1;

      for(const auto str : fillstr){

        //CopyHist(fillpath+"/Cutflow",str+"/Cutflow");
        FillHist(str+"/Nevents",0,weight,1,0.,1.);
        FillHist(str+"/MET",METv.Pt(),weight,2500,0.,2500.);

        for(unsigned int i=0;i<leptons.size();i++){
          FillHist(str+"/dRl"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(*leptons.at(i)),weight,60,0.,6.);
        }

        for(unsigned int i=0;i<jets.size();i++){
          FillHist(str+"/dRj"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(jets.at(i)),weight,60,0.,6.);
        }

        for(unsigned int i=0;i<fatjets.size();i++){
          FillHist(str+"/dRJ"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(fatjets.at(i)),weight,60,0.,6.);
        }

        FillPreselHists(str,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,weight);
        FillMassHists(str,METv,taus,jets,fatjets,LooseLeptons,TightLeptons,weight);

        if(region.first == WRTau_Core::BoostedPreselection || region.first == WRTau_Core::BoostedLowMassControlRegion || region.first == WRTau_Core::BoostedSignalRegion){

          double M1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
          double M2 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);
          double M3 = GetBoostedSRMass_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
          double M4 = GetBoostedSRMassN_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
          if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
          if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
          if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
          if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

        }

        if(region.first == WRTau_Core::ResolvedPreselection || region.first == WRTau_Core::ResolvedLowMassControlRegion || region.first == WRTau_Core::ResolvedSignalRegion){
          
          double M1 = GetResolvedSRMass(METv,taus,jets,TightLeptons,false);
          double M2 = GetResolvedSRMass(METv,taus,jets,TightLeptons,true);
          double M3 = GetResolvedSRMass_RecoNeutrino(METv,taus,jets,TightLeptons);
          double M4 = GetResolvedSRMassN_RecoNeutrino(METv,taus,jets,TightLeptons);
          if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
          if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
          if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
          if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

        }
      }
      
    }
  }
  
  return;

}

// (loose,tight) pair order
std::vector<Lepton *> WRTau_Core::ChooseLeptonColl(WRTau_Core::SearchRegion region, std::pair<std::vector<Lepton *>,std::vector<Lepton *>> LeptonPair){

  std::string regionname = GetRegionString(region);
  if(regionname.find("Boosted") != std::string::npos) return LeptonPair.first;
  if(regionname.find("Resolved") != std::string::npos) return LeptonPair.second;

  else return LeptonPair.first;

}

double WRTau_Core::GetTauIDLeptonFakeSF(const std::tuple<int,int,int> idtuple, const std::vector<Lepton*> leps, const std::vector<Gen>& gens){

  double w = 1.0;

  if(HasFlag("NonpromptLepton")){
    
    if(GetChannel(leps) == WRTau_Core::TauE){
      int genmatch = fabs(GetLeptonType(*leps.at(0),gens));
      w *= tauidsftool_vEl_map[idtuple]->getSFvsEta(leps.at(0)->Eta(),genmatch);
    }

    else if(GetChannel(leps) == WRTau_Core::TauMu){
      int genmatch = fabs(GetLeptonType(*leps.at(0),gens))+1;
      w *= tauidsftool_vMu_map[idtuple]->getSFvsEta(leps.at(0)->Eta(),genmatch);
    }
  }

  return w;

}

double WRTau_Core::GetMatchedWeight(const std::vector<Tau>& taus,const std::vector<Lepton *> leps,std::tuple<int,int,int> idtuple,bool highpT){

  if(IsDATA) return 1.0;
  
  // weight = w_tau * w_lep 
  else{

    double w_tau(1.0), w_lepton(1.0);


    if(!HasFlag("NonpromptTau")){
      if(highpT) w_tau = tauidsftool_map[idtuple]->getHighPTSFvsPT(taus.at(0).Pt());
      else w_tau = tauidsftool_map[idtuple]->getSFvsPT(taus.at(0).Pt());
    }
    if(!HasFlag("NonpromptLepton")){

      if(GetChannel(leps) == WRTau_Core::TauE){
        w_lepton *= mcCorr->ElectronID_SF("HEEP",leps.at(0)->Eta(),leps.at(0)->Pt());
        w_lepton *= mcCorr->ElectronReco_SF(leps.at(0)->Eta(),leps.at(0)->Pt());
      }

      // TODO : implement to get AnalyzerParam info for general input of SF WPs

      else if(GetChannel(leps) == WRTau_Core::TauMu){
        w_lepton *= mcCorr->MuonID_SF("NUM_HighPtID_DEN_TrackerMuons",leps.at(0)->Eta(),leps.at(0)->Pt());
        w_lepton *= mcCorr->MuonISO_SF("NUM_LooseRelTkIso_DEN_HighPtIDandIPCut",leps.at(0)->Eta(),leps.at(0)->Pt());
      }
    
    }

    return w_tau * w_lepton;

  }
  
}

Gen WRTau_Core::GetClosestGenJet(const std::vector<Gen>& gens, const Jet jet){
  
  std::vector<double> dRJet;
  
  for(unsigned int i=0; i<gens.size() ; i++){
    dRJet.push_back(jet.DeltaR(gens.at(i)));
  }

  int closestJetIndex = min_element(dRJet.begin(),dRJet.end())-dRJet.begin();
  
  return gens.at(closestJetIndex);

}

Gen WRTau_Core::GetClosestGenJet(const std::vector<Gen>& gens, const FatJet jet){
  
  std::vector<double> dRJet;
  
  for(unsigned int i=0; i<gens.size() ; i++){
    dRJet.push_back(jet.DeltaR(gens.at(i)));
  }

  int closestJetIndex = min_element(dRJet.begin(),dRJet.end())-dRJet.begin();
  
  return gens.at(closestJetIndex);

}

std::pair<Gen,Gen> WRTau_Core::GetSignalGenTaus(const std::vector<Gen>& gens){

  // Ntau , WRtau
  pair<Gen,Gen> SignalGenTaus;
  std::set<pair <int,int>> SignalGenTausIdx;

  for(const auto &g : gens){

    cout << "[WRTau_Core::GetSignalGenTaus] index / status / mIdx " << endl;
    if(fabs(g.PID()) == 15){

      vector<int> TauHistory = TrackGenSelfHistory(g,gens);
      cout << "[gentau = " << g.Index() << " , " << g.Status() << " , " << g.MotherIndex() << endl;
      cout << "[" << TauHistory[0] << " , " << TauHistory[1] << "]" << endl;

      if(TauHistory[1]!=-1){

        pair <int,int> IdxPair = make_pair(fabs(gens.at(TauHistory[1]).PID()),TauHistory[0]);
        cout << "[[" << IdxPair.first << " , " << IdxPair.second << "]]" << endl;
        if(IdxPair.first == 9900016 || IdxPair.first == 34)  SignalGenTausIdx.insert(IdxPair);

      }

    } 

  }

  int idx(0);

  //Get final physical descendant of origin signal taus
  for(const auto &pair : SignalGenTausIdx){

    //cout << "pair#" << idx << " :" <<  pair.first << " , " << pair.second << endl;
    //cout << "(PID = " <<  gens.at(pair.second).PID() << ")" << endl;
    int currentindex = pair.second; // before radiation etc , find last descendant 
    int daughterindex = GetDaughterIndex(gens.at(currentindex),gens);

    int idx_j(0);
    while(fabs(gens.at(daughterindex).PID())==15){

      currentindex = daughterindex;
      daughterindex = GetDaughterIndex(gens.at(daughterindex),gens);

      //cout << "- Idx@loop#" << idx_j << " :" << currentindex << " , " << daughterindex << endl;
      //cout << "- PID@loop#" << idx_j << " :" << gens.at(currentindex).PID() << " , " << gens.at(daughterindex).PID() << endl;
      //idx_j++;

      if(fabs(gens.at(daughterindex).PID())!=15) break;

    }

    //cout << currentindex << endl;
    //cout << gens.at(currentindex).PID() << endl;

    if(pair.first == 9900016)           SignalGenTaus.first = gens.at(currentindex);
    if(pair.first == 34)               SignalGenTaus.second = gens.at(currentindex);

    //idx++;
  }

  return SignalGenTaus;

}

int WRTau_Core::GetDaughterIndex(Gen me, const std::vector<Gen>& gens){

  int idx = -999;

  for(const auto &g : gens){
    if(g.MotherIndex() == me.Index()) idx = g.Index();
  }

  return idx;

}

std::vector<int> WRTau_Core::GetDaughterIndexVector(Gen me, const std::vector<Gen>& gens){

  std::vector<int> idx;

  for(const auto &g : gens){
    if(g.MotherIndex() == me.Index()) idx.push_back(g.Index());
  }

  return idx;

}

Gen WRTau_Core::GetStableTauDaughter(const Gen me,const std::vector<Gen>& gens){

  int currentindex = me.Index();
  int daughterindex = GetDaughterIndex(me,gens);

   while( fabs(gens.at(daughterindex).PID()) == 15 || gens.at(daughterindex).Status() != 1 ){

      currentindex = daughterindex;
      daughterindex = GetDaughterIndex(gens.at(daughterindex),gens);

      if(fabs(gens.at(daughterindex).PID())!=15){
        if(fabs(gens.at(daughterindex).Status()) == 1) break;
        else continue;
      }

    }

    return gens.at(daughterindex);

}

std::map<WRTau_Core::SearchRegion,bool> WRTau_Core::getGenLevelChannelMap(const std::vector<Gen>& gens){

  bool _isTauChannel(false);
  bool _isGenTauHTauH(false);
  bool _isGenTauHTauEl(false);
  bool _isGenTauHTauMu(false);
  bool _isGenTauElTauH(false);
  bool _isGenTauMuTauH(false);
  bool _isGenTauLTauL(false);
  
  for(const auto &gen : gens){
    if(gen.PID()==9900016){
      _isTauChannel = true;
      break;
    }
  }

  int nElTauFromN(0),nMuTauFromN(0);
  int nElTauFromWR(0),nMuTauFromWR(0);

  if(_isTauChannel){

    for(unsigned int i=2; i<gens.size();i++){

      Gen gen = gens.at(i);
      if(fabs(gen.PID())==11 || fabs(gen.PID())==13){

        std::vector<int> LeptonHistIdx = TrackGenSelfHistory(gen,gens);
        if(LeptonHistIdx[1]!=-1){
           
          Gen LeptonMother = gens.at(LeptonHistIdx[1]);

          if(fabs(LeptonMother.PID())==15){

            std::vector<int> TauHistIdx = TrackGenSelfHistory(LeptonMother,gens);
            if(TauHistIdx[1]!=-1){

              Gen TauMother = gens.at(TauHistIdx[1]);

              if(fabs(TauMother.PID())==9900016 && fabs(gen.PID())==11)    nElTauFromN  += 1;
              if(fabs(TauMother.PID())==9900016 && fabs(gen.PID())==13)    nMuTauFromN  += 1;
              if(fabs(TauMother.PID())==34 && fabs(gen.PID())==11)         nElTauFromWR += 1;
              if(fabs(TauMother.PID())==34 && fabs(gen.PID())==13)         nMuTauFromWR += 1;

            }
          }
        }
      }
    }
  }

  _isGenTauHTauH  = ((nElTauFromWR + nMuTauFromWR) == 0 ) && ((nElTauFromN + nMuTauFromN) == 0 );
  _isGenTauHTauEl = ((nElTauFromWR + nMuTauFromWR) == 0 ) && (nElTauFromN == 1);
  _isGenTauHTauMu = ((nElTauFromWR + nMuTauFromWR) == 0 ) && (nMuTauFromN == 1);
  _isGenTauElTauH = (nElTauFromWR == 1) && ((nElTauFromN + nMuTauFromN) == 0 );
  _isGenTauMuTauH = (nMuTauFromWR == 1) && ((nElTauFromN + nMuTauFromN) == 0 );
  _isGenTauLTauL  = ((nElTauFromWR  +nMuTauFromWR) != 0 ) && ((nElTauFromN + nMuTauFromN) != 0 );

  map<WRTau_Core::SearchRegion,bool> m_region = {
    {WRTau_Core::GenDebug,!_isTauChannel},  // debug flag
    {WRTau_Core::GenTauHTauH,_isGenTauHTauH},
    {WRTau_Core::GenTauHTauEl,_isGenTauHTauEl},
    {WRTau_Core::GenTauHTauMu,_isGenTauHTauMu},
    {WRTau_Core::GenTauElTauH,_isGenTauElTauH},
    {WRTau_Core::GenTauMuTauH,_isGenTauMuTauH},
    {WRTau_Core::GenTauLTauL,_isGenTauLTauL}
  };

  return m_region;

}


WRTau_Core::WRTau_Core(){

  histDir = GetTempDir_WRTauCore();

}

WRTau_Core::~WRTau_Core(){

}


