//-------------------------------------------------
//
//   Class: BMTrackFinder
//
//   L1 BM Track Finder EDProducer
//
//
//
//   Author :
//   J. Troconiz              UAM Madrid
//   Modified :
//   G. Flouris               U Ioannina
//--------------------------------------------------

#include "BMTrackFinder.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMTrackContainer.h"
//#include <DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h>

#include "../src/L1MuBMTFConfig.h"
#include "../interface/L1MuBMTFSetup.h"
#include "../interface/L1MuBMTrackFinder.h"

#include <iostream>
#include <iomanip>

using namespace std;

BMTrackFinder::BMTrackFinder(const edm::ParameterSet & pset) {

  produces<L1MuBMTrackContainer>("BMTF");
  //produces<vector<L1MuRegionalCand> >("BM"); -->
  produces<vector<l1t::L1TRegionalMuonCandidate> >("BM");


  setup1 = new L1MuBMTFSetup(pset,consumesCollector());
  usesResource("BMTrackFinder");
}

BMTrackFinder::~BMTrackFinder() {

  delete setup1;

}

void BMTrackFinder::produce(edm::Event& e, const edm::EventSetup& c) {

  if ( L1MuBMTFConfig::Debug(1) ) cout << endl;
  if ( L1MuBMTFConfig::Debug(1) ) cout << "**** L1MuonBMTFTrigger processing event  ****" << endl;

  L1MuBMTrackFinder* dtbx = setup1->TrackFinder();
  dtbx->clear();
  //cout<<"Point 1"<<endl;

  dtbx->run(e,c);
//cout<<"Point 2"<<endl;

  int ndt = dtbx->numberOfTracks();
  if ( L1MuBMTFConfig::Debug(1) ) cout << "Number of muons found by the L1 BBMX TRIGGER : "
                                       << ndt << endl;
//cout<<"Point 3"<<endl;

  auto_ptr<L1MuBMTrackContainer> tra_product(new L1MuBMTrackContainer);
  //auto_ptr<vector<L1MuRegionalCand> >  vec_product(new vector<L1MuRegionalCand>); -->
  std::auto_ptr<vector<l1t::L1TRegionalMuonCandidate>  > vec_product(new vector<l1t::L1TRegionalMuonCandidate> );

  vector<L1MuBMTrackCand>  dtTracks = dtbx->getcache0();
  tra_product->setContainer(dtTracks);
  //vector<L1MuRegionalCand>& BMTracks = dtbx->getcache(); -->
  vector<l1t::L1TRegionalMuonCandidate> & BMTracks = dtbx->getcache();
//cout<<"Point 1"<<endl;
//cout<<vec_product->size()<<"    "<<BMTracks.size()<<endl;

  *vec_product = BMTracks;

  //cout<<"BMTF"<<endl;
  e.put(tra_product,"BMTF");
  //cout<<"BM"<<endl;
  e.put(vec_product,"BM");

}
