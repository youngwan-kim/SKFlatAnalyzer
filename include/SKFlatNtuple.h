#ifndef SKFlatNtuple_h
#define SKFlatNtuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>
#include <vector>
#include <vector>

#include <iostream>

// Fixed size dimensions of array or collections stored in the TTree if any.

class SKFlatNtuple {
public :

   // Declaration of leaf types
   Bool_t          IsData;
   Int_t           nTotal;
   Int_t           run;
   ULong64_t       event;
   Int_t           lumi;
   Double_t        PUweight;
   Int_t           nPV;
   Bool_t          Flag_badMuons;
   Bool_t          Flag_duplicateMuons;
   Bool_t          Flag_noBadMuons;
   Bool_t          Flag_goodVertices;
   Bool_t          Flag_globalTightHalo2016Filter;
   Bool_t          Flag_HBHENoiseFilter;
   Bool_t          Flag_HBHENoiseIsoFilter;
   Bool_t          Flag_EcalDeadCellTriggerPrimitiveFilter;
   Bool_t          Flag_BadPFMuonFilter;
   Bool_t          Flag_BadChargedCandidateFilter;
   Bool_t          Flag_eeBadScFilter;
   Bool_t          Flag_ecalBadCalibFilter;
   Int_t           PVtrackSize;
   Double_t        PVchi2;
   Double_t        PVndof;
   Double_t        PVnormalizedChi2;
   Double_t        vertex_X;
   Double_t        vertex_Y;
   Double_t        vertex_Z;
   vector<string>  *HLT_TriggerName;
   vector<bool>    *HLT_TriggerFired;
   vector<int>     *HLT_TriggerPrescale;
   vector<double>  *jet_pt;
   vector<double>  *jet_eta;
   vector<double>  *jet_phi;
   vector<double>  *jet_charge;
   vector<double>  *jet_area;
   vector<double>  *jet_rho;
   vector<int>     *jet_partonFlavour;
   vector<int>     *jet_hadronFlavour;
   vector<double>  *jet_CSVv2;
   vector<double>  *jet_DeepCSV;
   vector<double>  *jet_DeepFlavour;
   vector<double>  *jet_CvsL;
   vector<double>  *jet_CvsB;
   vector<double>  *jet_DeepCvsL;
   vector<double>  *jet_DeepCvsB;
   vector<double>  *jet_chargedHadronEnergyFraction;
   vector<double>  *jet_neutralHadronEnergyFraction;
   vector<double>  *jet_neutralEmEnergyFraction;
   vector<double>  *jet_chargedEmEnergyFraction;
   vector<int>     *jet_chargedMultiplicity;
   vector<int>     *jet_neutralMultiplicity;
   vector<bool>    *jet_looseJetID;
   vector<bool>    *jet_tightJetID;
   vector<bool>    *jet_tightLepVetoJetID;
   vector<int>     *jet_partonPdgId;
   vector<int>     *jet_vtxNtracks;
   vector<double>  *jet_m;
   vector<double>  *jet_energy;
   vector<double>  *jet_PileupJetId;
   vector<double>  *jet_shiftedEnUp;
   vector<double>  *jet_shiftedEnDown;
   vector<double>  *fatjet_pt;
   vector<double>  *fatjet_eta;
   vector<double>  *fatjet_phi;
   vector<double>  *fatjet_charge;
   vector<double>  *fatjet_area;
   vector<double>  *fatjet_rho;
   vector<int>     *fatjet_partonFlavour;
   vector<int>     *fatjet_hadronFlavour;
   vector<double>  *fatjet_CSVv2;
   vector<double>  *fatjet_DeepCSV;
   vector<double>  *fatjet_DeepFlavour;
   vector<double>  *fatjet_CvsL;
   vector<double>  *fatjet_CvsB;
   vector<double>  *fatjet_DeepCvsL;
   vector<double>  *fatjet_DeepCvsB;
   vector<bool>    *fatjet_looseJetID;
   vector<bool>    *fatjet_tightJetID;
   vector<bool>    *fatjet_tightLepVetoJetID;
   vector<int>     *fatjet_partonPdgId;
   vector<int>     *fatjet_vtxNtracks;
   vector<double>  *fatjet_m;
   vector<double>  *fatjet_energy;
   vector<double>  *fatjet_puppi_tau1;
   vector<double>  *fatjet_puppi_tau2;
   vector<double>  *fatjet_puppi_tau3;
   vector<double>  *fatjet_puppi_tau4;
   vector<double>  *fatjet_softdropmass;
   vector<double>  *fatjet_chargedHadronEnergyFraction;
   vector<double>  *fatjet_neutralHadronEnergyFraction;
   vector<double>  *fatjet_neutralEmEnergyFraction;
   vector<double>  *fatjet_chargedEmEnergyFraction;
   vector<int>     *fatjet_chargedMultiplicity;
   vector<int>     *fatjet_neutralMultiplicity;
   vector<double>  *fatjet_shiftedEnUp;
   vector<double>  *fatjet_shiftedEnDown;
   vector<double>  *electron_MVAIso;
   vector<double>  *electron_MVANoIso;
   vector<double>  *electron_et;
   vector<double>  *electron_caloEnergy;
   vector<double>  *electron_Energy;
   vector<double>  *electron_Energy_Scale_Up;
   vector<double>  *electron_Energy_Scale_Down;
   vector<double>  *electron_Energy_Smear_Up;
   vector<double>  *electron_Energy_Smear_Down;
   vector<double>  *electron_pt;
   vector<double>  *electron_pt_Scale_Up;
   vector<double>  *electron_pt_Scale_Down;
   vector<double>  *electron_pt_Smear_Up;
   vector<double>  *electron_pt_Smear_Down;
   vector<double>  *electron_Px;
   vector<double>  *electron_Py;
   vector<double>  *electron_Pz;
   vector<double>  *electron_eta;
   vector<double>  *electron_phi;
   vector<int>     *electron_charge;
   vector<double>  *electron_gsfpt;
   vector<double>  *electron_gsfPx;
   vector<double>  *electron_gsfPy;
   vector<double>  *electron_gsfPz;
   vector<double>  *electron_gsfEta;
   vector<double>  *electron_gsfPhi;
   vector<int>     *electron_gsfCharge;
   vector<double>  *electron_scEta;
   vector<double>  *electron_scPhi;
   vector<double>  *electron_etaWidth;
   vector<double>  *electron_phiWidth;
   vector<double>  *electron_dEtaIn;
   vector<double>  *electron_dEtaInSeed;
   vector<double>  *electron_dPhiIn;
   vector<double>  *electron_sigmaIEtaIEta;
   vector<double>  *electron_Full5x5_SigmaIEtaIEta;
   vector<double>  *electron_HoverE;
   vector<double>  *electron_fbrem;
   vector<double>  *electron_eOverP;
   vector<double>  *electron_energyEC;
   vector<double>  *electron_Pnorm;
   vector<double>  *electron_InvEminusInvP;
   vector<double>  *electron_dxyVTX;
   vector<double>  *electron_dzVTX;
   vector<double>  *electron_dxy;
   vector<double>  *electron_sigdxy;
   vector<double>  *electron_dz;
   vector<double>  *electron_ip3D;
   vector<double>  *electron_sigip3D;
   vector<double>  *electron_dxyBS;
   vector<double>  *electron_dzBS;
   vector<double>  *electron_AEff03;
   vector<double>  *electron_chIso03;
   vector<double>  *electron_nhIso03;
   vector<double>  *electron_phIso03;
   vector<double>  *electron_pcIso03;
   vector<double>  *electron_puChIso03;
   vector<double>  *electron_chIso04;
   vector<double>  *electron_nhIso04;
   vector<double>  *electron_phIso04;
   vector<double>  *electron_pcIso04;
   vector<double>  *electron_puChIso04;
   vector<double>  *electron_relIsoCom03;
   vector<double>  *electron_relIsoCom04;
   vector<double>  *electron_relIsoBeta03;
   vector<double>  *electron_relIsoBeta04;
   vector<double>  *electron_relIsoRho03;
   vector<bool>    *electron_passConversionVeto;
   vector<bool>    *electron_isGsfCtfScPixChargeConsistent;
   vector<int>     *electron_mHits;
   vector<int>     *electron_crack;
   vector<int>     *electron_ecalDriven;
   vector<double>  *electron_isoEMHADDepth1;
   vector<double>  *electron_25over55;
   vector<double>  *electron_15over55;
   vector<double>  *electron_isoHADDepth2;
   vector<double>  *electron_isoptTrks;
   vector<double>  *electron_modIsoEMHADDepth1;
   vector<double>  *electron_modIsoptTrks;
   vector<double>  *electron_modIsoEMHADDepth1Orig;
   vector<double>  *electron_modIsoptTrksOrig;
   vector<double>  *electron_ambGsf0pt;
   vector<double>  *electron_ambGsf0Eta;
   vector<double>  *electron_ambGsf0Phi;
   vector<double>  *electron_ambGsf0Charge;
   vector<double>  *electron_ambGsf1pt;
   vector<double>  *electron_ambGsf1Eta;
   vector<double>  *electron_ambGsf1Phi;
   vector<double>  *electron_ambGsf1Charge;
   vector<double>  *electron_ambGsf2pt;
   vector<double>  *electron_ambGsf2Eta;
   vector<double>  *electron_ambGsf2Phi;
   vector<double>  *electron_ambGsf2Charge;
   vector<double>  *electron_ambGsf3pt;
   vector<double>  *electron_ambGsf3Eta;
   vector<double>  *electron_ambGsf3Phi;
   vector<double>  *electron_ambGsf3Charge;
   vector<double>  *electron_r9;
   vector<double>  *electron_scEnergy;
   vector<double>  *electron_scPreEnergy;
   vector<double>  *electron_scRawEnergy;
   vector<double>  *electron_scEt;
   vector<double>  *electron_E15;
   vector<double>  *electron_E25;
   vector<double>  *electron_E55;
   vector<double>  *electron_RelPFIso_dBeta;
   vector<double>  *electron_RelPFIso_Rho;
   vector<bool>    *electron_passVetoID;
   vector<bool>    *electron_passLooseID;
   vector<bool>    *electron_passMediumID;
   vector<bool>    *electron_passTightID;
   vector<bool>    *electron_passMVAID_noIso_WP80;
   vector<bool>    *electron_passMVAID_noIso_WP90;
   vector<bool>    *electron_passMVAID_iso_WP80;
   vector<bool>    *electron_passMVAID_iso_WP90;
   vector<bool>    *electron_passHEEPID;
   vector<double>  *electron_ptUnCorr;
   vector<double>  *electron_etaUnCorr;
   vector<double>  *electron_phiUnCorr;
   vector<double>  *electron_PxUnCorr;
   vector<double>  *electron_PyUnCorr;
   vector<double>  *electron_PzUnCorr;
   vector<double>  *electron_EnergyUnCorr;
   vector<double>  *electron_scEnergyUnCorr;
   vector<double>  *electron_scEtaUnCorr;
   vector<double>  *electron_scPhiUnCorr;
   vector<double>  *electron_scEtUnCorr;
   vector<double>  *electron_mva;
   vector<double>  *electron_zzmva;
   vector<int>     *electron_missinghits;
   vector<double>  *muon_PfChargedHadronIsoR05;
   vector<double>  *muon_PfNeutralHadronIsoR05;
   vector<double>  *muon_PfGammaIsoR05;
   vector<double>  *muon_PfChargedHadronIsoR04;
   vector<double>  *muon_PfNeutralHadronIsoR04;
   vector<double>  *muon_PfGammaIsoR04;
   vector<double>  *muon_PFSumPUIsoR04;
   vector<double>  *muon_PfChargedHadronIsoR03;
   vector<double>  *muon_PfNeutralHadronIsoR03;
   vector<double>  *muon_PfGammaIsoR03;
   vector<double>  *muon_PFSumPUIsoR03;
   vector<bool>    *muon_isPF;
   vector<bool>    *muon_isGlobal;
   vector<bool>    *muon_isTracker;
   vector<bool>    *muon_isStandAlone;
   vector<bool>    *muon_isTight;
   vector<bool>    *muon_isMedium;
   vector<bool>    *muon_isLoose;
   vector<bool>    *muon_isSoft;
   vector<bool>    *muon_isHighPt;
   vector<double>  *muon_dB;
   vector<double>  *muon_phi;
   vector<double>  *muon_eta;
   vector<double>  *muon_pt;
   vector<double>  *muon_cktpt;
   vector<double>  *muon_cktPx;
   vector<double>  *muon_cktPy;
   vector<double>  *muon_cktPz;
   vector<double>  *muon_cktptError;
   vector<double>  *muon_Px;
   vector<double>  *muon_Py;
   vector<double>  *muon_Pz;
   vector<double>  *muon_sumtrkpt;
   vector<double>  *muon_trkiso;
   vector<double>  *muon_hcaliso;
   vector<double>  *muon_ecaliso;
   vector<double>  *muon_trkisoR05;
   vector<double>  *muon_hcalisoR05;
   vector<double>  *muon_ecalisoR05;
   vector<int>     *muon_charge;
   vector<int>     *muon_nChambers;
   vector<int>     *muon_matchedstations;
   vector<int>     *muon_stationMask;
   vector<int>     *muon_nSegments;
   vector<double>  *muon_normchi;
   vector<int>     *muon_validhits;
   vector<int>     *muon_trackerHits;
   vector<int>     *muon_pixelHits;
   vector<int>     *muon_validmuonhits;
   vector<int>     *muon_trackerLayers;
   vector<double>  *muon_qoverp;
   vector<double>  *muon_theta;
   vector<double>  *muon_lambda;
   vector<double>  *muon_dxy;
   vector<double>  *muon_d0;
   vector<double>  *muon_dsz;
   vector<double>  *muon_dz;
   vector<double>  *muon_dxyBS;
   vector<double>  *muon_dzBS;
   vector<double>  *muon_dszBS;
   vector<double>  *muon_dxyVTX;
   vector<double>  *muon_dzVTX;
   vector<double>  *muon_dszVTX;
   vector<double>  *muon_dxycktVTX;
   vector<double>  *muon_dzcktVTX;
   vector<double>  *muon_dszcktVTX;
   vector<double>  *muon_vx;
   vector<double>  *muon_vy;
   vector<double>  *muon_vz;
   vector<double>  *muon_Best_pt;
   vector<double>  *muon_Best_ptError;
   vector<double>  *muon_Best_Px;
   vector<double>  *muon_Best_Py;
   vector<double>  *muon_Best_Pz;
   vector<double>  *muon_Best_eta;
   vector<double>  *muon_Best_phi;
   vector<double>  *muon_Inner_pt;
   vector<double>  *muon_Inner_ptError;
   vector<double>  *muon_Inner_Px;
   vector<double>  *muon_Inner_Py;
   vector<double>  *muon_Inner_Pz;
   vector<double>  *muon_Inner_eta;
   vector<double>  *muon_Inner_phi;
   vector<double>  *muon_Outer_pt;
   vector<double>  *muon_Outer_ptError;
   vector<double>  *muon_Outer_Px;
   vector<double>  *muon_Outer_Py;
   vector<double>  *muon_Outer_Pz;
   vector<double>  *muon_Outer_eta;
   vector<double>  *muon_Outer_phi;
   vector<double>  *muon_GLB_pt;
   vector<double>  *muon_GLB_ptError;
   vector<double>  *muon_GLB_Px;
   vector<double>  *muon_GLB_Py;
   vector<double>  *muon_GLB_Pz;
   vector<double>  *muon_GLB_eta;
   vector<double>  *muon_GLB_phi;
   vector<double>  *muon_TuneP_pt;
   vector<double>  *muon_TuneP_ptError;
   vector<double>  *muon_TuneP_Px;
   vector<double>  *muon_TuneP_Py;
   vector<double>  *muon_TuneP_Pz;
   vector<double>  *muon_TuneP_eta;
   vector<double>  *muon_TuneP_phi;
   vector<double>  *muon_roch_sf;
   vector<double>  *muon_roch_sf_up;
   vector<double>  *PDFWeights_Scale;
   vector<double>  *PDFWeights_Error;
   vector<double>  *PDFWeights_AlphaS;
   vector<double>  *gen_phi;
   vector<double>  *gen_eta;
   vector<double>  *gen_pt;
   vector<double>  *gen_Px;
   vector<double>  *gen_Py;
   vector<double>  *gen_Pz;
   vector<double>  *gen_E;
   vector<int>     *gen_mother_PID;
   vector<double>  *gen_mother_pt;
   vector<int>     *gen_mother_index;
   vector<int>     *gen_charge;
   vector<int>     *gen_status;
   vector<int>     *gen_PID;
   vector<int>     *gen_isPrompt;
   vector<int>     *gen_isPromptFinalState;
   vector<int>     *gen_isTauDecayProduct;
   vector<int>     *gen_isPromptTauDecayProduct;
   vector<int>     *gen_isDirectPromptTauDecayProductFinalState;
   vector<int>     *gen_isHardProcess;
   vector<int>     *gen_isLastCopy;
   vector<int>     *gen_isLastCopyBeforeFSR;
   vector<int>     *gen_isPromptDecayed;
   vector<int>     *gen_isDecayedLeptonHadron;
   vector<int>     *gen_fromHardProcessBeforeFSR;
   vector<int>     *gen_fromHardProcessDecayed;
   vector<int>     *gen_fromHardProcessFinalState;
   vector<int>     *gen_isMostlyLikePythia6Status3;
   Double_t        gen_weight;
   Double_t        genWeight_Q;
   Double_t        genWeight_X1;
   Double_t        genWeight_X2;
   Int_t           genWeight_id1;
   Int_t           genWeight_id2;
   Double_t        genWeight_alphaQCD;
   Double_t        genWeight_alphaQED;
   vector<double>  *photon_pt;
   vector<double>  *photon_eta;
   vector<double>  *photon_phi;
   vector<double>  *photon_scEta;
   vector<double>  *photon_scPhi;
   vector<double>  *photon_HoverE;
   vector<int>     *photon_hasPixelSeed;
   vector<double>  *photon_Full5x5_SigmaIEtaIEta;
   vector<double>  *photon_ChIso;
   vector<double>  *photon_NhIso;
   vector<double>  *photon_PhIso;
   vector<double>  *photon_ChIsoWithEA;
   vector<double>  *photon_NhIsoWithEA;
   vector<double>  *photon_PhIsoWithEA;
   vector<bool>    *photon_passMVAID_WP80;
   vector<bool>    *photon_passMVAID_WP90;
   vector<bool>    *photon_passLooseID;
   vector<bool>    *photon_passMediumID;
   vector<bool>    *photon_passTightID;
   vector<double>  *photon_ptUnCorr;
   vector<double>  *photon_etaUnCorr;
   vector<double>  *photon_phiUnCorr;
   Int_t           nPileUp;
   Double_t        pileUpReweightIn;
   Double_t        pileUpReweight;
   Double_t        pileUpReweightPlus;
   Double_t        pileUpReweightMinus;
   Double_t        pileUpReweightInMuonPhys;
   Double_t        pileUpReweightMuonPhys;
   Double_t        pileUpReweightPlusMuonPhys;
   Double_t        pileUpReweightMinusMuonPhys;
   Double_t        pfMET_pt;
   Double_t        pfMET_phi;
   Double_t        pfMET_Px;
   Double_t        pfMET_Py;
   Double_t        pfMET_SumEt;
   Double_t        pfMET_Type1_pt;
   Double_t        pfMET_Type1_phi;
   Double_t        pfMET_Type1_Px;
   Double_t        pfMET_Type1_Py;
   Double_t        pfMET_Type1_SumEt;
   Double_t        pfMET_Type1_PhiCor_pt;
   Double_t        pfMET_Type1_PhiCor_phi;
   Double_t        pfMET_Type1_PhiCor_Px;
   Double_t        pfMET_Type1_PhiCor_Py;
   Double_t        pfMET_Type1_PhiCor_SumEt;

