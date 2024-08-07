#include "WRTau_Core.h"

bool WRTau_Core::isSignalSample(){
  return MCSample.Contains("WRtoTauNtoTauTauJets");
}

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

  std::string year = "";
  if(DataEra=="2016preVFP") year = "UL2016_preVFP";
  else if(DataEra=="2016postVFP") year = "UL2016_postVFP";
  else year = "UL"+std::to_string(DataYear);

  for(const auto &vjet : vJet_vec){
    for(const auto &vel : vEl_vec){
      for(const auto &vmu : vMu_vec){
        std::tuple<int,int,int> IDtuple = std::make_tuple(vjet,vel,vmu);
        if( vjet > 2 && vjet < 6 ){
          tauidsftool_map[IDtuple]   = new TauIDSFTool(year,DeepTauVSjet,idname_map_str[vjet],idname_map_str[vel],false,false,false,true);
        }
        else tauidsftool_map[IDtuple]   = new TauIDSFTool(year,DeepTauVSjet,"Tight",idname_map_str[vel],false,false,false,true);
        tauidsftool_vEl_map[IDtuple] = new TauIDSFTool(year,DeepTauVSe,idname_map_str[vel],idname_map_str[vel]);
        tauidsftool_vMu_map[IDtuple] = new TauIDSFTool(year,DeepTauVSmu,idname_map_str[vmu],idname_map_str[vel]);
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
    FillHist(region+"/Tauh_eta",taus.at(0).Eta(),weight,100,-5.,5.);
    FillHist(region+"/Tauh_DM",taus.at(0).DecayMode(),weight,15,0.,15.);
    //FillLeptonPlots(TightLeptons,region+"/HighPtTight",weight);
    //FillLeptonPlots(LooseLeptons,region+"/HighPtLoose",weight);
    //cout << "[WRTauCore::FillPreselHists] region : " << region << endl;
    FillJetPlots(jets,fatjets,region+"/Jets",weight);
    //FillJetPlots(bjets,fatjets,region+"/BJets",weight);

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
  if(gen_closest.IsEmpty()){
    //cout << "[WRTau_Core::GetTauType] no close gen" << endl;
    TauType = -1;
  }
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

  if(TruthIdx<2) {
    //cout << "[WRTau_Core::GetTauType_Public,Error] Point1, TruthIdx " <<  TruthIdx  <<  endl;
    return -7;
  }
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
  if(TruthIdx == -1){
    TauType = 0;
    //cout << "[WRTau_Core::GetTauType_Public,Error] Point2, TruthIdx " <<  TruthIdx  <<  endl;
  }
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

  if(TauType == 0){
    if(HasFlag("debugTauGenMatch")) TruthColl.at(TruthIdx).Print();
    if(TruthColl.at(TruthIdx).MotherIndex() == 0 || TruthColl.at(TruthIdx).MotherIndex() == 1 ) TauType = -8 ;
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

bool WRTau_Core::IsNonPromptTau(const Tau tau, const std::vector<Gen>& gens){
  
  if(IsDATA) return false;
  if(GetTauType(tau,gens)<0) return true;
  else return false;

}

bool WRTau_Core::IsPromptTau(const Tau tau, const std::vector<Gen>& gens){
  
  if(IsDATA) return false;
  if(GetTauType(tau,gens)>0) return true;
  else return false;

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
  if (region == WRTau_Core::EXO_16_023_Preselection)            region_string = "EXO_16_023_Preselection";
  if (region == WRTau_Core::BaselinePreselection)               region_string = "BaselinePreselection";
  if (region == WRTau_Core::ResolvedPreselection)               region_string = "ResolvedPreselection";
  if (region == WRTau_Core::BoostedPreselection)                region_string = "BoostedPreselection";
  if (region == WRTau_Core::ResolvedLowMassControlRegion)       region_string = "ResolvedLowMassControlRegion";
  if (region == WRTau_Core::BoostedLowMassControlRegion)        region_string = "BoostedLowMassControlRegion";
  if (region == WRTau_Core::ResolvedLowMassControlRegionMass1)  region_string = "ResolvedLowMassControlRegionMass1";
  if (region == WRTau_Core::BoostedLowMassControlRegionMass1)   region_string = "BoostedLowMassControlRegionMass1";
  if (region == WRTau_Core::ResolvedSignalRegion)               region_string = "ResolvedSignalRegion";
  if (region == WRTau_Core::BoostedSignalRegion)                region_string = "BoostedSignalRegion";
  if (region == WRTau_Core::ResolvedSignalRegionMETInvert)      region_string = "ResolvedSignalRegionMETInvert";
  if (region == WRTau_Core::BoostedSignalRegionMETInvert)       region_string = "BoostedSignalRegionMETInvert";
  if (region == WRTau_Core::ResolvedSignalRegionMass1)          region_string = "ResolvedSignalRegionMass1";
  if (region == WRTau_Core::BoostedMassOptSel)                  region_string = "BoostedMassOptSel";
  if (region == WRTau_Core::ResolvedMassOptSel)                 region_string = "ResolvedMassOptSel";
  if (region == WRTau_Core::WJetsControlRegion)                 region_string = "WJetsControlRegion";
  if (region == WRTau_Core::QCDEnrichedControlRegionAK4)        region_string = "QCDEnrichedControlRegionAK4";
  if (region == WRTau_Core::QCDEnrichedControlRegionAK8)        region_string = "QCDEnrichedControlRegionAK8";
  if (region == WRTau_Core::FakeTTControlRegion)                region_string = "FakeTTControlRegion";
  if (region == WRTau_Core::FakeDYControlRegion)                region_string = "FakeDYControlRegion";
  if (region == WRTau_Core::BoostedSignalRegionLSFInvert)       region_string = "BoostedSignalRegionLSFInvert";
  if (region == WRTau_Core::BenchmarkResolvedPreselection)      region_string = "BenchmarkResolvedPreselection";
  if (region == WRTau_Core::BenchmarkBoostedPreselection)       region_string = "BenchmarkBoostedPreselection";
  if (region == WRTau_Core::BoostedSignalRegionMETInvertMTSame) region_string = "BoostedSignalRegionMETInvertMTSame";
  if (region == WRTau_Core::ResolvedSignalRegionMETInvertMTSame) region_string = "ResolvedSignalRegionMETInvertMTSame";
  if (region == WRTau_Core::TTEnrichedRegion)                    region_string = "TTEnrichedRegion";
  if (region == WRTau_Core::TTFakeMeasureRegion)                 region_string = "TTFakeMeasureRegion";
  
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
      Particle wr = taus.at(0) + *TightLeptons.at(0) + jets.at(0) + jets.at(1);
      if(!ignoreMET) mass = MT(wr,METv);
      else {
        wr = wr + METv;
        mass = wr.M();
      }
    }
  }  

  return mass;

}


double WRTau_Core::GetResolvedSRMass(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

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
      Particle wr = taus.at(0) + *TightLeptons.at(0) + jets.at(0) + jets.at(1);
      mass = wr.M();
    }
  }  

  return mass;

}


double WRTau_Core::GetResolvedST(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

  double st = -999;

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
      st = taus.at(0).Pt() + TightLeptons.at(0)->Pt() + jets.at(0).Pt() + jets.at(1).Pt();
    }
  }  

  return st;

}

double WRTau_Core::GetResolvedLT(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

  double lt = 0;

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
      for(const auto &l : TightLeptons){
        lt += l->Pt();
      }
    }
  }  

  return lt;

}

double WRTau_Core::GetResolvedHT(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

  double ht = 0;

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
      for(const auto &j : jets_ResolvedSR){
        if(j.Pt()>40) ht += j.Pt();
      }
    }
  }  

  return ht;

}

double WRTau_Core::GetResolvedSTwithMET(Particle METv, const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

  double st = -999;

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
      st = taus.at(0).Pt() + TightLeptons.at(0)->Pt() + jets.at(0).Pt() + jets.at(1).Pt() + METv.Pt();
    }
  }  

  return st;

}

double WRTau_Core::GetResolvedDeltaPhiLepTau(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

  double delta = 0;

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
      delta = taus.at(0).DeltaPhi(*TightLeptons.at(0));
    }
  }  

  return delta;

}

double WRTau_Core::GetResolvedDeltaPhiLepJets(const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<Lepton *> TightLeptons){

  double delta = 0;

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
      Particle jets_ = jets.at(0) + jets.at(1);
      delta = jets_.DeltaPhi(*TightLeptons.at(0));
    }
  }  

  return delta;

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
    if(leptons_BoostedSR.size()>0 ){
      Particle wr = taus.at(0) + AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0)) ;
      if(!ignoreMET) mass = MT(wr,METv);
      else {
        wr = wr + METv;
        mass = wr.M();}
    }
  }

  return mass;

}



double WRTau_Core::GetBoostedSRMass(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

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
    if(leptons_BoostedSR.size()>0 ){
      Particle wr = taus.at(0) + AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0)) ;
      mass = wr.M();
    }
  }

  return mass;

}


double WRTau_Core::GetBoostedST(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double st = -999;

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
    if(leptons_BoostedSR.size()>0 ){
      st = taus.at(0).Pt() + AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0)).Pt() ;
    }
  }

  return st;

}

double WRTau_Core::GetBoostedLT(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double lt = 0;

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
    if(leptons_BoostedSR.size()>0 ){
      for(const auto &lep : leptons_BoostedSR){
        lt += lep->Pt();
      }
    }
  }

  return lt;

}

double WRTau_Core::GetBoostedHT(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double ht = 0;

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
    if(leptons_BoostedSR.size()>0 ){
      for(const auto &J : fatjets_BoostedSR){
        if(J.Pt()>40) ht += J.Pt();
      }
    }
  }

  return ht;

}

double WRTau_Core::GetBoostedSTwithMET(Particle METv , const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double st = -999;

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
    if(leptons_BoostedSR.size()>0 ){
      st = taus.at(0).Pt() + AddFatJetAndLepton(fatjet_BoostedSR,*leptons_BoostedSR.at(0)).Pt() + METv.Pt();
    }
  }

  return st;

}



double WRTau_Core::GetBoostedDeltaPhiLepJets(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double delta = 0;

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
    if(leptons_BoostedSR.size()>0 ){
      delta = fatjet_BoostedSR.DeltaPhi(*leptons_BoostedSR.at(0));
    }
  }

  return delta;

}



double WRTau_Core::GetBoostedDeltaPhiLepTau(const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double delta = 0;

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
    if(leptons_BoostedSR.size()>0 ){
      delta = taus.at(0).DeltaPhi(*leptons_BoostedSR.at(0));
    }
  }

  return delta;

}


