#ifndef Electron_h
#define Electron_h

#include "Lepton.h"
#include "TString.h"

class Electron: public Lepton{

public:

  Electron();
  ~Electron();

  void SetSC(double sceta, double dcphi);
  inline double scEta() const {return j_scEta;};
  inline double scPhi() const {return j_scPhi;};
  void SetMVA(double mvaiso, double mvanoiso);
  inline double MVAIso() const {return j_mvaiso;}
  inline double MVANoIso() const {return j_mvanoiso;}

  inline void SetPassConversionVeto(bool b) { j_passConversionVeto = b; }
  inline int PassConversionVeto() const { return j_passConversionVeto; }
  inline void SetNMissingHits(int n) { j_NMissingHits = n; }
  inline int NMissingHits() const { return j_NMissingHits; };

  enum EtaRegion{
    IB, OB, GAP, EC
  };
  EtaRegion etaRegion(){
    double sceta = fabs(scEta());
    if( sceta < 0.8 ) return IB;
    else if( sceta < 1.444 ) return OB;
    else if( sceta < 1.566 ) return GAP;
    else return EC;
  }

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
  bool Pass_SUSYMVAWP(TString wp);
  bool Pass_SUSYTight();
  bool Pass_SUSYLoose();
  bool Pass_HNTight();

  void SetRelPFIso_Rho(double r);
  double EA();

  //==== ID
  bool PassID(TString ID);
  bool Pass_TESTID();

private:

  double j_scEta,j_scPhi;
  double j_mvaiso, j_mvanoiso;
  bool j_passConversionVeto;
  int j_NMissingHits;
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
  j_scEta = -999.;
  j_scPhi = -999.;
  j_mvaiso = -999.;
  j_mvanoiso = -999.;
  j_passConversionVeto = false;
  j_NMissingHits = 0;
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

void Electron::SetSC(double sceta, double dcphi){
  j_scEta = sceta;
  j_scPhi = dcphi;
}
void Electron::SetMVA(double mvaiso, double mvanoiso){
  j_mvaiso = mvaiso;
  j_mvanoiso = mvanoiso;
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

double Electron::EA(){

  double eta = fabs(this->scEta());

  if     (eta<1.0000) return 0.1566;
  else if(eta<1.4790) return 0.1626;
  else if(eta<2.0000) return 0.1073;
  else if(eta<2.2000) return 0.0854;
  else if(eta<2.3000) return 0.1051;
  else if(eta<2.4000) return 0.1204;
  else if(eta<5.0000) return 0.1524;
  else return 0.1524;

}

bool Electron::PassID(TString ID){

  //==== XXX veto Gap Always
  if(etaRegion()==GAP) return false;

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
  if(ID=="SUSYTight") return Pass_SUSYTight();
  if(ID=="SUSYLoose") return Pass_SUSYLoose();
  if(ID=="HNTight") return Pass_HNTight();
  if(ID=="NOCUT") return true;

  cout << "[Electron::PassID] No id : " << ID << endl;
  exit(EXIT_FAILURE);

  return false;
}

bool Electron::Pass_SUSYMVAWP(TString wp){

  double sceta = fabs(scEta());

    double cutA = 0.77;
    double cutB = 0.52;

  if(wp=="Tight"){
    if     (sceta<0.8)  { cutA = 0.77; cutB = 0.52; }
    else if(sceta<1.479){ cutA = 0.56; cutB = 0.11; } 
    else                { cutA = 0.48; cutB =-0.01; }
  }
  else if(wp=="Loose"){
    if     (sceta<0.8)  { cutA =-0.48; cutB =-0.85; }
    else if(sceta<1.479){ cutA =-0.67; cutB =-0.91; }
    else                { cutA =-0.49; cutB =-0.83; }
  }
  else{}

  double cutSlope = (cutB-cutA) / 10.;
  double cutFinal = std::min( cutA, std::max(cutB , cutA + cutSlope*(this->Pt()-15.) ) );

  //==== Using NoIso MVA, because we apply MiniIso later
  if(MVANoIso()>cutFinal) return true;
  else return false;

}

bool Electron::Pass_SUSYTight(){
  if(! Pass_SUSYMVAWP("Tight") ) return false;
  if(! (MiniRelIso()<0.1) ) return false;	
  if(! (fabs(dXY())<0.05 && fabs(dZ())<0.1 && fabs(IP3D()/IP3Derr())<8.) ) return false;
  //if(! PassConversionVeto() ) return false; //FIXME 
  if(! (NMissingHits()==0) ) return false;

  return true;
}

bool Electron::Pass_SUSYLoose(){
  if(! Pass_SUSYMVAWP("Loose") ) return false;
  if(! (MiniRelIso()<0.4) ) return false;
  if(! (fabs(dXY())<0.05 && fabs(dZ())<0.1 && fabs(IP3D()/IP3Derr())<8.) ) return false;
  //if(! PassConversionVeto() ) return false; //FIXME 
  if(! (NMissingHits()==0) ) return false;

  return true;
}

bool Electron::Pass_HNTight(){

  return true;
}

bool Electron::Pass_TESTID(){
  return true;
}

#endif
