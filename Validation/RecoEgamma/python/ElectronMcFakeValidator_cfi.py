
import FWCore.ParameterSet.Config as cms

electronMcFakeHistosCfg = cms.PSet(
  Nbinxyz = cms.int32(50),
  Nbinp = cms.int32(50), Nbinp2D = cms.int32(50), Pmax = cms.double(300.0),
  Nbinpt = cms.int32(50), Nbinpt2D = cms.int32(50), Nbinpteff = cms.int32(19),Ptmax = cms.double(100.0),
  Nbinfhits = cms.int32(30), Fhitsmax = cms.double(30.0),
  Nbinlhits = cms.int32(5), Lhitsmax = cms.double(10.0),
  Nbineta = cms.int32(50), Nbineta2D = cms.int32(50),Etamin = cms.double(-2.5), Etamax = cms.double(2.5),
  Nbindeta = cms.int32(100), Detamin = cms.double(-0.005), Detamax = cms.double(0.005), 
  Nbindetamatch = cms.int32(100), Nbindetamatch2D = cms.int32(50), Detamatchmin = cms.double(-0.05), Detamatchmax = cms.double(0.05),
  Nbinphi = cms.int32(64), Nbinphi2D = cms.int32(32), Phimin = cms.double(-3.2), Phimax = cms.double(3.2),
  Nbindphi = cms.int32(100), Dphimin = cms.double(-0.01), Dphimax = cms.double(0.01),
  Nbindphimatch = cms.int32(100), Nbindphimatch2D = cms.int32(50), Dphimatchmin = cms.double(-0.2), Dphimatchmax = cms.double(0.2),
  Nbineop = cms.int32(50), Nbineop2D = cms.int32(30), Eopmax = cms.double(5.0), Eopmaxsht = cms.double(3.0),
  Nbinmee = cms.int32(100), Meemin = cms.double(0.0), Meemax = cms.double(150.),
  Nbinhoe = cms.int32(100), Hoemin = cms.double(0.0), Hoemax = cms.double(0.5),
  Nbinpopmatching = cms.int32(75), Popmatchingmin = cms.double(0.0), Popmatchingmax = cms.double(1.5)
)

electronMcFakeValidator = cms.EDAnalyzer("ElectronMcFakeValidator",
  electronCollection = cms.InputTag("gsfElectrons"),
  matchingObjectCollection = cms.InputTag("iterativeCone5GenJets"),
  beamSpot = cms.InputTag("offlineBeamSpot"),
  readAOD = cms.bool(False),
  outputFile = cms.string(""),
  MaxPt = cms.double(100.0),
  DeltaR = cms.double(0.3),
  MaxAbsEta = cms.double(2.5),
  histosCfg = cms.PSet(electronMcFakeHistosCfg)
)




