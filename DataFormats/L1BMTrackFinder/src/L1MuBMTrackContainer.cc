//-------------------------------------------------
//
//   Class L1MuBMTrackContainer
//
//   Description: output data for BMTF trigger
//
//
//   Author List: Jorge Troconiz  UAM Madrid
//
//
//--------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMTrackContainer.h"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//---------------
// C++ Headers --
//---------------
using namespace std;

//-------------------
// Initializations --
//-------------------


//----------------
// Constructors --
//----------------
L1MuBMTrackContainer::L1MuBMTrackContainer() {}

//--------------
// Destructor --
//--------------
L1MuBMTrackContainer::~L1MuBMTrackContainer() {}

//--------------
// Operations --
//--------------
void L1MuBMTrackContainer::setContainer(const TrackContainer& inputTracks) {

  dtTracks = inputTracks;
}

L1MuBMTrackContainer::TrackContainer const* L1MuBMTrackContainer::getContainer() const {
  return &dtTracks;
}

bool L1MuBMTrackContainer::bxEmpty(int step) const {

  bool empty = true;

  for ( Trackiterator i  = dtTracks.begin();
                      i != dtTracks.end();
                      i++ ) {
    if  (step == i->bx()) empty = false;
  }

  return(empty);
}

int L1MuBMTrackContainer::bxSize(int step1, int step2) const {

  int size = 0;

  for ( Trackiterator i  = dtTracks.begin();
                      i != dtTracks.end();
                      i++ ) {
    if  (step1 <= i->bx() && step2 >= i->bx()
//      && i->quality_packed() != 0) size++;
      && i->hwQual() != 0) size++;
  }

  return(size);
}

L1MuBMTrackCand const* L1MuBMTrackContainer::dtTrackCand1(int wheel, int sect, int step) const {

  L1MuBMTrackCand const* rT=0;

  for ( Trackiterator i  = dtTracks.begin();
                      i != dtTracks.end();
                      i++ ) {
    if  (step == i->bx() && wheel == i->whNum() && sect == i->scNum()
      && i->TrkTag() == 0)
      rT = &(*i);
  }

  return(rT);
}

L1MuBMTrackCand const* L1MuBMTrackContainer::dtTrackCand2(int wheel, int sect, int step) const {

  L1MuBMTrackCand const* rT=0;

  for ( Trackiterator i  = dtTracks.begin();
                      i != dtTracks.end();
                      i++ ) {
    if  (step == i->bx() && wheel == i->whNum() && sect == i->scNum()
      && i->TrkTag() == 1)
      rT = &(*i);
  }

  return(rT);
}
