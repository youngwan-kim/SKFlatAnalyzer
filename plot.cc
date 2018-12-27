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

TString filedir="/data7/Users/hsseo/SKFlatOutput/v949cand2_2/SMPValidation/2017/";

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
TString setting;
vector<Sample> samples;
vector<Systematic> systematics;
void AddSample(TString name_,int type_,int colorcode_,vector<TString> files_){
  Sample sample;
  sample.name=name_;
  sample.type=type_;
  sample.colorcode=colorcode_;
  sample.files=files_;
  cout<<" [AddSample] "<<sample.name<<" "<<sample.type<<" "<<sample.colorcode<<endl;
  for(int i=0;i<sample.files.size();i++)
    cout<<"   "<<sample.files.at(i)<<endl;
    
  samples.push_back(sample);
}
void AddSample(TString name_,int type_,int colorcode_,TString file1,TString file2="",TString file3="",TString file4="",TString file5=""){
  vector<TString> files;
  if(file1!="") files.push_back(file1);
  if(file2!="") files.push_back(file2);
  if(file3!="") files.push_back(file3);
  if(file4!="") files.push_back(file4);
  if(file5!="") files.push_back(file5);
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
void Setup(TString key){
  setting=key;
  samples.clear();
  systematics.clear();
  cout<<"[Setup] setting="<<setting<<endl;
  if(key=="muon2017"){
    AddSample("data",0,1,"DATA/SMPValidation_SkimTree_SMP_DoubleMuon_B.root","DATA/SMPValidation_SkimTree_SMP_DoubleMuon_C.root","DATA/SMPValidation_SkimTree_SMP_DoubleMuon_D.root","DATA/SMPValidation_SkimTree_SMP_DoubleMuon_E.root","DATA/SMPValidation_SkimTree_SMP_DoubleMuon_F.root");
    AddSample("#gamma*/Z#rightarrow#mu#mu",1,2,"SMPValidation_SkimTree_SMP_DYJets.root");
  }else if(key=="electron2017"){
    AddSample("data",0,1,"DATA/SMPValidation_SkimTree_SMP_DoubleEG_B.root","DATA/SMPValidation_SkimTree_SMP_DoubleEG_C.root","DATA/SMPValidation_SkimTree_SMP_DoubleEG_D.root","DATA/SMPValidation_SkimTree_SMP_DoubleEG_E.root","DATA/SMPValidation_SkimTree_SMP_DoubleEG_F.root");
    AddSample("#gamma*/Z#rightarrowee",1,2,"SMPValidation_SkimTree_SMP_DYJets.root");
  }
  AddSample("#gamma*/Z#rightarrow#tau#tau",2,3,"SMPValidation_SkimTree_SMP_DYJets.root");
  AddSample("Diboson",3,4,"SMPValidation_SkimTree_SMP_WW_pythia.root","SMPValidation_SkimTree_SMP_WZ_pythia.root","SMPValidation_SkimTree_SMP_ZZ_pythia.root");
  AddSample("W",4,5,"SMPValidation_SkimTree_SMP_WJets_MG.root");
  AddSample("t#bar{t}",5,6,"SMPValidation_SkimTree_SMP_TTLL_powheg.root");
  
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
  AddSystematic("efficiencySF",-15,0,0);
  AddSystematic("totalsys",-255,0,0);

  cout<<"[Setup] nsample: "<<samples.size()<<endl;
  cout<<"[Setup] nsys: "<<systematics.size()<<endl;
}

void PrintKeys(TList* keys){
  for(int i=0;i<keys->GetSize();i++){
    TKey* key=(TKey*)keys->At(i);
    if(strcmp(key->GetClassName(),"TDirectoryFile")==0) PrintKeys(((TDirectoryFile*)key->ReadObj())->GetListOfKeys());   
    else cout<<key->GetMotherDir()->GetPath()<<" "<<key->GetName()<<"\n";
  }
}
void PrintHistList(int samplenum=1){
  TFile f(filedir+samples.at(samplenum).files.at(1));
  cout<<f.GetName()<<" "<<f.GetTitle()<<endl;
  PrintKeys(f.GetListOfKeys());
}
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
vector<TH1*> GetHists(TString dirname,TString datahistname,TString dyhistname,TString bghistname){
  vector<TH1*> hists;
  for(int i=0;i<samples.size();i++){
    TH1* hist=NULL;
    TString histname;
    if(samples[i].type==0) histname=datahistname;
    else if(samples[i].type==1) histname=dyhistname;
    else if(samples[i].type==2) histname=bghistname(0,bghistname.Last('/')+1)+"tau_"+bghistname(bghistname.Last('/')+1,bghistname.Length());
    else histname=bghistname;
    for(int j=0;j<samples[i].files.size();j++){
      if(!hist) hist=GetHist(filedir+samples[i].files[j],histname);
      else hist->Add(GetHist(filedir+samples[i].files[j],histname));
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
vector<TH1*> GetHists(TString dirname,TString histname){
  return GetHists(dirname,histname,histname,histname);
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
    if(fabs(hist->GetBinContent(i)-sys->GetBinContent(i))*10000>hist->GetBinContent(i)){
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
  vector<TH1*> hists_central=GetHists(filedir,histname,histname,histname);
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
	vector<TH1*> gethists=GetHists(filedir,datahistname,dyhistname,bghistname);
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
  vector<TString> histname;
  vector<int> histrebin;
  vector<double> histxmax,histxmin;
  TString channel[]={setting};
  for(int ichannel=0;ichannel<sizeof(channel)/sizeof(TString);ichannel++){
    TString region[]={"OS","OS_Z","OS_Z_y0.0to0.5","OS_Z_y0.5to1.0","OS_Z_y1.0to3.0","SS"};
    for(int iregion=0;iregion<sizeof(region)/sizeof(TString);iregion++){
      TString hname[12]={"dimass","dipt","dirap","l0pt","l0eta","l0riso","l1pt","l1eta","l1riso","lldelR","lldelphi","nPV"}; 
      int hrebin[12]={4,4,0,4,0,0,4,0,0,0,0,0};
      double hxmin[12]={80,0,0,0,0,0,0,0,0,0,0,0};
      double hxmax[12]={100,100,0,0,0,0,0,0,0,0,0,0};
      for(int ihist=0;ihist<sizeof(hname)/sizeof(TString);ihist++){
	histname.push_back(channel[ichannel]+"/"+region[iregion]+"/"+hname[ihist]);
	histrebin.push_back(iregion!=0&&ihist==0?0:hrebin[ihist]);
	histxmin.push_back(iregion==0&&ihist==0?0:hxmin[ihist]);
	histxmax.push_back(iregion==0&&ihist==0?0:hxmax[ihist]);
      }
    }
  }
  int nhist=histname.size();
  for(int i=0;i<systematics.size();i++) 
    for(int j=0;j<nhist;j++){
      TString this_histname=histname[j];
      cout<<"mkdir -p plot/"+this_histname(0,this_histname.Last('/')+1)+systematics[i].name<<endl;
      system("mkdir -p plot/"+this_histname(0,this_histname.Last('/')+1)+systematics[i].name);
    }
  TCanvas* c=NULL;
  for(int i=0;i<nhist;i++){
    TString this_histname=histname[i];
    cout<<this_histname<<" "<<histrebin[i]<<" "<<histxmin[i]<<" "<<histxmax[i]<<endl;
    c=GetCompareStack(this_histname,0,histrebin[i],histxmin[i],histxmax[i]);
    c->SaveAs("plot/"+this_histname+".png");
    delete c;
    for(int j=0;j<systematics.size();j++){
      int sysbit=0;
      TCanvas* c;
      if(systematics[j].type>1) c=GetCompareStack(this_histname,1<<j,histrebin[i],histxmin[i],histxmax[i]);
      else if(systematics[j].type==1) c=GetCompareStack(GetHists(filedir,this_histname+(systematics[j].exist_data?"_"+systematics[j].name:""),this_histname+"_"+systematics[j].name,this_histname+(systematics[j].exist_bg?"_"+systematics[j].name:"")),0,histrebin[i],histxmin[i],histxmax[i]);
      else c=GetCompareStack(this_histname,-systematics[j].type,histrebin[i],histxmin[i],histxmax[i]);
      c->SaveAs("plot/"+this_histname(0,this_histname.Last('/')+1)+systematics[j].name+this_histname(this_histname.Last('/'),this_histname.Length())+".png");
      delete c;
    }      
  }
}

