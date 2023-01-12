rm -rf ./*.root

cp /data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_ChargeFlip/2016preVFP/MuonCF__/HNL_ChargeFlip_DYJets.root 2016preDY.root
cp /data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_ChargeFlip/2016preVFP/MuonCF__/HNL_ChargeFlip_TTLL_powheg.root 2016preTT.root
cp /data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_ChargeFlip/2016postVFP/MuonCF__/HNL_ChargeFlip_DYJets.root 2016postDY.root
cp /data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_ChargeFlip/2016postVFP/MuonCF__/HNL_ChargeFlip_TTLL_powheg.root 2016postTT.root

cp /data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_ChargeFlip/2018/MuonCF__/HNL_ChargeFlip_DYJets.root 2018DY.root
cp /data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_ChargeFlip/2018/MuonCF__/HNL_ChargeFlip_TTLL_powheg.root 2018TT.root
cp /data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_ChargeFlip/2017/MuonCF__/HNL_ChargeFlip_DYJets.root 2017DY.root
cp /data9/Users/youngwan/SKFlatOutput/Run2UltraLegacy_v3/HNL_ChargeFlip/2017/MuonCF__/HNL_ChargeFlip_TTLL_powheg.root 2017TT.root

hadd HNL_ChargeFlip_2016.root 2016*.root
hadd HNL_ChargeFlip_2017.root 2017*.root
hadd HNL_ChargeFlip_2018.root 2018*.root