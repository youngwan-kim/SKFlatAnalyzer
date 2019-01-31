#include "AFBAnalyzer.h"

void AFBAnalyzer::initializeAnalyzer(){
  SetupSkimTree_AFB();
  SetupZPtWeight();
}

void AFBAnalyzer::executeEvent(){
  ////////////////////////check tautau events//////////////////
  prefix="";
  TLorentzVector genZ(genZ_Px,genZ_Py,genZ_Pz,genZ_E),genl0(genl0_Px,genl0_Py,genl0_Pz,genl0_E),genl1(genl1_Px,genl1_Py,genl1_Pz,genl1_E),genfsr=genZ-genl0-genl1;
  zptcor=1.;
  if(MCSample.Contains("DYJets")){
    if(isTauTau) prefix="tau_";
    else{
      zptcor*=GetZPtWeight(genZ.Pt(),genZ.Rapidity(),abs(genl0_PID)==13?Lepton::Flavour::MUON:Lepton::Flavour::ELECTRON);
      FillGenHists(abs(genl0_PID)==13?"muon2017gen/":"electron2017gen/","",genl0,genl1,genfsr,weight_norm_1invpb*gen_weight*zptcor);
      FillGenHists(abs(genl0_PID)==13?"muon2017gen/":"electron2017gen/","_nozptcor",genl0,genl1,genfsr,weight_norm_1invpb*gen_weight);
      FillHist((abs(genl0_PID)==13?"muon2017gen/":"electron2017gen/")+prefix+"dipty",genZ.Pt(),fabs(genZ.Rapidity()),weight_norm_1invpb*gen_weight*zptcor,zptcor_nptbin,(double*)zptcor_ptbin,zptcor_nybin,(double*)zptcor_ybin);
      FillHist((abs(genl0_PID)==13?"muon2017gen/":"electron2017gen/")+prefix+"dipty_nozptcor",genZ.Pt(),fabs(genZ.Rapidity()),weight_norm_1invpb*gen_weight,zptcor_nptbin,(double*)zptcor_ptbin,zptcor_nybin,(double*)zptcor_ybin);
    }
  }

  if(!PassMETFilter()) return;

  Event* ev=new Event;
  *ev=GetEvent();
  TString channelname,muontrigger,electrontrigger;
  if(DataYear==2016){
    vector<TString> muontrigger;
    muontrigger.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v");
    muontrigger.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v");
    muontrigger.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v");
    muontrigger.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v");
    electrontrigger="HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v";
    if(ev->PassTrigger(muontrigger)){
      channelname="muon2016";
      //if(!IsDATA||DataStream.Contains("DoubleMuon")) executeEventFromParameter(channelname,ev);
    }
    if(ev->PassTrigger(electrontrigger)){
      channelname="electron2016";
      //if(!IsDATA||DataStream.Contains("DoubleEG")) executeEventFromParameter(channelname,ev);
    }
  }
  else if(DataYear==2017){
    muontrigger="HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v";
    electrontrigger="HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";
    if(ev->PassTrigger(muontrigger)){
      channelname="muon2017";
      if(!IsDATA||DataStream.Contains("DoubleMuon")) executeEventFromParameter(channelname,ev);
    }
    if(ev->PassTrigger(electrontrigger)){
      channelname="electron2017";
      if(!IsDATA||DataStream.Contains("DoubleEG")) executeEventFromParameter(channelname,ev);
    }
  }
  delete ev;
}

