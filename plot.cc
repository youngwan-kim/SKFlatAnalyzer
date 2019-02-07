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
  bool setlog;
  TString option;
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
void AddHistogram(Directory& directory,TString name_,int rebin_,double xmin_,double xmax_,bool setlog_=false,TString option_=""){
  Histogram histogram;
  histogram.name=name_;
  histogram.rebin=rebin_;
  histogram.xmin=xmin_;
  histogram.xmax=xmax_;
  histogram.setlog=setlog_;
  histogram.option=option_;
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
  const int ybinnum=6;
  double yrange[ybinnum+1]={0.0,0.4,0.8,1.2,1.6,2.0,2.4};
  const int mbinnum=12;
  double massrange[mbinnum+1]={60,70,78,84,87,89,91,93,95,98,104,112,120};
  vector<TString> region;
  region.push_back(Form("OS_m%.0fto%.0f",massrange[0],massrange[mbinnum]));
  for(int iy=0;iy<ybinnum;iy++){
    region.push_back(Form("OS_m%.0fto%.0f_y%.1fto%.1f",massrange[0],massrange[mbinnum],yrange[iy],yrange[iy+1]));
    for(int im=0;im<mbinnum;im++){
      //region.push_back(Form("OS_m%.0fto%.0f_y%.1fto%.1f",massrange[im],massrange[im+1],yrange[iy],yrange[iy+1]));
    }
  }
  for(int i=0;i<region.size();i++){
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
    if(region[i].Contains("m60to120")) AddHistogram(directory,"AFB",0,0,0,false,"AFB");
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
vector<TH1*> GetHists(TString datahistname,TString dyhistname,TString bghistname,TString option){
  vector<TH1*> hists;
  if(option.Contains("AFB")){
    TString datahistname_forward=datahistname;
    TString dyhistname_forward=dyhistname;
    TString bghistname_forward=bghistname;
    datahistname_forward.ReplaceAll("AFB","forward");
    dyhistname_forward.ReplaceAll("AFB","forward");
    bghistname_forward.ReplaceAll("AFB","forward");
    TString datahistname_backward=datahistname;
    TString dyhistname_backward=dyhistname;
    TString bghistname_backward=bghistname;
    datahistname_backward.ReplaceAll("AFB","backward");
    dyhistname_backward.ReplaceAll("AFB","backward");
    bghistname_backward.ReplaceAll("AFB","backward");
    hists=GetHists(datahistname_forward,dyhistname_forward,bghistname_forward,"");
    vector<TH1*> hists_backward=GetHists(datahistname_backward,dyhistname_backward,bghistname_backward,"");
    hists.insert(hists.end(),hists_backward.begin(),hists_backward.end());
  }
  else{
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
      }
      hists.push_back(hist);
    }
  }
  return hists;
}
vector<TH1*> GetHists(TString histname,TString option){
  return GetHists(histname,histname,histname,option);
}
TH1* GetHistAFB(TH1* hist_forward,TH1* hist_backward){
  TH1* hist=(TH1*)hist_forward->Clone("afb");
  hist->Reset();
  for(int i=0;i<hist->GetNbinsX()+2;i++){
    double valf=hist_forward->GetBinContent(i);
    double ef=hist_forward->GetBinError(i);
    double valb=hist_backward->GetBinContent(i);
    double eb=hist_backward->GetBinError(i);
    hist->SetBinContent(i,(valf-valb)/(valf+valb));
    hist->SetBinError(i,2*sqrt(ef*ef*valb*valb+eb*eb*valf*valf)/pow(valf+valb,2));
  }
  return hist;
}
TH1* GetHMC(const vector<TH1*>& hists,TString option=""){
  if(option.Contains("AFB")){
    TH1* hist_forward=hists.at(1);
    TH1* hist_backward=hists.at(samples.size()+1);
    TH1* hist=GetHistAFB(hist_forward,hist_backward);
    hist->SetName("DY");
    return hist;        
  }else if(option.Contains("stack")){
    THStack* hstack=new THStack;
    for(int i=(int)hists.size()-1;i>0;i--){
      if(hists.at(i)) hstack->Add(hists.at(i),"HIST");
    }
    return (TH1*)hstack;
  }else if(option.Contains("BGSub")) return (TH1*)hists.at(1)->Clone();
  else{
    TH1* hist=(TH1*)hists.at(1)->Clone("hmc");
    for(int i=2;i<(int)hists.size();i++) if(hists.at(i)) hist->Add(hists.at(i));
    return hist;
  }
}
TH1* GetHMC(THStack* hstack){
  TList* list=hstack->GetHists();
  TH1* hist=(TH1*)list->At(0)->Clone("hmc");
  for(int i=1;i<list->GetSize();i++){
    hist->Add((TH1*)list->At(i));
  }
  return hist;
}
TH1* GetHData(const vector<TH1*>& hists,TString option=""){
  if(option.Contains("AFB")){
    vector<TH1*> hists_forward,hists_backward;
    hists_forward.insert(hists_forward.begin(),hists.begin(),hists.begin()+samples.size());
    hists_backward.insert(hists_backward.begin(),hists.begin()+samples.size(),hists.end());
    TH1* hist_forward=GetHData(hists_forward,"BGSub");
    TH1* hist_backward=GetHData(hists_backward,"BGSub");
    TH1* hist=GetHistAFB(hist_forward,hist_backward);
    delete hist_forward;delete hist_backward;
    hist->SetName("data");
    return hist;    
  }else if(option.Contains("BGSub")){
    TH1* hist=(TH1*)hists.at(0)->Clone("hdata");
    for(int i=2;i<(int)samples.size();i++) if(hists.at(i)) hist->Add(hists.at(i),-1.);
    return hist;
  }
  else return (TH1*)hists.at(0)->Clone();
}
TLegend* GetLegend(const vector<TH1*>& hists,TString option=""){
  int histssize=hists.size();
  if(option.Contains("BGSub")) histssize=2;
  TLegend* legend=new TLegend(0.67,0.88-histssize*0.07,0.89,0.88);
  for(int i=0;i<histssize;i++){
    TString att="f";
    if(i==0) att="lp";
    legend->AddEntry(hists.at(i),hists.at(i)->GetName(),att);
  }
  legend->SetBorderSize(0);
  return legend;
}
TLegend* GetLegend(TH1* h1,TH1* h2){
  vector<TH1*> hists;
  hists.push_back(h1);
  if(strstr(h2->ClassName(),"THStack")){
    TList* list=((THStack*)h2)->GetHists();
    for(int i=list->GetSize()-1;i>=0;i--){
      hists.push_back((TH1*)list->At(i));
    }
  }else hists.push_back(h2);
  return GetLegend(hists,"");
}
TH1* GetSysHistMax(TH1* central,const vector<TH1*>& variations){
  if(strstr(central->ClassName(),"THStack")) central=GetHMC((THStack*)central);
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
    if(fabs(hist->GetBinContent(i)-sys->GetBinContent(i))*1000000>fabs(hist->GetBinContent(i))){
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
TCanvas* GetCompare(TH1* h1,TH1* h1sys,TH1* h2,TH1* h2sys,int rebin,double xmin,double xmax,bool setlog){
  THStack *hstack=NULL;
  if(strstr(h2->ClassName(),"THStack")!=NULL){
    hstack=(THStack*)h2;
    h2=GetHMC(hstack);
  }
  if(rebin){
    h1->Rebin(rebin);
    if(h1sys) h1sys->Rebin(rebin);
    if(hstack){
      TList* list=hstack->GetHists();
      for(int i=0;i<list->GetSize();i++){
	((TH1*)list->At(i))->Rebin(rebin);
      }
    }
    h2->Rebin(rebin);
    if(h2sys) h2sys->Rebin(rebin);
  }
  if(xmin||xmax){
    h1->GetXaxis()->SetRangeUser(xmin,xmax);
  }

  h1->SetStats(0);
  h1->SetMarkerStyle(20);
  h1->SetMarkerSize(0.7);
  TH1 *h1total=NULL,*h2total=NULL;
  if(h1sys){
    h1total=(TH1*)h1->Clone("h1total");
    AddError(h1total,h1sys);
  }else{
    h1total=h1;
  }
  if(h2sys){
    h2total=(TH1*)h2->Clone("h2total");
    AddError(h2total,h2sys);
  }else{
    h2total=h2;
  }

  TCanvas* c1=new TCanvas(h2->GetTitle(),h2->GetTitle(),800,800);
  c1->Divide(1,2);
  c1->cd(1);
  gPad->SetPad(0,0.35,1,1);
  gPad->SetBottomMargin(0.02);

  h1total->Draw("e");
  h1total->GetXaxis()->SetLabelSize(0);
  h1total->GetXaxis()->SetTitle("");
  if(h1sys) h1->Draw("same e1");

  if(hstack){
    hstack->Draw("same");
    h2total->SetFillColor(4);
  }
  h2total->SetFillStyle(3002);
  h2total->Draw("same e2");
  if(h2sys) h2->Draw("same e2");

  TLegend* legend=GetLegend(h1,hstack?(TH1*)hstack:h2);
  legend->Draw();

  if(setlog){
    gPad->SetLogy();
  }else{
    double maximum=h1total->GetMaximum()>h2total->GetMaximum()?h1total->GetMaximum():h2total->GetMaximum();
    double minimum=h1total->GetMinimum()<h2total->GetMinimum()?h1total->GetMinimum():h2total->GetMinimum();
    double range=fabs(maximum-minimum);
    h1total->GetYaxis()->SetRangeUser(minimum?minimum-0.1*range:0,maximum+0.1*range);
  }
  h1total->Draw("same a e");

  c1->cd(2);
  gPad->SetPad(0,0,1,0.35);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.2);
  gPad->SetGridx();gPad->SetGridy();

  TH1* ratio=(TH1*)h1->Clone("ratio");
  ratio->SetTitle("");
  ratio->SetStats(0);
  ratio->Divide(h2);
  ratio->Draw();
  ratio->GetYaxis()->SetTitle("Data/Simulation");
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.5);
  ratio->GetYaxis()->SetRangeUser(0.8,1.2);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTitle(h2->GetTitle());
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetLabelSize(0.1);
  TLine *line=new TLine(xmin?xmin:ratio->GetXaxis()->GetXmin(),1,xmax?xmax:ratio->GetXaxis()->GetXmax(),1);
  line->SetLineColor(2);
  line->Draw();
  if(h1sys||h2sys){
    TH1* ratiosys=(TH1*)h2->Clone("ratiosys");
    for(int i=1;i<ratiosys->GetNbinsX()+1;i++){
      double yh1sys=h1sys?h1sys->GetBinContent(i):0;
      double eyh1sys=(yh1sys==0)?0:h1sys->GetBinError(i)/yh1sys;
      double yh2sys=h2sys?h2sys->GetBinContent(i):0;
      double eyh2sys=yh2sys==0?0:h2sys->GetBinError(i)/yh2sys;
      ratiosys->SetBinContent(i,1);
      ratiosys->SetBinError(i,sqrt(eyh1sys*eyh1sys+eyh2sys*eyh2sys));  
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
TCanvas* GetCompare(TString histname,int sysbit=0,int rebin=0,double xmin=0,double xmax=0,bool setlog=false,TString option=""){
  if(option.Contains("AFB")) option+=" BGSub";
  vector<TH1*> hists_central=GetHists(histname,histname,histname,option);
  TH1* hdata_central=GetHData(hists_central,option);
  TH1* hmc_central=GetHMC(hists_central,option);
  THStack* hstack_central=option.Contains("BGSub")?NULL:(THStack*)GetHMC(hists_central,"stack");
  vector<TH1*> hdata_syss,hmc_syss;
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
      vector<TH1*> hdata_variations;
      vector<TH1*> hmc_variations;
      for(int j=0;j<systematics[i].type;j++){
	if((systematics[i].type==9&&j==4)||(systematics[i].type==9&&j==8)) continue;
	TString datahistname=histname+(systematics[i].exist_data?("_"+systematics[i].name+suffix[j]):"");
	TString dyhistname=histname+"_"+systematics[i].name+suffix[j];
	TString bghistname=histname+(systematics[i].exist_bg?("_"+systematics[i].name+suffix[j]):"");
	cout<<datahistname<<" "<<dyhistname<<" "<<bghistname<<endl;
	vector<TH1*> gethists=GetHists(datahistname,dyhistname,bghistname,option);
	hdata_variations.push_back(GetHData(gethists,option));
	hmc_variations.push_back(GetHMC(gethists,option));
	for(int k=0;k<(int)gethists.size();k++) delete gethists.at(k);
      }
      hdata_syss.push_back(GetSysHistMax(hdata_central,hdata_variations));
      hmc_syss.push_back(GetSysHistMax(hmc_central,hmc_variations));
      for(int j=0;j<(int)hdata_variations.size();j++){
	delete hdata_variations.at(j);
	delete hmc_variations.at(j);
      }
      cout<<systematics[i].name+": "<<hdata_variations.size()<<" variations"<<endl;
    }
  }
  TH1 *hdata_sys=NULL,*hmc_sys=NULL;
  if(hdata_syss.size()>0){
    hdata_sys=(TH1*)hdata_central->Clone("hdata_sys");
    hmc_sys=(TH1*)hmc_central->Clone("hmc_sys");
    for(int i=0;i<hdata_sys->GetNbinsX()+2;i++){
      hdata_sys->SetBinError(i,0);
      hmc_sys->SetBinError(i,0);
    }
    for(int i=0;i<(int)hdata_syss.size();i++){
      AddError(hdata_sys,hdata_syss.at(i));
      AddError(hmc_sys,hmc_syss.at(i));
      delete hdata_syss.at(i);delete hmc_syss.at(i);
    }
  }
  TCanvas* c1=GetCompare(hdata_central,hdata_sys,hstack_central?(TH1*)hstack_central:hmc_central,hmc_sys,rebin,xmin,xmax,setlog);
  if(option.Contains("AFB")){
    ((TH1*)c1->GetPad(2)->GetPrimitive("ratio"))->GetYaxis()->SetRangeUser(0,2);
  }
  return c1;
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
      c=GetCompare(this_histname,0,histogram->rebin,histogram->xmin,histogram->xmax,histogram->setlog,histogram->option);
      c->SaveAs("plot/"+this_histname+".png");
      delete c;      
    }
    for(int is=0;is<smax;is++){
      cout<<"mkdir -p plot/"+directories[id].name+systematics[is].name<<endl;
      system("mkdir -p plot/"+directories[id].name+systematics[is].name);
      for(int ih=0;ih<hmax;ih++){
	Histogram *histogram=&(directories[id].histograms[ih]);
	TString this_histname=directories[id].name+histogram->name;
	if(systematics[is].type>=1) c=GetCompare(this_histname,1<<is,histogram->rebin,histogram->xmin,histogram->xmax,histogram->setlog,histogram->option);
	else c=GetCompare(this_histname,-systematics[is].type,histogram->rebin,histogram->xmin,histogram->xmax,histogram->setlog,histogram->option);
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
