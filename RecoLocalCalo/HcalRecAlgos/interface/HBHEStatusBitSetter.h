#ifndef HBHESTATUSBITSETTER_H
#define HBHESTATUSBITSETTER_H 1


#include "DataFormats/HcalDigi/interface/HBHEDataFrame.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "CondFormats/HcalObjects/interface/HcalLogicalMap.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"

class HBHEStatusBitSetter {
 public:
  HBHEStatusBitSetter();
  HBHEStatusBitSetter(double nominalPedestal,double hitEnergyMinimum,int hitMultiplicityThreshold,std::vector<edm::ParameterSet> pulseShapeParameterSets, 
		      int firstSample=0, 
		      int samplesToAdd=10);
  ~HBHEStatusBitSetter();
  void Clear();
  void SetFlagsFromDigi(HBHERecHit& hbhe, const HBHEDataFrame& digi, const HcalCoder& coder,
			const HcalCalibrations& calib);
  void SetFlagsFromRecHits(HBHERecHitCollection& rec);
 private:
  double hitEnergyMinimum_;
  int hitMultiplicityThreshold_;
  unsigned int firstSample_;
  unsigned int samplesToAdd_;
  double nominalPedestal_;
  HcalLogicalMap *logicalMap_;
  std::vector<int> hpdMultiplicity_;
  std::vector< std::vector<double> > pulseShapeParameters_;
};

#endif
