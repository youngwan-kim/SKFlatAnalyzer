#include "ttbar_test.h"

//========================
// TODO SelectElectrons
// TODO top pt reweighting ( mcCorr->GetTopPt )
// TODO jet cleaning ( JetsVeto... dR=0.4,0.8 )
// TODOne cut-flow histogram ( #of events ... ? )
// TODO Z+jet suppression by giving DY SR mass cut ( 91\pm15 Gev )

void ttbar_test::initializeAnalyzer(){
	
  MuonIDs = {
    "POGMedium","POGTight"
  };

  MuonIDSFKeys = {
    "NUM_MediumID_DEN_genTracks","NUM_TightID_DEN_genTracks"
  };

  MuonTriggers = {
    "HLT_IsoMu24_v","HLT_IsoTkMu24_v"
  };

  TriggerSafePtCut = 26.;

  //===================
  //==== b tagging ====
  //===================

  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back(JetTagging::Parameters(JetTagging::DeepCSV, 
					JetTagging::Medium, 
					JetTagging::incl,
					JetTagging::comb));

  mcCorr->SetJetTaggingParameters(jtps);
}

void ttbar_test::executeEvent(){

  AllMuons = GetAllMuons();
  AllJets = GetAllJets();
  AllElectrons = GetAllElectrons();
  Gens = GetGens(); // Gen used for top pt reweighting

  weight_Prefire = GetPrefireWeight(0);

  AnalyzerParameter param;

  for(unsigned int it_MuonID=0; it_MuonID<MuonIDs.size(); it_MuonID++){

    TString MuonID = MuonIDs.at(it_MuonID);
    TString MuonIDSFKey = MuonIDSFKeys.at(it_MuonID);
//  TString MuonTriggerSFKey = MuonTriggers.at(it_MuonID);

    param.Clear();
    param.Name = MuonID+"_Central";
    param.Muon_Tight_ID = MuonID;
    param.Muon_ID_SF_Key = MuonIDSFKey;
//  param.Muon_Trigger_SF_Key = MuonTriggerSFKey;

    param.Jet_ID = "tight";
    
    executeEventFromParameter(param);

  }  

}

void ttbar_test::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  if(!(ev.PassTrigger(MuonTriggers))) return;

  vector<Muon> muons = SelectMuons(AllMuons,param.Muon_Tight_ID,20.,2.4);
  vector<Jet> jets = SelectJets(AllJets,param.Jet_ID,30.,2.4);



  vector<Electron> electrons_veto = SelectElectrons(AllElectrons,"passVetoID",20,2.4); 
  vector<Muon> muons_veto = SelectMuons(AllMuons,"POGLoose",20,2.4);

  vector<Jet> jets_dR4 = JetsVetoLeptonInside(jets,electrons_veto,muons_veto,0.4);
  vector<Jet> bjets;

  std::sort(muons.begin(),muons.end(),PtComparing);
  std::sort(jets_dR4.begin(),jets_dR4.end(),PtComparing);

  int NBJets_dR4_NoSF(0); // int NBJets_dR8_NoSF(0);

  JetTagging::Parameters jtp_DeepCSV_Medium = JetTagging::Parameters(JetTagging::DeepCSV,
								     JetTagging::Medium,
								     JetTagging::incl,
								     JetTagging::comb);

  double btagWeight_dR4 = mcCorr->GetBTaggingReweight_1a(jets_dR4,jtp_DeepCSV_Medium);
//  cout << "[btagWeight::]" << btagWeight_dR4 << "\n" ;

  for(unsigned int j=0; j < jets_dR4.size(); j++){

    double this_discr = jets_dR4.at(j).GetTaggerResult(JetTagging::DeepCSV);

    if( this_discr > mcCorr->GetJetTaggingCutValue(JetTagging::DeepCSV, JetTagging::Medium)){
      NBJets_dR4_NoSF++;
      bjets.push_back(jets_dR4.at(j));
    }

  }

  std::sort(bjets.begin(),bjets.end(),PtComparing);

  //=========================
  //==== Event Selection ====
  //=========================

  //==== full events
  FillHist("CutFlow",0.,1.,8,0.,8);

  //==== Dimuon selection
  if(muons.size()!=2) return;
  FillHist("CutFlow",1.,1.,8,0,8); //test

  //==== OS selection
  if(muons.at(0).Charge()*muons.at(1).Charge()>0) return;
  FillHist("CutFlow",2.,1.,8,0,8);

  //==== leading muon pt is trigger safe
  if(muons.at(0).Pt()<=TriggerSafePtCut) return;
  FillHist("CutFlow",3.,1.,8,0,8);

  //==== #jets>1
  if(jets_dR4.size()<2) return;
  FillHist("CutFlow",4.,1.,8,0,8);

  //==== #bjets>0
  if(NBJets_dR4_NoSF==0) return;
  FillHist("CutFlow",5.,1.,8,0,8);

  Particle Cand = muons.at(0)+muons.at(1);
  if(Cand.M()>75 && Cand.M()<107) return;
  FillHist("CutFlow",6,1.,8,0,8); 

  if(muons.at(0).RelIso()>0.05 || muons.at(1).RelIso()>0.05) return;
  FillHist("CutFlow",7,1,8,0,8); 

  double trkpt_sum = 0;
  vector<double> muon_iso; 

//  cout << "AllMuonsSize :" << AllMuons.size() << "\n" ;

