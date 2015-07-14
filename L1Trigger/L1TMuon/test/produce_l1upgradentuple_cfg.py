import FWCore.ParameterSet.Config as cms
process = cms.Process("L1TMuonEmulation")
import os
import sys
import commands

process.load("FWCore.MessageLogger.MessageLogger_cfi")



VERBOSE = False
SAMPLE = "zmumu"#"minbias"
EDM_OUT = False
# min bias: 23635 => 3477 passed L1TMuonFilter (~6.7%)
NEVENTS = 10000
if VERBOSE:
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

if not VERBOSE:
    process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(5000)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

fnames = ['/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/16BF2D14-83E3-E411-B212-003048FFD756.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/26833213-83E3-E411-9238-0025905B8590.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/5E967412-83E3-E411-9DA0-003048FFD756.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/686FB705-83E3-E411-A8FC-003048FF9AC6.root',
      '/store/relval/CMSSW_7_5_0_pre1/RelValSingleMuPt10_UP15/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V7-v1/00000/8E6F7913-83E3-E411-B72F-0025905A48BB.root']
if SAMPLE == "zmumu":
  fnames = [
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/0023D81B-2980-E311-85A1-001E67398C0F.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/248FB042-3080-E311-A346-001E67397D00.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/4C39433F-2980-E311-9F07-001E67396A13.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/B20C0E40-2980-E311-A54A-001E67398C5A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/D834741D-2980-E311-87FF-002590200AD8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/E61A11C8-2980-E311-BBDC-002590A80E1E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/10000/F2CB6E35-2980-E311-B29B-002590200B10.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/00A82E15-B97F-E311-9215-002590200840.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/023A80BF-BA7F-E311-92C5-002481E15258.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/0659EF2E-C37F-E311-ADC9-001E67396DCE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/08B32FDD-B97F-E311-9FEC-001E67398D72.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/0A789A0D-C17F-E311-A771-001E67398223.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/0CFAAFBA-BB7F-E311-ACB7-002481E14F86.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/0CFB4BCE-BA7F-E311-839B-002590A3C95E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/0E1032FE-BA7F-E311-877F-002590A3711C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/0E1B7E4D-BA7F-E311-A9D4-001E67396653.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/0E7C216C-BA7F-E311-9308-001E67397D91.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/0EC31E1F-B97F-E311-9DBD-0025B3E06394.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/103789F2-B97F-E311-BB97-001E67396CFC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/106565F8-B87F-E311-A51B-001E673969FA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/107D42C2-B87F-E311-A6B8-00259020083C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/10A5D9E9-B87F-E311-A3B3-001E67396A09.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/16D225D7-BA7F-E311-AF2A-001E67396DB5.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/18BC9AE4-BA7F-E311-ADEA-0025902008B8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/18DAFE21-BA7F-E311-B99C-002481E14F38.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/1A969409-BB7F-E311-8072-002590200868.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/2045542F-C17F-E311-ACA6-002590A887EE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/209CBD92-BB7F-E311-AA3C-001E673972DD.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/20CF6C6E-BC7F-E311-8D96-002590A887F0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/26856501-BB7F-E311-B0EE-002590A37114.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/26AAB842-BB7F-E311-9631-002590200ABC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/26B0031F-BB7F-E311-AA29-0025902009C8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/26C47C54-BC7F-E311-88EB-0025B3E05C74.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/28623FC7-B97F-E311-86F1-002590A3707C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/286D0BD7-B97F-E311-8838-001E67398633.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/2A5F89C0-B87F-E311-B4FD-002481E14E82.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/2A8002C6-B97F-E311-8C8B-002590A88810.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/2C34F4AE-BA7F-E311-9A0E-001E67397003.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/2C5BE1A9-BA7F-E311-B3A3-001E67396874.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/2E1833F3-BA7F-E311-AA23-002481E14F20.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/2E9BD92F-C37F-E311-BEE3-001E67397314.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/3228A14D-BB7F-E311-B2F9-001E67398683.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/32E727B3-BA7F-E311-87BB-001E67397CAB.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/32EE0836-C37F-E311-89D8-001E67396E32.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/340BFF39-C37F-E311-A411-001E67397D73.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/34178356-BC7F-E311-9E39-0025902009A4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/34AF1811-BB7F-E311-94EC-002590200B68.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/363AF742-B97F-E311-884B-001E67398142.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/3A101B56-BC7F-E311-B7A6-0025B3E063E8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/3A697526-BB7F-E311-9EE3-001E67398D72.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/3A85FAA3-BA7F-E311-BD51-002590A80DD4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/3EF13D5D-BC7F-E311-88A1-001E67397AA3.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/3EFFE5D5-BA7F-E311-BAB6-001E67398DE5.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4037A8E9-B97F-E311-ACC6-002590A83190.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/420DB7FE-B87F-E311-9BCF-002590200B60.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/42F20DF0-B87F-E311-ADC1-0025902009B0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/447D724E-B97F-E311-B81E-0025B3E05BC4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/44DDDA11-BA7F-E311-A6E4-0025B3E06612.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/46A06CBF-BA7F-E311-B49F-001E67398043.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/483B26F7-B87F-E311-AC1B-00259020083C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/48AF363D-BC7F-E311-9E87-001E6739675C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4A6D24D8-B97F-E311-8E5A-0025B3E064F8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4A97C3AA-B97F-E311-BF97-002590A50046.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4AF3D8D3-BA7F-E311-8DF5-0025902009BC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4C248C88-BB7F-E311-8F45-001E67396617.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4C85013E-BC7F-E311-AEAA-001E67397F71.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4CABD918-BB7F-E311-B714-002590A36FA2.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4CB5A2D4-B87F-E311-BA85-00259021A43E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/4E484614-B97F-E311-A55C-002590A371AC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/501E4AB4-BA7F-E311-8777-001E673973EB.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/5084ED64-BA7F-E311-A3C0-001E6739732D.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/50950E24-BA7F-E311-8668-001E67396568.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/52E00139-BA7F-E311-B61D-001E67397F2B.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/56E1272C-BA7F-E311-AD3B-001E673965FE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/58C368E9-B97F-E311-B2CD-001E67398110.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/5A5ECD3A-BC7F-E311-9134-001E67396D6F.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/5C407DCC-B97F-E311-8CFF-002590A88802.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/5C837323-BB7F-E311-8B0D-002590200A80.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/5C9B5C19-BA7F-E311-9154-001E67397053.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/5CE9298F-BA7F-E311-8178-001E67396FE5.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/5CF93A1D-BB7F-E311-9E7B-001E673966B2.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/5E87F98B-BC7F-E311-B7DC-001E67396D6F.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/604BDA06-B97F-E311-96E3-002590200834.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/665F6150-BA7F-E311-8A75-0025B3E06508.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/66B1F80B-BB7F-E311-81B6-002590A4C6BE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/66CFCA21-BA7F-E311-BE80-002590A88800.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/68509E2B-C37F-E311-8007-0025B3E05DBE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/68699051-BA7F-E311-AA92-002590A370DC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/68902840-BC7F-E311-B24A-001E67398052.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/6A379EDE-B87F-E311-97BB-002590200A58.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/6A90CFCC-B87F-E311-A417-001E67396E64.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/6AD557D7-B87F-E311-8AD8-002590A4C6AE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/6C910135-C17F-E311-8808-002590200814.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/6CFF349F-BA7F-E311-AF8D-002590200898.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/6EB8E22D-C37F-E311-96EC-D8D385FF4A72.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7055763E-BC7F-E311-BF59-002590200AC0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/70C511FD-BA7F-E311-8806-001E67398BE7.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/70FB3327-BB7F-E311-A12E-001E67397B11.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/725C82A4-B97F-E311-BA81-001E67396C9D.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7262E1C0-BA7F-E311-9B2F-002590A83190.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/72F3BC2D-C37F-E311-9CBE-0025B3E063EA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7426F4CF-B97F-E311-9BB8-002590200898.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/748331F8-BA7F-E311-951A-0025B3E064F8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/74B4C9AB-BB7F-E311-97DD-001E67398E62.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/74E53D3E-BB7F-E311-BBD6-0025B3E06584.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/74FDF3E0-B87F-E311-8083-0025902008C4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/768A8AB4-BA7F-E311-B4B3-002590200A00.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/76BC301F-B97F-E311-A39D-0025902008C4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/78987330-B97F-E311-B814-001E67398CE1.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7A69AF48-BA7F-E311-835E-001E67396685.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7A7879A2-B97F-E311-921E-002590200850.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7C00E0C5-B97F-E311-BE67-001E6739751C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7E90D23A-BB7F-E311-9DB5-001E67396AC2.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7EBB4056-BB7F-E311-A566-002590A3C978.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/7ED908F0-B87F-E311-940E-001E67396A63.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/80185183-BA7F-E311-8825-002590A887AC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/8075E189-BC7F-E311-8831-002590A8881E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/80F0516E-BA7F-E311-9E9B-002590200A80.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/8203AA4C-BA7F-E311-B1F6-001E67396E1E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/824C3A4F-BB7F-E311-81E0-001E67397751.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/828717B4-BB7F-E311-8F31-001E6739811A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/84FB22AC-BA7F-E311-B2BF-001E67398C0F.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/901C0BDF-B87F-E311-93EA-002590200B34.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/902325EE-BA7F-E311-97BC-001E67397ADA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/9076B48D-BB7F-E311-83DA-001E67398534.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/9211141B-B97F-E311-AC4C-001E67398C5A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/92A80BEE-BB7F-E311-BF16-001E67398C1E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/92C4513B-BC7F-E311-A04C-001E67398E62.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/963F7458-BC7F-E311-A5FD-001E67397B25.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/96631F62-BB7F-E311-8AA7-001E6739815B.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/966CD25C-B97F-E311-82B2-001E673985E3.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/96C0E42D-C37F-E311-8AE6-001E67398390.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/96E483AA-BA7F-E311-AF77-001E67397053.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/96F55C30-C37F-E311-B272-002590200894.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/9843DB29-BA7F-E311-951A-001E67398C05.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/985C8AAC-BA7F-E311-9608-001E6739815B.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/98CD3541-C37F-E311-B1D8-002590A887F2.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/9A628E92-B97F-E311-86BA-001E673966B2.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/9A8C50C5-B87F-E311-9879-001E67398C5A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/9AC5F6C0-BA7F-E311-834B-0025902009B0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/9CBB892D-C37F-E311-974B-002481E14FCA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/9EEAA2FB-B97F-E311-B57F-001E67396BAD.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/A62DA2D4-B97F-E311-ACE9-002590A88800.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/A8AAAAC5-B97F-E311-A2D3-0025B3E06548.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/AA52A62F-C37F-E311-8978-001E67397CAB.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/AA8D84CB-B87F-E311-B679-002590A4FFE8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/ACC6E952-B97F-E311-981B-001E67396793.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/ACC76B14-BB7F-E311-A7AC-001E673983A4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/ACF636AD-B97F-E311-B1A0-002590200834.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B0485079-BA7F-E311-BE71-00259020084C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B063A387-BB7F-E311-8A77-0025902008E4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B09FC32D-C37F-E311-9A85-001E673973E1.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B4D285CF-B97F-E311-98B0-001E67396892.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B4D955A2-BA7F-E311-9C89-002590200970.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B4E24EBE-BA7F-E311-8CCD-0025902009E4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B64647D4-B97F-E311-B273-002590A88800.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B6770BF9-B87F-E311-8F59-002590A83174.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B6E81622-B97F-E311-A3CD-002590A371C4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B82CE52A-BA7F-E311-A215-001E6739811A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/B854ECD7-BA7F-E311-A774-001E673973E1.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/BAACA52F-C37F-E311-99A5-002590200B78.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/BC4B99CD-B87F-E311-A09D-00259020093C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/BC9C031A-BB7F-E311-B87C-001E6739850C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/BE3D8B49-BB7F-E311-AFFF-0025902008C8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/C0246B82-B97F-E311-9F3C-0025B3E05CDA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/C4794C90-BC7F-E311-983F-0025902009C0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/C4EDD95C-BC7F-E311-9E91-002590A8877A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/C69CE2DD-B87F-E311-BAC3-001E67397396.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/C6ADB939-BB7F-E311-825C-001E67398142.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/CC206100-BB7F-E311-A825-002590200810.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/D06C5662-B97F-E311-AB4D-002481E14C88.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/D20AE7DF-BA7F-E311-9B61-001E67396DCE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/D4E75082-BB7F-E311-889F-002590A887EE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/D60DA570-BA7F-E311-B1B1-002590200B50.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/D6888D3D-BC7F-E311-A1E3-002481E14E56.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/D88BF712-B97F-E311-BF34-002481E1501E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/D8E99B81-BB7F-E311-B64E-001E67397E90.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/DAB8962B-C37F-E311-9C60-001E67397747.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/DCDDF8A0-BA7F-E311-874A-001E67398228.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E033D851-BC7F-E311-B203-001E67397698.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E0820ABE-BA7F-E311-AE16-002590A4FFB8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E22F85E0-B97F-E311-818F-001E67396F9A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E2A3E677-BB7F-E311-8F3F-001E6739801B.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E2D30DFB-B87F-E311-B95E-001E67398C5A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E44C21E4-B97F-E311-B88C-002590200948.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E4AA0538-BA7F-E311-9053-0025B3E0645C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E4BD2DC5-BA7F-E311-BFDD-0025B31E330A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E4BEC430-C17F-E311-9B14-0025B3E05D3A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E617B121-BB7F-E311-96B8-D8D385FF4B32.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E6D2BE3C-C37F-E311-A1C7-002481E14F1E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/E8B2DCA8-BB7F-E311-B867-002590A8880A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/EA9F67B2-BA7F-E311-A1F4-001E673974EA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/EAA27697-B97F-E311-B6A1-002590200B50.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/EC5F744D-BA7F-E311-BBDE-0025902008AC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/ECC2833D-BC7F-E311-981B-002590200914.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/F03498DD-B97F-E311-A94D-001E67397D00.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/F08CC44E-BA7F-E311-9B88-001E673982E6.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/F21490A3-BA7F-E311-B6F8-002590A80E08.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/F2281651-B97F-E311-8F5D-001E673972F6.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/F24C59BB-BA7F-E311-A4D5-001E67397EEA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/F2E8BB26-BA7F-E311-B3B0-001E67396E0A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/F4ED9837-BB7F-E311-B181-002590A88804.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/110000/F81CA778-BB7F-E311-9FD8-002590A4C69A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/06ECCAB8-A077-E311-A9C7-001E67396FD1.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/10EBEEC2-A077-E311-97DE-001E67396A1D.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/14EB6FB1-A077-E311-A149-0025B3E066A4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/1807F2A5-A077-E311-BF01-001E673967C0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/1AC644B0-A077-E311-B378-002590A3C954.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/1E2ACDBF-A077-E311-97B8-001E67396DCE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/20C6F8A5-A077-E311-B926-0025B3E05E1C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/32368DD0-A077-E311-9508-002590A8880E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/360966BC-A077-E311-8435-001E67397D91.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/384A16C4-A077-E311-855C-001E6739801B.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/3E1737D0-A077-E311-A958-002590200B00.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/40BB82C5-A077-E311-88FF-001E6739707B.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/44A0C2C9-A077-E311-A506-002590200A38.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/4C3E52B7-A077-E311-8D0E-001E67398C1E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/5C8775C7-A077-E311-9ABC-002590A831B4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/5E033FB2-A077-E311-BAE4-002590A8882C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/621026AB-A077-E311-B6B1-001E67397751.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/66A7DFCE-A077-E311-B9FC-0025B3E05BA8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/72E2140D-A077-E311-A4ED-002590A3C954.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/748DE1CE-A077-E311-87F5-0025B3E06508.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/78BA2EBF-A077-E311-9123-0025B3E06508.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/7A079FAF-A077-E311-BC03-001E6739672F.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/8CAAFEA9-A077-E311-8C56-001E6739720B.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/8CB8E1A9-A077-E311-917A-0025B3E06658.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/92E92EA8-A077-E311-8799-001E67398CA0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/989A8BBC-A077-E311-93F5-002590A3CA14.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/A0EC91B1-A077-E311-9FA6-001E6739732D.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/A4DBFAC8-A077-E311-90DE-001E67396E05.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/AA02A3A8-A077-E311-8DFA-0025902008A8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/AA3BB1A7-A077-E311-8B23-001E67396ACC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/AAC7DDA3-A077-E311-A7EC-001E6739850C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/B44C9FA8-A077-E311-BB83-002590A831B4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/B61E1FCD-A077-E311-8B65-001E673974EA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/C0F504E8-A077-E311-B66D-0025B3E066A4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/CCB0BFA7-A077-E311-BD7B-001E6739720B.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/D4E361B6-A077-E311-8793-0025B3E0638E.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/D8BD84AB-A077-E311-8213-001E67398390.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/EE1065BC-A077-E311-820D-001E67397D91.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/F2269AA4-A077-E311-8D44-001E673974EA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/F419CDC2-A077-E311-9BE0-001E67398A43.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/DYToMuMu_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/20000/F4E83EA5-A077-E311-80E5-001E67396FCC.root'
       ]
