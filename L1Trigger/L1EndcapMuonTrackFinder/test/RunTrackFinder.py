import FWCore.ParameterSet.Config as cms

process = cms.Process('L1EMTF')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10))
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1))
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff")
process.load("L1TriggerConfig.L1ScalesProducers.L1MuTriggerPtScaleConfig_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('L1Trigger.L1EndcapMuonTrackFinder.L1TMuonTriggerPrimitiveProducer_cfi')
process.load('Configuration.Geometry.GeometryExtendedPostLS1Reco_cff')
process.load('Configuration.Geometry.GeometryExtendedPostLS1_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')


process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.L1TMuonEndcapTrackFinder = cms.EDProducer(
    'L1TMuonUpgradedTrackFinder',
    doGen = cms.untracked.bool(True),
    genSrc = cms.untracked.InputTag("genParticles"),
    primitiveSrcs = cms.VInputTag(
    cms.InputTag('L1TMuonTriggerPrimitives','CSC'),
    cms.InputTag('L1TMuonTriggerPrimitives','DT'),
    cms.InputTag('L1TMuonTriggerPrimitives','RPC')
    ),
    converterSrcs = cms.VInputTag(
    cms.InputTag('L1CSCTFTrackConverter'),
    cms.InputTag('L1DTTFTrackConverter'),
    cms.InputTag('L1RPCbTFTrackConverter'),
    cms.InputTag('L1RPCfTFTrackConverter'),
    cms.InputTag('L1TMuonSimpleDeltaEtaHitMatcher')
    ),
    lutParam = cms.PSet(
    isBeamStartConf = cms.untracked.bool(True),
    ReadPtLUT = cms.bool(False),
    PtMethod = cms.untracked.uint32(32)
    )
)

process.content = cms.EDAnalyzer("EventContentAnalyzer")

infile = ['/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/04DB6E17-72E2-E411-8311-0025905964BA.root',
       '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/24978F06-72E2-E411-8346-0025905A6084.root',
       '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/469C811A-72E2-E411-B1EF-0025905A6118.root',
       '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/AAD41A17-72E2-E411-A617-0025905A607E.root',
       '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt100_UP15/GEN-SIM-DIGI-RECO/MCRUN2_74_V7_FastSim-v1/00000/C4F4E747-71E2-E411-8305-0026189438AB.root' ]
#'file:/afs/cern.ch/work/m/mcarver/MuonDevelopment/CMSSW_7_2_0_pre6/src/L1Trigger/L1TMuon/test/Sector1Events/1p2_2p0_Job1.root'
#infile = 'file:MuGun_test.root'
fileOutName = "Histos.root"

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(infile)
    
    )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(
	fileOutName
))


outCommands = cms.untracked.vstring('keep *')

process.FEVTDEBUGoutput = cms.OutputModule(
    "PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = outCommands,
    fileName = cms.untracked.string('Emulator_EDM_Out.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)

process.L1TMuonSequence = cms.Sequence(process.L1TMuonTriggerPrimitives + 
									process.L1TMuonEndcapTrackFinder)

process.L1TMuonPath = cms.Path(process.L1TMuonSequence)

process.outPath = cms.EndPath(process.FEVTDEBUGoutput)

process.schedule = cms.Schedule(process.L1TMuonPath,process.outPath)

