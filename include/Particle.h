#ifndef Particle_h
#define Particle_h

#include "TLorentzVector.h"

class Particle: public TLorentzVector{

public:

  //==== Default Constructor with p4
  Particle();
  //==== TLorentzVector copy constructor.
  Particle(const TLorentzVector& p);
  //==== Copy constructor.
  Particle(const Particle& p);
  //==== Construct from 4-vector components
  Particle(double px, double py, double pz, double e);

  //==== Add Charge
  Particle& operator+=(const Particle& p);

  //==== Assignment operator uses KParticle copy constructor
  Particle& operator=(const Particle& p);

  ~Particle();

  void SetCharge(int q);
  inline int Charge() const {return j_Charge;}

private:
  int j_Charge;

};

Particle::Particle() :
  TLorentzVector(),
  j_Charge(0)
{
}

Particle::Particle(const TLorentzVector& p) :
  TLorentzVector(p),
  j_Charge(0)
{
}

Particle::Particle(const Particle& p) :
  TLorentzVector(p),
  j_Charge(p.Charge())
{
}

Particle::Particle(double px, double py, double pz, double e) :
  TLorentzVector(px, py, pz, e),
  j_Charge(0)
{
}


Particle& Particle::operator=(const Particle& p){
  if(this != &p){
    TLorentzVector::operator=(p);
    j_Charge = p.j_Charge;
  }

  return *this;
}

Particle& Particle::operator+= (const Particle& p){
  TLorentzVector::operator+=(p);
  j_Charge += p.j_Charge;
  return *this;
}

Particle::~Particle()
{
}

void Particle::SetCharge(int q){
  j_Charge = q;
}

#endif
