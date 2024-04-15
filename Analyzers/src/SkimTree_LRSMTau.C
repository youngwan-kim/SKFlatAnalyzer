#include "SkimTree_LRSMTau.h"

void SkimTree_LRSMTau::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_LRSMTau::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  triggers.clear();
  if(DataYear==2016){
    triggers = {
      "HLT_VLooseIsoPFTau120_Trk50_eta2p1_v",
      "HLT_VLooseIsoPFTau140_Trk50_eta2p1_v"
    };
    TriggerSafePtCut = 150.;
  }
  else if(DataYear==2017){
    triggers = {
      "HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"
    };
    TriggerSafePtCut = 190.;
  }
  else if(DataYear==2018){
    triggers = {
      "HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v"
    };
    TriggerSafePtCut = 190.;
  }
  else{
    cout << "[SkimTree_LRSMTau::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }

  cout << "[SkimTree_LRSMTau::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_LRSMTau::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

  //==== WR (Z') : lepton pt > 53 (65) GeV
  LeptonPtCut = 40.;
  //==== WR (Z') : jet pt > 40 GeV
  AK4JetPtCut = 30.;
  //==== WR (Z') : Jet pt > 200 (300) GeV
  AK8JetPtCut = 170.;

  cout << "[SkimTree_LRSMTau::initializeAnalyzer] LeptonPtCut = " << LeptonPtCut << endl;
  cout << "[SkimTree_LRSMTau::initializeAnalyzer] AK4JetPtCut = " << AK4JetPtCut << endl;
  cout << "[SkimTree_LRSMTau::initializeAnalyzer] AK8JetPtCut = " << AK8JetPtCut << endl;

}

void SkimTree_LRSMTau::executeEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);

  //==== Skim 1 ) trigger
  if(! (ev.PassTrigger(triggers)) ) return;

  //==== Skim 2 ) baseline presel 
  vector<Tau> alltaus = GetAllTaus();
  int Ntaus = 0;
  for(unsigned int i=0; i<alltaus.size(); i++){
    if(alltaus.at(i).Pt()>TriggerSafePtCut) Ntaus++;
  }
  vector<Muon> allmuons = UseTunePMuon( GetAllMuons() );
  vector<Muon> loosemuons = SelectMuons( allmuons, "POGHighPt" , 50, 2.4 );
  vector<Electron> allelectrons = GetAllElectrons();
  vector<Electron> looseelectrons = SelectElectrons( allelectrons, "LRSMLoose", 50, 2.4);
  int NlooseMu = loosemuons.size(); int NlooseEl = looseelectrons.size();

  bool isBaseline = (Ntaus > 0) && (NlooseEl > 0 || NlooseMu >0);

  if(!isBaseline) return;

  //==== Skim 3 ) jets (boosted/resolved)
  vector<Jet> jets = GetJets("tightLepVeto",40,2.4);
  vector<FatJet> fatjets = GetFatJets("tight",200,2.4);

  bool isResolvedPresel = isBaseline && jets.size()>1 ;
  bool isBoostedPresel = isBaseline && !isResolvedPresel && fatjets.size() ;

  if(!(isResolvedPresel || isBoostedPresel)) return;

  newtree->Fill();

}

void SkimTree_LRSMTau::executeEventFromParameter(AnalyzerParameter param){


}

SkimTree_LRSMTau::SkimTree_LRSMTau(){

  newtree = NULL;

}

SkimTree_LRSMTau::~SkimTree_LRSMTau(){

}


void SkimTree_LRSMTau::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}


