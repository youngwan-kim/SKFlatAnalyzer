#include<iostream>
#include<vector>
#include"TH2D.h"
#include"TFile.h"
#include"TKey.h"
#include"TString.h"
#include"THStack.h"
#include"TLegend.h"
#include"TCanvas.h"
#include"TLine.h"
using namespace std;
/////////////////////////////////////////////////////////////////////////////
///////////////////////////// struct and enum ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
struct Sample{
  TString name;
  int type;
  int colorcode;
  vector<TString> files;
};
struct Systematic{
  TString name;
  int type;
  bool exist_data;
  bool exist_bg;
};
struct Histogram{
  TString name;
  int rebin;
  double xmin;
  double xmax;
};
struct Directory{
  TString name;
  vector<Histogram> histograms;
};
enum SampleType{DATA,DY,TAU,BG};
TString GetStringSampleType(SampleType type){
  switch(type){
  case DATA: return "DATA";
  case DY: return "DY";
  case TAU: return "TAU";
  case BG: return "BG";
  default: return "Bad SampleType";
  }
}
TString GetStringEColor(EColor color){
  switch(color){
  case kBlack: return "kBlack";
  case kRed: return "kRed";
  case kGreen: return "kGreen";
  case kBlue: return "kBlue";
  case kYellow: return "kYellow";
  case kMagenta: return "kMagenta";
  default: return "Bad EColor";
  }
}
enum Channel{MUON,ELECTRON};
TString GetStringChannel(Channel channel){
  switch(channel){
  case MUON: return "muon";
  case ELECTRON: return "electron";
  default: return "Bad Channel";
  }
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// global variables ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
vector<Sample> samples;
vector<Systematic> systematics;
vector<Directory> directories;


/////////////////////////////////////////////////////////////////////////////
//////////////////// Add functions for global variables//////////////////////
/////////////////////////////////////////////////////////////////////////////
void AddSample(TString name_,SampleType type_,EColor colorcode_,vector<TString> files_){
  Sample sample;
  sample.name=name_;
  sample.type=type_;
  sample.colorcode=colorcode_;
  sample.files=files_;
  cout<<" [AddSample] "<<sample.name<<" "<<GetStringSampleType((SampleType)sample.type)<<" "<<GetStringEColor((EColor)sample.colorcode)<<endl;
  for(int i=0;i<sample.files.size();i++)
    cout<<"   "<<sample.files.at(i)<<endl;    
  samples.push_back(sample);
}
void AddSample(TString name_,SampleType type_,EColor colorcode_,TString file1,TString file2="",TString file3="",TString file4="",TString file5="",TString file6="",TString file7=""){
  vector<TString> files;
  if(file1!="") files.push_back(file1);
  if(file2!="") files.push_back(file2);
  if(file3!="") files.push_back(file3);
  if(file4!="") files.push_back(file4);
  if(file5!="") files.push_back(file5);
  if(file6!="") files.push_back(file6);
  if(file7!="") files.push_back(file7);
  AddSample(name_,type_,colorcode_,files);
}
void AddSystematic(TString name_,int type_,bool exist_data_,bool exist_bg_){
  Systematic systematic;
  systematic.name=name_;
  systematic.type=type_;
  systematic.exist_data=exist_data_;
  systematic.exist_bg=exist_bg_;
  cout<<" [AddSystematic] "<<systematic.name<<" "<<systematic.type<<" "<<systematic.exist_data<<" "<<systematic.exist_bg<<endl;
  systematics.push_back(systematic);
}
void AddHistogram(Directory& directory,TString name_,int rebin_,double xmin_,double xmax_){
  Histogram histogram;
  histogram.name=name_;
  histogram.rebin=rebin_;
  histogram.xmin=xmin_;
  histogram.xmax=xmax_;
  //cout<<" [AddHistogram] to <<histogram.name<<" "<<histogram.rebin<<" "<<histogram.xmin<<" "<<histogram.xmax<<endl;
  directory.histograms.push_back(histogram);
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Setup functions///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetupSamples(TString Analyzer,int channel,int year){
  TString syear=Form("%d",year);
  cout<<"[SetupSamples] "<<GetStringChannel((Channel)channel)<<year<<endl;
  TString filedir=TString(getenv("SKFlatOutputDir"))+getenv("SKFlatV")+"/"+Analyzer+"/";
  if(year==2017){
    if(channel==Channel::MUON){
      AddSample("data",SampleType::DATA,EColor::kBlack,filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_B.root",filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_C.root",filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_D.root",filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_E.root",filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_F.root");
      AddSample("#gamma*/Z#rightarrow#mu#mu",SampleType::DY,EColor::kRed,filedir+"2017/"+Analyzer+"_SkimTree_SMP_DYJets.root");
    }else if(channel==Channel::ELECTRON){
      AddSample("data",SampleType::DATA,EColor::kBlack,filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_B.root",filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_C.root",filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_D.root",filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_E.root",filedir+"2017/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_F.root");
      AddSample("#gamma*/Z#rightarrowee",SampleType::DY,EColor::kRed,filedir+"2017/"+Analyzer+"_SkimTree_SMP_DYJets.root");
    }else return "";
  }else if(year==2016){
    if(channel==Channel::MUON){
      AddSample("data",SampleType::DATA,EColor::kBlack,filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_B_ver2.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_C.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_D.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_E.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_F.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_G.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleMuon_H.root");
      AddSample("#gamma*/Z#rightarrow#mu#mu",SampleType::DY,EColor::kRed,filedir+"2016/"+Analyzer+"_SkimTree_SMP_DYJets.root");
    }else if(channel==Channel::ELECTRON){
      AddSample("data",SampleType::DATA,EColor::kBlack,filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_B_ver2.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_C.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_D.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_E.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_F.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_G.root",filedir+"2016/DATA/"+Analyzer+"_SkimTree_SMP_DoubleEG_H.root");
      AddSample("#gamma*/Z#rightarrowee",SampleType::DY,EColor::kRed,filedir+"2016/"+Analyzer+"_SkimTree_SMP_DYJets.root");
    }else return "";
  }else return "";
  AddSample("#gamma*/Z#rightarrow#tau#tau",SampleType::TAU,EColor::kGreen,filedir+syear+"/"+Analyzer+"_SkimTree_SMP_DYJets.root");
  AddSample("Diboson",SampleType::BG,EColor::kBlue,filedir+syear+"/"+Analyzer+"_SkimTree_SMP_WW_pythia.root",filedir+syear+"/"+Analyzer+"_SkimTree_SMP_WZ_pythia.root",filedir+syear+"/"+Analyzer+"_SkimTree_SMP_ZZ_pythia.root");
  AddSample("W",SampleType::BG,EColor::kYellow,filedir+syear+"/"+Analyzer+"_SkimTree_SMP_WJets_MG.root");
  AddSample("t#bar{t}",SampleType::BG,EColor::kMagenta,year==2017?filedir+"2017/"+Analyzer+"_SkimTree_SMP_TTLL_powheg.root":filedir+"2016/"+Analyzer+"_SkimTree_SMP_TT_powheg.root");
}
void SetupSystematics(){
  cout<<"[SetupSystematics]"<<endl;
  AddSystematic("RECOSF",2,0,1);
  AddSystematic("IDSF",2,0,1);
  AddSystematic("ISOSF",2,0,1);
  AddSystematic("triggerSF",2,0,1);
  AddSystematic("PUreweight",2,0,1);
  AddSystematic("prefireweight",2,0,1);
  AddSystematic("alphaS",2,0,0);
  AddSystematic("scalevariation",9,0,0);

  AddSystematic("noRECOSF",1,0,1);
  AddSystematic("noIDSF",1,0,1);
  AddSystematic("noISOSF",1,0,1);
  AddSystematic("notriggerSF",1,0,1);
  AddSystematic("noPUreweight",1,0,1);
  AddSystematic("noprefireweight",1,0,1);
  AddSystematic("nozptcor",1,0,0);
  AddSystematic("noefficiencySF",1,0,1);
  AddSystematic("IDSF_POG",1,0,1);
  AddSystematic("efficiencySF",-15,0,0);
  AddSystematic("totalsys",-255,0,0);
}
void SetupDirectories(int channel,int year){
  cout<<"[SetupDirectories] for "<<GetStringChannel((Channel)channel)<<year<<endl;
  TString region[]={"OS","OS_Z","OS_Z_y0.0to0.4","OS_Z_y0.4to0.8","OS_Z_y0.8to1.2","OS_Z_y1.2to1.6","OS_Z_y1.6to2.0","OS_Z_y2.0to2.4","SS"};
  for(int i=0;i<sizeof(region)/sizeof(TString);i++){
    Directory directory;
    directory.name=GetStringChannel((Channel)channel)+Form("%d",year)+"/"+region[i]+"/";
    cout<<directory.name<<" ";
    AddHistogram(directory,"dimass",0,80,100);
    AddHistogram(directory,"dipt",4,0,200);
    AddHistogram(directory,"dirap",0,0,0);
    AddHistogram(directory,"l0pt",4,0,200);
    AddHistogram(directory,"l0eta",0,0,0);
    AddHistogram(directory,"l0riso",0,0,0);
    AddHistogram(directory,"l1pt",4,0,200);
    AddHistogram(directory,"l1eta",0,0,0);
    AddHistogram(directory,"l1riso",0,0,0);
    AddHistogram(directory,"lldelR",0,0,0);
    AddHistogram(directory,"lldelphi",0,0,0);
    AddHistogram(directory,"nPV",0,0,0);
    directories.push_back(directory);
  }
  cout<<endl;
  directories[0].histograms[0].rebin=4;
  directories[0].histograms[0].xmin=0;
  directories[0].histograms[0].xmin=0;
}
TString Setup(int channel,int year){
  TString schannel=GetStringChannel((Channel)channel);
  TString syear=Form("%d",year);

  samples.clear();
  systematics.clear();
  directories.clear();

  SetupSamples("SMPValidation",channel,year);
  SetupSystematics();
  SetupDirectories(channel,year);

  cout<<"[Setup] nsample: "<<samples.size()<<endl;
  cout<<"[Setup] nsys: "<<systematics.size()<<endl;
  cout<<"[Setup] ndirectories: "<<directories.size()<<endl;  
  return schannel+syear;
}
void SetupAFBDirectories(int channel,int year){
  cout<<"[SetupAFBDirectories] for "<<GetStringChannel((Channel)channel)<<year<<endl;
  TString region[]={"OS_m60to120","OS_m60to120_y0.0to0.4","OS_m60to120_y0.4to0.8","OS_m60to120_y0.8to1.2","OS_m60to120_y1.2to1.6","OS_m60to120_y1.6to2.0","OS_m60to120_y2.0to2.4"};
  for(int i=0;i<sizeof(region)/sizeof(TString);i++){
    Directory directory;
    directory.name=GetStringChannel((Channel)channel)+Form("%d",year)+"/"+region[i]+"/";
    cout<<directory.name<<" ";
    AddHistogram(directory,"dimass",2,60,120);
    AddHistogram(directory,"dipt",4,0,200);
    AddHistogram(directory,"dirap",0,0,0);
    AddHistogram(directory,"l0pt",4,0,200);
    AddHistogram(directory,"l0eta",0,0,0);
    AddHistogram(directory,"l0riso",0,0,0);
    AddHistogram(directory,"l1pt",4,0,200);
    AddHistogram(directory,"l1eta",0,0,0);
    AddHistogram(directory,"l1riso",0,0,0);
    AddHistogram(directory,"lldelR",0,0,0);
    AddHistogram(directory,"lldelphi",0,0,0);
    AddHistogram(directory,"nPV",0,0,0);
    AddHistogram(directory,"costhetaCS",0,0,0);
    directories.push_back(directory);
  }
  cout<<endl;
}
TString SetupAFB(int channel,int year){
  TString schannel=GetStringChannel((Channel)channel);
  TString syear=Form("%d",year);

  samples.clear();
  systematics.clear();
  directories.clear();

  SetupSamples("AFBAnalyzer",channel,year);
  SetupSystematics();
  SetupAFBDirectories(channel,year);

  cout<<"[SetupAFB] nsample: "<<samples.size()<<endl;
  cout<<"[SetupAFB] nsys: "<<systematics.size()<<endl;
  cout<<"[SetupAFB] ndirectories: "<<directories.size()<<endl;  
  return schannel+syear;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// Core functions////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
TH1* GetHist(TString filename,TString histname){
  TFile f(filename);
  TH1* hist=(TH1*)f.Get(histname);
  if(hist){
    hist->SetDirectory(0);
    return hist;
  }else{
    cout<<"no "<<histname<<" in "<<filename<<endl;
    return NULL;
  }
}
vector<TH1*> GetHists(TString datahistname,TString dyhistname,TString bghistname){
  vector<TH1*> hists;
  for(int i=0;i<samples.size();i++){
    TH1* hist=NULL;
    TString histname;
    if(samples[i].type==SampleType::DATA) histname=datahistname;
    else if(samples[i].type==SampleType::DY) histname=dyhistname;
    else if(samples[i].type==SampleType::TAU) histname=bghistname(0,bghistname.Last('/')+1)+"tau_"+bghistname(bghistname.Last('/')+1,bghistname.Length());
    else histname=bghistname;
    for(int j=0;j<samples[i].files.size();j++){
      if(!hist) hist=GetHist(samples[i].files[j],histname);
      else hist->Add(GetHist(samples[i].files[j],histname));
    }
    if(hist){
      hist->SetName(samples[i].name);
      hist->SetTitle(dyhistname);
      hist->GetXaxis()->SetTitle(dyhistname);
      hist->SetLineColor(samples[i].colorcode);
      hist->SetFillColor(samples[i].colorcode);
      hists.push_back(hist);
    }
  }
  return hists;
}
vector<TH1*> GetHists(TString histname){
  return GetHists(histname,histname,histname);
}

THStack *GetStack(vector<TH1*> hists){
  THStack* hstack=new THStack;
  for(int i=(int)hists.size()-1;i>0;i--){
    hstack->Add(hists.at(i),"HIST");
  }
  return hstack;
}
TH1* GetMCHist(const vector<TH1*>& hists){
  TH1* hist=(TH1*)hists.at(1)->Clone("mchist");
  for(int i=2;i<(int)hists.size();i++) hist->Add(hists.at(i));
  return hist;
}
TLegend* GetLegend(vector<TH1*> hists){
  TLegend* legend=new TLegend(0.67,0.4,0.89,0.88);
  for(int i=0;i<(int)hists.size();i++){
    TString option="f";
    if(i==0) option="lp";
    legend->AddEntry(hists.at(i),hists.at(i)->GetName(),option);
  }
  legend->SetBorderSize(0);
  return legend;
}
TH1* GetSysHistMax(TH1* central,vector<TH1*> variations){
  TH1* syshist=(TH1*)central->Clone("sys");
  for(int i=1;i<syshist->GetNbinsX()+1;i++) syshist->SetBinError(i,0);
  for(int i=0;i<(int)variations.size();i++){
    for(int j=0;j<syshist->GetNbinsX()+1;j++){
      double diff=fabs(syshist->GetBinContent(j)-variations.at(i)->GetBinContent(j));
      if(diff>syshist->GetBinError(j)) syshist->SetBinError(j,diff);
    }
  }
  return syshist;
}
TH1* GetSysHistMax(TH1* central,TH1* variation1,TH1* variation2=NULL,TH1* variation3=NULL,TH1* variation4=NULL,TH1* variation5=NULL,TH1* variation6=NULL,TH1* variation7=NULL,TH1* variation8=NULL,TH1* variation9=NULL){
  vector<TH1*> variations;
  if(variation1) variations.push_back(variation1);
  if(variation2) variations.push_back(variation2);
  if(variation3) variations.push_back(variation3);
  if(variation4) variations.push_back(variation4);
  if(variation5) variations.push_back(variation5);
  if(variation6) variations.push_back(variation6);
  if(variation7) variations.push_back(variation7);
  if(variation8) variations.push_back(variation8);
  if(variation9) variations.push_back(variation9);
  return GetSysHistMax(central,variations);
}    
int AddError(TH1* hist,TH1* sys){
  for(int i=1;i<hist->GetNbinsX()+1;i++){
    if(fabs(hist->GetBinContent(i)-sys->GetBinContent(i))*10000>fabs(hist->GetBinContent(i))){
      //if(hist->GetBinContent(i)!=sys->GetBinContent(i)){
      cout<<"[AddError] sys hist is wrong"<<endl;
      cout.precision(20);
      cout<<i<<" "<<hist->GetBinContent(i)<<" "<<sys->GetBinContent(i)<<" "<<fabs(hist->GetBinContent(i)-sys->GetBinContent(i))<<endl;
      return -1;
    }
  }
  for(int i=1;i<hist->GetNbinsX()+1;i++){
    double err1=hist->GetBinError(i);
    double err2=sys->GetBinError(i);
    hist->SetBinError(i,sqrt(err1*err1+err2*err2));
  }
  return 1;
} 
TCanvas* GetCompareBGSub(vector<TH1*> hists,int rebin=0,double xmin=0,double xmax=0,bool setlog=0){
  if(rebin){
    for(int i=0;i<(int)hists.size();i++) hists.at(i)->Rebin(rebin);
  }
  if(xmin||xmax){
    for(int i=0;i<(int)hists.size();i++) hists.at(i)->GetXaxis()->SetRangeUser(xmin,xmax);
  }
  TCanvas* c1=new TCanvas(hists.at(1)->GetTitle(),hists.at(1)->GetTitle(),800,800);
  c1->Divide(1,2);
  c1->cd(1);
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  TH1* hdata=(TH1*)hists.at(0)->Clone("data_bgsub");
  for(int i=2;i<hists.size();i++) hdata->Add(hists.at(i),-1.);
  hdata->Draw("e");
  hdata->GetXaxis()->SetLabelSize(0);
  hdata->GetXaxis()->SetTitle("");
  hdata->SetStats(0);
  hdata->SetMarkerStyle(20);
  hdata->SetMarkerSize(0.7);

  TH1* mchist=hists.at(1);
  mchist->Draw("same hist");

  vector<TH1*> hists_bgsub;
  hists_bgsub.push_back(hdata);
  hists_bgsub.push_back(mchist);
  TLegend* legend=GetLegend(hists_bgsub);
  legend->Draw();

  if(hdata->GetMaximum()<mchist->GetMaximum()) hdata->GetYaxis()->SetRangeUser(0,mchist->GetMaximum()*1.1);
  if(setlog){
    if(hdata->GetMinimum()==0) hdata->SetMinimum(1000);
    gPad->SetLogy();
  }
  hdata->Draw("same a e");

  c1->cd(2);
  gPad->SetPad(0,0,1,0.35);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.2);
  gPad->SetGridx();gPad->SetGridy();
  TH1* ratio=(TH1*)hdata->Clone("ratio");
  ratio->SetTitle("");
  ratio->SetStats(0);
  ratio->Divide(mchist);
  ratio->Draw();
  ratio->GetYaxis()->SetTitle("Data/Simulation");
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.5);
  ratio->GetYaxis()->SetRangeUser(0.8,1.2);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTitle(hdata->GetTitle());
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetLabelSize(0.1);
  TLine *line=new TLine(xmin?xmin:ratio->GetXaxis()->GetXmin(),1,xmax?xmax:ratio->GetXaxis()->GetXmax(),1);
  line->SetLineColor(2);
  line->Draw();
  ratio->Draw("same");

  return c1;
}
TCanvas* GetCompareStack(vector<TH1*> hists,TH1* sys=NULL,int rebin=0,double xmin=0,double xmax=0,bool setlog=0){
  if(rebin){
    if(sys) sys->Rebin(rebin);
    for(int i=0;i<(int)hists.size();i++) hists.at(i)->Rebin(rebin);
  }
  if(xmin||xmax){
    if(sys) sys->GetXaxis()->SetRangeUser(xmin,xmax);
    for(int i=0;i<(int)hists.size();i++) hists.at(i)->GetXaxis()->SetRangeUser(xmin,xmax);
  }
  TCanvas* c1=new TCanvas(hists.at(1)->GetTitle(),hists.at(1)->GetTitle(),800,800);
  c1->Divide(1,2);
  c1->cd(1);
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);
  hists.at(0)->Draw("e");
  hists.at(0)->GetXaxis()->SetLabelSize(0);
  hists.at(0)->GetXaxis()->SetTitle("");
  hists.at(0)->SetStats(0);
  hists.at(0)->SetMarkerStyle(20);
  hists.at(0)->SetMarkerSize(0.7);
  THStack* hstack=GetStack(hists);
  hstack->Draw("same");

  TH1* mchist=GetMCHist(hists);
  TH1* mchistsys=(TH1*)mchist->Clone("mchistsys");
  if(sys) AddError(mchistsys,sys);
  mchistsys->SetFillStyle(3002);
  mchistsys->SetFillColor(4);
  mchistsys->Draw("same e2");

  TLegend* legend=GetLegend(hists);
  legend->Draw();

  if(hists.at(0)->GetMaximum()<mchist->GetMaximum()) hists.at(0)->GetYaxis()->SetRangeUser(0,mchist->GetMaximum()*1.1);
  if(setlog){
    if(hists.at(0)->GetMinimum()==0) hists.at(0)->SetMinimum(1000);
    gPad->SetLogy();
  }
  hists.at(0)->Draw("same a e");

  c1->cd(2);
  gPad->SetPad(0,0,1,0.35);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.2);
  gPad->SetGridx();gPad->SetGridy();
  TH1* ratio=(TH1*)hists.at(0)->Clone("ratio");
  ratio->SetTitle("");
  ratio->SetStats(0);
  ratio->Divide(mchist);
  ratio->Draw();
  ratio->GetYaxis()->SetTitle("Data/Simulation");
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.5);
  ratio->GetYaxis()->SetRangeUser(0.8,1.2);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTitle(hists.at(0)->GetTitle());
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetLabelSize(0.1);
  TLine *line=new TLine(xmin?xmin:ratio->GetXaxis()->GetXmin(),1,xmax?xmax:ratio->GetXaxis()->GetXmax(),1);
  line->SetLineColor(2);
  line->Draw();
  if(sys){
    TH1* ratiosys=(TH1*)sys->Clone("ratiosys");
    for(int i=1;i<ratiosys->GetNbinsX()+1;i++){
      double y=mchist->GetBinContent(i);
      if(y==0) continue;
      ratiosys->SetBinContent(i,ratiosys->GetBinContent(i)/y);
      ratiosys->SetBinError(i,ratiosys->GetBinError(i)/y);  
    }
    ratiosys->SetFillStyle(3002);
    ratiosys->SetFillColor(4);
    ratiosys->Draw("same p e2");
    TLegend* syslegend=new TLegend(0.6,0.75,0.89,0.95);
    syslegend->AddEntry(ratio,"Data/Simulation (Stat.)","lp");
    syslegend->AddEntry(ratiosys,"Syst.","f");
    syslegend->Draw();
  }
  ratio->Draw("same");

  return c1;
}
TCanvas* GetCompareStack(TString histname,int sysbit=0,int rebin=0,double xmin=0,double xmax=0,bool setlog=0){
  vector<TH1*> hists_central=GetHists(histname,histname,histname);
  TH1* central=GetMCHist(hists_central);
  vector<TH1*> syss;
  for(int i=0;i<systematics.size();i++){
    if((sysbit>>i)%2==1){
      cout<<"systype="<<systematics[i].type<<endl;
      if(systematics[i].type<1) continue;
      vector<TString> suffix;
      if(systematics[i].type==1) suffix.push_back("");
      else if(systematics[i].type==2){
	suffix.push_back("_up");
	suffix.push_back("_down");
      }else if(systematics[i].type>2){
	for(int j=0;j<systematics[i].type;j++) suffix.push_back(Form("%d",j));
      }
      vector<TH1*> variations;
      for(int j=0;j<systematics[i].type;j++){
	if((systematics[i].type==9&&j==4)||(systematics[i].type==9&&j==8)) continue;
	TString datahistname=histname+(systematics[i].exist_data?("_"+systematics[i].name+suffix[j]):"");
	TString dyhistname=histname+"_"+systematics[i].name+suffix[j];
	TString bghistname=histname+(systematics[i].exist_bg?("_"+systematics[i].name+suffix[j]):"");
	cout<<datahistname<<" "<<dyhistname<<" "<<bghistname<<endl;
	vector<TH1*> gethists=GetHists(datahistname,dyhistname,bghistname);
	variations.push_back(GetMCHist(gethists));
	for(int k=0;k<(int)gethists.size();k++) delete gethists.at(k);
      }
      syss.push_back(GetSysHistMax(central,variations));
      for(int j=0;j<(int)variations.size();j++) delete variations.at(j);
      cout<<systematics[i].name+": "<<variations.size()<<" variations"<<endl;
    }
  }
  TH1* sys_total=NULL;
  if(syss.size()>0){
    sys_total=(TH1*)central->Clone("sys");
    for(int i=0;i<sys_total->GetNbinsX()+2;i++) sys_total->SetBinError(i,0);
    for(int i=0;i<(int)syss.size();i++){
      AddError(sys_total,syss.at(i));
      delete syss.at(i);
    }
  }
  return GetCompareStack(hists_central,sys_total,rebin,xmin,xmax,setlog);
}
void SaveAll(){
  int dmax=directories.size();
  int smax=systematics.size();
  TCanvas* c=NULL;
  for(int id=0;id<dmax;id++){
    cout<<"mkdir -p plot/"+directories[id].name<<endl;
    system("mkdir -p plot/"+directories[id].name);
    int hmax=directories[id].histograms.size();
    for(int ih=0;ih<hmax;ih++){
      Histogram *histogram=&(directories[id].histograms[ih]);
      TString this_histname=directories[id].name+histogram->name;
      c=GetCompareStack(this_histname,0,histogram->rebin,histogram->xmin,histogram->xmax);
      c->SaveAs("plot/"+this_histname+".png");
      delete c;      
    }
    for(int is=0;is<smax;is++){
      cout<<"mkdir -p plot/"+directories[id].name+systematics[is].name<<endl;
      system("mkdir -p plot/"+directories[id].name+systematics[is].name);
      for(int ih=0;ih<hmax;ih++){
	Histogram *histogram=&(directories[id].histograms[ih]);
	TString this_histname=directories[id].name+histogram->name;
	if(systematics[is].type>1) c=GetCompareStack(this_histname,1<<is,histogram->rebin,histogram->xmin,histogram->xmax);
	else if(systematics[is].type==1) c=GetCompareStack(GetHists(this_histname+(systematics[is].exist_data?"_"+systematics[is].name:""),this_histname+"_"+systematics[is].name,this_histname+(systematics[is].exist_bg?"_"+systematics[is].name:"")),0,histogram->rebin,histogram->xmin,histogram->xmax);
	else c=GetCompareStack(this_histname,-systematics[is].type,histogram->rebin,histogram->xmin,histogram->xmax);
	c->SaveAs("plot/"+this_histname(0,this_histname.Last('/')+1)+systematics[is].name+this_histname(this_histname.Last('/'),this_histname.Length())+".png");
	delete c;
      }
    }
  }  
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// etc.//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void PrintKeys(TList* keys){
  for(int i=0;i<keys->GetSize();i++){
    TKey* key=(TKey*)keys->At(i);
    if(strcmp(key->GetClassName(),"TDirectoryFile")==0) PrintKeys(((TDirectoryFile*)key->ReadObj())->GetListOfKeys());   
    else cout<<key->GetMotherDir()->GetPath()<<" "<<key->GetName()<<"\n";
  }
}
void PrintHistList(int samplenum=1){
  TFile f(samples.at(samplenum).files.at(1));
  cout<<f.GetName()<<" "<<f.GetTitle()<<endl;
  PrintKeys(f.GetListOfKeys());
}
