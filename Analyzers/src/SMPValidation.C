#include "SMPValidation.h"

void SMPValidation::initializeAnalyzer(){
  /*
  if(!IsDATA){
    fChain->SetBranchStatus("gen_*",0);
    fChain->SetBranchStatus("gen_weight",1);
  }
  */
  //fChain->SetBranchStatus("jet_*",0);
  //fChain->SetBranchStatus("photon_*",0);
  //fChain->SetBranchStatus("fatjet_*",0);
  //fChain->SetBranchStatus("pfMET_*",0);
  
}

void SMPValidation::executeEvent(){
  if(!PassMETFilter()) return;

  Event* ev=new Event;
  *ev=GetEvent();
  TString channelname,muontrigger,electrontrigger;
  if(DataYear==2016){
    //NEEDTOADD
  }
  else if(DataYear==2017){
    muontrigger="HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v";
    electrontrigger="HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";
    if(ev->PassTrigger(muontrigger)){
      channelname="muon2017";
      if(!IsDATA||DataStream.Contains("DoubleMuon")) executeEventFromParameter(channelname,ev,muontrigger);
    }
    if(ev->PassTrigger(electrontrigger)){
      channelname="electron2017";
      if(!IsDATA||DataStream.Contains("DoubleEG")) executeEventFromParameter(channelname,ev,electrontrigger);
    }
  }
  delete ev;
}

