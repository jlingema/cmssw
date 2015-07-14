//-------------------------------------------------
//
//   Class L1MuBMChambThContainer
//
//   Description: input data for ETTF trigger
//
//
//   Author List: Jorge Troconiz  UAM Madrid
//
//
//--------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMChambThContainer.h"

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
L1MuBMChambThContainer::L1MuBMChambThContainer() {}

//--------------
// Destructor --
//--------------
L1MuBMChambThContainer::~L1MuBMChambThContainer() {}

//--------------
// Operations --
//--------------
void L1MuBMChambThContainer::setContainer(const The_Container& inputSegments) {

  theSegments = inputSegments;
}

L1MuBMChambThContainer::The_Container const* L1MuBMChambThContainer::getContainer() const {
  return &theSegments;
}

bool L1MuBMChambThContainer::bxEmpty(int step) const {

  bool empty = true;

  for ( The_iterator i  = theSegments.begin();
                     i != theSegments.end();
                     i++ ) {
    if  (step == i->bxNum()) empty = false;
  }

  return(empty);
}

int L1MuBMChambThContainer::bxSize(int step1, int step2) const {

  int size = 0;

  for ( The_iterator i  = theSegments.begin();
                     i != theSegments.end();
                     i++ ) {
    if  (step1 <= i->bxNum() && step2 >= i->bxNum()) size++;
  }

  return(size);
}

L1MuBMChambThDigi const* L1MuBMChambThContainer::chThetaSegm(int wheel, int stat, int sect, int step) const {

  L1MuBMChambThDigi const* rT=0;

  for ( The_iterator i  = theSegments.begin();
                     i != theSegments.end();
                     i++ ) {
    if  (step == i->bxNum() && wheel == i->whNum() && sect == i->scNum()
      && stat == i->stNum() )
      rT = &(*i);
  }

  return(rT);
}