double WRTau_Core::GetBoostedSRMass_RecoNeutrino(Particle METv,const std::vector<Tau>& taus,const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons){

  double mass = -999;

  vector<FatJet> fatjets_BoostedSR;
  vector<Lepton *> leptons_BoostedSR;
  FatJet fatjet_BoostedSR;

  for(const auto &J : fatjets){
    if(J.DeltaPhi(taus.at(0))>2.0 ) fatjets_BoostedSR.push_back(J);
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
    if(J.DeltaPhi(taus.at(0))>2.0 ) fatjets_BoostedSR.push_back(J);
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
  bool _isBoostedMassOptSel(false);
  bool _isBoostedLowMassControlRegionMass1(false);
  bool _isResolvedSignalRegion(false);
  bool _isBoostedSignalRegion(false);
  bool _isBoostedSignalRegionLSFInvert(false);
  bool _isResolvedSignalRegionMETInvert(false);
  bool _isBoostedSignalRegionMETInvert(false);
  bool _isResolvedMassOptSel(false);
  bool _isResolvedSignalRegionMass1(false);
  bool _isBoostedSignalRegionMass1(false);
  bool _isWJetsControlRegion(false);
  bool _isEXO16023Baseline(false);
  bool _isEXO16023Preselection(false);
  bool _isBoostedPreselectionWithLSF(false);
  bool _isBenchmarkResolvedPreselection(false);
  bool _isBenchmarkBoostedPreselection(false);
  bool _isResolvedSignalRegionMETInvertMTSame(false);
  bool _isBoostedSignalRegionMETInvertMTSame(false);
  bool _isTTEnrichedRegion(false);
  bool _isTTFakeMeasureRegion(false);


  bool hasAtLeast2AK4Jets = jets.size()>1;
  bool hasAtLeast1AK8Jets = fatjets.size()>0;
  bool hasAtLeast1TightLeptons = TightLeptons.size()>0;
  bool hasAtLeast1LooseLeptons = LooseLeptons.size()>0;
  
  _isTTFakeMeasureRegion = taus.size() > 0 ;
  _isTTEnrichedRegion = jets.size()>1 && bjets.size()>0 ;

  _isBaselinePreselection = taus.size()>0 && taus.at(0).Pt()>TriggerSafeTauPtCut        && LooseLeptons.size()==1;
  _isResolvedPreselection = _isBaselinePreselection && hasAtLeast2AK4Jets && TightLeptons.size()==1;
  _isBoostedPreselection = _isBaselinePreselection && !_isResolvedPreselection && hasAtLeast1AK8Jets;

  _isEXO16023Baseline = taus.size()==1 && taus.at(0).Pt()>TriggerSafeTauPtCut && hasAtLeast2AK4Jets && TightLeptons.size()==1;

  if(_isEXO16023Baseline){
    if(taus.at(0).DeltaR(*TightLeptons.at(0))>0.5){
      vector<Jet> candJets;
      int candIdx = 0;
      for(auto const& j : jets){
        if(j.DeltaR(*TightLeptons.at(0))>0.5 && j.DeltaR(taus.at(0))>0.5 ){
          candJets.push_back(j);
          candIdx++;
        }
      }
      if(candIdx>1){
        int pairIdx = 0;
        for(int j1=0 ; j1 < candJets.size(); ++j1){
          for(int j2=j1+1 ; j2 < candJets.size(); ++j2){
            if(candJets.at(j1).DeltaR(candJets.at(j2))>0.5)  pairIdx++;
          }
        }
        if(pairIdx>0) _isEXO16023Preselection = true;
      }
    }
  }


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

        _isBenchmarkResolvedPreselection = true;

        double mwr1 = GetResolvedSRMass_RecoNeutrino(METv,taus,jets,TightLeptons);
        //double mwr  = GetResolvedSRMass(METv,taus,jets,TightLeptons,true);
        double mwr  = GetResolvedSRMass(taus,jets,TightLeptons);

        if(METv.Pt()>METCut) _isResolvedMassOptSel = true;
        else{
          _isResolvedSignalRegionMETInvert = true;
          if(mwr>MTCut){
            _isResolvedSignalRegionMETInvertMTSame = true;
            //cout << "_isResolvedSignalRegionMETInvertMTSame" << endl;
          }
        }
        
        if(mwr<MTCut) _isResolvedLowMassControlRegion = true ; // TODO : study mass cut optimization ; make a submethod to vary mass cuts and check significance in 2D
        else if(mwr>MTCut && METv.Pt()>METCut) _isResolvedSignalRegion = true;

        if(mwr1<MRecoCut) _isResolvedLowMassControlRegionMass1 = true;
        else if(mwr1>MRecoCut && METv.Pt()>METCut) _isResolvedSignalRegionMass1 = true;
      }
    }  
  }

  if(!_isResolvedSignalRegion && _isBoostedPreselection){

    vector<double> mll;
    vector<FatJet> fatjets_BoostedSR;
    vector<FatJet> fatjets_BoostedSR_LSFInvert;
    vector<Lepton *> leptons_BoostedSR;
    //FatJet fatjet_BoostedSR;
    Particle ll;

    for(const auto &looselep : LooseLeptons){
      ll = *looselep + taus.at(0);
      mll.push_back(ll.M());
    }
    for(const auto &J : fatjets){
      if(J.DeltaPhi(taus.at(0))>2.0 && J.LSF()>LSFOptCut){
        fatjets_BoostedSR.push_back(J);
        //cout << "[WRTau_Core::GetRegion] J.LSF() = " <<  J.LSF() << endl ;
      }
    }
    if(fatjets_BoostedSR.size()>0){
      fatjet_BoostedSR = fatjets_BoostedSR.at(0);
      fatjet_BoostedSR_v.push_back(fatjet_BoostedSR);
      for(const auto &looselep : LooseLeptons){
        if(fatjet_BoostedSR.DeltaR(*looselep)<0.8) leptons_BoostedSR.push_back(looselep);
      }
      if(leptons_BoostedSR.size()>0){
        
        if(fatjet_BoostedSR.LSF()>LSFOptCut){

          _isBenchmarkBoostedPreselection = true;

          //double mwr  = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true); 
          double mwr  = GetBoostedSRMass(taus,fatjets,LooseLeptons); 
          double mwr1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);

          //cout << " MET,mWR = " << METv.Pt() << " , " << mwr << endl;
          if(METv.Pt()>METCut) _isBoostedMassOptSel = true;
          else {
            _isBoostedSignalRegionMETInvert = true;
            if(mwr>MTCut){
              //cout << "_isBoostedSignalRegionMETInvertMTSame MET,mWR = " << METv.Pt() << " , " << mwr << endl;
              _isBoostedSignalRegionMETInvertMTSame = true;
            }
          }
          
          if(mwr<MTCut) _isBoostedLowMassControlRegion = true ;
          else if(mwr>MTCut && METv.Pt()>METCut) _isBoostedSignalRegion = true;

          if(mwr1<MRecoCut) _isBoostedLowMassControlRegionMass1 = true ;
          else if(mwr1>MRecoCut && METv.Pt()>METCut) _isBoostedSignalRegionMass1 = true;

        }
        else if(fatjet_BoostedSR.LSF()<=LSFOptCut) _isBoostedSignalRegionLSFInvert = true;
      }
    }
  }



  map<WRTau_Core::SearchRegion,bool> m_region = {
    {WRTau_Core::BenchmarkResolvedPreselection,_isBenchmarkResolvedPreselection},
    {WRTau_Core::BenchmarkBoostedPreselection,_isBenchmarkBoostedPreselection},
    {WRTau_Core::EXO_16_023_Preselection,_isEXO16023Preselection},
    {WRTau_Core::BaselinePreselection,_isBaselinePreselection},
    {WRTau_Core::ResolvedPreselection,_isResolvedPreselection},
    {WRTau_Core::BoostedPreselection,_isBoostedPreselection},
    {WRTau_Core::ResolvedLowMassControlRegion,_isResolvedLowMassControlRegion},
    {WRTau_Core::BoostedLowMassControlRegion,_isBoostedLowMassControlRegion},
    {WRTau_Core::ResolvedLowMassControlRegionMass1,_isResolvedLowMassControlRegionMass1},
    {WRTau_Core::BoostedLowMassControlRegionMass1,_isBoostedLowMassControlRegionMass1},
    {WRTau_Core::BoostedSignalRegionLSFInvert,_isBoostedSignalRegionLSFInvert},
    {WRTau_Core::ResolvedSignalRegion,_isResolvedSignalRegion},
    {WRTau_Core::ResolvedSignalRegionMETInvert,_isResolvedSignalRegionMETInvert},
    {WRTau_Core::BoostedSignalRegionMass1,_isBoostedSignalRegionMass1},
    {WRTau_Core::ResolvedSignalRegionMass1,_isResolvedSignalRegionMass1},
    {WRTau_Core::BoostedSignalRegion,_isBoostedSignalRegion},
    {WRTau_Core::BoostedSignalRegionMETInvert,_isBoostedSignalRegionMETInvert},
    {WRTau_Core::WJetsControlRegion,_isWJetsControlRegion},
    {WRTau_Core::BoostedMassOptSel,_isBoostedMassOptSel},
    {WRTau_Core::ResolvedMassOptSel,_isResolvedMassOptSel},
    {WRTau_Core::ResolvedSignalRegionMETInvertMTSame,_isResolvedSignalRegionMETInvertMTSame},
    {WRTau_Core::BoostedSignalRegionMETInvertMTSame,_isBoostedSignalRegionMETInvertMTSame},
    {WRTau_Core::TTEnrichedRegion,_isTTEnrichedRegion},
    {WRTau_Core::TTFakeMeasureRegion,_isTTFakeMeasureRegion},
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

      TString TauPromptString = "";
      TString LeptonPromptString = "";

      if(!isSignalSample()){
        if(taus.size()>0 && taus.at(0).Pt()>=TriggerSafeTauPtCut){
          if(IsPromptTau(taus.at(0),gens)) TauPromptString = "__PromptTau";
          else if(IsNonPromptTau(taus.at(0),gens)) TauPromptString = "__NonPromptTau";
          else return;
        }

        if(leptons.size()>0){
          if(IsPromptLepton(*leptons.at(0),gens)) LeptonPromptString = "__PromptLepton";
          else if(IsNonPromptLepton(*leptons.at(0),gens)) LeptonPromptString = "__NonPromptLepton";
          else return;
        }
      }

      WRTau_Core::Channel ch = GetChannel(leptons);
      TObjArray *tokens = fillpath.Tokenize("/");
      TString paramName = ((TObjString *) tokens -> At(0)) -> GetString();
      TString tauIDString = ((TObjString *) tokens -> At(1)) -> GetString();
      if(TauPromptString!="") TauPromptString = "/"+TauPromptString;
      TString label = paramName+TauPromptString+LeptonPromptString+"/"+GetRegionString(region.first); 
      TString label_channel = label + "_"+GetChannelString(ch);

      std::vector<TString> fillstr = {label,label_channel};
      
      double weight = GetMatchedWeight(taus,gens,leptons,idtuple,highpT) * MCweight;
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

        //cout << "[WRTau_Core::FillPassingRegions] Called in " << str << endl;
        // Problem regarding
        FillPreselHists(str,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,weight);
        FillMassHists(str,METv,taus,jets,fatjets,LooseLeptons,TightLeptons,weight);

        //fatjet_BoostedSR.Print();

        if(fatjet_BoostedSR.Pt()>0.) {
          FillHist(str+"/dRJtau_Boosted",taus.at(0).DeltaR(fatjet_BoostedSR),weight,60,0.,6.);
          FillHist(str+"/FatJet_Pt", fatjet_BoostedSR.Pt(), weight, 5000, 0., 5000.);
          FillHist(str+"/FatJet_Eta", fatjet_BoostedSR.Eta(), weight, 60, -3., 3.);
          FillHist(str+"/FatJet_Mass", fatjet_BoostedSR.M(), weight, 3000, 0., 3000.);
          FillHist(str+"/FatJet_SDMass", fatjet_BoostedSR.SDMass(), weight, 3000, 0., 3000.);
          FillHist(str+"/FatJet_LSF", fatjet_BoostedSR.LSF(), weight, 100, 0., 1.);
      
        }

        auto it_b = std::find(BoostedRegions.begin(), BoostedRegions.end(), region.first);
        auto it_r = std::find(ResolvedRegions.begin(), ResolvedRegions.end(), region.first);


        if(it_b != BoostedRegions.end() ){

          double M1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
          double M2 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);
          //double M3 = GetBoostedSRMass_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
          //double M4 = GetBoostedSRMassN_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
          if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
          if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
          //if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
          //if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

        }

        if(it_r != ResolvedRegions.end() ){
          
          double M1 = GetResolvedSRMass(METv,taus,jets,TightLeptons,false);
          double M2 = GetResolvedSRMass(METv,taus,jets,TightLeptons,true);
          //double M3 = GetResolvedSRMass_RecoNeutrino(METv,taus,jets,TightLeptons);
          //double M4 = GetResolvedSRMassN_RecoNeutrino(METv,taus,jets,TightLeptons);
          if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
          if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
          //if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
          //if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

        }
      }
      
    }
  }
  
  return;

}

