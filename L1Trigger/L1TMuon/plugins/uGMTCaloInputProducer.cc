// -*- C++ -*-
//
// Package:    uGMTCaloInputProducer
// Class:      uGMTCaloInputProducer
//
/**\class uGMTCaloInputProducer uGMTCaloInputProducer.cc L1Trigger/L1TGlobalMuon/plugins/uGMTCaloInputProducer.cc

 Description: takes generated muons and fills them in the expected collections for the uGMT

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Joschka Philip Lingemann,40 3-B01,+41227671598,
//         Created:  Thu Oct  3 10:12:30 CEST 2013
// $Id$
//
//


// system include files
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/L1TMuon/interface/L1TGMTInputCaloSumFwd.h"
#include "DataFormats/L1TMuon/interface/L1TGMTInputCaloSum.h"
#include "DataFormats/L1TCalorimeter/interface/CaloTower.h"

#include "TMath.h"
#include "TRandom3.h"

//
// class declaration
//
namespace l1t {
class uGMTCaloInputProducer : public edm::EDProducer {
   public:
      explicit uGMTCaloInputProducer(const edm::ParameterSet&);
      ~uGMTCaloInputProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      edm::EDGetTokenT <l1t::CaloTowerBxCollection> m_caloTowerToken;
      edm::InputTag m_caloLabel;

};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
uGMTCaloInputProducer::uGMTCaloInputProducer(const edm::ParameterSet& iConfig) {
  //register your inputs:
  m_caloLabel = iConfig.getParameter<edm::InputTag> ("caloStage2Layer2Label");
  m_caloTowerToken = consumes <l1t::CaloTowerBxCollection> (m_caloLabel);
  //register your products
  produces<L1TGMTInputCaloSumCollection>("TriggerTowerSums");
  produces<L1TGMTInputCaloSumCollection>("TriggerTower2x2s");
}


uGMTCaloInputProducer::~uGMTCaloInputProducer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//


// ------------ method called to produce the data  ------------
void
uGMTCaloInputProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  std::auto_ptr<l1t::L1TGMTInputCaloSumCollection> towerSums (new L1TGMTInputCaloSumCollection());
  std::auto_ptr<l1t::L1TGMTInputCaloSumCollection> tower2x2s (new L1TGMTInputCaloSumCollection());

  edm::Handle<l1t::CaloTowerBxCollection> caloTowers;
  // Make sure that you can get genParticles
  std::map<int, l1t::L1TGMTInputCaloSum> sums;
  std::map<int, l1t::L1TGMTInputCaloSum> regs;

  if (iEvent.getByToken(m_caloTowerToken, caloTowers)) {
    for (auto it = caloTowers->begin(0); it != caloTowers->end(0); ++it) {
      const l1t::CaloTower& twr = *it;
      if (std::abs(twr.hwEta()) > 27) {
        continue;
      }
      int ieta2x2 = (twr.hwEta() + 27) / 2;
      int iphi2x2 = twr.hwPhi() / 2;
      int muon_idx = iphi2x2 * 28 + ieta2x2;
      if (regs.count(muon_idx) == 0) {
        regs[muon_idx] = l1t::L1TGMTInputCaloSum(twr.hwPt(), iphi2x2, ieta2x2, muon_idx);
      } else {
        regs.at(muon_idx).setEtBits(regs.at(muon_idx).etBits() + twr.hwPt());
      }

      // std::cout << "iphi; phi " << twr.hwPhi() << "; " << twr.phi() << " .. ieta; eta" << twr.hwEta() << "; " << twr.eta() << std::endl;

      for (int ieta = -27; ieta < 28; ++ieta) {
        for (int iphi = 0; iphi < 72; ++iphi) {
          int deta = std::abs(ieta - twr.hwEta());
          int dphi = iphi - twr.hwPhi();
          if (dphi > 36) {
            dphi -= 72;
          }
          if (dphi < -36) {
            dphi += 72;
          }
          dphi = std::abs(dphi);
          if (deta <= 4 && dphi <= 4) {
            int ietamu = (ieta + 27) / 2;
            int iphimu = iphi / 2;
            int idxmu = iphimu * 28 + ietamu;
            if (sums.count(idxmu) == 0) {
              sums[idxmu] = l1t::L1TGMTInputCaloSum(twr.hwPt(), iphimu, ietamu, idxmu);
            } else {
              sums.at(idxmu).setEtBits(sums.at(idxmu).etBits() + twr.hwPt());
            }
          }

        }
      }
    }
  } else {
    LogError("GlobalMuon") << "CaloTowers not found." << std::endl;
  }

  for (auto it = sums.begin(); it != sums.end(); ++it) {
    if (it->second.etBits() > 0) {
      l1t::L1TGMTInputCaloSum sum = l1t::L1TGMTInputCaloSum(it->second);
      // convert Et to correct scale:
      if (sum.etBits() > 31) {
        sum.setEtBits(31);
      }
      towerSums->push_back(sum);
    }
  }
  for (auto it = regs.begin(); it != regs.end(); ++it) {
    if (it->second.etBits() > 0) {
      tower2x2s->push_back(it->second);
    }
  }

  iEvent.put(towerSums, "TriggerTowerSums");
  iEvent.put(tower2x2s, "TriggerTower2x2s");

}

// ------------ method called once each job just before starting event loop  ------------
void
uGMTCaloInputProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
uGMTCaloInputProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void
uGMTCaloInputProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
uGMTCaloInputProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
uGMTCaloInputProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
uGMTCaloInputProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
uGMTCaloInputProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
}
//define this as a plug-in
DEFINE_FWK_MODULE(l1t::uGMTCaloInputProducer);
