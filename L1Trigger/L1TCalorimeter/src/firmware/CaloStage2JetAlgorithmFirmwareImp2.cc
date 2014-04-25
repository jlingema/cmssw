///
/// \class l1t::CaloStage2JetAlgorithmFirmwareImp2
///
/// \author: Adam Elwood and Matthew Citron
///
/// Description: Implementation of Jad's asymmetric map overlap algorithm with donut subtraction

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/CaloStage2JetAlgorithmFirmware.h"

#include "DataFormats/Math/interface/LorentzVector.h"

#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"

#include "CondFormats/L1TObjects/interface/CaloParams.h"

#include <vector>

l1t::CaloStage2JetAlgorithmFirmwareImp2::CaloStage2JetAlgorithmFirmwareImp2(CaloParams* params) :
  params_(params)
{


}


l1t::CaloStage2JetAlgorithmFirmwareImp2::~CaloStage2JetAlgorithmFirmwareImp2() {


}


void l1t::CaloStage2JetAlgorithmFirmwareImp2::processEvent(const std::vector<l1t::CaloTower> & towers,
    std::vector<l1t::Jet> & jets) {


  // find all possible jets
  create(towers, jets);

  // remove overlaps
  filter(jets);

  // sort
  sort(jets);

}


void l1t::CaloStage2JetAlgorithmFirmwareImp2::create(const std::vector<l1t::CaloTower> & towers,
      std::vector<l1t::Jet> & jets) {


   // generate jet mask
   // needs to be configurable at some point
   // just a square for now
   // for 1 do greater than, for 2 do greater than equal to
  int mask[9][9] = {
    { 1,1,1,1,1,1,1,1,1 },
    { 2,1,1,1,1,1,1,1,1 },
    { 2,2,1,1,1,1,1,1,1 },
    { 2,2,2,1,1,1,1,1,1 },
    { 2,2,2,2,0,1,1,1,1 },
    { 2,2,2,2,2,2,1,1,1 },
    { 2,2,2,2,2,2,2,1,1 },
    { 2,2,2,2,2,2,2,2,1 },
    { 2,2,2,2,2,2,2,2,2 }
  };


  // loop over jet positions
  for ( int ieta = etaMin_ ; ieta < etaMax_+1 ; ++ieta ) {
    if (ieta==0) continue;
    for ( int iphi = phiMin_ ; iphi < phiMax_+1 ; ++iphi ) {

      int iEt(0);
      const CaloTower& tow = CaloTools::getTower(towers, ieta, iphi); 
      int seedEt = tow.hwEtEm();
      seedEt += tow.hwEtHad();
      bool isMax(true);

      // loop over towers in this jet
      for( int deta = -4; deta < 5; ++deta ) {
	for( int dphi = -4; dphi < 5; ++dphi ) {

	  int towEt = 0;
	  int ietaTest = ieta+deta;
	  int iphiTest = iphi+dphi;

	  //Wrap around phi
	  if(iphiTest > phiMax_){
	    iphiTest = iphiTest -phiMax_ +phiMin_ -1;
	  }
	  else if(iphiTest < phiMin_){
	    iphiTest = iphiTest -phiMin_ +phiMax_ +1 ;
	  }

	  // check jet mask and sum tower et
	  // re-use calo tools sum method, but for single tower
	  if( mask[deta+4][dphi+4] == 1 ) { //Do greater than
	    if(ietaTest < etaMax_ || ietaTest > etaMin_){ //Only check if in the eta range
	      const CaloTower& tow = CaloTools::getTower(towers, ietaTest, iphiTest); 
	      towEt = tow.hwEtEm() + tow.hwEtHad();
	      iEt+=towEt;
	    }
	    isMax=(seedEt>towEt);
	  }
	  else if( mask[deta+4][dphi+4] == 2 ) { //Do greater than equal to
	    if(ietaTest < etaMax_ || ietaTest > etaMin_){ //Only check if in the eta range
	      const CaloTower& tow = CaloTools::getTower(towers, ietaTest, iphiTest); 
	      int towEt = tow.hwEtEm() + tow.hwEtHad();
	      iEt+=towEt;
	    }
	    isMax=(seedEt>=towEt);
	  }
	  if(!isMax) break; 
	}
	if(!isMax) break; 
      }

      // add the jet to the list
      if (iEt>params_->jetSeedThreshold() && isMax) {
	math::XYZTLorentzVector p4;
	l1t::Jet jet( p4, iEt, ieta, iphi, 0);
	jets.push_back( jet );
      }

    }
  }

}

