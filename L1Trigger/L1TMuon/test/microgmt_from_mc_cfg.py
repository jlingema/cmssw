import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TMuonEmulation")

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
                threshold  = cms.untracked.string('INFO'),
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
      # 'file:/afs/cern.ch/work/j/jlingema/public/omtf_input_test.root'
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/1431747F-84E3-E411-9537-0025905B85D8.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/1892A9F5-41E3-E411-811B-0026189438A7.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/36862409-75E2-E411-ACC6-00261894393F.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/3E6E9C77-84E3-E411-86C7-0025905A4964.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/5CF8490F-75E2-E411-83D2-00261894398C.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/A496F864-84E3-E411-8ECE-003048FFD752.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/B2F7971B-75E2-E411-9262-00261894380D.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/C20BD695-92E3-E411-A02C-002618943914.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/C80A8C08-75E2-E411-8FDC-002618943810.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValTTbar_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/F604DB80-84E3-E411-B32F-0025905A60EE.root',
      )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100))

###PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtendedPostLS1Reco_cff')
process.load('Configuration.Geometry.GeometryExtendedPostLS1_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# generate inputs that are not provided by emulators
process.uGMTInputProducer = cms.EDProducer("l1t::uGMTInputProducerFromGen",
)

process.load("L1Trigger.L1TMuon.microgmtemulator_cfi")


process.L1TMuonSeq = cms.Sequence( process.uGMTInputProducer +
                                   process.microGMTEmulator)

process.L1TMuonPath = cms.Path(process.L1TMuonSeq)


process.out = cms.OutputModule("PoolOutputModule",
  outputCommands = cms.untracked.vstring(
              'drop *',
              'keep *_*_*_L1TMuonEmulation'),
   fileName = cms.untracked.string("ugmt_ttbar_small_sample.root"),
)

process.output_step = cms.EndPath(process.out)

process.schedule = cms.Schedule(process.L1TMuonPath)
process.schedule.extend([process.output_step])
