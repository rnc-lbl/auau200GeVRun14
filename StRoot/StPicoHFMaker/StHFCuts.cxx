#include <limits>
#include <cmath>

#ifdef __ROOT__
#include "StHFCuts.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoEvent.h"

#include "StHFPair.h"
#include "StHFTriplet.h"

ClassImp(StHFCuts)

// _________________________________________________________
StHFCuts::StHFCuts() 
: TNamed("HFCutsBase", "HFCutsBase"), mEventStatMax(6),
  mVzMax(6.), mVzVpdVzMax(3.), mTriggerWord(0x1F),
  mNHitsFitMax(15), mRequireHFT(true), mNHitsFitnHitsMax(0.52),
  
  mTPCNSigmaPionMax(3.), 
  mTOFNSigmaPionMax(3.), 
  mPionPtMin(std::numeric_limits<float>::min()),  mPionPtMax(std::numeric_limits<float>::max()), 
  mPionEtaMin(std::numeric_limits<float>::min()), mPionEtaMax(std::numeric_limits<float>::max()), 
  mPionPtTOFMin(std::numeric_limits<float>::min()), mPionPtTOFMax(std::numeric_limits<float>::max()), 

  mTPCNSigmaKaonMax(3.), 
  mTOFNSigmaKaonMax(3.), 
  mKaonPtMin(std::numeric_limits<float>::min()),  mKaonPtMax(std::numeric_limits<float>::max()), 
  mKaonEtaMin(std::numeric_limits<float>::min()), mKaonEtaMax(std::numeric_limits<float>::max()), 
  mKaonPtTOFMin(std::numeric_limits<float>::min()), mKaonPtTOFMax(std::numeric_limits<float>::max()), 

  mTPCNSigmaProtonMax(3.), 
  mTOFNSigmaProtonMax(3.), 
  mProtonPtMin(std::numeric_limits<float>::min()),  mProtonPtMax(std::numeric_limits<float>::max()), 
  mProtonEtaMin(std::numeric_limits<float>::min()), mProtonEtaMax(std::numeric_limits<float>::max()), 
  mProtonPtTOFMin(std::numeric_limits<float>::min()), mProtonPtTOFMax(std::numeric_limits<float>::max()), 

  mSecondaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mSecondaryPairDecayLengthMin(std::numeric_limits<float>::min()), mSecondaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryPairCosThetaMin(std::numeric_limits<float>::min()), 
  mSecondaryPairMassMin(std::numeric_limits<float>::min()), mSecondaryPairMassMax(std::numeric_limits<float>::max()), 

  mTertiaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mTertiaryPairDecayLengthMin(std::numeric_limits<float>::min()), mTertiaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mTertiaryPairCosThetaMin(std::numeric_limits<float>::min()), 
  mTertiaryPairMassMin(std::numeric_limits<float>::min()), mTertiaryPairMassMax(std::numeric_limits<float>::max()), 

  mSecondaryTripletDcaDaughters12Max(std::numeric_limits<float>::max()), mSecondaryTripletDcaDaughters23Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDcaDaughters31Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDecayLengthMin(std::numeric_limits<float>::min()), mSecondaryTripletDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryTripletCosThetaMin(std::numeric_limits<float>::min()), 
  mSecondaryTripletMassMin(std::numeric_limits<float>::min()), mSecondaryTripletMassMax(std::numeric_limits<float>::max()) {
  // -- default constructor
}