void WRTau_Core::FillPassingRegions(map<pair<WRTau_Core::SearchRegion,double>, bool> m_Cut, TString cutvar , Particle METv,const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight, std::tuple<int,int,int> idtuple, bool highpT){

    for(auto const& p_cutregion : m_Cut){
      WRTau_Core::SearchRegion cutregion = p_cutregion.first.first;
      double cut = p_cutregion.first.second;

      //cout << "[WRTau_Core::FillPassingRegions] Start FillPassingRegion_Cut with " <<  cutregion  << endl;
      if(p_cutregion.second){

        std::pair<std::vector<Lepton *>,std::vector<Lepton *>> PairVecLeps = std::make_pair(LooseLeptons,TightLeptons);
        vector<Lepton *> leptons = ChooseLeptonColl(cutregion,PairVecLeps);

        WRTau_Core::Channel ch = GetChannel(leptons);
        
        TString TauPromptString = "";
        TString LeptonPromptString = "";

        if(!isSignalSample()){
          if(taus.size()>0 && taus.at(0).Pt()>=TriggerSafeTauPtCut){
            if(IsPromptTau(taus.at(0),gens)) TauPromptString = "__PromptTau";
            else if(IsNonPromptTau(taus.at(0),gens)) TauPromptString = "__NonPromptTau";
            else return;
          }
          //cout << "[WRTau_Core::FillPassingRegions] (TauType,isPromptTau,String) = (" << GetTauType(taus.at(0),gens) << "," <<  IsPromptTau(taus.at(0),gens) << " , " << TauPromptString << ")" << endl; 
          //cout << "[WRTau_Core::FillPassingRegions] (TauType,isNonPromptTau,String) = (" << GetTauType(taus.at(0),gens) << "," <<  IsNonPromptTau(taus.at(0),gens) << " , " << TauPromptString << ")" << endl; 

          if(leptons.size()>0){
            if(IsPromptLepton(*leptons.at(0),gens)) LeptonPromptString = "__PromptLepton";
            else if(IsNonPromptLepton(*leptons.at(0),gens)) LeptonPromptString = "__NonPromptLepton";
            else return;
          }
        }

        TObjArray *tokens = fillpath.Tokenize("/");
        TString paramName = ((TObjString *) tokens -> At(0)) -> GetString();
        TString tauIDString = ((TObjString *) tokens -> At(1)) -> GetString();

        TString cutval = std::to_string(cut);
        cutval.Remove(cutval.Length() - 4, 4);
        cutval.ReplaceAll(".", "p");

        TString label = paramName+TauPromptString+LeptonPromptString+"/"+tauIDString+"/"+GetRegionString(cutregion)+ "_"+ cutvar+ cutval;
        TString label_channel = label + "_"+GetChannelString(ch);

        std::vector<TString> fillstr = {label,label_channel};
        //cout << "[WRTau_Core::FillPassingRegions] Start GetMatchedWeight" << endl;
        double weight = GetMatchedWeight(taus,gens,leptons,idtuple,highpT) * MCweight;
        //cout << "[WRTau_Core::FillPassingRegions] End GetMatchedWeight" << endl;
        //cout << "[WRTau_Core::FillPassingRegions] Start GetTauIDLeptonFakeSF" << endl;
        weight *= GetTauIDLeptonFakeSF(idtuple,leptons,gens);
        //cout << "[WRTau_Core::FillPassingRegions] End GetTauIDLeptonFakeSF" << endl;
        if(HasFlag("unweighted")) weight = 1;

        for(const auto str : fillstr){

          //cout << "[WRTau_Core::FillPassingRegions] FillStr " << str << endl;
          //CopyHist(fillpath+"/Cutflow",str+"/Cutflow");
          FillHist(str+"/Nevents",0,weight,1,0.,1.);
          /*FillHist(str+"/MET",METv.Pt(),weight,2500,0.,2500.);

          if(leptons.size()>0){
            for(unsigned int i=0;i<leptons.size();i++){
              FillHist(str+"/dRl"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(*leptons.at(i)),weight,60,0.,6.);
            }
          }

          if(jets.size()>0){
            for(unsigned int i=0;i<jets.size();i++){
              FillHist(str+"/dRj"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(jets.at(i)),weight,60,0.,6.);
            }
          }

          if(fatjets.size()>0){
            for(unsigned int i=0;i<fatjets.size();i++){
              FillHist(str+"/dRJ"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(fatjets.at(i)),weight,60,0.,6.);
            }
          }*/
          //cout << "[WRTau_Core::FillPassingRegions] Start FillPreselHists" << endl;
          //FillPreselHists(str,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,weight);
          //cout << "[WRTau_Core::FillPassingRegions] End FillPreselHists" << endl;
          //cout << "[WRTau_Core::FillPassingRegions] Start FillMassHists" << endl;
          //FillMassHists(str,METv,taus,jets,fatjets,LooseLeptons,TightLeptons,weight);
          //cout << "[WRTau_Core::FillPassingRegions] End FillMassHists" << endl;

          //auto it_b = std::find(BoostedRegions.begin(), BoostedRegions.end(), cutregion);
          //auto it_r = std::find(ResolvedRegions.begin(), ResolvedRegions.end(), cutregion);

          //cout << "[WRTau_Core::FillPassingRegions] Get it_b , it_r" << endl;
          /*
          if( it_b != BoostedRegions.end() ){

            double M1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
            //cout << "[WRTau_Core::FillPassingRegions] Get Boosted M1" << endl;
            double M2 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);
            //cout << "[WRTau_Core::FillPassingRegions] Get Boosted M2" << endl;
            //double M3 = GetBoostedSRMass_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
            //double M4 = GetBoostedSRMassN_RecoNeutrino(METv,taus,fatjets,LooseLeptons);
            if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
            if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
            //if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
            //if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

          }

          if( it_r != ResolvedRegions.end() ){

            double M1 = GetResolvedSRMass(METv,taus,jets,TightLeptons,false);
            //cout << "[WRTau_Core::FillPassingRegions] Get Resolved M1" << endl;
            double M2 = GetResolvedSRMass(METv,taus,jets,TightLeptons,true);
            //cout << "[WRTau_Core::FillPassingRegions] Get Resolved M2" << endl;
            //double M3 = GetResolvedSRMass_RecoNeutrino(METv,taus,jets,TightLeptons);
            //double M4 = GetResolvedSRMassN_RecoNeutrino(METv,taus,jets,TightLeptons);
            if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,5000,0.,5000.);
            if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,5000,0.,5000.);
            //if(M3>0) FillHist(str+"/ProperMRecoNu",M3,weight,5000,0.,5000.);
            //if(M4>0) FillHist(str+"/ProperMRecoNu_N",M4,weight,5000,0.,5000.);

          }*/
        }
      }
    }
    
    return;

}

