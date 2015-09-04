//-------------------------------------------------
//
//   Class: L1MuBMTrackFinder
//
//   Description: L1 barrel Muon Trigger Track Finder
//
//
//
//   Author :
//   N. Neumeister            CERN EP
//   J. Troconiz              UAM Madrid
//
//--------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------

#include "L1Trigger/L1BarrelMuonTrackFinder/interface/L1MuBMTrackFinder.h"

//---------------
// C++ Headers --
//---------------

#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/Event.h>
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
//#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackCand.h"
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMTrackCand.h"
#include "L1Trigger/L1BarrelMuonTrackFinder/src/L1MuBMTFConfig.h"
#include "L1Trigger/L1BarrelMuonTrackFinder/src/L1MuBMSecProcId.h"
#include "L1Trigger/L1BarrelMuonTrackFinder/src/L1MuBMSecProcMap.h"
#include "L1Trigger/L1BarrelMuonTrackFinder/src/L1MuBMSectorProcessor.h"
#include "L1Trigger/L1BarrelMuonTrackFinder/src/L1MuBMEtaProcessor.h"
#include "L1Trigger/L1BarrelMuonTrackFinder/src/L1MuBMWedgeSorter.h"
#include "L1Trigger/L1BarrelMuonTrackFinder/src/L1MuBMMuonSorter.h"
#include "L1Trigger/L1BarrelMuonTrackFinder/interface/L1MuBMTrack.h"

#include "DataFormats/L1TMuon/interface/L1TRegionalMuonCandidate.h"


using namespace std;

//---------------------------------
//       class L1MuBMTrackFinder
//---------------------------------


//----------------
// Constructors --
//----------------

L1MuBMTrackFinder::L1MuBMTrackFinder(const edm::ParameterSet & ps,edm::ConsumesCollector && iC) {

  // set configuration parameters
  if ( m_config == 0 ) m_config = new L1MuBMTFConfig(ps);

  if ( L1MuBMTFConfig::Debug(1) ) cout << endl;
  if ( L1MuBMTFConfig::Debug(1) ) cout << "**** entering L1MuBMTrackFinder ****" << endl;
  if ( L1MuBMTFConfig::Debug(1) ) cout << endl;

  m_spmap = new L1MuBMSecProcMap();
  m_epvec.reserve(12);
  m_wsvec.reserve(12);
  m_ms = 0;

  _cache.reserve(4*17);
  _cache0.reserve(144*17);

  iC.consumes<L1MuDTChambPhDigi>(L1MuBMTFConfig::getBMDigiInputTag());
}


//--------------
// Destructor --
//--------------

L1MuBMTrackFinder::~L1MuBMTrackFinder() {

  delete m_spmap;

  vector<L1MuBMEtaProcessor*>::iterator it_ep = m_epvec.begin();
  while ( it_ep != m_epvec.end() ) {
    delete (*it_ep);
    it_ep++;
  }
  m_epvec.clear();

  vector<L1MuBMWedgeSorter*>::iterator it_ws = m_wsvec.begin();
  while ( it_ws != m_wsvec.end() ) {
    delete (*it_ws);
    it_ws++;
  }
  m_wsvec.clear();

  delete m_ms;

  if ( m_config ) delete m_config;
  m_config = 0;

}


//--------------
// Operations --
//--------------

//
// setup MTTF configuration
//
void L1MuBMTrackFinder::setup() {

  // build the barrel Muon Trigger Track Finder

  if ( L1MuBMTFConfig::Debug(1) ) cout << endl;
  if ( L1MuBMTFConfig::Debug(1) ) cout << "**** L1MuBMTrackFinder building ****" << endl;
  if ( L1MuBMTFConfig::Debug(1) ) cout << endl;

  // create new sector processors
  for ( int wh = -3; wh <= 3; wh++ ) {
    if ( wh == 0 ) continue;
    for ( int sc = 0; sc < 12; sc++ ) {
      L1MuBMSecProcId tmpspid(wh,sc);
      L1MuBMSectorProcessor* sp = new L1MuBMSectorProcessor(*this,tmpspid);
      if ( L1MuBMTFConfig::Debug(2) ) cout << "creating " << tmpspid << endl;
      m_spmap->insert(tmpspid,sp);
    }
  }

  // create new eta processors and wedge sorters
  for ( int sc = 0; sc < 12; sc++ ) {
    L1MuBMEtaProcessor* ep = new L1MuBMEtaProcessor(*this,sc);
    if ( L1MuBMTFConfig::Debug(2) ) cout << "creating Eta Processor " << sc << endl;
    m_epvec.push_back(ep);
    L1MuBMWedgeSorter* ws = new L1MuBMWedgeSorter(*this,sc);
    if ( L1MuBMTFConfig::Debug(2) ) cout << "creating Wedge Sorter " << sc << endl;
    m_wsvec.push_back(ws);
  }

  // create new muon sorter
  if ( L1MuBMTFConfig::Debug(2) ) cout << "creating BM Muon Sorter " << endl;
  m_ms = new L1MuBMMuonSorter(*this);

}


