#include <limits>

#include "StHFCuts.h"

#include "StHFPair.h"
#include "StHFTriplet.h"

ClassImp(StHFCuts)

// _________________________________________________________
StHFCuts::StHFCuts() : StPicoCutsBase("HFCutsBase"), 
  mSecondaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mSecondaryPairDecayLengthMin(std::numeric_limits<float>::min()), mSecondaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryPairCosThetaMin(std::numeric_limits<float>::min()), 
  mSecondaryPairMassMin(std::numeric_limits<float>::min()), mSecondaryPairMassMax(std::numeric_limits<float>::max()), 
  mSecondaryDcaToPvMax(std::numeric_limits<float>::max()),

  mTertiaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mTertiaryPairDecayLengthMin(std::numeric_limits<float>::min()), mTertiaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mTertiaryPairCosThetaMin(std::numeric_limits<float>::min()), 
  mTertiaryPairMassMin(std::numeric_limits<float>::min()), mTertiaryPairMassMax(std::numeric_limits<float>::max()), 
  mTertiaryDcaToPvMax(std::numeric_limits<float>::max()),

  mSecondaryTripletDcaDaughters12Max(std::numeric_limits<float>::max()), mSecondaryTripletDcaDaughters23Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDcaDaughters31Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDecayLengthMin(std::numeric_limits<float>::min()), mSecondaryTripletDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryTripletCosThetaMin(std::numeric_limits<float>::min()), 
  mSecondaryTripletMassMin(std::numeric_limits<float>::min()), mSecondaryTripletMassMax(std::numeric_limits<float>::max()),
  mPiDcaCut( 0 ), mPDcaCut( 0 ), mKDcaCut( 0 ) {
  // -- default constructor
}

// _________________________________________________________
StHFCuts::StHFCuts(const Char_t *name) : StPicoCutsBase(name), 
  mSecondaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mSecondaryPairDecayLengthMin(std::numeric_limits<float>::min()), mSecondaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryPairCosThetaMin(std::numeric_limits<float>::min()), 
  mSecondaryPairMassMin(std::numeric_limits<float>::min()), mSecondaryPairMassMax(std::numeric_limits<float>::max()), 
  mSecondaryDcaToPvMax(std::numeric_limits<float>::max()),

  mTertiaryPairDcaDaughtersMax(std::numeric_limits<float>::max()), 
  mTertiaryPairDecayLengthMin(std::numeric_limits<float>::min()), mTertiaryPairDecayLengthMax(std::numeric_limits<float>::max()), 
  mTertiaryPairCosThetaMin(std::numeric_limits<float>::min()), 
  mTertiaryPairMassMin(std::numeric_limits<float>::min()), mTertiaryPairMassMax(std::numeric_limits<float>::max()), 
  mTertiaryDcaToPvMax(std::numeric_limits<float>::max()),

  mSecondaryTripletDcaDaughters12Max(std::numeric_limits<float>::max()), mSecondaryTripletDcaDaughters23Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDcaDaughters31Max(std::numeric_limits<float>::max()), 
  mSecondaryTripletDecayLengthMin(std::numeric_limits<float>::min()), mSecondaryTripletDecayLengthMax(std::numeric_limits<float>::max()), 
  mSecondaryTripletCosThetaMin(std::numeric_limits<float>::min()), 
  mSecondaryTripletMassMin(std::numeric_limits<float>::min()), mSecondaryTripletMassMax(std::numeric_limits<float>::max()),
  mPiDcaCut( 0 ), mPDcaCut( 0 ), mKDcaCut( 0 ) {
  // -- constructor
}

// _________________________________________________________
StHFCuts::~StHFCuts() { 
  // destructor
  
}

// =======================================================================

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
	   pair.dcaDaughters() < mSecondaryPairDcaDaughtersMax &&
	   pair.DcaToPrimaryVertex() < mSecondaryDcaToPvMax);
}

// _________________________________________________________
bool StHFCuts::isGoodTertiaryVertexPair(StHFPair const & pair) const {
  // -- check for good tertiary vertex pair

  return ( pair.m() > mTertiaryPairMassMin && pair.m() < mTertiaryPairMassMax &&
	   std::cos(pair.pointingAngle()) > mTertiaryPairCosThetaMin &&
	   pair.decayLength() > mTertiaryPairDecayLengthMin && pair.decayLength() < mTertiaryPairDecayLengthMax &&
	   pair.dcaDaughters() < mTertiaryPairDcaDaughtersMax &&
	   pair.DcaToPrimaryVertex() < mTertiaryDcaToPvMax);
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
