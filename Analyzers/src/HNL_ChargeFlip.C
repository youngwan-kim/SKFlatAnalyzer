#include "HNL_ChargeFlip.h"

void HNL_ChargeFlip::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}

void HNL_ChargeFlip::executeEvent(){

  AnalyzerParameter param = InitialiseHNLParameter("SignalStudy","UL");

  executeEventFromParameter(param);

}

// check if fitting is useless
/*
 execute under --userflag CFfitval
1. pick up some processes with large xsec (ttbar DY)
2. choose OS ee events
3-1. use CF rate fit function to estimate SS ee from OS ee events
3-2. use CF rate bin values to estimate SS ee from OS ee events

and compare 3-1 and 3-2

*/

void HNL_ChargeFlip::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  double weight(1.);
  Event ev = GetEvent();

  double this_mc_weight = ev.GetTriggerLumi("Full") * MCweight(true, true) * GetKFactor();
  weight *= this_mc_weight;

  // Electrons 
  std::vector<Electron> ElectronColltmp;
  if(param.Electron_Tight_ID.Contains("HEEP")) ElectronColltmp = GetElectrons(param.Electron_Tight_ID,50.,2.5);
  else ElectronColltmp = GetElectrons(param.Electron_Tight_ID,30.,2.5);

  // Muons
  std::vector<Muon> MuonColl; std::vector<Muon> MuonCollVeto;
  if(param.Muon_Tight_ID.Contains("HEEP")) MuonColl = GetMuons(param.Muon_Tight_ID,25.,2.4);
  else MuonColl = GetMuons(param.Muon_Tight_ID,10.,2.4);
  MuonCollVeto = GetMuons(param.Muon_Veto_ID,10.,2.4);
  std::sort(MuonColl.begin(),MuonColl.end(),PtComparing);

  // Gen
  vector<Gen> gens = GetGens();
  vector<Electron> ElectronColl = ElectronPromptOnly(ElectronColltmp,gens,param);
  vector<Muon> PromptMuonColl = MuonPromptOnly(MuonColl,gens,param);
  std::sort(ElectronColl.begin(),ElectronColl.end(),PtComparing);

  // Jet
  vector<Jet> JetColl = GetJets(param.Jet_ID,10.,5.);
  vector<Jet> BJetColl;
  JetTagging::Parameters jtp;
  jtp = JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb);
  BJetColl = SelBJets(JetColl,jtp);

  if(HasFlag("MuonCF")){

    /*
    1. 2 muons (25,25)
    2. 3rd lepton veto
    3. mll > 60
    4. anti-btag

    --> mll plot in MuonCFrate/SS(OS)ij where i,j is pT bin #
    */

    //if(!IsDATA) weight *= MCweight() * ev.GetTriggerLumi("Full");

    if(MuonColl.size()!=2) return;
    if(!(MuonColl.at(0).Pt()>25&&MuonColl.at(1).Pt()>25)) return;
    if(MuonCollVeto.size()>2) return;
    Particle ll = MuonColl.at(0) + MuonColl.at(1) ;
    if(ll.M()<=60) return;
    if(BJetColl.size()>0) return;

    double mllbin[7] = {60.,90.,120.,175.,300.,500.,1000.};
    // (pT1,pT2) = {00,10,11,20,21,22}
    TString l1index("0"); TString l2index("0");
    TString bin("00");

    if(25<=MuonColl.at(0).Pt()&&MuonColl.at(0).Pt()<=100) l1index = "0";
    else if(100<MuonColl.at(0).Pt()&&MuonColl.at(0).Pt()<=200) l1index = "1";
    else if(200<MuonColl.at(0).Pt()) l1index = "2";

    if(25<=MuonColl.at(1).Pt()&&MuonColl.at(1).Pt()<=100) l2index = "0";
    else if(100<MuonColl.at(1).Pt()&&MuonColl.at(1).Pt()<=200) l2index = "1";
    else if(200<MuonColl.at(1).Pt()) l2index = "2";

    bin = l1index+l2index;
    std::map<TString, double> binmap = {{"00",0.},{"10",1.},{"11",2.},{"20",3.},{"21",4.},{"22",5.}};

    int nOS(0); int nSS(0);

    if(MuonColl.at(0).Charge()*MuonColl.at(1).Charge()>0){
      FillHist(param.Name+"/MuonCFrate/SS"+bin,ll.M(),weight,6,mllbin,"m(ll)");
      FillHist(param.Name+"/MuonCFrate/nSS",binmap[bin],weight,6,0.,6.);
    }
    if(MuonColl.at(0).Charge()*MuonColl.at(1).Charge()<0){
      FillHist(param.Name+"/MuonCFrate/OS"+bin,ll.M(),weight,6,mllbin,"m(ll)");
      FillHist(param.Name+"/MuonCFrate/nOS",binmap[bin],weight,6,0.,6.);
    }

  }


  if(HasFlag("CFfitval")){

    // 1. Choose OS ee events ( use same MC samples from previous CFrate extraction iteration )
    // 2. Choose SS ee events from OS ee events ( genuine chargeflipped events )
    //   - save into histo file using ptinv bins ( or ptbins )
    // 3. Comparision 
    //   a) use bin value of previously derived CF rate
    //   b) use fitting function value 
    //   --> check which is better , if discrepancy is not that big one would neglect the fitting procedure

    if(IsDATA) return;
    // OS ee selection
    if( ElectronColl.size() != 2 ) return;
    Particle ll = ElectronColl.at(0) + ElectronColl.at(1);
    if(!( 75 < ll.M() && ll.M() < 105 )) return;
    if( ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge() > 0 ) {
      FillHist(param.Name+"/LeadLeptonPt_fromSS",ElectronColl.at(0).Pt(),1.,34,30.,200.);
      return;
    }
    int flipnum(0); int flipindex(0);

    double fitCFrate0 = GetCFratesFromFunction("HNTightV1",ElectronColl.at(0).Pt(),ElectronColl.at(0).Eta());
    double binCFrate0 = GetCFratesFromHistograms("2016",ElectronColl.at(0).Pt(),ElectronColl.at(0).Eta());
    double fitCFrate1 = GetCFratesFromFunction("HNTightV1",ElectronColl.at(1).Pt(),ElectronColl.at(1).Eta());
    double binCFrate1 = GetCFratesFromHistograms("2016",ElectronColl.at(1).Pt(),ElectronColl.at(1).Eta());
    
    double weight_fit = fitCFrate0/(1.-fitCFrate0) + fitCFrate1/(1.-fitCFrate1);
    double weight_bin = binCFrate0/(1.-binCFrate0) + binCFrate1/(1.-binCFrate1);

    double weight_fit_test = fitCFrate0/(1.-fitCFrate0);
    double weight_bin_test = binCFrate0/(1.-binCFrate0);

    //cout <<  fitCFrate << endl;
    //cout <<  binCFrate << endl;
    FillHist(param.Name+"/LeadLeptonPt_FitFunction",ElectronColl.at(0).Pt(),fitCFrate0,34,30.,200.);
    FillHist(param.Name+"/LeadLeptonPt_BinValue",ElectronColl.at(0).Pt(),binCFrate0,34,30.,200.);
    
    FillHist(param.Name+"/LeadLeptonPt_FitWeightTest",ElectronColl.at(0).Pt(),weight_fit_test,34,30.,200.);
    FillHist(param.Name+"/LeadLeptonPt_BinWeightTest",ElectronColl.at(0).Pt(),weight_bin_test,34,30.,200.);

    FillHist(param.Name+"/LeadLeptonPt_FitWeight",ElectronColl.at(0).Pt(),weight_fit,34,30.,200.);
    FillHist(param.Name+"/LeadLeptonPt_BinWeight",ElectronColl.at(0).Pt(),weight_bin,34,30.,200.);


    /*
    for( unsigned int i=0; i < ElectronColl.size(); i++ ){

      Gen truth_lep = GetGenMatchedLepton(ElectronColl.at(i),gens);
      if(GetLeptonType(ElectronColl.at(i),gens) <= 0 || GetLeptonType(ElectronColl.at(i),gens) >=4) continue;
      if(truth_lep.PID() ==0 ) continue;

      int truth_lep_Charge;
      if(truth_lep.PID() == 11) truth_lep_Charge = -1;
      if(truth_lep.PID() == -11) truth_lep_Charge = 1;
    
      // charge flipped events     
      if(truth_lep_Charge*ElectronColl.at(i).Charge() < 0){
	      
        flipnum += 1; 
        flipindex = i;

      }
      
    }*/

    //if(flipnum == 1) FillHist(param.Name+"/LeadLeptonPt_fromOS",ElectronColl.at(0).Pt(),1.,34,30.,200.);
    //if(flipnum > 0) FillHist(param.Name+"/LeadLeptonPt_fromOS_test",ElectronColl.at(0).Pt(),1.,34,30.,200.);

    /*
    if(ElectronColl.at(flipindex).scEta()<0.8) FillHist(param.Name+"/CFrate/EtaRegion1_OS", 1./ElectronColl.at(flipindex).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
    if(0.8<=ElectronColl.at(flipindex).scEta()&&ElectronColl.at(flipindex).scEta()<1.4442) FillHist(param.Name+"/CFrate/EtaRegion1_OS", 1./ElectronColl.at(flipindex).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
    if(1.556<=ElectronColl.at(flipindex).scEta()&&ElectronColl.at(flipindex).scEta()<2.5)
    FillHist(param.Name+"/CFrate/CFTest", 0., 1., 2, 0., 1.,"bin");
    if(flipnum != 1) return;
    FillHist(param.Name+"/CFrate/CFTest", 1., 1., 2, 0., 1.,"bin");
    if(ElectronColl.at(flipindex).scEta()<0.8) FillHist(param.Name+"/CFrate/EtaRegion1_SS", 1./ElectronColl.at(flipindex).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
    if(0.8<=ElectronColl.at(flipindex).scEta()&&ElectronColl.at(flipindex).scEta()<1.4442) FillHist(param.Name+"/CFrate/EtaRegion2_SS", 1./ElectronColl.at(flipindex).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
    if(1.556<=ElectronColl.at(flipindex).scEta()&&ElectronColl.at(flipindex).scEta()<2.5) FillHist(param.Name+"/CFrate/EtaRegion3_SS", 1./ElectronColl.at(flipindex).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
    */
    
  }


  if(HasFlag("CFratePtInverse") || HasFlag("CFratePtBinned")){

    if(IsDATA) return;
    
    for(unsigned int i=0 ; i < ElectronColl.size() ; i++){
      Gen truth_lep = GetGenMatchedLepton(ElectronColl.at(i),gens);
      if(GetLeptonType(ElectronColl.at(i),gens) <= 0 || GetLeptonType(ElectronColl.at(i),gens) >=4) continue;
      if(truth_lep.PID() ==0 ) continue;

      int truth_lep_Charge;
      if(truth_lep.PID() == 11) truth_lep_Charge = -1;
      else if(truth_lep.PID() == -11) truth_lep_Charge = 1;
      // 
      if(HasFlag("CFratePtInverse")){
        if(abs(ElectronColl.at(i).scEta())<0.8){
          FillHist(param.Name+"/CFrate/EtaRegion1_Denom", 1./ElectronColl.at(i).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
          if(truth_lep_Charge*ElectronColl.at(i).Charge()<0){
            FillHist(param.Name+"/CFrate/EtaRegion1_Num", 1./ElectronColl.at(i).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
          }
        }
        else if(0.8<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<1.4442){
          FillHist(param.Name+"/CFrate/EtaRegion2_Denom", 1./ElectronColl.at(i).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
          if(truth_lep_Charge*ElectronColl.at(i).Charge()<0){
            FillHist(param.Name+"/CFrate/EtaRegion2_Num", 1./ElectronColl.at(i).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
          }
        }
        else if(1.556<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<2.5){
          FillHist(param.Name+"/CFrate/EtaRegion3_Denom", 1./ElectronColl.at(i).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
          if(truth_lep_Charge*ElectronColl.at(i).Charge()<0){
            FillHist(param.Name+"/CFrate/EtaRegion3_Num", 1./ElectronColl.at(i).Pt(), 1., 40, 0., 0.04,"#pT^{-1}");
          }
        }

        FillHist(param.Name+"/CFrate_Eta/Denom", ElectronColl.at(i).scEta(), 1., 50, -2.5, 2.5,"scEta");
        FillHist(param.Name+"/CFrate_IP/Denom", ElectronColl.at(i).dXY(), 1., 200, -0.1, 0.1,"dXY");
        FillHist(param.Name+"/CFrate_2D/Denom", 1./ElectronColl.at(i).Pt(), ElectronColl.at(i).scEta(), 1., 40, 0., 0.04, 50, -2.5, 2.5);
        if(truth_lep_Charge*ElectronColl.at(i).Charge()<0){
          FillHist(param.Name+"/CFrate_Eta/Num", ElectronColl.at(i).scEta(), 1., 50, -2.5, 2.5);
          FillHist(param.Name+"/CFrate_IP/Num", ElectronColl.at(i).dXY(), 1., 200, -0.1, 0.1);
          FillHist(param.Name+"/CFrate_2D/Num", 1./ElectronColl.at(i).Pt(), ElectronColl.at(i).scEta(), 1., 40, 0., 0.04, 50, -2.5, 2.5);
        }
      }
      
      else if(HasFlag("CFratePtBinned")){
        double pTbin[10] = {0.,20.,30.,40.,50.,100.,200.,500.,1000.,2000.};
        //double pTbin[9] = {0.,20.,30.,40.,50.,100.,200.,500.,1000.};
        int nbin = 9;//pTbin.size()-1;
        if(abs(ElectronColl.at(i).scEta())<0.8){
          FillHist(param.Name+"/CFrate/EtaRegion1_Denom", ElectronColl.at(i).Pt(), 1.,nbin, pTbin,"pT");
          if(truth_lep_Charge*ElectronColl.at(i).Charge()<0){
            FillHist(param.Name+"/CFrate/EtaRegion1_Num", ElectronColl.at(i).Pt(), 1.,nbin, pTbin,"pT");
          }
        }
        else if(0.8<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<1.4442){
          FillHist(param.Name+"/CFrate/EtaRegion2_Denom", ElectronColl.at(i).Pt(), 1.,nbin, pTbin,"pT");
          if(truth_lep_Charge*ElectronColl.at(i).Charge()<0){
            FillHist(param.Name+"/CFrate/EtaRegion2_Num", ElectronColl.at(i).Pt(), 1.,nbin, pTbin,"pT");
          }
        }
        else if(1.556<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<2.5){
          FillHist(param.Name+"/CFrate/EtaRegion3_Denom", ElectronColl.at(i).Pt(), 1.,nbin, pTbin,"pT");
          if(truth_lep_Charge*ElectronColl.at(i).Charge()<0){
            FillHist(param.Name+"/CFrate/EtaRegion3_Num", ElectronColl.at(i).Pt(), 1.,nbin, pTbin,"pT");
          }
        }
      }
    }
  }
}

HNL_ChargeFlip::HNL_ChargeFlip(){

}

HNL_ChargeFlip::~HNL_ChargeFlip(){

}


