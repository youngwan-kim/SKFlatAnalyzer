#ifndef AnalyzerCore_h
#define AnalyzerCore_h

#include "TLorentzVector.h"
#include "TString.h"

#include "SKFlatNtuple.C"
#include "Event.h"
#include "Particle.h"
#include "Lepton.h"
#include "Muon.h"
#include "Electron.h"
#include "Jet.h"

#define M_Z 91.1876

class AnalyzerCore: public SKFlatNtuple {

public:

  AnalyzerCore();
  ~AnalyzerCore();

  virtual void executeEvent(){

  };

  //==== Get objects

  Event GetEvent();

  std::vector<Muon> GetAllMuons();
  std::vector<Muon> GetMuons(TString id, double ptmin=-999., double fetamax=999.);

  std::vector<Electron> GetAllElectrons();
  std::vector<Electron> GetElectrons(TString id, double ptmin=-999., double fetamax=999.);

  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon> muons);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron> electrons);

  std::vector<Jet> GetAllJets();

  //==== Tools
  bool IsOnZ(double m, double width);

  //==== Plotting

  std::map< TString, TH1D* > maphist_TH1D;
  TH1D* GetHist1D(TString histname);
  void FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max);

  std::map< TString, std::map<TString, TH1D*> > JSmaphist_TH1D;
  TH1D* JSGetHist1D(TString suffix, TString histname);
  void JSFillHist(TString suffix, TString histname, double value, double weight, int n_bin, double x_min, double x_max);

  void WriteHist();

  //==== Output rootfile

  TFile *outfile;
  void SetOutfilePath(TString outname){
    outfile = new TFile(outname,"RECREATE");
  };

};



#endif

