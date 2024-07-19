#include "FakeBackgroundEstimator.h"

FakeBackgroundEstimator::FakeBackgroundEstimator() : 
IgnoreNoHist(true),
HasLooseLepton(false)
{

  histDir = TDirectoryHelper::GetTempDirectory("FakeBackgroundEstimator");

}

void FakeBackgroundEstimator::ReadHistograms(){

  TString datapath = getenv("DATA_DIR");
  datapath = datapath+"/"+GetEra()+"/FakeRate/";

  TDirectory* origDir = gDirectory;

  string elline;
  ifstream in(datapath+"/histmap_Electron.txt");
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString a,b,c,d,e;
    is >> a; // <ID>
    is >> b; // <rootfilename>
    TFile *file = new TFile(datapath+"/"+b);
    TList *histlist = file->GetListOfKeys();
    for(int i=0;i<histlist->Capacity();i++){
      TString this_frname = histlist->At(i)->GetName();
      histDir->cd();
      map_hist_Electron[a+"_"+this_frname] = (TH2D *)file->Get(this_frname)->Clone();
      file->Close();
      delete file;
      origDir->cd();
      //cout << "[FakeBackgroundEstimator::FakeBackgroundEstimator] map_hist_Electron : " << a+"_"+this_frname << endl;
    }
  }

  string elline2;
  ifstream in2(datapath+"/histmap_Muon.txt");
  while(getline(in2,elline2)){
    std::istringstream is( elline2 );
    TString a,b,c,d,e;
    is >> a; // <ID>
    is >> b; // <rootfilename>
    TFile *file = new TFile(datapath+"/"+b);
    TList *histlist = file->GetListOfKeys();
    for(int i=0;i<histlist->Capacity();i++){
      TString this_frname = histlist->At(i)->GetName();
      histDir->cd();
      map_hist_Muon[a+"_"+this_frname] = (TH2D *)file->Get(this_frname)->Clone();
      file->Close();
      delete file;
      origDir->cd();
      //cout << "[FakeBackgroundEstimator::FakeBackgroundEstimator] map_hist_Muon : " << a+"_"+this_frname << endl;
    }
  }

  // Tau histogram 
  TFile *file1 = new TFile(datapath+"/Tau/PromptRate.root");
  histDir->cd();
  Tau_PR_Boosted_El  = (TH1D *) file1->Get("BoostedSignalRegionMETInvertMTSame_ElTau_PRSubtract_All_All")->Clone();
  file1->Close();
  delete file1;
  origDir->cd();

  TFile *file2 = new TFile(datapath+"/Tau/PromptRate.root");
  histDir->cd();
  Tau_PR_Resolved_El = (TH1D *) file2->Get("ResolvedSignalRegionMETInvertMTSame_ElTau_PRSubtract_All_All")->Clone();
  file2->Close();
  delete file2;
  origDir->cd();

  TFile *file3 = new TFile(datapath+"/Tau/PromptRate.root");
  histDir->cd();
  Tau_PR_Boosted_Mu  = (TH1D *) file3->Get("BoostedSignalRegionMETInvertMTSame_MuTau_PRSubtract_All_All")->Clone();
  file3->Close();
  delete file3;
  origDir->cd();

  TFile *file4 = new TFile(datapath+"/Tau/PromptRate.root");
  histDir->cd();
  Tau_PR_Resolved_Mu = (TH1D *) file4->Get("ResolvedSignalRegionMETInvertMTSame_MuTau_PRSubtract_All_All")->Clone();
  file4->Close();
  delete file4;
  origDir->cd();

  // Tau FakeRate Functions
  TString datapath_taufake = getenv("DATA_DIR");
  datapath_taufake += "/TauFake";

  TFile *fileTauFR = new TFile(datapath_taufake+"/FitResultsNew.root");

  std::vector<TString> region  = {"ResolvedSignalRegionMETInvert", "BoostedSignalRegionMETInvert"};
  std::vector<TString> channel = {"ElTau", "MuTau"};
  
  TString YearString;
  YearString.Form("%d",GetYear()); 

  for(TString rg : region) {
      for(TString ch : channel) {
          histDir->cd();
          map_TF1_tau[rg+"_"+ch+"_"+YearString]    =  (TF1 *)  fileTauFR->Get(rg+"_"+ch+"_"+YearString);
          cout << "Set " << rg+"_"+ch+"_"+YearString << "fit TF1 @ " << map_TF1_tau[rg+"_"+ch+"_"+YearString] << endl;
          //fileTauFR->Close();
          //delete fileTauFR;
          origDir->cd();
      }
  }

  fileTauFR->Close();
  delete fileTauFR;

  TFile *fileTauFR_ = new TFile(datapath_taufake+"/FitResultsNew_Unc.root");

  for(TString rg : region) {
      for(TString ch : channel) {
          histDir->cd();
          map_FitErr_tau[rg+"_"+ch+"_"+YearString] = (TH1D*) fileTauFR_->Get(rg+"_"+ch+"_"+YearString+"_err")->Clone();
          cout << "Set " << rg+"_"+ch+"_"+YearString << " fit err TH1D @ " << map_FitErr_tau[rg+"_"+ch+"_"+YearString] << endl;
          origDir->cd();
      }
  }

  fileTauFR_->Close();
  delete fileTauFR_;

  std::vector<TString> region_new  = {"ResolvedSignalRegionMETInvertMTSame", "BoostedSignalRegionMETInvertMTSame"};

  // Tau FR Histogram
  TFile *fileTauFR_Binned = new TFile(datapath_taufake+"/"+YearString+".root");
  for(TString rg : region_new) {
      for(TString ch : channel) {
          histDir->cd();
          map_hist_tau[rg+"_"+ch] = (TH1D*) fileTauFR_Binned->Get(rg+"_"+ch+"_DataDrivenSubtract_All_All")->Clone();
          cout << "Set " << rg+"_"+ch << " TH1D @ " << map_hist_tau[rg+"_"+ch] << endl;
          origDir->cd();
      }
  }

  fileTauFR_Binned->Close();
  delete fileTauFR_Binned;


}

