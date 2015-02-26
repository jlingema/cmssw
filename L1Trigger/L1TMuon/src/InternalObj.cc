#include "L1Trigger/L1TMuon/interface/InternalObj.h"

std::ostream & operator<< (std::ostream &out, const InternalObj &o){
  out<<"InternalObj: ";
  out <<" pt: "<<o.pt<<", eta: "<<o.eta<<", phi: "<<o.phi
      <<", q: "<<o.q<<", bx: "<<o.bx
      <<", disc: "<<o.disc<<" refLayer: "<<o.refLayer;
  
  return out;
}

