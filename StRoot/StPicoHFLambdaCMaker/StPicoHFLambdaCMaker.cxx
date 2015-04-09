#include <vector>

#include "TNtuple.h"
#include "TClonesArray.h"

#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"
#include "phys_constants.h"

#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoBTofPidTraits.h"

#include "StPicoHFMaker/StPicoHFEvent.h"
#include "StPicoHFMaker/StHFCuts.h"
#include "StPicoHFMaker/StHFPair.h"
#include "StPicoHFMaker/StHFTriplet.h"

#include "StPicoHFLambdaCMaker.h"

ClassImp(StPicoHFLambdaCMaker)

// _________________________________________________________
StPicoHFLambdaCMaker::StPicoHFLambdaCMaker(char const* name, StPicoDstMaker* picoMaker, char const* outputBaseFileName,  
					   char const* inputHFListHFtree = "") :
  StPicoHFMaker(name, picoMaker, outputBaseFileName, inputHFListHFtree),
  mDecayChannel(kPionKaonProton), mNtupleSecondary(NULL), mNtupleTertiary(NULL) {
  // constructor
}

// _________________________________________________________
StPicoHFLambdaCMaker::~StPicoHFLambdaCMaker() {
  // destructor
}

// _________________________________________________________
int StPicoHFLambdaCMaker::InitHF() {

  mNtupleSecondary = new TNtuple("secondary", "secondary", "p1pt:p2pt:charges:m:pt:eta:phi:cosPntAngle:dLength:p1Dca:p2Dca:cosThetaStar:dcaDaugthers");

  if (isDecayMode() == StPicoHFEvent::kTwoAndTwoParticleDecay)
    mNtupleTertiary = new TNtuple("tertiary", "tertiary", "p1pt:p2pt:charges:m:pt:eta:phi:cosPntAngle:dLength:p1Dca:p2Dca:cosThetaStar:dcaDaugthers"); 

  return kStOK;
}

// _________________________________________________________
int StPicoHFLambdaCMaker::FinishHF() {
  
  mNtupleSecondary->Write();
  
  if (isDecayMode() == StPicoHFEvent::kTwoAndTwoParticleDecay)
    mNtupleTertiary->Write();
   
  return kStOK;
}

// _________________________________________________________
int StPicoHFLambdaCMaker::MakeHF() {

  if (isMakerMode() == StPicoHFMaker::kWrite) {
    createCandidates();
  }
  else if (isMakerMode() == StPicoHFMaker::kRead) {
    analyseCandidates();
  }
  else if (isMakerMode() == StPicoHFMaker::kAnalyse) {
    createCandidates();
    analyseCandidates();
  }

  return kStOK;
}

