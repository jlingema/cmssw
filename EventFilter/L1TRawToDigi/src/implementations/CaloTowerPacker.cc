#include "DataFormats/L1TCalorimeter/interface/CaloTower.h"
#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "EventFilter/L1TRawToDigi/interface/L1TDigiToRaw.h"

#include "CaloTowerPacker.h"

namespace l1t {
   class CaloTowerPacker : public BasePacker {
      public:
         CaloTowerPacker(const edm::ParameterSet&);
         virtual Blocks pack(const edm::Event&);
         virtual void fetchToken(L1TDigiToRaw*);
      private:
         edm::InputTag towerTag_;
         edm::EDGetToken towerToken_;
   };

   PackerList
   CaloTowerPackerFactory::create(const edm::ParameterSet& cfg, const FirmwareVersion& fw, const int fedid)
   {
      return {std::shared_ptr<BasePacker>(new CaloTowerPacker(cfg))};
   }

   CaloTowerPacker::CaloTowerPacker(const edm::ParameterSet& cfg) :
      towerTag_(cfg.getParameter<edm::InputTag>("CaloTowers"))
   {
   }

   Blocks
   CaloTowerPacker::pack(const edm::Event& event)
   {

      edm::Handle<CaloTowerBxCollection> towers;
      event.getByToken(towerToken_, towers);

      Blocks res;

      for (int i = towers->getFirstBX(); i <= towers->getLastBX(); ++i) {

        for (int phi = 1; phi <=72; phi=phi+2) { // Two phi values per link

          Block blk;
          blk.id = 2*phi - 2; // Block IDs start at zero and span even numbers up to 142

          for (int eta = 1; eta <=41; eta++) { // This is abs(eta) since +/- eta are interleaved in time

            // Get four towers +/- eta and phi and phi+1 to all be packed in this loop
            l1t::CaloTower t1 = towers->at(i,l1t::CaloTools::caloTowerHash(eta,phi));
            l1t::CaloTower t2 = towers->at(i,l1t::CaloTools::caloTowerHash(eta,phi+1));
            l1t::CaloTower t3 = towers->at(i,l1t::CaloTools::caloTowerHash(-1*eta,phi));
            l1t::CaloTower t4 = towers->at(i,l1t::CaloTools::caloTowerHash(-1*eta,phi+1));

            // Merge phi and phi+1 into one block (phi is LSW, phi+1 is MSW)
            uint32_t word1 = \
	      (t1.hwPt() & 0x1FF) |
	      (t1.hwEtRatio() & 0x7) << 9 |
	      (t1.hwQual() & 0xF) << 12;

	    word1 = word1 |
	      (t2.hwPt() & 0x1FF) << 16 |
	      (t2.hwEtRatio() & 0x7) << 25 |
	      (t2.hwQual() & 0xF) << 28;

            blk.load.push_back(word1);
            
            // Do it all again for -eta

            uint32_t word2 = \
              (t3.hwPt() & 0x1FF) |
              (t3.hwEtRatio() & 0x7) << 9 |
              (t3.hwQual() & 0xF) << 12;

            word2 = word2 |
              (t4.hwPt() & 0x1FF) << 16 |
              (t4.hwEtRatio() & 0x7) << 25 |
              (t4.hwQual() & 0xF) << 28;

            blk.load.push_back(word2);

          }

          res.push_back(blk);

        }
      }
      
      return res;
   }
  
   void
   CaloTowerPacker::fetchToken(L1TDigiToRaw* digi2raw)
   {
      towerToken_ = digi2raw->consumes<CaloTowerBxCollection>(towerTag_);
   }
}
