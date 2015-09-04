#include <DataFormats/L1BMTrackFinder/interface/L1MuBMChambPhDigi.h>
#include <DataFormats/L1BMTrackFinder/interface/L1MuBMChambThDigi.h>
#include <DataFormats/L1BMTrackFinder/interface/L1MuBMTrackCand.h>
#include <DataFormats/L1BMTrackFinder/interface/L1MuBMChambPhContainer.h>
#include <DataFormats/L1BMTrackFinder/interface/L1MuBMChambThContainer.h>
#include <DataFormats/L1BMTrackFinder/interface/L1MuBMTrackContainer.h>
#include <DataFormats/Common/interface/Wrapper.h>

namespace DataFormats_L1BMTrackFinder {
  struct dictionary {
    L1MuBMChambPhDigi ph_S;
    L1MuBMChambThDigi th_S;
    L1MuBMTrackCand   tr_S;

    std::vector<L1MuBMChambPhDigi> ph_V;
    std::vector<L1MuBMChambThDigi> th_V;
    std::vector<L1MuBMTrackCand>   tr_V;

    L1MuBMChambPhContainer ph_K;
    L1MuBMChambThContainer th_K;
    L1MuBMTrackContainer   tr_K;

    edm::Wrapper<L1MuBMChambPhContainer> ph_W;
    edm::Wrapper<L1MuBMChambThContainer> th_W;
    edm::Wrapper<L1MuBMTrackContainer>   tr_W;
  };
}
