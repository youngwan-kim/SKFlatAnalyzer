#include "AnalyzerCore.h"

AnalyzerCore::AnalyzerCore(){



}

AnalyzerCore::~AnalyzerCore(){

  for(std::map< TString, TH1D* >::iterator mapit = maphist_TH1D.begin(); mapit!=maphist_TH1D.end(); mapit++){
    delete mapit->second;
  }
  maphist_TH1D.clear();

  outfile->Close();
}

Event AnalyzerCore::GetEvent(){

  Event ev;
  if(!IsDATA) ev.SetMCweight(gen_weight);
  ev.SetTrigger(*HLT_TriggerName,*HLT_TriggerFired);
  ev.SetMET(pfMET_pt,pfMET_Px,pfMET_Py);

  return ev;

}

std::vector<Muon> AnalyzerCore::GetAllMuons(){

  std::vector<Muon> out;
  for(unsigned int i=0; i<muon_pt->size(); i++){

    Muon mu;
    double rc = muon_roch_sf->at(i);
    double rc_err = muon_roch_sf_up->at(i);

    mu.SetMiniAODPt(muon_pt->at(i));
    mu.SetPtEtaPhiM(muon_pt->at(i)*rc, muon_eta->at(i), muon_phi->at(i), muon_mass->at(i));
    mu.SetMomentumUpDown( (rc+rc_err)*muon_pt->at(i), (rc-rc_err)*muon_pt->at(i) );
    mu.SetCharge(muon_charge->at(i));
    mu.SetdXY(muon_dxyVTX->at(i), muon_dxyerrVTX->at(i));
    mu.SetdZ(muon_dzVTX->at(i), muon_dzerrVTX->at(i));
    mu.SetIP3D(muon_3DIPVTX->at(i), muon_3DIPerrVTX->at(i));
    mu.SetisPOGTight(muon_isTight->at(i));
    mu.SetisPOGMedium(muon_isMedium->at(i));
    mu.SetisPOGLoose(muon_isLoose->at(i));
    mu.SetIso(muon_PfChargedHadronIsoR04->at(i),muon_PfNeutralHadronIsoR04->at(i),muon_PfGammaIsoR04->at(i),muon_PFSumPUIsoR04->at(i));

    //==== Should be set after Eta is set
    mu.SetMiniIso(
      muon_PfChargedHadronMiniIso->at(i), 
      muon_PfNeutralHadronMiniIso->at(i), 
      muon_PfGammaMiniIso->at(i), 
      muon_PFSumPUMiniIso->at(i),
      Rho,
      mu.EA()
    );

    out.push_back(mu);

  }
  return out;

}

