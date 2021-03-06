#ifndef GroupedCkfTrajectoryBuilder_H
#define GroupedCkfTrajectoryBuilder_H

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "RecoTracker/CkfPattern/interface/BaseCkfTrajectoryBuilder.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"

#include "TrackingTools/PatternTools/interface/TempTrajectory.h"

#include <vector>




/** A highly configurable trajectory builder that allows full
 *  exploration of the combinatorial tree of possible continuations,
 *  and provides efficient ways of trimming the combinatorial tree.
 */

class GroupedCkfTrajectoryBuilder : public BaseCkfTrajectoryBuilder {
  
 public:
  /// constructor from ParameterSet
  GroupedCkfTrajectoryBuilder(const edm::ParameterSet&              conf,
			      const TrajectoryStateUpdator*         updator,
			      const Propagator*                     propagatorAlong,
			      const Propagator*                     propagatorOpposite,
			      const Chi2MeasurementEstimatorBase*   estimator,
			      const TransientTrackingRecHitBuilder* RecHitBuilder,
			      const MeasurementTracker*             measurementTracker,
			      const TrajectoryFilter*               filter,
			      const TrajectoryFilter*               inOutFilter);

  /// destructor
  virtual ~GroupedCkfTrajectoryBuilder(){}

  /// set Event for the internal MeasurementTracker data member
  virtual void setEvent(const edm::Event& event) const;

  /// trajectories building starting from a seed
  TrajectoryContainer trajectories(const TrajectorySeed&) const;

  /// trajectories building starting from a seed, return in an already allocated vector
  void trajectories(const TrajectorySeed&, TrajectoryContainer &ret) const;

  /// trajectories building starting from a seed with a region
  TrajectoryContainer trajectories(const TrajectorySeed&, const TrackingRegion&) const;

  /// trajectories building starting from a seed with a region
  void trajectories(const TrajectorySeed&, TrajectoryContainer &ret, const TrackingRegion&) const;

  /** trajectories re-building in the seeding region.
      It looks for additional measurements in the seeding region of the 
      intial trajectories.
      Only valid trajectories are returned. Invalid ones are dropped from the input
      collection.
  **/
  void  rebuildSeedingRegion(const TrajectorySeed&,
			     TrajectoryContainer& result) const ;  

  // Access to lower level components
  const TrajectoryStateUpdator&  updator() const    {return *theUpdator;}
  const Chi2MeasurementEstimatorBase&    estimator() const  {return *theEstimator;}

  //   PropagationDirection        direction() const  {return theDirection;}

  /** Chi**2 Cut on the new Trajectory Measurements to consider */
  double 	chiSquareCut()		{return theChiSquareCut;}

  /** Maximum number of trajectory candidates to propagate to the next layer. */
  int 		maxCand()		{return theMaxCand;}


  /** Chi**2 Penalty for each lost hit. */
  float 	lostHitPenalty()	{return theLostHitPenalty;}

  //   /** Tells whether an intermediary cleaning stage should take place during TB. */
  //   bool 		intermediateCleaning()	{return theIntermediateCleaning;}

  /// Pt cut
  double ptCut() {return theptCut;}

  /// Mass hypothesis used for propagation 
  double mass() {return theMass;}

protected:

  virtual void analyseSeed(const TrajectorySeed& seed) const{}

  virtual void analyseMeasurements( const std::vector<TM>& meas, 
				    const Trajectory& traj) const{}
  virtual void analyseResult( const TrajectoryContainer& result) const {}

private :
  /// no copy constructor
  GroupedCkfTrajectoryBuilder (const GroupedCkfTrajectoryBuilder&);

  /// no assignment operator
  GroupedCkfTrajectoryBuilder& operator= (const GroupedCkfTrajectoryBuilder&);

  /// common part of both public trajectory building methods
  void buildTrajectories (const TrajectorySeed&,
                                  TrajectoryContainer &ret,
	                          const TrajectoryFilter*) const;
  
  inline bool tkxor(bool a, bool b) const {return (a||b) && !(a&&b);}
  // to be ported later

