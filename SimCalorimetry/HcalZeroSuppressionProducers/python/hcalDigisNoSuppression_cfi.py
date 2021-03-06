import FWCore.ParameterSet.Config as cms

simHcalDigis = cms.EDProducer("HcalRealisticZS",
    digiLabel = cms.InputTag("simHcalUnsuppressedDigis"),
    markAndPass = cms.bool(False),
    HBlevel = cms.int32(-999),
    HElevel = cms.int32(-999),
    HOlevel = cms.int32(-999),
    HFlevel = cms.int32(-999),
    useConfigZSvalues = cms.int32(1)
)



