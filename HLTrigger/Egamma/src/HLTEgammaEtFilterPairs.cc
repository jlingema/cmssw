/** \class HLTEgammaEtFilterPairs
 *
 * $Id: HLTEgammaEtFilterPairs.cc,v 1.1 2008/10/14 14:52:57 ghezzi Exp $
 *
 *  \author Alessio Ghezzi
 *
 */

#include "HLTrigger/Egamma/interface/HLTEgammaEtFilterPairs.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"

//
// constructors and destructor
//
HLTEgammaEtFilterPairs::HLTEgammaEtFilterPairs(const edm::ParameterSet& iConfig)
{
   inputTag_ = iConfig.getParameter< edm::InputTag > ("inputTag");
   etcutEB1_  = iConfig.getParameter<double> ("etcut1EB");
   etcutEE1_  = iConfig.getParameter<double> ("etcut1EE");
   etcutEB2_  = iConfig.getParameter<double> ("etcut2EB");
   etcutEE2_  = iConfig.getParameter<double> ("etcut2EE");
   store_ = iConfig.getUntrackedParameter<bool> ("SaveTag",false) ;
   relaxed_ = iConfig.getUntrackedParameter<bool> ("relaxed",true) ;
   L1IsoCollTag_= iConfig.getParameter< edm::InputTag > ("L1IsoCand"); 
   L1NonIsoCollTag_= iConfig.getParameter< edm::InputTag > ("L1NonIsoCand"); 

   //register your products
   produces<trigger::TriggerFilterObjectWithRefs>();
}

HLTEgammaEtFilterPairs::~HLTEgammaEtFilterPairs(){}


// ------------ method called to produce the data  ------------
bool
HLTEgammaEtFilterPairs::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace trigger;
  // The filter object
    std::auto_ptr<trigger::TriggerFilterObjectWithRefs> filterproduct (new trigger::TriggerFilterObjectWithRefs(path(),module()));
    if( store_ ){filterproduct->addCollectionTag(L1IsoCollTag_);}
    if( store_ && relaxed_){filterproduct->addCollectionTag(L1NonIsoCollTag_);}

  edm::Handle<trigger::TriggerFilterObjectWithRefs> PrevFilterOutput;
  iEvent.getByLabel (inputTag_,PrevFilterOutput);

  std::vector<edm::Ref<reco::RecoEcalCandidateCollection> > recoecalcands;                // vref with your specific C++ collection type
  PrevFilterOutput->getObjects(TriggerCluster, recoecalcands);
  // they list should be interpreted as pairs:
  // <recoecalcands[0],recoecalcands[1]>
  // <recoecalcands[2],recoecalcands[3]>
  // <recoecalcands[4],recoecalcands[5]>
  // .......

  // Should I check that the size of recoecalcands is even ?
  int n(0);

  for (unsigned int i=0; i<recoecalcands.size(); i=i+2) {
    
     edm::Ref<reco::RecoEcalCandidateCollection> r1 = recoecalcands[i];
     edm::Ref<reco::RecoEcalCandidateCollection> r2 = recoecalcands[i+1];
     //  std::cout<<"EtFilter: 1) Et Eta phi: "<<r1->et()<<" "<<r1->eta()<<" "<<r1->phi()<<" 2) Et eta phi: "<<r2->et()<<" "<<r2->eta()<<" "<<r2->phi()<<std::endl;
     bool first  = (fabs(r1->eta()) < 1.479 &&  r1->et()  >= etcutEB1_) || (fabs(r1->eta()) >= 1.479 &&  r1->et()  >= etcutEE1_);
     bool second = (fabs(r2->eta()) < 1.479 &&  r2->et()  >= etcutEB2_) || (fabs(r2->eta()) >= 1.479 &&  r2->et()  >= etcutEE2_);

    if ( first && second ) {
      n++;
      filterproduct->addObject(TriggerCluster,r1 );
      filterproduct->addObject(TriggerCluster,r2 );
    }
  }

  
  // filter decision
  bool accept(n>=1);
  
  // put filter object into the Event
  iEvent.put(filterproduct);
  
  return accept;
}
