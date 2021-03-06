#ifndef EventFilter_SiStripRawToDigi_SiStripRawToClustersLazyUnpacker_H
#define EventFilter_SiStripRawToDigi_SiStripRawToClustersLazyUnpacker_H

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/LazyGetter.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "CalibFormats/SiStripObjects/interface/SiStripRegionCabling.h"
#include "RecoLocalTracker/SiStripClusterizer/interface/StripClusterizerAlgorithm.h"
#include "RecoLocalTracker/SiStripZeroSuppression/interface/SiStripRawProcessingAlgorithms.h"
#include "EventFilter/SiStripRawToDigi/interface/SiStripRawToDigiUnpacker.h"
#include "EventFilter/SiStripRawToDigi/interface/SiStripFEDBuffer.h"
#include <vector>
#include <list>

namespace sistrip {
  
  class RawToClustersLazyUnpacker : public edm::LazyUnpacker<SiStripCluster> {
    
  public:
    
    typedef edm::DetSet<SiStripCluster> DetSet;
    
    RawToClustersLazyUnpacker(const SiStripRegionCabling&, StripClusterizerAlgorithm&, SiStripRawProcessingAlgorithms&, const FEDRawDataCollection&, bool = false); 
    
    virtual ~RawToClustersLazyUnpacker();
    
    virtual void fill(const uint32_t&, record_type&); 
    
  private:
    
    /// private default constructor
    RawToClustersLazyUnpacker();
    
    /// raw data
    const FEDRawDataCollection* raw_;
    
    /// cabling
    const SiStripRegionCabling::Cabling* regions_;
    
    /// clusterizer algorithm
    StripClusterizerAlgorithm* const clusterizer_;
    
    /// raw processing algorithms
    SiStripRawProcessingAlgorithms* const rawAlgos_;
    
    /// FED event cache
    std::vector< sistrip::FEDBuffer* > buffers_;
    
    /// raw-to-digi
    RawToDigiUnpacker rawToDigi_;
    
    /// dump frequency
    bool dump_;
    
  };
  
}

#endif ///  EventFilter_SiStripRawToDigi_SiStripRawToClustersLazyUnpacker_H