elif SAMPLE == "minbias":
  fnames = [
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/00276D94-AA88-E311-9C90-0025905A6060.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/004F8058-6F88-E311-B971-0025905A6094.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/005C8F98-C288-E311-ADF1-0026189438BD.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/006A1FB8-7D88-E311-B61B-0025905A60A0.root',
       # 'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/00980346-6A88-E311-BAA2-00261894392F.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/00B4AAB7-C188-E311-B5F5-002354EF3BD2.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/00F7037F-6D88-E311-8F23-003048678E92.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/026C4246-7388-E311-93EF-00261894397F.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/02738D73-5D88-E311-BDD4-0025905A6082.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/028EDF3E-7088-E311-A8C9-0025905A48F0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/02BC960C-BD88-E311-9E47-0025905A6126.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/02BD6472-B588-E311-A9DF-0025905A60CE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/02F196B2-9B88-E311-814B-00261894382A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/02FF9851-6A88-E311-A5B3-00261894396F.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0424D9A3-6588-E311-8197-0026189438F4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/042D2EE1-8D88-E311-88EB-0025905A48BC.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/04A2F676-B088-E311-B725-0025905A6110.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/04E85E91-8388-E311-A7A6-003048679214.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0636FCC9-6788-E311-ADE1-002354EF3BDD.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/063AF50B-BA88-E311-9485-003048FFD75C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/064247FB-8988-E311-8100-003048679266.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0675622E-A488-E311-AF0A-0018F3D096BE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/06DB3351-9F88-E311-9C8F-0025905A60A8.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/06DE59C3-BD88-E311-ACBC-0025905A60E0.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/083A1BFE-7C88-E311-9452-0025905938B4.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0882E85A-5688-E311-8403-00248C55CC9D.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/08A5F044-AA88-E311-8F15-001BFCDBD1BA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/08B38386-7788-E311-9FF8-0026189438AA.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/08CB991A-9288-E311-8AE6-003048678B1A.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0A3C1165-6288-E311-83C5-0025905A60EE.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0A7C4811-A888-E311-B4A6-00261894387C.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0A84E848-7388-E311-A74D-003048678F62.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0AA58FB5-8E88-E311-9D9F-0025905A6076.root',
       'root://xrootd.unl.edu//store/mc/Fall13dr/Neutrino_Pt-2to20_gun/GEN-SIM-RAW/tsg_PU20bx25_POSTLS162_V2-v1/00000/0ABE3DE4-BB88-E311-8AE1-002618943921.root'
  ]

