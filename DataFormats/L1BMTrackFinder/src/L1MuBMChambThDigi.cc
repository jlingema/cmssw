//-------------------------------------------------
//
//   Class L1MuBMChambThDigi
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
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMChambThDigi.h"

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
L1MuBMChambThDigi::L1MuBMChambThDigi() {

  bx              = -100;
  wheel           = 0;
  sector          = 0;
  station         = 0;

  for(int i=0;i<7;i++) {
    m_outPos[i] = 0;
    m_outQual[i] = 0;
  }
}

L1MuBMChambThDigi::L1MuBMChambThDigi( int ubx, int uwh, int usc, int ust,
                                      int* upos, int* uqual ) {

  bx              = ubx;
  wheel           = uwh;
  sector          = usc;
  station         = ust;

  for(int i=0;i<7;i++) {
    m_outPos[i] = upos[i];
    m_outQual[i] = uqual[i];
  }
}

L1MuBMChambThDigi::L1MuBMChambThDigi( int ubx, int uwh, int usc, int ust,
                                      int* upos ) {

  bx              = ubx;
  wheel           = uwh;
  sector          = usc;
  station         = ust;

  for(int i=0;i<7;i++) {
    m_outPos[i] = upos[i];
    m_outQual[i] = 0;
  }
}

//--------------
// Destructor --
//--------------
L1MuBMChambThDigi::~L1MuBMChambThDigi() {
}

//--------------
// Operations --
//--------------
int L1MuBMChambThDigi::bxNum() const {
  return bx;
}

int L1MuBMChambThDigi::whNum() const {
  return wheel;
}
int L1MuBMChambThDigi::scNum() const {
  return sector;
}
int L1MuBMChambThDigi::stNum() const {
  return station;
}

int L1MuBMChambThDigi::code(const int i) const {
  if (i<0||i>=7) return 0;

  return (int)(m_outPos[i]+m_outQual[i]);
}

int L1MuBMChambThDigi::position(const int i) const {
  if (i<0||i>=7) return 0;

  return (int)m_outPos[i];
}

int L1MuBMChambThDigi::quality(const int i) const {
  if (i<0||i>=7) return 0;

  return (int)m_outQual[i];
}
