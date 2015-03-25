#include <limits>
#include <cmath>

#ifdef __ROOT__
#include "StHFCuts.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StDcaGeometry.h" // remove in official production
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "StPicoDstMaker/StPicoTrack.h"

#include "StHFPair.h"
#include "StHFSecondaryPair.h"
#include "StHFTriplet.h"

ClassImp(StHFCuts)

// _________________________________________________________
StHFCuts::StHFCuts() : mVzMax(100.), mVzVpdVzMax(6.),
  mNHitsFitMax(20), mRequireHFT(true), mNHitsFitnHitsMax(0.52),
  
  mNSigmaPion(3.), mPionPt(0.2), mPionEta(999.),
  mNSigmaKaon(2.5), mKaonPt(0.2), mKaonEta(999.),
  mNSigmaProton(2.5), mProtonPt(0.2), mProtonEta(999.),
  
  mPrimDcaDaughtersMax(0.0200), mPrimDecayLengthMin(0.0030), mPrimMassMin(1.6), mPrimMassMax(2.1),
  mSecondDcaDaughtersMax(0.0200), mSecondDecayLengthMin(0.0030), mSecondMassMin(1.6), mSecondMassMax(2.1) {
  // -- private constructor 
}

StHFCuts* mStHFCuts = NULL;

// _________________________________________________________
StHFCuts* StHFCuts::Instance() {
  // -- Get Instance of singleton
  
  if (!mStHFCuts)
    mStHFCuts = new StHFCuts;

  return mStHFCuts;
}

// _________________________________________________________
bool StHFCuts::IsGoodEvent() {
  // TODO JMT 

  return true;
}

// _________________________________________________________
bool StHFCuts::IsGoodPrimaryPair(StHFPair const & pair) const {
  if (pair.dcaDaughters() <  mPrimDcaDaughtersMax &&
      pair.decayLength() > mPrimDecayLengthMin &&
      pair.m() > mPrimMassMin &&
      pair.m() < mPrimMassMax) return true;
  
  return false;
}

// _________________________________________________________
bool StHFCuts::IsGoodSecondaryPair(StHFSecondaryPair const & pair) const {
  // TODO JMT 
  
  return true;
}

// _________________________________________________________
bool StHFCuts::IsGoodTriplet(StHFTriplet const & triplet) const {
  // TODO JMT 
  
  return true;
}


#endif // __ROOT__
