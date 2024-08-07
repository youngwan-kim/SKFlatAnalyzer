#include "SkimTree_SingleTau.h"

void SkimTree_SingleTau::initializeAnalyzer(){


  outfile->cd();
  cout << "[SkimTree_SingleTau::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
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
    cout << "[SkimTree_SingleTau::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }

  cout << "[SkimTree_SingleTau::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_SingleTau::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

}

void SkimTree_SingleTau::executeEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);

  //==== Skim 1 ) trigger
  if(! (ev.PassTrigger(triggers)) ) return;
  newtree->Fill();

}

void SkimTree_SingleTau::executeEventFromParameter(AnalyzerParameter param){
 

}

SkimTree_SingleTau::SkimTree_SingleTau(){

  newtree = NULL;

}

SkimTree_SingleTau::~SkimTree_SingleTau(){

}



void SkimTree_SingleTau::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}

