import FWCore.ParameterSet.Config as cms

l1tStage1CaloAnalyzer = cms.EDAnalyzer('l1t::Stage2CaloAnalyzer',
    towerToken = cms.InputTag("l1tCaloStage1Digis"), #After Compression
    towerPreCompressionToken = cms.InputTag("l1tCaloStage1Layer1Digis"), #Before Compression
    clusterToken = cms.InputTag("l1tCaloStage1Digis"),
    egToken = cms.InputTag("l1tCaloStage1Digis"),
    tauToken = cms.InputTag("l1tCaloStage1Digis"),
    jetToken = cms.InputTag("l1tCaloStage1Digis"),
    etSumToken = cms.InputTag("l1tCaloStage1Digis")
)