/*  for(unsigned int i=0 ; i<allmuons.size() ; i++){
  
    particles.push_back(allmuons.at(i));
  
  }

  for(unsigned int i=0 ; i<allelectrons.size() ; i++){ 
    
    particles.push_back(allelectrons.size());
  
  }
*/

  for(unsigned int i=0 ; i<muons.size(); i++){

    for(unsigned int j=0 ; j<AllMuons.size(); j++){
      
      if(muons.at(i).DeltaR(AllMuons.at(j))<0.3 && muons.at(i).DeltaR(AllMuons.at(j))>0 ) trkpt_sum += AllMuons.at(j).Pt();

    }

    for(unsigned int j=0 ; j<AllElectrons.size(); j++){

      if(muons.at(i).DeltaR(AllElectrons.at(j))<0.3 ) trkpt_sum +=AllElectrons.at(j).Pt();

    }
    
    muon_iso.push_back(trkpt_sum/muons.at(i).Pt()); 
    cout << "muoniso(" << i << "):" << muon_iso.at(i) << "\n" ;
    cout << "muon.trkiso(" << i << "):" << muons.at(i).TrkIso() << "\n" ;
    cout << "muon.reliso(" << i << "):" << muons.at(i).RelIso() << "\n" ;     

  } 
    



  //=========================
  //==== Event weighting ====
  //=========================
  
  double weight = 1.;
  if(!IsDATA){

    double topptWeight = mcCorr->GetTopPtReweight(Gens);
//    std::cout << "topptWeight : " << topptWeight << "\n" ;

    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full");
    weight *= ev.MCweight();
    weight *= topptWeight;
    weight *= weight_Prefire;
//    cout << "[weight1::]" << weight << "\n" ;
    FillHist(param.Name+"/Leading_JetPt"+param.Name,jets_dR4.at(0).Pt(),weight,30,0.,450.);
    FillHist(param.Name+"/Leading_BJetPt"+param.Name,bjets.at(0).Pt(),weight,30,0.,450.);
    FillHist(param.Name+"/NJets_"+param.Name,jets.size(),weight,10.,0.,10.);
    FillHist(param.Name+"/NJets_dR4_"+param.Name,jets_dR4.size(),weight,10,0,10);
    FillHist(param.Name+"/NBJets_dR4_NoSF_"+param.Name,NBJets_dR4_NoSF,weight,10.,0.,10.);
    FillHist(param.Name+"/NBJets_dR4_SF_"+param.Name,NBJets_dR4_NoSF,weight*btagWeight_dR4,10.,0.,10.);

    FillHist(param.Name+"/MET_"+param.Name,METv.Pt(),weight,25,0,250);
    FillHist(param.Name+"/METphi_"+param.Name,METv.Phi(),weight,60,-3.,3.);

      for(unsigned int i=0; i<muons.size();i++){
        
        double this_idsf = mcCorr->MuonID_SF(param.Muon_ID_SF_Key,
                                             muons.at(i).Eta(),
                                             muons.at(i).MiniAODPt());

	double this_isosf = mcCorr->MuonISO_SF("NUM_TightRelIso_DEN_TightIDandIPCut",
					       muons.at(i).Eta(),
					       muons.at(i).MiniAODPt());

	double this_trigsf = 1.;
	
	if(param.Muon_Tight_ID == "POGTight"){

	  this_trigsf = mcCorr->MuonTrigger_SF("POGTight","IsoMu24",muons);
	
	}
	
	weight *= this_idsf*this_isosf*this_trigsf; 
	cout << "[weight2::]" << weight << "\n" ;
      }

  }

  //=========================
  //==== Fill Histograms ====
  //=========================

/*  for(unsigned int j=0; j < jets_dR4.size(); j++){

    TString itoa = TString::Itoa(j,10);
    double this_discr = jets_dR4.at(j).GetTaggerResult(JetTagging::DeepCSV);

    FillHist(param.Name+"/Jets_dR4"+"/Jets_dR4_"+itoa+"_DeepCSV_"+param.Name,this_discr,weight,120,0.,1.2);

  } */

  if(IsDATA){ 

    FillHist(param.Name+"/MET_"+param.Name,METv.Pt(),weight,25,0,250);
    FillHist(param.Name+"/METphi_"+param.Name,METv.Phi(),weight,60,-3.,3.);
    FillHist(param.Name+"/Leading_JetPt"+param.Name,jets_dR4.at(0).Pt(),weight,30,0.,450.);
    FillHist(param.Name+"/Leading_BJetPt"+param.Name,bjets.at(0).Pt(),weight,30,0.,450.);
    FillHist(param.Name+"/NJets_"+param.Name,jets.size(),weight,10.,0.,10.);
    FillHist(param.Name+"/NJets_dR4_"+param.Name,jets_dR4.size(),weight,10,0,10);
    FillHist(param.Name+"/NBJets_dR4_NoSF_"+param.Name,NBJets_dR4_NoSF,weight,10.,0.,10.);
    FillHist(param.Name+"/NBJets_dR4_SF_"+param.Name,NBJets_dR4_NoSF,weight*btagWeight_dR4,10.,0.,10.);

    FillHist(param.Name+"/MET_"+param.Name,METv.Pt(),weight,25,0,250);
    FillHist(param.Name+"/METphi_"+param.Name,METv.Phi(),weight,60,-3.,3.);

  }


    FillHist(param.Name+"/Muon_Pt_"+param.Name,muons.at(0).Pt(),weight,25,0.,250.);
    FillHist(param.Name+"/Muon_Eta_"+param.Name,muons.at(0).Eta(),weight,30,-3.0,3.0);
    FillHist(param.Name+"/Muon_Phi_"+param.Name,muons.at(0).Phi(),weight,35,-3.5,3.5);

}

ttbar_test::ttbar_test(){

}

ttbar_test::~ttbar_test(){

}


