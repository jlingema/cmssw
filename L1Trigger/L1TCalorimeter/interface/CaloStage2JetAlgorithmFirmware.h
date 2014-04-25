///
/// Description: Firmware headers
///
/// Implementation:
///    Concrete firmware implementations
///
/// \author: Jim Brooke - University of Bristol
/// Modified: Adam Elwood - ICL

//
//

#ifndef CaloStage2JetAlgorithmFirmware_H
#define CaloStage2JetAlgorithmFirmware_H

#include "L1Trigger/L1TCalorimeter/interface/CaloStage2JetAlgorithm.h"
#include "CondFormats/L1TObjects/interface/CaloParams.h"

namespace l1t {

  // Imp1 is for v1 and v2
  class CaloStage2JetAlgorithmFirmwareImp1 : public CaloStage2JetAlgorithm {
  public:
    CaloStage2JetAlgorithmFirmwareImp1(CaloParams* params);
    virtual ~CaloStage2JetAlgorithmFirmwareImp1();
    virtual void processEvent(const std::vector<CaloTower> & towers,
			      std::vector<Jet> & jets);

    void create(const std::vector<CaloTower> & towers,
		std::vector<Jet> & jets);
    
    void filter(std::vector<Jet> & jets);
    
    void sort(std::vector<Jet> & jets);

    bool jetIsZero(l1t::Jet jet);

  private:

    CaloParams* const params_;
    
  };

  // Imp2 is Jad's algorithm with possible donut subtraction
  class CaloStage2JetAlgorithmFirmwareImp2 : public CaloStage2JetAlgorithm {
  public:
    CaloStage2JetAlgorithmFirmwareImp2(CaloParams* params);
    virtual ~CaloStage2JetAlgorithmFirmwareImp2();
    virtual void processEvent(const std::vector<CaloTower> & towers,
			      std::vector<Jet> & jets);

    void create(const std::vector<CaloTower> & towers,
		std::vector<Jet> & jets);

    void getRing(int32_t shift, std::vector<std::vector<int> > & ring, 
        const std::vector<CaloTower> & towers, std::vector<Jet> & jets);
    
    void filter(std::vector<Jet> & jets);
    
    void sort(std::vector<Jet> & jets);

    bool jetIsZero(l1t::Jet jet);

  private:

    CaloParams* const params_;
    int etaMin_ = -27;
    int etaMax_ = 27;
    int phiMin_ = 1;
    int phiMax_ = 72;
    
  };

}

#endif
