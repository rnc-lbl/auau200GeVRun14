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

#if 0
  for (unsigned short ik = 0; ik < mIdxPicoKaons.size(); ++ik) {
      StPicoTrack const * kaon = mPicoDst->track(mIdxPicoKaons[ik]);
      
      // make Kπ pairs
      for (unsigned short ip = 0; ip < mIdxPicoPions.size(); ++ip) {
	if (mIdxPicoKaons[ik] == mIdxPicoPions[ip]) continue;
	  
	  StPicoTrack const * pion = mPicoDst->track(mIdxPicoPions[ip]);
	  
	  StHFPair kaonPion(kaon, pion, M_KAON_PLUS, M_PION_PLUS, mIdxPicoKaons[ik], mIdxPicoPions[ip], mPrimVtx, mBField);
	  
	  if (!mHFCuts->IsGoodPrimaryPair(kaonPion)) continue;
	  
	  mPicoHFEvent->addHFPrimary(&kaonPion);
	} // .. end make Kπ pairs
    } // .. end of kaons loop
#endif  

  return kStOK;
}


//-----------------------------------------------------------------------------
bool StPicoHFLambdaCMaker::isPion(StPicoTrack const * const trk, float const & bTofBeta) const {
  // JMT add ETA cut
  return true; //(trk->pMom().perp() >= hfcuts::pionPt && fabs(trk->nSigmaPion()) < hfcuts::nSigmaPion) ? true : false;
}

//-----------------------------------------------------------------------------
bool StPicoHFLambdaCMaker::isKaon(StPicoTrack const * const trk, float const & bTofBeta) const {
  // JMT add ETA cut
  return true ; //(trk->pMom().perp() >= hfcuts::kaonPt  && fabs(trk->nSigmaKaon()) < hfcuts::nSigmaKaon) ? true : false;
}

//-----------------------------------------------------------------------------
bool StPicoHFLambdaCMaker::isProton(StPicoTrack const * const trk, float const & bTofBeta) const {
  // JMT add ETA cut
  return true; //(trk->pMom().perp() >= hfcuts::protonPt  && fabs(trk->nSigmaProton()) < hfcuts::nSigmaProton) ? true: false;
}

