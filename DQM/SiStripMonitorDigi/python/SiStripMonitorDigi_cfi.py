import FWCore.ParameterSet.Config as cms

# SiStripMonitorDigi
SiStripMonitorDigi = cms.EDAnalyzer("SiStripMonitorDigi",
                                  
    # add digi producers same way as Domenico in SiStripClusterizer
   
    DigiProducersList = cms.VInputTag(
      cms.InputTag('siStripDigis','ZeroSuppressed'),
      cms.InputTag('siStripZeroSuppression','VirginRaw'),
      cms.InputTag('siStripZeroSuppression','ProcessedRaw'),
      cms.InputTag('siStripZeroSuppression','ScopeMode')
    ),

    TH1ADCsCoolestStrip = cms.PSet(
       Nbinx = cms.int32(60),
       xmin = cms.double(-0.5),
       xmax = cms.double(299.5),
       layerswitchon = cms.bool(False),
       moduleswitchon = cms.bool(True)
    ),
    TH1ADCsHottestStrip = cms.PSet(
        Nbinx = cms.int32(60),
        xmin = cms.double(-0.5),
        xmax = cms.double(299.5),
        layerswitchon = cms.bool(False),
        moduleswitchon = cms.bool(True)
    ),
    TH1DigiADCs = cms.PSet(
        Nbinx = cms.int32(64),
        xmin = cms.double(-0.5),
        xmax = cms.double(255.5),        
        layerswitchon = cms.bool(True),
        moduleswitchon = cms.bool(True)
    ),
    TH1NumberOfDigis = cms.PSet(
        Nbinx = cms.int32(50),
        xmin = cms.double(-0.5),
        xmax = cms.double(999.5),
        layerswitchon = cms.bool(True),
        moduleswitchon = cms.bool(True)
    ),
    TH1NumberOfDigisPerStrip = cms.PSet(
        Nbinx = cms.int32(768),
        xmin = cms.double(-0.5),
        xmax = cms.double(767.5),
        moduleswitchon = cms.bool(False)
    ),                        
    TH1StripOccupancy = cms.PSet(
        Nbinx = cms.int32(51),
        xmin = cms.double(-0.01),
        xmax = cms.double(1.01),
        layerswitchon = cms.bool(True),        
        moduleswitchon = cms.bool(True)
    ),
    TProfNumberOfDigi = cms.PSet(
        Nbinx = cms.int32(100),
        xmin = cms.double(-0.5),
        xmax = cms.double(499.5),
        layerswitchon = cms.bool(False),        
        moduleswitchon = cms.bool(False)        
    ),
    TProfDigiADC = cms.PSet(
        Nbinx = cms.int32(100),
        xmin = cms.double(0.0),
        xmax = cms.double(499.5),
        layerswitchon = cms.bool(False),
        moduleswitchon = cms.bool(False)        
    ),

    TProfTotalNumberOfDigis = cms.PSet(
        Nbins = cms.int32(360),
        xmin = cms.double(0.0),
        xmax = cms.double(1.0*60*60),
        ymin = cms.double(0.0),
        ymax = cms.double(0.0),
        subdetswitchon = cms.bool(False)
    ),

    TkHistoMap_On = cms.bool(True),

    CreateTrendMEs = cms.bool(False),
    Trending = cms.PSet(
        Nbins = cms.int32(600),
        xmin = cms.double(0.0),
        xmax = cms.double(1.0*60*60),        
        ymin = cms.double(0.0),
        ymax = cms.double(10000.0)        
    ),


    TProfDigiApvCycle = cms.PSet(
        Nbins = cms.int32(70),
        xmin = cms.double(-0.5),
        xmax = cms.double(69.5),
        Nbinsy = cms.int32(200),
        ymin = cms.double(0.0),
        ymax = cms.double(0.0),
        subdetswitchon = cms.bool(False)
        ),

    TH2DigiApvCycle = cms.PSet(
        Nbins = cms.int32(70),
        xmin = cms.double(-0.5),
        xmax = cms.double(69.5),
        Nbinsy = cms.int32(200),
        ymin = cms.double(0.0),
        yfactor = cms.double(0.2),
        subdetswitchon = cms.bool(False)
    ),

    TProfTotalNumberOfDigisVsLS = cms.PSet(
        subdetswitchon           = cms.bool(False)                
    ),
                                    
    TotalNumberOfDigisFailure = cms.PSet(
        subdetswitchon        = cms.bool(False),
        Nbins = cms.int32(6),
        xmin = cms.double(0.5),
        xmax = cms.double(6.5),
        Nbinsy = cms.int32(1),
        ymin = cms.double(0.5),
        ymax = cms.double(1.5)
    ),

    xLumiProf = cms.int32(5),

    Mod_On = cms.bool(True),

    HistoryProducer = cms.InputTag("consecutiveHEs"),
    ApvPhaseProducer = cms.InputTag("APVPhases"),

    UseDCSFiltering = cms.bool(True),
    topDir = cms.string('SiStrip'),                                    
                                    
    # rest of parameters
    SelectAllDetectors = cms.bool(False),
    ShowMechanicalStructureView = cms.bool(True),
    ShowReadoutView = cms.bool(False),
    ShowControlView = cms.bool(False),                                  
    CalculateStripOccupancy = cms.bool(False),                                  
    ResetMEsEachRun = cms.bool(False),
    # by default do not write out any file with histograms
    # can overwrite this in .cfg file with: replace SiStripMonitorDigi.OutputMEsInRootFile = true
    OutputMEsInRootFile = cms.bool(False),
    OutputFileName = cms.string('test_digi.root')
)
