/** \class HLTMuonIsoFilter
 *
 * See header file for documentation
 *
 *  \author J. Alcaraz
 *
 */

#include "HLTrigger/Muon/interface/HLTMuonIsoFilter.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerRefsCollections.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"

#include "RecoMuon/MuonIsolation/interface/MuonIsolatorFactory.h"

#include <iostream>
//
// constructors and destructor
//
HLTMuonIsoFilter::HLTMuonIsoFilter(const edm::ParameterSet& iConfig) :
   candTag_ (iConfig.getParameter< edm::InputTag > ("CandTag") ),
   previousCandTag_ (iConfig.getParameter<edm::InputTag > ("PreviousCandTag")),
   depTag_  (iConfig.getParameter< std::vector< edm::InputTag > >("DepTag" ) ),
   theDepositIsolator(0),
   min_N_   (iConfig.getParameter<int> ("MinN")),
   saveTag_  (iConfig.getUntrackedParameter<bool> ("SaveTag",false))
{
  std::stringstream tags;
  for (unsigned int i=0;i!=depTag_.size();++i)
    tags<<" IsoTag["<<i<<"] : "<<depTag_[i].encode()<<" \n";
   LogDebug("HLTMuonIsoFilter") << " candTag : " << candTag_.encode()
				<< "\n" << tags 
				<< "  MinN : " << min_N_;

   edm::ParameterSet isolatorPSet = iConfig.getParameter<edm::ParameterSet>("IsolatorPSet");
   if (isolatorPSet.empty()) {
     theDepositIsolator=0;
       }else{
     std::string type = isolatorPSet.getParameter<std::string>("ComponentName");
     theDepositIsolator = MuonIsolatorFactory::get()->create(type, isolatorPSet);
   }
   
   //register your products
   produces<trigger::TriggerFilterObjectWithRefs>();
   if (theDepositIsolator) produces<edm::ValueMap<bool> >();
}

HLTMuonIsoFilter::~HLTMuonIsoFilter()
{
}

//
// member functions
//

// ------------ method called to produce the data  ------------
bool
HLTMuonIsoFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace trigger;
   using namespace reco;

   // All HLT filters must create and fill an HLT filter object,
   // recording any reconstructed physics objects satisfying (or not)
   // this HLT filter, and place it in the Event.

   // The filter object
   auto_ptr<TriggerFilterObjectWithRefs>
     filterproduct (new TriggerFilterObjectWithRefs(path(),module()));
   
   //the decision map
   std::auto_ptr<edm::ValueMap<bool> > 
     isoMap( new edm::ValueMap<bool> ());
   
   // get hold of trks
   Handle<RecoChargedCandidateCollection> mucands;
   if(saveTag_)filterproduct->addCollectionTag(candTag_);
   iEvent.getByLabel (candTag_,mucands);
   Handle<TriggerFilterObjectWithRefs> previousLevelCands;
   iEvent.getByLabel (previousCandTag_,previousLevelCands);
   vector<RecoChargedCandidateRef> vcands;
   previousLevelCands->getObjects(TriggerMuon,vcands);

   
   //get hold of energy deposition
   unsigned int nDep=depTag_.size();
   std::vector< Handle<edm::ValueMap<reco::IsoDeposit> > > depMap(nDep);
   Handle<edm::ValueMap<bool> > decisionMap;
   muonisolation::MuIsoBaseIsolator::DepositContainer isoContainer(nDep);
   if (theDepositIsolator){
     for (unsigned int i=0;i!=nDep;++i) iEvent.getByLabel (depTag_[i],depMap[i]);
   }else{
     iEvent.getByLabel(depTag_.front(), decisionMap);
   }
   
   // look at all mucands,  check cuts and add to filter object
   int nIsolatedMu = 0;
   unsigned int nMu=mucands->size();
   std::vector<bool> isos(nMu, false);
   unsigned int iMu=0;
   for (; iMu<nMu; iMu++) {
     RecoChargedCandidateRef candref(mucands,iMu);
     
     //did this candidate triggered at previous stage.
     if (!triggerdByPreviousLevel(candref,vcands)) continue;
   
     //reference to the track
     TrackRef tk = candref->get<TrackRef>();

     if (theDepositIsolator){
       //get the deposits
       for(unsigned int iDep=0;iDep!=nDep;++iDep){
	 const edm::ValueMap<reco::IsoDeposit> ::value_type & muonDeposit = (*(depMap[iDep]))[tk];
	 LogDebug("HLTMuonIsoFilter") << " Muon with q*pt= " << tk->charge()*tk->pt() << ", eta= " << tk->eta() << "; has deposit["<<iDep<<"]: " << muonDeposit.print();
	 isoContainer[iDep] = muonisolation::MuIsoBaseIsolator::DepositAndVetos(&muonDeposit);
       }
       
       //get the selection
       muonisolation::MuIsoBaseIsolator::Result selection = theDepositIsolator->result( isoContainer, *tk );
       isos[iMu]=selection.valBool;
       
     }else{
       //get the decision from the event
       isos[iMu]=(*decisionMap)[tk];
     }
     LogDebug("HLTMuonIsoFilter") << " Muon with q*pt= " << tk->charge()*tk->pt() << ", eta= " << tk->eta() << "; "<<(isos[iMu]?"Is an isolated muon.":"Is NOT an isolated muon.");
       
     if (!isos[iMu]) continue;
     
     nIsolatedMu++;
     filterproduct->addObject(TriggerMuon,candref);
   }

   // filter decision
   const bool accept (nIsolatedMu >= min_N_);

   // put filter object into the Event
   iEvent.put(filterproduct);

   if (theDepositIsolator){
     //put the decision map
     if (nMu!=0){
       edm::ValueMap<bool> ::Filler isoFiller(*isoMap);     
       // get a track ref
       TrackRef aRef = mucands->front().get<TrackRef>();
       // get the corresponding handle
       edm::Handle<reco::TrackCollection> HandleToTrackRef;
       iEvent.get(aRef.id(), HandleToTrackRef);
       isoFiller.insert(HandleToTrackRef, isos.begin(), isos.end());
       isoFiller.fill();
     }
     iEvent.put(isoMap);   
   }

   LogDebug("HLTMuonIsoFilter") << " >>>>> Result of HLTMuonIsoFilter is " << accept << ", number of muons passing isolation cuts= " << nIsolatedMu; 

   return accept;
}

bool HLTMuonIsoFilter::triggerdByPreviousLevel(const reco::RecoChargedCandidateRef & candref, const std::vector<reco::RecoChargedCandidateRef>& vcands){
  bool ok=false;
  unsigned int i=0;
  unsigned int i_max=vcands.size();
  for (;i!=i_max;++i){
    if (candref == vcands[i]) { ok=true; break;}
  }

  return ok;
}
																						       
