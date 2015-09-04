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
#ifndef L1MuBMChambThContainer_H
#define L1MuBMChambThContainer_H

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMChambThDigi.h"

//----------------------
// Base Class Headers --
//----------------------
#include <vector>

//---------------
// C++ Headers --
//---------------


//              ---------------------
//              -- Class Interface --
//              ---------------------

class L1MuBMChambThContainer {

 public:

  typedef std::vector<L1MuBMChambThDigi>  The_Container;
  typedef The_Container::const_iterator   The_iterator;

  //  Constructors
  L1MuBMChambThContainer();

  //  Destructor
  ~L1MuBMChambThContainer();

  void setContainer(const The_Container& inputSegments);

  The_Container const* getContainer() const;

  bool bxEmpty(int step) const;

  int bxSize(int step1, int step2) const;

  L1MuBMChambThDigi const* chThetaSegm(int wheel, int stat, int sect, int bx) const;

 private:

  The_Container theSegments; 

};

#endif
