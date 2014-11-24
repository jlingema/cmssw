#!/usr/bin/env python
import sys

"""
The parameters can be changed by adding commandline arguments of the form
::

    runGlobalFakeInputProducer.py nevents=-1

The latter can be used to change parameters in crab.
"""

job = 0 #job number
njob = 1 #number of jobs
nevents = 3564 #number of events
rootout = False #whether to produce root file
dump = False #dump python

# Argument parsing
# vvv

if len(sys.argv) > 1 and sys.argv[1].endswith('.py'):
    sys.argv.pop(0)
if len(sys.argv) == 2 and ':' in sys.argv[1]:
    argv = sys.argv[1].split(':')
else:
    argv = sys.argv[1:]

for arg in argv:
    (k, v) = map(str.strip, arg.split('='))
    if k not in globals():
        raise "Unknown argument '%s'!" % (k,)
    if type(globals()[k]) == bool:
        globals()[k] = v.lower() in ('y', 'yes', 'true', 't', '1')
    elif type(globals()[k]) == int:
        globals()[k] = int(v)
    else:
        globals()[k] = v

neventsPerJob = nevents/njob
skip = job * neventsPerJob

if skip>4:
    skip = skip-4
    neventsPerJob = neventsPerJob+4

import FWCore.ParameterSet.Config as cms

process = cms.Process('L1TEMULATION')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')



# Select the Message Logger output you would like to see:
#
process.load('FWCore.MessageService.MessageLogger_cfi')
#process.load('L1Trigger/L1TYellow/l1t_debug_messages_cfi')
#process.load('L1Trigger/L1TYellow/l1t_info_messages_cfi')

process.load('L1Trigger/L1TGlobal/debug_messages_cfi')
process.MessageLogger.l1t_debug.l1t.limit = cms.untracked.int32(100000)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(neventsPerJob)
    )

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    #### MinBias:
    # fileNames = cms.untracked.vstring(
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/088590EF-DA59-E411-9F9E-0025905B85E8.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/2A5714A6-D959-E411-834E-0025905A607A.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/3076DDFD-D959-E411-96F3-0025905B8610.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/46B344AD-D959-E411-A9BC-0026189438AD.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/46FCEB7D-E159-E411-8487-0025905B8606.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/6AC993CD-E959-E411-8490-0026189438E9.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/6E732DFF-D959-E411-8D19-003048FFD730.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/82D03701-DA59-E411-8337-0025905A6090.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/8495BBCD-EA59-E411-8E86-0025905A609E.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/941920C5-E159-E411-863E-0025905A48F0.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/9C5A16D3-D559-E411-A360-002618943842.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/A8D2F19B-DA59-E411-A44D-002618FDA207.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/C452A5B9-D659-E411-BDDB-0025905A48C0.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/D0B4C29C-DB59-E411-BA75-0025905AA9F0.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/D86ABAEC-DA59-E411-A943-002618943809.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/DE4D1CFE-D859-E411-AF2D-002590596490.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/EADBBEA1-DA59-E411-9A46-0025905A60D2.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValMinBias_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/F0977A93-E359-E411-A0E0-002354EF3BDD.root",
    # ),
    # #### SingleMuPt100
    # fileNames = cms.untracked.vstring (
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValSingleMuPt100/GEN-SIM-DIGI-RECO/PRE_LS172_V15_FastSim-v1/00000/30173E37-DB59-E411-A4C1-0025905B8592.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValSingleMuPt100/GEN-SIM-DIGI-RECO/PRE_LS172_V15_FastSim-v1/00000/6294F742-DA59-E411-9692-0025905A60BE.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValSingleMuPt100/GEN-SIM-DIGI-RECO/PRE_LS172_V15_FastSim-v1/00000/9C6C5A46-DA59-E411-8EB7-0025905A612E.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValSingleMuPt100/GEN-SIM-DIGI-RECO/PRE_LS172_V15_FastSim-v1/00000/B2DA168C-DB59-E411-85CA-0025905A6118.root",
    # ),
    # #### ZMM_13
    # fileNames = cms.untracked.vstring (
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/6841C456-EC59-E411-98E3-002590596468.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/727AA77E-E459-E411-99CE-0025905938A4.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/9AB4E16C-E159-E411-8BC8-002618943843.root",
    #     "/store/relval/CMSSW_7_3_0_pre1/RelValZMM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/B4FDFC56-EC59-E411-A74B-0025905A48E4.root",
    # ),
    #### TtbarRelval_13
    # fileNames = cms.untracked.vstring (
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/2088E3F2-5842-E411-A614-0025905A6080.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/26782EF3-5842-E411-AAD0-0025905B85A2.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/26A7E8F0-5842-E411-A53A-00261894398D.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/32B26189-5D42-E411-97D8-002618943976.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/6AB614F9-5842-E411-9C66-002618943870.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/6C874C8A-5D42-E411-A03D-0026189438B9.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/80FFAD28-5942-E411-8AF6-00261894392F.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/82F1577B-5942-E411-8D8E-002618943986.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/94BD99F2-5842-E411-80AC-003048FFCC2C.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/A6C5A67A-5942-E411-ABCD-00261894397A.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/B8F19FEE-5842-E411-B24C-0026189438E7.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/D249C9F3-5842-E411-B1C8-0025905B858E.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/DCAE702D-5942-E411-86B8-0026189438E0.root',
    #     '/store/relval/CMSSW_7_2_0_pre6/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_PRE_LS172_V12-v1/00000/ECBAC728-5942-E411-AF08-0026189438F8.root',
    # ),
    #### WM_13
    fileNames = cms.untracked.vstring (
        "/store/relval/CMSSW_7_3_0_pre1/RelValWM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/5A2253B3-CE59-E411-BB6C-003048FFD770.root",
        "/store/relval/CMSSW_7_3_0_pre1/RelValWM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/805AA3B4-CE59-E411-A5B1-0025905B85AA.root",
        "/store/relval/CMSSW_7_3_0_pre1/RelValWM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/9202D8F6-D959-E411-B764-0025905B85E8.root",
        "/store/relval/CMSSW_7_3_0_pre1/RelValWM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/92D2E356-D059-E411-8EDD-0025905A6110.root",
        "/store/relval/CMSSW_7_3_0_pre1/RelValWM_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PRE_LS172_V15-v1/00000/E836F7B2-D859-E411-9659-0025905A6118.root",
    ),
    skipEvents = cms.untracked.uint32(skip)
    )