process.source = cms.Source(
    'PoolSource',
    # MinBias test:
    # fileNames = cms.untracked.vstring()
    # fileNames = cms.untracked.vstring()

    fileNames = cms.untracked.vstring(
      fnames
      )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(NEVENTS))

###PostLS1 geometry used
process.load('Configuration.Geometry.GeometryExtendedPostLS1Reco_cff')
process.load('Configuration.Geometry.GeometryExtendedPostLS1_cff')
############################
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

path = os.environ['CMSSW_BASE']+"/src/L1Trigger/L1OverlapMuonTrackFinder/data/"

process.load('L1Trigger.L1EndcapMuonTrackFinder.L1TMuonTriggerPrimitiveProducer_cfi')
if SAMPLE == "zmumu" or SAMPLE == "minbias":
  process.L1TMuonTriggerPrimitives.CSC.src = cms.InputTag('csctfDigis')
  process.L1TMuonTriggerPrimitives.DT.src = cms.InputTag('dttfDigis')
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

process.L1MuonFilter = cms.EDFilter("SelectL1Muons",)

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
    fileName = cms.string('/afs/cern.ch/work/j/jlingema/private/l1ntuples_upgrade/l1ntuple_{sample}.root'.format(sample=SAMPLE))
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

if SAMPLE == "minbias":
  process.L1TMuonSeq = cms.Sequence(
      process.gtDigis
      +process.gtEvmDigis
      # +process.gctDigis
      +process.dttfDigis
      +process.csctfDigis
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
      *process.L1MuonFilter
      *process.l1NtupleProducer
      *process.l1MuonUpgradeTreeProducer
      )
else:
  process.L1TMuonSeq = cms.Sequence(
      process.gtDigis
      +process.gtEvmDigis
      # +process.gctDigis
      +process.dttfDigis
      +process.csctfDigis
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
      *process.l1NtupleProducer
      *process.l1MuonUpgradeTreeProducer
      )

process.L1TMuonPath = cms.Path(process.L1TMuonSeq)

process.out = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(
              'drop *',
              'keep *_*_*_L1TMuonEmulation'),
   fileName = cms.untracked.string("l1tmuon_test.root"),
)


process.schedule = cms.Schedule(process.L1TMuonPath)
if EDM_OUT:
  process.output_step = cms.EndPath(process.out)
  process.schedule.extend([process.output_step])
