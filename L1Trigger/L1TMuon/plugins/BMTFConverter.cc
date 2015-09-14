// -*- C++ -*-
//
// Package:    BMTFConverter
// Class:      BMTFConverter
//
/**\class BMTFConverter BMTFConverter.cc L1Trigger/L1TGlobalMuon/plugins/BMTFConverter.cc

 Description: Takes txt-file input and produces barrel- / overlap- / forward TF muons

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

#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"

#include <iostream>
//
// class declaration
//
namespace l1t {
class BMTFConverter : public edm::EDProducer {
   public:
      explicit BMTFConverter(const edm::ParameterSet&);
      ~BMTFConverter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      // ----------member data ---------------------------
      edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> m_barrelTfInputToken;
      edm::InputTag m_barrelTfInputTag;
      std::map<int, int> ptMap_;
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
BMTFConverter::BMTFConverter(const edm::ParameterSet& iConfig) : m_barrelTfInputTag("bmtfEmulator", "BM")
{
  m_barrelTfInputToken = consumes<l1t::RegionalMuonCandBxCollection>(m_barrelTfInputTag);
  //register your products
  produces<RegionalMuonCandBxCollection>("ConvBMTFMuons");
  ptMap_[0] = 0;
  ptMap_[1] = 0;
  ptMap_[2] = 3;
  ptMap_[3] = 4;
  ptMap_[4] = 5;
  ptMap_[5] = 6;
  ptMap_[6] = 7;
  ptMap_[7] = 8;
  ptMap_[8] = 9;
  ptMap_[9] = 10;
  ptMap_[10] = 12;
  ptMap_[11] = 14;
  ptMap_[12] = 16;
  ptMap_[13] = 20;
  ptMap_[14] = 24;
  ptMap_[15] = 28;
  ptMap_[16] = 32;
  ptMap_[17] = 36;
  ptMap_[18] = 40;
  ptMap_[19] = 50;
  ptMap_[20] = 60;
  ptMap_[21] = 70;
  ptMap_[22] = 80;
  ptMap_[23] = 90;
  ptMap_[24] = 100;
  ptMap_[25] = 120;
  ptMap_[26] = 140;
  ptMap_[27] = 160;
  ptMap_[28] = 180;
  ptMap_[29] = 200;
  ptMap_[30] = 240;
  ptMap_[31] = 280;
}


BMTFConverter::~BMTFConverter()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//


// ------------ method called to produce the data  ------------
void
BMTFConverter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  std::auto_ptr<l1t::RegionalMuonCandBxCollection> convMuons (new l1t::RegionalMuonCandBxCollection());

  Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
  iEvent.getByToken(m_barrelTfInputToken, bmtfMuons);
  int minBX = bmtfMuons->getFirstBX();
  int maxBX = bmtfMuons->getLastBX();
  for (int bx = minBX; bx <= maxBX; ++bx) {
    for (auto mu = bmtfMuons->begin(bx); mu != bmtfMuons->end(bx); ++mu) {
      // if (bx != 0) { std::cout << "found bmtf in bx " << bx << std::endl; }
      l1t::RegionalMuonCand convMu((*mu));
      // int convPt = ptMap_.at(mu->hwPt());
      // int convPhi = (mu->hwPhi() * 4) - (mu->processor() * 48);
      // int convEta = getSigned(mu->hwEta())*3.54;
      int convEta = (mu->hwEta() - 32)*3.54;
      // convMu.setHwPt(convPt);
      // convMu.setHwPhi(convPhi);
      convMu.setHwEta(convEta);
      // convMu.setTFIdentifiers(mu->processor()+1, mu->trackFinderType());
      convMuons->push_back(0, convMu);
    }
  }

  iEvent.put(convMuons, "ConvBMTFMuons");
}

// ------------ method called once each job just before starting event loop  ------------
void
BMTFConverter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
BMTFConverter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void
BMTFConverter::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
BMTFConverter::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
BMTFConverter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
BMTFConverter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BMTFConverter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
}
//define this as a plug-in
DEFINE_FWK_MODULE(l1t::BMTFConverter);
