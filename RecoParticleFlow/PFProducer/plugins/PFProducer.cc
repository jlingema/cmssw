#include "RecoParticleFlow/PFProducer/plugins/PFProducer.h"
#include "RecoParticleFlow/PFProducer/interface/PFAlgo.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoParticleFlow/PFClusterTools/interface/PFEnergyCalibration.h"
#include "RecoParticleFlow/PFClusterTools/interface/PFClusterCalibration.h"
#include "RecoParticleFlow/PFClusterTools/interface/PFEnergyCalibrationHF.h"
#include "RecoParticleFlow/PFClusterTools/interface/PFSCEnergyCalibration.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include <sstream>

using namespace std;

using namespace boost;

using namespace edm;



PFProducer::PFProducer(const edm::ParameterSet& iConfig) {
  
  unsigned int newCalib = 
    iConfig.getParameter<unsigned int>("pf_newCalib");

  // Create a PFClusterCalbration auto_ptr
  boost::shared_ptr<pftools::PFClusterCalibration> 
    clusterCalibration( new pftools::PFClusterCalibration() );

  // Initialise function parameters properly.
  double lowEP0 
    = iConfig.getParameter<double>("pfcluster_lowEP0");
  double lowEP1 
    = iConfig.getParameter<double>("pfcluster_lowEP1");
  double globalP0 
    = iConfig.getParameter<double>("pfcluster_globalP0");
  double globalP1 
    = iConfig.getParameter<double>("pfcluster_globalP1");
  // 
  clusterCalibration->setCorrections(lowEP0, lowEP1, globalP0, globalP1);
 
  unsigned int allowNegative    
	= iConfig.getParameter<unsigned int>("pfcluster_allowNegative");
  clusterCalibration->setAllowNegativeEnergy(allowNegative);
	
  unsigned int doCorrection
    = iConfig.getParameter<unsigned int>("pfcluster_doCorrection");
  clusterCalibration->setDoCorrection(doCorrection);
 
  double barrelEta
    = iConfig.getParameter<double>("pfcluster_barrelEndcapEtaDiv");
  clusterCalibration->setBarrelBoundary(barrelEta);
	
  /* Now obsolete 
  double ecalEcut = 
    iConfig.getParameter<double>("pfcluster_ecalECut");
  double hcalEcut = 
    iConfig.getParameter<double>("pfcluster_hcalECut");

  clusterCalibration->setEcalHcalEnergyCuts(ecalEcut,hcalEcut);
  */
 
  std::vector<std::string>* names = clusterCalibration->getKnownSectorNames();
  for(std::vector<std::string>::iterator i = names->begin(); i != names->end(); ++i) {
    std::string sector = *i;
    std::vector<double> params
    = iConfig.getParameter<std::vector<double> >(sector);
    clusterCalibration->setEvolutionParameters(sector, params);
  }

  //Finally set eta correction
  unsigned int doEtaCorrection = iConfig.getParameter<unsigned int>("pfcluster_doEtaCorrection");   
  clusterCalibration->setDoEtaCorrection(doEtaCorrection);

  std::vector<double> etaCorrectionParams = 
    iConfig.getParameter<std::vector<double> >("pfcluster_etaCorrection");
  clusterCalibration->setEtaCorrectionParameters(etaCorrectionParams);
  // use configuration file to setup input/output collection names
  //std::cout << "Finished initialisaing PFClusterCalibration: it looks like...\n";
  //std::cout  << *clusterCalibration << std::endl;

  //Done with PFClusterCalibration //

  //--ab: get calibration factors for HF:
  bool calibHF_use;
  std::vector<double>  calibHF_eta_step;
  std::vector<double>  calibHF_a_EMonly;
  std::vector<double>  calibHF_b_HADonly;
  std::vector<double>  calibHF_a_EMHAD;
  std::vector<double>  calibHF_b_EMHAD;
  calibHF_use =     iConfig.getParameter<bool>("calibHF_use");
  calibHF_eta_step  = iConfig.getParameter<std::vector<double> >("calibHF_eta_step");
  calibHF_a_EMonly  = iConfig.getParameter<std::vector<double> >("calibHF_a_EMonly");
  calibHF_b_HADonly = iConfig.getParameter<std::vector<double> >("calibHF_b_HADonly");
  calibHF_a_EMHAD   = iConfig.getParameter<std::vector<double> >("calibHF_a_EMHAD");
  calibHF_b_EMHAD   = iConfig.getParameter<std::vector<double> >("calibHF_b_EMHAD");
  boost::shared_ptr<PFEnergyCalibrationHF>  
    thepfEnergyCalibrationHF ( new PFEnergyCalibrationHF(calibHF_use,calibHF_eta_step,calibHF_a_EMonly,calibHF_b_HADonly,calibHF_a_EMHAD,calibHF_b_EMHAD) ) ;
  //-----------------

  inputTagBlocks_ 
    = iConfig.getParameter<InputTag>("blocks");

  //Post cleaning of the muons
  inputTagMuons_ 
    = iConfig.getParameter<InputTag>("muons");
  postMuonCleaning_
    = iConfig.getParameter<bool>("postMuonCleaning");

  usePFElectrons_
    = iConfig.getParameter<bool>("usePFElectrons");    

  useEGammaElectrons_
    = iConfig.getParameter<bool>("useEGammaElectrons");    

  if(  useEGammaElectrons_) {
    inputTagEgammaElectrons_ = iConfig.getParameter<edm::InputTag>("egammaElectrons");
  }

  electronOutputCol_
    = iConfig.getParameter<std::string>("pf_electron_output_col");

  bool usePFSCEleCalib;
  std::vector<double>  calibPFSCEle_barrel;
  std::vector<double>  calibPFSCEle_endcap;
  usePFSCEleCalib =     iConfig.getParameter<bool>("usePFSCEleCalib");
  calibPFSCEle_barrel = iConfig.getParameter<std::vector<double> >("calibPFSCEle_barrel");
  calibPFSCEle_endcap = iConfig.getParameter<std::vector<double> >("calibPFSCEle_endcap");
  boost::shared_ptr<PFSCEnergyCalibration>  
    thePFSCEnergyCalibration ( new PFSCEnergyCalibration(calibPFSCEle_barrel,calibPFSCEle_endcap ));
			       
  bool useEGammaSupercluster = iConfig.getParameter<bool>("useEGammaSupercluster");
  double sumEtEcalIsoForEgammaSC_barrel = iConfig.getParameter<double>("sumEtEcalIsoForEgammaSC_barrel");
  double sumEtEcalIsoForEgammaSC_endcap = iConfig.getParameter<double>("sumEtEcalIsoForEgammaSC_endcap");
  double coneEcalIsoForEgammaSC = iConfig.getParameter<double>("coneEcalIsoForEgammaSC");
  double sumPtTrackIsoForEgammaSC_barrel = iConfig.getParameter<double>("sumPtTrackIsoForEgammaSC_barrel");
  double sumPtTrackIsoForEgammaSC_endcap = iConfig.getParameter<double>("sumPtTrackIsoForEgammaSC_endcap");
  double coneTrackIsoForEgammaSC = iConfig.getParameter<double>("coneTrackIsoForEgammaSC");
  unsigned int nTrackIsoForEgammaSC  = iConfig.getParameter<unsigned int>("nTrackIsoForEgammaSC");


  // register products
  produces<reco::PFCandidateCollection>();
  produces<reco::PFCandidateCollection>("CleanedHF");
  produces<reco::PFCandidateCollection>("CleanedCosmicsMuons");
  produces<reco::PFCandidateCollection>("CleanedTrackerAndGlobalMuons");
  produces<reco::PFCandidateCollection>("CleanedFakeMuons");
  produces<reco::PFCandidateCollection>("CleanedPunchThroughMuons");
  produces<reco::PFCandidateCollection>("CleanedPunchThroughNeutralHadrons");
  produces<reco::PFCandidateCollection>("AddedMuonsAndHadrons");


  if (usePFElectrons_) {
    produces<reco::PFCandidateCollection>(electronOutputCol_);
    produces<reco::PFCandidateElectronExtraCollection>(electronExtraOutputCol_);
  }

  double nSigmaECAL 
    = iConfig.getParameter<double>("pf_nsigma_ECAL");
  double nSigmaHCAL 
    = iConfig.getParameter<double>("pf_nsigma_HCAL");
  
  
  double e_slope
    = iConfig.getParameter<double>("pf_calib_ECAL_slope");
  double e_offset 
    = iConfig.getParameter<double>("pf_calib_ECAL_offset");
  
  double eh_eslope
    = iConfig.getParameter<double>("pf_calib_ECAL_HCAL_eslope");
  double eh_hslope 
    = iConfig.getParameter<double>("pf_calib_ECAL_HCAL_hslope");
  double eh_offset 
    = iConfig.getParameter<double>("pf_calib_ECAL_HCAL_offset");
  
  double h_slope
    = iConfig.getParameter<double>("pf_calib_HCAL_slope");
  double h_offset 
    = iConfig.getParameter<double>("pf_calib_HCAL_offset");
  double h_damping 
    = iConfig.getParameter<double>("pf_calib_HCAL_damping");

  //PFElectrons Configuration
  double mvaEleCut
    = iConfig.getParameter<double>("pf_electron_mvaCut");

  string mvaWeightFileEleID
    = iConfig.getParameter<string>("pf_electronID_mvaWeightFile");

  bool applyCrackCorrectionsForElectrons
    = iConfig.getParameter<bool>("pf_electronID_crackCorrection");

  string path_mvaWeightFileEleID;
  if(usePFElectrons_)
    {
      path_mvaWeightFileEleID = edm::FileInPath ( mvaWeightFileEleID.c_str() ).fullPath();
    }


  //Secondary tracks and displaced vertices parameters

  bool rejectTracks_Bad
    = iConfig.getParameter<bool>("rejectTracks_Bad");

  bool rejectTracks_Step45
    = iConfig.getParameter<bool>("rejectTracks_Step45");

  bool usePFNuclearInteractions
    = iConfig.getParameter<bool>("usePFNuclearInteractions");

  bool usePFConversions
    = iConfig.getParameter<bool>("usePFConversions");  

  bool usePFDecays
    = iConfig.getParameter<bool>("usePFDecays");

  double dptRel_DispVtx
    = iConfig.getParameter<double>("dptRel_DispVtx");

  edm::ParameterSet iCfgCandConnector 
    = iConfig.getParameter<edm::ParameterSet>("iCfgCandConnector");


  boost::shared_ptr<PFEnergyCalibration> 
    calibration( new PFEnergyCalibration( e_slope,
					  e_offset, 
					  eh_eslope,
					  eh_hslope,
					  eh_offset,
					  h_slope,
					  h_offset,
					  h_damping,
					  newCalib ) );

  int algoType 
    = iConfig.getParameter<unsigned>("algoType");
  
  switch(algoType) {
  case 0:
    pfAlgo_.reset( new PFAlgo);
    break;
   default:
    assert(0);
  }
  
  pfAlgo_->setParameters( nSigmaECAL, 
			  nSigmaHCAL,
			  calibration,
			  clusterCalibration,
			  thepfEnergyCalibrationHF,
			  newCalib);

  //PFElectrons: call the method setpfeleparameters
  pfAlgo_->setPFEleParameters(mvaEleCut,
			      path_mvaWeightFileEleID,
			      usePFElectrons_,
			      thePFSCEnergyCalibration,
			      sumEtEcalIsoForEgammaSC_barrel,
			      sumEtEcalIsoForEgammaSC_endcap,
			      coneEcalIsoForEgammaSC,
			      sumPtTrackIsoForEgammaSC_barrel,
			      sumPtTrackIsoForEgammaSC_endcap,
			      nTrackIsoForEgammaSC,
			      coneTrackIsoForEgammaSC,
			      applyCrackCorrectionsForElectrons,
			      usePFSCEleCalib,
			      useEGammaElectrons_,
			      useEGammaSupercluster);
  
  //  pfAlgo_->setPFConversionParameters(usePFConversions);
  
  //Secondary tracks and displaced vertices parameters
  
  pfAlgo_->setDisplacedVerticesParameters(rejectTracks_Bad,
					  rejectTracks_Step45,
					  usePFNuclearInteractions,
 					  usePFConversions,
	 				  usePFDecays,
					  dptRel_DispVtx);
  




  if (usePFNuclearInteractions)
    pfAlgo_->setCandConnectorParameters( iCfgCandConnector );

  // Muon parameters
  std::vector<double> muonHCAL
    = iConfig.getParameter<std::vector<double> >("muon_HCAL");  
  std::vector<double> muonECAL
    = iConfig.getParameter<std::vector<double> >("muon_ECAL");  
  assert ( muonHCAL.size() == 2 && muonECAL.size() == 2 );
  
  // Fake track parameters
  double nSigmaTRACK
    = iConfig.getParameter<double>("nsigma_TRACK");  
  
  double ptError
    = iConfig.getParameter<double>("pt_Error");  
  
  std::vector<double> factors45
    = iConfig.getParameter<std::vector<double> >("factors_45");  
  assert ( factors45.size() == 2 );
  
  bool usePFMuonMomAssign
    = iConfig.getParameter<bool>("usePFMuonMomAssign");

  // Set muon and fake track parameters
  pfAlgo_->setPFMuonAndFakeParameters(muonHCAL,
				      muonECAL,
				      nSigmaTRACK,
				      ptError,
				      factors45,
				      usePFMuonMomAssign);
  
  //Post cleaning of the HF
  bool postHFCleaning
    = iConfig.getParameter<bool>("postHFCleaning");

  double minHFCleaningPt 
    = iConfig.getParameter<double>("minHFCleaningPt");
  double minSignificance
    = iConfig.getParameter<double>("minSignificance");
  double maxSignificance
    = iConfig.getParameter<double>("maxSignificance");
  double minSignificanceReduction
    = iConfig.getParameter<double>("minSignificanceReduction");
  double maxDeltaPhiPt
    = iConfig.getParameter<double>("maxDeltaPhiPt");
  double minDeltaMet
    = iConfig.getParameter<double>("minDeltaMet");

  // Set post HF cleaning muon parameters
  pfAlgo_->setPostHFCleaningParameters(postHFCleaning,
				       minHFCleaningPt,
				       minSignificance,
				       maxSignificance,
				       minSignificanceReduction,
				       maxDeltaPhiPt,
				       minDeltaMet);

  // Input tags for HF cleaned rechits
  inputTagCleanedHF_ 
    = iConfig.getParameter< std::vector<edm::InputTag> >("cleanedHF");

  //MIKE: Vertex Parameters
  vertices_ = iConfig.getParameter<edm::InputTag>("vertexCollection");
  useVerticesForNeutral_ = iConfig.getParameter<bool>("useVerticesForNeutral");



  verbose_ = 
    iConfig.getUntrackedParameter<bool>("verbose",false);

  bool debug_ = 
    iConfig.getUntrackedParameter<bool>("debug",false);

  pfAlgo_->setDebug( debug_ );

}