// _________________________________________________________
int StPicoHFLambdaCMaker::createCandidates() {
  // create candidate pairs/ triplet and fill them in arrays (in StPicoHFEvent)

  // -- Decay channel proton - K0Short (pi+ - pi-)
  if (mDecayChannel == StPicoHFLambdaCMaker::kProtonK0short) {

    // cout << " N pions    : " << mIdxPicoPions.size()  << endl;
    // cout << " N kaons    : " << mIdxPicoKaons.size()  << endl;
    // cout << " N protons  : " << mIdxPicoProtons.size()  << endl;

    createTertiaryK0Shorts();

    if (mPicoHFEvent->nHFTertiaryVertices() > 0) {
      TClonesArray const * ak0Short = mPicoHFEvent->aHFTertiaryVertices();
      
      for (unsigned int idxProton = 0; idxProton < mIdxPicoProtons.size(); ++idxProton) {
	StPicoTrack const *proton = mPicoDst->track(mIdxPicoProtons[idxProton]);
	
	for (unsigned int idxK0Short = 0; idxK0Short < mPicoHFEvent->nHFTertiaryVertices(); ++idxK0Short) {
	  StHFPair const* k0Short = static_cast<StHFPair*>(ak0Short->At(idxK0Short));
	  
	  if (mIdxPicoProtons[idxProton] == k0Short->particle1Idx() || mIdxPicoProtons[idxProton] == k0Short->particle1Idx()) 
	    continue;
	  
	  // -- JMT UPDATE tertiary vertex
	  StHFPair lambdaC(proton, k0Short, M_PROTON, M_KAON_0_SHORT, 
			   mIdxPicoProtons[idxProton], idxK0Short, mPrimVtx, mBField);
	  if (!mHFCuts->isGoodSecondaryVertexPair(lambdaC)) 
	    continue;
	  
	  mPicoHFEvent->addHFSecondaryVertexPair(&lambdaC);
	  
	} // for (unsigned int idxK0Short = 0; idxK0Short <  mPicoHFEvent->nHFTertiaryVertices(); ++idxK0Short) {
      } // for (unsigned int idxProton = 0; idxProton < mIdxPicoProtons.size(); ++idxProton) {
    } //  if (mPicoHFEvent->nHFTertiaryVertices() > 0) {

    // cout << "      N K0Shorts : " << mPicoHFEvent->nHFTertiaryVertices() << endl;
    // cout << "      N Lambda_C : " << mPicoHFEvent->nHFSecondaryVertices() << endl;

  } // if (mDecayChannel == StPicoHFLambdaCMaker::kProtonK0short) {

  // -- Decay channel proton pi+ K-
  else  if (mDecayChannel == StPicoHFLambdaCMaker::kPionKaonProton) {
    for (unsigned short idxProton = 0; idxProton < mIdxPicoProtons.size(); ++idxProton) {
      StPicoTrack const *proton = mPicoDst->track(mIdxPicoProtons[idxProton]);
      
      for (unsigned short idxKaon = 0; idxKaon < mIdxPicoKaons.size(); ++idxKaon) {
	StPicoTrack const *kaon = mPicoDst->track(mIdxPicoKaons[idxKaon]);
	
	if (mIdxPicoKaons[idxKaon] == mIdxPicoProtons[idxProton]) 
	  continue;
	
	StHFPair tmpProtonKaon(kaon, proton, M_KAON_PLUS, M_PROTON, 
			       mIdxPicoKaons[idxKaon], mIdxPicoProtons[idxProton], mPrimVtx, mBField);
	if (!mHFCuts->isClosePair(tmpProtonKaon)) 
	  continue;
	
  	for (unsigned short idxPion = 0; idxPion < mIdxPicoPions.size(); ++idxPion) {
	  StPicoTrack const *pion = mPicoDst->track(mIdxPicoPions[idxPion]);
	  
	  if (mIdxPicoProtons[idxProton] == mIdxPicoPions[idxPion] || mIdxPicoKaons[idxKaon] == mIdxPicoPions[idxPion]) 
	    continue;
	  
	  StHFTriplet triplet(kaon, proton, pion, M_KAON_PLUS, M_PROTON, M_PION_PLUS, 
			      mIdxPicoKaons[idxKaon], mIdxPicoProtons[idxProton], mIdxPicoPions[idxPion], mPrimVtx, mBField);
	  if (!mHFCuts->isGoodSecondaryVertexTriplet(triplet)) 
	    continue;
	  
	  mPicoHFEvent->addHFSecondaryVertexTriplet(&triplet);
	  
	} // for (unsigned short idxPion = 0; idxPion < mIdxPicoPions.size(); ++idxPion) {
      } // for (unsigned short idxKaon = 0; idxKaon < mIdxPicoKaons.size(); ++idxKaon) {
    } // for (unsigned short idxProton = 0; idxProton < mIdxPicoProtons.size(); ++idxProton) {
  } // else  if (mDecayChannel == StPicoHFLambdaCMaker::kPionKaonProton) {

 return kStOK;
}

