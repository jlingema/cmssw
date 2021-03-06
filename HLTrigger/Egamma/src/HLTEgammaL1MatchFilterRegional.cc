/** \class HLTEgammaL1MatchFilterRegional
 *
 * $Id: HLTEgammaL1MatchFilterRegional.cc,v 1.8 2008/04/21 04:59:38 wsun Exp $
 *
 *  \author Monica Vazquez Acosta (CERN)
 *
 */

#include "HLTrigger/Egamma/interface/HLTEgammaL1MatchFilterRegional.h"

//#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"



#include "CondFormats/L1TObjects/interface/L1CaloGeometry.h"
#include "CondFormats/DataRecord/interface/L1CaloGeometryRecord.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#define TWOPI 6.283185308
//
// constructors and destructor
//
HLTEgammaL1MatchFilterRegional::HLTEgammaL1MatchFilterRegional(const edm::ParameterSet& iConfig)
{
   candIsolatedTag_ = iConfig.getParameter< edm::InputTag > ("candIsolatedTag");
   l1IsolatedTag_ = iConfig.getParameter< edm::InputTag > ("l1IsolatedTag");
   candNonIsolatedTag_ = iConfig.getParameter< edm::InputTag > ("candNonIsolatedTag");
   l1NonIsolatedTag_ = iConfig.getParameter< edm::InputTag > ("l1NonIsolatedTag");
   L1SeedFilterTag_ = iConfig.getParameter< edm::InputTag > ("L1SeedFilterTag");
   ncandcut_  = iConfig.getParameter<int> ("ncandcut");
   doIsolated_   = iConfig.getParameter<bool>("doIsolated");


   region_eta_size_      = iConfig.getParameter<double> ("region_eta_size");
   region_eta_size_ecap_ = iConfig.getParameter<double> ("region_eta_size_ecap");
   region_phi_size_      = iConfig.getParameter<double> ("region_phi_size");
   barrel_end_           = iConfig.getParameter<double> ("barrel_end");   
   endcap_end_           = iConfig.getParameter<double> ("endcap_end");   

   //register your products
   produces<trigger::TriggerFilterObjectWithRefs>();
}

HLTEgammaL1MatchFilterRegional::~HLTEgammaL1MatchFilterRegional(){}


// ------------ method called to produce the data  ------------
bool
HLTEgammaL1MatchFilterRegional::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // std::cout <<"runnr "<<iEvent.id().run()<<" event "<<iEvent.id().event()<<std::endl;
  using namespace trigger;
  using namespace l1extra;
  //using namespace std;
  std::auto_ptr<trigger::TriggerFilterObjectWithRefs> filterobject (new trigger::TriggerFilterObjectWithRefs(path(),module()));

  edm::Ref<reco::RecoEcalCandidateCollection> ref;

  // Get the CaloGeometry
  edm::ESHandle<L1CaloGeometry> l1CaloGeom ;
  iSetup.get<L1CaloGeometryRecord>().get(l1CaloGeom) ;


  // look at all candidates,  check cuts and add to filter object
  int n(0);

  // Get the recoEcalCandidates
  edm::Handle<reco::RecoEcalCandidateCollection> recoIsolecalcands;
  iEvent.getByLabel(candIsolatedTag_,recoIsolecalcands);


  edm::Handle<trigger::TriggerFilterObjectWithRefs> L1SeedOutput;

  iEvent.getByLabel (L1SeedFilterTag_,L1SeedOutput);

  std::vector<l1extra::L1EmParticleRef > l1EGIso;       
  L1SeedOutput->getObjects(TriggerL1IsoEG, l1EGIso);
  
  std::vector<l1extra::L1EmParticleRef > l1EGNonIso;       
  L1SeedOutput->getObjects(TriggerL1NoIsoEG, l1EGNonIso);

  
  for (reco::RecoEcalCandidateCollection::const_iterator recoecalcand= recoIsolecalcands->begin(); recoecalcand!=recoIsolecalcands->end(); recoecalcand++) {

  
    if(fabs(recoecalcand->eta()) < endcap_end_){
      //SC should be inside the ECAL fiducial volume

      bool matchedSCIso = matchedToL1Cand(l1EGIso,recoecalcand->eta(),recoecalcand->phi());

      //now due to SC cleaning given preference to isolated candiates, 
      //if there is an isolated and nonisolated L1 cand in the same eta/phi bin
      //the corresponding SC will be only in the isolated SC collection
      //so if we are !doIsolated_, we need to run over the nonisol L1 collection as well 
      bool matchedSCNonIso=false;
      if(!doIsolated_){
	matchedSCNonIso =  matchedToL1Cand(l1EGNonIso,recoecalcand->eta(),recoecalcand->phi());
      }
       

      if(matchedSCIso || matchedSCNonIso) {
	n++;

	ref = edm::Ref<reco::RecoEcalCandidateCollection>(recoIsolecalcands, distance(recoIsolecalcands->begin(),recoecalcand) );       
	filterobject->addObject(TriggerCluster, ref);
      }//end  matched check

    }//end endcap fiduical check
    
  }//end loop over all isolated RecoEcalCandidates
  

  if(!doIsolated_) {
    edm::Handle<reco::RecoEcalCandidateCollection> recoNonIsolecalcands;
    iEvent.getByLabel(candNonIsolatedTag_,recoNonIsolecalcands);
    
    for (reco::RecoEcalCandidateCollection::const_iterator recoecalcand= recoNonIsolecalcands->begin(); recoecalcand!=recoNonIsolecalcands->end(); recoecalcand++) {
      if(fabs(recoecalcand->eta()) < endcap_end_){
	bool matchedSCNonIso =  matchedToL1Cand(l1EGNonIso,recoecalcand->eta(),recoecalcand->phi());
	
	if(matchedSCNonIso) {
	  n++;
	  ref = edm::Ref<reco::RecoEcalCandidateCollection>(recoNonIsolecalcands, distance(recoNonIsolecalcands->begin(),recoecalcand) );       
	  filterobject->addObject(TriggerCluster, ref);
	}//end  matched check
	
      }//end endcap fiduical check
      
    }//end loop over all isolated RecoEcalCandidates
  }//end doIsolatedCheck
  

  // filter decision
  bool accept(n>=ncandcut_);
  
  // put filter object into the Event
  iEvent.put(filterobject);
  
  // if(!accept)std::cout <<"FAILING FILTER"<<std::endl;

  return accept;
}


bool 
HLTEgammaL1MatchFilterRegional::matchedToL1Cand(const std::vector<l1extra::L1EmParticleRef >& l1Cands,const float scEta,const float scPhi) 
{
  for (unsigned int i=0; i<l1Cands.size(); i++) {
    //ORCA matching method
    double etaBinLow  = 0.;
    double etaBinHigh = 0.;	
    if(fabs(scEta) < barrel_end_){
      etaBinLow = l1Cands[i]->eta() - region_eta_size_/2.;
      etaBinHigh = etaBinLow + region_eta_size_;
    }
    else{
      etaBinLow = l1Cands[i]->eta() - region_eta_size_ecap_/2.;
      etaBinHigh = etaBinLow + region_eta_size_ecap_;
    }
    
    float deltaphi=fabs(scPhi -l1Cands[i]->phi());
    if(deltaphi>TWOPI) deltaphi-=TWOPI;
    if(deltaphi>TWOPI/2.) deltaphi=TWOPI-deltaphi;
    
    if(scEta < etaBinHigh && scEta > etaBinLow && deltaphi <region_phi_size_/2. )  {
      return true;
    }
  }
  return false;
}