// _________________________________________________________
StHFCuts::StHFCuts(const Char_t *name) 
  : TNamed(name, name), mEventStatMax(6),
  mVzMax(6.), mVzVpdVzMax(3.), mTriggerWord(0x1F),
  mNHitsFitMax(15), mRequireHFT(true), mNHitsFitnHitsMax(0.52),
  
  mTPCNSigmaPionMax(3.), 
  mTOFNSigmaPionMax(3.), 
  mPionPtMin(std::numeric_limits<float>::min()),  mPionPtMax(std::numeric_limits<float>::max()), 
  mPionEtaMin(std::numeric_limits<float>::min()), mPionEtaMax(std::numeric_limits<float>::max()), 
  mPionPtTOFMin(std::numeric_limits<float>::min()), mPionPtTOFMax(std::numeric_limits<float>::max()), 

  mTPCNSigmaKaonMax(3.), 
  mTOFNSigmaKaonMax(3.), 
  mKaonPtMin(std::numeric_limits<float>::min()),  mKaonPtMax(std::numeric_limits<float>::max()), 
  mKaonEtaMin(std::numeric_limits<float>::min()), mKaonEtaMax(std::numeric_limits<float>::max()), 
  mKaonPtTOFMin(std::numeric_limits<float>::min()), mKaonPtTOFMax(std::numeric_limits<float>::max()), 

  mTPCNSigmaProtonMax(3.), 
  mTOFNSigmaProtonMax(3.), 
  mProtonPtMin(std::numeric_limits<float>::min()),  mProtonPtMax(std::numeric_limits<float>::max()), 
  mProtonEtaMin(std::numeric_limits<float>::min()), mProtonEtaMax(std::numeric_limits<float>::max()), 
  mProtonPtTOFMin(std::numeric_limits<float>::min()), mProtonPtTOFMax(std::numeric_limits<float>::max()), 

  mSecondaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mSecondaryPairDecayLengthMin(std::numeric_limits<float>::min()), mSecondaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryPairCosThetaMin(std::numeric_limits<float>::min()), 
  mSecondaryPairMassMin(std::numeric_limits<float>::min()), mSecondaryPairMassMax(std::numeric_limits<float>::max()), 

  mTertiaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mTertiaryPairDecayLengthMin(std::numeric_limits<float>::min()), mTertiaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mTertiaryPairCosThetaMin(std::numeric_limits<float>::min()), 
  mTertiaryPairMassMin(std::numeric_limits<float>::min()), mTertiaryPairMassMax(std::numeric_limits<float>::max()), 

  mSecondaryTripletDcaDaughters12Max(std::numeric_limits<float>::max()), mSecondaryTripletDcaDaughters23Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDcaDaughters31Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDecayLengthMin(std::numeric_limits<float>::min()), mSecondaryTripletDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryTripletCosThetaMin(std::numeric_limits<float>::min()), 
  mSecondaryTripletMassMin(std::numeric_limits<float>::min()), mSecondaryTripletMassMax(std::numeric_limits<float>::max()) {
  // -- constructor
}

// _________________________________________________________
bool StHFCuts::isGoodEvent(StPicoEvent const * const picoEvent, int *aEventCuts = NULL) {

  // -- quick method without 
  if (!aEventCuts) {
    return ((picoEvent->triggerWord() & mTriggerWord) &&
	    picoEvent->ranking() > 0 &&	
	    fabs(picoEvent->primaryVertex().z()) < mVzMax &&
	    fabs(picoEvent->primaryVertex().z() - picoEvent->vzVpd()) < mVzVpdVzMax);
  }
    
  // -- reset event cuts
  for (unsigned int ii = 0; ii < mEventStatMax; ++ii)
    aEventCuts[ii] = 0;
  
  unsigned  int iCut = 0;

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

  // ... if needed

  // -- is rejected
  bool isGoodEvent = true;
  for (unsigned int ii = 0; ii < mEventStatMax; ++ii)
    if  (aEventCuts[ii])
      isGoodEvent = false;
        
  return isGoodEvent;
}

// _________________________________________________________
bool StHFCuts::isGoodTrack(StPicoTrack const * const trk) const {
  // -- require at least one hit on every layer of PXL and IST.
  return ((!mRequireHFT || trk->isHFTTrack()) && 
	  trk->nHitsFit() >= mNHitsFitMax);
}

// _________________________________________________________
bool StHFCuts::isTPCPion(StPicoTrack const * const trk) const {
  // -- check for good pion in TPC

  return ( trk->pMom().perp() >= mPionPtMin && trk->pMom().perp() < mPionPtMax &&
	   trk->pMom().pseudoRapidity() >= mPionEtaMin && trk->pMom().pseudoRapidity() < mPionEtaMax &&
	   fabs(trk->nSigmaPion()) < mTPCNSigmaPionMax );
}

// _________________________________________________________
bool StHFCuts::isTPCKaon(StPicoTrack const * const trk) const {
  // -- check for good kaon in TPC

  return ( trk->pMom().perp() >= mKaonPtMin && trk->pMom().perp() < mKaonPtMax &&
	   trk->pMom().pseudoRapidity() >= mKaonEtaMin && trk->pMom().pseudoRapidity() < mKaonEtaMax &&
	   fabs(trk->nSigmaKaon()) < mTPCNSigmaKaonMax );
}

