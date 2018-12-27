#include "plot.cc"
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

void SaveZPtWeight(){
  TH1* zptcor_muon=ExtractZPtWeight("muon2017");
  TH1* zptcor_electron=ExtractZPtWeight("electron2017");
  
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
  zptcor_muon->Write();
  zptcor_electron->SetName(Form("electron2017_iter%d",iter));
  zptcor_electron->Write();
}

