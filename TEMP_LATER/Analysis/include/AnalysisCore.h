#ifndef AnalysisCore_h
#define AnalysisCore_h

#include "TLorentzVector.h"
#include "TString.h"
#include "TH1.h"

#include "SKFlatNtuple.h"
#include "Event.h"
#include "Particle.h"
#include "Lepton.h"
#include "Muon.h"

class AnalysisCore: public SKFlatNtuple {

public:

  AnalysisCore();
  ~AnalysisCore();

  Event GetEvent();
  std::vector<Muon> GetAllMuons();
  std::vector<Muon> GetMuons(TString id, double ptmin=-999., double fetamax=999.);

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

