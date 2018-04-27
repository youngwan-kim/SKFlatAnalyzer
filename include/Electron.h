#ifndef Electron_h
#define Electron_h

#include "Lepton.h"
#include "TString.h"

class Electron: public Lepton{

public:

  Electron();
  ~Electron();

  void SetPOGIDs(std::vector<bool> bs);
  inline bool passVetoID() const {return j_passVetoID;}
  inline bool passLooseID() const {return j_passLooseID;}
  inline bool passMediumID() const {return j_passMediumID;}
  inline bool passTightID() const {return j_passTightID;}
  inline bool passMVAID_noIso_WP80() const {return j_passMVAID_noIso_WP80;}
  inline bool passMVAID_noIso_WP90() const {return j_passMVAID_noIso_WP90;}
  inline bool passMVAID_iso_WP80() const {return j_passMVAID_iso_WP80;}
  inline bool passMVAID_iso_WP90() const {return j_passMVAID_iso_WP90;}
  inline bool passHEEPID() const {return j_passHEEPID;}

  void SetRelPFIso_Rho(double r);

  //==== ID
  bool PassID(TString ID);
  bool Pass_TESTID();

private:

  bool j_passVetoID;
  bool j_passLooseID;
  bool j_passMediumID;
  bool j_passTightID;
  bool j_passMVAID_noIso_WP80;
  bool j_passMVAID_noIso_WP90;
  bool j_passMVAID_iso_WP80;
  bool j_passMVAID_iso_WP90;
  bool j_passHEEPID;

  double j_RelPFIso_Rho;

};

Electron::Electron() : Lepton() {
  j_passVetoID = false;
  j_passLooseID = false;
  j_passMediumID = false;
  j_passTightID = false;
  j_passMVAID_noIso_WP80 = false;
  j_passMVAID_noIso_WP90 = false;
  j_passMVAID_iso_WP80 = false;
  j_passMVAID_iso_WP90 = false;
  j_passHEEPID = false;
  this->SetLeptonFlavour(ELECTRON);
}

Electron::~Electron(){
}

void Electron::SetPOGIDs(std::vector<bool> bs){
  j_passVetoID = bs.at(0);
  j_passLooseID = bs.at(1);
  j_passMediumID = bs.at(2);
  j_passTightID = bs.at(3);
  j_passMVAID_noIso_WP80 = bs.at(4);
  j_passMVAID_noIso_WP90 = bs.at(5);
  j_passMVAID_iso_WP80 = bs.at(6);
  j_passMVAID_iso_WP90 = bs.at(7);
  j_passHEEPID = bs.at(8);
}

void Electron::SetRelPFIso_Rho(double r){
  j_passTightID = r;
  this->SetRelIso(r);
}

bool Electron::PassID(TString ID){
  if(ID=="TEST") return Pass_TESTID();
  if(ID=="passVetoID") return passVetoID();
  if(ID=="passLooseID") return passLooseID();
  if(ID=="passMediumID") return passMediumID();
  if(ID=="passTightID") return passTightID();
  if(ID=="passMVAID_noIso_WP80") return passMVAID_noIso_WP80();
  if(ID=="passMVAID_noIso_WP90") return passMVAID_noIso_WP90();
  if(ID=="passMVAID_iso_WP80") return passMVAID_iso_WP80();
  if(ID=="passMVAID_iso_WP90") return passMVAID_iso_WP90();
  if(ID=="passHEEPID") return passHEEPID();
  return false;
}

bool Electron::Pass_TESTID(){
  return true;
}

#endif
