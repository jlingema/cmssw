import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TMuonEmulation")
import os
import sys
import commands

process.load("FWCore.MessageLogger.MessageLogger_cfi")

verbose = False

if verbose:
    process.MessageLogger = cms.Service("MessageLogger",
       suppressInfo       = cms.untracked.vstring('AfterSource', 'PostModule'),
       destinations   = cms.untracked.vstring(
                                             'detailedInfo'
                                             ,'critical'
                                             ,'cout'
                    ),
       categories = cms.untracked.vstring(
                                        'CondDBESSource'
                                        ,'EventSetupDependency'
                                        ,'Geometry'
                                        ,'MuonGeom'
                                        ,'GetManyWithoutRegistration'
                                        ,'GetByLabelWithoutRegistration'
                                        ,'Alignment'
                                        ,'SiStripBackPlaneCorrectionDepESProducer'
                                        ,'SiStripLorentzAngleDepESProducer'
                                        ,'SiStripQualityESProducer'
                                        ,'TRACKER'
                                        ,'HCAL'
        ),
       critical       = cms.untracked.PSet(
                        threshold = cms.untracked.string('ERROR')
        ),
       detailedInfo   = cms.untracked.PSet(
                      threshold  = cms.untracked.string('INFO'),
                      CondDBESSource  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                      EventSetupDependency  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                      Geometry  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                      MuonGeom  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                      Alignment  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                      GetManyWithoutRegistration  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                      GetByLabelWithoutRegistration  = cms.untracked.PSet (limit = cms.untracked.int32(0) )

       ),
       cout   = cms.untracked.PSet(
                threshold  = cms.untracked.string('WARNING'),
                CondDBESSource  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                EventSetupDependency  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                Geometry  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                MuonGeom  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                Alignment  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                GetManyWithoutRegistration  = cms.untracked.PSet (limit = cms.untracked.int32(0) ),
                GetByLabelWithoutRegistration  = cms.untracked.PSet (limit = cms.untracked.int32(0) )
                ),
)

if not verbose:
    process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(5000)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring(
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/16BF2D14-83E3-E411-B212-003048FFD756.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/26833213-83E3-E411-9238-0025905B8590.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/5E967412-83E3-E411-9DA0-003048FFD756.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/686FB705-83E3-E411-A8FC-003048FF9AC6.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/8E6F7913-83E3-E411-B72F-0025905A48BB.root'
      # MinBias
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/168F493A-18E2-E411-B4DB-0025905A611C.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/18F0D483-3AE2-E411-B838-002590596468.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/201FF053-18E2-E411-8F67-0025905A607A.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/2E03084A-18E2-E411-A278-0025905B858E.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/4CCB6439-18E2-E411-AD10-0025905B85AE.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/64679018-18E2-E411-BE88-0025905A60B8.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/6624EE1E-18E2-E411-93CD-0025905A60A6.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/74C43C0E-18E2-E411-BD51-0025905964C0.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/7651B439-18E2-E411-98EF-0025905A60E0.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/76CE1835-18E2-E411-A079-0025905A612E.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/8C48D51F-18E2-E411-8FE4-0025905A6088.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/905F7122-18E2-E411-9B0C-0025905A48BC.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/9AC8BDBA-1AE2-E411-B698-0025905938A4.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/9C32F212-72E2-E411-917D-0025905A612E.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/9EFB19DA-2FE3-E411-AC6E-003048FFD752.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/A225AF0D-12E2-E411-919E-00261894393F.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/A635D3E1-78E2-E411-BF17-003048FFCB8C.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/ACFC933C-18E2-E411-922A-0025905A60BE.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/B2B93F19-18E2-E411-AAB9-0025905A60D2.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/B2C80B11-18E2-E411-B662-0025905A60CE.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/BE0DDD48-18E2-E411-86B5-0025905A6076.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/BECF893A-12E2-E411-A394-0025905A497A.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/BEDC1B1D-18E2-E411-A90A-0025905B85AA.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/C8F28E3A-12E2-E411-8242-0025905A497A.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/CC366BF4-74E2-E411-B80C-002618FDA204.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/CE154754-12E2-E411-BE38-0025905A610C.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/D4ED1660-12E2-E411-8BB8-0025905938AA.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/D6474152-12E2-E411-A5D4-0025905A6118.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/DE8BFB31-18E2-E411-927E-0025905A6090.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/E0143211-18E2-E411-B345-0025905A60FE.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/E4227318-18E2-E411-AB99-0025905A48B2.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/E47CC83C-48E3-E411-B7E6-0025905A6122.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/EC9F66BC-36E3-E411-AD21-002618943849.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/F09BA862-81E2-E411-B5FE-002618943854.root',
      # '/store/relval/CMSSW_7_5_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/FA2D530D-18E2-E411-A2E9-0025905A48F0.root'
      )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000))

###PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtendedPostLS1Reco_cff')
process.load('Configuration.Geometry.GeometryExtendedPostLS1_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

path = os.environ['CMSSW_BASE']+"/src/L1Trigger/L1OverlapMuonTrackFinder/data/"

process.load('L1Trigger.L1EndcapMuonTrackFinder.L1TMuonTriggerPrimitiveProducer_cfi')

###OMTF emulator configuration
process.omtfEmulator = cms.EDProducer("OMTFProducer",
                                      TriggerPrimitiveSrc = cms.InputTag('L1TMuonTriggerPrimitives'),
                                      dumpResultToXML = cms.bool(False),
                                      XMLDumpFileName = cms.string("TestEvents.xml"),
                                      dumpGPToXML = cms.bool(False),
                                      readEventsFromXML = cms.bool(False),
                                      eventsXMLFiles = cms.vstring("TestEvents.xml"),
                                      dropRPCPrimitives = cms.bool(False),
                                      dropDTPrimitives = cms.bool(False),
                                      dropCSCPrimitives = cms.bool(False),
                                      omtf = cms.PSet(
        configXMLFile = cms.string(path+"hwToLogicLayer_721_5760.xml"),
        patternsXMLFiles = cms.vstring(path+"Patterns_ipt4_31_5760.xml"),
        )
                                      )

process.uGMTInputProducer = cms.EDProducer("l1t::uGMTInputProducerFromGen",
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

process.load("L1TriggerDPG.L1Ntuples.l1NtupleProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1RecoTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1ExtraTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1MuonRecoTreeProducer_cfi")
process.load("L1TriggerDPG.L1Ntuples.l1MuonUpgradeTreeProducer_cfi")

process.load("L1Trigger.L1TMuon.microgmtemulator_cfi")

process.microGMTEmulator.overlapTFInput = cms.InputTag("omtfEmulator", "OMTF")
process.l1MuonUpgradeTreeProducer.omtfTag = cms.InputTag("omtfEmulator", "OMTF")
process.microGMTEmulator.forwardTFInput = cms.InputTag("L1TMuonEndcapTrackFinder", "EMUTF")
process.l1MuonUpgradeTreeProducer.emtfTag = cms.InputTag("L1TMuonEndcapTrackFinder", "EMUTF")

#disable pre-loaded cancel-out lookup tables (they currently contain only 0)
process.microGMTEmulator.OvlNegSingleMatchQualLUTSettings.filename = cms.string("")
process.microGMTEmulator.OvlPosSingleMatchQualLUTSettings.filename = cms.string("")
process.microGMTEmulator.FOPosMatchQualLUTSettings.filename = cms.string("")
process.microGMTEmulator.FONegMatchQualLUTSettings.filename = cms.string("")
# output file
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('l1ntuple.root')
)

process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")

# analysis


process.l1NtupleProducer.hltSource            = cms.InputTag("none")
process.l1NtupleProducer.gtSource             = cms.InputTag("none")
process.l1NtupleProducer.gctCentralJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctNonIsoEmSource    = cms.InputTag("none")
process.l1NtupleProducer.gctForwardJetsSource = cms.InputTag("none")
process.l1NtupleProducer.gctIsoEmSource       = cms.InputTag("none")
process.l1NtupleProducer.gctEnergySumsSource  = cms.InputTag("none")
process.l1NtupleProducer.gctTauJetsSource     = cms.InputTag("none")
process.l1NtupleProducer.gctIsoTauJetsSource  = cms.InputTag("none")
process.l1NtupleProducer.rctSource            = cms.InputTag("none")
process.l1NtupleProducer.dttfSource           = cms.InputTag("none")
process.l1NtupleProducer.ecalSource           = cms.InputTag("none")
process.l1NtupleProducer.hcalSource           = cms.InputTag("none")
process.l1NtupleProducer.csctfTrkSource       = cms.InputTag("none")
process.l1NtupleProducer.csctfLCTSource       = cms.InputTag("none")
process.l1NtupleProducer.csctfLCTSource       = cms.InputTag("none")
process.l1NtupleProducer.generatorSource    = cms.InputTag("genParticles")
csctfDTStubsSource   = cms.InputTag("none")



process.L1TMuonSeq = cms.Sequence(
    process.gtDigis
    +process.gtEvmDigis
    # +process.gctDigis
    # +process.dttfDigis
    # +process.csctfDigis
    # +process.l1extraParticles
    # +process.l1ExtraTreeProducer
    # +process.l1GtTriggerMenuLite
    # +process.l1MenuTreeProducer
    # +process.l1RecoTreeProducer
    # +process.l1MuonRecoTreeProducer
    +process.L1TMuonTriggerPrimitives
    +process.omtfEmulator
    +process.L1TMuonEndcapTrackFinder
    +process.uGMTInputProducer
    +process.microGMTEmulator
    +process.l1NtupleProducer
    +process.l1MuonUpgradeTreeProducer
    )
    #  +
    # process.emtfEmulator +
    # process.bmtfEmulator +
    # process.caloEmulator)

process.L1TMuonPath = cms.Path(process.L1TMuonSeq)


process.out = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
              'drop *',
              'keep *_*_*_L1TMuonEmulation'),
   fileName = cms.untracked.string("l1tmuon_test.root"),
)

process.output_step = cms.EndPath(process.out)

process.schedule = cms.Schedule(process.L1TMuonPath)
process.schedule.extend([process.output_step])
