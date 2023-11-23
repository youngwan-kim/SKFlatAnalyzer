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
  if(FirstTauAncestorHistory[1]>0){
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
    //cout << "[WRTau_Core::GetTauAncestor] Loop (currentidx_PID)=" << gens.at(currentidx).PID() << endl;
    //if(motherindex != -1) cout << "[WRTau_Core::GetTauAncestor] Loop (motheridx_PID)=" << "," << gens.at(motherindex).PID() << endl;


    if(currentidx <= 2 || motherindex < 0){
      motherindex = -1;
      break;
    }
    else if(motherindex == 0){
      motherindex = 0 ;
      break;
    }
    else if(fabs(gens.at(motherindex).PID()) == 15) break;
  }

  //cout << "[WRTau_Core::GetTauAncestor] Final (currentidx,motheridx)=" << currentidx << "," << motherindex << endl;
  //cout << "[WRTau_Core::GetTauAncestor] Final (currentidx_PID)=" << gens.at(currentidx).PID() << endl;
  //if(motherindex!=-1) cout << "[WRTau_Core::GetTauAncestor] Final (motheridx_PID)=" << "," << gens.at(motherindex).PID() << endl;
  //cout << "[WRTau_Core::GetTauAncestor] End" << endl;

  vector<int> out = {currentidx, motherindex};

  return out;

}

vector<Tau> WRTau_Core::TauPromptOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens){

  if(IsDATA) return taus;

  std::vector<Tau> out;

  for(unsigned int i=0; i<taus.size(); i++){
    if(GetTauType(taus.at(i), gens)<0) continue;
    out.push_back(taus.at(i));
  }

  return out;

}

vector<Tau> WRTau_Core::TauFakeOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens){

  if(IsDATA) return taus;

  std::vector<Tau> out;

  for(unsigned int i=0; i<taus.size(); i++){
    if(GetTauType(taus.at(i), gens)>0) continue;
    out.push_back(taus.at(i));
  }

  return out;

}

bool WRTau_Core::IsFakeTau(const Tau tau, const std::vector<Gen>& gens){
  
  if(IsDATA) return false;
  if(GetTauType(tau,gens)<0) return true;
  else false;

}