//
// run MTTF
//
void L1MuBMTrackFinder::run(const edm::Event& e, const edm::EventSetup& c) {

  // run the barrel Muon Trigger Track Finder

  edm::Handle<L1MuDTChambPhContainer> dttrig;
  e.getByLabel(L1MuBMTFConfig::getBMDigiInputTag(),dttrig);
  if ( dttrig->getContainer()->size() == 0 ) return;

  if ( L1MuBMTFConfig::Debug(2) ) cout << endl;
  if ( L1MuBMTFConfig::Debug(2) ) cout << "**** L1MuBMTrackFinder processing ------****" << endl;
  if ( L1MuBMTFConfig::Debug(2) ) cout << endl;



  int bx_min = L1MuBMTFConfig::getBxMin();
  int bx_max = L1MuBMTFConfig::getBxMax();

  for ( int bx = bx_min; bx <= bx_max; bx++ ) {

  if ( dttrig->bxEmpty(bx) ) continue;

  if ( L1MuBMTFConfig::Debug(2) ) cout << "L1MuBMTrackFinder processing bunch-crossing : " << bx << endl;

    // reset MTTF
    reset();

    // run sector processors
    L1MuBMSecProcMap::SPmap_iter it_sp = m_spmap->begin();
    while ( it_sp != m_spmap->end() ) {
      if ( L1MuBMTFConfig::Debug(2) ) cout << "running "
                                           << (*it_sp).second->id() << endl;
      if ( (*it_sp).second ) (*it_sp).second->run(bx,e,c);
      if ( L1MuBMTFConfig::Debug(2) && (*it_sp).second ) (*it_sp).second->print();
      it_sp++;
    }

    // run eta processors
    vector<L1MuBMEtaProcessor*>::iterator it_ep = m_epvec.begin();
    while ( it_ep != m_epvec.end() ) {
      if ( L1MuBMTFConfig::Debug(2) ) cout << "running Eta Processor "
                                       << (*it_ep)->id() << endl;
      if ( *it_ep ) (*it_ep)->run(bx,e,c);
      if ( L1MuBMTFConfig::Debug(2) && *it_ep ) (*it_ep)->print();
      it_ep++;
    }

    // read sector processors
    it_sp = m_spmap->begin();
    while ( it_sp != m_spmap->end() ) {
      if ( L1MuBMTFConfig::Debug(2) ) cout << "reading "
                                           << (*it_sp).second->id() << endl;
      for ( int number = 0; number < 2; number++ ) {
        const L1MuBMTrack* cand = (*it_sp).second->tracK(number);
//        if ( cand && !cand->empty() ) _cache0.push_back(L1MuDTTrackCand(cand->getDataWord(),cand->bx(),
//                                              cand->spid().wheel(),cand->spid().sector(),number,cand->address(1),
//                                              cand->address(2),cand->address(3),cand->address(4),cand->tc()));

        if ( cand && !cand->empty() )  _cache0.push_back(L1MuBMTrackCand(cand->pt(),cand->phi(),cand->eta(),cand->charge(),cand->quality(),
                                                                         cand->bx(),cand->spid().wheel(),cand->spid().sector(),number,
                                                                        cand->address(1),cand->address(2),cand->address(3),cand->address(4),cand->tc()));

      }
      it_sp++;
    }

    // run wedge sorters
    vector<L1MuBMWedgeSorter*>::iterator it_ws = m_wsvec.begin();
    while ( it_ws != m_wsvec.end() ) {
      if ( L1MuBMTFConfig::Debug(2) ) cout << "running Wedge Sorter "
                                           << (*it_ws)->id() << endl;
      if ( *it_ws ) (*it_ws)->run();
      if ( L1MuBMTFConfig::Debug(2) && *it_ws ) (*it_ws)->print();
      it_ws++;
    }

    // run muon sorter
    if ( L1MuBMTFConfig::Debug(2) ) cout << "running BM Muon Sorter" << endl;
    if ( m_ms ) m_ms->run();
    if ( L1MuBMTFConfig::Debug(2) && m_ms ) m_ms->print();


//cout <<m_ms->numberOfTracks()<<"<--------------------"<<endl;
    // store found track candidates in container (cache)
    if ( m_ms->numberOfTracks() > 0 ) {
      const vector<const L1MuBMTrack*>&  mttf_cont = m_ms->tracks();
      vector<const L1MuBMTrack*>::const_iterator iter;
      for ( iter = mttf_cont.begin(); iter != mttf_cont.end(); iter++ ) {
        //if ( *iter ) _cache.push_back(L1MuRegionalCand((*iter)->getDataWord(),(*iter)->bx()));

//        if ( *iter ) _cache.push_back(l1t::L1TRegionalMuonCandidate((*iter)->hwPt(),
//                                                               (*iter)->hwPhi(),
//                                                               (*iter)->hwEta(),
//                                                               (*iter)->hwSign(),
//                                                               (*iter)->hwSignValid(),
//                                                               (*iter)->hwQual(),
//                                                               (*iter)->bx()
//                                                               ));
//
        if ( *iter ){ _cache.push_back(l1t::L1TRegionalMuonCandidate( (*iter)->hwPt(),
                                                               (*iter)->hwPhi(),
                                                               (*iter)->hwEta(),
                                                               (*iter)->hwSign(),
                                                               (*iter)->hwSignValid(),
                                                               (*iter)->hwQual(),
							       (*iter)->spid().sector(),
							       l1t::tftype::bmtf,
                                                               (*iter)->bx()

));
//l1t::L1TRegionalMuonCandidate::setTFIdentifiers((*iter)->spid().sector(),l1t::tftype::bmtf );

}
      }
    }

  }

}


