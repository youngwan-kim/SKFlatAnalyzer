#include "AnalysisCore.h"

AnalysisCore::AnalysisCore(){

}

AnalysisCore::~AnalysisCore(){

  for(std::map< TString, TH1D* >::iterator mapit = maphist_TH1D.begin(); mapit!=maphist_TH1D.end(); mapit++){
    delete mapit->second;
  }
  maphist_TH1D.clear();

  outfile->Close();
}

Event AnalysisCore::GetEvent(){

  Event ev;
  if(!IsThisDataFile) ev.SetMCweight(gen_weight);
  ev.SetTrigger(*HLT_TriggerName,*HLT_TriggerFired);
  ev.SetMET(pfMET_pt,pfMET_Px,pfMET_Py);

  return ev;

}

std::vector<Muon> AnalysisCore::GetAllMuons(){

  std::vector<Muon> out;
  for(unsigned int i=0; i<muon_pt->size(); i++){

    Muon mu;
    mu.SetPtEtaPhiM(muon_pt->at(i), muon_eta->at(i), muon_phi->at(i), muon_mass->at(i));
    mu.SetCharge(muon_charge->at(i));
    mu.SetdXY(muon_dxyVTX->at(i));
    mu.SetdZ(muon_dzVTX->at(i));
    mu.SetisPOGTight(muon_isTight->at(i));
    mu.SetIso(muon_PfChargedHadronIsoR04->at(i),muon_PfNeutralHadronIsoR04->at(i),muon_PfGammaIsoR04->at(i),muon_PFSumPUIsoR04->at(i));
    out.push_back(mu);

  }
  return out;

}

std::vector<Muon> AnalysisCore::GetMuons(TString id, double ptmin, double fetamax){

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

std::vector<Electron> AnalysisCore::GetAllElectrons(){

  std::vector<Electron> out;
  for(unsigned int i=0; i<electron_pt->size(); i++){


    Electron el;
    el.SetPtEtaPhiE(electron_pt->at(i), electron_eta->at(i), electron_phi->at(i), electron_Energy->at(i));
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

std::vector<Electron> AnalysisCore::GetElectrons(TString id, double ptmin, double fetamax){

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

void AnalysisCore::FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max){

  TH1D *this_hist = GetHist1D(histname);
  if( !this_hist ){
    this_hist = new TH1D(histname, "", n_bin, x_min, x_max);
    maphist_TH1D[histname] = this_hist;
  }

  this_hist->Fill(value, weight);

}

TH1D* AnalysisCore::GetHist1D(TString histname){

  TH1D* h = NULL;
  std::map<TString, TH1D*>::iterator mapit = maphist_TH1D.find(histname);
  if(mapit != maphist_TH1D.end()) return mapit->second;

  return h;

}

void AnalysisCore::WriteHist1D(){

  outfile->cd();
  for(std::map< TString, TH1D* >::iterator mapit = maphist_TH1D.begin(); mapit!=maphist_TH1D.end(); mapit++){
    mapit->second->Write();
  }

}

void AnalysisCore::WriteHist(){

  WriteHist1D();

}