process.output =cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
        'keep *_*_*_L1TEMULATION',
    ),
    fileName = cms.untracked.string('testGlobalMCInputProducer_'+`job`+'.root')
    )
    
process.options = cms.untracked.PSet()

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS1', '')

# Flag to switch between using MC particles and injecting individual particles
useMCtoGT = True

process.uGMTInputProducer = cms.EDProducer("l1t::uGMTInputProducerFromGen",
)



process.load("L1Trigger.L1TGlobalMuon.microgmtemulator_cfi")


process.microGMTEmulator.barrelTFInput = cms.InputTag("uGMTInputProducer", "BarrelTFMuons")
process.microGMTEmulator.overlapTFInput = cms.InputTag("uGMTInputProducer", "OverlapTFMuons")
process.microGMTEmulator.forwardTFInput = cms.InputTag("uGMTInputProducer", "ForwardTFMuons")
process.microGMTEmulator.triggerTowerInput = cms.InputTag("uGMTInputProducer", "TriggerTowerSums")


process.dumpGT = cms.EDAnalyzer("l1t::GtInputDump",
        egInputTag    = cms.InputTag("gtInput"),
        muInputTag    = cms.InputTag("microGMTEmulator"),
        tauInputTag   = cms.InputTag("gtInput"),
        jetInputTag   = cms.InputTag("gtInput"),
        etsumInputTag = cms.InputTag("gtInput"), 
        minBx         = cms.int32(0),
        maxBx         = cms.int32(0)
         )
process.dumpED = cms.EDAnalyzer("EventContentAnalyzer")
process.dumpES = cms.EDAnalyzer("PrintEventSetupContent")

process.mcL1GTinput = cms.EDProducer("l1t::GenToInputProducer",
                                     bxFirst = cms.int32(-2),
                                     bxLast = cms.int32(2),
                     maxMuCand = cms.int32(8),
                     maxJetCand = cms.int32(12),
                     maxEGCand  = cms.int32(12),
                     maxTauCand = cms.int32(8),                                      
                                     jetEtThreshold = cms.double(1),
                                     tauEtThreshold = cms.double(1),
                                     egEtThreshold  = cms.double(1),
                                     muEtThreshold  = cms.double(1),
                     emptyBxTrailer = cms.int32(5),
                     emptyBxEvt = cms.int32(neventsPerJob)
                                     )

process.mcL1GTinput.maxMuCand = cms.int32(8)
process.mcL1GTinput.maxJetCand = cms.int32(12)
process.mcL1GTinput.maxEGCand  = cms.int32(12)
process.mcL1GTinput.maxTauCand = cms.int32(8)

