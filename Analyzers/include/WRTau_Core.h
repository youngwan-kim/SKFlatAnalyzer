#ifndef WRTau_Core_h
#define WRTau_Core_h

#include "AnalyzerCore.h"
#include "TauIDSFTool.h"

class WRTau_Core : public AnalyzerCore {

public:

  WRTau_Core();
  ~WRTau_Core();

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
  
  bool isResolvedPreselection(const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                             const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                             const std::vector<Lepton *> TightLeptons);

  // Objects 
  double GetTauIDSF(TString vsJetWP, TString vsEleWP, int DM, double pt);
  std::vector<Jet> VetoTauFromJets(const std::vector<Jet>& jets, const std::vector<Tau>& taus, double dR=0.4);
  std::vector<FatJet> VetoTauFromFatJets(const std::vector<FatJet>& fatjets, const std::vector<Tau>& taus, double dR=0.8);
  std::vector<Lepton*> VetoTauFromLeptons(const std::vector<Lepton *> leptons, const std::vector<Tau>& taus, double dR=0.4);
  std::vector<Tau> VetoLeptonsFromTaus(const std::vector<Lepton *> leptons, const std::vector<Tau>& taus, double dR=0.4);

  // Filling basic histograms after preselection (requiring >1 hadronic tau with pT>190 GeV)
  void FillPreselHists(TString region,const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                       const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                       const std::vector<Lepton *> TightLeptons, double weight); 
  void FillMassHists(TString region, const Particle METv, const std::vector<Tau>& taus, const std::vector<Jet>& jets, 
                       const std::vector<FatJet>& fatjets,const std::vector<Lepton *> LooseLeptons, 
                       const std::vector<Lepton *> TightLeptons, double weight);
  void CopyHist(TString histname0, TString histname1);

  // Others
  TDirectory* GetTempDir_WRTauCore();

};



#endif

