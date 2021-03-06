import FWCore.ParameterSet.Config as cms

ecalEndcapStatusFlagsTask = cms.EDAnalyzer("EEStatusFlagsTask",
    prefixME = cms.untracked.string('EcalEndcap'),
    enableCleanup = cms.untracked.bool(False),
    mergeRuns = cms.untracked.bool(False),    
    EcalRawDataCollection = cms.InputTag("ecalEBunpacker")
)

