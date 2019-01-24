#include "plot.cc"
/*
This is functions for Z boson pT correction.
You should use dileptonPt-dileptonRapidity 2D histogram in SMPValidation ( ex. muon2017/OS_Z/dipty)
and apply the SF in generator level iteratively.
Lastly, you can apply normalization SF to make rapidity distribution remain same.

Usage: This is made for 2017 SMPValidation code. For 2016, you should change paths and histogram name below.
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
SaveZPtWeight()
.q
4. Iterate 2. and 3. enough. (Actually in this stage, you don't need to submit all samples. It is OK with just DY sample)
5. Make histograms for genlevel. (You shouldn't use skimmed tree)
python python/SKFlat.py -a SMPValidation -y 2017 -i DYJets -n 100
6. Save normalization correction 
root
.L zptcor.cc
SaveZPtWeight(1)
.q
*/
TH1* ExtractZPtWeight(TString channelname){
  Setup(channelname);
  vector<TH1*> hists=GetHists(filedir,channelname+"/OS_Z/dipty");
  TCanvas* cc=GetCompareBGSub(hists);
  TH1* ratio=(TH1*)cc->GetPad(2)->GetPrimitive("ratio");
  ratio->SetDirectory(0);
  ratio->GetXaxis()->UnZoom();
  ratio->GetYaxis()->UnZoom();
  return ratio;
}
TH1* ExtractNormWeight(TString channelname){
  Setup(channelname);
  TH2D* genZrap=(TH2D*)GetHist(filedir+"/SMPValidation_DYJets.root",channelname+"gen/genZrap");
  TH2D* genZrap_nozptcor=(TH2D*)GetHist(filedir+"/SMPValidation_DYJets.root",channelname+"gen/genZrap_nozptcor");
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

void SaveZPtWeight(bool isnorm=0){
  TH1* (*func)(TString);
  if(isnorm) func=ExtractNormWeight;
  else func=ExtractZPtWeight;
  TH1* zptcor_muon=func("muon2017");
  TH1* zptcor_electron=func("electron2017");
  
  TFile f("data/v949cand2_2/2017/ZPt/ZPtWeight.root","update");
  int iter=0;
  TList* keys=f.GetListOfKeys();
  for(int i=0;i<keys->GetSize();i++){
    TString name=keys->At(i)->GetName();
    TString iterstring=name(name.Index("iter")+4,1);
    int this_iter=iterstring.Atoi();
    if(this_iter>=iter) iter=this_iter+1;
  }
  zptcor_muon->SetName(Form("muon2017_iter%d",iter));
  if(isnorm) zptcor_muon->SetName("muon2017_norm");
  zptcor_muon->Write();
  zptcor_electron->SetName(Form("electron2017_iter%d",iter));
  if(isnorm) zptcor_electron->SetName("electron2017_norm");
  zptcor_electron->Write();
}

