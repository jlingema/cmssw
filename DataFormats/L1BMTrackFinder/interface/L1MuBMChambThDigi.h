//-------------------------------------------------
//
//   Class L1MuBMChambThDigi	
//
//   Description: input data for ETTF trigger
//
//
//   Author List: Jorge Troconiz  UAM Madrid
//
//
//--------------------------------------------------
#ifndef L1MuBMChambThDigi_H
#define L1MuBMChambThDigi_H

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


//----------------------
// Base Class Headers --
//----------------------


//---------------
// C++ Headers --
//---------------


//              ---------------------
//              -- Class Interface --
//              ---------------------

typedef unsigned char myint8;

class L1MuBMChambThDigi {

 public:

  //  Constructors
  L1MuBMChambThDigi();

  L1MuBMChambThDigi( int ubx, int uwh, int usc, int ust,
	             int* uos, int* uqual );

  L1MuBMChambThDigi( int ubx, int uwh, int usc, int ust,
	             int* uos );

  //  Destructor
  ~L1MuBMChambThDigi();

  // Operations
  int bxNum()       const;
  int whNum()       const;
  int scNum()       const;
  int stNum()       const;

  int code(const int i) const;
  int position(const int i) const;
  int quality(const int i) const;

 private:

  int bx;
  int wheel;
  int sector;
  int station;

  myint8 m_outPos[7];
  myint8 m_outQual[7];
};

#endif