  bool advanceOneLayer( TempTrajectory& traj, 
			const TrajectoryFilter* regionalCondition,
			const Propagator* propagator, 
                        bool inOut,
			TempTrajectoryContainer& newCand, 
			TempTrajectoryContainer& result) const;

  void groupedLimitedCandidates( TempTrajectory& startingTraj, 
				 const TrajectoryFilter* regionalCondition,
				 const Propagator* propagator, 
                                 bool inOut,
				 TempTrajectoryContainer& result) const;

  /// try to find additional hits in seeding region
  void rebuildSeedingRegion (TempTrajectory& startingTraj,
			     TempTrajectoryContainer& result) const ;

   //** try to find additional hits in seeding region for a candidate
   //* (returns number of trajectories added) *
  int rebuildSeedingRegion (const std::vector<const TrackingRecHit*>& seedHits,
			    TempTrajectory& candidate,
			    TempTrajectoryContainer& result) const ;

  // ** Backward fit of trajectory candidate except seed. Fit result and
  // *  remaining hits are returned in fittedTracks and remainingHits.
  // *  FittedTracks is empty if no fit was done. *
  void backwardFit (TempTrajectory& candidate, unsigned int nSeed,
		    const TrajectoryFitter& fitter,
		    TempTrajectoryContainer& fittedTracks,
		    std::vector<const TrackingRecHit*>& remainingHits) const;

  /// Verifies presence of a RecHits in a range of TrajectoryMeasurements.
  bool verifyHits (TempTrajectory::DataContainer::const_iterator rbegin,
		   size_t maxDepth,
		   const std::vector<const TrackingRecHit*>& hits) const;

  /// intermediate cleaning in the case of grouped measurements
  void groupedIntermediaryClean(TempTrajectoryContainer& theTrajectories) const ;

  /// fills in a list of layers from a container of TrajectoryMeasurements
  /// the "fillme" vector is cleared beforehand.
  void layers (const TempTrajectory::DataContainer& measurements,
               std::vector<const DetLayer*> &fillme) const;

  /// change of propagation direction
  inline PropagationDirection oppositeDirection (PropagationDirection dir) const {
    if ( dir==alongMomentum )  return oppositeToMomentum;
    else if ( dir==oppositeToMomentum )  return alongMomentum;
    return dir;
  }


private:
  TrajectoryFilter*              theConfigurableCondition;

  //   typedef deque< const TrajectoryFilter*>   StopCondContainer;
  //   StopCondContainer              theStopConditions;

  double theChiSquareCut;       /**< Chi**2 Cut on the new Trajectory Measurements to consider */

  double theptCut;              /**< ptCut */

  double theMass;               /**< Mass hypothesis used for propagation */

  int theMaxCand;               /**< Maximum number of trajectory candidates 
		                     to propagate to the next layer. */
  float theLostHitPenalty;      /**< Chi**2 Penalty for each lost hit. */
  float theFoundHitBonus;       /**< Chi**2 bonus for each found hit (favours candidates with
				     more measurements) */
  bool theIntermediateCleaning;	/**< Tells whether an intermediary cleaning stage 
                                     should take place during TB. */

  bool theAlwaysUseInvalid;

  bool theLockHits;             /**< Lock hits when building segments in a layer */
  bool theBestHitOnly;          /**< Use only best hit / group when building segments */

  bool theRequireSeedHitsInRebuild; 
                               /**< Only accept rebuilt trajectories if they contain the seed hits. */
  unsigned int theMinNrOfHitsForRebuild;     
                                /**< Minimum nr. of non-seed hits required for rebuild. 
                                     If ==0 the seeding part will remain untouched. */
  unsigned int theMinNrOf2dHitsForRebuild;   
                                /**< Minimum nr. of non-seed 2D hits required for rebuild. */


  mutable TempTrajectoryContainer work_; // Better here than alloc every time
  enum work_MaxSize_Size_ { work_MaxSize_ = 50 };  // if it grows above this number, it is forced to resize to half this amount when cleared
};

#endif
