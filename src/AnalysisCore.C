#include "AnalysisCore.h"

AnalysisCore::AnalysisCore(){

}

AnalysisCore::~AnalysisCore(){

  for(std::map< TString, TH1D* >::iterator mapit = maphist_TH1D.begin(); mapit!=maphist_TH1D.end(); mapit++){
    delete mapit->second;
  }
  maphist_TH1D.clear();

  outfile->Close();
}

void AnalysisCore::FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max){

  TH1D *this_hist = GetHist1D(histname);
  if( !this_hist ){
    this_hist = new TH1D(histname, "", n_bin, x_min, x_max);
    maphist_TH1D[histname] = this_hist;
  }

  this_hist->Fill(value, weight);

}

TH1D* AnalysisCore::GetHist1D(TString histname){

  TH1D* h = NULL;
  std::map<TString, TH1D*>::iterator mapit = maphist_TH1D.find(histname);
  if(mapit != maphist_TH1D.end()) return mapit->second;

  return h;

}

void AnalysisCore::WriteHist1D(){

  outfile->cd();
  for(std::map< TString, TH1D* >::iterator mapit = maphist_TH1D.begin(); mapit!=maphist_TH1D.end(); mapit++){
    mapit->second->Write();
  }

}

void AnalysisCore::WriteHist(){

  WriteHist1D();

}