// _________________________________________________________
int StPicoHFLambdaCMaker::analyseCandidates() {
  // -- analyse pairs/triplet, which have been filled in arrays 
  //    (in StPicoHFEvent) before ( via createCandidates() or via readCandidates() )
  //    fill histograms or nTuples ... etc

  // -- Decay channel proton - K0Short (pi+ - pi-)
  if (mDecayChannel == StPicoHFLambdaCMaker::kProtonK0short) {

    // -- fill nTuple / hists for tertiary K0shorts 
    // -----------------------------------------------
    TClonesArray const * aK0Short = mPicoHFEvent->aHFTertiaryVertices();

    for (unsigned int idxK0Short = 0; idxK0Short < mPicoHFEvent->nHFTertiaryVertices(); ++idxK0Short) {
      StHFPair const* k0Short = static_cast<StHFPair*>(aK0Short->At(idxK0Short));

      StPicoTrack const* pion1  = mPicoDst->track(k0Short->particle1Idx());
      StPicoTrack const* pion2  = mPicoDst->track(k0Short->particle2Idx());
      
      if (!isPion(pion1) || !isPion(pion1))
	continue;

      if (!mHFCuts->isGoodTertiaryVertexPair(k0Short)) 
	continue;
      
      mNtupleTertiary->Fill(pion1->gPt(), pion2->gPt(), pion1->charge()*pion2->charge(),
			    k0Short->m(), k0Short->pt(), k0Short->eta(), k0Short->phi(), std::cos(k0Short->pointingAngle()),
			    k0Short->decayLength(), k0Short->particle1Dca(), k0Short->particle2Dca(),  
			    k0Short->cosThetaStar(), k0Short->dcaDaughters());
			    			 
    } // for (unsigned int idxK0Short = 0; idxK0Short <  mPicoHFEvent->nHFTertiaryVertices(); ++idxK0Short) {


    // -- fill nTuple / hists for secondary lambdaCs
    // -----------------------------------------------
    TClonesArray const * aLambdaC = mPicoHFEvent->aHFSecondaryVertices();
    
    for (unsigned int idxLambdaC = 0; idxLambdaC < mPicoHFEvent->nHFSecondaryVertices(); ++idxLambdaC) {
      StHFPair const* lambdaC = static_cast<StHFPair*>(aLambdaC->At(idxLambdaC));
      StHFPair const* k0Short = static_cast<StHFPair*>(aK0Short->At(lambdaC->particle2Idx()));

      StPicoTrack const* proton = mPicoDst->track(lambdaC->particle1Idx());
      StPicoTrack const* pion1  = mPicoDst->track(k0Short->particle1Idx());
      StPicoTrack const* pion2  = mPicoDst->track(k0Short->particle2Idx());

      if (!isProton(proton) || !isPion(pion1) || !isPion(pion1))
	continue;
      
      // if (pion1->charge() == pion2->charge())
      // 	continue;

      // JMT - recalculate topological cuts with updated secondary vertex
      
      if (!mHFCuts->isGoodTertiaryVertexPair(k0Short)) 
	continue;

      if (!mHFCuts->isGoodSecondaryVertexPair(lambdaC)) 
	continue;
     
      mNtupleSecondary->Fill(proton->gPt(), k0Short->pt(), pion1->charge()*pion2->charge(),
			     lambdaC->m(), lambdaC->pt(), lambdaC->eta(), lambdaC->phi(), std::cos(lambdaC->pointingAngle()),
			     lambdaC->decayLength(), lambdaC->particle1Dca(), lambdaC->particle2Dca(), 
			     lambdaC->cosThetaStar(), lambdaC->dcaDaughters());
            
    } // for (unsigned int idxLambdaC = 0; idxLambdaC <  mPicoHFEvent->nHFSecondaryVertices(); ++idxLambdaC) {
  } // if (mDecayChannel == StPicoHFLambdaCMaker::kProtonK0short) {

  // -- Decay channel proton pi+ K-
  else  if (mDecayChannel == StPicoHFLambdaCMaker::kPionKaonProton) {
    
    // -- fill nTuple / hists for secondary lambdaCs
    // -----------------------------------------------
    TClonesArray const * aLambdaC = mPicoHFEvent->aHFSecondaryVertices();
    
    for (unsigned int idxLambdaC = 0; idxLambdaC <  mPicoHFEvent->nHFSecondaryVertices(); ++idxLambdaC) {
      StHFTriplet const* lambdaC = static_cast<StHFTriplet*>(aLambdaC->At(idxLambdaC));

      StPicoTrack const* proton = mPicoDst->track(lambdaC->particle1Idx());
      StPicoTrack const* kaon   = mPicoDst->track(lambdaC->particle2Idx());
      StPicoTrack const* pion   = mPicoDst->track(lambdaC->particle3Idx());

      if (!isProton(proton) || !isKaon(kaon) || !isPion(pion))
	continue;
      
      // JMT - recalculate topological cuts with updated secondary vertex
      
      if (!mHFCuts->isGoodSecondaryVertexTriplet(lambdaC)) 
	continue;
     
      float isCorrectSign = (kaon->charge() != pion->charge() && pion->charge() == proton->charge()) ? 1. : -1.;

      mNtupleSecondary->Fill(proton->gPt(), kaon->gPt(), pion->gPt(), isCorrectSign,
			     lambdaC->m(), lambdaC->pt(), lambdaC->eta(), lambdaC->phi(), lambdaC->pointingAngle(),
			     lambdaC->decayLength(), lambdaC->particle1Dca(), lambdaC->particle2Dca(), lambdaC->particle3Dca(),
			     lambdaC->cosThetaStar());
      //			     lambdaC->dcaDaughters12(), lambdaC->dcaDaughters23(), lambdaC->dcaDaughters31());
      
    } // for (unsigned int idxLambdaC = 0; idxLambdaC <  mPicoHFEvent->nHFSecondaryVertices(); ++idxLambdaC) {
  } // else  if (mDecayChannel == StPicoHFLambdaCMaker::kPionKaonProton) {

 return kStOK;
}
  
// _________________________________________________________
bool StPicoHFLambdaCMaker::isPion(StPicoTrack const * const trk) const {
  // -- is good pion 
  float tofBeta = 0;
  return isPion(trk, tofBeta);
}

// _________________________________________________________
bool StPicoHFLambdaCMaker::isPion(StPicoTrack const * const trk, float const & tofBeta = 0) const {
  // -- is good pion 
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCPion(trk)); 
}

// _________________________________________________________
bool StPicoHFLambdaCMaker::isKaon(StPicoTrack const * const trk) const {
  // -- is good kaon 
  float tofBeta = getTofBeta(trk);
  return isKaon(trk, tofBeta);
} 

// _________________________________________________________
bool StPicoHFLambdaCMaker::isKaon(StPicoTrack const * const trk, float const & tofBeta =0) const {
  // -- is good kaon
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCKaon(trk) && mHFCuts->isTOFKaon(trk, tofBeta));
} 

// _________________________________________________________
bool StPicoHFLambdaCMaker::isProton(StPicoTrack const * const trk) const {
  // -- good proton
  float tofBeta = getTofBeta(trk);
  return isProton(trk, tofBeta);
}

// _________________________________________________________
bool StPicoHFLambdaCMaker::isProton(StPicoTrack const * const trk, float const & tofBeta = 0) const {
  // -- good proton
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCProton(trk) && mHFCuts->isTOFProton(trk, tofBeta));
}

