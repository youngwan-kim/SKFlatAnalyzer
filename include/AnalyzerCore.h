#ifndef AnalyzerCore_h
#define AnalyzerCore_h

#include "TLorentzVector.h"
#include "TString.h"
#include "TMath.h"

#include "SKFlatNtuple.C"
#include "MCCorrection.C"
#include "Event.h"
#include "Particle.h"
#include "Gen.h"
#include "Lepton.h"
#include "Muon.h"
#include "Electron.h"
#include "Jet.h"
#include "FatJet.h"

#include "AnalyzerParameter.h"

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

  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon>& muons);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron>& electrons);

  std::vector<Jet> GetAllJets();
  std::vector<Jet> GetJets(TString id, double ptmin=-999, double fetamax=999.);

  std::vector<FatJet> GetAllFatJets();
  std::vector<FatJet> GetFatJets(TString id, double ptmin=-999, double fetamax=999.);

  std::vector<Gen> GetGens();

  //==== Event Filters

  bool PassMETFilter();

  //==== Tools

  MCCorrection mccor;
  bool IsOnZ(double m, double width);
  double MT(TLorentzVector a, TLorentzVector b);
  bool HasFlag(TString flag);
  std::vector<Jet> JetsAwayFromFatJet(std::vector<Jet> jets, std::vector<FatJet> fatjets, double mindr=1.0);
  std::vector<Jet> JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron> els, std::vector<Muon> mus);

  //==== GenMatching
  void PrintGen(std::vector<Gen> gens);
  Gen GetGenMatchedLepton(Lepton lep, std::vector<Gen> gens);
  Gen GetGenMathcedPhoton(Lepton lep, std::vector<Gen> gens);
  vector<int> TrackGenSelfHistory(Gen me, std::vector<Gen> gens);
  bool IsFromHadron(Gen me, std::vector<Gen> gens);
  int GetLeptonType(Lepton lep, std::vector<Gen> gens);
  int GetGenPhotonType(Gen genph, std::vector<Gen> gens);

  //==== Plotting

  std::map< TString, TH1D* > maphist_TH1D;
  TH1D* GetHist1D(TString histname);
  void FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max);

  std::map< TString, std::map<TString, TH1D*> > JSmaphist_TH1D;
  TH1D* JSGetHist1D(TString suffix, TString histname);
  void JSFillHist(TString suffix, TString histname, double value, double weight, int n_bin, double x_min, double x_max);

  void WriteHist();

  //==== Quick Plotters
  void FillLeptonPlots(std::vector<Lepton *> leps, TString this_region, double weight);

  //==== Output rootfile

  TFile *outfile;
  void SetOutfilePath(TString outname){
    outfile = new TFile(outname,"RECREATE");
  };

};



#endif