std::vector<Muon> AnalyzerCore::GetMuons(TString id, double ptmin, double fetamax){

  std::vector<Muon> muons = GetAllMuons();
  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    Muon this_muon=muons.at(i);
    if(!( this_muon.Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << this_muon.Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(this_muon.Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(this_muon.Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( this_muon.PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back(this_muon);
  }
  return out;

}

std::vector<Electron> AnalyzerCore::GetAllElectrons(){

  std::vector<Electron> out;
  for(unsigned int i=0; i<electron_pt->size(); i++){

    Electron el;
    el.SetPtEtaPhiE(electron_pt->at(i), electron_eta->at(i), electron_phi->at(i), electron_Energy->at(i));
    el.SetSC(electron_scEta->at(i), electron_scPhi->at(i));
    el.SetCharge(electron_charge->at(i));
    el.SetdXY(electron_dxyVTX->at(i), electron_dxyerrVTX->at(i));
    el.SetdZ(electron_dzVTX->at(i), electron_dzerrVTX->at(i));
    el.SetIP3D(electron_3DIPVTX->at(i), electron_3DIPerrVTX->at(i));
    el.SetMVA(electron_MVAIso->at(i), electron_MVANoIso->at(i));
    //el.SetPassConversionVeto(electron_passConversionVeto->at(i)); //FIXME not filled in v946p1_2..
    el.SetNMissingHits(electron_mHits->at(i));

    std::vector<bool> ids = {
      electron_passVetoID->at(i),
      electron_passLooseID->at(i),
      electron_passMediumID->at(i),
      electron_passTightID->at(i),
      electron_passMVAID_noIso_WP80->at(i),
      electron_passMVAID_noIso_WP90->at(i),
      electron_passMVAID_iso_WP80->at(i),
      electron_passMVAID_iso_WP90->at(i),
      electron_passHEEPID->at(i),
    };
    el.SetPOGIDs(ids);
    el.SetRelPFIso_Rho(electron_RelPFIso_Rho->at(i));

    //==== Should be ran after SCeta is set
    el.SetMiniIso(
      electron_chMiniIso->at(i),
      electron_nhMiniIso->at(i),
      electron_phMiniIso->at(i),
      electron_puChMiniIso->at(i),
      Rho,
      el.EA()
    );

    out.push_back(el);

  }
  return out;

}

std::vector<Electron> AnalyzerCore::GetElectrons(TString id, double ptmin, double fetamax){

  std::vector<Electron> electrons = GetAllElectrons();
  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    Electron this_electron= electrons.at(i);
    if(!( this_electron.Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << this_electron.Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(this_electron.Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(this_electron.Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( this_electron.PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back(this_electron);
  }
  return out;

}

std::vector<Lepton *> AnalyzerCore::MakeLeptonPointerVector(std::vector<Muon>& muons){

  std::vector<Lepton *> out;
  for(unsigned int i=0; i<muons.size(); i++){
    Lepton *l = (Lepton *)(&muons.at(i));
    //cout << muons.at(i).Pt() << " -> " << l->Pt() << endl;
    out.push_back(l);
  }
  return out;

}
std::vector<Lepton *> AnalyzerCore::MakeLeptonPointerVector(std::vector<Electron>& electrons){

  std::vector<Lepton *> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    Lepton *l = (Lepton *)(&electrons.at(i));
    out.push_back(l);
  }
  return out;

}

std::vector<Jet> AnalyzerCore::GetAllJets(){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jet_pt->size(); i++){
    Jet jet;
    jet.SetPtEtaPhiM(jet_pt->at(i), jet_eta->at(i), jet_phi->at(i), jet_m->at(i));
    jet.SetCharge(jet_charge->at(i));

    jet.SetArea(jet_area->at(i));
    jet.SetGenFlavours(jet_partonFlavour->at(i), jet_hadronFlavour->at(i));
    std::vector<double> tvs = {
      jet_CSVv2->at(i),
      jet_DeepCSV->at(i),
      jet_DeepCvsL->at(i),
      jet_DeepCvsB->at(i),
      jet_DeepFlavour_b->at(i),
      jet_DeepFlavour_bb->at(i),
      jet_DeepFlavour_lepb->at(i),
      jet_DeepFlavour_c->at(i),
      jet_DeepFlavour_uds->at(i),
      jet_DeepFlavour_g->at(i),
      jet_CvsL->at(i),
      jet_CvsB->at(i),
    };
    jet.SetTaggerResults(tvs);
    jet.SetEnergyFractions(jet_chargedHadronEnergyFraction->at(i), jet_neutralHadronEnergyFraction->at(i), jet_neutralEmEnergyFraction->at(i), jet_chargedEmEnergyFraction->at(i));
    jet.SetMultiplicities(jet_chargedMultiplicity->at(i), jet_neutralMultiplicity->at(i));
    jet.SetPileupJetId(jet_PileupJetId->at(i));

    out.push_back(jet);
  }

  return out;

}

std::vector<Jet> AnalyzerCore::GetJets(TString id, double ptmin, double fetamax){

  std::vector<Jet> jets = GetAllJets();
  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    Jet this_jet= jets.at(i);
    if(!( this_jet.Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << this_jet.Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(this_jet.Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(this_jet.Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( this_jet.PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back(this_jet);
  }
  return out;

}

std::vector<Gen> AnalyzerCore::GetGens(){

  std::vector<Gen> out;
  for(unsigned int i=0; i<gen_pt->size(); i++){

    Gen gen;

    gen.SetIsEmpty(false);
    gen.SetPtEtaPhiE(gen_pt->at(i), gen_eta->at(i), gen_phi->at(i), gen_E->at(i));
    gen.SetCharge(gen_charge->at(i));
    gen.SetPIDandStatus(gen_PID->at(i), gen_status->at(i));
    gen.SetMother(gen_mother_PID->at(i), gen_mother_index->at(i));
    gen.SetGenStatusFlag_isPrompt( gen_isPrompt->at(i) );
    gen.SetGenStatusFlag_isPromptFinalState( gen_isPromptFinalState->at(i) );
    gen.SetGenStatusFlag_isTauDecayProduct( gen_isTauDecayProduct->at(i) );
    gen.SetGenStatusFlag_isPromptTauDecayProduct( gen_isPromptTauDecayProduct->at(i) );
    gen.SetGenStatusFlag_isDirectPromptTauDecayProductFinalState( gen_isDirectPromptTauDecayProductFinalState->at(i) );
    gen.SetGenStatusFlag_isHardProcess( gen_isHardProcess->at(i) );
    gen.SetGenStatusFlag_isLastCopy( gen_isLastCopy->at(i) );
    gen.SetGenStatusFlag_isLastCopyBeforeFSR( gen_isLastCopyBeforeFSR->at(i) );
    gen.SetGenStatusFlag_isPromptDecayed( gen_isPromptDecayed->at(i) );
    gen.SetGenStatusFlag_isDecayedLeptonHadron( gen_isDecayedLeptonHadron->at(i) );
    gen.SetGenStatusFlag_fromHardProcessBeforeFSR( gen_fromHardProcessBeforeFSR->at(i) );
    gen.SetGenStatusFlag_fromHardProcessDecayed( gen_fromHardProcessDecayed->at(i) );
    gen.SetGenStatusFlag_fromHardProcessFinalState( gen_fromHardProcessFinalState->at(i) );
    gen.SetGenStatusFlag_isMostlyLikePythia6Status3( gen_isMostlyLikePythia6Status3->at(i) );

    out.push_back(gen);

  }

  return out;

}

bool AnalyzerCore::PassMETFilter(){

  if(!Flag_goodVertices) return false;
  if(!Flag_globalTightHalo2016Filter) return false;
  if(!Flag_HBHENoiseFilter) return false;
  if(!Flag_HBHENoiseIsoFilter) return false;
  if(!Flag_EcalDeadCellTriggerPrimitiveFilter) return false;
  if(!Flag_BadPFMuonFilter) return false;
  if(!Flag_BadChargedCandidateFilter) return false;
  if(IsDATA && !Flag_eeBadScFilter) return false;
  if(!Flag_ecalBadCalibFilter) return false;

  return true;

}

bool AnalyzerCore::IsOnZ(double m, double width){
  if( fabs(m-M_Z) < width ) return true;
  else return false;
}

double AnalyzerCore::MT(TLorentzVector a, TLorentzVector b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
}

void AnalyzerCore::PrintGen(std::vector<Gen> gens){

  cout << "===========================================================" << endl;
  cout << "RunNumber:EventNumber = " << run << ":" << event << endl;
  cout << "index\tPID\tStatus\tMIdx\tMPID\tPt\tEta\tPhi\tM" << endl;
  for(unsigned int i=0; i<gens.size(); i++){
    Gen gen = gens.at(i);
    cout << i << "\t" << gen.PID() << "\t" << gen.Status() << "\t" << gen.MotherIndex() << "\t" << gen.MotherPID() << "\t";
    printf("%.2f\t%.2f\t%.2f\t%.2f\n",gen.Pt(), gen.Eta(), gen.Phi(), gen.M());

  }

}

Gen AnalyzerCore::GetGenMatchedLepton(Lepton lep, std::vector<Gen> gens){

  int reco_PID = -999;
  if(lep.LeptonFlavour()==Lepton::ELECTRON) reco_PID = 11;
  else if(lep.LeptonFlavour()==Lepton::MUON) reco_PID = 13;
  else{
    cout << "[AnalyzerCore::GetLeptonType] input lepton flavour not set" << endl;
    exit(EXIT_FAILURE);
  }

  double min_dR = 0.3;
  Gen gen_closest;
  bool GenMatched = false;
  for(unsigned int i=0; i<gens.size(); i++){

    Gen gen = gens.at(i);

    //==== PID & deltaR matching first
    if( abs(gen.PID() ) != reco_PID ) continue;

    if( gen.DeltaR( lep ) < min_dR ){
      GenMatched = true;
      min_dR = gen.DeltaR( lep ) ;
      gen_closest = gen;
    }

  }

  return gen_closest;

}

int AnalyzerCore::GetLeptonType(Lepton lep, std::vector<Gen> gens){

  Gen gen_closest = GetGenMatchedLepton(lep, gens);

  //==== No mathced gen-lepton -> fake
  if(gen_closest.IsEmpty()) return -1;

  //==== Prompt
  if( gen_closest.isPromptFinalState() ){
    return 1;
  }
  //=== From Tau
  else if( gen_closest.isDirectPromptTauDecayProductFinalState() ){
    return 2;
  }
  else{
    //=== Fake
    return -1;
  }
    
}

TH1D* AnalyzerCore::GetHist1D(TString histname){

  TH1D *h = NULL;
  std::map<TString, TH1D*>::iterator mapit = maphist_TH1D.find(histname);
  if(mapit != maphist_TH1D.end()) return mapit->second;

  return h;

}

void AnalyzerCore::FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max){

  TH1D *this_hist = GetHist1D(histname);
  if( !this_hist ){
    this_hist = new TH1D(histname, "", n_bin, x_min, x_max);
    maphist_TH1D[histname] = this_hist;
  }

  this_hist->Fill(value, weight);

}

TH1D* AnalyzerCore::JSGetHist1D(TString suffix, TString histname){

  TH1D *h = NULL;

  std::map< TString, std::map<TString, TH1D*> >::iterator mapit = JSmaphist_TH1D.find(suffix);
  if(mapit==JSmaphist_TH1D.end()){
    return h;
  }
  else{

    std::map<TString, TH1D*> this_maphist = mapit->second;
    std::map<TString, TH1D*>::iterator mapitit = this_maphist.find(histname);
    if(mapitit != this_maphist.end()) return mapitit->second;

  }

  return h;

}

void AnalyzerCore::JSFillHist(TString suffix, TString histname, double value, double weight, int n_bin, double x_min, double x_max){

  TH1D *this_hist = JSGetHist1D(suffix, histname);
  if( !this_hist ){

    this_hist = new TH1D(histname, "", n_bin, x_min, x_max);
    (JSmaphist_TH1D[suffix])[histname] = this_hist;

  }

  this_hist->Fill(value, weight);

}

void AnalyzerCore::WriteHist(){

  outfile->cd();
  for(std::map< TString, TH1D* >::iterator mapit = maphist_TH1D.begin(); mapit!=maphist_TH1D.end(); mapit++){
    mapit->second->Write();
  }

  outfile->cd();
  for(std::map< TString, std::map<TString, TH1D*> >::iterator mapit=JSmaphist_TH1D.begin(); mapit!=JSmaphist_TH1D.end(); mapit++){

    TString this_suffix = mapit->first;
    std::map< TString, TH1D* > this_maphist = mapit->second;

    outfile->mkdir(this_suffix);
    outfile->cd(this_suffix);

    for(std::map< TString, TH1D* >::iterator mapit = this_maphist.begin(); mapit!=this_maphist.end(); mapit++){
      mapit->second->Write();
    }

    outfile->cd();

  }

}


void AnalyzerCore::FillLeptonPlots(std::vector<Lepton *> leps, TString this_region, double weight){

  for(unsigned int i=0; i<leps.size(); i++){

    TString this_itoa = TString::Itoa(i,10);

    JSFillHist(this_region, "Lepton_"+this_itoa+"_Pt_"+this_region, leps[i]->Pt(), weight, 500, 0., 500.);
    JSFillHist(this_region, "Lepton_"+this_itoa+"_Eta_"+this_region, leps[i]->Eta(), weight, 60, -3., 3.);
    JSFillHist(this_region, "Lepton_"+this_itoa+"_RelIso_"+this_region, leps[i]->RelIso(), weight, 100, 0., 1.);
    JSFillHist(this_region, "Lepton_"+this_itoa+"_MiniRelIso_"+this_region, leps[i]->MiniRelIso(), weight, 100, 0., 1.);

    JSFillHist(this_region, "Lepton_"+this_itoa+"_dXY_"+this_region, fabs(leps[i]->dXY()), weight, 500, 0., 0.05);
    JSFillHist(this_region, "Lepton_"+this_itoa+"_dXYSig_"+this_region, fabs(leps[i]->dXY()/leps[i]->dXYerr()), weight, 100, 0., 10);
    JSFillHist(this_region, "Lepton_"+this_itoa+"_dZ_"+this_region, fabs(leps[i]->dZ()), weight, 500, 0., 0.5);
    JSFillHist(this_region, "Lepton_"+this_itoa+"_dZSig_"+this_region, fabs(leps[i]->dZ()/leps[i]->dZerr()), weight, 100, 0., 10);
    JSFillHist(this_region, "Lepton_"+this_itoa+"_IP3D_"+this_region, fabs(leps[i]->IP3D()), weight, 500, 0., 0.5);
    JSFillHist(this_region, "Lepton_"+this_itoa+"_IP3DSig_"+this_region, fabs(leps[i]->IP3D()/leps[i]->IP3Derr()), weight, 100, 0., 10);


  }

}