void SMPValidation::executeEventFromParameter(TString channelname,Event* ev,TString trigger){
  std::vector<Muon> muons=GetMuons("POGTightWithTightIso",7.,2.4);
  std::sort(muons.begin(),muons.end(),PtComparing);
  std::vector<Electron> electrons=GetElectrons("passMediumID",9.,2.5);
  std::sort(electrons.begin(),electrons.end(),PtComparing);
  std::vector<Lepton*> leps;
  double lep0ptcut,lep1ptcut,etacut;
  double (MCCorrection::*LeptonID_SF)(TString,double,double,int)=NULL;
  double (MCCorrection::*LeptonISO_SF)(TString,double,double,int)=NULL;
  double (MCCorrection::*LeptonReco_SF)(double,double,int)=NULL;
  TString LeptonID_key,LeptonISO_key,LeptonReco_key,triggerSF_key0,triggerSF_key1;
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
    LeptonID_key="passMediumID";
    triggerSF_key0="Trigger_SF_Ele23_MediumID";
    triggerSF_key1="Trigger_SF_Ele12_MediumID";
  }else{
    cout<<"[SMPValidation::executeEventFromParameter] wrong channelname"<<endl;
    return;
  }
  
  /////////////////lumi weight///////////////////
  double weight=1.,totalweight=1.;
  if(!IsDATA){
    weight=weight_norm_1invpb*ev->MCweight()*ev->GetTriggerLumi(trigger);
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
	cout <<"[SMPValidation::executeEventFromParameter] It is not lepton"<<endl;
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
  double prefireweight=GetPrefireWeight(0);
  double prefireweight_up=GetPrefireWeight(1);
  double prefireweight_down=GetPrefireWeight(-1);
  totalweight*=prefireweight;
  FillHist("cutflow",10.5,totalweight,20,0,20);

  ///////////////////////weight systematics//////////////////
  map<TString,double> map_systematic;
  if(!IsDATA){
    map_systematic["noPUreweight"]=weight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight;
    map_systematic["PUreweight_up"]=weight*PUreweight_up*RECOSF*IDSF*ISOSF*triggerSF*prefireweight;
    map_systematic["PUreweight_down"]=weight*PUreweight_down*RECOSF*IDSF*ISOSF*triggerSF*prefireweight;
  
    map_systematic["noefficiencySF"]=weight*PUreweight*prefireweight;
  
    map_systematic["noRECOSF"]=weight*PUreweight*IDSF*ISOSF*triggerSF*prefireweight;
    map_systematic["RECOSF_up"]=weight*PUreweight*RECOSF_up*IDSF*ISOSF*triggerSF*prefireweight;
    map_systematic["RECOSF_down"]=weight*PUreweight*RECOSF_down*IDSF*ISOSF*triggerSF*prefireweight;
    
    map_systematic["noIDSF"]=weight*PUreweight*RECOSF*ISOSF*triggerSF*prefireweight;
    map_systematic["IDSF_up"]=weight*PUreweight*RECOSF*IDSF_up*ISOSF*triggerSF*prefireweight;
    map_systematic["IDSF_down"]=weight*PUreweight*RECOSF*IDSF_down*ISOSF*triggerSF*prefireweight;
    
    map_systematic["noISOSF"]=weight*PUreweight*RECOSF*IDSF*triggerSF*prefireweight;
    map_systematic["ISOSF_up"]=weight*PUreweight*RECOSF*IDSF*ISOSF_up*triggerSF*prefireweight;
    map_systematic["ISOSF_down"]=weight*PUreweight*RECOSF*IDSF*ISOSF_down*triggerSF*prefireweight;
    
    map_systematic["notriggerSF"]=weight*PUreweight*RECOSF*IDSF*ISOSF*prefireweight;
    map_systematic["triggerSF_up"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF_up*prefireweight;
    map_systematic["triggerSF_down"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF_down*prefireweight;

    map_systematic["noprefireweight"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF;
    map_systematic["prefireweight_up"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight_up;
    map_systematic["prefireweight_down"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight_down;
    
    if(PDFWeights_AlphaS->size()==2){
      map_systematic["alphaS_up"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*PDFWeights_AlphaS->at(0);
      map_systematic["alphaS_down"]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*PDFWeights_AlphaS->at(1);
    }
  
    for(int i=0;i<(int)PDFWeights_Scale->size();i++){
      map_systematic[Form("scalevariation%d",i)]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*PDFWeights_Scale->at(i);
    }
  }

  ////////////////////////check tautau events//////////////////
  TString prefix="";
  Gen genZ,genl0,genl1,genfsr,gentau0,gentau1;
  if(MCSample.Contains("DYJets")){
    vector<Gen> gens=GetGens();
    for(int i=0;i<(int)gens.size();i++){
      if(!gens.at(i).isPrompt()) continue;
      if(gentau0.IsEmpty()&&abs(gens.at(i).PID())==15) gentau0=gens.at(i);
      else if(!gentau0.IsEmpty()&&gens.at(i).PID()==-gentau0.PID()){
	gentau1=gens.at(i);
	prefix="tau_";
	break;
      }
    }
  }

  ///////////////////////fill hists///////////////////////
  if(leps.at(0)->Charge()*leps.at(1)->Charge()<0){
    FillHist("cutflow",11.5,totalweight,20,0,20);
    FillBasicHists(channelname+"/OS/"+prefix,"",leps,totalweight);
    FillSystematicHists(channelname+"/OS/"+prefix,"",leps,map_systematic);
    if(((*leps.at(0))+(*leps.at(1))).M()<100&&((*leps.at(0))+(*leps.at(1))).M()>80){
      FillHist("cutflow",12.5,totalweight,20,0,20);
      FillBasicHists(channelname+"/OS_Z/"+prefix,"",leps,totalweight);
      FillSystematicHists(channelname+"/OS_Z/"+prefix,"",leps,map_systematic);
      double yrange[4]={0,0.4,1.0,2.4};
      for(int i=0;i<3;i++){
	if(fabs(((*leps.at(0))+(*leps.at(1))).Rapidity())>=yrange[i]&&fabs(((*leps.at(0))+(*leps.at(1))).Rapidity())<yrange[i+1]){
	  FillBasicHists(channelname+Form("/OS_Z_y%.1fto%.1f/",yrange[i],yrange[i+1])+prefix,"",leps,totalweight);
	  FillSystematicHists(channelname+Form("/OS_Z_y%.1fto%.1f/",yrange[i],yrange[i+1])+prefix,"",leps,map_systematic);
	}
      }
    }
  }else{
    FillHist("cutflow",13.5,totalweight,20,0,20);
    FillBasicHists(channelname+"/SS/"+prefix,"",leps,totalweight);
    FillSystematicHists(channelname+"/SS/"+prefix,"",leps,map_systematic);
  }
}

void SMPValidation::FillBasicHists(TString prefix,TString suffix,const vector<Lepton*>& leps,double w){
  Particle dilepton=((*leps.at(0))+(*leps.at(1)));
  FillHist(prefix+"dimass"+suffix,dilepton.M(),w,400,0,400);
  FillHist(prefix+"dipt"+suffix,dilepton.Pt(),w,400,0,400);
  FillHist(prefix+"dirap"+suffix,dilepton.Rapidity(),w,50,-5,5);
  for(int i=0;i<(int)leps.size();i++){
    FillHist(Form("%sl%dpt%s",prefix.Data(),i,suffix.Data()),leps.at(i)->Pt(),w,200,0,200);
    FillHist(Form("%sl%deta%s",prefix.Data(),i,suffix.Data()),leps.at(i)->Eta(),w,50,-5,5);
    FillHist(Form("%sl%driso%s",prefix.Data(),i,suffix.Data()),leps.at(i)->RelIso(),w,30,0,0.3);
  }
  FillHist(prefix+"lldelR"+suffix,leps.at(0)->DeltaR(*leps.at(1)),w,70,0,7);  
  FillHist(prefix+"lldelphi"+suffix,leps.at(0)->DeltaPhi(*leps.at(1)),w,80,-4,4);
  FillHist(prefix+"nPV"+suffix,nPV,w,60,0,60);
}
void SMPValidation::FillSystematicHists(TString prefix,TString suffix,const vector<Lepton*>& leps,map<TString,double> map_systematic){
  for(auto iter=map_systematic.begin();iter!=map_systematic.end();iter++){
    FillBasicHists(prefix,"_"+iter->first+suffix,leps,iter->second);
  }
}
double SMPValidation::DileptonTrigger_SF(TString SFhistkey0,TString SFhistkey1,const vector<Lepton*>& leps,int sys){
  if(IsDATA) return 1;
  if(leps.size()!=2){
    cout<<"[SMPValidation::Trigger_SF] only dilepton algorithm"<<endl;
    return 1;
  }
  map<TString,TH2F*>* map_hist_Lepton=NULL;
  if(leps[0]->LeptonFlavour()==Lepton::MUON){
    map_hist_Lepton=&mcCorr.map_hist_Muon;
  }else if(leps[0]->LeptonFlavour()==Lepton::ELECTRON){
    map_hist_Lepton=&mcCorr.map_hist_Electron;
  }else{
    cout <<"[SMPValidation::Trigger_SF] Not ready"<<endl;
    exit(EXIT_FAILURE);
  }    
      
  TH2F* this_hist[2]={NULL,NULL};
  double triggerSF=1.;
  this_hist[0]=(*map_hist_Lepton)[SFhistkey0];
  this_hist[1]=(*map_hist_Lepton)[SFhistkey1];
  if(!this_hist[0]||!this_hist[1]){
    cout << "[SMPValidation::Trigger_SF] No "<<SFhistkey0<<" or "<<SFhistkey1<<endl;
    exit(EXIT_FAILURE);
  }
  for(int i=0;i<(int)leps.size();i++){
    double this_pt,this_eta;
    if(leps[i]->LeptonFlavour()==Lepton::MUON){
      this_pt=((Muon*)leps.at(i))->MiniAODPt();
      this_eta=leps.at(i)->Eta();
    }else if(leps[i]->LeptonFlavour()==Lepton::ELECTRON){
      this_pt=leps.at(i)->Pt();
      this_eta=((Electron*)leps.at(i))->scEta();
    }else{
      cout << "[SMPValidation::Trigger_SF] It is not lepton"<<endl;
      exit(EXIT_FAILURE);
    }
    double ptmin=this_hist[i]->GetYaxis()->GetXmin();
    double ptmax=this_hist[i]->GetYaxis()->GetXmax();
    double etamin=this_hist[i]->GetXaxis()->GetXmin();
    double etamax=this_hist[i]->GetXaxis()->GetXmax();    
    if(this_pt<ptmin) this_pt=ptmin+0.001;
    if(this_pt>ptmax) this_pt=ptmax-0.001;

    if(etamin>=0) this_eta=fabs(this_eta);
    if(this_eta<etamin) this_eta=etamin+0.001;
    if(this_eta>etamax) this_eta=etamax-0.001;
    triggerSF*=this_hist[i]->GetBinContent(this_hist[i]->FindBin(this_eta,this_pt))+sys*this_hist[i]->GetBinError(this_hist[i]->FindBin(this_eta,this_pt));
  }
  return triggerSF;
}

SMPValidation::SMPValidation(){

}

SMPValidation::~SMPValidation(){

}