// _________________________________________________________
bool StHFCuts::isTPCProton(StPicoTrack const * const trk) const {
  // -- check for good proton in TPC

  return ( trk->pMom().perp() >= mProtonPtMin && trk->pMom().perp() < mProtonPtMax &&
	   trk->pMom().pseudoRapidity() >= mProtonEtaMin && trk->pMom().pseudoRapidity() < mProtonEtaMax &&
	   fabs(trk->nSigmaProton()) < mTPCNSigmaProtonMax );
}

// _________________________________________________________
bool StHFCuts::isTOFPion(StPicoTrack const *trk, float const & bTofBeta) const {
  // -- check for good pion in TOF - in a different pT range than for TPC

  // -- JMT FIX ME WITH REAL TOF CHECK

  return ( trk->pMom().perp() >= mPionPtTOFMin && trk->pMom().perp() < mPionPtTOFMax &&
	   fabs(trk->nSigmaPion()) < mTOFNSigmaPionMax );
}

// _________________________________________________________
bool StHFCuts::isTOFKaon(StPicoTrack const *trk, float const & bTofBeta) const {
  // -- check for good kaon in TOF - in a different pT range than for TPC

  // -- JMT FIX ME WITH REAL TOF CHECK

  return ( trk->pMom().perp() >= mKaonPtTOFMin && trk->pMom().perp() < mKaonPtTOFMax &&
	   fabs(trk->nSigmaKaon()) < mTOFNSigmaKaonMax );
}

// _________________________________________________________
bool StHFCuts::isTOFProton(StPicoTrack const *trk, float const & bTofBeta) const {
  // -- check for good proton in TOF - in a different pT range than for TPC

  // -- JMT FIX ME WITH REAL TOF CHECK

  return ( trk->pMom().perp() >= mProtonPtTOFMin && trk->pMom().perp() < mProtonPtTOFMax &&
	   fabs(trk->nSigmaProton()) < mTOFNSigmaProtonMax );
}

// _________________________________________________________
bool StHFCuts::isClosePair(StHFPair const & pair) const {
  // -- check for a pair which is close in dca w/o mass constraint,
  //    using secondary vertex cuts

  return ( std::cos(pair.pointingAngle()) > mSecondaryPairCosThetaMin &&
	   pair.decayLength() > mSecondaryPairDecayLengthMin && pair.decayLength() < mSecondaryPairDecayLengthMax &&
	   pair.dcaDaughters() < mSecondaryPairDcaDaughtersMax);
}

// _________________________________________________________
bool StHFCuts::isGoodSecondaryVertexPair(StHFPair const & pair) const {
  // -- check for good secondary vertex pair

  return ( pair.m() > mSecondaryPairMassMin && pair.m() < mSecondaryPairMassMax &&
	   std::cos(pair.pointingAngle()) > mSecondaryPairCosThetaMin &&
	   pair.decayLength() > mSecondaryPairDecayLengthMin && pair.decayLength() < mSecondaryPairDecayLengthMax &&
	   pair.dcaDaughters() < mSecondaryPairDcaDaughtersMax);
}

// _________________________________________________________
bool StHFCuts::isGoodTertiaryVertexPair(StHFPair const & pair) const {
  // -- check for good tertiary vertex pair

  return ( pair.m() > mTertiaryPairMassMin && pair.m() < mTertiaryPairMassMax &&
	   std::cos(pair.pointingAngle()) > mTertiaryPairCosThetaMin &&
	   pair.decayLength() > mTertiaryPairDecayLengthMin && pair.decayLength() < mTertiaryPairDecayLengthMax &&
	   pair.dcaDaughters() < mTertiaryPairDcaDaughtersMax);
}

// _________________________________________________________
bool StHFCuts::isGoodSecondaryVertexTriplet(StHFTriplet const & triplet) const {
  // -- check for good secondary vertex triplet

  return ( triplet.m() > mSecondaryTripletMassMin && triplet.m() < mSecondaryTripletMassMax &&
	   std::cos(triplet.pointingAngle()) > mSecondaryTripletCosThetaMin &&
	   triplet.decayLength() > mSecondaryTripletDecayLengthMin && triplet.decayLength() < mSecondaryTripletDecayLengthMax &&
	   triplet.dcaDaughters12() < mSecondaryTripletDcaDaughters12Max &&
	   triplet.dcaDaughters23() < mSecondaryTripletDcaDaughters23Max &&
	   triplet.dcaDaughters31() < mSecondaryTripletDcaDaughters31Max);
}

#endif // __ROOT__