   // List of branches
   TBranch        *b_IsData;   //!
   TBranch        *b_nTotal;   //!
   TBranch        *b_runNum;   //!
   TBranch        *b_evtNum;   //!
   TBranch        *b_lumiBlock;   //!
   TBranch        *b_PUweight;   //!
   TBranch        *b_nVertices;   //!
   TBranch        *b_Flag_badMuons;   //!
   TBranch        *b_Flag_duplicateMuons;   //!
   TBranch        *b_Flag_noBadMuons;   //!
   TBranch        *b_Flag_goodVertices;   //!
   TBranch        *b_Flag_globalTightHalo2016Filter;   //!
   TBranch        *b_Flag_HBHENoiseFilter;   //!
   TBranch        *b_Flag_HBHENoiseIsoFilter;   //!
   TBranch        *b_Flag_EcalDeadCellTriggerPrimitiveFilter;   //!
   TBranch        *b_Flag_BadPFMuonFilter;   //!
   TBranch        *b_Flag_BadChargedCandidateFilter;   //!
   TBranch        *b_Flag_eeBadScFilter;   //!
   TBranch        *b_Flag_ecalBadCalibFilter;   //!
   TBranch        *b_PVtrackSize;   //!
   TBranch        *b_PVchi2;   //!
   TBranch        *b_PVndof;   //!
   TBranch        *b_PVnormalizedChi2;   //!
   TBranch        *b_PVx;   //!
   TBranch        *b_PVy;   //!
   TBranch        *b_PVz;   //!
   TBranch        *b_HLT_TriggerName;   //!
   TBranch        *b_HLT_TriggerFired;   //!
   TBranch        *b_HLT_TriggerPrescale;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_charge;   //!
   TBranch        *b_jet_area;   //!
   TBranch        *b_jet_rho;   //!
   TBranch        *b_jet_partonFlavour;   //!
   TBranch        *b_jet_hadronFlavour;   //!
   TBranch        *b_jet_CSVv2;   //!
   TBranch        *b_jet_DeepCSV;   //!
   TBranch        *b_jet_DeepFlavour;   //!
   TBranch        *b_jet_CvsL;   //!
   TBranch        *b_jet_CvsB;   //!
   TBranch        *b_jet_DeepCvsL;   //!
   TBranch        *b_jet_DeepCvsB;   //!
   TBranch        *b_jet_chargedHadronEnergyFraction;   //!
   TBranch        *b_jet_neutralHadronEnergyFraction;   //!
   TBranch        *b_jet_neutralEmEnergyFraction;   //!
   TBranch        *b_jet_chargedEmEnergyFraction;   //!
   TBranch        *b_jet_chargedMultiplicity;   //!
   TBranch        *b_jet_neutralMultiplicity;   //!
   TBranch        *b_jet_looseJetID;   //!
   TBranch        *b_jet_tightJetID;   //!
   TBranch        *b_jet_tightLepVetoJetID;   //!
   TBranch        *b_jet_partonPdgId;   //!
   TBranch        *b_jet_vtxNtracks;   //!
   TBranch        *b_jet_m;   //!
   TBranch        *b_jet_energy;   //!
   TBranch        *b_jet_PileupJetId;   //!
   TBranch        *b_jet_shiftedEnUp;   //!
   TBranch        *b_jet_shiftedEnDown;   //!
   TBranch        *b_fatjet_pt;   //!
   TBranch        *b_fatjet_eta;   //!
   TBranch        *b_fatjet_phi;   //!
   TBranch        *b_fatjet_charge;   //!
   TBranch        *b_fatjet_area;   //!
   TBranch        *b_fatjet_rho;   //!
   TBranch        *b_fatjet_partonFlavour;   //!
   TBranch        *b_fatjet_hadronFlavour;   //!
   TBranch        *b_fatjet_CSVv2;   //!
   TBranch        *b_fatjet_DeepCSV;   //!
   TBranch        *b_fatjet_DeepFlavour;   //!
   TBranch        *b_fatjet_CvsL;   //!
   TBranch        *b_fatjet_CvsB;   //!
   TBranch        *b_fatjet_DeepCvsL;   //!
   TBranch        *b_fatjet_DeepCvsB;   //!
   TBranch        *b_fatjet_looseJetID;   //!
   TBranch        *b_fatjet_tightJetID;   //!
   TBranch        *b_fatjet_tightLepVetoJetID;   //!
   TBranch        *b_fatjet_partonPdgId;   //!
   TBranch        *b_fatjet_vtxNtracks;   //!
   TBranch        *b_fatjet_m;   //!
   TBranch        *b_fatjet_energy;   //!
   TBranch        *b_fatjet_puppi_tau1;   //!
   TBranch        *b_fatjet_puppi_tau2;   //!
   TBranch        *b_fatjet_puppi_tau3;   //!
   TBranch        *b_fatjet_puppi_tau4;   //!
   TBranch        *b_fatjet_softdropmass;   //!
   TBranch        *b_fatjet_chargedHadronEnergyFraction;   //!
   TBranch        *b_fatjet_neutralHadronEnergyFraction;   //!
   TBranch        *b_fatjet_neutralEmEnergyFraction;   //!
   TBranch        *b_fatjet_chargedEmEnergyFraction;   //!
   TBranch        *b_fatjet_chargedMultiplicity;   //!
   TBranch        *b_fatjet_neutralMultiplicity;   //!
   TBranch        *b_fatjet_shiftedEnUp;   //!
   TBranch        *b_fatjet_shiftedEnDown;   //!
   TBranch        *b_electron_MVAIso;   //!
   TBranch        *b_electron_MVANoIso;   //!
   TBranch        *b_electron_et;   //!
   TBranch        *b_electron_caloEnergy;   //!
   TBranch        *b_electron_Energy;   //!
   TBranch        *b_electron_Energy_Scale_Up;   //!
   TBranch        *b_electron_Energy_Scale_Down;   //!
   TBranch        *b_electron_Energy_Smear_Up;   //!
   TBranch        *b_electron_Energy_Smear_Down;   //!
   TBranch        *b_electron_pt;   //!
   TBranch        *b_electron_pt_Scale_Up;   //!
   TBranch        *b_electron_pt_Scale_Down;   //!
   TBranch        *b_electron_pt_Smear_Up;   //!
   TBranch        *b_electron_pt_Smear_Down;   //!
   TBranch        *b_electron_Px;   //!
   TBranch        *b_electron_Py;   //!
   TBranch        *b_electron_Pz;   //!
   TBranch        *b_electron_eta;   //!
   TBranch        *b_electron_phi;   //!
   TBranch        *b_electron_charge;   //!
   TBranch        *b_electron_gsfpt;   //!
   TBranch        *b_electron_gsfPx;   //!
   TBranch        *b_electron_gsfPy;   //!
   TBranch        *b_electron_gsfPz;   //!
   TBranch        *b_electron_gsfEta;   //!
   TBranch        *b_electron_gsfPhi;   //!
   TBranch        *b_electron_gsfCharge;   //!
   TBranch        *b_electron_scEta;   //!
   TBranch        *b_electron_scPhi;   //!
   TBranch        *b_electron_etaWidth;   //!
   TBranch        *b_electron_phiWidth;   //!
   TBranch        *b_electron_dEtaIn;   //!
   TBranch        *b_electron_dEtaInSeed;   //!
   TBranch        *b_electron_dPhiIn;   //!
   TBranch        *b_electron_sigmaIEtaIEta;   //!
   TBranch        *b_electron_Full5x5_SigmaIEtaIEta;   //!
   TBranch        *b_electron_HoverE;   //!
   TBranch        *b_electron_fbrem;   //!
   TBranch        *b_electron_eOverP;   //!
   TBranch        *b_electron_energyEC;   //!
   TBranch        *b_electron_Pnorm;   //!
   TBranch        *b_electron_InvEminusInvP;   //!
   TBranch        *b_electron_dxyVTX;   //!
   TBranch        *b_electron_dzVTX;   //!
   TBranch        *b_electron_dxy;   //!
   TBranch        *b_electron_sigdxy;   //!
   TBranch        *b_electron_dz;   //!
   TBranch        *b_electron_ip3D;   //!
   TBranch        *b_electron_sigip3D;   //!
   TBranch        *b_electron_dxyBS;   //!
   TBranch        *b_electron_dzBS;   //!
   TBranch        *b_electron_AEff03;   //!
   TBranch        *b_electron_chIso03;   //!
   TBranch        *b_electron_nhIso03;   //!
   TBranch        *b_electron_phIso03;   //!
   TBranch        *b_electron_pcIso03;   //!
   TBranch        *b_electron_puChIso03;   //!
   TBranch        *b_electron_chIso04;   //!
   TBranch        *b_electron_nhIso04;   //!
   TBranch        *b_electron_phIso04;   //!
   TBranch        *b_electron_pcIso04;   //!
   TBranch        *b_electron_puChIso04;   //!
   TBranch        *b_electron_relIsoCom03;   //!
   TBranch        *b_electron_relIsoCom04;   //!
   TBranch        *b_electron_relIsoBeta03;   //!
   TBranch        *b_electron_relIsoBeta04;   //!
   TBranch        *b_electron_relIsoRho03;   //!
   TBranch        *b_electron_passConversionVeto;   //!
   TBranch        *b_electron_isGsfCtfScPixChargeConsistent;   //!
   TBranch        *b_electron_mHits;   //!
   TBranch        *b_electron_crack;   //!
   TBranch        *b_electron_ecalDriven;   //!
   TBranch        *b_electron_isoEMHADDepth1;   //!
   TBranch        *b_electron_25over55;   //!
   TBranch        *b_electron_15over55;   //!
   TBranch        *b_electron_isoHADDepth2;   //!
   TBranch        *b_electron_isoptTrks;   //!
   TBranch        *b_electron_modIsoEMHADDepth1;   //!
   TBranch        *b_electron_modIsoptTrks;   //!
   TBranch        *b_electron_modIsoEMHADDepth1Orig;   //!
   TBranch        *b_electron_modIsoptTrksOrig;   //!
   TBranch        *b_electron_ambGsf0pt;   //!
   TBranch        *b_electron_ambGsf0Eta;   //!
   TBranch        *b_electron_ambGsf0Phi;   //!
   TBranch        *b_electron_ambGsf0Charge;   //!
   TBranch        *b_electron_ambGsf1pt;   //!
   TBranch        *b_electron_ambGsf1Eta;   //!
   TBranch        *b_electron_ambGsf1Phi;   //!
   TBranch        *b_electron_ambGsf1Charge;   //!
   TBranch        *b_electron_ambGsf2pt;   //!
   TBranch        *b_electron_ambGsf2Eta;   //!
   TBranch        *b_electron_ambGsf2Phi;   //!
   TBranch        *b_electron_ambGsf2Charge;   //!
   TBranch        *b_electron_ambGsf3pt;   //!
   TBranch        *b_electron_ambGsf3Eta;   //!
   TBranch        *b_electron_ambGsf3Phi;   //!
   TBranch        *b_electron_ambGsf3Charge;   //!
   TBranch        *b_electron_r9;   //!
   TBranch        *b_electron_scEnergy;   //!
   TBranch        *b_electron_scPreEnergy;   //!
   TBranch        *b_electron_scRawEnergy;   //!
   TBranch        *b_electron_scEt;   //!
   TBranch        *b_electron_E15;   //!
   TBranch        *b_electron_E25;   //!
   TBranch        *b_electron_E55;   //!
   TBranch        *b_electron_RelPFIso_dBeta;   //!
   TBranch        *b_electron_RelPFIso_Rho;   //!
   TBranch        *b_electron_passVetoID;   //!
   TBranch        *b_electron_passLooseID;   //!
   TBranch        *b_electron_passMediumID;   //!
   TBranch        *b_electron_passTightID;   //!
   TBranch        *b_electron_passMVAID_noIso_WP80;   //!
   TBranch        *b_electron_passMVAID_noIso_WP90;   //!
   TBranch        *b_electron_passMVAID_iso_WP80;   //!
   TBranch        *b_electron_passMVAID_iso_WP90;   //!
   TBranch        *b_electron_passHEEPID;   //!
   TBranch        *b_electron_ptUnCorr;   //!
   TBranch        *b_electron_etaUnCorr;   //!
   TBranch        *b_electron_phiUnCorr;   //!
   TBranch        *b_electron_PxUnCorr;   //!
   TBranch        *b_electron_PyUnCorr;   //!
   TBranch        *b_electron_PzUnCorr;   //!
   TBranch        *b_electron_EnergyUnCorr;   //!
   TBranch        *b_electron_scEnergyUnCorr;   //!
   TBranch        *b_electron_scEtaUnCorr;   //!
   TBranch        *b_electron_scPhiUnCorr;   //!
   TBranch        *b_electron_scEtUnCorr;   //!
   TBranch        *b_electron_mva;   //!
   TBranch        *b_electron_zzmva;   //!
   TBranch        *b_electron_missinghits;   //!
   TBranch        *b_muon_PfChargedHadronIsoR05;   //!
   TBranch        *b_muon_PfNeutralHadronIsoR05;   //!
   TBranch        *b_muon_PfGammaIsoR05;   //!
   TBranch        *b_muon_PfChargedHadronIsoR04;   //!
   TBranch        *b_muon_PfNeutralHadronIsoR04;   //!
   TBranch        *b_muon_PfGammaIsoR04;   //!
   TBranch        *b_muon_PFSumPUIsoR04;   //!
   TBranch        *b_muon_PfChargedHadronIsoR03;   //!
   TBranch        *b_muon_PfNeutralHadronIsoR03;   //!
   TBranch        *b_muon_PfGammaIsoR03;   //!
   TBranch        *b_muon_PFSumPUIsoR03;   //!
   TBranch        *b_muon_isPF;   //!
   TBranch        *b_muon_isGlobal;   //!
   TBranch        *b_muon_isTracker;   //!
   TBranch        *b_muon_isStandAlone;   //!
   TBranch        *b_muon_isTight;   //!
   TBranch        *b_muon_isMedium;   //!
   TBranch        *b_muon_isLoose;   //!
   TBranch        *b_muon_isSoft;   //!
   TBranch        *b_muon_isHighPt;   //!
   TBranch        *b_muon_dB;   //!
   TBranch        *b_muon_phi;   //!
   TBranch        *b_muon_eta;   //!
   TBranch        *b_muon_pt;   //!
   TBranch        *b_muon_cktpt;   //!
   TBranch        *b_muon_cktPx;   //!
   TBranch        *b_muon_cktPy;   //!
   TBranch        *b_muon_cktPz;   //!
   TBranch        *b_muon_cktptError;   //!
   TBranch        *b_muon_Px;   //!
   TBranch        *b_muon_Py;   //!
   TBranch        *b_muon_Pz;   //!
   TBranch        *b_muon_sumtrkpt;   //!
   TBranch        *b_muon_trkiso;   //!
   TBranch        *b_muon_hcaliso;   //!
   TBranch        *b_muon_ecaliso;   //!
   TBranch        *b_muon_trkisoR05;   //!
   TBranch        *b_muon_hcalisoR05;   //!
   TBranch        *b_muon_ecalisoR05;   //!
   TBranch        *b_muon_charge;   //!
   TBranch        *b_muon_nChambers;   //!
   TBranch        *b_muon_matchedstations;   //!
   TBranch        *b_muon_stationMask;   //!
   TBranch        *b_muon_nSegments;   //!
   TBranch        *b_muon_normchi;   //!
   TBranch        *b_muon_validhits;   //!
   TBranch        *b_muon_trackerHits;   //!
   TBranch        *b_muon_pixelHits;   //!
   TBranch        *b_muon_validmuonhits;   //!
   TBranch        *b_muon_trackerLayers;   //!
   TBranch        *b_muon_qoverp;   //!
   TBranch        *b_muon_theta;   //!
   TBranch        *b_muon_lambda;   //!
   TBranch        *b_muon_dxy;   //!
   TBranch        *b_muon_d0;   //!
   TBranch        *b_muon_dsz;   //!
   TBranch        *b_muon_dz;   //!
   TBranch        *b_muon_dxyBS;   //!
   TBranch        *b_muon_dzBS;   //!
   TBranch        *b_muon_dszBS;   //!
   TBranch        *b_muon_dxyVTX;   //!
   TBranch        *b_muon_dzVTX;   //!
   TBranch        *b_muon_dszVTX;   //!
   TBranch        *b_muon_dxycktVTX;   //!
   TBranch        *b_muon_dzcktVTX;   //!
   TBranch        *b_muon_dszcktVTX;   //!
   TBranch        *b_muon_vx;   //!
   TBranch        *b_muon_vy;   //!
   TBranch        *b_muon_vz;   //!
   TBranch        *b_muon_Best_pt;   //!
   TBranch        *b_muon_Best_ptError;   //!
   TBranch        *b_muon_Best_Px;   //!
   TBranch        *b_muon_Best_Py;   //!
   TBranch        *b_muon_Best_Pz;   //!
   TBranch        *b_muon_Best_eta;   //!
   TBranch        *b_muon_Best_phi;   //!
   TBranch        *b_muon_Inner_pt;   //!
   TBranch        *b_muon_Inner_ptError;   //!
   TBranch        *b_muon_Inner_Px;   //!
   TBranch        *b_muon_Inner_Py;   //!
   TBranch        *b_muon_Inner_Pz;   //!
   TBranch        *b_muon_Inner_eta;   //!
   TBranch        *b_muon_Inner_phi;   //!
   TBranch        *b_muon_Outer_pt;   //!
   TBranch        *b_muon_Outer_ptError;   //!
   TBranch        *b_muon_Outer_Px;   //!
   TBranch        *b_muon_Outer_Py;   //!
   TBranch        *b_muon_Outer_Pz;   //!
   TBranch        *b_muon_Outer_eta;   //!
   TBranch        *b_muon_Outer_phi;   //!
   TBranch        *b_muon_GLB_pt;   //!
   TBranch        *b_muon_GLB_ptError;   //!
   TBranch        *b_muon_GLB_Px;   //!
   TBranch        *b_muon_GLB_Py;   //!
   TBranch        *b_muon_GLB_Pz;   //!
   TBranch        *b_muon_GLB_eta;   //!
   TBranch        *b_muon_GLB_phi;   //!
   TBranch        *b_muon_TuneP_pt;   //!
   TBranch        *b_muon_TuneP_ptError;   //!
   TBranch        *b_muon_TuneP_Px;   //!
   TBranch        *b_muon_TuneP_Py;   //!
   TBranch        *b_muon_TuneP_Pz;   //!
   TBranch        *b_muon_TuneP_eta;   //!
   TBranch        *b_muon_TuneP_phi;   //!
   TBranch        *b_muon_roch_sf;   //!
   TBranch        *b_muon_roch_sf_up;   //!
   TBranch        *b_PDFWeights_Scale;   //!
   TBranch        *b_PDFWeights_Error;   //!
   TBranch        *b_PDFWeights_AlphaS;   //!
   TBranch        *b_gen_phi;   //!
   TBranch        *b_gen_eta;   //!
   TBranch        *b_gen_pt;   //!
   TBranch        *b_gen_Px;   //!
   TBranch        *b_gen_Py;   //!
   TBranch        *b_gen_Pz;   //!
   TBranch        *b_gen_E;   //!
   TBranch        *b_gen_mother_PID;   //!
   TBranch        *b_gen_mother_pt;   //!
   TBranch        *b_gen_mother_index;   //!
   TBranch        *b_gen_charge;   //!
   TBranch        *b_gen_status;   //!
   TBranch        *b_gen_PID;   //!
   TBranch        *b_gen_isPrompt;   //!
   TBranch        *b_gen_isPromptFinalState;   //!
   TBranch        *b_gen_isTauDecayProduct;   //!
   TBranch        *b_gen_isPromptTauDecayProduct;   //!
   TBranch        *b_gen_isDirectPromptTauDecayProductFinalState;   //!
   TBranch        *b_gen_isHardProcess;   //!
   TBranch        *b_gen_isLastCopy;   //!
   TBranch        *b_gen_isLastCopyBeforeFSR;   //!
   TBranch        *b_gen_isPromptDecayed;   //!
   TBranch        *b_gen_isDecayedLeptonHadron;   //!
   TBranch        *b_gen_fromHardProcessBeforeFSR;   //!
   TBranch        *b_gen_fromHardProcessDecayed;   //!
   TBranch        *b_gen_fromHardProcessFinalState;   //!
   TBranch        *b_gen_isMostlyLikePythia6Status3;   //!
   TBranch        *b_gen_weight;   //!
   TBranch        *b_genWeight_Q;   //!
   TBranch        *b_genWeight_X1;   //!
   TBranch        *b_genWeight_X2;   //!
   TBranch        *b_genWeight_id1;   //!
   TBranch        *b_genWeight_id2;   //!
   TBranch        *b_genWeight_alphaQCD;   //!
   TBranch        *b_genWeight_alphaQED;   //!
   TBranch        *b_photon_pt;   //!
   TBranch        *b_photon_eta;   //!
   TBranch        *b_photon_phi;   //!
   TBranch        *b_photon_scEta;   //!
   TBranch        *b_photon_scPhi;   //!
   TBranch        *b_photon_HoverE;   //!
   TBranch        *b_photon_hasPixelSeed;   //!
   TBranch        *b_photon_Full5x5_SigmaIEtaIEta;   //!
   TBranch        *b_photon_ChIso;   //!
   TBranch        *b_photon_NhIso;   //!
   TBranch        *b_photon_PhIso;   //!
   TBranch        *b_photon_ChIsoWithEA;   //!
   TBranch        *b_photon_NhIsoWithEA;   //!
   TBranch        *b_photon_PhIsoWithEA;   //!
   TBranch        *b_photon_passMVAID_WP80;   //!
   TBranch        *b_photon_passMVAID_WP90;   //!
   TBranch        *b_photon_passLooseID;   //!
   TBranch        *b_photon_passMediumID;   //!
   TBranch        *b_photon_passTightID;   //!
   TBranch        *b_photon_ptUnCorr;   //!
   TBranch        *b_photon_etaUnCorr;   //!
   TBranch        *b_photon_phiUnCorr;   //!
   TBranch        *b_nPileUp;   //!
   TBranch        *b_pileUpReweightIn;   //!
   TBranch        *b_pileUpReweight;   //!
   TBranch        *b_pileUpReweightPlus;   //!
   TBranch        *b_pileUpReweightMinus;   //!
   TBranch        *b_pileUpReweightInMuonPhys;   //!
   TBranch        *b_pileUpReweightMuonPhys;   //!
   TBranch        *b_pileUpReweightPlusMuonPhys;   //!
   TBranch        *b_pileUpReweightMinusMuonPhys;   //!
   TBranch        *b_pfMET_pt;   //!
   TBranch        *b_pfMET_phi;   //!
   TBranch        *b_pfMET_Px;   //!
   TBranch        *b_pfMET_Py;   //!
   TBranch        *b_pfMET_SumEt;   //!
   TBranch        *b_pfMET_Type1_pt;   //!
   TBranch        *b_pfMET_Type1_phi;   //!
   TBranch        *b_pfMET_Type1_Px;   //!
   TBranch        *b_pfMET_Type1_Py;   //!
   TBranch        *b_pfMET_Type1_SumEt;   //!
   TBranch        *b_pfMET_Type1_PhiCor_pt;   //!
   TBranch        *b_pfMET_Type1_PhiCor_phi;   //!
   TBranch        *b_pfMET_Type1_PhiCor_Px;   //!
   TBranch        *b_pfMET_Type1_PhiCor_Py;   //!
   TBranch        *b_pfMET_Type1_PhiCor_SumEt;   //!