bool WRTau_Core::IsPromptTau(const Tau tau, const std::vector<Gen>& gens){
  
  if(IsDATA) return false;
  if(GetTauType(tau,gens)>0) return true;
  else false;

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
  if (region == WRTau_Core::ResolvedLowMassControlRegion)       region_string="ResolvedLowMassControlRegion";
  if (region == WRTau_Core::BoostedLowMassControlRegion)        region_string="BoostedLowMassControlRegion";
  if (region == WRTau_Core::ResolvedLowMassControlRegionMass1)  region_string="ResolvedLowMassControlRegionMass1";
  if (region == WRTau_Core::BoostedLowMassControlRegionMass1)   region_string="BoostedLowMassControlRegionMass1";
  if (region == WRTau_Core::ResolvedSignalRegion)               region_string="ResolvedSignalRegion";
  if (region == WRTau_Core::BoostedSignalRegion)                region_string="BoostedSignalRegion";
  if (region == WRTau_Core::ResolvedSignalRegionMass1)          region_string="ResolvedSignalRegionMass1";
  if (region == WRTau_Core::BoostedSignalRegionMass1)           region_string="BoostedSignalRegionMass1";
  if (region == WRTau_Core::WJetsControlRegion)                 region_string="WJetsControlRegion";
  if (region == WRTau_Core::QCDEnrichedControlRegionAK4)        region_string="QCDEnrichedControlRegionAK4";
  if (region == WRTau_Core::QCDEnrichedControlRegionAK8)        region_string="QCDEnrichedControlRegionAK8";

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
    if(J.DeltaPhi(taus.at(0))>2.0 && J.LSF()>0.6) fatjets_BoostedSR.push_back(J);
  }

  if(fatjets_BoostedSR.size()>0){
    fatjet_BoostedSR = fatjets_BoostedSR.at(0);
    for(const auto &looselep : LooseLeptons){
      if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
    }
    if(leptons_BoostedSR.size()>0 ){
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
    if(J.DeltaPhi(taus.at(0))>2.0 && J.LSF()>0.6) fatjets_BoostedSR.push_back(J);
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
    if(J.DeltaPhi(taus.at(0))>2.0 && J.LSF()>0.6) fatjets_BoostedSR.push_back(J);
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

map<WRTau_Core::SearchRegion,bool> WRTau_Core::GetGenRegion(const std::vector<Gen>& gens){

  std::map<WRTau_Core::SearchRegion,bool> GenRegionMap = getGenLevelChannelMap(gens);
  std::map<WRTau_Core::SearchRegion,bool> map;
  
  bool _isGenResolvedElSignalRegion(false);
  bool _isGenResolvedMuSignalRegion(false);
  bool _isGenBoostedElSignalRegion(false);
  bool _isGenBoostedMuSignalRegion(false);

  for(auto const& m : GenRegionMap){

    if(m.first == WRTau_Core::GenTauHTauEl){

    }

    else if(m.first == WRTau_Core::GenTauHTauMu){

    }

  }

  map = {
    {WRTau_Core::GenResolvedElSignalRegion,_isGenResolvedElSignalRegion},
    {WRTau_Core::GenResolvedMuSignalRegion,_isGenResolvedMuSignalRegion},
    {WRTau_Core::GenBoostedElSignalRegion,_isGenBoostedElSignalRegion},
    {WRTau_Core::GenBoostedMuSignalRegion,_isGenBoostedMuSignalRegion},
  };

  return map;

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

  bool hasAtLeast2AK4Jets = jets.size()>1;
  bool hasAtLeast1AK8Jets = fatjets.size()>0;
  bool hasAtLeast1TightLeptons = TightLeptons.size()>0;
  bool hasAtLeast1LooseLeptons = LooseLeptons.size()>0;

  _isBaselinePreselection = taus.size()>0 && taus.at(0).Pt()>190 && LooseLeptons.size()==1;
  _isResolvedPreselection = _isBaselinePreselection && hasAtLeast2AK4Jets && TightLeptons.size()==1;
  _isBoostedPreselection = _isBaselinePreselection && !_isResolvedPreselection && hasAtLeast1AK8Jets;


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
        double mwr1 = GetResolvedSRMass_RecoNeutrino(METv,taus,jets,TightLeptons);
        double mwr  = GetResolvedSRMass(METv,taus,jets,TightLeptons,false);
        
        if(mwr<800) _isResolvedLowMassControlRegion = true ; // TODO : study mass cut optimization ; make a submethod to vary mass cuts and check significance in 2D
        else if(mwr>800) _isResolvedSignalRegion = true;

        if(mwr1<800) _isResolvedLowMassControlRegionMass1 = true;
        else if(mwr1>800) _isResolvedSignalRegionMass1 = true;
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
      if(J.DeltaPhi(taus.at(0))>2.0 && J.LSF()>0.6) fatjets_BoostedSR.push_back(J);
    }
    if(fatjets_BoostedSR.size()>0){
      fatjet_BoostedSR = fatjets_BoostedSR.at(0);
      for(const auto &looselep : LooseLeptons){
        if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
      }
      if(leptons_BoostedSR.size()>0 && fatjet_BoostedSR.LSF()>0.6){
        
        //double mwr1 = GetBoostedSRMass_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
        double mwr  = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
        double mwr1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);

        if(mwr<250) _isBoostedLowMassControlRegion = true ;
        else if(mwr>250) _isBoostedSignalRegion = true;

        if(mwr1<800) _isBoostedLowMassControlRegionMass1 = true ;
        else if(mwr1>800) _isBoostedSignalRegionMass1 = true;

        //cout << "-----" << endl;
        //cout << "mwr,mwr1 : " << mwr << " , " << mwr1 << endl;
        //cout << "isLMCR,isLMCR1 : " << _isBoostedLowMassControlRegion << " , " << _isBoostedLowMassControlRegionMass1 << endl;
        
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
    {WRTau_Core::WJetsControlRegion,_isWJetsControlRegion}
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

        if(region.first == WRTau_Core::BoostedPreselection || region.first == WRTau_Core::BoostedLowMassControlRegion || region.first == WRTau_Core::BoostedLowMassControlRegionMass1 || region.first == WRTau_Core::BoostedSignalRegion || region.first == WRTau_Core::BoostedSignalRegionMass1){

          double M1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
          double M2 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);
          double M3 = GetBoostedSRMass_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
          double M4 = GetBoostedSRMassN_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
          if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
          if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
          if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
          if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

        }

        if(region.first == WRTau_Core::ResolvedPreselection || region.first == WRTau_Core::ResolvedLowMassControlRegion || region.first == WRTau_Core::ResolvedLowMassControlRegionMass1  || region.first == WRTau_Core::ResolvedSignalRegion){
          
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

void WRTau_Core::FillPassingRegions(map<pair<WRTau_Core::SearchRegion,double>, bool> m_LSFCut,Particle METv,const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight, std::tuple<int,int,int> idtuple, bool highpT){

    for(auto const& p_cutregion : m_LSFCut){
      WRTau_Core::SearchRegion cutregion = p_cutregion.first.first;
      double cut = p_cutregion.first.second;

      if(p_cutregion.second){

        std::pair<std::vector<Lepton *>,std::vector<Lepton *>> PairVecLeps = std::make_pair(LooseLeptons,TightLeptons);
        vector<Lepton *> leptons = ChooseLeptonColl(cutregion,PairVecLeps);

        WRTau_Core::Channel ch = GetChannel(leptons);
        
        TString cutval = std::to_string(cut);
        cutval.Remove(cutval.Length() - 4, 4);
        cutval.ReplaceAll(".", "p");

        TString label = fillpath+"/"+GetRegionString(cutregion) + "_LSF"+ cutval; 
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

          if(cutregion == WRTau_Core::BoostedPreselection || cutregion == WRTau_Core::BoostedLowMassControlRegion || cutregion == WRTau_Core::BoostedLowMassControlRegionMass1 || cutregion == WRTau_Core::BoostedSignalRegion){

            double M1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
            double M2 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);
            double M3 = GetBoostedSRMass_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
            double M4 = GetBoostedSRMassN_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
            if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
            if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
            if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
            if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

          }

          if(cutregion == WRTau_Core::ResolvedPreselection || cutregion == WRTau_Core::ResolvedLowMassControlRegion || cutregion == WRTau_Core::ResolvedLowMassControlRegionMass1  || cutregion == WRTau_Core::ResolvedSignalRegion){

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

void WRTau_Core::FillPassingRegions(map<WRTau_Core::SearchRegion,bool> m_region,WRTau_Core::SearchRegion r,Particle METv, const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight,std::tuple<int,int,int> idtuple, bool highpT){
  
    if(m_region[r]==true){

      std::pair<std::vector<Lepton *>,std::vector<Lepton *>> PairVecLeps = std::make_pair(LooseLeptons,TightLeptons);
      vector<Lepton *> leptons = ChooseLeptonColl(r,PairVecLeps);

      WRTau_Core::Channel ch = GetChannel(leptons);
      TString label = fillpath+"/"+GetRegionString(r); 
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

        if(r == WRTau_Core::BoostedPreselection || r == WRTau_Core::BoostedLowMassControlRegion || r == WRTau_Core::BoostedLowMassControlRegionMass1 || r == WRTau_Core::BoostedSignalRegion){

          double M1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
          double M2 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);
          double M3 = GetBoostedSRMass_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
          double M4 = GetBoostedSRMassN_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
          if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
          if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
          if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
          if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

        }

        if(r == WRTau_Core::ResolvedPreselection || r  == WRTau_Core::ResolvedLowMassControlRegion || r == WRTau_Core::ResolvedLowMassControlRegionMass1  || r  == WRTau_Core::ResolvedSignalRegion){
          
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
  
  return;

}

map<pair<WRTau_Core::SearchRegion,double>, bool> WRTau_Core::LSFCutter(map<WRTau_Core::SearchRegion,bool> m_region,vector<double> LSFCuts,std::vector<FatJet> fatjets){

  map<pair<WRTau_Core::SearchRegion,double>, bool> cutmap;
  cutmap.insert(std::make_pair(std::make_pair(WRTau_Core::None,-999.),false));
  int i(0);
  for(const auto r : BoostedRegions){
    if(m_region[r]){
      for(const auto cut : LSFCuts){
        cutmap.insert(std::make_pair(std::make_pair(r,cut),fatjets.at(0).LSF()>cut));
      }
      i++;
    }
  }
  if(i!=0) cutmap.erase(std::make_pair(WRTau_Core::None,-999.));
  return cutmap;
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
    if(leps.size()>0){
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
    }

    return w_tau * w_lepton;

  }
  
}

double WRTau_Core::GetMatchedWeight(const std::vector<Tau>& taus,std::tuple<int,int,int> idtuple,bool highpT){

  if(IsDATA) return 1.0;
  
  // weight = w_tau * w_lep 
  else{

    double w_tau(1.0);


    if(!HasFlag("NonpromptTau")){
      if(highpT) w_tau = tauidsftool_map[idtuple]->getHighPTSFvsPT(taus.at(0).Pt());
      else w_tau = tauidsftool_map[idtuple]->getSFvsPT(taus.at(0).Pt());
    }

    return w_tau ;

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

double WRTau_Core::SetupWeight(Event ev, AnalyzerParameter param){

  //=== Apply MC weight                                                                                                                                                                                    
  if(IsDATA) return 1.;

  double prefire_weight = 1.;
  
  if(param.syst_ == AnalyzerParameter::PrefireUp) prefire_weight = GetPrefireWeight(1);
  else if(param.syst_ == AnalyzerParameter::PrefireDown)  prefire_weight = GetPrefireWeight(-1);
  else  prefire_weight = GetPrefireWeight(0);


  double pileup_weight(1.);
  if(param.syst_ == AnalyzerParameter::PUUp) pileup_weight= GetPileUpWeight(nPileUp,1);
  else if(param.syst_ == AnalyzerParameter::PUDown) pileup_weight= GetPileUpWeight(nPileUp,-1);
  else pileup_weight= GetPileUpWeight(nPileUp,0);
  
  double this_mc_weight = ev.GetTriggerLumi("Full") * MCweight(true, true) * pileup_weight * GetKFactor()*prefire_weight;
  //if(MCSample.Contains("Type") && Analyzer == "HNL_SignalLeptonOpt") this_mc_weight = MCweight(true, false) * pileup_weight * prefire_weight;


  FillWeightHist("PrefireWeight_" ,GetPrefireWeight(0));
  FillWeightHist("MCWeight_" ,MCweight(true, true));
  FillWeightHist("MCFullWeight_" , this_mc_weight);
  FillWeightHist("PileupWeight_" , GetPileUpWeight(nPileUp,0));

  double w_topptrw(1.);
  if(MCSample.Contains("TT") and MCSample.Contains("powheg")) {
    w_topptrw = mcCorr->GetTopPtReweight(GetGens()); 
    FillWeightHist("TopPtWeight_" , w_topptrw);
  }
  
  //if(MCSample.Contains("DYJets")) {
  //  double dyRW =   mcCorr->GetOfficialDYReweight(GetGens(),0);
  //  FillWeightHist("DYReWeight_" , dyRW);
  //  this_mc_weight= this_mc_weight*dyRW;
 
 // }
  

  return this_mc_weight;
  
}

Particle WRTau_Core::GetvMET(TString METType){

  bool IsType1   = METType.Contains("T1");
  bool IsxyCorr  = METType.Contains("xyCorr");
  bool UsePuppi  = METType.Contains("Puppi");
  bool IsFixxyCorr  = METType.Contains("xyULCorr");

  //METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, TString year, bool isMC, int npv, bool isUL =false,bool ispuppi=false)r

  double Met_pt(0.), Met_phi(0.);

  Particle vMET;
  if(UsePuppi){
    if(IsType1){
      if(IsxyCorr) {
	Met_pt=PuppiMET_Type1_PhiCor_pt; 
	Met_phi = PuppiMET_Type1_PhiCor_phi;
      } 
      else  {
	Met_pt=PuppiMET_Type1_pt;
	Met_phi=PuppiMET_Type1_phi;
      }
    }// T1
    else{
      Met_pt=PuppiMET_pt;
      Met_phi=PuppiMET_phi;
    }
  } // PUPPI
  
  else{
    // PRMET
    if(IsType1){
      if(IsxyCorr) {
	Met_pt =pfMET_Type1_PhiCor_pt;
	Met_phi=pfMET_Type1_PhiCor_phi;
      }
      else{
	Met_pt =pfMET_Type1_pt;
	Met_phi =pfMET_Type1_phi;
      }
    }
    else{
      Met_pt=pfMET_pt;
      Met_phi=pfMET_phi;
    }
  }
  if(IsFixxyCorr) {
    TString Year= "2016";
    if(DataYear==2017) Year= "2017";
    if(DataYear==2018) Year= "2018";
    std::pair<double,double> METPair = METXYCorr_Met_MetPhi(Met_pt, Met_phi, run, Year, !IsData, nPV, true,UsePuppi);
    Met_pt=METPair.first;
    Met_phi=METPair.second;
  }

  vMET.SetPtEtaPhiM(Met_pt, 0., Met_phi, 0.);
  return vMET;

}

Particle WRTau_Core::GetvMET(TString METType, AnalyzerParameter param){

  bool IsType1   = METType.Contains("T1");
  bool IsxyCorr  = METType.Contains("xyCorr");
  bool UsePuppi  = METType.Contains("Puppi");

  bool IsJetSmear  = METType.Contains("JetSmear");

  int IdxSyst = -1;

  if(param.syst_ == AnalyzerParameter::METUnclUp)   IdxSyst = 10;
  if(param.syst_ == AnalyzerParameter::METUnclDown)   IdxSyst = 11;

  // Use CMSSW MET SYSTa
  if(METType.Contains("CMSSW")){
    if(param.syst_ == AnalyzerParameter::JetEnUp)  IdxSyst = 2;
    if(param.syst_ == AnalyzerParameter::JetEnDown)  IdxSyst = 3;
    if(param.syst_ == AnalyzerParameter::JetResUp)  IdxSyst = 0;
    if(param.syst_ == AnalyzerParameter::JetResDown)  IdxSyst = 1;
  }
  bool ApplySyst = (!IsDATA) && IdxSyst >= 0;

  Particle vMET;

  if(UsePuppi){
    if(IsType1){
      if( (!ApplySyst) ){
	if(IsxyCorr) vMET.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.); 
	else         vMET.SetPtEtaPhiM(PuppiMET_Type1_pt, 0., PuppiMET_Type1_phi, 0.); 
      }
      else{
	if(IsxyCorr)  vMET.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.);
	else         vMET.SetPtEtaPhiM(PuppiMET_Type1_pt_shifts->at(IdxSyst), 0.,  PuppiMET_Type1_phi_shifts->at(IdxSyst), 0.); 
      }
    }
    
  }
  else{
    if(IsType1){
      if( (!ApplySyst) or ( IdxSyst>=0 && (!isfinite(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst))) ) ){
	if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt, 0., pfMET_Type1_PhiCor_phi, 0.); 
	else         vMET.SetPtEtaPhiM(pfMET_Type1_pt, 0., pfMET_Type1_phi, 0.); 
      }
      else{
	if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst), 0., pfMET_Type1_PhiCor_phi_shifts->at(IdxSyst), 0.); 
	else         vMET.SetPtEtaPhiM(pfMET_Type1_pt_shifts->at(IdxSyst), 0., pfMET_Type1_phi_shifts->at(IdxSyst), 0.); 
      }
    }

  }

  if (!IsJetSmear && param.syst_ == AnalyzerParameter::Central) return vMET;
  
  Particle vMETSmeared;
  vector<Jet> _jets = GetJets(param, param.Jet_ID, 10., 5.);
  if (IsJetSmear) vMETSmeared = UpdateMETSmearedJet(vMET, _jets);

  if(param.syst_ == AnalyzerParameter::Central) return vMETSmeared;
  double MET = vMETSmeared.Pt();
  double METPhi = vMETSmeared.Pt();
  //if(param.syst_ == AnalyzerParameter::JetResUp ) CorrectedMETJER(1, GetJets(param, 10., 5.), GetFatJets(param, 200., 5.), MET,METPhi);
  //if(param.syst_ == AnalyzerParameter::JetResDown) CorrectedMETJER(-11, GetJets(param, 10., 5.), GetFatJets(param, 200., 5.), MET,METPhi);
  // Write CorrectedMETXXhttps://github.com/jedori0228/LQanalyzer/blob/CatAnalyzer_13TeV_v8-0-7.36_HNAnalyzer/LQAnalysis/Analyzers/src/AnalyzerCore.cc#L4826

  Particle vMETSyst;
  vMETSyst.SetPtEtaPhiM(MET, 0., METPhi, 0.);

  
  return vMETSyst;
}

std::pair<double,double> WRTau_Core::METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, TString year, bool isMC, int npv, bool isUL ,bool ispuppi){

  std::pair<double,double>  TheXYCorr_Met_MetPhi(uncormet,uncormet_phi);
  
  if(npv>100) npv=100;
  int runera =-1;
  bool usemetv2 =false;
  if(isMC && year == "2016" && !isUL) runera = y2016MC;
  else if(isMC && year == "2017" && !isUL) {runera = y2017MC; usemetv2 =true;}
  else if(isMC && year == "2018" && !isUL) runera = y2018MC;
  else if(isMC && year == "2016APV" && isUL) runera = yUL2016MCAPV;
  else if(isMC && year == "2016nonAPV" && isUL) runera = yUL2016MCnonAPV;
  else if(isMC && year == "2017" && isUL) runera = yUL2017MC;
  else if(isMC && year == "2018" && isUL) runera = yUL2018MC;
  
  
  else if(!isMC && runnb >=272007 && runnb <=275376 && !isUL) runera = y2016B;
  else if(!isMC && runnb >=275657 && runnb <=276283 && !isUL) runera = y2016C;
  else if(!isMC && runnb >=276315 && runnb <=276811 && !isUL) runera = y2016D;
  else if(!isMC && runnb >=276831 && runnb <=277420 && !isUL) runera = y2016E;
  else if(!isMC && runnb >=277772 && runnb <=278808 && !isUL) runera = y2016F;
  else if(!isMC && runnb >=278820 && runnb <=280385 && !isUL) runera = y2016G;
  else if(!isMC && runnb >=280919 && runnb <=284044 && !isUL) runera = y2016H;
  
  else if(!isMC && runnb >=297020 && runnb <=299329 && !isUL){ runera = y2017B; usemetv2 =true;}
  else if(!isMC && runnb >=299337 && runnb <=302029 && !isUL){ runera = y2017C; usemetv2 =true;}
  else if(!isMC && runnb >=302030 && runnb <=303434 && !isUL){ runera = y2017D; usemetv2 =true;}
  else if(!isMC && runnb >=303435 && runnb <=304826 && !isUL){ runera = y2017E; usemetv2 =true;}
  else if(!isMC && runnb >=304911 && runnb <=306462 && !isUL){ runera = y2017F; usemetv2 =true;}
  
  else if(!isMC && runnb >=315252 && runnb <=316995 && !isUL) runera = y2018A;
  else if(!isMC && runnb >=316998 && runnb <=319312 && !isUL) runera = y2018B;
  else if(!isMC && runnb >=319313 && runnb <=320393 && !isUL) runera = y2018C;
  else if(!isMC && runnb >=320394 && runnb <=325273 && !isUL) runera = y2018D;

  else if(!isMC && runnb >=315252 && runnb <=316995 && isUL) runera = yUL2018A;
  else if(!isMC && runnb >=316998 && runnb <=319312 && isUL) runera = yUL2018B;
  else if(!isMC && runnb >=319313 && runnb <=320393 && isUL) runera = yUL2018C;
  else if(!isMC && runnb >=320394 && runnb <=325273 && isUL) runera = yUL2018D;

  else if(!isMC && runnb >=297020 && runnb <=299329 && isUL){ runera = yUL2017B; usemetv2 =false;}
  else if(!isMC && runnb >=299337 && runnb <=302029 && isUL){ runera = yUL2017C; usemetv2 =false;}
  else if(!isMC && runnb >=302030 && runnb <=303434 && isUL){ runera = yUL2017D; usemetv2 =false;}
  else if(!isMC && runnb >=303435 && runnb <=304826 && isUL){ runera = yUL2017E; usemetv2 =false;}
  else if(!isMC && runnb >=304911 && runnb <=306462 && isUL){ runera = yUL2017F; usemetv2 =false;}

  else if(!isMC && runnb >=272007 && runnb <=275376 && isUL) runera = yUL2016B;
  else if(!isMC && runnb >=275657 && runnb <=276283 && isUL) runera = yUL2016C;
  else if(!isMC && runnb >=276315 && runnb <=276811 && isUL) runera = yUL2016D;
  else if(!isMC && runnb >=276831 && runnb <=277420 && isUL) runera = yUL2016E;
  else if(!isMC && ((runnb >=277772 && runnb <=278768) || runnb==278770) && isUL) runera = yUL2016F;
  else if(!isMC && ((runnb >=278801 && runnb <=278808) || runnb==278769) && isUL) runera = yUL2016Flate;
  else if(!isMC && runnb >=278820 && runnb <=280385 && isUL) runera = yUL2016G;
  else if(!isMC && runnb >=280919 && runnb <=284044 && isUL) runera = yUL2016H;


  else {
    //Couldn't find data/MC era => no correction applied
    return TheXYCorr_Met_MetPhi;
  }
  
  double METxcorr(0.),METycorr(0.);

  if(!usemetv2){//Current recommendation for 2016 and 2018
    if(!ispuppi){
      if(runera==y2016B) METxcorr = -(-0.0478335*npv -0.108032);
      if(runera==y2016B) METycorr = -(0.125148*npv +0.355672);
      if(runera==y2016C) METxcorr = -(-0.0916985*npv +0.393247);
      if(runera==y2016C) METycorr = -(0.151445*npv +0.114491);
      if(runera==y2016D) METxcorr = -(-0.0581169*npv +0.567316);
      if(runera==y2016D) METycorr = -(0.147549*npv +0.403088);
      if(runera==y2016E) METxcorr = -(-0.065622*npv +0.536856);
      if(runera==y2016E) METycorr = -(0.188532*npv +0.495346);
      if(runera==y2016F) METxcorr = -(-0.0313322*npv +0.39866);
      if(runera==y2016F) METycorr = -(0.16081*npv +0.960177);
      if(runera==y2016G) METxcorr = -(0.040803*npv -0.290384);
      if(runera==y2016G) METycorr = -(0.0961935*npv +0.666096);
      if(runera==y2016H) METxcorr = -(0.0330868*npv -0.209534);
      if(runera==y2016H) METycorr = -(0.141513*npv +0.816732);
      if(runera==y2017B) METxcorr = -(-0.259456*npv +1.95372);
      if(runera==y2017B) METycorr = -(0.353928*npv -2.46685);
      if(runera==y2017C) METxcorr = -(-0.232763*npv +1.08318);
      if(runera==y2017C) METycorr = -(0.257719*npv -1.1745);
      if(runera==y2017D) METxcorr = -(-0.238067*npv +1.80541);
      if(runera==y2017D) METycorr = -(0.235989*npv -1.44354);
      if(runera==y2017E) METxcorr = -(-0.212352*npv +1.851);
      if(runera==y2017E) METycorr = -(0.157759*npv -0.478139);
      if(runera==y2017F) METxcorr = -(-0.232733*npv +2.24134);
      if(runera==y2017F) METycorr = -(0.213341*npv +0.684588);
      if(runera==y2018A) METxcorr = -(0.362865*npv -1.94505);
      if(runera==y2018A) METycorr = -(0.0709085*npv -0.307365);
      if(runera==y2018B) METxcorr = -(0.492083*npv -2.93552);
      if(runera==y2018B) METycorr = -(0.17874*npv -0.786844);
      if(runera==y2018C) METxcorr = -(0.521349*npv -1.44544);
      if(runera==y2018C) METycorr = -(0.118956*npv -1.96434);
      if(runera==y2018D) METxcorr = -(0.531151*npv -1.37568);
      if(runera==y2018D) METycorr = -(0.0884639*npv -1.57089);
      if(runera==y2016MC) METxcorr = -(-0.195191*npv -0.170948);
      if(runera==y2016MC) METycorr = -(-0.0311891*npv +0.787627);
      if(runera==y2017MC) METxcorr = -(-0.217714*npv +0.493361);
      if(runera==y2017MC) METycorr = -(0.177058*npv -0.336648);
      if(runera==y2018MC) METxcorr = -(0.296713*npv -0.141506);
      if(runera==y2018MC) METycorr = -(0.115685*npv +0.0128193);
    
      //UL2017
      if(runera==yUL2017B) METxcorr = -(-0.211161*npv +0.419333);
      if(runera==yUL2017B) METycorr = -(0.251789*npv +-1.28089);
      if(runera==yUL2017C) METxcorr = -(-0.185184*npv +-0.164009);
      if(runera==yUL2017C) METycorr = -(0.200941*npv +-0.56853);
      if(runera==yUL2017D) METxcorr = -(-0.201606*npv +0.426502);
      if(runera==yUL2017D) METycorr = -(0.188208*npv +-0.58313);
      if(runera==yUL2017E) METxcorr = -(-0.162472*npv +0.176329);
      if(runera==yUL2017E) METycorr = -(0.138076*npv +-0.250239);
      if(runera==yUL2017F) METxcorr = -(-0.210639*npv +0.72934);
      if(runera==yUL2017F) METycorr = -(0.198626*npv +1.028);
      if(runera==yUL2017MC) METxcorr = -(-0.300155*npv +1.90608);
      if(runera==yUL2017MC) METycorr = -(0.300213*npv +-2.02232);

      //UL2018
      if(runera==yUL2018A) METxcorr = -(0.263733*npv +-1.91115);
      if(runera==yUL2018A) METycorr = -(0.0431304*npv +-0.112043);
      if(runera==yUL2018B) METxcorr = -(0.400466*npv +-3.05914);
      if(runera==yUL2018B) METycorr = -(0.146125*npv +-0.533233);
      if(runera==yUL2018C) METxcorr = -(0.430911*npv +-1.42865);
      if(runera==yUL2018C) METycorr = -(0.0620083*npv +-1.46021);
      if(runera==yUL2018D) METxcorr = -(0.457327*npv +-1.56856);
      if(runera==yUL2018D) METycorr = -(0.0684071*npv +-0.928372);
      if(runera==yUL2018MC) METxcorr = -(0.183518*npv +0.546754);
      if(runera==yUL2018MC) METycorr = -(0.192263*npv +-0.42121);

      //UL2016
      if(runera==yUL2016B) METxcorr = -(-0.0214894*npv +-0.188255);
      if(runera==yUL2016B) METycorr = -(0.0876624*npv +0.812885);
      if(runera==yUL2016C) METxcorr = -(-0.032209*npv +0.067288);
      if(runera==yUL2016C) METycorr = -(0.113917*npv +0.743906);
      if(runera==yUL2016D) METxcorr = -(-0.0293663*npv +0.21106);
      if(runera==yUL2016D) METycorr = -(0.11331*npv +0.815787);
      if(runera==yUL2016E) METxcorr = -(-0.0132046*npv +0.20073);
      if(runera==yUL2016E) METycorr = -(0.134809*npv +0.679068);
      if(runera==yUL2016F) METxcorr = -(-0.0543566*npv +0.816597);
      if(runera==yUL2016F) METycorr = -(0.114225*npv +1.17266);
      if(runera==yUL2016Flate) METxcorr = -(0.134616*npv +-0.89965);
      if(runera==yUL2016Flate) METycorr = -(0.0397736*npv +1.0385);
      if(runera==yUL2016G) METxcorr = -(0.121809*npv +-0.584893);
      if(runera==yUL2016G) METycorr = -(0.0558974*npv +0.891234);
      if(runera==yUL2016H) METxcorr = -(0.0868828*npv +-0.703489);
      if(runera==yUL2016H) METycorr = -(0.0888774*npv +0.902632);
      if(runera==yUL2016MCnonAPV) METxcorr = -(-0.153497*npv +-0.231751);
      if(runera==yUL2016MCnonAPV) METycorr = -(0.00731978*npv +0.243323);
      if(runera==yUL2016MCAPV) METxcorr = -(-0.188743*npv +0.136539);
      if(runera==yUL2016MCAPV) METycorr = -(0.0127927*npv +0.117747);



    }
    
    //UL2017Puppi
    if(ispuppi){
      if(runera==yUL2017B) METxcorr = -(-0.00382117*npv +-0.666228);
      if(runera==yUL2017B) METycorr = -(0.0109034*npv +0.172188);
      if(runera==yUL2017C) METxcorr = -(-0.00110699*npv +-0.747643);
      if(runera==yUL2017C) METycorr = -(-0.0012184*npv +0.303817);
      if(runera==yUL2017D) METxcorr = -(-0.00141442*npv +-0.721382);
      if(runera==yUL2017D) METycorr = -(-0.0011873*npv +0.21646);
      if(runera==yUL2017E) METxcorr = -(0.00593859*npv +-0.851999);
      if(runera==yUL2017E) METycorr = -(-0.00754254*npv +0.245956);
      if(runera==yUL2017F) METxcorr = -(0.00765682*npv +-0.945001);
      if(runera==yUL2017F) METycorr = -(-0.0154974*npv +0.804176);
      if(runera==yUL2017MC) METxcorr = -(-0.0102265*npv +-0.446416);
      if(runera==yUL2017MC) METycorr = -(0.0198663*npv +0.243182);

      //UL2018Puppi
      if(runera==yUL2018A) METxcorr = -(-0.0073377*npv +0.0250294);
      if(runera==yUL2018A) METycorr = -(-0.000406059*npv +0.0417346);
      if(runera==yUL2018B) METxcorr = -(0.00434261*npv +0.00892927);
      if(runera==yUL2018B) METycorr = -(0.00234695*npv +0.20381);
      if(runera==yUL2018C) METxcorr = -(0.00198311*npv +0.37026);
      if(runera==yUL2018C) METycorr = -(-0.016127*npv +0.402029);
      if(runera==yUL2018D) METxcorr = -(0.00220647*npv +0.378141);
      if(runera==yUL2018D) METycorr = -(-0.0160244*npv +0.471053);
      if(runera==yUL2018MC) METxcorr = -(-0.0214557*npv +0.969428);
      if(runera==yUL2018MC) METycorr = -(0.0167134*npv +0.199296);

      //UL2016Puppi
      if(runera==yUL2016B) METxcorr = -(-0.00109025*npv +-0.338093);
      if(runera==yUL2016B) METycorr = -(-0.00356058*npv +0.128407);
      if(runera==yUL2016C) METxcorr = -(-0.00271913*npv +-0.342268);
      if(runera==yUL2016C) METycorr = -(0.00187386*npv +0.104);
      if(runera==yUL2016D) METxcorr = -(-0.00254194*npv +-0.305264);
      if(runera==yUL2016D) METycorr = -(-0.00177408*npv +0.164639);
      if(runera==yUL2016E) METxcorr = -(-0.00358835*npv +-0.225435);
      if(runera==yUL2016E) METycorr = -(-0.000444268*npv +0.180479);
      if(runera==yUL2016F) METxcorr = -(0.0056759*npv +-0.454101);
      if(runera==yUL2016F) METycorr = -(-0.00962707*npv +0.35731);
      if(runera==yUL2016Flate) METxcorr = -(0.0234421*npv +-0.371298);
      if(runera==yUL2016Flate) METycorr = -(-0.00997438*npv +0.0809178);
      if(runera==yUL2016G) METxcorr = -(0.0182134*npv +-0.335786);
      if(runera==yUL2016G) METycorr = -(-0.0063338*npv +0.093349);
      if(runera==yUL2016H) METxcorr = -(0.015702*npv +-0.340832);
      if(runera==yUL2016H) METycorr = -(-0.00544957*npv +0.199093);
      if(runera==yUL2016MCnonAPV) METxcorr = -(-0.0058341*npv +-0.395049);
      if(runera==yUL2016MCnonAPV) METycorr = -(0.00971595*npv +-0.101288);
      if(runera==yUL2016MCAPV) METxcorr = -(-0.0060447*npv +-0.4183);
      if(runera==yUL2016MCAPV) METycorr = -(0.008331*npv +-0.0990046);


    }


  }
  else {//these are the corrections for v2 MET recipe (currently recommended for 2017)
    if(runera==y2016B) METxcorr = -(-0.0374977*npv +0.00488262);
    if(runera==y2016B) METycorr = -(0.107373*npv +-0.00732239);
    if(runera==y2016C) METxcorr = -(-0.0832562*npv +0.550742);
    if(runera==y2016C) METycorr = -(0.142469*npv +-0.153718);
    if(runera==y2016D) METxcorr = -(-0.0400931*npv +0.753734);
    if(runera==y2016D) METycorr = -(0.127154*npv +0.0175228);
    if(runera==y2016E) METxcorr = -(-0.0409231*npv +0.755128);
    if(runera==y2016E) METycorr = -(0.168407*npv +0.126755);
    if(runera==y2016F) METxcorr = -(-0.0161259*npv +0.516919);
    if(runera==y2016F) METycorr = -(0.141176*npv +0.544062);
    if(runera==y2016G) METxcorr = -(0.0583851*npv +-0.0987447);
    if(runera==y2016G) METycorr = -(0.0641427*npv +0.319112);
    if(runera==y2016H) METxcorr = -(0.0706267*npv +-0.13118);
    if(runera==y2016H) METycorr = -(0.127481*npv +0.370786);
    if(runera==y2017B) METxcorr = -(-0.19563*npv +1.51859);
    if(runera==y2017B) METycorr = -(0.306987*npv +-1.84713);
    if(runera==y2017C) METxcorr = -(-0.161661*npv +0.589933);
    if(runera==y2017C) METycorr = -(0.233569*npv +-0.995546);
    if(runera==y2017D) METxcorr = -(-0.180911*npv +1.23553);
    if(runera==y2017D) METycorr = -(0.240155*npv +-1.27449);
    if(runera==y2017E) METxcorr = -(-0.149494*npv +0.901305);
    if(runera==y2017E) METycorr = -(0.178212*npv +-0.535537);
    if(runera==y2017F) METxcorr = -(-0.165154*npv +1.02018);
    if(runera==y2017F) METycorr = -(0.253794*npv +0.75776);
    if(runera==y2018A) METxcorr = -(0.362642*npv +-1.55094);
    if(runera==y2018A) METycorr = -(0.0737842*npv +-0.677209);
    if(runera==y2018B) METxcorr = -(0.485614*npv +-2.45706);
    if(runera==y2018B) METycorr = -(0.181619*npv +-1.00636);
    if(runera==y2018C) METxcorr = -(0.503638*npv +-1.01281);
    if(runera==y2018C) METycorr = -(0.147811*npv +-1.48941);
    if(runera==y2018D) METxcorr = -(0.520265*npv +-1.20322);
    if(runera==y2018D) METycorr = -(0.143919*npv +-0.979328);
    if(runera==y2016MC) METxcorr = -(-0.159469*npv +-0.407022);
    if(runera==y2016MC) METycorr = -(-0.0405812*npv +0.570415);
    if(runera==y2017MC) METxcorr = -(-0.182569*npv +0.276542);
    if(runera==y2017MC) METycorr = -(0.155652*npv +-0.417633);
    if(runera==y2018MC) METxcorr = -(0.299448*npv +-0.13866);
    if(runera==y2018MC) METycorr = -(0.118785*npv +0.0889588);




    
  }

  double CorrectedMET_x = uncormet *cos( uncormet_phi)+METxcorr;
  double CorrectedMET_y = uncormet *sin( uncormet_phi)+METycorr;

  double CorrectedMET = sqrt(CorrectedMET_x*CorrectedMET_x+CorrectedMET_y*CorrectedMET_y);
  double CorrectedMETPhi;
  if(CorrectedMET_x==0 && CorrectedMET_y>0) CorrectedMETPhi = TMath::Pi();
  else if(CorrectedMET_x==0 && CorrectedMET_y<0 )CorrectedMETPhi = -TMath::Pi();
  else if(CorrectedMET_x >0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x);
  else if(CorrectedMET_x <0&& CorrectedMET_y>0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) + TMath::Pi();
  else if(CorrectedMET_x <0&& CorrectedMET_y<0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) - TMath::Pi();
  else CorrectedMETPhi =0;

  TheXYCorr_Met_MetPhi.first= CorrectedMET;
  TheXYCorr_Met_MetPhi.second= CorrectedMETPhi;
  return TheXYCorr_Met_MetPhi;

}


/*std::map<WRTau_Core::SearchRegion,std::pair<int,int>> WRTau_Core::getGenLevelChannelMap(const std::vector<Gen>& gens){

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

}*/


WRTau_Core::WRTau_Core(){

  histDir = GetTempDir_WRTauCore();

}

WRTau_Core::~WRTau_Core(){

}


