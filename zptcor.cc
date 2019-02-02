#include "plot.cc"
/*
This is functions for Z boson pT correction.
You should use dileptonPt-dileptonRapidity 2D histogram in SMPValidation ( ex. muon2017/OS_Z/dipty)
and apply the SF in generator level iteratively.
Lastly, you can apply normalization SF to make rapidity distribution remain same.

Usage: AutoIteration will do below
0. source setup.sh
1. Make empty root file for ZPt correction.
ex) root
TFile f("data/v949cand2_2/2017/ZPt/ZPtWeight.root","update");
.q
2. Make histograms with all samples
source SMPValidationSubmit.sh
3. Save first ZPt correction 
root
.L zptcor.cc
SaveZPtWeightALL(0)
.q
4. Iterate 2. and 3. enough. (Actually in this stage, you don't need to submit all samples. It is OK with just DY sample)
5. Make histograms for genlevel.
python python/SKFlat.py -a SMPValidation --skim SkimTree_GEN -y 2017 -i DYJets -n 100;
python python/SKFlat.py -a SMPValidation --skim SkimTree_GEN -y 2016 -i DYJets -n 100;
6. Save normalization correction 
root
.L zptcor.cc
SaveZPtWeightALL(1)
.q
*/
TH1* ExtractZPtWeight(int channel,int year){
  TString channelname=Setup(channel,year);
  vector<TH1*> hists=GetHists(filedir,channelname+"/OS_Z/dipty");
  TCanvas* cc=GetCompareBGSub(hists);
  TH1* ratio=(TH1*)cc->GetPad(2)->GetPrimitive("ratio");
  ratio->SetDirectory(0);
  ratio->GetXaxis()->UnZoom();
  ratio->GetYaxis()->UnZoom();
  return ratio;
}
TH1* ExtractNormWeight(int channel,int year){
  TString channelname=Setup(channel,year);
  TString syear=Form("%d",year);
  TH2D* genZrap=(TH2D*)GetHist(filedir+syear+"/SMPValidation_SkimTree_GEN_DYJets.root",channelname+"gen/genZrap");
  TH2D* genZrap_nozptcor=(TH2D*)GetHist(filedir+syear+"/SMPValidation_SkimTree_GEN_DYJets.root",channelname+"gen/genZrap_nozptcor");
  double ptbins[2]={0,400};
  double ybins[200]={};
  genZrap->GetXaxis()->GetLowEdge(ybins);
  ybins[genZrap->GetXaxis()->GetNbins()]=genZrap->GetXaxis()->GetXmax();
  
  TH2D* ratio=new TH2D("zptcor_norm","zptcor_norm",1,ptbins,genZrap->GetXaxis()->GetNbins(),ybins);
  for(int iy=0;iy<ratio->GetYaxis()->GetNbins()+2;iy++){
    if(genZrap->GetBinContent(iy)==0) continue;
    double norm=genZrap_nozptcor->GetBinContent(iy)/genZrap->GetBinContent(iy);
    for(int ix=0;ix<ratio->GetXaxis()->GetNbins()+2;ix++){
      ratio->SetBinContent(ix,iy,norm);
    }
  }
  ratio->SetDirectory(0);
  return (TH1*)ratio;
}

void SaveZPtWeight(int channel,int year,bool isnorm){
  TH1* (*func)(int,int);
  if(isnorm) func=ExtractNormWeight;
  else func=ExtractZPtWeight;
  TH1* zptcor=func(channel,year);
  TString zptfile=getenv("SKFlat_WD")+TString("/data/")+getenv("SKFlatV")+"/"+Form("%d",year)+"/ZPt/ZPtWeight.root";
  TFile f(zptfile,"update");
  int iter=0;
  TList* keys=f.GetListOfKeys();
  for(int i=0;i<keys->GetSize();i++){
    TString name=keys->At(i)->GetName();
    if(!name.Contains(GetStringChannel((Channel)channel))) continue;
    TString iterstring=name(name.Index("iter")+4,1);
    int this_iter=iterstring.Atoi();
    if(this_iter>=iter) iter=this_iter+1;
  }
  zptcor->SetName(Form("%s%d_iter%d",GetStringChannel((Channel)channel).Data(),year,iter));
  if(isnorm) zptcor->SetName(Form("%s%d_norm",GetStringChannel((Channel)channel).Data(),year));
  zptcor->Write();
}
void SaveZPtWeightAll(bool isnorm){
  SaveZPtWeight(Channel::MUON,2016,isnorm);
  SaveZPtWeight(Channel::ELECTRON,2016,isnorm);
  SaveZPtWeight(Channel::MUON,2017,isnorm);
  SaveZPtWeight(Channel::ELECTRON,2017,isnorm);
}
void AutoIteration(int iteration){
  //recreate
  TFile f2016(getenv("SKFlat_WD")+TString("/data/")+getenv("SKFlatV")+"/2016/ZPt/ZPtWeight.root","recreate");
  f2016.Close();
  TFile f2017(getenv("SKFlat_WD")+TString("/data/")+getenv("SKFlatV")+"/2017/ZPt/ZPtWeight.root","recreate");
  f2017.Close();
  
  //first iteration
  system("source SMPValidationSumit.sh");
  SaveZPtWeightAll(0);

  //remaining iterations
  for(int i=1;i<iteration;i++){
    system("python python/SKFlat.py -a SMPValidation -y 2017 -i DYJets -n 100 --skim SkimTree_SMP;");
    system("python python/SKFlat.py -a SMPValidation -y 2016 -i DYJets -n 100 --skim SkimTree_SMP;");
    SaveZPtWeightAll(0);
  }

  //For normalization 
  system("python python/SKFlat.py -a SMPValidation -y 2017 -i DYJets -n 100 --skim SkimTree_GEN;");
  system("python python/SKFlat.py -a SMPValidation -y 2016 -i DYJets -n 100 --skim SkimTree_GEN;");
  SaveZPtWeightAll(1);
}
    
