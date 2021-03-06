import FWCore.ParameterSet.Config as cms
process = cms.Process("PileUp")


process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")
process.RandomNumberGeneratorService.generator.initialSeed = 123456789

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100000)
)


from Configuration.Generator.PythiaUEZ2Settings_cfi import *

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    crossSection = cms.untracked.double(72850000000.),
    comEnergy = cms.double(8000.0),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        processParameters = cms.vstring('MSEL=0         ! User defined processes', 
            'MSUB(11)=1     ! Min bias process', 
            'MSUB(12)=1     ! Min bias process', 
            'MSUB(13)=1     ! Min bias process', 
            'MSUB(28)=1     ! Min bias process', 
            'MSUB(53)=1     ! Min bias process', 
            'MSUB(68)=1     ! Min bias process', 
            'MSUB(92)=1     ! Min bias process, single diffractive', 
            'MSUB(93)=1     ! Min bias process, single diffractive', 
            'MSUB(94)=1     ! Min bias process, double diffractive', 
            'MSUB(95)=1     ! Min bias process'),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters', 'myParameters'),
        #    Disable the ctau check
        myParameters = cms.vstring('PARJ(71) = -1.')
    )
)


process.prodPU = cms.EDProducer("producePileUpEvents",
    PUParticleFilter = cms.PSet(
        # Protons with energy larger than EProton (GeV) are all kept
        EProton = cms.double(5000.0),
        # Particles with |eta| > etaMax (momentum direction at primary vertex) 
        # are not simulated - 7.0 includes CASTOR (which goes to 6.6) 
        etaMax = cms.double(7.0),
        # Charged particles with pT < pTMin (GeV/c) are not simulated
        pTMin = cms.double(0.0),
        # Particles with energy smaller than EMin (GeV) are not simulated
        EMin = cms.double(0.0)
    ),
    PUEventFile = cms.untracked.string('MinBias8TeV.root'),
    SavePileUpEvents = cms.bool(True),
    BunchPileUpEventSize = cms.uint32(1000)
)

process.source = cms.Source("EmptySource")
process.p = cms.Path(process.generator*process.prodPU)