FakeBackgroundEstimator::~FakeBackgroundEstimator(){

}

double FakeBackgroundEstimator::GetTauPromptRate(TString region,TString channel,double pt, int sys){

  double value = 1.;
  double error = 0.;

  if(pt>=1000.) pt = 999.;

  //cout << "[FakeBackgroundEstimator::GetTauPromptRate] region = " << region << ", channel = " << channel << ", pt = " << pt << endl;

  if(region == "Boosted"){

    if(channel == "ElTau") {
      //cout << "[FakeBackgroundEstimator::GetTauPromptRate] Boosted ElTau Called " << endl;
      int this_bin = Tau_PR_Boosted_El->FindBin(pt);
      value = Tau_PR_Boosted_El->GetBinContent(this_bin);
      error = Tau_PR_Boosted_El->GetBinError(this_bin); 
    }
    else if(channel == "MuTau") {
      //cout << "[FakeBackgroundEstimator::GetTauPromptRate] Boosted MuTau Called " << endl;
      int this_bin = Tau_PR_Boosted_Mu->FindBin(pt);
      value = Tau_PR_Boosted_Mu->GetBinContent(this_bin);
      error = Tau_PR_Boosted_Mu->GetBinError(this_bin); 
    }
  }
  else if(region == "Resolved"){

    if(channel == "ElTau"){
      //cout << "[FakeBackgroundEstimator::GetTauPromptRate] Resolved ElTau Called " << endl;
      int this_bin = Tau_PR_Resolved_El->FindBin(pt);
      value = Tau_PR_Resolved_El->GetBinContent(this_bin);
      error = Tau_PR_Resolved_El->GetBinError(this_bin); 
    }
    else if(channel == "MuTau"){
      //cout << "[FakeBackgroundEstimator::GetTauPromptRate] Resolved MuTau Called " << endl;
      int this_bin = Tau_PR_Resolved_Mu->FindBin(pt);
      value = Tau_PR_Resolved_Mu->GetBinContent(this_bin);
      error = Tau_PR_Resolved_Mu->GetBinError(this_bin); 
    }

  }

  return value+double(sys)*error;

}

