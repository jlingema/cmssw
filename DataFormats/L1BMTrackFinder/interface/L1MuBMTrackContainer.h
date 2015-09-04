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
#ifndef L1MuBMTrackContainer_H
#define L1MuBMTrackContainer_H

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "DataFormats/L1BMTrackFinder/interface/L1MuBMTrackCand.h"

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


class L1MuBMTrackContainer {

 public:

  typedef std::vector<L1MuBMTrackCand>    TrackContainer;
  typedef TrackContainer::const_iterator  Trackiterator;
  typedef TrackContainer::iterator        TrackIterator;

  //  Constructors
  L1MuBMTrackContainer();

  //  Destructor
  ~L1MuBMTrackContainer();

  void setContainer(const TrackContainer& inputTracks);

  TrackContainer const* getContainer() const;

  bool bxEmpty(int step) const;

  int bxSize(int step1, int step2) const;

  L1MuBMTrackCand const* dtTrackCand1(int wheel, int sect, int bx) const;

  L1MuBMTrackCand const* dtTrackCand2(int wheel, int sect, int bx) const;


 private:

  TrackContainer dtTracks;

};

#endif
