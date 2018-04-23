#define SKFlatNtuple_cxx
#include "SKFlatNtuple.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void SKFlatNtuple::Loop(){

  Long64_t nentries = fChain->GetEntries();
  //nentries = 1000;
  //cout << "nentries = " << nentries << endl;


  for(Long64_t jentry=0; jentry<nentries;jentry++){

    if(jentry%1000==0){
      cout << "[SKFlatNtuple::Loop] " << jentry << "/" << nentries << " ("<<100.*jentry/nentries<<" %)" << endl;
    }

    fChain->GetEntry(jentry);

    executeEvent();

    //std::cout << jentry << " :" << muon_pt->size() << std::endl;

  }

}