//Hi Adam
void l1t::CaloStage2JetAlgorithmFirmwareImp2::getRing(int32_t shift, std::vector<std::vector<int> > & ring, const std::vector<l1t::CaloTower> & towers,
    std::vector<l1t::Jet> & jets) {

  for (auto itJet = jets.begin(); itJet != jets.end(); ++itJet)
  {

    std::vector<int> ringParts(4); 

    int jetEta=itJet->hwEta();
    int jetPhi=itJet->hwPhi();

    int iphiUp = (jetPhi + shift > phiMax_) ? phiMin_ + shift - (phiMax_ - jetPhi) - 1:jetPhi+shift;
    int iphiDown = (jetPhi - shift < phiMin_) ? phiMax_-(shift - (jetPhi - phiMin_)) + 1:jetPhi-shift;
    int ietaUp = (jetEta + shift > etaMax_) ? 999 : jetEta+shift;
    int ietaDown = (jetEta - shift < etaMin_) ? 999 : jetEta-shift;

    for (int ieta = jetEta - shift+1; ieta != jetEta + shift; ++ieta)   
    {
      if (ieta > etaMax_ || ieta < etaMin_) continue;
      const CaloTower& tow = CaloTools::getTower(towers, ieta, iphiUp);
      int towEt = tow.hwEtEm() + tow.hwEtHad();
      ringParts[0]+=towEt;
      const CaloTower& tow2 = CaloTools::getTower(towers, ieta, iphiDown);
      towEt = tow2.hwEtEm() + tow2.hwEtHad();
      ringParts[1]+=towEt;
    } 
    for (int iphi = jetPhi - shift+1; iphi != jetPhi + shift; ++iphi)   
    {
      int towerPhi;
      if (iphi < phiMin_)
      {
	towerPhi = phiMax_-(shift - (jetPhi - phiMin_)) + 1;
      }
      else if (iphi > phiMax_)
      {
	towerPhi = phiMin_ + shift - (phiMax_ - jetPhi) - 1;
      }
      else 
      {
	towerPhi = iphi;
      }
      //if (ieta > etaMax_ || ieta < etaMin_) continue;
      const CaloTower& tow = CaloTools::getTower(towers, ietaUp, towerPhi);
      int towEt = tow.hwEtEm() + tow.hwEtHad();
      ringParts[2]+=towEt;
      const CaloTower& tow2 = CaloTools::getTower(towers, ietaDown, towerPhi);
      towEt = tow2.hwEtEm() + tow2.hwEtHad();
      ringParts[3]+=towEt;
    } 
    //Store it in a ring associated with a jet
    ring.push_back(ringParts);
  }
}

void l1t::CaloStage2JetAlgorithmFirmwareImp2::filter(std::vector<l1t::Jet> & jets) {

  //  jets.erase(std::remove_if(jets.begin(), jets.end(), jetIsZero) );

}


void l1t::CaloStage2JetAlgorithmFirmwareImp2::sort(std::vector<l1t::Jet> & jets) {

  // do nothing for now!

}

// remove jets with zero et
bool l1t::CaloStage2JetAlgorithmFirmwareImp2::jetIsZero(l1t::Jet jet) { return jet.hwPt()==0; }

