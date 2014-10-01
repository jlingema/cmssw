#ifndef __l1microgmtextrapolationlut_h
#define __l1microgmtextrapolationlut_h

#include "MicroGMTLUT.h"
#include "MicroGMTConfiguration.h"

namespace l1t {
  class MicroGMTExtrapolationLUT : MicroGMTLUT {
    public: 
      MicroGMTExtrapolationLUT (const edm::ParameterSet& iConfig, const std::string& setName, int type);
      MicroGMTExtrapolationLUT (const edm::ParameterSet& iConfig, const char* setName, int type);
      virtual ~MicroGMTExtrapolationLUT ();



      // returns the index corresponding to the calo tower sum 
      int lookup(int charge, int angle, int pt) const;
      
      int hashInput(int charge, int angle, int pt) const;
      void unHashInput(int input, int& charge, int& angle, int& pt) const;
    private:
      void getParameters(const edm::ParameterSet& iConfig, const char* setName, int type);

      int m_chrgInWidth;
      int m_angleInWidth;
      int m_ptRedInWidth;

      int m_chrgMask;
      int m_angleMask;
      int m_ptRedMask;
  };
}
#endif /* defined(__l1microgmtextrapolationlut_h) */