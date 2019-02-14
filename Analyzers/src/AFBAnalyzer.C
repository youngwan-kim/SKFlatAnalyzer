#include "AFBAnalyzer.h"

void AFBAnalyzer::executeEventFromParameter(TString channelname,Event* ev){
  std::vector<Muon> muons=GetMuons("POGTightWithTightIso",0.0,2.4);
  std::sort(muons.begin(),muons.end(),PtComparing);
  std::vector<Electron> electrons=GetElectrons("passMediumID",0.0,2.5);
  for(int i=0;i<(int)electrons.size();i++){
    if(fabs(electrons.at(i).scEta())>1.4442&&fabs(electrons.at(i).scEta())<1.566){
      electrons.erase(electrons.begin()+i);
      i--;
    }
  }
  std::sort(electrons.begin(),electrons.end(),PtComparing);
  std::vector<Lepton*> leps;
  double lep0ptcut,lep1ptcut;
  double (MCCorrection::*LeptonID_SF)(TString,double,double,int)=NULL;
  double (MCCorrection::*LeptonISO_SF)(TString,double,double,int)=NULL;
  double (MCCorrection::*LeptonReco_SF)(double,double,int)=NULL;
  double (MCCorrection::*PileUpWeight)(int,int)=(DataYear==2017?&MCCorrection::GetPileUpWeightBySampleName:&MCCorrection::GetPileUpWeight);
  TString LeptonID_key,LeptonID_key_POG,LeptonISO_key,LeptonReco_key,triggerSF_key0,triggerSF_key1;
  if(channelname.Contains("muon")){
    leps=MakeLeptonPointerVector(muons);
    lep0ptcut=20.;
    lep1ptcut=10.;
    LeptonID_SF=&MCCorrection::MuonID_SF;
    LeptonISO_SF=&MCCorrection::MuonISO_SF;
    LeptonID_key="NUM_TightID_DEN_genTracks";
    LeptonISO_key="NUM_TightRelIso_DEN_TightIDandIPCut";
    triggerSF_key0="IsoMu17_POGTight";
    triggerSF_key1="Mu8_POGTight";
 }else if(channelname.Contains("electron")){
    leps=MakeLeptonPointerVector(electrons);
    lep0ptcut=25.;
    lep1ptcut=15.;
    LeptonID_SF=&MCCorrection::ElectronID_SF;
    LeptonReco_SF=&MCCorrection::ElectronReco_SF;
    LeptonID_key="passMediumID_jihkim";
    LeptonID_key_POG="passMediumID";
    triggerSF_key0="Ele23_MediumID";
    triggerSF_key1="Ele12_MediumID";
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
  FillHist(channelname+"/"+prefix+"cutflow",0.5,totalweight,20,0,20);

  /////////////////PUreweight///////////////////
  double PUreweight=1.,PUreweight_up=1.,PUreweight_down=1.;
  if(!IsDATA){
    PUreweight=(mcCorr->*PileUpWeight)(nPileUp,0);
    PUreweight_up=(mcCorr->*PileUpWeight)(nPileUp,1);
    PUreweight_down=(mcCorr->*PileUpWeight)(nPileUp,-1);
    totalweight*=PUreweight;
  }
  FillHist(channelname+"/"+prefix+"cutflow",1.5,totalweight,20,0,20);
  
  /////////////////kinematic selections///////////////////
  if(leps.size()<2) return;
  FillHist(channelname+"/"+prefix+"cutflow",2.5,totalweight,20,0,20);
  if(leps.at(0)->Pt()<lep0ptcut||leps.at(1)->Pt()<lep1ptcut) return;
  FillHist(channelname+"/"+prefix+"cutflow",3.5,totalweight,20,0,20);
  if(leps.size()!=2) return;
  FillHist(channelname+"/"+prefix+"cutflow",4.5,totalweight,20,0,20);
  
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

      double this_RECOSF=LeptonReco_SF?(mcCorr->*LeptonReco_SF)(this_eta,this_pt,0):1.;
      double this_RECOSF_up=LeptonReco_SF?(mcCorr->*LeptonReco_SF)(this_eta,this_pt,1):1.;
      double this_RECOSF_down=LeptonReco_SF?(mcCorr->*LeptonReco_SF)(this_eta,this_pt,-1):1.;
      RECOSF*=this_RECOSF; RECOSF_up*=this_RECOSF_up; RECOSF_down*=this_RECOSF_down;

      double this_IDSF=LeptonID_SF?(mcCorr->*LeptonID_SF)(LeptonID_key,this_eta,this_pt,0):1.;
      double this_IDSF_up=LeptonID_SF?(mcCorr->*LeptonID_SF)(LeptonID_key,this_eta,this_pt,1):1.;
      double this_IDSF_down=LeptonID_SF?(mcCorr->*LeptonID_SF)(LeptonID_key,this_eta,this_pt,-1):1.;
      IDSF*=this_IDSF; IDSF_up*=this_IDSF_up; IDSF_down*=this_IDSF_down;
      
      if(LeptonID_key_POG!=""){
	double this_IDSF_POG=LeptonID_SF?(mcCorr->*LeptonID_SF)(LeptonID_key_POG,this_eta,this_pt,0):1.;
	double this_IDSF_POG_up=LeptonID_SF?(mcCorr->*LeptonID_SF)(LeptonID_key_POG,this_eta,this_pt,1):1.;
	double this_IDSF_POG_down=LeptonID_SF?(mcCorr->*LeptonID_SF)(LeptonID_key_POG,this_eta,this_pt,-1):1.;
	IDSF_POG*=this_IDSF_POG; IDSF_POG_up*=this_IDSF_POG_up; IDSF_POG_down*=this_IDSF_POG_down;
      }

      double this_ISOSF=LeptonISO_SF?(mcCorr->*LeptonISO_SF)(LeptonISO_key,this_eta,this_pt,0):1.;
      double this_ISOSF_up=LeptonISO_SF?(mcCorr->*LeptonISO_SF)(LeptonISO_key,this_eta,this_pt,1):1.;
      double this_ISOSF_down=LeptonISO_SF?(mcCorr->*LeptonISO_SF)(LeptonISO_key,this_eta,this_pt,-1):1.;
      ISOSF*=this_ISOSF; ISOSF_up*=this_ISOSF_up; ISOSF_down*=this_ISOSF_down;
    }
  }
  totalweight*=RECOSF;
  FillHist(channelname+"/"+prefix+"cutflow",5.5,totalweight,20,0,20);
  totalweight*=IDSF;
  FillHist(channelname+"/"+prefix+"cutflow",6.5,totalweight,20,0,20);
  totalweight*=ISOSF;
  FillHist(channelname+"/"+prefix+"cutflow",7.5,totalweight,20,0,20);

  double triggerSF=1.,triggerSF_up=1.,triggerSF_down=1.;
  if(!IsDATA){
    triggerSF*=DileptonTrigger_SF(triggerSF_key0,triggerSF_key1,leps,0);
    triggerSF_up*=DileptonTrigger_SF(triggerSF_key0,triggerSF_key1,leps,1);
    triggerSF_down*=DileptonTrigger_SF(triggerSF_key0,triggerSF_key1,leps,-1);
  }
  totalweight*=triggerSF;
  FillHist(channelname+"/"+prefix+"cutflow",8.5,totalweight,20,0,20);

  //////////////////////no MC for M<50////////////////
  if(((*leps.at(0))+(*leps.at(1))).M()<60) return;
  FillHist(channelname+"/"+prefix+"cutflow",9.5,totalweight,20,0,20);


  //////////////////////PrefileWeight////////////////////
  double prefireweight=1.;
  double prefireweight_up=1.;
  double prefireweight_down=1.;
  if(!IsDATA){
    prefireweight=L1PrefireReweight_Central;
    prefireweight_up=L1PrefireReweight_Up;
    prefireweight_down=L1PrefireReweight_Down;
  }
  totalweight*=prefireweight;
  FillHist(channelname+"/"+prefix+"cutflow",10.5,totalweight,20,0,20);

  totalweight*=zptcor;
  FillHist(channelname+"/"+prefix+"cutflow",11.5,totalweight,20,0,20);

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
    for(int i=0;i<(int)PDFWeights_Error->size();i++){
      map_systematic[Form("pdf%d",i)]=weight*PUreweight*RECOSF*IDSF*ISOSF*triggerSF*prefireweight*zptcor*PDFWeights_Error->at(i);
    }
  }

  
  ///////////////////////fill hists///////////////////////
  if(leps.at(0)->Charge()*leps.at(1)->Charge()<0){
    FillHist(channelname+"/"+prefix+"cutflow",12.5,totalweight,20,0,20);
    double dimass=((*leps.at(0))+(*leps.at(1))).M();
    double dirap=((*leps.at(0))+(*leps.at(1))).Rapidity();
    if(dimass>=massrange[0]&&dimass<massrange[massbinnum]){
      FillHist(channelname+"/"+prefix+"cutflow",13.5,totalweight,20,0,20);
      FillAFBHists(channelname+Form("/OS_m%.0fto%.0f/",massrange[0],massrange[massbinnum])+prefix,"",leps,totalweight);
      FillAFBSystematicHists(channelname+Form("/OS_m%.0fto%.0f/",massrange[0],massrange[massbinnum])+prefix,"",leps,map_systematic);
      for(int iy=0;iy<zptcor_nybin;iy++){
	if(fabs(dirap)>=zptcor_ybin[iy]&&fabs(dirap)<zptcor_ybin[iy+1]){
	  FillAFBHists(channelname+Form("/OS_m%.0fto%.0f_y%.1fto%.1f/",massrange[0],massrange[massbinnum],zptcor_ybin[iy],zptcor_ybin[iy+1])+prefix,"",leps,totalweight);
	  FillAFBSystematicHists(channelname+Form("/OS_m%.0fto%.0f_y%.1fto%.1f/",massrange[0],massrange[massbinnum],zptcor_ybin[iy],zptcor_ybin[iy+1])+prefix,"",leps,map_systematic);
	  for(int im=0;im<massbinnum;im++){
	    if(dimass>=massrange[im]&&dimass<massrange[im+1]){
	      //FillAFBHists(channelname+Form("/OS_m%.0fto%.0f_y%.1fto%.1f/",massrange[im],massrange[im+1],zptcor_ybin[iy],zptcor_ybin[iy+1])+prefix,"",leps,totalweight);
	      //FillAFBSystematicHists(channelname+Form("/OS_m%.0fto%.0f_y%.1fto%.1f/",massrange[im],massrange[im+1],zptcor_ybin[iy],zptcor_ybin[iy+1])+prefix,"",leps,map_systematic);
	    }
	  }
	}
      }
    }
  }else{
    FillHist(channelname+"/"+prefix+"cutflow",14.5,totalweight,20,0,20);
    FillAFBHists(channelname+"/SS/"+prefix,"",leps,totalweight);
    FillAFBSystematicHists(channelname+"/SS/"+prefix,"",leps,map_systematic);
  }
}
AFBAnalyzer::AFBAnalyzer(){
}
AFBAnalyzer::~AFBAnalyzer(){
}
double AFBAnalyzer::GetCosThetaCS(const vector<Lepton*>& leps){
  TLorentzVector *l0,*l1;
  if(leps.at(0)->Charge()<0&&leps.at(1)->Charge()>0){
    l0=leps.at(0);
    l1=leps.at(1);
  }else if(leps.at(0)->Charge()>0&&leps.at(1)->Charge()<0){
    l0=leps.at(1);
    l1=leps.at(0);
  }else{
    cout <<"[AFBAnalyzer::GetCosThetaCS] same sign event"<<endl;
    exit(EXIT_FAILURE);
  }

  TLorentzVector dilepton=*l0+*l1;
  double l0pp=(l0->E()+l0->Pz())/sqrt(2);
  double l0pm=(l0->E()-l0->Pz())/sqrt(2);
  double l1pp=(l1->E()+l1->Pz())/sqrt(2);
  double l1pm=(l1->E()-l1->Pz())/sqrt(2);
  double dimass=dilepton.M();
  double dipt=dilepton.Pt();
  double directon=dilepton.Pz()/fabs(dilepton.Pz());
  return directon*2*(l0pp*l1pm-l0pm*l1pp)/sqrt(dimass*dimass*(dimass*dimass+dipt*dipt));
} 
void AFBAnalyzer::FillAFBHists(TString pre,TString suf,const vector<Lepton*>& leps,double w){
  FillBasicHists(pre,suf,leps,w);
  if(pre.Contains("OS")){
    double cost=GetCosThetaCS(leps);
    FillHist(pre+"costhetaCS"+suf,cost,w,40,-1,1);
    FillHist(pre+"abscosthetaCS"+suf,fabs(cost),w,20,0,1);
    if(pre.Contains("m60to120")){
      double dimass=((*leps.at(0))+(*leps.at(1))).M();
      if(cost>0) FillHist(pre+"forward"+suf,dimass,w,massbinnum,(double*)massrange);
      else FillHist(pre+"backward"+suf,dimass,w,massbinnum,(double*)massrange);
    }
  }
}
void AFBAnalyzer::FillAFBSystematicHists(TString pre,TString suf,const vector<Lepton*>& leps,map<TString,double> map_systematic){
  for(auto iter=map_systematic.begin();iter!=map_systematic.end();iter++){
    FillAFBHists(pre,"_"+iter->first+suf,leps,iter->second);
  }
}
