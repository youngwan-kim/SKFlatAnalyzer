#ifndef WRTau_Core_h
#define WRTau_Core_h

#include "AnalyzerCore.h"
#include "TauIDSFTool.h"

class WRTau_Core : public AnalyzerCore {

public:

  WRTau_Core();
  ~WRTau_Core();

  std::string DeepTauVSjet = "DeepTau2017v2p1VSjet";
  std::string DeepTauVSe = "DeepTau2017v2p1VSe";
  std::string DeepTauVSmu = "DeepTau2017v2p1VSmu";

  map<int,TString> idname_map = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  map<int,std::string> idname_map_str = {{0,"VVVLoose"},{1,"VVLoose"},{2,"VLoose"},{3,"Loose"},{4,"Medium"},{5,"Tight"},{6,"VTight"},
                                 {8,"VVVLoose"},{9,"VVLoose"},{10,"VLoose"},{11,"Loose"},{12,"Medium"},{13,"Tight"},
                                 {17,"VVLoose"},{18,"VLoose"},{19,"Loose"},{20,"Medium"},{21,"Tight"}};

  // cache tauidsftool once during initialization
  map<int,TauIDSFTool*> tauidsftool_map = {{0,NULL},{1,NULL},{2,NULL},{3,NULL},{4,NULL},{5,NULL},{6,NULL}}

  vector<int> vJet_vec; vector<int> vEl_vec; vector<int> vMu_vec;

  TDirectory *histDir;
  TH1D *TauIDSFHist;

  // Regions
  const inline bool isPreselection(const std::vector<Tau>& taus){ return (taus.size()>0); };
  const inline bool hasAtLeast2AK4Jets(const std::vector<Jet>& jets){ return (jets.size()>1); };
  const inline bool hasAtLeast1AK8Jets(const std::vector<FatJet>& fatjets){ return (fatjets.size()>0); };
  const inline bool hasAtLeast1Leptons(const std::vector<Lepton *> TightLeptons){ return (TightLeptons.size()>0); };

  bool isBoostedPreselection(const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                             const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                             const std::vector<Lepton *> TightLeptons);

  bool isBoostedPreselectionTest(const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                             const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                             const std::vector<Lepton *> TightLeptons);
  
  bool isResolvedPreselection(const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                             const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                             const std::vector<Lepton *> TightLeptons);

  // Objects 
  double GetTauIDSF(TString vsJetWP, TString vsEleWP, int DM, double pt,bool GetFromDM=false);
  std::vector<Jet> VetoTauFromJets(const std::vector<Jet>& jets, const std::vector<Tau>& taus, double dR=0.4);
  std::vector<FatJet> VetoTauFromFatJets(const std::vector<FatJet>& fatjets, const std::vector<Tau>& taus, double dR=0.8);
  std::vector<Lepton*> VetoTauFromLeptons(const std::vector<Lepton *> leptons, const std::vector<Tau>& taus, double dR=0.4);
  std::vector<Tau> VetoLeptonsFromTaus(const std::vector<Lepton *> leptons, const std::vector<Tau>& taus, double dR=0.4);
  std::vector<Electron> ElectronPromptOnly_Tau(const std::vector<Electron>& electrons, const std::vector<Gen>& gens);
  std::vector<Electron> ElectronNonPromptOnly_Tau(const std::vector<Electron>& electrons, const std::vector<Gen>& gens);
  std::vector<Muon> MuonPromptOnly_Tau(const std::vector<Muon>& muons, const std::vector<Gen>& gens);
  std::vector<Muon> MuonNonPromptOnly_Tau(const std::vector<Muon>& muons, const std::vector<Gen>& gens);

  // Truth Matching
  LHE GetClosestTauLHE(const Tau tau, const std::vector<LHE>& LHEs);
  Gen GetClosestTauGen(const Tau tau, const std::vector<Gen>& gens);
  void FillClosestTauGen(const Tau tau, const std::vector<Gen>& gens, TString region,double weight);
  void FillClosestTauLHE(const Tau tau, const std::vector<LHE>& LHEs, TString region,double weight);
  int GetTauType(const Tau tau, const std::vector<Gen>& gens);
  int GetTauType_Public(int TruthIdx, const std::vector<Gen>& TruthColl);
  vector<int> GetTauAncestor(const Gen& me, const std::vector<Gen>& gens);
  vector<Tau> TauPromptOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens);
  vector<Tau> TauFakeOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens);
  vector<Tau> TauErrorOnly(const std::vector<Tau>& taus, const std::vector<Gen>& gens);

  // Variables
  double GetST(std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Tau> taus, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle METv);
  double GetST(std::vector<Lepton *> leptons,  std::vector<Tau> taus, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle METv);

  // Filling basic histograms after preselection (requiring >1 hadronic tau with pT>190 GeV)
  void FillPreselHists(TString region,const std::vector<Tau>& taus, const std::vector<Jet>& jets, const std::vector<Jet>& bjets,
                       const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                       const std::vector<Lepton *> TightLeptons, double weight); 
  void FillMassHists(TString region, const Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                       const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                       const std::vector<Lepton *> TightLeptons, double weight);
  /*void FillChannelHists(TString region, const Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                       const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                       const std::vector<Lepton *> TightLeptons, double weight);*/                       
  void CopyHist(TString histname0, TString histname1);

  // Others
  TDirectory* GetTempDir_WRTauCore();

};



#endif

