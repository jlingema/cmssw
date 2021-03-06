// -*- C++ -*-
//
// Package:    SiStripMonitorQuality
// Class:      SiStripMonitorQuality
// 
/**\class SiStripMonitorDigi SiStripMonitorDigi.cc DQM/SiStripMonitorDigi/src/SiStripMonitorDigi.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Suchandra Dutta
//         Created:  Fri Dec  7 20:50 CET 2007
// $Id: SiStripMonitorQuality.cc,v 1.4 2008/04/12 15:16:05 dutta Exp $
//
//

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripQuality.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "DQM/SiStripCommon/interface/SiStripHistoId.h"


#include <FWCore/Framework/interface/EventSetup.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQM/SiStripMonitorPedestals/interface/SiStripMonitorQuality.h"

// std
#include <cstdlib>
#include <cmath>
#include <numeric>
#include <algorithm>

SiStripMonitorQuality::SiStripMonitorQuality(edm::ParameterSet const& iConfig):
  dqmStore_(edm::Service<DQMStore>().operator->()),
  conf_(iConfig),
  m_cacheID_(0)


{
  edm::LogInfo("SiStripMonitorQuality") <<"SiStripMonitorQuality  " 
					  << " Constructing....... ";     
}


SiStripMonitorQuality::~SiStripMonitorQuality()
{
  edm::LogInfo("SiStripMonitorQuality") <<"SiStripMonitorQuality  " 
					  << " Destructing....... ";     
}
//
// -- Begin Job
//
void SiStripMonitorQuality::beginJob() {
}
//
// -- BeginRun
//
void SiStripMonitorQuality::beginRun(edm::Run const& run, edm::EventSetup const& eSetup){
}


// ------------ method called to produce the data  ------------
void SiStripMonitorQuality::analyze(edm::Event const& iEvent, edm::EventSetup const& eSetup)
{
  unsigned long long cacheID = eSetup.get<SiStripQualityRcd>().cacheIdentifier();  
  if (m_cacheID_ == cacheID) return;
  
  m_cacheID_ = cacheID;       

  std::string quality_label = conf_.getParameter<std::string>("StripQualityLabel");
  eSetup.get<SiStripQualityRcd>().get(quality_label,stripQuality_);
  eSetup.get<SiStripDetCablingRcd>().get( detCabling_ );
  
  edm::LogInfo("SiStripMonitorQuality") << "SiStripMonitorQuality::analyze: "<<
    " Reading SiStripQuality "<< std::endl;

  SiStripBadStrip::RegistryIterator rbegin = stripQuality_->getRegistryVectorBegin();
  SiStripBadStrip::RegistryIterator rend   = stripQuality_->getRegistryVectorEnd();
  uint32_t detid;
    
  if (rbegin==rend) return;

  for (SiStripBadStrip::RegistryIterator rp=rbegin; rp != rend; ++rp) {
    
    detid = rp->detid;
    // Check consistency in DetId
    if (detid == 0 || detid == 0xFFFFFFFF){
      edm::LogError("SiStripMonitorQuality") <<"SiStripMonitorQuality::analyze : " 
					       << "Wrong DetId !!!!!! " <<  detid << " Neglecting !!!!!! ";
      continue;
    }
    // check if the detid is connected in cabling
    if (!detCabling_->IsConnected(detid)) {
      edm::LogError("SiStripMonitorQuality") <<"SiStripMonitorQuality::analyze : " 
					   << " DetId " <<  detid << " not connected,  Neglecting !!!!!! ";
      continue;
    }
    MonitorElement* me = getQualityME(detid);
    SiStripBadStrip::Range range = SiStripBadStrip::Range( stripQuality_->getDataVectorBegin()+rp->ibegin , 
							   stripQuality_->getDataVectorBegin()+rp->iend );
    SiStripBadStrip::ContainerIterator it=range.first;
    for(;it!=range.second;++it){
      unsigned int value=(*it);
      short str_start = stripQuality_->decode(value).firstStrip;
      short str_end   = str_start + stripQuality_->decode(value).range;
      for ( short isr = str_start; isr < str_end + 1; isr++) {        
        if (isr <= (me->getNbinsX()-1)) me->Fill(isr+1, 1.0);
      }
    }
  }
}
//
// -- End Run
//    
void SiStripMonitorQuality::endRun(edm::Run const& run, edm::EventSetup const& eSetup) {
  bool outputMEsInRootFile = conf_.getParameter<bool>("OutputMEsInRootFile");
  std::string outputFileName = conf_.getParameter<std::string>("OutputFileName");
  if (outputMEsInRootFile) {    
    dqmStore_->showDirStructure();
    dqmStore_->save(outputFileName);
  }
}
//
// -- End Job
//
void SiStripMonitorQuality::endJob(void){
  edm::LogInfo("SiStripMonitorQuality") <<"SiStripMonitorQuality::EndJob: " 
					  << " Finishing!! ";        
}
//
// -- End Job
//
MonitorElement* SiStripMonitorQuality::getQualityME(uint32_t idet){

  std::map<uint32_t, MonitorElement* >::iterator pos = QualityMEs.find(idet);
  MonitorElement* det_me;
  if (pos != QualityMEs.end()) {
    det_me = pos->second;
    det_me->Reset();
  } else {
    int nStrip =  detCabling_->nApvPairs(idet) * 256;

    // use SistripHistoId for producing histogram id (and title)
    SiStripHistoId hidmanager;
    // create SiStripFolderOrganizer
    SiStripFolderOrganizer folder_organizer;
    // set appropriate folder using SiStripFolderOrganizer
    folder_organizer.setDetectorFolder(idet); // pass the detid to this method

    std::string hid;
    hid = hidmanager.createHistoId("StripQualityFromCondDB","det", idet);
    
    std::map<uint32_t, MonitorElement* >::iterator pos = QualityMEs.find(idet);

    det_me = dqmStore_->book1D(hid, hid, nStrip,0.5,nStrip+0.5);
    dqmStore_->tag(det_me, idet);
    det_me->setAxisTitle("Strip Number",1);
    det_me->setAxisTitle("Quality Flag from CondDB ",2);
    QualityMEs.insert( std::make_pair(idet, det_me));
  }
  return det_me;
}
DEFINE_FWK_MODULE(SiStripMonitorQuality);

