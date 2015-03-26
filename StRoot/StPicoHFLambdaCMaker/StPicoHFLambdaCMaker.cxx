#include <vector>

#include "TTree.h"
#include "TFile.h"
#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"
#include "phys_constants.h"

#include "../StPicoDstMaker/StPicoDst.h"
#include "../StPicoDstMaker/StPicoDstMaker.h"
#include "../StPicoDstMaker/StPicoEvent.h"
#include "../StPicoDstMaker/StPicoTrack.h"
#include "../StPicoDstMaker/StPicoBTofPidTraits.h"

#include "../StPicoHFMaker/StPicoHFEvent.h"
#include "../StPicoHFMaker/StHFCuts.h"
#include "../StPicoHFMaker/StHFPair.h"
#include "../StPicoHFMaker/StHFTriplet.h"

#include "StPicoHFLambdaCMaker.h"

ClassImp(StPicoHFLambdaCMaker)

//-----------------------------------------------------------------------------
StPicoHFLambdaCMaker::StPicoHFLambdaCMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName)
: StPicoHFEventMaker(name, picoMaker, outName) {
  // constructor

}

//-----------------------------------------------------------------------------
StPicoHFLambdaCMaker::~StPicoHFLambdaCMaker() {
  // destructor
}


//-----------------------------------------------------------------------------
Int_t StPicoHFLambdaCMaker::InitHF() {
  return kStOK;
}


//-----------------------------------------------------------------------------
Int_t StPicoHFLambdaCMaker::FinishHF() {
  return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoHFLambdaCMaker::MakeHF() {

  // if (mDecayChannel == StPicoHFLambdaCMaker::K0short_Proton)
  //  CreateSecondaryK0Short();



  for (unsigned short idxProton = 0; idxProton < mIdxPicoProtons.size(); ++idxProton) {
      StPicoTrack const *proton = mPicoDst->track(mIdxPicoProtons[idxProton]);

      for (unsigned short idxKaon = 0; idxKaon < mIdxPicoKaons.size(); ++idxKaon) {
	StPicoTrack const *kaon = mPicoDst->track(mIdxPicoKaons[idxKaon]);
	
	if (mIdxPicoKaons[idxKaon] == mIdxPicoProtons[idxProton]) 
	  continue;

	StHFPair tmpProtonKaon(kaon, proton, M_KAON_PLUS, M_PROTON, 
			       mIdxPicoKaons[idxKaon], mIdxPicoProtons[idxProton], mPrimVtx, mBField);
	if (!mHFCuts->IsGoodPrimaryPair(tmpProtonKaon)) 
	  continue;

  	for (unsigned short idxPion = 0; idxPion < mIdxPicoPions.size(); ++idxPion) {
	  StPicoTrack const *pion = mPicoDst->track(mIdxPicoPions[idxPion]);

	  if (mIdxPicoProtons[idxProton] == mIdxPicoPions[idxPion] || mIdxPicoKaons[idxKaon] == mIdxPicoPions[idxPion]) 
	    continue;

	  StHFTriplet triplet(kaon, proton, pion, M_KAON_PLUS, M_PROTON, M_PION_PLUS, 
			      mIdxPicoKaons[idxKaon], mIdxPicoProtons[idxProton], mIdxPicoPions[idxPion], mPrimVtx, mBField);
	  if (!mHFCuts->IsGoodTriplet(triplet)) 
	    continue;

	  mPicoHFEvent->addHFPrimary(&triplet);

	} // for (unsigned short idxPion = 0; idxPion < mIdxPicoPions.size(); ++idxPion) {
      } // for (unsigned short idxKaon = 0; idxKaon < mIdxPicoKaons.size(); ++idxKaon) {
  } // for (unsigned short idxProton = 0; idxProton < mIdxPicoProtons.size(); ++idxProton) {


  return kStOK;
}


//-----------------------------------------------------------------------------
bool StPicoHFLambdaCMaker::isPion(StPicoTrack const * const trk, float const & bTofBeta) const {
  // JMT add ETA cut

  
  return (trk->pMom().perp() >= 0.2 && fabs(trk->nSigmaPion()) <2.5) ? true : false;
}

//-----------------------------------------------------------------------------
bool StPicoHFLambdaCMaker::isKaon(StPicoTrack const * const trk, float const & bTofBeta) const {
  // JMT add ETA cut
  return (trk->pMom().perp() >= 0.2 && fabs(trk->nSigmaKaon()) <2.5) ? true : false;
}

//-----------------------------------------------------------------------------
bool StPicoHFLambdaCMaker::isProton(StPicoTrack const * const trk, float const & bTofBeta) const {
  // JMT add ETA cut
  return (trk->pMom().perp() >= 0.2 && fabs(trk->nSigmaProton()) <2.5) ? true : false;
}

