import FWCore.ParameterSet.Config as cms

externalLHEProducer = cms.EDProducer("ExternalLHEProducer",
    args = cms.vstring('/cvmfs/cms.cern.ch/phys_generator/gridpacks/2017/13TeV/madgraph/V5_2.6.5/DileptonTypeIHeavyN/DYTypeI_NLO_DF_M95_slc6_amd64_gcc630_CMSSW_9_3_16_tarball.tar.xz'),
    nEvents = cms.untracked.uint32(5000),
    numberOfParameters = cms.uint32(1),
    outputFile = cms.string('cmsgrid_final.lhe'),
    scriptName = cms.FileInPath('GeneratorInterface/LHEInterface/data/run_generic_tarball_cvmfs.sh')
)
import FWCore.ParameterSet.Config as cms

from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import *
from Configuration.Generator.Pythia8aMCatNLOSettings_cfi import *
from Configuration.Generator.PSweightsPythia.PythiaPSweightsSettings_cfi import *

generator = cms.EDFilter("Pythia8HadronizerFilter",
    maxEventsToPrint = cms.untracked.int32(1),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(13000.),
    PythiaParameters = cms.PSet(
        pythia8CommonSettingsBlock,
        pythia8CP5SettingsBlock,
        pythia8aMCatNLOSettingsBlock,
        pythia8PSweightsSettingsBlock,
        processParameters = cms.vstring(
            'TimeShower:nPartonsInBorn = 0', #number of coloured particles (before resonance decays) in born matrix element
        ),
        parameterSets = cms.vstring('pythia8CommonSettings',
                                    'pythia8CP5Settings',
                                    'pythia8aMCatNLOSettings',
                                    'pythia8PSweightsSettings',
                                    'processParameters',
                                    )
    )
)

LepPtFilter1 = cms.EDFilter("MCMultiParticleFilter",
    AcceptMore = cms.bool(True),
    NumRequired = cms.int32(2),
    ParticleID  = cms.vint32           (     13,     -13,      13,     -13,      11,     -11,   13, -13,  11,  -11,  13, -13,  11, -11),
    MotherID    = cms.untracked.vint32 (9900012, 9900012, 9900014, 9900014, 9900012, 9900012,  -24,  24, -24,   24,   0,   0,   0,   0),
    PtMin       = cms.vdouble          (     5.,      5.,      5.,      5.,      5.,      5.,   5.,  5.,  5.,   5.,  5.,  5.,  5.,  5.),
    EtaMax      = cms.vdouble          (    1E6,     1E6,     1E6,     1E6,     1E6,     1E6,  1E6, 1E6, 1E6,  1E6, 1E6, 1E6, 1E6, 1E6),
    Status      = cms.vint32           (      1,       1,       1,       1,       1,       1,    1,   1,   1,    1,  23,  23,  23,  23)
)

LepPtFilter2 = cms.EDFilter("MCMultiParticleFilter",
    AcceptMore = cms.bool(True),
    NumRequired = cms.int32(1),
    ParticleID  = cms.vint32           (     13,     -13,      13,     -13,      11,     -11,   13, -13,  11,  -11,  13, -13,  11, -11),
    MotherID    = cms.untracked.vint32 (9900012, 9900012, 9900014, 9900014, 9900012, 9900012,  -24,  24, -24,   24,   0,   0,   0,   0),
    PtMin       = cms.vdouble          (    15.,     15.,     15.,     15.,     15.,     15.,  15., 15., 15.,  15., 15., 15., 15., 15.),
    EtaMax      = cms.vdouble          (    1E6,     1E6,     1E6,     1E6,     1E6,     1E6,  1E6, 1E6, 1E6,  1E6, 1E6, 1E6, 1E6, 1E6),
    Status      = cms.vint32           (      1,       1,       1,       1,       1,       1,    1,   1,   1,    1,  23,  23,  23,  23)
)

ProductionFilterSequence = cms.Sequence(generator*LepPtFilter1*LepPtFilter2)


# Link to generator fragment:
# ./genFragments/Hadronizer/13TeV/Hadronizer_TuneCP5_13TeV_aMCatNLO_0p_LHE_pythia8_LepPtFilter_cff.py
