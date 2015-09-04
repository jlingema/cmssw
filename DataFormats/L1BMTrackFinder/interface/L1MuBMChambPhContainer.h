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
#ifndef L1MuBMChambPhContainer_H
#define L1MuBMChambPhContainer_H

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMChambPhDigi.h"

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


class L1MuBMChambPhContainer {

 public:

  typedef std::vector<L1MuBMChambPhDigi>  Phi_Container;
  typedef Phi_Container::const_iterator   Phi_iterator;

  //  Constructors
  L1MuBMChambPhContainer();

  //  Destructor
  ~L1MuBMChambPhContainer();

  void setContainer(const Phi_Container& inputSegments);

  Phi_Container const* getContainer() const;

  bool bxEmpty(int step) const;

  int bxSize(int step1, int step2) const;

  L1MuBMChambPhDigi const* chPhiSegm1(int wheel, int stat, int sect, int bx) const;

  L1MuBMChambPhDigi const* chPhiSegm2(int wheel, int stat, int sect, int bx) const;

 private:

  Phi_Container phiSegments; 

};

#endif
