//-------------------------------------------------
//
//   Class L1MuBMChambPhContainer
//
//   Description: input data for PHTF trigger
//
//
//   Author List: Jorge Troconiz  UAM Madrid
//
//
//--------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMChambPhContainer.h"

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
L1MuBMChambPhContainer::L1MuBMChambPhContainer() {}

//--------------
// Destructor --
//--------------
L1MuBMChambPhContainer::~L1MuBMChambPhContainer() {}

//--------------
// Operations --
//--------------
void L1MuBMChambPhContainer::setContainer(const Phi_Container& inputSegments) {

  phiSegments = inputSegments;
}

L1MuBMChambPhContainer::Phi_Container const* L1MuBMChambPhContainer::getContainer() const {
  return &phiSegments;
}

bool L1MuBMChambPhContainer::bxEmpty(int step) const {

  bool empty = true;

  for ( Phi_iterator i  = phiSegments.begin();
                     i != phiSegments.end();
                     i++ ) {
    if  (step == i->bxNum()) empty = false;
  }

  return(empty);
}

int L1MuBMChambPhContainer::bxSize(int step1, int step2) const {

  int size = 0;

  for ( Phi_iterator i  = phiSegments.begin();
                     i != phiSegments.end();
                     i++ ) {
    if  (step1 <= i->bxNum() && step2 >= i->bxNum() 
      && i->Ts2Tag() == 0 && i->code() != 7) size++;
    if  (step1 <= i->bxNum()-1 && step2 >= i->bxNum()-1 
      && i->Ts2Tag() == 1 && i->code() != 7) size++;
  }

  return(size);
}

L1MuBMChambPhDigi const* L1MuBMChambPhContainer::chPhiSegm1(int wheel, int stat, int sect, int step) const {

  L1MuBMChambPhDigi const* rT=0;

  for ( Phi_iterator i  = phiSegments.begin();
                     i != phiSegments.end();
                     i++ ) {
    if  (step == i->bxNum() && wheel == i->whNum() && sect == i->scNum()
      && stat == i->stNum() && i->Ts2Tag() == 0)
      rT = &(*i);
  }

  return(rT);
}

L1MuBMChambPhDigi const* L1MuBMChambPhContainer::chPhiSegm2(int wheel, int stat, int sect, int step) const {

  L1MuBMChambPhDigi const* rT=0;

  for ( Phi_iterator i  = phiSegments.begin();
                     i != phiSegments.end();
                     i++ ) {
    if  (step == i->bxNum()-1 && wheel == i->whNum() && sect == i->scNum()
      && stat == i->stNum() && i->Ts2Tag() == 1)
      rT = &(*i);
  }

  return(rT);
}
