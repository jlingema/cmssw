//-------------------------------------------------
//
//   Class L1MuBMChambPhDigi
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
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMChambPhDigi.h"

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
L1MuBMChambPhDigi::L1MuBMChambPhDigi() {

  bx              = -100;
  wheel           = 0;
  sector          = 0;
  station         = 0;
  radialAngle     = 0;
  bendingAngle    = 0;
  qualityCode     = 7;
  Ts2TagCode      = 0;
  BxCntCode       = 0;
}

L1MuBMChambPhDigi::L1MuBMChambPhDigi( int ubx, int uwh, int usc, int ust,
                         int uphr, int uphb, int uqua, int utag, int ucnt ) {

  bx              = ubx;
  wheel           = uwh;
  sector          = usc;
  station         = ust;
  radialAngle     = uphr;
  bendingAngle    = uphb;
  qualityCode     = uqua;
  Ts2TagCode      = utag;
  BxCntCode       = ucnt;
}

//--------------
// Destructor --
//--------------
L1MuBMChambPhDigi::~L1MuBMChambPhDigi() {
}

//--------------
// Operations --
//--------------
int L1MuBMChambPhDigi::bxNum() const {
  return bx;
}

int L1MuBMChambPhDigi::whNum() const {
  return wheel;
}
int L1MuBMChambPhDigi::scNum() const {
  return sector;
}
int L1MuBMChambPhDigi::stNum() const {
  return station;
}

int L1MuBMChambPhDigi::phi() const {
  return radialAngle;
}

int L1MuBMChambPhDigi::phiB() const {
  return bendingAngle;
}

int L1MuBMChambPhDigi::code() const {
  return qualityCode;
}

int L1MuBMChambPhDigi::Ts2Tag() const {
  return Ts2TagCode;
}

int L1MuBMChambPhDigi::BxCnt() const {
  return BxCntCode;
}
