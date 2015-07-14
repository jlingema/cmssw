//-------------------------------------------------
//
//   Class L1MuBMTrackCand
//
//   Description: output data for BMTF trigger
//
//
//   Author List: Jorge Troconiz  UAM Madrid
//
//
//--------------------------------------------------
#ifndef L1MuBMTrackCand_H
#define L1MuBMTrackCand_H

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


//----------------------
// Base Class Headers --
//----------------------

//#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1TMuon/interface/L1TRegionalMuonCandidate.h"

//---------------
// C++ Headers --
//---------------

//              ---------------------
//              -- Class Interface --
//              ---------------------

class L1MuBMTrackCand: public l1t::L1TRegionalMuonCandidate {

 public:

  //  Constructors
  L1MuBMTrackCand();

  L1MuBMTrackCand( unsigned dataword, int bx, int uwh, int usc, int utag,
                   int adr1, int adr2, int adr3, int adr4, int utc );

  L1MuBMTrackCand( unsigned type_idx, unsigned phi, unsigned eta, unsigned pt, unsigned charge,
		   unsigned ch_valid, unsigned finehalo, unsigned quality, int bx,
                   int uwh, int usc, int utag, int adr1, int adr2, int adr3, int adr4 );
L1MuBMTrackCand( int pt, int phi, int eta, int charge, int quality, int bx,
                                  int uwh, int usc, int utag,
                                  int adr1, int adr2, int adr3, int adr4, int utc ) ;
  //  Destructor
  ~L1MuBMTrackCand();

  // Operations
  int whNum()        const;
  int scNum()        const;
  int stNum(int ust) const;
  int TCNum()        const;
  int TrkTag()       const;

  void setTC();
  void setAdd(int ust);

 private:

  int wheel;
  int sector;
  int TrkTagCode;
  int TClassCode;
  int TrkAdd[4];

};

#endif
