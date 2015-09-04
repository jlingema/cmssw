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
#ifndef L1MuBMChambPhDigi_H
#define L1MuBMChambPhDigi_H

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

class L1MuBMChambPhDigi {

 public:

  //  Constructors
  L1MuBMChambPhDigi();

  L1MuBMChambPhDigi( int ubx, int uwh, int usc, int ust,
	int uphr, int uphb, int uqua, int utag, int ucnt );

  //  Destructor
  ~L1MuBMChambPhDigi();

  // Operations
  int bxNum()       const;
  int whNum()       const;
  int scNum()       const;
  int stNum()       const;
  int phi()         const;
  int phiB()        const;
  int code()        const;
  int Ts2Tag()      const;
  int BxCnt()       const;

 private:

  int bx;
  int wheel;
  int sector;
  int station;
  int radialAngle;
  int bendingAngle;
  int qualityCode;
  int Ts2TagCode;
  int BxCntCode;
};

#endif
