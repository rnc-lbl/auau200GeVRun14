#include <limits>
#include <cmath>

#ifdef __ROOT__
#include "StHFCuts.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "../StPicoDstMaker/StPicoTrack.h"
#include "../StPicoDstMaker/StPicoEvent.h"

#include "StHFPair.h"
#include "StHFSecondaryPair.h"
#include "StHFTriplet.h"

ClassImp(StHFCuts)

// _________________________________________________________
StHFCuts::StHFCuts() : mVzMax(6.), mVzVpdVzMax(3.), mTriggerWord(0x1F),
  mNHitsFitMax(15), mRequireHFT(true), mNHitsFitnHitsMax(0.52),
  
  // mNSigmaPion(3.),    mPionPt(0.2),   mPionEta(999.),
  // mNSigmaKaon(2.5),   mKaonPt(0.2),   mKaonEta(999.),
  // mNSigmaProton(2.5), mProtonPt(0.2), mProtonEta(999.),
  
  mPrimaryDcaDaughtersMax(0.0200), mPrimaryDecayLengthMin(0.0030), mPrimaryDecayLengthMax(1.), 
  mPrimaryCosThetaMin(0.), mPrimaryMassMin(1.6), mPrimaryMassMax(2.1),

  mSecondaryDcaDaughtersMax(0.0200), mSecondaryDecayLengthMin(0.0030), mSecondaryDecayLengthMax(1.),
  mSecondaryCosThetaMin(0.), mSecondaryMassMin(1.6), mSecondaryMassMax(2.1),

  mTripletDcaDaughters12Max(0.0200), mTripletDcaDaughters23Max(0.0200), mTripletDcaDaughters31Max(0.0200), 
  mTripletDecayLengthMin(0.0030),   mTripletDecayLengthMax(1.), 
  mTripletCosThetaMin(0.), mTripletMassMin(1.6), mTripletMassMax(2.1) {
  // -- private constructor 
}

StHFCuts* StHFCuts::mStHFCuts = NULL;

// _________________________________________________________
StHFCuts* StHFCuts::Instance() {
  // -- Get Instance of singleton
  
  if (!mStHFCuts)
    mStHFCuts = new StHFCuts;

  return mStHFCuts;
}

// _________________________________________________________
bool StHFCuts::IsGoodTrack(StPicoTrack const * const trk) const
{
  // Require at least one hit on every layer of PXL and IST.
  // It is done here for tests on the preview II data.
  // The new StPicoTrack which is used in official production has a method to check this
  return ((!mRequireHFT || trk->isHFTTrack()) && 
	  trk->nHitsFit() >= mNHitsFitMax);
}

// _________________________________________________________
bool StHFCuts::IsGoodEvent(StPicoEvent const * const picoEvent, int *aEventCuts = NULL, const int eventStatMax = 0) {

  // -- quick method without 
  if (!aEventCuts) {
    return ((picoEvent->triggerWord() & mTriggerWord) &&
	    picoEvent->ranking() > 0 &&	
	    fabs(picoEvent->primaryVertex().z()) < mVzMax &&
	    fabs(picoEvent->primaryVertex().z() - picoEvent->vzVpd()) < mVzVpdVzMax);
  }
    
  // -- reset event cuts
  for (Int_t ii = 0; ii < eventStatMax; ++ii)
    aEventCuts[ii] = 0;
  
  Int_t iCut = 0;

  // -- 0 - before event cuts
  aEventCuts[iCut] = 0;

  // -- 1 - No Trigger fired
  ++iCut;
  if (!(picoEvent->triggerWord() & mTriggerWord))
    aEventCuts[iCut] = 1;

  // -- 2 - Vertex ranking 
  ++iCut;
  if (picoEvent->ranking() <= 0)	
    aEventCuts[iCut] = 1;

  // -- 3 - Vertex z outside cut window
  ++iCut;
  if (fabs(picoEvent->primaryVertex().z()) >= mVzMax)
    aEventCuts[iCut] = 1;

  // -- 4 Vertex z - vertex_z(vpd) outside cut window
  ++iCut;
  if (fabs(picoEvent->primaryVertex().z() - picoEvent->vzVpd()) >= mVzVpdVzMax)
    aEventCuts[iCut] = 1;  
  
  // -- 5 check for centrality info




  // -- is rejected
  bool isGoodEvent = true;
  for (Int_t ii = 0; ii < eventStatMax; ++ii)
    if  (aEventCuts[ii])
      isGoodEvent = false;
        
  return isGoodEvent;
}

// _________________________________________________________
bool StHFCuts::IsGoodPrimaryPair(StHFPair const & pair) const {
  return ( pair.m() > mPrimaryMassMin && pair.m() < mPrimaryMassMax &&
	   std::cos(pair.pointingAngle()) > mPrimaryCosThetaMin &&
	   pair.decayLength() > mPrimaryDecayLengthMin && pair.decayLength() < mPrimaryDecayLengthMax &&
	   pair.dcaDaughters() < mPrimaryDcaDaughtersMax);
}

// _________________________________________________________
bool StHFCuts::IsGoodSecondaryPair(StHFSecondaryPair const & pair) const {
  return ( pair.m() > mSecondaryMassMin && pair.m() < mSecondaryMassMax &&
	   std::cos(pair.pointingAngle()) > mSecondaryCosThetaMin &&
	   pair.decayLength() > mSecondaryDecayLengthMin && pair.decayLength() < mSecondaryDecayLengthMax &&
	   pair.dcaDaughters() < mSecondaryDcaDaughtersMax);
}

// _________________________________________________________
bool StHFCuts::IsGoodTriplet(StHFTriplet const & triplet) const {
  return ( triplet.m() > mTripletMassMin && triplet.m() < mTripletMassMax &&
	   std::cos(triplet.pointingAngle()) > mTripletCosThetaMin &&
	   triplet.decayLength() > mTripletDecayLengthMin && triplet.decayLength() < mTripletDecayLengthMax &&
	   triplet.dcaDaughters12() < mTripletDcaDaughters12Max &&
	   triplet.dcaDaughters23() < mTripletDcaDaughters23Max &&
	   triplet.dcaDaughters31() < mTripletDcaDaughters31Max);
}


#endif // __ROOT__
