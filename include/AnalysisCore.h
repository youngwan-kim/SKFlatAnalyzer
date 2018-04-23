#ifndef AnalysisCore_h
#define AnalysisCore_h

#include "SKFlatNtuple.C"

class AnalysisCore: public SKFlatNtuple {

public:

  AnalysisCore();
  ~AnalysisCore();

  virtual void executeEvent(){

  };

  std::map< TString, TH1D* > maphist_TH1D;
  void FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max);
  TH1D* GetHist1D(TString histname);
  void WriteHist1D(); 


  void WriteHist();

  TFile *outfile;
  void SetOutfilePath(TString outname){
    outfile = new TFile(outname,"RECREATE");
  };

};



#endif

