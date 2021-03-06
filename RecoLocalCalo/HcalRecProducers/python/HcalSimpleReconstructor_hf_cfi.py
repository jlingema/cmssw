import FWCore.ParameterSet.Config as cms

hfreco = cms.EDProducer("HcalSimpleReconstructor",
    correctionPhaseNS = cms.double(0.0),
    digiLabel = cms.InputTag("hcalDigis"),
    samplesToAdd = cms.int32(2),
    Subdetector = cms.string('HF'),
    firstSample = cms.int32(4),
    correctForPhaseContainment = cms.bool(False),
    correctForTimeslew = cms.bool(False),
    dropZSmarkedPassed = cms.bool(True)
)