double FakeBackgroundEstimator::GetElectronFakeRate(TString ID, TString key, double eta, double pt, int sys){

  //cout << "[FakeBackgroundEstimator::GetElectronFakeRate] ID = " << ID << ", key = " << key << endl;
  //cout << "[FakeBackgroundEstimator::GetElectronFakeRate] eta = " << eta << ", pt = " << pt << endl;

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);

  if(pt>=80) pt = 79.9;
  if(eta>=2.5) eta = 2.49;
  //==== HOTFIX FIXME
  //if(eta>=1.479 && pt>=150) pt = 149.;

  std::map< TString, TH2D* >::const_iterator mapit;
  mapit = map_hist_Electron.find(ID);

  if(mapit==map_hist_Electron.end()){
    if(IgnoreNoHist) return 1.;
    else{
      cout << "[FakeBackgroundEstimator::GetElectronFakeRate] No"<< ID+"_"+key <<endl;
      exit(ENODATA);
    }
  }

  int this_bin = (mapit->second)->FindBin(pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  //cout << "[FakeBackgroundEstimator::FakeBackgroundEstimator] value = " << value << endl;

  return value+double(sys)*error;

}

double FakeBackgroundEstimator::GetMuonFakeRate(TString ID, TString key, double eta, double pt, int sys){

  //cout << "[FakeBackgroundEstimator::GetMuonFakeRate] ID = " << ID << ", key = " << key << endl;
  //cout << "[FakeBackgroundEstimator::GetMuonFakeRate] eta = " << eta << ", pt = " << pt << endl;

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);

  if(pt>=200) pt = 199;
  if(eta>=2.5) eta = 2.49;
  //==== HOTFIX FIXME
  if(eta<0.8 && pt>=150) pt = 149.;

  std::map< TString, TH2D* >::const_iterator mapit;
  mapit = map_hist_Muon.find(ID+"_"+key);

  if(mapit==map_hist_Muon.end()){
    if(IgnoreNoHist) return 1.;
    else{
      cout << "[FakeBackgroundEstimator::GetMuonFakeRate] No"<< ID+"_"+key <<endl;
      exit(ENODATA);
    }
  }

  int this_bin = (mapit->second)->FindBin(pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  //cout << "[FakeBackgroundEstimator::FakeBackgroundEstimator] value = " << value << endl;

  return value+double(sys)*error;

}

double FakeBackgroundEstimator::GetWeight(vector<Lepton *> lepptrs, AnalyzerParameter param, int sys){

  double this_weight = -1.;
  vector<double> FRs;
  for(unsigned int i=0; i<lepptrs.size(); i++){

    double this_fr = -999.;

    if(lepptrs.at(i)->LeptonFlavour()==Lepton::ELECTRON){

      Electron *el = (Electron *)( lepptrs.at(i) );
      if( el->PassID(param.Electron_Tight_ID) ) continue;

      double this_pt = el->Pt();
      if(param.Electron_UsePtCone) this_pt = el->PtCone();
      this_fr = GetElectronFakeRate(param.Electron_FR_ID, param.Electron_FR_Key, fabs(el->scEta()), this_pt, sys);

      this_weight *= -1.*this_fr/(1.-this_fr);

      FRs.push_back(this_fr);
    }
    else{

      Muon *mu = (Muon *)( lepptrs.at(i) );
      if( mu->PassID(param.Muon_Tight_ID) ) continue;

      double this_pt = mu->Pt();
      if(param.Muon_UsePtCone) this_pt = mu->PtCone();
      this_fr = GetMuonFakeRate(param.Muon_FR_ID, param.Muon_FR_Key, fabs(mu->Eta()), this_pt, sys);

      this_weight *= -1.*this_fr/(1.-this_fr);

      FRs.push_back(this_fr);
    }


  }

/*
  for(unsigned int i=0; i<FRs.size(); i++){
    cout << FRs.at(i) << endl;
  }
*/

  if(FRs.size()==0){
    HasLooseLepton = false;
    return 0;
  }
  else{
    HasLooseLepton = true;
    return this_weight;
  }

}




