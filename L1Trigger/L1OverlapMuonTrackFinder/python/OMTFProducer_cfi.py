import FWCore.ParameterSet.Config as cms

###OMTF emulator configuration
omtfEmulator = cms.EDProducer("OMTFProducer",
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
                                  configXMLFile = cms.FileInPath("L1Trigger/L1OverlapMuonTrackFinder/data/hwToLogicLayer_750.xml"),
                                  patternsXMLFiles = cms.FileInPath("L1Trigger/L1OverlapMuonTrackFinder/data/Patterns_ipt4_31_750.xml"),
                              )
)