// THIS FILLPASSREGION
void WRTau_Core::FillPassingRegions(map<WRTau_Core::SearchRegion,bool> m_region,WRTau_Core::SearchRegion r,Particle METv, const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight,std::tuple<int,int,int> idtuple, bool highpT){
  
    if(m_region[r]==true){

      std::pair<std::vector<Lepton *>,std::vector<Lepton *>> PairVecLeps = std::make_pair(LooseLeptons,TightLeptons);
      vector<Lepton *> leptons = ChooseLeptonColl(r,PairVecLeps);

      TString TauPromptString = "";
      TString LeptonPromptString = "";

      if(!IsDATA){
        if(!isSignalSample()){
          if(taus.size()>0 && taus.at(0).Pt()>=TriggerSafeTauPtCut){
            if(IsPromptTau(taus.at(0),gens)) TauPromptString = "__PromptTau";
            else if(IsNonPromptTau(taus.at(0),gens) && HasFlag("RunApplicationRegion")) TauPromptString = "__NonPromptTau";
            else return;
          }
    
          if(leptons.size()>0){
            if(IsPromptLepton(*leptons.at(0),gens)) LeptonPromptString = "__PromptLepton";
            else                                    LeptonPromptString = "__NonPromptLepton";
          }
        }
      }

      if(HasFlag("PromptTau")){
        TauPromptString = "";
        LeptonPromptString = "";
      }
      if(HasFlag("RunApplicationRegion")) LeptonPromptString = "";

      WRTau_Core::Channel ch = GetChannel(leptons);
      if(TauPromptString!="") TauPromptString = "/"+TauPromptString;
      TString label = fillpath+TauPromptString+LeptonPromptString+"/"+GetRegionString(r); 
      TString label_channel = label + "_"+GetChannelString(ch);

      if(HasFlag("debugFill")) cout << "[WRTau_Core::FillPassingRegions] label = " << label << endl;
      if(HasFlag("debugFill")) cout << "[WRTau_Core::FillPassingRegions] label_channel = " << label_channel << endl;

      std::vector<TString> fillstr = {label_channel};
      
      if(HasFlag("RunApplicationRegion")){
        fillstr.clear();
        fillstr.push_back(label);
        TString nJets = "";
        if(isBoostedRegion(r)){
          if(jets.size()<2) nJets = TString::Itoa(jets.size(),10);
          else nJets = "2";
        }
        else if(isResolvedRegion(r)){
          if(jets.size()<4) nJets = TString::Itoa(jets.size(),10);
          else nJets = "4";
        }
        TString DM = TString::Itoa(taus.at(0).DecayMode(),10);
        fillstr.push_back(label+"_j"+nJets);
        fillstr.push_back(label+"_DM"+DM); 
      }

      double weight_var;
      double weight = GetMatchedWeight(taus,gens,leptons,idtuple,highpT) * MCweight;
      weight *= GetTauIDLeptonFakeSF(idtuple,leptons,gens);
      if(HasFlag("TauFake")){
        weight_var = weight * GetTauFRWeight(taus.at(0),leptons,gens,r,3);
        //weight *= GetTauFRWeight(taus.at(0),leptons,gens,r,2);
        weight *= GetTauFF(taus.at(0),r);
      }
      if(HasFlag("unweighted")) weight = 1;
      if(isBoostedRegion(r) && !IsDATA){
        weight *= LSFSF(ch,Syst_LSFSF);
      }

      //if(IsDATA) weight = 1.0;

      for(const auto str : fillstr){

        if(HasFlag("TauFake")){
          //FillHist(str+"/TauFakeWeight",GetTauFRWeight(taus.at(0),leptons,gens,r,2),1.,5000,0.,50.);
          //FillHist(str+"/TauFakeRate",GetTauFRfromBins(taus.at(0),leptons,r),1.,100,0.,1.);
          FillHist(str+"/TauFakeWeight",GetTauFF(taus.at(0),r),1.,1000,0.,1.);
        }
        //CopyHist(fillpath+"/Cutflow",str+"/Cutflow");
        FillHist(str+"/Nevents",0,weight,1,0.,1.);
        FillHist(str+"/MET",METv.Pt(),weight,2500,0.,2500.);
        FillHist(str+"/dRltau",taus.at(0).DeltaR(*leptons.at(0)),weight,60,0.,6.);

        //fatjet_BoostedSR.Print();

        if(fatjet_BoostedSR.Pt()>0.) {
          FillHist(str+"/FatJet/dRJtau",taus.at(0).DeltaR(fatjet_BoostedSR),weight,60,0.,6.);
          FillHist(str+"/FatJet/Pt", fatjet_BoostedSR.Pt(), weight, 5000, 0., 5000.);
          FillHist(str+"/FatJet/Eta", fatjet_BoostedSR.Eta(), weight, 60, -3., 3.);
          FillHist(str+"/FatJet/Mass", fatjet_BoostedSR.M(), weight, 3000, 0., 3000.);
          FillHist(str+"/FatJet/SDMass", fatjet_BoostedSR.SDMass(), weight, 3000, 0., 3000.);
          FillHist(str+"/FatJet/LSF", fatjet_BoostedSR.LSF(), weight, 100, 0., 1.);
        }

        
        FillPreselHists(str,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,weight);
        FillMassHists(str,METv,taus,jets,fatjets,LooseLeptons,TightLeptons,weight);

        auto it_b = std::find(BoostedRegions.begin(), BoostedRegions.end(), r);
        auto it_r = std::find(ResolvedRegions.begin(), ResolvedRegions.end(), r);

        if( it_b != BoostedRegions.end() ){

          if(HasFlag("debugFill")) cout << str << endl;
          FillLeptonPlots(leptons,str+"/HighPtLoose",weight);
          FillHist(str+"/dRJtau",taus.at(0).DeltaR(fatjets.at(0)),weight,60,0.,6.);
          double M1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
          double M2 = GetBoostedSRMass(taus,fatjets,LooseLeptons);
          double ST = GetBoostedST(taus,fatjets,LooseLeptons);
          if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,20000,0.,20000.);
          if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,20000,0.,20000.);
          if(ST>0) FillHist(str+"/BoostedST",M2,weight,20000,0.,20000.);
          if( GetRegionString(r) == "BenchmarkBoostedPreselection" ){
            if(HasFlag("debugFill")) cout << "fill BenchmarkBoostedPreselection" << endl;
            //FillHist(str+"/MET_MtWR",METv.Pt(),M1,weight,2500,0.,2500.,5000,0.,5000);
            //FillHist(str+"/MET_MeffWR",METv.Pt(),M2,weight,2500,0.,2500.,10000,0.,10000);
            FillHist(str+"/MET_ST",METv.Pt(),ST,weight,2500,0.,2500.,10000,0.,10000);
            FillHist(str+"/MET_METoverST",METv.Pt(),METv.Pt()/ST,weight,2500,0.,2500.,10000,0.,10000);
            FillHist(str+"/MET_MET2overST",METv.Pt(),METv.Pt()*METv.Pt()/ST,weight,2500,0.,2500.,10000,0.,10000);
          }

        }

        if( it_r != ResolvedRegions.end() ){
          if(HasFlag("debugFill")) cout << str << endl;
          FillLeptonPlots(leptons,str+"/HighPtTight",weight);
          FillHist(str+"/dRj0tau",taus.at(0).DeltaR(jets.at(0)),weight,60,0.,6.);
          FillHist(str+"/dRj1tau",taus.at(0).DeltaR(jets.at(1)),weight,60,0.,6.);
          double M1 = GetResolvedSRMass(METv,taus,jets,TightLeptons,false);
          double M2 = GetResolvedSRMass(taus,jets,TightLeptons);
          double ST = GetResolvedST(taus,jets,TightLeptons);
          if(M1>0) FillHist(str+"/ProperMTWR",M1,weight,20000,0.,20000.);
          if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight,20000,0.,20000.);
          if(ST>0) FillHist(str+"/BoostedST",M2,weight,20000,0.,20000.);
          if( GetRegionString(r) == "BenchmarkResolvedPreselection" ){
            if(HasFlag("debugFill")) cout << "fill BenchmarkResolvedPreselection" << endl;
            //FillHist(str+"/MET_MtWR",METv.Pt(),M1,weight,2500,0.,2500.,5000,0.,5000);
            //FillHist(str+"/MET_MeffWR",METv.Pt(),M2,weight,2500,0.,2500.,10000,0.,10000);
            FillHist(str+"/MET_ST",METv.Pt(),ST,weight,2500,0.,2500.,10000,0.,10000);
            FillHist(str+"/MET_METoverST",METv.Pt(),METv.Pt()/ST,weight,2500,0.,2500.,10000,0.,10000);
            FillHist(str+"/MET_MET2overST",METv.Pt(),METv.Pt()*METv.Pt()/ST,weight,2500,0.,2500.,10000,0.,10000);
          }

        }
      }

      /*if(HasFlag("TauFake")){
        TString label_var = fillpath+TauPromptString+LeptonPromptString+"_TauFRWtVarDeg3/"+GetRegionString(r); 
        TString label_var_channel = label_var + "_"+GetChannelString(ch);

        std::vector<TString> fillstr = {label_var,label_var_channel};
        
        for(const auto str : fillstr){

          //CopyHist(fillpath+"/Cutflow",str+"/Cutflow");
          FillHist(str+"/Nevents",0,weight_var,1,0.,1.);
          FillHist(str+"/MET",METv.Pt(),weight_var,2500,0.,2500.);

          for(unsigned int i=0;i<leptons.size();i++){
            FillHist(str+"/dRl"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(*leptons.at(i)),weight_var,60,0.,6.);
          }

          for(unsigned int i=0;i<jets.size();i++){
            FillHist(str+"/dRj"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(jets.at(i)),weight_var,60,0.,6.);
          }

          for(unsigned int i=0;i<fatjets.size();i++){
            FillHist(str+"/dRJ"+TString::Itoa(i,10)+"tau",taus.at(0).DeltaR(fatjets.at(i)),weight_var,60,0.,6.);
          }

          //fatjet_BoostedSR.Print();

          if(fatjet_BoostedSR.Pt()>0.) {
            FillHist(str+"/FatJet/dRJtau",taus.at(0).DeltaR(fatjet_BoostedSR),weight_var,60,0.,6.);
            FillHist(str+"/FatJet/Pt", fatjet_BoostedSR.Pt(), weight_var, 5000, 0., 5000.);
            FillHist(str+"/FatJet/Eta", fatjet_BoostedSR.Eta(), weight_var, 60, -3., 3.);
            FillHist(str+"/FatJet/Mass", fatjet_BoostedSR.M(), weight_var, 3000, 0., 3000.);
            FillHist(str+"/FatJet/SDMass", fatjet_BoostedSR.SDMass(), weight_var, 3000, 0., 3000.);
            FillHist(str+"/FatJet/LSF", fatjet_BoostedSR.LSF(), weight_var, 100, 0., 1.);
          }

          FillPreselHists(str,taus,jets,bjets,fatjets,LooseLeptons,TightLeptons,weight_var);
          FillMassHists(str,METv,taus,jets,fatjets,LooseLeptons,TightLeptons,weight_var);

          auto it_b = std::find(BoostedRegions.begin(), BoostedRegions.end(), r);
          auto it_r = std::find(ResolvedRegions.begin(), ResolvedRegions.end(), r);


          if( it_b != BoostedRegions.end() ){

            double M1 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,false);
            double M2 = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,true);
            if(M1>0) FillHist(str+"/ProperMTWR",M1,weight_var,20000,0.,20000.);
            if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight_var,20000,0.,20000.);

          }

          if( it_r != ResolvedRegions.end() ){

            double M1 = GetResolvedSRMass(METv,taus,jets,TightLeptons,false);
            double M2 = GetResolvedSRMass(METv,taus,jets,TightLeptons,true);
            if(M1>0) FillHist(str+"/ProperMTWR",M1,weight_var,20000,0.,20000.);
            if(M2>0) FillHist(str+"/ProperMeffWR",M2,weight_var,20000,0.,20000.);

          }
        }
      }*/
      
    }
  
  return;

}

