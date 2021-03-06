import FWCore.ParameterSet.Config as cms

from RecoEcal.EgammaClusterProducers.ecalRecHitFlags_cfi import *
from RecoEcal.EgammaClusterProducers.ecalSeverityLevelAlgos_cfi import *
from RecoEcal.EgammaClusterProducers.ecalSeverityLevelFlags_cfi import *

isolationSumsCalculator = cms.PSet(
    #required inputs
    ComponentName = cms.string('isolationSumsCalculator'),
    #What collection of photons do I run on?
    #What labels do I use for my products?
    #What rechit collection do I use for ECAL iso?                          
    barrelEcalRecHitProducer = cms.string('ecalRecHit'),
    barrelEcalRecHitCollection = cms.string('EcalRecHitsEB'),
    endcapEcalRecHitProducer = cms.string('ecalRecHit'),
    endcapEcalRecHitCollection = cms.string('EcalRecHitsEE'),
    #What tower collection do I use for HCAL iso?
    HcalRecHitProducer = cms.string('towerMaker'),
    HcalRecHitCollection = cms.string(''),
    # Photon will be marked as being near phi module boundary if
    #  it is closer than this.  Currently half a crystal.
    #  1 Ecal Crystal = 0.0174 radians = 1 degree
    modulePhiBoundary =   cms.double(0.0087),
    # Photon will be marked as being near an eta boundary if
    #  it is between the 0th and 1st element, or the 2nd and 3rd, or the 4th and 5th...
    moduleEtaBoundary = cms.vdouble(0.0, 0.02, 0.43, 0.46, 0.78, 0.81, 1.13, 1.15, 1.45, 1.58),
    #What collection of tracks do I use for Track Isolation?
    trackProducer = cms.InputTag("generalTracks"),
    #use beam spot for track isolation
    beamSpotProducer = cms.InputTag("offlineBeamSpot"),
    #switches, turn on quality cuts for various quantities.
    vetoClustered  = cms.bool(False),  #will remove clustered rechits from ecal iso sum
    useNumCrystals = cms.bool(True),  #will define the veto region by number of crystals in stead of geometrically
    #configuration of parameters for isolation
#### BARREL
    #tracks
    isolationtrackThresholdA_Barrel = cms.double(0.0),
    TrackConeOuterRadiusA_Barrel    = cms.double(0.4),
    TrackConeInnerRadiusA_Barrel    = cms.double(0.04),
    isolationtrackEtaSliceA_Barrel  = cms.double(0.015),
    longImpactParameterA_Barrel     = cms.double(0.2),
    transImpactParameterA_Barrel    = cms.double(0.1),
#
    isolationtrackThresholdB_Barrel = cms.double(0.0),
    TrackConeOuterRadiusB_Barrel    = cms.double(0.3),
    TrackConeInnerRadiusB_Barrel    = cms.double(0.04),
    isolationtrackEtaSliceB_Barrel  = cms.double(0.015),
    longImpactParameterB_Barrel     = cms.double(0.2),
    transImpactParameterB_Barrel    = cms.double(0.1),
    #Ecal rechits 
    EcalRecHitInnerRadiusA_Barrel   = cms.double(3.5),
    EcalRecHitOuterRadiusA_Barrel   = cms.double(0.4),
    EcalRecHitEtaSliceA_Barrel      = cms.double(2.5),
    EcalRecHitThreshEA_Barrel       = cms.double(0.08),
    EcalRecHitThreshEtA_Barrel      = cms.double(0.0),
#
    EcalRecHitInnerRadiusB_Barrel   = cms.double(3.5),
    EcalRecHitOuterRadiusB_Barrel   = cms.double(0.3),
    EcalRecHitEtaSliceB_Barrel      = cms.double(2.5),
    EcalRecHitThreshEB_Barrel       = cms.double(0.08),
    EcalRecHitThreshEtB_Barrel      = cms.double(0.0),
    #Hcal towers
    HcalTowerInnerRadiusA_Barrel    = cms.double(0.15),
    HcalTowerOuterRadiusA_Barrel    = cms.double(0.4),
    HcalTowerThreshEA_Barrel        = cms.double(0.0),
    HcalDepth1TowerInnerRadiusA_Barrel = cms.double(0.15),
    HcalDepth1TowerOuterRadiusA_Barrel = cms.double(0.4),
    HcalDepth1TowerThreshEA_Barrel     = cms.double(0.0),
    HcalDepth2TowerInnerRadiusA_Barrel = cms.double(0.15),
    HcalDepth2TowerOuterRadiusA_Barrel = cms.double(0.4),
    HcalDepth2TowerThreshEA_Barrel     = cms.double(0.0),
#
    HcalTowerInnerRadiusB_Barrel       = cms.double(0.15),
    HcalTowerOuterRadiusB_Barrel       = cms.double(0.3),
    HcalTowerThreshEB_Barrel           = cms.double(0.0),
    HcalDepth1TowerInnerRadiusB_Barrel = cms.double(0.15),
    HcalDepth1TowerOuterRadiusB_Barrel = cms.double(0.3),
    HcalDepth1TowerThreshEB_Barrel     = cms.double(0.0),
    HcalDepth2TowerInnerRadiusB_Barrel = cms.double(0.15),
    HcalDepth2TowerOuterRadiusB_Barrel = cms.double(0.3),
    HcalDepth2TowerThreshEB_Barrel     = cms.double(0.0),

#### ENDCAP
    #tracks
    isolationtrackThresholdA_Endcap  = cms.double(0.0),
    TrackConeOuterRadiusA_Endcap     = cms.double(0.4),
    TrackConeInnerRadiusA_Endcap     = cms.double(0.04),
    isolationtrackEtaSliceA_Endcap   = cms.double(0.015),
    longImpactParameterA_Endcap      = cms.double(0.2),
    transImpactParameterA_Endcap     = cms.double(0.1),
##
    isolationtrackThresholdB_Endcap  = cms.double(0.0),
    TrackConeOuterRadiusB_Endcap     = cms.double(0.3),
    TrackConeInnerRadiusB_Endcap     = cms.double(0.04),
    isolationtrackEtaSliceB_Endcap   = cms.double(0.015),
    longImpactParameterB_Endcap      = cms.double(0.2),
    transImpactParameterB_Endcap     = cms.double(0.1),
    #Ecal rechits 
    EcalRecHitInnerRadiusA_Endcap    = cms.double(3.5),
    EcalRecHitOuterRadiusA_Endcap    = cms.double(0.4),
    EcalRecHitEtaSliceA_Endcap       = cms.double(2.5),
    EcalRecHitThreshEA_Endcap        = cms.double(0.0),
    EcalRecHitThreshEtA_Endcap       = cms.double(0.1),
#
    EcalRecHitInnerRadiusB_Endcap    = cms.double(3.5),
    EcalRecHitOuterRadiusB_Endcap    = cms.double(0.3),
    EcalRecHitEtaSliceB_Endcap       = cms.double(2.5),
    EcalRecHitThreshEB_Endcap        = cms.double(0.0),
    EcalRecHitThreshEtB_Endcap       = cms.double(0.1),

    severityLevelCut = cms.int32(4),
    severityRecHitThreshold = cms.double(5.0),
    spikeIdString = cms.string('kSwissCrossBordersIncluded'),
    spikeIdThreshold = cms.double(0.95),

    #Hcal towers
    HcalTowerInnerRadiusA_Endcap       = cms.double(0.15),
    HcalTowerOuterRadiusA_Endcap       = cms.double(0.4),
    HcalTowerThreshEA_Endcap           = cms.double(0.0),
    HcalDepth1TowerInnerRadiusA_Endcap = cms.double(0.15),
    HcalDepth1TowerOuterRadiusA_Endcap = cms.double(0.4),
    HcalDepth1TowerThreshEA_Endcap     = cms.double(0.0),
    HcalDepth2TowerInnerRadiusA_Endcap = cms.double(0.15),
    HcalDepth2TowerOuterRadiusA_Endcap = cms.double(0.4),
    HcalDepth2TowerThreshEA_Endcap     = cms.double(0.0),
#
    HcalTowerInnerRadiusB_Endcap       = cms.double(0.15),
    HcalTowerOuterRadiusB_Endcap       = cms.double(0.3),
    HcalTowerThreshEB_Endcap           = cms.double(0.0),
    HcalDepth1TowerInnerRadiusB_Endcap = cms.double(0.15),
    HcalDepth1TowerOuterRadiusB_Endcap = cms.double(0.3),
    HcalDepth1TowerThreshEB_Endcap     = cms.double(0.0),
    HcalDepth2TowerInnerRadiusB_Endcap = cms.double(0.15),
    HcalDepth2TowerOuterRadiusB_Endcap = cms.double(0.3),
    HcalDepth2TowerThreshEB_Endcap     = cms.double(0.0),

    recHitFlagsToBeExcluded = cms.vint32(
        ecalRecHitFlag_kFaultyHardware,
        ecalRecHitFlag_kPoorCalib,
#        ecalRecHitFlag_kSaturated,
#        ecalRecHitFlag_kLeadingEdgeRecovered,
#        ecalRecHitFlag_kNeighboursRecovered,
        ecalRecHitFlag_kTowerRecovered,
        ecalRecHitFlag_kDead
    ),


)


