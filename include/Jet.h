#ifndef Jet_h
#define Jet_h

#include "Particle.h"

class Jet: public Particle {
public:

  Jet();
  ~Jet();

  void SetAreaAndRho(double area, double rho);
  void SetGenFlavours(double pf, double hf);
  void SetTaggerResults(std::vector<double> ds);
  void SetEnergyFractions(double cH, double nH, double nEM, double cEM);
  void SetMultiplicities(double cM, double nM);
  void SetPileupJetId(double v);

  enum Tagger{
    CSVv2, DeepCSV, DeepFlavour, CvsL, CvsB, DeepCvsL, DeepCvsB
  };
  enum WP{
    Loose, Medium, Tight
  };
  double GetTaggerResult(Tagger tg);
  bool IsTagged(Tagger tg, WP wp);

private:

  double  j_area;
  double  j_rho;
  int j_partonFlavour;
  int j_hadronFlavour;
  double  j_CSVv2;
  double  j_DeepCSV;
  double  j_DeepFlavour;
  double  j_CvsL;
  double  j_CvsB;
  double  j_DeepCvsL;
  double  j_DeepCvsB;
  double  j_chargedHadronEnergyFraction;
  double  j_neutralHadronEnergyFraction;
  double  j_neutralEmEnergyFraction;
  double  j_chargedEmEnergyFraction;
  int j_chargedMultiplicity;
  int j_neutralMultiplicity;
  double j_PileupJetId;
};

Jet::Jet() : Particle() {
  j_area=-999.;
  j_rho=0.;
  j_partonFlavour=-999;
  j_hadronFlavour=-999;
  j_CSVv2=-999.;
  j_DeepCSV=-999.;
  j_DeepFlavour=-999.;
  j_CvsL=-999.;
  j_CvsB=-999.;
  j_DeepCvsL=-999.;
  j_DeepCvsB=-999.;
  j_chargedHadronEnergyFraction=-999.;
  j_neutralHadronEnergyFraction=-999.;
  j_neutralEmEnergyFraction=-999.;
  j_chargedEmEnergyFraction=-999.;
  j_chargedMultiplicity=-999;
  j_neutralMultiplicity=-999;
  j_PileupJetId=-999.;
}

Jet::~Jet(){

}

void Jet::SetAreaAndRho(double area, double rho){
  j_area = area;
  j_rho = rho;
}
void Jet::SetGenFlavours(double pf, double hf){
  j_partonFlavour = pf;
  j_hadronFlavour = hf;
}
void Jet::SetTaggerResults(std::vector<double> ds){
  j_CSVv2        = ds.at(0);
  j_DeepCSV      = ds.at(1);
  j_DeepFlavour  = ds.at(2);
  j_CvsL         = ds.at(3);
  j_CvsB         = ds.at(4);
  j_DeepCvsL     = ds.at(5);
  j_DeepCvsB     = ds.at(6);
}
void Jet::SetEnergyFractions(double cH, double nH, double nEM, double cEM){
  j_chargedHadronEnergyFraction = cH;
  j_neutralHadronEnergyFraction = nH;
  j_neutralEmEnergyFraction = nEM;
  j_chargedEmEnergyFraction = cEM;
}
void Jet::SetMultiplicities(double cM, double nM){
  j_chargedMultiplicity = cM;
  j_neutralMultiplicity = nM;
}
void Jet::SetPileupJetId(double v){
  j_PileupJetId = v;
}

double Jet::GetTaggerResult(Tagger tg){
  if(tg==CSVv2) return j_CSVv2;
  else if(tg==DeepCSV) return j_DeepCSV;
  else if(tg==DeepFlavour) return j_DeepFlavour;
  else if(tg==CvsL) return j_CvsL;
  else if(tg==CvsB) return j_CvsB;
  else if(tg==DeepCvsL) return j_DeepCvsL;
  else if(tg==DeepCvsB) return j_DeepCvsB;
  else{
    cout << "[Jet::GetTaggerResult] ERROR; Wrong tagger" << endl;
    return -999;
  }
}


bool Jet::IsTagged(Tagger tg, WP wp){

  double tr = GetTaggerResult(tg);

  if(tg==CSVv2){
    if(wp==Loose) return (tr>0.5803);
    else if(wp==Medium) return (tr>0.8838);
    else if(wp==Tight) return (tr>0.9693);
    else return false;
  }
  else if(tg==DeepCSV){
    if(wp==Loose) return (tr>0.1522);
    else if(wp==Medium) return (tr>0.4941);
    else if(wp==Tight) return (tr>0.8001);
    else return false;
  }
  else if(tg==DeepFlavour){
    if(wp==Loose) return (tr>0.0574);
    else if(wp==Medium) return (tr>0.4318);
    else if(wp==Tight) return (tr>0.9068);
    else return false;
  }  
  else if(tg==CvsL){
    if(wp==Loose) return (tr>-0.53);
    else if(wp==Medium) return (tr>0.07);
    else if(wp==Tight) return (tr>0.87);
    else return false;
  }  
  else if(tg==CvsB){
    if(wp==Loose) return (tr>-0.26);
    else if(wp==Medium) return (tr>-0.10);
    else if(wp==Tight) return (tr>-0.3);
    else return false;
  }  
  else if(tg==DeepCvsL){
    if(wp==Loose) return (tr>0.05);
    else if(wp==Medium) return (tr>0.15);
    else if(wp==Tight) return (tr>0.8);
    else return false;
  }  
  else if(tg==DeepCvsB){
    if(wp==Loose) return (tr>0.33);
    else if(wp==Medium) return (tr>0.28);
    else if(wp==Tight) return (tr>0.1);
    else return false;
  }  
  else{
    cout << "[Jet::IsTagged] ERROR; Wrong tagger" << endl;
    return false;
  }


}

#endif