   SKFlatNtuple();
   virtual ~SKFlatNtuple();

   virtual Int_t GetEntry(Long64_t entry);

   virtual void SetTreeName(TString tname){
     fChain = new TChain(tname);
   }

   virtual void AddFile(TString filename){
     fChain->Add(filename);
   }

   bool IsThisDataFile;
   virtual void Init();
   virtual void Loop();

   virtual void executeEvent(){

   };

   TChain *fChain;

};

#endif

#ifdef SKFlatNtuple_cxx
SKFlatNtuple::SKFlatNtuple(){
  IsThisDataFile = false;
}

SKFlatNtuple::~SKFlatNtuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SKFlatNtuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

void SKFlatNtuple::Init()
{

   // Set object pointer
   HLT_TriggerName = 0;
   HLT_TriggerFired = 0;
   HLT_TriggerPrescale = 0;
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_charge = 0;
   jet_area = 0;
   jet_rho = 0;
   jet_partonFlavour = 0;
   jet_hadronFlavour = 0;
   jet_CSVv2 = 0;
   jet_DeepCSV = 0;
   jet_DeepFlavour = 0;
   jet_CvsL = 0;
   jet_CvsB = 0;
   jet_DeepCvsL = 0;
   jet_DeepCvsB = 0;
   jet_chargedHadronEnergyFraction = 0;
   jet_neutralHadronEnergyFraction = 0;
   jet_neutralEmEnergyFraction = 0;
   jet_chargedEmEnergyFraction = 0;
   jet_chargedMultiplicity = 0;
   jet_neutralMultiplicity = 0;
   jet_looseJetID = 0;
   jet_tightJetID = 0;
   jet_tightLepVetoJetID = 0;
   jet_partonPdgId = 0;
   jet_vtxNtracks = 0;
   jet_m = 0;
   jet_energy = 0;
   jet_PileupJetId = 0;
   jet_shiftedEnUp = 0;
   jet_shiftedEnDown = 0;
   fatjet_pt = 0;
   fatjet_eta = 0;
   fatjet_phi = 0;
   fatjet_charge = 0;
   fatjet_area = 0;
   fatjet_rho = 0;
   fatjet_partonFlavour = 0;
   fatjet_hadronFlavour = 0;
   fatjet_CSVv2 = 0;
   fatjet_DeepCSV = 0;
   fatjet_DeepFlavour = 0;
   fatjet_CvsL = 0;
   fatjet_CvsB = 0;
   fatjet_DeepCvsL = 0;
   fatjet_DeepCvsB = 0;
   fatjet_looseJetID = 0;
   fatjet_tightJetID = 0;
   fatjet_tightLepVetoJetID = 0;
   fatjet_partonPdgId = 0;
   fatjet_vtxNtracks = 0;
   fatjet_m = 0;
   fatjet_energy = 0;
   fatjet_puppi_tau1 = 0;
   fatjet_puppi_tau2 = 0;
   fatjet_puppi_tau3 = 0;
   fatjet_puppi_tau4 = 0;
   fatjet_softdropmass = 0;
   fatjet_chargedHadronEnergyFraction = 0;
   fatjet_neutralHadronEnergyFraction = 0;
   fatjet_neutralEmEnergyFraction = 0;
   fatjet_chargedEmEnergyFraction = 0;
   fatjet_chargedMultiplicity = 0;
   fatjet_neutralMultiplicity = 0;
   fatjet_shiftedEnUp = 0;
   fatjet_shiftedEnDown = 0;
   electron_MVAIso = 0;
   electron_MVANoIso = 0;
   electron_et = 0;
   electron_caloEnergy = 0;
   electron_Energy = 0;
   electron_Energy_Scale_Up = 0;
   electron_Energy_Scale_Down = 0;
   electron_Energy_Smear_Up = 0;
   electron_Energy_Smear_Down = 0;
   electron_pt = 0;
   electron_pt_Scale_Up = 0;
   electron_pt_Scale_Down = 0;
   electron_pt_Smear_Up = 0;
   electron_pt_Smear_Down = 0;
   electron_Px = 0;
   electron_Py = 0;
   electron_Pz = 0;
   electron_eta = 0;
   electron_phi = 0;
   electron_charge = 0;
   electron_gsfpt = 0;
   electron_gsfPx = 0;
   electron_gsfPy = 0;
   electron_gsfPz = 0;
   electron_gsfEta = 0;
   electron_gsfPhi = 0;
   electron_gsfCharge = 0;
   electron_scEta = 0;
   electron_scPhi = 0;
   electron_etaWidth = 0;
   electron_phiWidth = 0;
   electron_dEtaIn = 0;
   electron_dEtaInSeed = 0;
   electron_dPhiIn = 0;
   electron_sigmaIEtaIEta = 0;
   electron_Full5x5_SigmaIEtaIEta = 0;
   electron_HoverE = 0;
   electron_fbrem = 0;
   electron_eOverP = 0;
   electron_energyEC = 0;
   electron_Pnorm = 0;
   electron_InvEminusInvP = 0;
   electron_dxyVTX = 0;
   electron_dzVTX = 0;
   electron_dxy = 0;
   electron_sigdxy = 0;
   electron_dz = 0;
   electron_ip3D = 0;
   electron_sigip3D = 0;
   electron_dxyBS = 0;
   electron_dzBS = 0;
   electron_AEff03 = 0;
   electron_chIso03 = 0;
   electron_nhIso03 = 0;
   electron_phIso03 = 0;
   electron_pcIso03 = 0;
   electron_puChIso03 = 0;
   electron_chIso04 = 0;
   electron_nhIso04 = 0;
   electron_phIso04 = 0;
   electron_pcIso04 = 0;
   electron_puChIso04 = 0;
   electron_relIsoCom03 = 0;
   electron_relIsoCom04 = 0;
   electron_relIsoBeta03 = 0;
   electron_relIsoBeta04 = 0;
   electron_relIsoRho03 = 0;
   electron_passConversionVeto = 0;
   electron_isGsfCtfScPixChargeConsistent = 0;
   electron_mHits = 0;
   electron_crack = 0;
   electron_ecalDriven = 0;
   electron_isoEMHADDepth1 = 0;
   electron_25over55 = 0;
   electron_15over55 = 0;
   electron_isoHADDepth2 = 0;
   electron_isoptTrks = 0;
   electron_modIsoEMHADDepth1 = 0;
   electron_modIsoptTrks = 0;
   electron_modIsoEMHADDepth1Orig = 0;
   electron_modIsoptTrksOrig = 0;
   electron_ambGsf0pt = 0;
   electron_ambGsf0Eta = 0;
   electron_ambGsf0Phi = 0;
   electron_ambGsf0Charge = 0;
   electron_ambGsf1pt = 0;
   electron_ambGsf1Eta = 0;
   electron_ambGsf1Phi = 0;
   electron_ambGsf1Charge = 0;
   electron_ambGsf2pt = 0;
   electron_ambGsf2Eta = 0;
   electron_ambGsf2Phi = 0;
   electron_ambGsf2Charge = 0;
   electron_ambGsf3pt = 0;
   electron_ambGsf3Eta = 0;
   electron_ambGsf3Phi = 0;
   electron_ambGsf3Charge = 0;
   electron_r9 = 0;
   electron_scEnergy = 0;
   electron_scPreEnergy = 0;
   electron_scRawEnergy = 0;
   electron_scEt = 0;
   electron_E15 = 0;
   electron_E25 = 0;
   electron_E55 = 0;
   electron_RelPFIso_dBeta = 0;
   electron_RelPFIso_Rho = 0;
   electron_passVetoID = 0;
   electron_passLooseID = 0;
   electron_passMediumID = 0;
   electron_passTightID = 0;
   electron_passMVAID_noIso_WP80 = 0;
   electron_passMVAID_noIso_WP90 = 0;
   electron_passMVAID_iso_WP80 = 0;
   electron_passMVAID_iso_WP90 = 0;
   electron_passHEEPID = 0;
   electron_ptUnCorr = 0;
   electron_etaUnCorr = 0;
   electron_phiUnCorr = 0;
   electron_PxUnCorr = 0;
   electron_PyUnCorr = 0;
   electron_PzUnCorr = 0;
   electron_EnergyUnCorr = 0;
   electron_scEnergyUnCorr = 0;
   electron_scEtaUnCorr = 0;
   electron_scPhiUnCorr = 0;
   electron_scEtUnCorr = 0;
   electron_mva = 0;
   electron_zzmva = 0;
   electron_missinghits = 0;
   muon_PfChargedHadronIsoR05 = 0;
   muon_PfNeutralHadronIsoR05 = 0;
   muon_PfGammaIsoR05 = 0;
   muon_PfChargedHadronIsoR04 = 0;
   muon_PfNeutralHadronIsoR04 = 0;
   muon_PfGammaIsoR04 = 0;
   muon_PFSumPUIsoR04 = 0;
   muon_PfChargedHadronIsoR03 = 0;
   muon_PfNeutralHadronIsoR03 = 0;
   muon_PfGammaIsoR03 = 0;
   muon_PFSumPUIsoR03 = 0;
   muon_isPF = 0;
   muon_isGlobal = 0;
   muon_isTracker = 0;
   muon_isStandAlone = 0;
   muon_isTight = 0;
   muon_isMedium = 0;
   muon_isLoose = 0;
   muon_isSoft = 0;
   muon_isHighPt = 0;
   muon_dB = 0;
   muon_phi = 0;
   muon_eta = 0;
   muon_pt = 0;
   muon_cktpt = 0;
   muon_cktPx = 0;
   muon_cktPy = 0;
   muon_cktPz = 0;
   muon_cktptError = 0;
   muon_Px = 0;
   muon_Py = 0;
   muon_Pz = 0;
   muon_sumtrkpt = 0;
   muon_trkiso = 0;
   muon_hcaliso = 0;
   muon_ecaliso = 0;
   muon_trkisoR05 = 0;
   muon_hcalisoR05 = 0;
   muon_ecalisoR05 = 0;
   muon_charge = 0;
   muon_nChambers = 0;
   muon_matchedstations = 0;
   muon_stationMask = 0;
   muon_nSegments = 0;
   muon_normchi = 0;
   muon_validhits = 0;
   muon_trackerHits = 0;
   muon_pixelHits = 0;
   muon_validmuonhits = 0;
   muon_trackerLayers = 0;
   muon_qoverp = 0;
   muon_theta = 0;
   muon_lambda = 0;
   muon_dxy = 0;
   muon_d0 = 0;
   muon_dsz = 0;
   muon_dz = 0;
   muon_dxyBS = 0;
   muon_dzBS = 0;
   muon_dszBS = 0;
   muon_dxyVTX = 0;
   muon_dzVTX = 0;
   muon_dszVTX = 0;
   muon_dxycktVTX = 0;
   muon_dzcktVTX = 0;
   muon_dszcktVTX = 0;
   muon_vx = 0;
   muon_vy = 0;
   muon_vz = 0;
   muon_Best_pt = 0;
   muon_Best_ptError = 0;
   muon_Best_Px = 0;
   muon_Best_Py = 0;
   muon_Best_Pz = 0;
   muon_Best_eta = 0;
   muon_Best_phi = 0;
   muon_Inner_pt = 0;
   muon_Inner_ptError = 0;
   muon_Inner_Px = 0;
   muon_Inner_Py = 0;
   muon_Inner_Pz = 0;
   muon_Inner_eta = 0;
   muon_Inner_phi = 0;
   muon_Outer_pt = 0;
   muon_Outer_ptError = 0;
   muon_Outer_Px = 0;
   muon_Outer_Py = 0;
   muon_Outer_Pz = 0;
   muon_Outer_eta = 0;
   muon_Outer_phi = 0;
   muon_GLB_pt = 0;
   muon_GLB_ptError = 0;
   muon_GLB_Px = 0;
   muon_GLB_Py = 0;
   muon_GLB_Pz = 0;
   muon_GLB_eta = 0;
   muon_GLB_phi = 0;
   muon_TuneP_pt = 0;
   muon_TuneP_ptError = 0;
   muon_TuneP_Px = 0;
   muon_TuneP_Py = 0;
   muon_TuneP_Pz = 0;
   muon_TuneP_eta = 0;
   muon_TuneP_phi = 0;
   muon_roch_sf = 0;
   muon_roch_sf_up = 0;
   PDFWeights_Scale = 0;
   PDFWeights_Error = 0;
   PDFWeights_AlphaS = 0;
   gen_phi = 0;
   gen_eta = 0;
   gen_pt = 0;
   gen_Px = 0;
   gen_Py = 0;
   gen_Pz = 0;
   gen_E = 0;
   gen_mother_PID = 0;
   gen_mother_pt = 0;
   gen_mother_index = 0;
   gen_charge = 0;
   gen_status = 0;
   gen_PID = 0;
   gen_isPrompt = 0;
   gen_isPromptFinalState = 0;
   gen_isTauDecayProduct = 0;
   gen_isPromptTauDecayProduct = 0;
   gen_isDirectPromptTauDecayProductFinalState = 0;
   gen_isHardProcess = 0;
   gen_isLastCopy = 0;
   gen_isLastCopyBeforeFSR = 0;
   gen_isPromptDecayed = 0;
   gen_isDecayedLeptonHadron = 0;
   gen_fromHardProcessBeforeFSR = 0;
   gen_fromHardProcessDecayed = 0;
   gen_fromHardProcessFinalState = 0;
   gen_isMostlyLikePythia6Status3 = 0;
   photon_pt = 0;
   photon_eta = 0;
   photon_phi = 0;
   photon_scEta = 0;
   photon_scPhi = 0;
   photon_HoverE = 0;
   photon_hasPixelSeed = 0;
   photon_Full5x5_SigmaIEtaIEta = 0;
   photon_ChIso = 0;
   photon_NhIso = 0;
   photon_PhIso = 0;
   photon_ChIsoWithEA = 0;
   photon_NhIsoWithEA = 0;
   photon_PhIsoWithEA = 0;
   photon_passMVAID_WP80 = 0;
   photon_passMVAID_WP90 = 0;
   photon_passLooseID = 0;
   photon_passMediumID = 0;
   photon_passTightID = 0;
   photon_ptUnCorr = 0;
   photon_etaUnCorr = 0;
   photon_phiUnCorr = 0;
   // Set branch addresses and branch pointers

   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("IsData", &IsData, &b_IsData);
   fChain->SetBranchAddress("nTotal", &nTotal, &b_nTotal);
   fChain->SetBranchAddress("run", &run, &b_runNum);
   fChain->SetBranchAddress("event", &event, &b_evtNum);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumiBlock);
   fChain->SetBranchAddress("PUweight", &PUweight, &b_PUweight);
   fChain->SetBranchAddress("nPV", &nPV, &b_nVertices);
   fChain->SetBranchAddress("Flag_badMuons", &Flag_badMuons, &b_Flag_badMuons);
   fChain->SetBranchAddress("Flag_duplicateMuons", &Flag_duplicateMuons, &b_Flag_duplicateMuons);
   fChain->SetBranchAddress("Flag_noBadMuons", &Flag_noBadMuons, &b_Flag_noBadMuons);
   fChain->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices, &b_Flag_goodVertices);
   fChain->SetBranchAddress("Flag_globalTightHalo2016Filter", &Flag_globalTightHalo2016Filter, &b_Flag_globalTightHalo2016Filter);
   fChain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter, &b_Flag_HBHENoiseFilter);
   fChain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter, &b_Flag_HBHENoiseIsoFilter);
   fChain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter, &b_Flag_EcalDeadCellTriggerPrimitiveFilter);
   fChain->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter, &b_Flag_BadPFMuonFilter);
   fChain->SetBranchAddress("Flag_BadChargedCandidateFilter", &Flag_BadChargedCandidateFilter, &b_Flag_BadChargedCandidateFilter);
   fChain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter, &b_Flag_eeBadScFilter);
   fChain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter, &b_Flag_ecalBadCalibFilter);
   fChain->SetBranchAddress("PVtrackSize", &PVtrackSize, &b_PVtrackSize);
   fChain->SetBranchAddress("PVchi2", &PVchi2, &b_PVchi2);
   fChain->SetBranchAddress("PVndof", &PVndof, &b_PVndof);
   fChain->SetBranchAddress("PVnormalizedChi2", &PVnormalizedChi2, &b_PVnormalizedChi2);
   fChain->SetBranchAddress("vertex_X", &vertex_X, &b_PVx);
   fChain->SetBranchAddress("vertex_Y", &vertex_Y, &b_PVy);
   fChain->SetBranchAddress("vertex_Z", &vertex_Z, &b_PVz);
   fChain->SetBranchAddress("HLT_TriggerName", &HLT_TriggerName, &b_HLT_TriggerName);
   fChain->SetBranchAddress("HLT_TriggerFired", &HLT_TriggerFired, &b_HLT_TriggerFired);
   fChain->SetBranchAddress("HLT_TriggerPrescale", &HLT_TriggerPrescale, &b_HLT_TriggerPrescale);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_charge", &jet_charge, &b_jet_charge);
   fChain->SetBranchAddress("jet_area", &jet_area, &b_jet_area);
   fChain->SetBranchAddress("jet_rho", &jet_rho, &b_jet_rho);
   fChain->SetBranchAddress("jet_partonFlavour", &jet_partonFlavour, &b_jet_partonFlavour);
   fChain->SetBranchAddress("jet_hadronFlavour", &jet_hadronFlavour, &b_jet_hadronFlavour);
   fChain->SetBranchAddress("jet_CSVv2", &jet_CSVv2, &b_jet_CSVv2);
   fChain->SetBranchAddress("jet_DeepCSV", &jet_DeepCSV, &b_jet_DeepCSV);
   fChain->SetBranchAddress("jet_DeepFlavour", &jet_DeepFlavour, &b_jet_DeepFlavour);
   fChain->SetBranchAddress("jet_CvsL", &jet_CvsL, &b_jet_CvsL);
   fChain->SetBranchAddress("jet_CvsB", &jet_CvsB, &b_jet_CvsB);
   fChain->SetBranchAddress("jet_DeepCvsL", &jet_DeepCvsL, &b_jet_DeepCvsL);
   fChain->SetBranchAddress("jet_DeepCvsB", &jet_DeepCvsB, &b_jet_DeepCvsB);
   fChain->SetBranchAddress("jet_chargedHadronEnergyFraction", &jet_chargedHadronEnergyFraction, &b_jet_chargedHadronEnergyFraction);
   fChain->SetBranchAddress("jet_neutralHadronEnergyFraction", &jet_neutralHadronEnergyFraction, &b_jet_neutralHadronEnergyFraction);
   fChain->SetBranchAddress("jet_neutralEmEnergyFraction", &jet_neutralEmEnergyFraction, &b_jet_neutralEmEnergyFraction);
   fChain->SetBranchAddress("jet_chargedEmEnergyFraction", &jet_chargedEmEnergyFraction, &b_jet_chargedEmEnergyFraction);
   fChain->SetBranchAddress("jet_chargedMultiplicity", &jet_chargedMultiplicity, &b_jet_chargedMultiplicity);
   fChain->SetBranchAddress("jet_neutralMultiplicity", &jet_neutralMultiplicity, &b_jet_neutralMultiplicity);
   fChain->SetBranchAddress("jet_looseJetID", &jet_looseJetID, &b_jet_looseJetID);
   fChain->SetBranchAddress("jet_tightJetID", &jet_tightJetID, &b_jet_tightJetID);
   fChain->SetBranchAddress("jet_tightLepVetoJetID", &jet_tightLepVetoJetID, &b_jet_tightLepVetoJetID);
   fChain->SetBranchAddress("jet_partonPdgId", &jet_partonPdgId, &b_jet_partonPdgId);
   fChain->SetBranchAddress("jet_vtxNtracks", &jet_vtxNtracks, &b_jet_vtxNtracks);
   fChain->SetBranchAddress("jet_m", &jet_m, &b_jet_m);
   fChain->SetBranchAddress("jet_energy", &jet_energy, &b_jet_energy);
   fChain->SetBranchAddress("jet_PileupJetId", &jet_PileupJetId, &b_jet_PileupJetId);
   fChain->SetBranchAddress("jet_shiftedEnUp", &jet_shiftedEnUp, &b_jet_shiftedEnUp);
   fChain->SetBranchAddress("jet_shiftedEnDown", &jet_shiftedEnDown, &b_jet_shiftedEnDown);
   fChain->SetBranchAddress("fatjet_pt", &fatjet_pt, &b_fatjet_pt);
   fChain->SetBranchAddress("fatjet_eta", &fatjet_eta, &b_fatjet_eta);
   fChain->SetBranchAddress("fatjet_phi", &fatjet_phi, &b_fatjet_phi);
   fChain->SetBranchAddress("fatjet_charge", &fatjet_charge, &b_fatjet_charge);
   fChain->SetBranchAddress("fatjet_area", &fatjet_area, &b_fatjet_area);
   fChain->SetBranchAddress("fatjet_rho", &fatjet_rho, &b_fatjet_rho);
   fChain->SetBranchAddress("fatjet_partonFlavour", &fatjet_partonFlavour, &b_fatjet_partonFlavour);
   fChain->SetBranchAddress("fatjet_hadronFlavour", &fatjet_hadronFlavour, &b_fatjet_hadronFlavour);
   fChain->SetBranchAddress("fatjet_CSVv2", &fatjet_CSVv2, &b_fatjet_CSVv2);
   fChain->SetBranchAddress("fatjet_DeepCSV", &fatjet_DeepCSV, &b_fatjet_DeepCSV);
   fChain->SetBranchAddress("fatjet_DeepFlavour", &fatjet_DeepFlavour, &b_fatjet_DeepFlavour);
   fChain->SetBranchAddress("fatjet_CvsL", &fatjet_CvsL, &b_fatjet_CvsL);
   fChain->SetBranchAddress("fatjet_CvsB", &fatjet_CvsB, &b_fatjet_CvsB);
   fChain->SetBranchAddress("fatjet_DeepCvsL", &fatjet_DeepCvsL, &b_fatjet_DeepCvsL);
   fChain->SetBranchAddress("fatjet_DeepCvsB", &fatjet_DeepCvsB, &b_fatjet_DeepCvsB);
   fChain->SetBranchAddress("fatjet_looseJetID", &fatjet_looseJetID, &b_fatjet_looseJetID);
   fChain->SetBranchAddress("fatjet_tightJetID", &fatjet_tightJetID, &b_fatjet_tightJetID);
   fChain->SetBranchAddress("fatjet_tightLepVetoJetID", &fatjet_tightLepVetoJetID, &b_fatjet_tightLepVetoJetID);
   fChain->SetBranchAddress("fatjet_partonPdgId", &fatjet_partonPdgId, &b_fatjet_partonPdgId);
   fChain->SetBranchAddress("fatjet_vtxNtracks", &fatjet_vtxNtracks, &b_fatjet_vtxNtracks);
   fChain->SetBranchAddress("fatjet_m", &fatjet_m, &b_fatjet_m);
   fChain->SetBranchAddress("fatjet_energy", &fatjet_energy, &b_fatjet_energy);
   fChain->SetBranchAddress("fatjet_puppi_tau1", &fatjet_puppi_tau1, &b_fatjet_puppi_tau1);
   fChain->SetBranchAddress("fatjet_puppi_tau2", &fatjet_puppi_tau2, &b_fatjet_puppi_tau2);
   fChain->SetBranchAddress("fatjet_puppi_tau3", &fatjet_puppi_tau3, &b_fatjet_puppi_tau3);
   fChain->SetBranchAddress("fatjet_puppi_tau4", &fatjet_puppi_tau4, &b_fatjet_puppi_tau4);
   fChain->SetBranchAddress("fatjet_softdropmass", &fatjet_softdropmass, &b_fatjet_softdropmass);
   fChain->SetBranchAddress("fatjet_chargedHadronEnergyFraction", &fatjet_chargedHadronEnergyFraction, &b_fatjet_chargedHadronEnergyFraction);
   fChain->SetBranchAddress("fatjet_neutralHadronEnergyFraction", &fatjet_neutralHadronEnergyFraction, &b_fatjet_neutralHadronEnergyFraction);
   fChain->SetBranchAddress("fatjet_neutralEmEnergyFraction", &fatjet_neutralEmEnergyFraction, &b_fatjet_neutralEmEnergyFraction);
   fChain->SetBranchAddress("fatjet_chargedEmEnergyFraction", &fatjet_chargedEmEnergyFraction, &b_fatjet_chargedEmEnergyFraction);
   fChain->SetBranchAddress("fatjet_chargedMultiplicity", &fatjet_chargedMultiplicity, &b_fatjet_chargedMultiplicity);
   fChain->SetBranchAddress("fatjet_neutralMultiplicity", &fatjet_neutralMultiplicity, &b_fatjet_neutralMultiplicity);
   fChain->SetBranchAddress("fatjet_shiftedEnUp", &fatjet_shiftedEnUp, &b_fatjet_shiftedEnUp);
   fChain->SetBranchAddress("fatjet_shiftedEnDown", &fatjet_shiftedEnDown, &b_fatjet_shiftedEnDown);
   fChain->SetBranchAddress("electron_MVAIso", &electron_MVAIso, &b_electron_MVAIso);
   fChain->SetBranchAddress("electron_MVANoIso", &electron_MVANoIso, &b_electron_MVANoIso);
   fChain->SetBranchAddress("electron_et", &electron_et, &b_electron_et);
   fChain->SetBranchAddress("electron_caloEnergy", &electron_caloEnergy, &b_electron_caloEnergy);
   fChain->SetBranchAddress("electron_Energy", &electron_Energy, &b_electron_Energy);
   fChain->SetBranchAddress("electron_Energy_Scale_Up", &electron_Energy_Scale_Up, &b_electron_Energy_Scale_Up);
   fChain->SetBranchAddress("electron_Energy_Scale_Down", &electron_Energy_Scale_Down, &b_electron_Energy_Scale_Down);
   fChain->SetBranchAddress("electron_Energy_Smear_Up", &electron_Energy_Smear_Up, &b_electron_Energy_Smear_Up);
   fChain->SetBranchAddress("electron_Energy_Smear_Down", &electron_Energy_Smear_Down, &b_electron_Energy_Smear_Down);
   fChain->SetBranchAddress("electron_pt", &electron_pt, &b_electron_pt);
   fChain->SetBranchAddress("electron_pt_Scale_Up", &electron_pt_Scale_Up, &b_electron_pt_Scale_Up);
   fChain->SetBranchAddress("electron_pt_Scale_Down", &electron_pt_Scale_Down, &b_electron_pt_Scale_Down);
   fChain->SetBranchAddress("electron_pt_Smear_Up", &electron_pt_Smear_Up, &b_electron_pt_Smear_Up);
   fChain->SetBranchAddress("electron_pt_Smear_Down", &electron_pt_Smear_Down, &b_electron_pt_Smear_Down);
   fChain->SetBranchAddress("electron_Px", &electron_Px, &b_electron_Px);
   fChain->SetBranchAddress("electron_Py", &electron_Py, &b_electron_Py);
   fChain->SetBranchAddress("electron_Pz", &electron_Pz, &b_electron_Pz);
   fChain->SetBranchAddress("electron_eta", &electron_eta, &b_electron_eta);
   fChain->SetBranchAddress("electron_phi", &electron_phi, &b_electron_phi);
   fChain->SetBranchAddress("electron_charge", &electron_charge, &b_electron_charge);
   fChain->SetBranchAddress("electron_gsfpt", &electron_gsfpt, &b_electron_gsfpt);
   fChain->SetBranchAddress("electron_gsfPx", &electron_gsfPx, &b_electron_gsfPx);
   fChain->SetBranchAddress("electron_gsfPy", &electron_gsfPy, &b_electron_gsfPy);
   fChain->SetBranchAddress("electron_gsfPz", &electron_gsfPz, &b_electron_gsfPz);
   fChain->SetBranchAddress("electron_gsfEta", &electron_gsfEta, &b_electron_gsfEta);
   fChain->SetBranchAddress("electron_gsfPhi", &electron_gsfPhi, &b_electron_gsfPhi);
   fChain->SetBranchAddress("electron_gsfCharge", &electron_gsfCharge, &b_electron_gsfCharge);
   fChain->SetBranchAddress("electron_scEta", &electron_scEta, &b_electron_scEta);
   fChain->SetBranchAddress("electron_scPhi", &electron_scPhi, &b_electron_scPhi);
   fChain->SetBranchAddress("electron_etaWidth", &electron_etaWidth, &b_electron_etaWidth);
   fChain->SetBranchAddress("electron_phiWidth", &electron_phiWidth, &b_electron_phiWidth);
   fChain->SetBranchAddress("electron_dEtaIn", &electron_dEtaIn, &b_electron_dEtaIn);
   fChain->SetBranchAddress("electron_dEtaInSeed", &electron_dEtaInSeed, &b_electron_dEtaInSeed);
   fChain->SetBranchAddress("electron_dPhiIn", &electron_dPhiIn, &b_electron_dPhiIn);
   fChain->SetBranchAddress("electron_sigmaIEtaIEta", &electron_sigmaIEtaIEta, &b_electron_sigmaIEtaIEta);
   fChain->SetBranchAddress("electron_Full5x5_SigmaIEtaIEta", &electron_Full5x5_SigmaIEtaIEta, &b_electron_Full5x5_SigmaIEtaIEta);
   fChain->SetBranchAddress("electron_HoverE", &electron_HoverE, &b_electron_HoverE);
   fChain->SetBranchAddress("electron_fbrem", &electron_fbrem, &b_electron_fbrem);
   fChain->SetBranchAddress("electron_eOverP", &electron_eOverP, &b_electron_eOverP);
   fChain->SetBranchAddress("electron_energyEC", &electron_energyEC, &b_electron_energyEC);
   fChain->SetBranchAddress("electron_Pnorm", &electron_Pnorm, &b_electron_Pnorm);
   fChain->SetBranchAddress("electron_InvEminusInvP", &electron_InvEminusInvP, &b_electron_InvEminusInvP);
   fChain->SetBranchAddress("electron_dxyVTX", &electron_dxyVTX, &b_electron_dxyVTX);
   fChain->SetBranchAddress("electron_dzVTX", &electron_dzVTX, &b_electron_dzVTX);
   fChain->SetBranchAddress("electron_dxy", &electron_dxy, &b_electron_dxy);
   fChain->SetBranchAddress("electron_sigdxy", &electron_sigdxy, &b_electron_sigdxy);
   fChain->SetBranchAddress("electron_dz", &electron_dz, &b_electron_dz);
   fChain->SetBranchAddress("electron_ip3D", &electron_ip3D, &b_electron_ip3D);
   fChain->SetBranchAddress("electron_sigip3D", &electron_sigip3D, &b_electron_sigip3D);
   fChain->SetBranchAddress("electron_dxyBS", &electron_dxyBS, &b_electron_dxyBS);
   fChain->SetBranchAddress("electron_dzBS", &electron_dzBS, &b_electron_dzBS);
   fChain->SetBranchAddress("electron_AEff03", &electron_AEff03, &b_electron_AEff03);
   fChain->SetBranchAddress("electron_chIso03", &electron_chIso03, &b_electron_chIso03);
   fChain->SetBranchAddress("electron_nhIso03", &electron_nhIso03, &b_electron_nhIso03);
   fChain->SetBranchAddress("electron_phIso03", &electron_phIso03, &b_electron_phIso03);
   fChain->SetBranchAddress("electron_pcIso03", &electron_pcIso03, &b_electron_pcIso03);
   fChain->SetBranchAddress("electron_puChIso03", &electron_puChIso03, &b_electron_puChIso03);
   fChain->SetBranchAddress("electron_chIso04", &electron_chIso04, &b_electron_chIso04);
   fChain->SetBranchAddress("electron_nhIso04", &electron_nhIso04, &b_electron_nhIso04);
   fChain->SetBranchAddress("electron_phIso04", &electron_phIso04, &b_electron_phIso04);
   fChain->SetBranchAddress("electron_pcIso04", &electron_pcIso04, &b_electron_pcIso04);
   fChain->SetBranchAddress("electron_puChIso04", &electron_puChIso04, &b_electron_puChIso04);
   fChain->SetBranchAddress("electron_relIsoCom03", &electron_relIsoCom03, &b_electron_relIsoCom03);
   fChain->SetBranchAddress("electron_relIsoCom04", &electron_relIsoCom04, &b_electron_relIsoCom04);
   fChain->SetBranchAddress("electron_relIsoBeta03", &electron_relIsoBeta03, &b_electron_relIsoBeta03);
   fChain->SetBranchAddress("electron_relIsoBeta04", &electron_relIsoBeta04, &b_electron_relIsoBeta04);
   fChain->SetBranchAddress("electron_relIsoRho03", &electron_relIsoRho03, &b_electron_relIsoRho03);
   fChain->SetBranchAddress("electron_passConversionVeto", &electron_passConversionVeto, &b_electron_passConversionVeto);
   fChain->SetBranchAddress("electron_isGsfCtfScPixChargeConsistent", &electron_isGsfCtfScPixChargeConsistent, &b_electron_isGsfCtfScPixChargeConsistent);
   fChain->SetBranchAddress("electron_mHits", &electron_mHits, &b_electron_mHits);
   fChain->SetBranchAddress("electron_crack", &electron_crack, &b_electron_crack);
   fChain->SetBranchAddress("electron_ecalDriven", &electron_ecalDriven, &b_electron_ecalDriven);
   fChain->SetBranchAddress("electron_isoEMHADDepth1", &electron_isoEMHADDepth1, &b_electron_isoEMHADDepth1);
   fChain->SetBranchAddress("electron_25over55", &electron_25over55, &b_electron_25over55);
   fChain->SetBranchAddress("electron_15over55", &electron_15over55, &b_electron_15over55);
   fChain->SetBranchAddress("electron_isoHADDepth2", &electron_isoHADDepth2, &b_electron_isoHADDepth2);
   fChain->SetBranchAddress("electron_isoptTrks", &electron_isoptTrks, &b_electron_isoptTrks);
   fChain->SetBranchAddress("electron_modIsoEMHADDepth1", &electron_modIsoEMHADDepth1, &b_electron_modIsoEMHADDepth1);
   fChain->SetBranchAddress("electron_modIsoptTrks", &electron_modIsoptTrks, &b_electron_modIsoptTrks);
   fChain->SetBranchAddress("electron_modIsoEMHADDepth1Orig", &electron_modIsoEMHADDepth1Orig, &b_electron_modIsoEMHADDepth1Orig);
   fChain->SetBranchAddress("electron_modIsoptTrksOrig", &electron_modIsoptTrksOrig, &b_electron_modIsoptTrksOrig);
   fChain->SetBranchAddress("electron_ambGsf0pt", &electron_ambGsf0pt, &b_electron_ambGsf0pt);
   fChain->SetBranchAddress("electron_ambGsf0Eta", &electron_ambGsf0Eta, &b_electron_ambGsf0Eta);
   fChain->SetBranchAddress("electron_ambGsf0Phi", &electron_ambGsf0Phi, &b_electron_ambGsf0Phi);
   fChain->SetBranchAddress("electron_ambGsf0Charge", &electron_ambGsf0Charge, &b_electron_ambGsf0Charge);
   fChain->SetBranchAddress("electron_ambGsf1pt", &electron_ambGsf1pt, &b_electron_ambGsf1pt);
   fChain->SetBranchAddress("electron_ambGsf1Eta", &electron_ambGsf1Eta, &b_electron_ambGsf1Eta);
   fChain->SetBranchAddress("electron_ambGsf1Phi", &electron_ambGsf1Phi, &b_electron_ambGsf1Phi);
   fChain->SetBranchAddress("electron_ambGsf1Charge", &electron_ambGsf1Charge, &b_electron_ambGsf1Charge);
   fChain->SetBranchAddress("electron_ambGsf2pt", &electron_ambGsf2pt, &b_electron_ambGsf2pt);
   fChain->SetBranchAddress("electron_ambGsf2Eta", &electron_ambGsf2Eta, &b_electron_ambGsf2Eta);
   fChain->SetBranchAddress("electron_ambGsf2Phi", &electron_ambGsf2Phi, &b_electron_ambGsf2Phi);
   fChain->SetBranchAddress("electron_ambGsf2Charge", &electron_ambGsf2Charge, &b_electron_ambGsf2Charge);
   fChain->SetBranchAddress("electron_ambGsf3pt", &electron_ambGsf3pt, &b_electron_ambGsf3pt);
   fChain->SetBranchAddress("electron_ambGsf3Eta", &electron_ambGsf3Eta, &b_electron_ambGsf3Eta);
   fChain->SetBranchAddress("electron_ambGsf3Phi", &electron_ambGsf3Phi, &b_electron_ambGsf3Phi);
   fChain->SetBranchAddress("electron_ambGsf3Charge", &electron_ambGsf3Charge, &b_electron_ambGsf3Charge);
   fChain->SetBranchAddress("electron_r9", &electron_r9, &b_electron_r9);
   fChain->SetBranchAddress("electron_scEnergy", &electron_scEnergy, &b_electron_scEnergy);
   fChain->SetBranchAddress("electron_scPreEnergy", &electron_scPreEnergy, &b_electron_scPreEnergy);
   fChain->SetBranchAddress("electron_scRawEnergy", &electron_scRawEnergy, &b_electron_scRawEnergy);
   fChain->SetBranchAddress("electron_scEt", &electron_scEt, &b_electron_scEt);
   fChain->SetBranchAddress("electron_E15", &electron_E15, &b_electron_E15);
   fChain->SetBranchAddress("electron_E25", &electron_E25, &b_electron_E25);
   fChain->SetBranchAddress("electron_E55", &electron_E55, &b_electron_E55);
   fChain->SetBranchAddress("electron_RelPFIso_dBeta", &electron_RelPFIso_dBeta, &b_electron_RelPFIso_dBeta);
   fChain->SetBranchAddress("electron_RelPFIso_Rho", &electron_RelPFIso_Rho, &b_electron_RelPFIso_Rho);
   fChain->SetBranchAddress("electron_passVetoID", &electron_passVetoID, &b_electron_passVetoID);
   fChain->SetBranchAddress("electron_passLooseID", &electron_passLooseID, &b_electron_passLooseID);
   fChain->SetBranchAddress("electron_passMediumID", &electron_passMediumID, &b_electron_passMediumID);
   fChain->SetBranchAddress("electron_passTightID", &electron_passTightID, &b_electron_passTightID);
   fChain->SetBranchAddress("electron_passMVAID_noIso_WP80", &electron_passMVAID_noIso_WP80, &b_electron_passMVAID_noIso_WP80);
   fChain->SetBranchAddress("electron_passMVAID_noIso_WP90", &electron_passMVAID_noIso_WP90, &b_electron_passMVAID_noIso_WP90);
   fChain->SetBranchAddress("electron_passMVAID_iso_WP80", &electron_passMVAID_iso_WP80, &b_electron_passMVAID_iso_WP80);
   fChain->SetBranchAddress("electron_passMVAID_iso_WP90", &electron_passMVAID_iso_WP90, &b_electron_passMVAID_iso_WP90);
   fChain->SetBranchAddress("electron_passHEEPID", &electron_passHEEPID, &b_electron_passHEEPID);
   fChain->SetBranchAddress("electron_ptUnCorr", &electron_ptUnCorr, &b_electron_ptUnCorr);
   fChain->SetBranchAddress("electron_etaUnCorr", &electron_etaUnCorr, &b_electron_etaUnCorr);
   fChain->SetBranchAddress("electron_phiUnCorr", &electron_phiUnCorr, &b_electron_phiUnCorr);
   fChain->SetBranchAddress("electron_PxUnCorr", &electron_PxUnCorr, &b_electron_PxUnCorr);
   fChain->SetBranchAddress("electron_PyUnCorr", &electron_PyUnCorr, &b_electron_PyUnCorr);
   fChain->SetBranchAddress("electron_PzUnCorr", &electron_PzUnCorr, &b_electron_PzUnCorr);
   fChain->SetBranchAddress("electron_EnergyUnCorr", &electron_EnergyUnCorr, &b_electron_EnergyUnCorr);
   fChain->SetBranchAddress("electron_scEnergyUnCorr", &electron_scEnergyUnCorr, &b_electron_scEnergyUnCorr);
   fChain->SetBranchAddress("electron_scEtaUnCorr", &electron_scEtaUnCorr, &b_electron_scEtaUnCorr);
   fChain->SetBranchAddress("electron_scPhiUnCorr", &electron_scPhiUnCorr, &b_electron_scPhiUnCorr);
   fChain->SetBranchAddress("electron_scEtUnCorr", &electron_scEtUnCorr, &b_electron_scEtUnCorr);
   fChain->SetBranchAddress("electron_mva", &electron_mva, &b_electron_mva);
   fChain->SetBranchAddress("electron_zzmva", &electron_zzmva, &b_electron_zzmva);
   fChain->SetBranchAddress("electron_missinghits", &electron_missinghits, &b_electron_missinghits);
   fChain->SetBranchAddress("muon_PfChargedHadronIsoR05", &muon_PfChargedHadronIsoR05, &b_muon_PfChargedHadronIsoR05);
   fChain->SetBranchAddress("muon_PfNeutralHadronIsoR05", &muon_PfNeutralHadronIsoR05, &b_muon_PfNeutralHadronIsoR05);
   fChain->SetBranchAddress("muon_PfGammaIsoR05", &muon_PfGammaIsoR05, &b_muon_PfGammaIsoR05);
   fChain->SetBranchAddress("muon_PfChargedHadronIsoR04", &muon_PfChargedHadronIsoR04, &b_muon_PfChargedHadronIsoR04);
   fChain->SetBranchAddress("muon_PfNeutralHadronIsoR04", &muon_PfNeutralHadronIsoR04, &b_muon_PfNeutralHadronIsoR04);
   fChain->SetBranchAddress("muon_PfGammaIsoR04", &muon_PfGammaIsoR04, &b_muon_PfGammaIsoR04);
   fChain->SetBranchAddress("muon_PFSumPUIsoR04", &muon_PFSumPUIsoR04, &b_muon_PFSumPUIsoR04);
   fChain->SetBranchAddress("muon_PfChargedHadronIsoR03", &muon_PfChargedHadronIsoR03, &b_muon_PfChargedHadronIsoR03);
   fChain->SetBranchAddress("muon_PfNeutralHadronIsoR03", &muon_PfNeutralHadronIsoR03, &b_muon_PfNeutralHadronIsoR03);
   fChain->SetBranchAddress("muon_PfGammaIsoR03", &muon_PfGammaIsoR03, &b_muon_PfGammaIsoR03);
   fChain->SetBranchAddress("muon_PFSumPUIsoR03", &muon_PFSumPUIsoR03, &b_muon_PFSumPUIsoR03);
   fChain->SetBranchAddress("muon_isPF", &muon_isPF, &b_muon_isPF);
   fChain->SetBranchAddress("muon_isGlobal", &muon_isGlobal, &b_muon_isGlobal);
   fChain->SetBranchAddress("muon_isTracker", &muon_isTracker, &b_muon_isTracker);
   fChain->SetBranchAddress("muon_isStandAlone", &muon_isStandAlone, &b_muon_isStandAlone);
   fChain->SetBranchAddress("muon_isTight", &muon_isTight, &b_muon_isTight);
   fChain->SetBranchAddress("muon_isMedium", &muon_isMedium, &b_muon_isMedium);
   fChain->SetBranchAddress("muon_isLoose", &muon_isLoose, &b_muon_isLoose);
   fChain->SetBranchAddress("muon_isSoft", &muon_isSoft, &b_muon_isSoft);
   fChain->SetBranchAddress("muon_isHighPt", &muon_isHighPt, &b_muon_isHighPt);
   fChain->SetBranchAddress("muon_dB", &muon_dB, &b_muon_dB);
   fChain->SetBranchAddress("muon_phi", &muon_phi, &b_muon_phi);
   fChain->SetBranchAddress("muon_eta", &muon_eta, &b_muon_eta);
   fChain->SetBranchAddress("muon_pt", &muon_pt, &b_muon_pt);
   fChain->SetBranchAddress("muon_cktpt", &muon_cktpt, &b_muon_cktpt);
   fChain->SetBranchAddress("muon_cktPx", &muon_cktPx, &b_muon_cktPx);
   fChain->SetBranchAddress("muon_cktPy", &muon_cktPy, &b_muon_cktPy);
   fChain->SetBranchAddress("muon_cktPz", &muon_cktPz, &b_muon_cktPz);
   fChain->SetBranchAddress("muon_cktptError", &muon_cktptError, &b_muon_cktptError);
   fChain->SetBranchAddress("muon_Px", &muon_Px, &b_muon_Px);
   fChain->SetBranchAddress("muon_Py", &muon_Py, &b_muon_Py);
   fChain->SetBranchAddress("muon_Pz", &muon_Pz, &b_muon_Pz);
   fChain->SetBranchAddress("muon_sumtrkpt", &muon_sumtrkpt, &b_muon_sumtrkpt);
   fChain->SetBranchAddress("muon_trkiso", &muon_trkiso, &b_muon_trkiso);
   fChain->SetBranchAddress("muon_hcaliso", &muon_hcaliso, &b_muon_hcaliso);
   fChain->SetBranchAddress("muon_ecaliso", &muon_ecaliso, &b_muon_ecaliso);
   fChain->SetBranchAddress("muon_trkisoR05", &muon_trkisoR05, &b_muon_trkisoR05);
   fChain->SetBranchAddress("muon_hcalisoR05", &muon_hcalisoR05, &b_muon_hcalisoR05);
   fChain->SetBranchAddress("muon_ecalisoR05", &muon_ecalisoR05, &b_muon_ecalisoR05);
   fChain->SetBranchAddress("muon_charge", &muon_charge, &b_muon_charge);
   fChain->SetBranchAddress("muon_nChambers", &muon_nChambers, &b_muon_nChambers);
   fChain->SetBranchAddress("muon_matchedstations", &muon_matchedstations, &b_muon_matchedstations);
   fChain->SetBranchAddress("muon_stationMask", &muon_stationMask, &b_muon_stationMask);
   fChain->SetBranchAddress("muon_nSegments", &muon_nSegments, &b_muon_nSegments);
   fChain->SetBranchAddress("muon_normchi", &muon_normchi, &b_muon_normchi);
   fChain->SetBranchAddress("muon_validhits", &muon_validhits, &b_muon_validhits);
   fChain->SetBranchAddress("muon_trackerHits", &muon_trackerHits, &b_muon_trackerHits);
   fChain->SetBranchAddress("muon_pixelHits", &muon_pixelHits, &b_muon_pixelHits);
   fChain->SetBranchAddress("muon_validmuonhits", &muon_validmuonhits, &b_muon_validmuonhits);
   fChain->SetBranchAddress("muon_trackerLayers", &muon_trackerLayers, &b_muon_trackerLayers);
   fChain->SetBranchAddress("muon_qoverp", &muon_qoverp, &b_muon_qoverp);
   fChain->SetBranchAddress("muon_theta", &muon_theta, &b_muon_theta);
   fChain->SetBranchAddress("muon_lambda", &muon_lambda, &b_muon_lambda);
   fChain->SetBranchAddress("muon_dxy", &muon_dxy, &b_muon_dxy);
   fChain->SetBranchAddress("muon_d0", &muon_d0, &b_muon_d0);
   fChain->SetBranchAddress("muon_dsz", &muon_dsz, &b_muon_dsz);
   fChain->SetBranchAddress("muon_dz", &muon_dz, &b_muon_dz);
   fChain->SetBranchAddress("muon_dxyBS", &muon_dxyBS, &b_muon_dxyBS);
   fChain->SetBranchAddress("muon_dzBS", &muon_dzBS, &b_muon_dzBS);
   fChain->SetBranchAddress("muon_dszBS", &muon_dszBS, &b_muon_dszBS);
   fChain->SetBranchAddress("muon_dxyVTX", &muon_dxyVTX, &b_muon_dxyVTX);
   fChain->SetBranchAddress("muon_dzVTX", &muon_dzVTX, &b_muon_dzVTX);
   fChain->SetBranchAddress("muon_dszVTX", &muon_dszVTX, &b_muon_dszVTX);
   fChain->SetBranchAddress("muon_dxycktVTX", &muon_dxycktVTX, &b_muon_dxycktVTX);
   fChain->SetBranchAddress("muon_dzcktVTX", &muon_dzcktVTX, &b_muon_dzcktVTX);
   fChain->SetBranchAddress("muon_dszcktVTX", &muon_dszcktVTX, &b_muon_dszcktVTX);
   fChain->SetBranchAddress("muon_vx", &muon_vx, &b_muon_vx);
   fChain->SetBranchAddress("muon_vy", &muon_vy, &b_muon_vy);
   fChain->SetBranchAddress("muon_vz", &muon_vz, &b_muon_vz);
   fChain->SetBranchAddress("muon_Best_pt", &muon_Best_pt, &b_muon_Best_pt);
   fChain->SetBranchAddress("muon_Best_ptError", &muon_Best_ptError, &b_muon_Best_ptError);
   fChain->SetBranchAddress("muon_Best_Px", &muon_Best_Px, &b_muon_Best_Px);
   fChain->SetBranchAddress("muon_Best_Py", &muon_Best_Py, &b_muon_Best_Py);
   fChain->SetBranchAddress("muon_Best_Pz", &muon_Best_Pz, &b_muon_Best_Pz);
   fChain->SetBranchAddress("muon_Best_eta", &muon_Best_eta, &b_muon_Best_eta);
   fChain->SetBranchAddress("muon_Best_phi", &muon_Best_phi, &b_muon_Best_phi);
   fChain->SetBranchAddress("muon_Inner_pt", &muon_Inner_pt, &b_muon_Inner_pt);
   fChain->SetBranchAddress("muon_Inner_ptError", &muon_Inner_ptError, &b_muon_Inner_ptError);
   fChain->SetBranchAddress("muon_Inner_Px", &muon_Inner_Px, &b_muon_Inner_Px);
   fChain->SetBranchAddress("muon_Inner_Py", &muon_Inner_Py, &b_muon_Inner_Py);
   fChain->SetBranchAddress("muon_Inner_Pz", &muon_Inner_Pz, &b_muon_Inner_Pz);
   fChain->SetBranchAddress("muon_Inner_eta", &muon_Inner_eta, &b_muon_Inner_eta);
   fChain->SetBranchAddress("muon_Inner_phi", &muon_Inner_phi, &b_muon_Inner_phi);
   fChain->SetBranchAddress("muon_Outer_pt", &muon_Outer_pt, &b_muon_Outer_pt);
   fChain->SetBranchAddress("muon_Outer_ptError", &muon_Outer_ptError, &b_muon_Outer_ptError);
   fChain->SetBranchAddress("muon_Outer_Px", &muon_Outer_Px, &b_muon_Outer_Px);
   fChain->SetBranchAddress("muon_Outer_Py", &muon_Outer_Py, &b_muon_Outer_Py);
   fChain->SetBranchAddress("muon_Outer_Pz", &muon_Outer_Pz, &b_muon_Outer_Pz);
   fChain->SetBranchAddress("muon_Outer_eta", &muon_Outer_eta, &b_muon_Outer_eta);
   fChain->SetBranchAddress("muon_Outer_phi", &muon_Outer_phi, &b_muon_Outer_phi);
   fChain->SetBranchAddress("muon_GLB_pt", &muon_GLB_pt, &b_muon_GLB_pt);
   fChain->SetBranchAddress("muon_GLB_ptError", &muon_GLB_ptError, &b_muon_GLB_ptError);
   fChain->SetBranchAddress("muon_GLB_Px", &muon_GLB_Px, &b_muon_GLB_Px);
   fChain->SetBranchAddress("muon_GLB_Py", &muon_GLB_Py, &b_muon_GLB_Py);
   fChain->SetBranchAddress("muon_GLB_Pz", &muon_GLB_Pz, &b_muon_GLB_Pz);
   fChain->SetBranchAddress("muon_GLB_eta", &muon_GLB_eta, &b_muon_GLB_eta);
   fChain->SetBranchAddress("muon_GLB_phi", &muon_GLB_phi, &b_muon_GLB_phi);
   fChain->SetBranchAddress("muon_TuneP_pt", &muon_TuneP_pt, &b_muon_TuneP_pt);
   fChain->SetBranchAddress("muon_TuneP_ptError", &muon_TuneP_ptError, &b_muon_TuneP_ptError);
   fChain->SetBranchAddress("muon_TuneP_Px", &muon_TuneP_Px, &b_muon_TuneP_Px);
   fChain->SetBranchAddress("muon_TuneP_Py", &muon_TuneP_Py, &b_muon_TuneP_Py);
   fChain->SetBranchAddress("muon_TuneP_Pz", &muon_TuneP_Pz, &b_muon_TuneP_Pz);
   fChain->SetBranchAddress("muon_TuneP_eta", &muon_TuneP_eta, &b_muon_TuneP_eta);
   fChain->SetBranchAddress("muon_TuneP_phi", &muon_TuneP_phi, &b_muon_TuneP_phi);
   fChain->SetBranchAddress("muon_roch_sf", &muon_roch_sf, &b_muon_roch_sf);
   fChain->SetBranchAddress("muon_roch_sf_up", &muon_roch_sf_up, &b_muon_roch_sf_up);

   if(IsThisDataFile){
     fChain->SetBranchAddress("PDFWeights_Scale", &PDFWeights_Scale, &b_PDFWeights_Scale);
     fChain->SetBranchAddress("PDFWeights_Error", &PDFWeights_Error, &b_PDFWeights_Error);
     fChain->SetBranchAddress("PDFWeights_AlphaS", &PDFWeights_AlphaS, &b_PDFWeights_AlphaS);
		 fChain->SetBranchAddress("gen_phi", &gen_phi, &b_gen_phi);
		 fChain->SetBranchAddress("gen_eta", &gen_eta, &b_gen_eta);
		 fChain->SetBranchAddress("gen_pt", &gen_pt, &b_gen_pt);
		 fChain->SetBranchAddress("gen_Px", &gen_Px, &b_gen_Px);
		 fChain->SetBranchAddress("gen_Py", &gen_Py, &b_gen_Py);
		 fChain->SetBranchAddress("gen_Pz", &gen_Pz, &b_gen_Pz);
		 fChain->SetBranchAddress("gen_E", &gen_E, &b_gen_E);
		 fChain->SetBranchAddress("gen_mother_PID", &gen_mother_PID, &b_gen_mother_PID);
		 fChain->SetBranchAddress("gen_mother_pt", &gen_mother_pt, &b_gen_mother_pt);
		 fChain->SetBranchAddress("gen_mother_index", &gen_mother_index, &b_gen_mother_index);
		 fChain->SetBranchAddress("gen_charge", &gen_charge, &b_gen_charge);
		 fChain->SetBranchAddress("gen_status", &gen_status, &b_gen_status);
		 fChain->SetBranchAddress("gen_PID", &gen_PID, &b_gen_PID);
		 fChain->SetBranchAddress("gen_isPrompt", &gen_isPrompt, &b_gen_isPrompt);
		 fChain->SetBranchAddress("gen_isPromptFinalState", &gen_isPromptFinalState, &b_gen_isPromptFinalState);
		 fChain->SetBranchAddress("gen_isTauDecayProduct", &gen_isTauDecayProduct, &b_gen_isTauDecayProduct);
		 fChain->SetBranchAddress("gen_isPromptTauDecayProduct", &gen_isPromptTauDecayProduct, &b_gen_isPromptTauDecayProduct);
		 fChain->SetBranchAddress("gen_isDirectPromptTauDecayProductFinalState", &gen_isDirectPromptTauDecayProductFinalState, &b_gen_isDirectPromptTauDecayProductFinalState);
		 fChain->SetBranchAddress("gen_isHardProcess", &gen_isHardProcess, &b_gen_isHardProcess);
		 fChain->SetBranchAddress("gen_isLastCopy", &gen_isLastCopy, &b_gen_isLastCopy);
		 fChain->SetBranchAddress("gen_isLastCopyBeforeFSR", &gen_isLastCopyBeforeFSR, &b_gen_isLastCopyBeforeFSR);
		 fChain->SetBranchAddress("gen_isPromptDecayed", &gen_isPromptDecayed, &b_gen_isPromptDecayed);
		 fChain->SetBranchAddress("gen_isDecayedLeptonHadron", &gen_isDecayedLeptonHadron, &b_gen_isDecayedLeptonHadron);
		 fChain->SetBranchAddress("gen_fromHardProcessBeforeFSR", &gen_fromHardProcessBeforeFSR, &b_gen_fromHardProcessBeforeFSR);
		 fChain->SetBranchAddress("gen_fromHardProcessDecayed", &gen_fromHardProcessDecayed, &b_gen_fromHardProcessDecayed);
		 fChain->SetBranchAddress("gen_fromHardProcessFinalState", &gen_fromHardProcessFinalState, &b_gen_fromHardProcessFinalState);
		 fChain->SetBranchAddress("gen_isMostlyLikePythia6Status3", &gen_isMostlyLikePythia6Status3, &b_gen_isMostlyLikePythia6Status3);
		 fChain->SetBranchAddress("gen_weight", &gen_weight, &b_gen_weight);
		 fChain->SetBranchAddress("genWeight_Q", &genWeight_Q, &b_genWeight_Q);
		 fChain->SetBranchAddress("genWeight_X1", &genWeight_X1, &b_genWeight_X1);
		 fChain->SetBranchAddress("genWeight_X2", &genWeight_X2, &b_genWeight_X2);
		 fChain->SetBranchAddress("genWeight_id1", &genWeight_id1, &b_genWeight_id1);
		 fChain->SetBranchAddress("genWeight_id2", &genWeight_id2, &b_genWeight_id2);
		 fChain->SetBranchAddress("genWeight_alphaQCD", &genWeight_alphaQCD, &b_genWeight_alphaQCD);
		 fChain->SetBranchAddress("genWeight_alphaQED", &genWeight_alphaQED, &b_genWeight_alphaQED);
   }

   fChain->SetBranchAddress("photon_pt", &photon_pt, &b_photon_pt);
   fChain->SetBranchAddress("photon_eta", &photon_eta, &b_photon_eta);
   fChain->SetBranchAddress("photon_phi", &photon_phi, &b_photon_phi);
   fChain->SetBranchAddress("photon_scEta", &photon_scEta, &b_photon_scEta);
   fChain->SetBranchAddress("photon_scPhi", &photon_scPhi, &b_photon_scPhi);
   fChain->SetBranchAddress("photon_HoverE", &photon_HoverE, &b_photon_HoverE);
   fChain->SetBranchAddress("photon_hasPixelSeed", &photon_hasPixelSeed, &b_photon_hasPixelSeed);
   fChain->SetBranchAddress("photon_Full5x5_SigmaIEtaIEta", &photon_Full5x5_SigmaIEtaIEta, &b_photon_Full5x5_SigmaIEtaIEta);
   fChain->SetBranchAddress("photon_ChIso", &photon_ChIso, &b_photon_ChIso);
   fChain->SetBranchAddress("photon_NhIso", &photon_NhIso, &b_photon_NhIso);
   fChain->SetBranchAddress("photon_PhIso", &photon_PhIso, &b_photon_PhIso);
   fChain->SetBranchAddress("photon_ChIsoWithEA", &photon_ChIsoWithEA, &b_photon_ChIsoWithEA);
   fChain->SetBranchAddress("photon_NhIsoWithEA", &photon_NhIsoWithEA, &b_photon_NhIsoWithEA);
   fChain->SetBranchAddress("photon_PhIsoWithEA", &photon_PhIsoWithEA, &b_photon_PhIsoWithEA);
   fChain->SetBranchAddress("photon_passMVAID_WP80", &photon_passMVAID_WP80, &b_photon_passMVAID_WP80);
   fChain->SetBranchAddress("photon_passMVAID_WP90", &photon_passMVAID_WP90, &b_photon_passMVAID_WP90);
   fChain->SetBranchAddress("photon_passLooseID", &photon_passLooseID, &b_photon_passLooseID);
   fChain->SetBranchAddress("photon_passMediumID", &photon_passMediumID, &b_photon_passMediumID);
   fChain->SetBranchAddress("photon_passTightID", &photon_passTightID, &b_photon_passTightID);
   fChain->SetBranchAddress("photon_ptUnCorr", &photon_ptUnCorr, &b_photon_ptUnCorr);
   fChain->SetBranchAddress("photon_etaUnCorr", &photon_etaUnCorr, &b_photon_etaUnCorr);
   fChain->SetBranchAddress("photon_phiUnCorr", &photon_phiUnCorr, &b_photon_phiUnCorr);
   fChain->SetBranchAddress("nPileUp", &nPileUp, &b_nPileUp);
   fChain->SetBranchAddress("pileUpReweightIn", &pileUpReweightIn, &b_pileUpReweightIn);
   fChain->SetBranchAddress("pileUpReweight", &pileUpReweight, &b_pileUpReweight);
   fChain->SetBranchAddress("pileUpReweightPlus", &pileUpReweightPlus, &b_pileUpReweightPlus);
   fChain->SetBranchAddress("pileUpReweightMinus", &pileUpReweightMinus, &b_pileUpReweightMinus);
   fChain->SetBranchAddress("pileUpReweightInMuonPhys", &pileUpReweightInMuonPhys, &b_pileUpReweightInMuonPhys);
   fChain->SetBranchAddress("pileUpReweightMuonPhys", &pileUpReweightMuonPhys, &b_pileUpReweightMuonPhys);
   fChain->SetBranchAddress("pileUpReweightPlusMuonPhys", &pileUpReweightPlusMuonPhys, &b_pileUpReweightPlusMuonPhys);
   fChain->SetBranchAddress("pileUpReweightMinusMuonPhys", &pileUpReweightMinusMuonPhys, &b_pileUpReweightMinusMuonPhys);
   fChain->SetBranchAddress("pfMET_pt", &pfMET_pt, &b_pfMET_pt);
   fChain->SetBranchAddress("pfMET_phi", &pfMET_phi, &b_pfMET_phi);
   fChain->SetBranchAddress("pfMET_Px", &pfMET_Px, &b_pfMET_Px);
   fChain->SetBranchAddress("pfMET_Py", &pfMET_Py, &b_pfMET_Py);
   fChain->SetBranchAddress("pfMET_SumEt", &pfMET_SumEt, &b_pfMET_SumEt);
   fChain->SetBranchAddress("pfMET_Type1_pt", &pfMET_Type1_pt, &b_pfMET_Type1_pt);
   fChain->SetBranchAddress("pfMET_Type1_phi", &pfMET_Type1_phi, &b_pfMET_Type1_phi);
   fChain->SetBranchAddress("pfMET_Type1_Px", &pfMET_Type1_Px, &b_pfMET_Type1_Px);
   fChain->SetBranchAddress("pfMET_Type1_Py", &pfMET_Type1_Py, &b_pfMET_Type1_Py);
   fChain->SetBranchAddress("pfMET_Type1_SumEt", &pfMET_Type1_SumEt, &b_pfMET_Type1_SumEt);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_pt", &pfMET_Type1_PhiCor_pt, &b_pfMET_Type1_PhiCor_pt);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_phi", &pfMET_Type1_PhiCor_phi, &b_pfMET_Type1_PhiCor_phi);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_Px", &pfMET_Type1_PhiCor_Px, &b_pfMET_Type1_PhiCor_Px);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_Py", &pfMET_Type1_PhiCor_Py, &b_pfMET_Type1_PhiCor_Py);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_SumEt", &pfMET_Type1_PhiCor_SumEt, &b_pfMET_Type1_PhiCor_SumEt);
}


#endif