void WRTau_Core::FillPassingRegions_2DScan(map<WRTau_Core::SearchRegion,bool> m_region,WRTau_Core::SearchRegion r,Particle METv, const std::vector<Gen>& gens,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                        const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,
                        TString fillpath, double MCweight,std::tuple<int,int,int> idtuple, bool highpT){
  
    if(m_region[r]==true){

      std::pair<std::vector<Lepton *>,std::vector<Lepton *>> PairVecLeps = std::make_pair(LooseLeptons,TightLeptons);
      vector<Lepton *> leptons = ChooseLeptonColl(r,PairVecLeps);

      TString TauPromptString = "";
      TString LeptonPromptString = "";

      if(!IsDATA){
        if(!isSignalSample()){
          if(taus.size()>0 && taus.at(0).Pt()>=TriggerSafeTauPtCut){
            if(IsPromptTau(taus.at(0),gens)) TauPromptString = "__PromptTau";
            else return;
          }
    
          if(leptons.size()>0){
            if(IsPromptLepton(*leptons.at(0),gens)) LeptonPromptString = "__PromptLepton";
            else if(IsNonPromptLepton(*leptons.at(0),gens)) LeptonPromptString = "__NonPromptLepton";
            else return;
          }
        }
      }

      WRTau_Core::Channel ch = GetChannel(leptons);
      if(TauPromptString!="") TauPromptString = "/"+TauPromptString;
      TString label = fillpath+TauPromptString+LeptonPromptString+"/"+GetRegionString(r); 
      TString label_channel = label + "_"+GetChannelString(ch);

      if(HasFlag("debugFill")) cout << "[WRTau_Core::FillPassingRegions] label = " << label << endl;
      if(HasFlag("debugFill")) cout << "[WRTau_Core::FillPassingRegions] label_channel = " << label_channel << endl;

      std::vector<TString> fillstr = {label_channel};
      
      double weight_var;
      double weight = GetMatchedWeight(taus,gens,leptons,idtuple,highpT) * MCweight;
      weight *= GetTauIDLeptonFakeSF(idtuple,leptons,gens);
      if(HasFlag("TauFake")){
        weight_var = weight * GetTauFRWeight(taus.at(0),leptons,gens,r,3);
        weight *= GetTauFRWeight(taus.at(0),leptons,gens,r,2);
      }
      if(HasFlag("unweighted")) weight = 1;
      if(isBoostedRegion(r)){
        weight *= LSFSF(ch,Syst_LSFSF);
      }

      for(const auto str : fillstr){


        auto it_b = std::find(BoostedRegions.begin(), BoostedRegions.end(), r);
        auto it_r = std::find(ResolvedRegions.begin(), ResolvedRegions.end(), r);

        double STwithMET = 0.;
        double LT = 0.;
        double HT = 0.;
        double DeltaPhiLepTau = 0.;
        double DeltaPhiLepJets = 0.;


        if( it_b != BoostedRegions.end() ){

          STwithMET = GetBoostedSTwithMET(METv,taus,fatjets,LooseLeptons);
          LT = GetBoostedLT(taus,fatjets,LooseLeptons);
          HT = GetBoostedHT(taus,fatjets,LooseLeptons);
          DeltaPhiLepTau = GetBoostedDeltaPhiLepTau(taus,fatjets,LooseLeptons);
          DeltaPhiLepJets = GetBoostedDeltaPhiLepJets(taus,fatjets,LooseLeptons);

        }

        if( it_r != ResolvedRegions.end() ){

          STwithMET = GetResolvedSTwithMET(METv,taus,jets,TightLeptons);
          LT = GetResolvedLT(taus,jets,TightLeptons);
          HT = GetResolvedHT(taus,jets,TightLeptons);
          DeltaPhiLepTau =  GetResolvedDeltaPhiLepTau(taus,jets,TightLeptons);
          DeltaPhiLepJets = GetResolvedDeltaPhiLepJets(taus,jets,TightLeptons);
          
        }

        FillHist(str+"/MET_ST",METv.Pt(),STwithMET,weight,2500,0.,2500.,10000,0.,10000);
        FillHist(str+"/MET_LT",METv.Pt(),LT,weight,2500,0.,2500.,10000,0.,10000);
        FillHist(str+"/MET_HT",METv.Pt(),HT,weight,2500,0.,2500.,10000,0.,10000);
        FillHist(str+"/MET_dPhiLepTau",METv.Pt(),DeltaPhiLepTau,weight,2500,0.,2500.,6500,0.,6.5);
        FillHist(str+"/MET_dPhiLepjets",METv.Pt(),DeltaPhiLepJets,weight,2500,0.,2500.,6500,0.,6.5);

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

map<pair<WRTau_Core::SearchRegion,double>, bool> WRTau_Core::MassCutter(map<WRTau_Core::SearchRegion,bool> m_region,vector<double> MassCuts,
                                                              Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,
                                                              const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons,bool ignoreMET){


  //cout << "[WRTauCore::MassCutter] Start MassCutter" << endl;
  map<pair<WRTau_Core::SearchRegion,double>, bool> cutmap;
  cutmap.insert(std::make_pair(std::make_pair(WRTau_Core::None,-999.),false));
  int i(0); double mass(0.);
  for(const auto r : MassOptRegions){
    if(m_region[r]){
        //cout << "[WRTauCore::MassCutter] Cutting Region " << r << endl;
        if(r == WRTau_Core::BoostedMassOptSel)         mass = GetBoostedSRMass(METv,taus,fatjets,LooseLeptons,ignoreMET); 
        else if(r == WRTau_Core::ResolvedMassOptSel)   mass = GetResolvedSRMass(METv,taus,jets,TightLeptons,ignoreMET);
        for(const auto cut : MassCuts){
          cutmap.insert(std::make_pair(std::make_pair(r,cut),mass>cut));
        }
      i++;
    }
  }
  if(i!=0) cutmap.erase(std::make_pair(WRTau_Core::None,-999.));
  //cout << "[WRTauCore::MassCutter] End MassCutter with cutmap size " << cutmap.size() << endl;
  return cutmap;
}

map<pair<WRTau_Core::SearchRegion,double>, bool> WRTau_Core::MassCutter(map<WRTau_Core::SearchRegion,bool> m_region,vector<double> MassCuts,
                                                              Particle METv,const std::vector<Tau>& taus,const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets,
                                                              const std::vector<Lepton *> LooseLeptons, const std::vector<Lepton *> TightLeptons){


  //cout << "[WRTauCore::MassCutter] Start MassCutter" << endl;
  map<pair<WRTau_Core::SearchRegion,double>, bool> cutmap;
  cutmap.insert(std::make_pair(std::make_pair(WRTau_Core::None,-999.),false));
  int i(0); double mass(0.);
  for(const auto r : MassOptRegions){
    if(m_region[r]){
        //cout << "[WRTauCore::MassCutter] Cutting Region " << r << endl;
        if(r == WRTau_Core::BoostedMassOptSel)         mass = GetBoostedSRMass_RecoNeutrino(METv,taus,fatjets,LooseLeptons); 
        else if(r == WRTau_Core::ResolvedMassOptSel)   mass = GetResolvedSRMass_RecoNeutrino(METv,taus,jets,TightLeptons);
        for(const auto cut : MassCuts){
          cutmap.insert(std::make_pair(std::make_pair(r,cut),mass>cut));
        }
      i++;
    }
  }
  if(i!=0) cutmap.erase(std::make_pair(WRTau_Core::None,-999.));
  //cout << "[WRTauCore::MassCutter] End MassCutter with cutmap size " << cutmap.size() << endl;
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

  if(leps.size()>0 && IsNonPromptLepton(*leps.at(0),gens)){
    
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

double WRTau_Core::GetMatchedWeight(const std::vector<Tau>& taus,const std::vector<Gen>& gens,const std::vector<Lepton *> leps,std::tuple<int,int,int> idtuple,bool highpT){

  if(IsDATA) return 1.0;
  
  // weight = w_tau * w_lep 
  else{

    double w_tau(1.0), w_lepton(1.0);


    if(taus.size()>0){
      if(IsPromptTau(taus.at(0),gens)){
        if(highpT) w_tau = tauidsftool_map[idtuple]->getHighPTSFvsPT(taus.at(0).Pt(),Syst_TauIDSF);
        else w_tau = tauidsftool_map[idtuple]->getSFvsPT(taus.at(0).Pt(),Syst_TauIDSF);
      }
    }
    if(leps.size()>0){
      if(IsPromptLepton(*leps.at(0),gens)){

        if(GetChannel(leps) == WRTau_Core::TauE){
          w_lepton *= mcCorr->ElectronID_SF("HEEP",leps.at(0)->Eta(),leps.at(0)->Pt(),Syst_ElectronIDSF);
          w_lepton *= mcCorr->ElectronReco_SF(leps.at(0)->Eta(),leps.at(0)->Pt(),Syst_ElectronRecoSF);
        }

        // TODO : MuonRECO SF derivation

        else if(GetChannel(leps) == WRTau_Core::TauMu){
          w_lepton *= mcCorr->MuonID_SF("NUM_HighPtID_DEN_TrackerMuons",leps.at(0)->Eta(),leps.at(0)->Pt(),Syst_MuonIDSF);
          w_lepton *= mcCorr->MuonISO_SF("NUM_LooseRelTkIso_DEN_HighPtIDandIPCut",leps.at(0)->Eta(),leps.at(0)->Pt(),Syst_MuonISOSF);
        }
      }
      /*else if(IsNonPromptLepton(leps.at(0),gens)){ 
        if(GetLeptonType(leps.at(0),gens)==3){
          if(leps.at(0).IsElectron()) w_lepton *= tauidsftool_vEl_map[idtuple]->getSFvsEta(leps.at(0).Eta(),3);
          if(leps.at(0).IsMuon()) w_lepton *= tauidsftool_vMu_map[idtuple]->getSFvsEta(leps.at(0).Eta(),5);
        }
      }*/
    }

    return w_tau * w_lepton;

  }
  
}

double WRTau_Core::GetMatchedWeight(const std::vector<Tau>& taus,const std::vector<Gen>& gens,std::tuple<int,int,int> idtuple,bool highpT){

  if(IsDATA) return 1.0;
  
  // weight = w_tau * w_lep 
  else{

    double w_tau(1.0);


    if(taus.size()>0){
      if(IsPromptTau(taus.at(0),gens) && (std::get<0>(idtuple) != 0)){
        if(highpT) w_tau = tauidsftool_map[idtuple]->getHighPTSFvsPT(taus.at(0).Pt());
        else w_tau = tauidsftool_map[idtuple]->getSFvsPT(taus.at(0).Pt());
      }
    }

    return w_tau ;

  }
  
}

bool WRTau_Core::isBoostedRegion(WRTau_Core::SearchRegion region){

  std::string str = GetRegionString(region);  
  if (str.find("Boosted") != std::string::npos) return true; 
  else return false;

}

bool WRTau_Core::isResolvedRegion(WRTau_Core::SearchRegion region){

  std::string str = GetRegionString(region);  
  if (str.find("Resolved") != std::string::npos) return true; 
  else return false;

}



double WRTau_Core::GetTauFR(const Tau tau, WRTau_Core::SearchRegion region){

  double x = 0.; double p[4] ;

  bool isResolved(false), isBoosted(false);
  isResolved = isResolvedRegion(region);
  isBoosted  = isBoostedRegion(region);

  if(tau.Pt() > 1000.) x = 999.;
  else x = tau.Pt() ;

  if(DataEra=="2017" && isResolved){
    p[0] = -0.05132294652031895; p[1]= 0.0021713765630938077; p[2] = -4.078799237979005e-06 ; p[3] = 0.16864743043836866;
  }
  if(DataEra=="2017" && isBoosted){
     p[0] = -0.19161378669718473; p[1]= 0.0027668856251805594; p[2] = -4.613292834793495e-06 ; p[3] = 0.16864743043836866;
  } 
  if(DataEra=="2018" && isResolved){
    p[0] = -0.04584515677433494; p[1]= 0.0020963301990769858; p[2] = -3.7003681819559146e-06; p[3] = 0.053110591379367605;
  } 
  if(DataEra=="2018" && isBoosted){
     p[0] = -0.08407703581903803; p[1]= 0.0020744293484913153; p[2] = -3.2683183767448108e-06; p[3] = 0.15779344723593997;
  } 

  double a = p[0] + p[1] * x + p[2] * x * x ;
  return std::max(a,p[3]);

}

double WRTau_Core::GetTauFR_SingleFit(const Tau tau, const std::vector<Lepton *> leps, WRTau_Core::SearchRegion region){

  double x = tau.Pt();
  double value = 0.;

  if(tau.Pt() > 1000.) x = 999.;

  std::string rg = "";
  TString ch = GetChannelString(GetChannel(leps));

  if(isBoostedRegion(region)) rg = "BoostedSignalRegionMETInvert";
  else if(isResolvedRegion(region)) rg = "ResolvedSignalRegionMETInvert";

  TString YearString;
  YearString.Form("%d",GetYear()); 

  std::map< TString, TF1* >::const_iterator mapit;
  mapit = fakeEst->map_TF1_tau.find(rg+"_"+ch+"_"+YearString);

  if(mapit == fakeEst->map_TF1_tau.end()){
    cout << "[WRTauCore::GetTauFR_SingleFit] TF1 No "  << rg+"_"+ch+"_"+YearString << endl;
    exit(ENODATA);
  }

  /*for (const auto& pair : fakeEst->map_FitErr_tau) {
    std::cout << pair.first << ": " << pair.second << " , " << pair.second->GetNbinsX() << " , " << pair.second.GetBinContent(1) << std::endl;
  }*/

  std::map< TString, TH1D* >::const_iterator mapit_h;
  mapit_h = fakeEst->map_FitErr_tau.find(rg+"_"+ch+"_"+YearString);

  if(mapit_h == fakeEst->map_FitErr_tau.end()){
    cout << "[WRTauCore::GetTauFR_SingleFit] TH1D No "  << rg+"_"+ch+"_"+YearString << endl;
    exit(ENODATA);
  }
  //cout << "[WRTauCore::GetTauFR_SingleFit] "  << (mapit_h->second)->Get << endl;

  TH1D* h_err = (TH1D*) (mapit_h->second)->Clone();

  //cout << "[WRTauCore::GetTauFR_SingleFit] "  

  value = (mapit->second)->Eval(x);
  //int this_bin = 0;
  int this_bin = int(x)-189;
  this_bin = h_err->FindBin(x);

  //cout << x << " , " << this_bin << " , " << value << endl;
  double err = 0.;
  double err_raw = h_err->GetBinError(this_bin);
  //cout << x << " , " << this_bin << " , " << value << " + " << TauFRErr << "*" << err_raw << endl;
  err = std::max(0.0, h_err->GetBinError(this_bin));
  //cout << x << " , " << this_bin << " , " << value << " + " << TauFRErr << "*" << err << endl;

  return value + TauFRErr * err ;

}

double WRTau_Core::GetTauFF_QCD(const Tau tau){

  // QCD FF : (2016,2017,2018) * (DM0+1,DM10+11)
  double x = tau.Pt();
  int dm = tau.DecayMode();
  TString DMTag = "";

  if(dm == 0 || dm == 1) DMTag = "DM1prong";
  else if(dm == 10 || dm == 11) DMTag = "DM3prong";
  else{
    cout << "[WRTau_Core::GetTauFF_QCD] DecayMode = " << dm << " unavailable" << endl;
    exit(ENODATA);
  }

  TString YearString;
  YearString.Form("%d",GetYear()); 
  //cout << "debug 0" << endl;
  std::map< TString, TH1D* >::const_iterator mapit;
  mapit = fakeEst->map_TauFF_QCD.find(DMTag);
  //cout << "debug 1" << endl;
  if(mapit == fakeEst->map_TauFF_QCD.end()){
    cout << "[WRTauCore::GetTauFF_QCD] No "  << DMTag << endl;
    exit(ENODATA);
  }

  //cout << "debug 2" << endl;
  TH1D* h = (TH1D*) (mapit->second)->Clone();
  //cout << "debug 3" << endl;
  int nBins = h->GetNbinsX();
  double lastBinEdge = h->GetBinLowEdge(nBins + 1);
  if(x > lastBinEdge) x = lastBinEdge-0.001;

  //cout << nBins << " , " << lastBinEdge << " , " ;

  int this_bin = 0 ;
  this_bin = h->FindBin(x);

  double err = 0.;
  double val = 0.;
  val = h->GetBinContent(this_bin);
  err = h->GetBinError(this_bin);  

  //cout << val + TauFRErr * err << endl;
  return val + TauFRErr * err ;
}


double WRTau_Core::GetTauFF_TT(const Tau tau){

  // QCD FF : (2016,2017,2018) * (DM0,DM1,DM10,DM11)
  double x = tau.Pt();
  int dm = tau.DecayMode();
  TString DMTag = "";

  DMTag = "DM"+TString::Itoa(dm,10);

  TString YearString;
  YearString.Form("%d",GetYear()); 

  std::map< TString, TH1D* >::const_iterator mapit;
  mapit = fakeEst->map_TauFF_TT.find(DMTag);

  if(mapit == fakeEst->map_TauFF_TT.end()){
    cout << "[WRTauCore::GetTauFF_TT] No "  << DMTag << endl;
    exit(ENODATA);
  }

  TH1D* h = (TH1D*) (mapit->second)->Clone();
  int nBins = h->GetNbinsX();
  double lastBinEdge = h->GetBinLowEdge(nBins + 1);
  if(x > lastBinEdge) x = lastBinEdge-0.001;

  int this_bin = 0 ;
  this_bin = h->FindBin(x);

  double err = 0.;
  double val = 0.;
  val = h->GetBinContent(this_bin);
  err = h->GetBinError(this_bin);  

  return val + TauFRErr * err ;
}

double WRTau_Core::GetTauFF(const Tau tau, WRTau_Core::SearchRegion region){

  // Ratio : (2016a,2016b,2017,2018) * (DM0,DM1,DM10,DM11) * AR(SR,LMCR,QCDMR)
  double x = tau.Pt();
  int dm = tau.DecayMode();
  
  std::string str_region = GetRegionString(region);
  TString str_search = "";

  std::string substrings_to_remove[] = {"Resolved", "Boosted"};

  for (const auto& substr : substrings_to_remove) {
      std::size_t start_pos = 0;
      while ((start_pos = str_region.find(substr, start_pos)) != std::string::npos) {
          str_region.replace(start_pos, substr.length(), "");
      }
  }

  TString DMTag = "";
  DMTag = "DM"+TString::Itoa(dm,10);
  
  double FF_QCD = GetTauFF_QCD(tau);
  double FF_TT  = GetTauFF_TT(tau);

  std::map< TString, TH1D* >::const_iterator mapit_QCD;
  std::map< TString, TH1D* >::const_iterator mapit_TT;
  mapit_QCD = fakeEst->map_TauFF_Ratio.find(DMTag+"_"+str_region+"_QCD");
  mapit_TT  = fakeEst->map_TauFF_Ratio.find(DMTag+"_"+str_region+"_TT");

  if(mapit_QCD == fakeEst->map_TauFF_Ratio.end()){
    cout << "[WRTauCore::GetTauFF] No map_TauFF_Ratio for "  << DMTag+"_"+str_region+"_QCD" << endl;
    exit(ENODATA);
  }

  if(mapit_TT == fakeEst->map_TauFF_Ratio.end()){
    cout << "[WRTauCore::GetTauFF] No map_TauFF_Ratio for "  << DMTag+"_"+str_region+"_TT" << endl;
    exit(ENODATA);
  }

  TH1D* h_QCD = (TH1D*) (mapit_QCD->second)->Clone();
  int nBins_QCD = h_QCD->GetNbinsX();
  double pt_QCD = x;
  double lastBinEdge_QCD = h_QCD->GetBinLowEdge(nBins_QCD + 1);
  if(pt_QCD > lastBinEdge_QCD) pt_QCD = lastBinEdge_QCD-0.001;

  int this_bin_QCD = 0 ;
  this_bin_QCD = h_QCD->FindBin(pt_QCD);
  double ratio_QCD = h_QCD->GetBinContent(this_bin_QCD);

  TH1D* h_TT = (TH1D*) (mapit_TT->second)->Clone();
  int nBins_TT = h_TT->GetNbinsX();
  double pt_TT = x;
  double lastBinEdge_TT = h_TT->GetBinLowEdge(nBins_TT + 1);
  if(pt_TT > lastBinEdge_TT) pt_TT = lastBinEdge_TT-0.001;

  int this_bin_TT = 0 ;
  this_bin_TT = h_TT->FindBin(pt_TT);
  double ratio_TT = h_TT->GetBinContent(this_bin_TT);

  return ratio_QCD * FF_QCD + ratio_TT * FF_TT;

}

double WRTau_Core::GetTauFRfromBins(const Tau tau, const std::vector<Lepton *> leps, WRTau_Core::SearchRegion region){

  double x = tau.Pt();
  //double value = 0.;

  if(tau.Pt() > 1000.) x = 999.;

  std::string rg = "";
  TString ch = GetChannelString(GetChannel(leps));

  if(isBoostedRegion(region)) rg = "BoostedSignalRegionMETInvertMTSame";
  else if(isResolvedRegion(region)) rg = "ResolvedSignalRegionMETInvertMTSame";

  TString YearString;
  YearString.Form("%d",GetYear()); 

  std::map< TString, TH1D* >::const_iterator mapit;
  mapit = fakeEst->map_hist_tau.find(rg+"_"+ch);

  if(mapit == fakeEst->map_hist_tau.end()){
    cout << "[WRTauCore::GetTauFRfromBins] No "  << rg+"_"+ch << endl;
    exit(ENODATA);
  }

  TH1D* h = (TH1D*) (mapit->second)->Clone();

  int this_bin = 0 ;
  this_bin = h->FindBin(x);

  double err = 0.;
  double val = 0.;
  val = h->GetBinContent(this_bin);
  err = h->GetBinError(this_bin);  

  if(HasFlag("TauFRDebug")) cout << val << " + " << TauFRErr << " * " << err << endl;
  return val + TauFRErr * err ;

}

double WRTau_Core::GetTauFR(const Tau tau, const std::vector<Lepton *> leps, WRTau_Core::SearchRegion region, const int deg){

  double x = 0.; double x0 = 0.;
  double p[deg]; double tail(0.); double head(0.);
  WRTau_Core::Channel ch = GetChannel(leps);

  bool isResolved(false), isBoosted(false);
  isResolved = isResolvedRegion(region);
  isBoosted  = isBoostedRegion(region);

  if(tau.Pt() > 1000.) x = 999.;
  else x = tau.Pt() ;

  if(deg == 2){
    if(DataYear==2016 && isResolved){
      if(ch == WRTau_Core::TauE)       { p[0] = -0.831243237214809 ; p[1] = 0.008446894180263877 ; p[2] = -1.535807242317035e-05 ;tail = 0.1725287944801986 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = -0.15653336452885183 ; p[1] = 0.0021700790382151807 ; p[2] = -3.4107918586764663e-06 ;tail = 0.11446997172410935 ;}
    }
    if(DataYear==2016 && isBoosted){
      if(ch == WRTau_Core::TauE)       { p[0] = -0.14783201770423549 ; p[1] = 0.002291474864145053 ; p[2] = -3.5336099585763808e-06 ;tail = 0.21210511790473885 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = -0.30547419484119687 ; p[1] = 0.0034276899219565426 ; p[2] = -6.17947567461921e-06 ;tail = 0.0959035744085898 ;}
    } 
    if(DataEra=="2017" && isResolved){
      if(ch == WRTau_Core::TauE)       { p[0] = -1.4001274630732372  ; p[1] = 0.01452282648020714 ; p[2] = -2.8902584837849924e-05 ;tail = 0.3165701345762715 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = -0.03161676869605785 ; p[1] = 0.0013482007655308769 ; p[2] = -2.3948823764251938e-06 ;tail = 0.07196337096413731 ;}
    }
    if(DataEra=="2017" && isBoosted){
      if(ch == WRTau_Core::TauE)       { p[0] = -0.29723912122872836 ; p[1] = 0.0034342755525335325 ; p[2] = -5.7350302218065596e-06 ;tail = 0.1547928400042227 ; }
      else if(ch == WRTau_Core::TauMu) { p[0] = -0.05524685540892613 ; p[1] = 0.0015157907473644023 ; p[2] = -2.547591191874851e-06 ;tail = 0.14102732343325047 ; }
    } 
    if(DataEra=="2018" && isResolved){
      if(ch == WRTau_Core::TauE)       { p[0] = 1.009908681215489 ; p[1] = -0.005583946686936777 ; p[2] = 1.1202289138393658e-05 ;tail = 0.41789256502130945 ; }
      else if(ch == WRTau_Core::TauMu) { p[0] = 0.0547245873619237 ; p[1] = 0.0008724551468712127 ; p[2] = -1.7092455524811627e-06 ;tail = 0.07477745525518946 ; }
    } 
    if(DataEra=="2018" && isBoosted){
      if(ch == WRTau_Core::TauE)       { p[0] = -0.29759761136978863 ; p[1] = 0.0034983219862958354 ; p[2] = -5.691249372788673e-06 ;tail = 0.14411400961605803 ; }
      else if(ch == WRTau_Core::TauMu) { p[0] = 0.18637005703555454 ; p[1] = -0.0002055282846335179 ; p[2] = 7.919895853886084e-07 ;tail = 0.2028897466489163 ; }
    } 
    head = p[0] + p[1] * x + p[2] * x * x ;
  }
  else if(deg == 3){
    if(DataYear==2016 && isResolved){
      if(ch == WRTau_Core::TauE)       { p[0] = 1.6048945015782925 ; p[1] = -0.018952815931256417 ; p[2] = 8.46657886189916e-05 ;  p[3] = -1.1840041540973055e-07 ;tail = 0.17252879462092222 ;x0 = 399.083449262323 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = -0.763360414333092 ; p[1] = 0.00833167067543171 ; p[2] = -2.3220028631676983e-05 ;  p[3] = 2.011277511053682e-08 ;tail = 0.11446997178554827 ;x0 = 484.89028599728783 ;}
    }
    if(DataYear==2016 && isBoosted){
      if(ch == WRTau_Core::TauE)       { p[0] = -1.2209634097808408 ; p[1] = 0.014069163620798644 ; p[2] = -4.516305426626977e-05 ;  p[3] = 4.730985337608265e-08 ;tail = 0.2121051179073685 ;x0 = 364.3403760963194 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = -1.4522773934898419 ; p[1] = 0.015875348899732907 ; p[2] = -4.96354275692718e-05 ;  p[3] = 4.873886266969021e-08 ;tail = 0.09590357454565306 ;x0 = 421.08831869824036 ;}
    } 
    if(DataEra=="2017" && isResolved){
      if(ch == WRTau_Core::TauE)       { p[0] = 2.9363093123562773 ; p[1] = -0.037681175212527875 ; p[2] = 0.0001773680893060778 ;  p[3] = -2.6736796253372166e-07 ;tail = 0.3165701345242996 ;x0 = 351.0169016567174 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = -0.11248549120115643 ; p[1] = 0.0021583325627080137 ; p[2] = -4.96936626663147e-06 ;  p[3] = 2.5910007386413747e-09 ;tail = 0.07196337096902905 ;x0 = 552.5478904507842 ;}
    }
    if(DataEra=="2017" && isBoosted){
      if(ch == WRTau_Core::TauE)       { p[0] = -0.15555586989914988 ; p[1] = 0.0018805396027885136 ; p[2] = -2.485355964205635e-07 ;  p[3] = -6.228027856439282e-09 ;tail = 0.15479284009613192 ;x0 = 480.40144528864926 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = -1.7562786356252562 ; p[1] = 0.020129333047017636 ; p[2] = -6.814996468737049e-05 ;  p[3] = 7.438535070015779e-08 ;tail = 0.14102732353773137 ;x0 = 360.7181061491338 ;}
    } 
    if(DataEra=="2018" && isResolved){
      if(ch == WRTau_Core::TauE)       { p[0] = -0.8422488840414395 ; p[1] = 0.01607895970173253 ; p[2] = -7.133733518882485e-05 ;  p[3] = 1.0226738482751318e-07 ;tail = 0.4178925649608058 ;x0 = 273.22352597429585 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = 0.48320288475145673 ; p[1] = -0.003445470477049935 ; p[2] = 1.20885557121088e-05 ;  p[3] = -1.3934868159435819e-08 ;tail = 0.07477745526859059 ;x0 = 520.4353614603767 ;}
    } 
    if(DataEra=="2018" && isBoosted){
      if(ch == WRTau_Core::TauE)       { p[0] = -0.5557504627464769 ; p[1] = 0.006210957725518246 ; p[2] = -1.4796904985306322e-05 ;  p[3] = 9.723996753511515e-09 ;tail = 0.144114009592232 ;x0 = 551.6484606339233 ;}
      else if(ch == WRTau_Core::TauMu) { p[0] = 3.5779840861879277 ; p[1] = -0.039130570752796856 ; p[2] = 0.00014605790095818737 ;  p[3] = -1.759671673219771e-07 ;tail = 0.20288974675568403 ;x0 = 404.69280175462666 ;}
    } 
    if(x>x0) head = p[0] + p[1] * x + p[2] * x * x + p[3] * x * x * x ;
    else head = tail ;
  }

  return std::max(head,tail);

}


// Considering only tau when estimating lepton fakes with MC
double WRTau_Core::GetTauFRWeight(const Tau tau, const std::vector<Lepton *> leps, const std::vector<Gen>& gens, WRTau_Core::SearchRegion region,const int deg){
  
    //cout << "[WRTau_Core::GetTauFRWeight] Method called ... " << endl;

  double r_f = GetTauFRfromBins(tau,leps,region);
  double w_f = r_f / (1-r_f);
  double w_p = 1.; double r_p = 0.;
  double x = 0.;

  //cout << "[WRTau_Core::GetTauFRWeight] GetTauFR : " <<  r_f <<  endl;

  TString channel = GetChannelString(GetChannel(leps));

  if(tau.Pt() > 1000.) x = 999.;
  else x = tau.Pt() ;

  //cout << "[WRTau_Core::GetTauFRWeight] GetChannelString : " << channel << endl;

  if(isResolvedRegion(region)) r_p = fakeEst->GetTauPromptRate("Resolved",channel,x);
  if(isBoostedRegion(region))  r_p = fakeEst->GetTauPromptRate("Boosted",channel,x);
  w_p = (1-r_p)/r_p;
  double coeff = 1./(1.-w_f*w_p);

  if(HasFlag("TauFRDebug")) cout << GetRegionString(region) << "_" << channel << " : pt,r_f,r_p,w_f,w_p,coeff,TauFRErr = " << x << " , " << r_f << " , " << r_p << " , " << w_f << " , " << w_p << " , " << coeff << " , " << TauFRErr << endl; 

  //cout << "[WRTau_Core::GetTauFRWeight] GetTauPromptRate : " << r_p << endl;

  /* AN2016-261 matrix method :
  The number of fake passing leptons is given by the total number of events with no leptons
  passing the tight cut multiplied by e, from which we subtract the number of events with 1
  lepton passing, multiplied by eη .
  In conclusion, all leptons (prompt or fake) which fail the tight cuts have to be weighted by e,
  fake leptons which pass by eη and prompt leptons which pass by 1.
  */
  if(!tau.passTIDvJet()){
    if(HasFlag("TauFRDebug")) cout << "weight = " << coeff << " * " << w_f << " = " << coeff*w_f << endl;
    return w_f;
  }
  else {
    if(HasFlag("TauFRDebug")) cout << "weight = -" << coeff << " * " << w_f << " * " << w_p <<  " = " << -coeff*w_f*w_p << endl;
    return 1.;
  }
  /*
  else{
    if(IsNonPromptTau(tau,gens)){
      return coeff*w_f * w_p; 
    }
    else if(!IsNonPromptTau(tau,gens)){
      return coeff;
    }
  }
  if(IsNonPromptTau(tau,gens)){
    if(tau.passTIDvJet()){
      if(HasFlag("FRTest")) cout << "case 2 called" << endl;
      return coeff*w_f * w_p; 
    }
  }
  else{
    if(HasFlag("FRTest")) cout << "none called, returning with 1.0" << endl;
    return coeff;
  }*/

}

// Considering both electron and tau fakes (Resolved region, eltau channel)
double WRTau_Core::GetElTauFRWeight(const Tau tau, const Electron el,const std::vector<Gen>& gens, WRTau_Core::SearchRegion region){

  //cout << "[WRTau_Core::GetElTauFRWeight] Method called ... " << endl; 

  double w = -999.;
  double f_tau = 1.; //GetTauFR(tau,WRTau_Core::TauE,region,2);
  double f_ele = fakeEst->GetElectronFakeRate("WRTau_Resolved","",el.Eta(),el.Pt());
  double p_tau = 1.0;
  double x = 0.;

  //cout << "[WRTau_Core::GetElTauFRWeight] fakeEst->GetElectronFakeRate =  " << f_ele << endl;

  if(tau.Pt() > 1000.) x = 999.;
  else x = tau.Pt() ;

  if(isResolvedRegion(region)) p_tau = fakeEst->GetTauPromptRate("Resolved","ElTau",x);
  if(isBoostedRegion(region))  p_tau = fakeEst->GetTauPromptRate("Boosted","ElTau",x);

  //cout << "[WRTau_Core::GetElTauFRWeight] fakeEst->GetTauPromptRate = " << p_tau << endl;

  double wf_tau = f_tau / (1.-f_tau) ;
  double wp_tau = (1.-p_tau) / p_tau ;
  double wf_ele = f_ele / (1.-f_ele) ;

  bool isPromptEle = IsPromptLepton(el,gens);
  bool isFakeEle   = !isPromptEle;
  bool isPromptTau = IsPromptTau(tau,gens);
  bool isFakeTau   = !isPromptTau;

  /*
    i.e for 
      N_t(00) : !passEleTight && !passTauTight
      N_t(10) : passEleTight && !passTauTight
  */
  bool passTauTight = tau.passTIDvJet();        // T *1
  bool passEleTight = el.PassID("passHEEPID");  // T 1*

  if(!passEleTight && !passTauTight)                               w = wf_tau * wf_ele;
  else if(!passEleTight && passTauTight){
    if((isPromptEle && isPromptTau) || (isFakeEle && isPromptTau)) w = wf_ele;
    if((isPromptEle && isFakeTau)   || (isFakeEle && isFakeTau))   w = wf_ele * wf_tau * wp_tau;
  }
  else if(passEleTight && !passTauTight){
    if(isPromptEle)                                            w = wf_tau;
    else                                                       w = wf_ele * wf_tau;
  }
  else{
    if(isPromptEle && isPromptTau)                             w = 1.;
    if(isPromptEle && isFakeTau)                               w = wf_tau * wp_tau;
    if(isFakeEle && isPromptTau)                               w = wf_ele;
    if(isFakeEle && isFakeTau)                                 w = wf_ele * wf_tau * wp_tau;
  }

  if(w<0.) cout << "[WRTau_Core::GetElTauFRWeight] Something's wrong ... weight = " << w << endl;

  return w;

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

  if(HasFlag("SignalDebug") && !_isTauChannel) PrintGen(gens);

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
      if(IsxyCorr) { Met_pt  = PuppiMET_Type1_PhiCor_pt;  Met_phi = PuppiMET_Type1_PhiCor_phi;} 
      else  {        Met_pt  = PuppiMET_Type1_pt; Met_phi = PuppiMET_Type1_phi; }
    } // T1
    else{            Met_pt  = PuppiMET_pt; Met_phi=PuppiMET_phi; }
  } 
  
  else{ // PFMET
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
	      else          vMET.SetPtEtaPhiM(PuppiMET_Type1_pt_shifts->at(IdxSyst), 0.,  PuppiMET_Type1_phi_shifts->at(IdxSyst), 0.); 
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
  double METPhi = vMETSmeared.Phi();
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

Particle WRTau_Core::GetvCorrMET(TString METType, AnalyzerParameter param, Particle METUncorr){

  //// THIS FUNCTION UPDATES MET BASED ON JET Smearing / muon rocc  
  //// Follows https://twiki.cern.ch/twiki/bin/view/CMS/MissingETRun2Corrections                                                                                                                                                                                               
  //// Jets in simulation can be smeared (as shown in JetResolution twiki) to achieve better agreement with data. This is done by default in GetAllJets().....  This correction is a propagation of the smeared such jets to MET. The Smeared MET correction replaces the vector sum of transverse momenta of particles which can be clustered as jets with the vector sum of the transverse momenta of the jets to which smearing is applied.                                                                                                                                                               
  //// Jets pt > 15 GeV not near PF muon OR electrons                                                                                                                                                                                                                                                                         
  
  bool SmearJets  = METType.Contains("SmearJet");
  bool SmearMuons = METType.Contains("SmearMuon");
  bool SmearBoth  = !SmearJets &&  !SmearMuons;
  
  if(SmearBoth || SmearJets) {
    
    std::vector<Jet>  Jets        = SelectJets(param,param.Jet_ID,40,2.4);
    std::vector<Muon> loose_muons = GetMuons("POGLoose",     10.,  2.4);
    
    std::vector<Jet> jets_corr;
    for(auto ij : Jets){
      
      if(ij.Pt() < 15.) continue;
      if(fabs(ij.Eta()) > 2.5) continue;
      
      bool overlap_mu(false);
      for(auto imu : loose_muons) {
        if(imu.DeltaR(ij) < 0.4) {
	        overlap_mu=true;
	        break;
       }
      }

      if(overlap_mu) continue;
      
      double jetEMFrac = ij.ChargedEmEnergyFraction() + ij.NeutralEmEnergyFraction();
      if (jetEMFrac > 0.9) continue;
      jets_corr.push_back(ij);
    }
   
   Particle UpdatedMET = UpdateMETSmearedJet(METUncorr,jets_corr);
   if(SmearJets) return UpdatedMET;

   std::vector<Muon> tight_muons = GetMuons(param.Muon_Tight_ID, 20.,  2.4);
   Particle UpdatedMET2 = UpdateMET(UpdatedMET,tight_muons);
   
   return UpdatedMET2;

  }
  else if(SmearMuons){
    
    std::vector<Muon> tight_muons = GetMuons(param.Muon_Tight_ID, 20.,  2.4);
    Particle UpdatedMET = UpdateMET(METUncorr,tight_muons);
   
    return UpdatedMET;

  }

 return METUncorr;

}


Particle WRTau_Core::GetvMET(TString METType, AnalyzerParameter param,bool PropSmearing){

  bool ApplySyst      = (!IsDATA) && (param.syst_ != AnalyzerParameter::Central);

  Particle vStandMET = GetMiniAODvMET(METType);
  if(!ApplySyst && !PropSmearing) return vStandMET;  //// This function calls central values stored in MINMIAOD OR POG COrrected                                                                                                                                                                                              
  if(!ApplySyst && PropSmearing)  return GetvCorrMET(METType,param,vStandMET);

  bool UsePuppi     = METType.Contains("Puppi");
  bool IsxyCorr     = METType.Contains("xyCorr");

  int IdxSyst = -1;
  if(param.syst_ == AnalyzerParameter::METUnclUp)     IdxSyst = 10;
  if(param.syst_ == AnalyzerParameter::METUnclDown)   IdxSyst = 11;
  if(param.syst_ == AnalyzerParameter::JetResUp)      IdxSyst = 0;
  if(param.syst_ == AnalyzerParameter::JetResDown)    IdxSyst = 1;
  if(param.syst_ == AnalyzerParameter::JetEnUp)       IdxSyst = 2;
  if(param.syst_ == AnalyzerParameter::JetEnDown)     IdxSyst = 3;
  if(param.syst_ == AnalyzerParameter::MuonEnUp)      IdxSyst = 4;
  if(param.syst_ == AnalyzerParameter::MuonEnDown)    IdxSyst = 5;
  if(param.syst_ == AnalyzerParameter::ElectronEnUp)  IdxSyst = 6;
  if(param.syst_ == AnalyzerParameter::ElectronEnDown)IdxSyst = 7;

  Particle vMETSyst;

  if(IdxSyst>=0 && IdxSyst < 8){

    if(UsePuppi){
      if( isfinite(PuppiMET_Type1_pt_shifts->at(IdxSyst)))  vMETSyst = UpdateMETSyst(PuppiMET_Type1_pt, PuppiMET_Type1_phi, PuppiMET_Type1_pt_shifts->at(IdxSyst),PuppiMET_Type1_phi_shifts->at(IdxSyst), vStandMET);
      else return vStandMET;
    }
    else{
      if(isfinite(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst))){
        if(IsxyCorr) vMETSyst = UpdateMETSyst(pfMET_Type1_PhiCor_pt, pfMET_Type1_PhiCor_phi, pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst), pfMET_Type1_PhiCor_phi_shifts->at(IdxSyst), vStandMET);
        else         vMETSyst = UpdateMETSyst(pfMET_Type1_pt, pfMET_Type1_phi, pfMET_Type1_pt_shifts->at(IdxSyst), pfMET_Type1_phi_shifts->at(IdxSyst), vStandMET);
      }
      else return vStandMET;
    }
  }

  return vMETSyst;
}




Particle WRTau_Core::GetvMET(TString METType, AnalyzerParameter param,
                                 std::vector<Jet> jets, std::vector<FatJet> fatjets,
                                 std::vector<Muon> muons, std::vector<Electron> electrons,
                                 bool PropSmearing){

  ////// This function is used to get MET both central and systematic                                                                                                                                                                                                                                                         

  bool ApplySyst      = (!IsDATA) && (param.syst_ != AnalyzerParameter::Central);

  Particle vStandMET = GetMiniAODvMET(METType);
  if(!ApplySyst && !PropSmearing) return vStandMET;  //// This function calls central values stored in MINMIAOD OR POG COrrected                                                                                                                                                                                              
  if(!ApplySyst && PropSmearing)  return GetvCorrMET(METType,param,vStandMET);

  bool UsePuppi     = METType.Contains("Puppi");
  bool IsxyCorr     = METType.Contains("xyCorr");

  int IdxSyst = -1;
  if(param.syst_ == AnalyzerParameter::METUnclUp)             IdxSyst = 10;
  else if(param.syst_ == AnalyzerParameter::METUnclDown)           IdxSyst = 11;
  else if(param.syst_ == AnalyzerParameter::JetResUp)              IdxSyst = 0;
  else if(param.syst_ == AnalyzerParameter::JetResDown)            IdxSyst = 1;
  else if(param.syst_ == AnalyzerParameter::JetEnUp)               IdxSyst = 2;
  else if(param.syst_ == AnalyzerParameter::JetEnDown)             IdxSyst = 3;
  else if(param.syst_ == AnalyzerParameter::MuonEnUp)              IdxSyst = 4;
  else if(param.syst_ == AnalyzerParameter::MuonEnDown)            IdxSyst = 5;
  else if(param.syst_ == AnalyzerParameter::ElectronEnUp)          IdxSyst = 6;
  else if(param.syst_ == AnalyzerParameter::ElectronEnDown)        IdxSyst = 7;
  // syst source not defined in CMSSW
  else                                                        IdxSyst = 21;

  Particle vMETSyst = PropSmearing ? GetvCorrMET(METType,param,vStandMET) : vStandMET;

  if(IdxSyst >= 20 )  vMETSyst = UpdateMETSyst(param, vStandMET, jets, fatjets, muons, electrons);
  else if(IdxSyst>=0){

    if(UsePuppi) {
      if(isfinite(PuppiMET_Type1_pt_shifts->at(IdxSyst))){
        vMETSyst = UpdateMETSyst(PuppiMET_Type1_pt, PuppiMET_Type1_phi, PuppiMET_Type1_pt_shifts->at(IdxSyst),PuppiMET_Type1_phi_shifts->at(IdxSyst), vStandMET);
      }
    }
    else {
      if(isfinite(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst))){
        if(IsxyCorr) vMETSyst = UpdateMETSyst(pfMET_Type1_PhiCor_pt, pfMET_Type1_PhiCor_phi, pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst), pfMET_Type1_PhiCor_phi_shifts->at(IdxSyst), vStandMET);
        else         vMETSyst = UpdateMETSyst(pfMET_Type1_pt, pfMET_Type1_phi, pfMET_Type1_pt_shifts->at(IdxSyst), pfMET_Type1_phi_shifts->at(IdxSyst), vStandMET);
      }
    }
  }
  else{
    cout << "[WRTau_Core::GetvMET] There is no matched syst type;" << endl;
    cout << "[WRTau_Core::GetvMET] Current syst index : " << IdxSyst << "," << param.GetSystType() << endl;
    exit(EXIT_FAILURE);
  }

  return vMETSyst;

}

map<TString, Particle> WRTau_Core::METMap( AnalyzerParameter param){

  vector<TString> vmets = {"T1xyCorr",
                           "PuppiT1xyCorr",
                           "T1",
                           "PuppiT1",
                           "PuppiT1xyULCorr",
                           "T1xyULCorr"};


  Particle METv      = GetMiniAODvMET("T1xyCorr");
  Particle PuppiMETv = GetMiniAODvMET("PuppiT1xyCorr");
  Particle METvNoPhi = GetMiniAODvMET("T1");
  Particle PuppiMETvNoPhi = GetMiniAODvMET("PuppiT1");
  Particle PuppiMETvULPhiCorr = GetMiniAODvMET("PuppiT1xyULCorr");
  Particle METvULPhiCorr = GetMiniAODvMET("T1xyULCorr");
  map<TString, Particle> mapmet;
  for(auto i : vmets) {
    mapmet[i] = GetvMET(i,param);
    mapmet[i+"_propsmear"] = GetvMET(i,param,true);
    mapmet[i+"_SmearJet_propsmear"] = GetvMET(i+"SmearJet",param,true);
    mapmet[i+"_SmearMuon_propsmear"] = GetvMET(i+"SmearMuon",param,true);
  }

  return mapmet;
}

Particle WRTau_Core::GetMiniAODvMET(TString METType){
  //PuppiT1xyULCorr
  bool IsType1      = METType.Contains("T1");
  bool IsxyCorr     = METType.Contains("xyCorr");
  bool UsePuppi     = METType.Contains("Puppi");
  bool IsFixxyCorr  = METType.Contains("xyULCorr");

  //METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, TString year, bool isMC, int npv, bool isUL =false,bool ispuppi=false)r                                                          

  
  double Met_pt(0.), Met_phi(0.);

  Particle vMET;

  if(UsePuppi && IsType1  && IsxyCorr)  { Met_pt = PuppiMET_Type1_PhiCor_pt ; Met_phi = PuppiMET_Type1_PhiCor_phi;}
  if(UsePuppi && IsType1  && !IsxyCorr) { Met_pt = PuppiMET_Type1_pt;         Met_phi = PuppiMET_Type1_phi;}
  if(UsePuppi && !IsType1 && !IsxyCorr) { Met_pt = PuppiMET_pt;               Met_phi = PuppiMET_phi;}

  if(!UsePuppi&& IsType1  && IsxyCorr)  { Met_pt = pfMET_Type1_PhiCor_pt;     Met_phi = pfMET_Type1_PhiCor_phi;}
  if(!UsePuppi&& IsType1  && !IsxyCorr) { Met_pt = pfMET_Type1_pt;            Met_phi = pfMET_Type1_phi;}
  if(!UsePuppi&& !IsType1 && !IsxyCorr) { Met_pt = pfMET_pt;                  Met_phi = pfMET_phi;}

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

double WRTau_Core::GetTauTriggerSF(const int syst){
  
  // https://twiki.cern.ch/twiki/bin/view/CMS/TauTrigger#Run_II_Trigger_Scale_Factors
  // Flat SF given for single highpt hadronic tau triggers
  
  if(DataYear == 2016) return 0.88 + syst * 0.08 ;
  if(DataYear == 2017) return 1.08 + syst * 0.10 ;
  if(DataYear == 2018) return 0.87 + syst * 0.11 ;
  
  return 1.0;

}

double WRTau_Core::LSFSF(WRTau_Core::Channel ch,const int syst){

  double LSFSF_EJet_Central = 1.04;
  double LSFSF_EJet_Error_Up = 0.09;
  double LSFSF_EJet_Error_Down = 0.08;

  double LSFSF_MJet_Central = 1.01;
  double LSFSF_MJet_Error_Up = 0.06;
  double LSFSF_MJet_Error_Down = 0.06;

  if(DataYear==2017){

    LSFSF_EJet_Central = 1.02;
    LSFSF_EJet_Error_Up = 0.08;
    LSFSF_EJet_Error_Down = 0.08;

    LSFSF_MJet_Central = 0.98;
    LSFSF_MJet_Error_Up = 0.07;
    LSFSF_MJet_Error_Down = 0.07;

  }
  if(DataYear==2018){

    LSFSF_EJet_Central = 1.05;
    LSFSF_EJet_Error_Up = 0.07;
    LSFSF_EJet_Error_Down = 0.06;

    LSFSF_MJet_Central = 1.04;
    LSFSF_MJet_Error_Up = 0.06;
    LSFSF_MJet_Error_Down = 0.05;

  }

  if(ch == WRTau_Core::TauE){
    if(syst==0)     return LSFSF_EJet_Central;
    else if(syst>0) return LSFSF_EJet_Central+LSFSF_EJet_Error_Up;
    else           return LSFSF_EJet_Central-LSFSF_EJet_Error_Down;
  }
  else if(ch == WRTau_Core::TauMu){
    if(syst==0)     return LSFSF_MJet_Central;
    else if(syst>0) return LSFSF_MJet_Central+LSFSF_MJet_Error_Up;
    else           return LSFSF_MJet_Central-LSFSF_MJet_Error_Down;
  }
  else{
    cerr << "[WRTau_Analyzer::LSFSF] wrong lepflav  " << endl;
    exit(EXIT_FAILURE);
    return 1.;
  }


}


WRTau_Core::WRTau_Core(){

  histDir = GetTempDir_WRTauCore();

}

WRTau_Core::~WRTau_Core(){

}


