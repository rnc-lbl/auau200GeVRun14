#include <limits>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>

#ifdef __ROOT__
#include "StHFCuts.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "StBTofUtil/tofPathLength.hh"

#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoBTofPidTraits.h"

#include "StHFPair.h"
#include "StHFTriplet.h"

ClassImp(StHFCuts)

// _________________________________________________________
StHFCuts::StHFCuts() 
: TNamed("HFCutsBase", "HFCutsBase"), mPicoDst(NULL), mEventStatMax(7), mTOFResolution(0.013),
  mBadRunListFileName("picoList_bad_MB.list"), 
  mVzMax(6.), mVzVpdVzMax(3.), mTriggerWord(0x1F),
  mNHitsFitMax(15), mRequireHFT(true), mNHitsFitnHitsMax(0.52),
  
  mTPCNSigmaPionMax(3.),
  mTOFDeltaOneOverBetaPionMax(0.04), 
  mPionPtMin(std::numeric_limits<float>::min()),  mPionPtMax(std::numeric_limits<float>::max()), 
  mPionPtTOFMin(std::numeric_limits<float>::min()), mPionPtTOFMax(std::numeric_limits<float>::max()), 

  mTPCNSigmaKaonMax(3.), 
  mTOFDeltaOneOverBetaKaonMax(0.04), 
  mKaonPtMin(std::numeric_limits<float>::min()),  mKaonPtMax(std::numeric_limits<float>::max()), 
  mKaonPtTOFMin(std::numeric_limits<float>::min()), mKaonPtTOFMax(std::numeric_limits<float>::max()), 

  mTPCNSigmaProtonMax(3.), 
  mTOFDeltaOneOverBetaProtonMax(0.04), 
  mProtonPtMin(std::numeric_limits<float>::min()),  mProtonPtMax(std::numeric_limits<float>::max()), 
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
: TNamed(name, name), mPicoDst(NULL), mEventStatMax(7), mTOFResolution(0.013),
  mBadRunListFileName("picoList_bad_MB.list"), 
  mVzMax(6.), mVzVpdVzMax(3.), mTriggerWord(0x1F),
  mNHitsFitMax(15), mRequireHFT(true), mNHitsFitnHitsMax(0.52),
  
  mTPCNSigmaPionMax(3.), 
  mTOFDeltaOneOverBetaPionMax(0.04), 
  mPionPtMin(std::numeric_limits<float>::min()),  mPionPtMax(std::numeric_limits<float>::max()), 
  mPionPtTOFMin(std::numeric_limits<float>::min()), mPionPtTOFMax(std::numeric_limits<float>::max()), 

  mTPCNSigmaKaonMax(3.), 
  mTOFDeltaOneOverBetaKaonMax(0.04), 
  mKaonPtMin(std::numeric_limits<float>::min()),  mKaonPtMax(std::numeric_limits<float>::max()), 
  mKaonPtTOFMin(std::numeric_limits<float>::min()), mKaonPtTOFMax(std::numeric_limits<float>::max()), 

  mTPCNSigmaProtonMax(3.), 
  mTOFDeltaOneOverBetaProtonMax(0.04), 
  mProtonPtMin(std::numeric_limits<float>::min()),  mProtonPtMax(std::numeric_limits<float>::max()), 
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
void StHFCuts::init() {
  // -- init cuts class

  // -- Read in bad run list and fill vector
  // -----------------------------------------

  // -- open list
  ifstream runs;

  // -- open in working dir
  runs.open(mBadRunListFileName.Data());
  if (!runs.is_open()) {
    runs.open(Form("picoLists/%s", mBadRunListFileName.Data()));
    if (!runs.is_open()) {
      cout << "StHFCuts::init --Bad run list found :" << mBadRunListFileName << endl;
      exit(EXIT_FAILURE);
    }
  }

  Int_t runId = 0;
  while( runs >> runId )
    mVecBadRunList.push_back(runId);
  
  runs.close();

  // -- sort bad runs vector
  std::sort(mVecBadRunList.begin(), mVecBadRunList.end());
}