void AFBAnalyzer::executeEventFromParameter(TString channelname,Event* ev){
  std::vector<Muon> muons=GetMuons("POGTightWithTightIso",7.,2.4);
  std::sort(muons.begin(),muons.end(),PtComparing);
  std::vector<Electron> electrons=GetElectrons("passMediumID",9.,2.5);
  std::sort(electrons.begin(),electrons.end(),PtComparing);
  std::vector<Lepton*> leps;
  double lep0ptcut,lep1ptcut,etacut;
  double (MCCorrection::*LeptonID_SF)(TString,double,double,int)=NULL;
  double (MCCorrection::*LeptonISO_SF)(TString,double,double,int)=NULL;
  double (MCCorrection::*LeptonReco_SF)(double,double,int)=NULL;
  TString LeptonID_key,LeptonID_key_POG,LeptonISO_key,LeptonReco_key,triggerSF_key0,triggerSF_key1;
  if(channelname.Contains("muon")){
    leps=MakeLeptonPointerVector(muons);
    lep0ptcut=20.;
    lep1ptcut=10.;
    etacut=2.4;
    LeptonID_SF=&MCCorrection::MuonID_SF;
    LeptonISO_SF=&MCCorrection::MuonISO_SF;
    LeptonID_key="NUM_TightID_DEN_genTracks";
    LeptonISO_key="NUM_TightRelIso_DEN_TightIDandIPCut";
    triggerSF_key0="Trigger_SF_IsoMu17_POGTight";
    triggerSF_key1="Trigger_SF_Mu8_POGTight";
 }else if(channelname.Contains("electron")){
    leps=MakeLeptonPointerVector(electrons);
    lep0ptcut=25.;
    lep1ptcut=15.;
    etacut=2.5;
    LeptonID_SF=&MCCorrection::ElectronID_SF;
    LeptonReco_SF=&MCCorrection::ElectronReco_SF;
    LeptonID_key="passMediumID_jihkim";
    LeptonID_key_POG="passMediumID";
    triggerSF_key0="Trigger_SF_Ele23_MediumID";
    triggerSF_key1="Trigger_SF_Ele12_MediumID";
  }else{
    cout<<"[AFBAnalyzer::executeEventFromParameter] wrong channelname"<<endl;
    return;
  }
  
  /////////////////lumi weight///////////////////
  double weight=1.,totalweight=1.;
  if(!IsDATA){
    weight=weight_norm_1invpb*ev->MCweight()*ev->GetTriggerLumi("Full");
    totalweight*=weight;
  }
  FillHist("cutflow",0.5,totalweight,20,0,20);

  /////////////////PUreweight///////////////////
  double PUreweight=1.,PUreweight_up=1.,PUreweight_down=1.;
  if(!IsDATA){
    PUreweight=mcCorr.GetPileUpWeightAsSampleName(0,nPileUp);
    PUreweight_up=mcCorr.GetPileUpWeightAsSampleName(1,nPileUp);
    PUreweight_down=mcCorr.GetPileUpWeightAsSampleName(-1,nPileUp);
    totalweight*=PUreweight;
  }
  FillHist("cutflow",1.5,totalweight,20,0,20);
  
  /////////////////kinematic selections///////////////////
  if(leps.size()<2) return;
  FillHist("cutflow",2.5,totalweight,20,0,20);
  if(leps.at(0)->Pt()<lep0ptcut||leps.at(1)->Pt()<lep1ptcut) return;
  if(fabs(leps.at(0)->Eta())>etacut||fabs(leps.at(1)->Eta()>etacut)) return;
  FillHist("cutflow",3.5,totalweight,20,0,20);
  if(leps.size()!=2) return;
  FillHist("cutflow",4.5,totalweight,20,0,20);
  
  /////////////////efficiency scale factors///////////////////
  double IDSF=1.,IDSF_up=1.,IDSF_down=1.;
  double IDSF_POG=1.,IDSF_POG_up=1.,IDSF_POG_down=1.;
  double ISOSF=1.,ISOSF_up=1.,ISOSF_down=1.;
  double RECOSF=1.,RECOSF_up=1.,RECOSF_down=1.;
  if(!IsDATA){
    for(int i=0;i<(int)leps.size();i++){
      double this_pt,this_eta;
      if(leps[i]->LeptonFlavour()==Lepton::MUON){
	this_pt=((Muon*)leps.at(i))->MiniAODPt();
	this_eta=leps.at(i)->Eta();
      }else if(leps[i]->LeptonFlavour()==Lepton::ELECTRON){
	this_pt=leps.at(i)->Pt();
	this_eta=((Electron*)leps.at(i))->scEta();
      }else{
	cout <<"[AFBAnalyzer::executeEventFromParameter] It is not lepton"<<endl;
	exit(EXIT_FAILURE);
      }

      double this_RECOSF=LeptonReco_SF?(mcCorr.*LeptonReco_SF)(this_eta,this_pt,0):1.;
      double this_RECOSF_up=LeptonReco_SF?(mcCorr.*LeptonReco_SF)(this_eta,this_pt,1):1.;
      double this_RECOSF_down=LeptonReco_SF?(mcCorr.*LeptonReco_SF)(this_eta,this_pt,-1):1.;
      RECOSF*=this_RECOSF; RECOSF_up*=this_RECOSF_up; RECOSF_down*=this_RECOSF_down;

      double this_IDSF=LeptonID_SF?(mcCorr.*LeptonID_SF)(LeptonID_key,this_eta,this_pt,0):1.;
      double this_IDSF_up=LeptonID_SF?(mcCorr.*LeptonID_SF)(LeptonID_key,this_eta,this_pt,1):1.;
      double this_IDSF_down=LeptonID_SF?(mcCorr.*LeptonID_SF)(LeptonID_key,this_eta,this_pt,-1):1.;
      IDSF*=this_IDSF; IDSF_up*=this_IDSF_up; IDSF_down*=this_IDSF_down;
      
      if(LeptonID_key_POG!=""){
	double this_IDSF_POG=LeptonID_SF?(mcCorr.*LeptonID_SF)(LeptonID_key_POG,this_eta,this_pt,0):1.;
	double this_IDSF_POG_up=LeptonID_SF?(mcCorr.*LeptonID_SF)(LeptonID_key_POG,this_eta,this_pt,1):1.;
	double this_IDSF_POG_down=LeptonID_SF?(mcCorr.*LeptonID_SF)(LeptonID_key_POG,this_eta,this_pt,-1):1.;
	IDSF_POG*=this_IDSF_POG; IDSF_POG_up*=this_IDSF_POG_up; IDSF_POG_down*=this_IDSF_POG_down;
      }

      double this_ISOSF=LeptonISO_SF?(mcCorr.*LeptonISO_SF)(LeptonISO_key,this_eta,this_pt,0):1.;
      double this_ISOSF_up=LeptonISO_SF?(mcCorr.*LeptonISO_SF)(LeptonISO_key,this_eta,this_pt,1):1.;
      double this_ISOSF_down=LeptonISO_SF?(mcCorr.*LeptonISO_SF)(LeptonISO_key,this_eta,this_pt,-1):1.;
      ISOSF*=this_ISOSF; ISOSF_up*=this_ISOSF_up; ISOSF_down*=this_ISOSF_down;
    }
  }
  totalweight*=RECOSF;
  FillHist("cutflow",5.5,totalweight,20,0,20);
  totalweight*=IDSF;
  FillHist("cutflow",6.5,totalweight,20,0,20);
  totalweight*=ISOSF;
  FillHist("cutflow",7.5,totalweight,20,0,20);

  double triggerSF=1.,triggerSF_up=1.,triggerSF_down=1.;
  if(!IsDATA){
    triggerSF*=DileptonTrigger_SF(triggerSF_key0,triggerSF_key1,leps,0);
    triggerSF_up*=DileptonTrigger_SF(triggerSF_key0,triggerSF_key1,leps,1);
    triggerSF_down*=DileptonTrigger_SF(triggerSF_key0,triggerSF_key1,leps,-1);
  }
  totalweight*=triggerSF;
  FillHist("cutflow",8.5,totalweight,20,0,20);

  //////////////////////no MC for M<50////////////////
  if(((*leps.at(0))+(*leps.at(1))).M()<60) return;
  FillHist("cutflow",9.5,totalweight,20,0,20);


  //////////////////////PrefileWeight////////////////////
  double prefireweight=L1PrefireReweight_Central;
  double prefireweight_up=L1PrefireReweight_Up;
  double prefireweight_down=L1PrefireReweight_Down;
  totalweight*=prefireweight;
  FillHist("cutflow",10.5,totalweight,20,0,20);

  totalweight*=zptcor;
  FillHist("cutflow",11.5,totalweight,20,0,20);

  ///////////////////////weight systematics//////////////////
  map<TString,double> map_systematic;
  if(!IsDATA){
    map_systematic["noPUreweight"]=weight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*zptcor;
    map_systematic["PUreweight_up"]=weight*PUreweight_up*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*zptcor;
    map_systematic["PUreweight_down"]=weight*PUreweight_down*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*zptcor;
  
    map_systematic["noefficiencySF"]=weight*PUreweight*prefireweight*zptcor;
  
    map_systematic["noRECOSF"]=weight*PUreweight*IDSF*ISOSF*triggerSF*prefireweight*zptcor;
    map_systematic["RECOSF_up"]=weight*PUreweight*RECOSF_up*IDSF*ISOSF*triggerSF*prefireweight*zptcor;
    map_systematic["RECOSF_down"]=weight*PUreweight*RECOSF_down*IDSF*ISOSF*triggerSF*prefireweight*zptcor;
    
    map_systematic["noIDSF"]=weight*PUreweight*RECOSF*ISOSF*triggerSF*prefireweight*zptcor;
    map_systematic["IDSF_up"]=weight*PUreweight*RECOSF*IDSF_up*ISOSF*triggerSF*prefireweight*zptcor;
    map_systematic["IDSF_down"]=weight*PUreweight*RECOSF*IDSF_down*ISOSF*triggerSF*prefireweight*zptcor;

    map_systematic["IDSF_POG"]=weight*PUreweight*RECOSF*IDSF_POG*ISOSF*triggerSF*prefireweight*zptcor;
    map_systematic["IDSF_POG_up"]=weight*PUreweight*RECOSF*IDSF_POG_up*ISOSF*triggerSF*prefireweight*zptcor;
    map_systematic["IDSF_POG_down"]=weight*PUreweight*RECOSF*IDSF_POG_down*ISOSF*triggerSF*prefireweight*zptcor;
    
    map_systematic["noISOSF"]=weight*PUreweight*RECOSF*IDSF*triggerSF*prefireweight*zptcor;
    map_systematic["ISOSF_up"]=weight*PUreweight*RECOSF*IDSF*ISOSF_up*triggerSF*prefireweight*zptcor;
    map_systematic["ISOSF_down"]=weight*PUreweight*RECOSF*IDSF*ISOSF_down*triggerSF*prefireweight*zptcor;
    
    map_systematic["notriggerSF"]=weight*PUreweight*RECOSF*IDSF*ISOSF*prefireweight*zptcor;
    map_systematic["triggerSF_up"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF_up*prefireweight*zptcor;
    map_systematic["triggerSF_down"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF_down*prefireweight*zptcor;

    map_systematic["noprefireweight"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*zptcor;
    map_systematic["prefireweight_up"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight_up*zptcor;
    map_systematic["prefireweight_down"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight_down*zptcor;
    
    map_systematic["nozptcor"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight;

    if(PDFWeights_AlphaS->size()==2){
      map_systematic["alphaS_up"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*zptcor*PDFWeights_AlphaS->at(0);
      map_systematic["alphaS_down"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*zptcor*PDFWeights_AlphaS->at(1);
    }
  
    for(int i=0;i<(int)PDFWeights_Scale->size();i++){
      map_systematic[Form("scalevariation%d",i)]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*zptcor*PDFWeights_Scale->at(i);
    }
  }

  
  
  ///////////////////////fill hists///////////////////////
  if(leps.at(0)->Charge()*leps.at(1)->Charge()<0){
    FillHist("cutflow",12.5,totalweight,20,0,20);
    FillBasicHists(channelname+"/OS/"+prefix,"",leps,totalweight);
    FillSystematicHists(channelname+"/OS/"+prefix,"",leps,map_systematic);
    if(((*leps.at(0))+(*leps.at(1))).M()<100&&((*leps.at(0))+(*leps.at(1))).M()>80){
      FillHist("cutflow",13.5,totalweight,20,0,20);
      FillBasicHists(channelname+"/OS_Z/"+prefix,"",leps,totalweight);
      FillSystematicHists(channelname+"/OS_Z/"+prefix,"",leps,map_systematic);
      for(int i=0;i<zptcor_nybin;i++){
	if(fabs(((*leps.at(0))+(*leps.at(1))).Rapidity())>=zptcor_ybin[i]&&fabs(((*leps.at(0))+(*leps.at(1))).Rapidity())<zptcor_ybin[i+1]){
	  FillBasicHists(channelname+Form("/OS_Z_y%.1fto%.1f/",zptcor_ybin[i],zptcor_ybin[i+1])+prefix,"",leps,totalweight);
	  FillSystematicHists(channelname+Form("/OS_Z_y%.1fto%.1f/",zptcor_ybin[i],zptcor_ybin[i+1])+prefix,"",leps,map_systematic);
	}
      }
      FillHist(channelname+"/OS_Z/"+prefix+"dipty",((*leps.at(0))+(*leps.at(1))).Pt(),fabs(((*leps.at(0))+(*leps.at(1))).Rapidity()),totalweight,zptcor_nptbin,(double*)zptcor_ptbin,zptcor_nybin,(double*)zptcor_ybin);
      FillHist(channelname+"/OS_Z/"+prefix+"dipty_nozptcor",((*leps.at(0))+(*leps.at(1))).Pt(),fabs(((*leps.at(0))+(*leps.at(1))).Rapidity()),map_systematic["nozptcor"],zptcor_nptbin,(double*)zptcor_ptbin,zptcor_nybin,(double*)zptcor_ybin);
    }
  }else{
    FillHist("cutflow",14.5,totalweight,20,0,20);
    FillBasicHists(channelname+"/SS/"+prefix,"",leps,totalweight);
    FillSystematicHists(channelname+"/SS/"+prefix,"",leps,map_systematic);
  }
}
AFBAnalyzer::AFBAnalyzer(){
}
AFBAnalyzer::~AFBAnalyzer(){
}