PFProducer::~PFProducer() {}


void 
PFProducer::beginJob() {}

void 
PFProducer::beginRun(edm::Run & run, 
		     const edm::EventSetup & es) {}


void 
PFProducer::produce(Event& iEvent, 
		    const EventSetup& iSetup) {
  
  LogDebug("PFProducer")<<"START event: "
			<<iEvent.id().event()
			<<" in run "<<iEvent.id().run()<<endl;
  

  // Get The vertices from the event
  // and assign dynamic vertex parameters
  edm::Handle<reco::VertexCollection> vertices;
  bool gotVertices = iEvent.getByLabel(vertices_,vertices);
  if(!gotVertices) {
    ostringstream err;
    err<<"Cannot find vertices for this event.Continuing Without them ";
    LogError("PFProducer")<<err.str()<<endl;
  }

  //Assign the PFAlgo Parameters
  pfAlgo_->setPFVertexParameters(useVerticesForNeutral_,*vertices);

  // get the collection of blocks 

  Handle< reco::PFBlockCollection > blocks;

  LogDebug("PFProducer")<<"getting blocks"<<endl;
  bool found = iEvent.getByLabel( inputTagBlocks_, blocks );  

  if(!found ) {

    ostringstream err;
    err<<"cannot find blocks: "<<inputTagBlocks_;
    LogError("PFProducer")<<err.str()<<endl;
    
    throw cms::Exception( "MissingProduct", err.str());
  }

  // get the collection of muons 

  Handle< reco::MuonCollection > muons;

  if ( postMuonCleaning_ ) {

    LogDebug("PFProducer")<<"getting muons"<<endl;
    found = iEvent.getByLabel( inputTagMuons_, muons );  

    if(!found) {
      ostringstream err;
      err<<"cannot find muons: "<<inputTagMuons_;
      LogError("PFProducer")<<err.str()<<endl;
    
      throw cms::Exception( "MissingProduct", err.str());
    }

  }

  if (useEGammaElectrons_) {
    Handle < reco::GsfElectronCollection > egelectrons;
    
    LogDebug("PFProducer")<<" Reading e/gamma electrons activated "<<endl;
    found = iEvent.getByLabel( inputTagEgammaElectrons_, egelectrons );  
    
    if(!found) {
      ostringstream err;
      err<<"cannot find electrons: "<<inputTagEgammaElectrons_;
      LogError("PFProducer")<<err.str()<<endl;
    
      throw cms::Exception( "MissingProduct", err.str());
    }
    
    pfAlgo_->setEGElectronCollection(*egelectrons);
  }

  
  LogDebug("PFProducer")<<"particle flow is starting"<<endl;

  assert( blocks.isValid() );
 
  pfAlgo_->reconstructParticles( blocks );

  if(verbose_) {
    ostringstream  str;
    str<<(*pfAlgo_)<<endl;
    //    cout << (*pfAlgo_) << endl;
    LogInfo("PFProducer") <<str.str()<<endl;
  }  


  if ( postMuonCleaning_ )
    pfAlgo_->postMuonCleaning( muons, *vertices );

  // Florian 5/01/2011
  // Save the PFElectron Extra Collection First as to be able to create valid References  
  if(usePFElectrons_)   {  
    auto_ptr< reco::PFCandidateElectronExtraCollection >
      pOutputElectronCandidateExtraCollection( pfAlgo_->transferElectronExtra() ); 

    const edm::OrphanHandle<reco::PFCandidateElectronExtraCollection > electronExtraProd=
      iEvent.put(pOutputElectronCandidateExtraCollection,electronExtraOutputCol_);      
    pfAlgo_->setElectronExtraRef(electronExtraProd);
  }


  // Save cosmic cleaned muon candidates
  auto_ptr< reco::PFCandidateCollection > 
    pCosmicsMuonCleanedCandidateCollection( pfAlgo_->transferCosmicsMuonCleanedCandidates() ); 
  // Save tracker/global cleaned muon candidates
  auto_ptr< reco::PFCandidateCollection > 
    pTrackerAndGlobalCleanedMuonCandidateCollection( pfAlgo_->transferCleanedTrackerAndGlobalMuonCandidates() ); 
  // Save fake cleaned muon candidates
  auto_ptr< reco::PFCandidateCollection > 
    pFakeCleanedMuonCandidateCollection( pfAlgo_->transferFakeMuonCleanedCandidates() ); 
  // Save punch-through cleaned muon candidates
  auto_ptr< reco::PFCandidateCollection > 
    pPunchThroughMuonCleanedCandidateCollection( pfAlgo_->transferPunchThroughMuonCleanedCandidates() ); 
  // Save punch-through cleaned neutral hadron candidates
  auto_ptr< reco::PFCandidateCollection > 
    pPunchThroughHadronCleanedCandidateCollection( pfAlgo_->transferPunchThroughHadronCleanedCandidates() ); 
  // Save added muon candidates
  auto_ptr< reco::PFCandidateCollection > 
    pAddedMuonCandidateCollection( pfAlgo_->transferAddedMuonCandidates() ); 

  // Check HF overcleaning
  reco::PFRecHitCollection hfCopy;
  for ( unsigned ihf=0; ihf<inputTagCleanedHF_.size(); ++ihf ) {
    Handle< reco::PFRecHitCollection > hfCleaned;
    bool foundHF = iEvent.getByLabel( inputTagCleanedHF_[ihf], hfCleaned );  
    if (!foundHF) continue;
    for ( unsigned jhf=0; jhf<(*hfCleaned).size(); ++jhf ) { 
      hfCopy.push_back( (*hfCleaned)[jhf] );
    }
  }
  pfAlgo_->checkCleaning( hfCopy );

  // Save recovered HF candidates
  auto_ptr< reco::PFCandidateCollection > 
    pCleanedCandidateCollection( pfAlgo_->transferCleanedCandidates() ); 

  
  // Save the final PFCandidate collection
  auto_ptr< reco::PFCandidateCollection > 
    pOutputCandidateCollection( pfAlgo_->transferCandidates() ); 
  

  
  LogDebug("PFProducer")<<"particle flow: putting products in the event"<<endl;
  if ( verbose_ ) std::cout <<"particle flow: putting products in the event. Here the full list"<<endl;
  int nC=0;
  for( reco::PFCandidateCollection::const_iterator  itCand =  (*pOutputCandidateCollection).begin(); itCand !=  (*pOutputCandidateCollection).end(); itCand++) {
    nC++;
      if (verbose_ ) std::cout << nC << ")" << (*itCand).particleId() << std::endl;

  }

  // Write in the event
  iEvent.put(pOutputCandidateCollection);
  iEvent.put(pCleanedCandidateCollection,"CleanedHF");

  if ( postMuonCleaning_ ) { 
    iEvent.put(pCosmicsMuonCleanedCandidateCollection,"CleanedCosmicsMuons");
    iEvent.put(pTrackerAndGlobalCleanedMuonCandidateCollection,"CleanedTrackerAndGlobalMuons");
    iEvent.put(pFakeCleanedMuonCandidateCollection,"CleanedFakeMuons");
    iEvent.put(pPunchThroughMuonCleanedCandidateCollection,"CleanedPunchThroughMuons");
    iEvent.put(pPunchThroughHadronCleanedCandidateCollection,"CleanedPunchThroughNeutralHadrons");
    iEvent.put(pAddedMuonCandidateCollection,"AddedMuonsAndHadrons");
  }

  if(usePFElectrons_)
    {
      auto_ptr< reco::PFCandidateCollection >  
	pOutputElectronCandidateCollection( pfAlgo_->transferElectronCandidates() ); 
      iEvent.put(pOutputElectronCandidateCollection,electronOutputCol_);

    }
}