//
// reset MTTF
//
void L1MuBMTrackFinder::reset() {

  L1MuBMSecProcMap::SPmap_iter it_sp = m_spmap->begin();
  while ( it_sp != m_spmap->end() ) {
    if ( (*it_sp).second ) (*it_sp).second->reset();
    it_sp++;
  }

  vector<L1MuBMEtaProcessor*>::iterator it_ep = m_epvec.begin();
  while ( it_ep != m_epvec.end() ) {
    if ( *it_ep ) (*it_ep)->reset();
    it_ep++;
  }

  vector<L1MuBMWedgeSorter*>::iterator it_ws = m_wsvec.begin();
  while ( it_ws != m_wsvec.end() ) {
    if ( *it_ws ) (*it_ws)->reset();
    it_ws++;
  }

  if ( m_ms ) m_ms->reset();

}


//
// return Sector Processor container
//
const L1MuBMSectorProcessor* L1MuBMTrackFinder::sp(const L1MuBMSecProcId& id) const {

  return m_spmap->sp(id);

}


//
// return number of muon candidates found by the barrel MTTF
//
int L1MuBMTrackFinder::numberOfTracks() {

  return _cache.size();

}


L1MuBMTrackFinder::TFtracks_const_iter L1MuBMTrackFinder::begin() {

  return _cache.begin();

}


L1MuBMTrackFinder::TFtracks_const_iter L1MuBMTrackFinder::end() {

  return _cache.end();

}


void L1MuBMTrackFinder::clear() {

  _cache.clear();
  _cache0.clear();

}


//
// return number of muon candidates found by the barrel MTTF at a given bx
//
int L1MuBMTrackFinder::numberOfTracks(int bx) {

  int number = 0;
  for ( TFtracks_const_iter it  = _cache.begin(); it != _cache.end(); it++ ) {
    if ( (*it).bx() == bx ) number++;
  }

  return number;

}


// static data members

L1MuBMTFConfig* L1MuBMTrackFinder::m_config = 0;
