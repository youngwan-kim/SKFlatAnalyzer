#ifndef ZTauTau_h
#define ZTauTau_h

#include "WRTau_Core.h"

class ZTauTau : public WRTau_Core {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<TString> Triggers;

  ZTauTau();
  ~ZTauTau();

  vector<Tau> taus_temp;
  vector<Tau> taus;

  TauIDSFTool *tauSFTool_VSJet;
  // TauIDSFTool *tauIDSFtool = NULL;
  // TauIDSFTool *tauSFTool_VSJet = NULL;

  std::string DeepTauVSjet = "DeepTau2017v2p1VSjet";
  std::string DeepTauVSe = "DeepTau2017v2p1VSe";
  std::string DeepTauVSmu = "DeepTau2017v2p1VSmu";

  map<int,TString> idname_map = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  map<int,std::string> idname_map_str = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  vector<int> vJet_vec = {2,4,5}; vector<int> vEl_vec = {8,13}; vector<int> vMu_vec = {18,21};

  TString idname,path;

};



#endif

