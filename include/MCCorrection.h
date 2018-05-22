#ifndef MCCorrection_h
#define MCCorrection_h

#include "TString.h"
#include "Lepton.h"
#include "Muon.h"
#include "Electron.h"

class MCCorrection{

public:

  MCCorrection();
  ~MCCorrection();

  bool IgnoreNoHist;

  double MuonID_SF(TString ID, double eta, double pt, int sys=0);
  double MuonISO_SF(TString ID, double eta, double pt, int sys=0);
  double MuonTrigger_Eff(TString ID, TString trig, int DataOrMC, double eta, double pt, int sys=0);
  double MuonTrigger_SF(TString ID, TString trig, std::vector<Muon> muons, int sys=0);
  std::map< TString, TH2F* > map_hist_Muon;

  double ElectronReco_SF(double sceta, double pt, int sys=0);
  double ElectronID_SF(TString ID, double sceta, double pt, int sys=0);
  std::map< TString, TH2F* > map_hist_Electron;

};

#endif
