import FWCore.ParameterSet.Config as cms

HLTHiggsBits_WW = cms.EDAnalyzer("HLTHiggsBits",
  ##  hcalDigisLabel = cms.string('hcalTriggerPrimitiveDigis'),
    muon = cms.string('muons'),
  ##  L1GTObjectMapTag = cms.InputTag("l1GtEmulDigis"),
    histName = cms.string(''),
    OutputMEsInRootFile = cms.bool(False),
 ##  histName = cms.string("#outputfile#"),
  ##  calotowers = cms.string('towerMaker'),
   ## L1GTReadoutRecord = cms.InputTag("l1GtEmulDigis"),
    Photon = cms.string('correctedPhotons'),
  ##  genmet = cms.string('genMet'),
    MCTruth = cms.InputTag("genParticles"),
    hltBitNames = cms.vstring('HLT_Mu3','HLT_Mu9','HLT_Mu15','HLT_Ele10_LW_L1R','HLT_Ele10_LW_EleId_L1R','HLT_Ele15_SW_L1R','HLT_Ele15_SW_LooseTrackIso_L1R'),
    hltBitNamesEG = cms.vstring('HLT_Ele10_LW_L1R','HLT_Ele10_LW_EleId_L1R','HLT_Ele15_SW_L1R','HLT_Ele15_SW_LooseTrackIso_L1R','HLT_DoubleEle5_SW_L1R'),
    hltBitNamesMu = cms.vstring('HLT_Mu3','HLT_Mu9','HLT_Mu15','HLT_DoubleMu3'),
    hltBitNamesPh = cms.vstring(''),
    hltBitNamesTau = cms.vstring(''),
  ##  hltLabels  =  cms.vstring(' '),
   ## hltLabelsMu  =  cms.vstring(' '),
   ## hltLabelsEg  =  cms.vstring(' '),
   ## L1ExtraParticleMap = cms.InputTag("l1extraParticleMap"),
   # Electron = cms.string('pixelMatchGsfElectrons'),
    Electron = cms.string('gsfElectrons'),
   ## genjets = cms.string('iterativeCone5GenJets'),
   ## recmet = cms.string('met'),
   # Nchannel = cms.int32(0),
    Nchannel = cms.int32(2), 
   ## recjets = cms.string('iterativeCone5CaloJets'),
     OutputFileName = cms.string(''),
    DQMFolder = cms.untracked.string("HLT/Higgs/HWW"),
  ##  particleMapSource = cms.string('l1extraParticleMap'),
   ## l1extramc = cms.string('l1extraParticles'),
    HLTriggerResults = cms.InputTag("TriggerResults","","HLT"),
    RunParameters = cms.PSet(
        Debug = cms.bool(True),
        Monte = cms.bool(True),
        EtaMax = cms.double(2.5),
        EtaMin = cms.double(-2.5)
    )#,
  ##  ecalDigisLabel = cms.string('ecalTriggerPrimitiveDigis')
)


