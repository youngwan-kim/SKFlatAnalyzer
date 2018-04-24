#ifndef Muon_h
#define Muon_h

#include "Lepton.h"
#include "TString.h"

class Muon: public Lepton{

public:

  Muon();
  ~Muon();

  inline bool isPOGTight() const {return j_isPOGTight;}
  void SetisPOGTight(bool b);

  void SetIso(double ch04, double nh04, double ph04, double pu04);
  void CalcPFRelIso();

  //==== ID
  bool PassID(TString ID);
  bool Pass_POGTight();
  bool Pass_TESTID();

private:

  bool j_isPOGTight;
  double j_PFCH04, j_PFNH04, j_PFPH04, j_PU04;

};

Muon::Muon() : Lepton() {
  j_isPOGTight = false;
  j_PFCH04 = 999.;
  j_PFNH04 = 999.;
  j_PFPH04 = 999.;
  j_PU04 = 999.;
}

Muon::~Muon(){
}

void Muon::SetisPOGTight(bool b){
  j_isPOGTight = b;
}

void Muon::SetIso(double ch04, double nh04, double ph04, double pu04){
  j_PFCH04 = ch04;
  j_PFNH04 = nh04;
  j_PFPH04 = ph04;
  j_PU04 = pu04;
  CalcPFRelIso();
}

void Muon::CalcPFRelIso(){
  double reliso = j_PFCH04+std::max( 0., j_PFNH04 + j_PFPH04 - 0.5*j_PU04 );
  this->SetRelIso(reliso);
}

bool Muon::PassID(TString ID){
  if(ID=="POGTight") return Pass_POGTight();
  if(ID=="TEST") return Pass_TESTID();
  return false;
}
bool Muon::Pass_POGTight(){
  return j_isPOGTight;
}
bool Muon::Pass_TESTID(){
  if(!( Pass_POGTight() )) return false;
  if(!( RelIso()<0.15 ))  return false;
  return true;
}

#endif
