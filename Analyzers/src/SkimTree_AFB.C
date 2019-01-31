#include "SkimTree_AFB.h"

void SkimTree_AFB::initializeAnalyzer(){

  newtree = fChain->CloneTree(0);
  newtree->SetBranchStatus("jet_*",0);
  newtree->SetBranchStatus("fatjet_*",0);
  newtree->SetBranchStatus("gen_*",0);
  newtree->SetBranchStatus("gen_weight",1);
  newtree->SetBranchStatus("photon_*",0);
  newtree->SetBranchStatus("pfMET_*",0);
  newtree->Branch("isTauTau",&isTauTau);
  newtree->Branch("genZ_Px",&genZ_Px);
  newtree->Branch("genZ_Py",&genZ_Py);
  newtree->Branch("genZ_Pz",&genZ_Pz);
  newtree->Branch("genZ_E",&genZ_E);
  newtree->Branch("genl0_PID",&genl0_PID);
  newtree->Branch("genl0_Px",&genl0_Px);
  newtree->Branch("genl0_Py",&genl0_Py);
  newtree->Branch("genl0_Pz",&genl0_Pz);
  newtree->Branch("genl0_E",&genl0_E);
  newtree->Branch("genl1_PID",&genl1_PID);
  newtree->Branch("genl1_Px",&genl1_Px);
  newtree->Branch("genl1_Py",&genl1_Py);
  newtree->Branch("genl1_Pz",&genl1_Pz);
  newtree->Branch("genl1_E",&genl1_E);
  if(!MCSample.Contains("DYJets")){
    newtree->SetBranchStatus("isTauTau",0);
    newtree->SetBranchStatus("genZ_Px",0);
    newtree->SetBranchStatus("genZ_Py",0);
    newtree->SetBranchStatus("genZ_Pz",0);
    newtree->SetBranchStatus("genZ_E",0);
    newtree->SetBranchStatus("genl0_PID",0);
    newtree->SetBranchStatus("genl0_Px",0);
    newtree->SetBranchStatus("genl0_Py",0);
    newtree->SetBranchStatus("genl0_Pz",0);
    newtree->SetBranchStatus("genl0_E",0);
    newtree->SetBranchStatus("genl1_PID",0);
    newtree->SetBranchStatus("genl1_Px",0);
    newtree->SetBranchStatus("genl1_Py",0);
    newtree->SetBranchStatus("genl1_Pz",0);
    newtree->SetBranchStatus("genl1_E",0);
  }

  triggers.clear();
  if(DataYear==2016){
    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"
    };
  }
  else if(DataYear==2017){
    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"
    };
  }

  cout << "[SkimTree_AFB::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_AFB::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

}

void SkimTree_AFB::executeEvent(){
  isTauTau=0;
  Gen genl0,genl1,genfsr,genhardl0,genhardl1;
  TLorentzVector genZ;
  if(MCSample.Contains("DYJets")){
    vector<Gen> gens=GetGens();
    for(int i=0;i<(int)gens.size();i++){
      if(!gens.at(i).isPrompt()) continue;
      if(gens.at(i).isHardProcess()){
        if(genhardl0.IsEmpty()&&(abs(gens.at(i).PID())==11||abs(gens.at(i).PID())==13||abs(gens.at(i).PID())==15)) genhardl0=gens.at(i);
        else if(!genhardl0.IsEmpty()&&gens.at(i).PID()==-genhardl0.PID()){
          genhardl1=gens.at(i);
          if(abs(genhardl1.PID())==15){
            isTauTau=1;
            break;
          }
        }
      }
      if(gens.at(i).Status()==1){
        if(genl0.IsEmpty()&&(abs(gens.at(i).PID())==11||abs(gens.at(i).PID())==13)) genl0=gens.at(i);
        else if(!genl0.IsEmpty()&&gens.at(i).PID()==-genl0.PID()){
          genl1=gens.at(i);
        }
        else if(gens.at(i).PID()==22){
          genfsr+=gens.at(i);
        }
      }
    }
    if(abs(genhardl0.PID())!=15){
      genZ=(genl0+genl1+genfsr);
      if(genZ.Pt()==0) cout<<"[SkimTree_AFB] wrong gen selection"<<endl;
    }
    genZ_Px=genZ.Px();
    genZ_Py=genZ.Py();
    genZ_Pz=genZ.Pz();
    genZ_E=genZ.E();
    genl0_PID=genl0.PID();
    genl0_Px=genl0.Px();
    genl0_Py=genl0.Py();
    genl0_Pz=genl0.Pz();
    genl0_E=genl0.E();
    genl1_PID=genl1.PID();
    genl1_Px=genl1.Px();
    genl1_Py=genl1.Py();
    genl1_Pz=genl1.Pz();
    genl1_E=genl1.E();
  }

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);

  if( ev.PassTrigger(triggers) ){
    newtree->Fill();
  }

}

SkimTree_AFB::SkimTree_AFB(){

}

SkimTree_AFB::~SkimTree_AFB(){

}

void SkimTree_AFB::SetupSkimTree_AFB(){
  fChain->SetBranchAddress("isTauTau",&isTauTau);
  fChain->SetBranchAddress("genZ_Px",&genZ_Px);
  fChain->SetBranchAddress("genZ_Py",&genZ_Py);
  fChain->SetBranchAddress("genZ_Pz",&genZ_Pz);
  fChain->SetBranchAddress("genZ_E",&genZ_E);
  fChain->SetBranchAddress("genl0_PID",&genl0_PID);
  fChain->SetBranchAddress("genl0_Px",&genl0_Px);
  fChain->SetBranchAddress("genl0_Py",&genl0_Py);
  fChain->SetBranchAddress("genl0_Pz",&genl0_Pz);
  fChain->SetBranchAddress("genl0_E",&genl0_E);
  fChain->SetBranchAddress("genl1_PID",&genl1_PID);
  fChain->SetBranchAddress("genl1_Px",&genl1_Px);
  fChain->SetBranchAddress("genl1_Py",&genl1_Py);
  fChain->SetBranchAddress("genl1_Pz",&genl1_Pz);
  fChain->SetBranchAddress("genl1_E",&genl1_E);
}  

void SkimTree_AFB::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}


