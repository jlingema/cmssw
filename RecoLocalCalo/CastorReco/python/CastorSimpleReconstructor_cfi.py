import FWCore.ParameterSet.Config as cms

castorreco = cms.EDProducer("CastorSimpleReconstructor",
    correctionPhaseNS = cms.double(0.0),
    digiLabel = cms.InputTag("castorDigis"),
    samplesToAdd = cms.int32(2),
    Subdetector = cms.string('CASTOR'),
    firstSample = cms.int32(4),
    correctForPhaseContainment = cms.bool(False),
    correctForTimeslew = cms.bool(False)
)