// _________________________________________________________
bool StHFCuts::isGoodEvent(StPicoDst const * const picoDst, int *aEventCuts = NULL) {
  // -- method to check if good event
  //    sets also mPicoDst and mPrimVtx
  
  // -- set current mPicoDst 
  mPicoDst = picoDst;

  // -- get picoDst event
  StPicoEvent* picoEvent = mPicoDst->event();

  // -- set current primary vertex
  mPrimVtx = picoEvent->primaryVertex();

  // -- quick method without providing stats
  if (!aEventCuts) {
    return (isGoodRun(picoEvent) && (picoEvent->triggerWord() & mTriggerWord) &&
	    fabs(picoEvent->primaryVertex().z()) < mVzMax &&
	    fabs(picoEvent->primaryVertex().z() - picoEvent->vzVpd()) < mVzVpdVzMax);
  }
    
  // -- reset event cuts
  for (unsigned int ii = 0; ii < mEventStatMax; ++ii)
    aEventCuts[ii] = 0;
  
  unsigned int iCut = 0;

  // -- 0 - before event cuts
  aEventCuts[iCut] = 0;

  // -- 1 - is bad run
  ++iCut;
  if (!isGoodRun(picoEvent))
    aEventCuts[iCut] = 1;

  // -- 2 - No Trigger fired
  ++iCut;
  if (!(picoEvent->triggerWord() & mTriggerWord))
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
bool StHFCuts::isGoodRun(StPicoEvent const * const picoEvent) const {
  // -- is good run (not in bad runlist)

  return (!(std::binary_search(mVecBadRunList.begin(), mVecBadRunList.end(), picoEvent->runId())));
}

// _________________________________________________________
bool StHFCuts::isGoodTrack(StPicoTrack const * const trk) const {
  // -- require at least one hit on every layer of PXL and IST.
  return ((!mRequireHFT || trk->isHFTTrack()) && 
	  trk->nHitsFit() >= mNHitsFitMax);
}

// =======================================================================

// _________________________________________________________
bool StHFCuts::isTPCPion(StPicoTrack const * const trk) const {
  // -- check for good pion in TPC

  return ( trk->gPt() >= mPionPtMin && trk->gPt() < mPionPtMax &&
	   fabs(trk->nSigmaPion()) < mTPCNSigmaPionMax );
}

// _________________________________________________________
bool StHFCuts::isTPCKaon(StPicoTrack const * const trk) const {
  // -- check for good kaon in TPC

  return ( trk->gPt() >= mKaonPtMin && trk->gPt() < mKaonPtMax &&
	   fabs(trk->nSigmaKaon()) < mTPCNSigmaKaonMax );
}

// _________________________________________________________
bool StHFCuts::isTPCProton(StPicoTrack const * const trk) const {
  // -- check for good proton in TPC

  return ( trk->gPt() >= mProtonPtMin && trk->gPt() < mProtonPtMax &&
	   fabs(trk->nSigmaProton()) < mTPCNSigmaProtonMax );
}

// =======================================================================

// _________________________________________________________
bool StHFCuts::isTOFPion(StPicoTrack const *trk) const {
  // -- check for good pion in TOF - in a different pT range than for TPC
  //    if no TOF information on track return false;

  float tofBeta = getTofBeta(trk);
  return isTOFPion(trk, tofBeta);
}

// _________________________________________________________
bool StHFCuts::isHybridTOFPion(StPicoTrack const *trk) const {
  // -- check for good pion in TOF - in a different pT range than for TPC
  //    if no TOF information on track return true;

  float tofBeta = getTofBeta(trk);
  return (tofBeta > 0) ? isTOFPion(trk, tofBeta) : true;
}

// _________________________________________________________
bool StHFCuts::isTOFPion(StPicoTrack const *trk, float const & tofBeta) const {
  // -- check for good pion in TOF - in a different pT range than for TPC

  if (tofBeta <= 0) 
    return false;

  double ptot = trk->dcaGeometry().momentum().mag();
  float  beta = ptot/sqrt(ptot*ptot+M_PION_PLUS*M_PION_PLUS);
  bool isParticle = (fabs(1/tofBeta - 1/beta) < mTOFDeltaOneOverBetaPionMax) ? true : false;
    
  return (trk->gPt() >= mPionPtTOFMin && trk->gPt() < mPionPtTOFMax && isParticle);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

// _________________________________________________________
bool StHFCuts::isTOFKaon(StPicoTrack const *trk) const {
  // -- check for good kaon in TOF - in a different pT range than for TPC
  //    if no TOF information on track return false;

  float tofBeta = getTofBeta(trk);
  return isTOFKaon(trk, tofBeta);
}

// _________________________________________________________
bool StHFCuts::isHybridTOFKaon(StPicoTrack const *trk) const {
  // -- check for good kaon in TOF - in a different pT range than for TPC
  //    if no TOF information on track return true;

  float tofBeta = getTofBeta(trk);
  return (tofBeta > 0) ? isTOFKaon(trk, tofBeta) : true;
}

// _________________________________________________________
bool StHFCuts::isTOFKaon(StPicoTrack const *trk, float const & tofBeta) const {
  // -- check for good kaon in TOF - in a different pT range than for TPC

  if (tofBeta <= 0) 
    return false;

  double ptot = trk->dcaGeometry().momentum().mag();
  float  beta = ptot/sqrt(ptot*ptot+M_KAON_PLUS*M_KAON_PLUS);
  bool isParticle = (fabs(1/tofBeta - 1/beta) < mTOFDeltaOneOverBetaKaonMax) ? true : false;
 
  return (trk->gPt() >= mKaonPtTOFMin && trk->gPt() < mKaonPtTOFMax && isParticle);
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

// _________________________________________________________
bool StHFCuts::isTOFProton(StPicoTrack const *trk) const {
  // -- check for good proton in TOF - in a different pT range than for TPC
  //    if no TOF information on track return false;

  float tofBeta = getTofBeta(trk);
  return isTOFProton(trk, tofBeta);
}

// _________________________________________________________
bool StHFCuts::isHybridTOFProton(StPicoTrack const *trk) const {
  // -- check for good proton in TOF - in a different pT range than for TPC
  //    if no TOF information on track return true;

  float tofBeta = getTofBeta(trk);
  return (tofBeta > 0) ? isTOFProton(trk, tofBeta) : true;
}

// _________________________________________________________
bool StHFCuts::isTOFProton(StPicoTrack const *trk, float const & tofBeta) const {
  // -- check for good proton in TOF - in a different pT range than for TPC

  if (tofBeta <= 0) 
    return false;

  double ptot = trk->dcaGeometry().momentum().mag();
  float  beta = ptot/sqrt(ptot*ptot+M_PROTON*M_PROTON);
  bool isParticle = (fabs(1/tofBeta - 1/beta) < mTOFDeltaOneOverBetaProtonMax) ? true : false;
 
  return (trk->gPt() >= mProtonPtTOFMin && trk->gPt() < mProtonPtTOFMax && isParticle);
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

// _________________________________________________________
const float StHFCuts::getTofBeta(StPicoTrack const * const trk) const {
  // -- provide beta of TOF for pico track

  float beta = std::numeric_limits<float>::quiet_NaN();
  
  int index2tof = trk->bTofPidTraitsIndex();
  if(index2tof >= 0) {

    StPicoBTofPidTraits *tofPid = mPicoDst->btofPidTraits(index2tof);
    if(tofPid) {
      beta = tofPid->btofBeta();
      
      if (beta < 1e-4) {
        StThreeVectorF const btofHitPos = tofPid->btofHitPos();
        StPhysicalHelixD helix = trk->helix();
        float pathLength = tofPathLength(&mPrimVtx, &btofHitPos, helix.curvature());
        float tof = tofPid->btof();
        beta = (tof > 0) ? pathLength / (tof * (C_C_LIGHT / 1.e9)) : beta = std::numeric_limits<float>::quiet_NaN();
      }
    }
  }
  
  return beta;
}

#endif // __ROOT__
