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
    cout << "[WRTauCore::FillMassHists] Wrong selection region" << endl;
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
    cout << "[WRTauCore::CopyHist] Wrong original histogram" << endl;
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


WRTau_Core::WRTau_Core(){

  histDir = GetTempDir_WRTauCore();

}

WRTau_Core::~WRTau_Core(){

}