# Fake the input
process.fakeL1GTinput = cms.EDProducer("l1t::FakeInputProducer",

# Note: There is no error checking on these parameters...you are responsible. 
                       egParams = cms.untracked.PSet(
                   egBx    = cms.untracked.vint32(-2, -1,  0,  0,  1,  2),
               egHwPt  = cms.untracked.vint32(10, 20, 30, 61, 40, 50),
               egHwPhi = cms.untracked.vint32(11, 21, 31, 61, 41, 51),
               egHwEta = cms.untracked.vint32(12, 22, 32, 62, 42, 52),
               egIso   = cms.untracked.vint32( 0,  0,  1,  1,  0,  0)
               ),
               
                       muParams = cms.untracked.PSet(
                   muBx    = cms.untracked.vint32(0, -1,  0,  0,  1,  2),
               muHwPt  = cms.untracked.vint32(5, 20, 30, 61, 40, 50),
               muHwPhi = cms.untracked.vint32(11, 21, 31, 61, 41, 51),
               muHwEta = cms.untracked.vint32(12, 22, 32, 62, 42, 52),
               muIso   = cms.untracked.vint32( 0,  0,  1,  1,  0,  0)
               ),

                       tauParams = cms.untracked.PSet(
                   tauBx    = cms.untracked.vint32(),
               tauHwPt  = cms.untracked.vint32(),
               tauHwPhi = cms.untracked.vint32(),
               tauHwEta = cms.untracked.vint32(),
               tauIso   = cms.untracked.vint32()
               ),
               
                       jetParams = cms.untracked.PSet(
                   jetBx    = cms.untracked.vint32(  0,   0,   2,   1,   1,   2),
               jetHwPt  = cms.untracked.vint32(100, 200, 130, 170,  85, 145),
               jetHwPhi = cms.untracked.vint32(  2,  67,  10,   3,  78,  10),
               jetHwEta = cms.untracked.vint32(  1,  19,  11,   0,  17,  11)
               ),
               
                       etsumParams = cms.untracked.PSet(
                   etsumBx    = cms.untracked.vint32( -2, -1,   0,  1,  2),
               etsumHwPt  = cms.untracked.vint32(  2,  1, 204,  3,  4),  
               etsumHwPhi = cms.untracked.vint32(  2,  1,  20,  3,  4)
               )                                                   
                    )

## Load our L1 menu
process.load('L1Trigger.L1TGlobal.StableParametersConfig_cff')

process.load('L1Trigger.L1TGlobal.TriggerMenuXml_cfi')
process.TriggerMenuXml.TriggerMenuLuminosity = 'startup'
#process.TriggerMenuXml.DefXmlFile = 'L1_Example_Menu_2013.xml'
# process.TriggerMenuXml.DefXmlFile = 'L1Menu_Reference_2014.xml'
process.TriggerMenuXml.DefXmlFile = 'L1Menu_Muons2014.xml'

process.load('L1Trigger.L1TGlobal.TriggerMenuConfig_cff')
process.es_prefer_l1GtParameters = cms.ESPrefer('l1t::TriggerMenuXmlProducer','TriggerMenuXml')


process.simL1uGtDigis = cms.EDProducer("l1t::GtProducer",
    #TechnicalTriggersUnprescaled = cms.bool(False),
    ProduceL1GtObjectMapRecord = cms.bool(True),
    AlgorithmTriggersUnmasked = cms.bool(False),
    EmulateBxInEvent = cms.int32(1),
    L1DataBxInEvent = cms.int32(5),
    AlgorithmTriggersUnprescaled = cms.bool(False),
    ProduceL1GtDaqRecord = cms.bool(True),
    GmtInputTag = cms.InputTag("microGMTEmulator"),
    caloInputTag = cms.InputTag("gtInput"),
    AlternativeNrBxBoardDaq = cms.uint32(0),
    #WritePsbL1GtDaqRecord = cms.bool(True),
    BstLengthBytes = cms.int32(-1),
    Verbosity = cms.untracked.int32(0)
)

process.dumpGTRecord = cms.EDAnalyzer("l1t::GtRecordDump",
                egInputTag    = cms.InputTag("gtInput"),
        muInputTag    = cms.InputTag("microGMTEmulator"),
        tauInputTag   = cms.InputTag("gtInput"),
        jetInputTag   = cms.InputTag("gtInput"),
        etsumInputTag = cms.InputTag("gtInput"),
        uGtRecInputTag = cms.InputTag("simL1uGtDigis"),
        uGtAlgInputTag = cms.InputTag("simL1uGtDigis"),
        uGtExtInputTag = cms.InputTag("simL1uGtDigis"),
        bxOffset       = cms.int32(skip),
        minBx          = cms.int32(-2),
        maxBx          = cms.int32(2),
        minBxVec       = cms.int32(0),
        maxBxVec       = cms.int32(0),      
        dumpGTRecord   = cms.bool(False),
        dumpVectors    = cms.bool(True),
        tvFileName     = cms.string( ("TestVector_%03d.txt") % job )
         )



process.load("L1Trigger.GlobalTriggerAnalyzer.l1GtTrigReport_cfi")
process.l1GtTrigReport.L1GtRecordInputTag = "simL1uGtDigis"
process.l1GtTrigReport.PrintVerbosity = 2
process.report = cms.Path(process.l1GtTrigReport)

process.MessageLogger.categories.append("MuConditon")

if useMCtoGT:
    process.gtInput = process.mcL1GTinput.clone()
else:
    process.gtInput = process.fakeL1GTinput.clone()

process.p1 = cms.Path(
    process.uGMTInputProducer
    *process.microGMTEmulator
    *process.gtInput
# #    *process.dumpGT
    *process.simL1uGtDigis
    *process.dumpGTRecord
#    * process.debug
#    *process.dumpED
#    *process.dumpES
    )

process.schedule = cms.Schedule(
    process.p1
    )
#process.schedule.append(process.report)
if rootout:
    process.outpath = cms.EndPath(process.output)
    process.schedule.append(process.outpath)

# Spit out filter efficiency at the end.
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

if dump:
    outfile = open('dump_runGlobalFakeInputProducer_'+`job`+'.py','w')
    print >> outfile,process.dumpPython()
    outfile.close()
