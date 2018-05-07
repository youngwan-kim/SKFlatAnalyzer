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
  if(!IsThisDataFile) ev.SetMCweight(gen_weight);
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
    mu.SetdXY(muon_dxyVTX->at(i));
    mu.SetdZ(muon_dzVTX->at(i));
    mu.SetisPOGTight(muon_isTight->at(i));
    mu.SetIso(muon_PfChargedHadronIsoR04->at(i),muon_PfNeutralHadronIsoR04->at(i),muon_PfGammaIsoR04->at(i),muon_PFSumPUIsoR04->at(i));
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
    el.SetdXY(electron_dxyVTX->at(i));
    el.SetdZ(electron_dzVTX->at(i));
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

    jet.SetAreaAndRho(jet_area->at(i), jet_rho->at(i));
    jet.SetGenFlavours(jet_partonFlavour->at(i), jet_hadronFlavour->at(i));
    std::vector<double> tvs = {
      jet_CSVv2->at(i),
      jet_DeepCSV->at(i),

      -999, //FIXME
      //jet_DeepFlavour->at(i), //FIXME

      jet_CvsL->at(i),
      jet_CvsB->at(i),
      jet_DeepCvsL->at(i),
      jet_DeepCvsB->at(i),
    };
    jet.SetTaggerResults(tvs);
    jet.SetEnergyFractions(jet_chargedHadronEnergyFraction->at(i), jet_neutralHadronEnergyFraction->at(i), jet_neutralEmEnergyFraction->at(i), jet_chargedEmEnergyFraction->at(i));
    jet.SetMultiplicities(jet_chargedMultiplicity->at(i), jet_neutralMultiplicity->at(i));
    jet.SetPileupJetId(jet_PileupJetId->at(i));

    out.push_back(jet);
  }

  return out;

}

bool AnalyzerCore::IsOnZ(double m, double width){
  if( fabs(m-M_Z) < width ) return true;
  else return false;
}

double AnalyzerCore::MT(TLorentzVector a, TLorentzVector b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
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


