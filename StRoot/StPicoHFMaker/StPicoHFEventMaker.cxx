#include <vector>

#include "TTree.h"
#include "TFile.h"
#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"
#include "../StPicoDstMaker/StPicoDst.h"
#include "../StPicoDstMaker/StPicoDstMaker.h"
#include "../StPicoDstMaker/StPicoEvent.h"
#include "../StPicoDstMaker/StPicoTrack.h"
#include "../StPicoDstMaker/StPicoBTofPidTraits.h"
#include "StPicoHFEvent.h"
#include "StPicoHFEventMaker.h"

#include "StHFCuts.h"
#include "StHFPair.h"
#include "StHFSecondaryPair.h"
#include "StHFTriplet.h"
#include "phys_constants.h"

ClassImp(StPicoHFEventMaker)

//-----------------------------------------------------------------------------
StPicoHFEventMaker::StPicoHFEventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName)
: StMaker(name), mPicoDst(NULL), mHFCuts(NULL), mPicoHFEvent(NULL), 
  mBField(0.), mDecayMode(StPicoHFEvent::pair), mMakerMode(0),
  mPicoDstMaker(picoMaker), mPicoEvent(NULL), mOutputFile(NULL), mTree(NULL) {
  
   mOutputFile = new TFile(outName, "RECREATE");
   mOutputFile->SetCompressionLevel(1);
}

 //-----------------------------------------------------------------------------
 StPicoHFEventMaker::~StPicoHFEventMaker() {
    /* mTree is owned by mOutputFile directory, it will be destructed once
     * the file is closed in ::Finish() */
 }

 //-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::Init() {
  if (!mHFCuts)
    mHFCuts = StHFCuts::Instance();
  
  mPicoHFEvent = new StPicoHFEvent(mDecayMode);
  
  // -- create OutputTree
  int BufSize = (int)pow(2., 16.);
  int Split = 1;
  if (!mTree) 
    mTree = new TTree("T", "T", BufSize);
  mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
  mTree->Branch("dEvent", "StPicoHFEvent", &mPicoHFEvent, BufSize, Split);
  
  //   mOutputList = new TList();
  
  Reset();
  InitHF();
  return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::Finish() {
  FinishHF();

  mOutputFile->cd();
  mOutputFile->Write();
  mOutputFile->Close();
  return kStOK;
}

//-----------------------------------------------------------------------------
void StPicoHFEventMaker::Reset() {
  mIdxPicoPions.clear();
  mIdxPicoKaons.clear();
  mIdxPicoProtons.clear();
  
  mPicoHFEvent->clear("C");
}

//-----------------------------------------------------------------------------
void StPicoHFEventMaker::Clear(Option_t *opt) {
  Reset();
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::Make() {
   if (!mPicoDstMaker)
   {
      LOG_WARN << " No PicoDstMaker! Skip! " << endm;
      return kStWarn;
   }

   mPicoDst = mPicoDstMaker->picoDst();
   if (!mPicoDst)
   {
      LOG_WARN << " No PicoDst! Skip! " << endm;
      return kStWarn;
   }

   Int_t iReturn = kStOK;

   if (isGoodEvent())
   {
      UInt_t nTracks = mPicoDst->numberOfTracks();

      for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
      {
         StPicoTrack* trk = mPicoDst->track(iTrack);

         if (!trk || !mHFCuts->IsGoodTrack(trk)) continue;

         float const beta = getTofBeta(trk);
         if (isPion(trk, beta))   mIdxPicoPions.push_back(iTrack);
         if (isKaon(trk, beta))   mIdxPicoKaons.push_back(iTrack);
         if (isProton(trk, beta)) mIdxPicoProtons.push_back(iTrack);

      } // .. end tracks loop

     iReturn = MakeHF();
      
   } //.. end of good event fill

   // This should never be inside the good event block
   // because we want to save information about all events, good or bad
   mTree->Fill();

   Reset();

   return (kStOK && iReturn);
}

//-----------------------------------------------------------------------------
void StPicoHFEventMaker::CreateSecondaryK0Short() {
  // -- Create candidate secondary K0shorts

  for (unsigned short idxPion1 = 0; idxPion1 < mIdxPicoPions.size(); ++idxPion1) {
    StPicoTrack const * pion1 = mPicoDst->track(mIdxPicoPions[idxPion1]);

    for (unsigned short idxPion2 = idxPion1+1 ; idxPion2 < mIdxPicoPions.size(); ++idxPion2) {
      StPicoTrack const * pion2 = mPicoDst->track(mIdxPicoPions[idxPion2]);      

      if (mIdxPicoPions[idxPion1] == mIdxPicoPions[idxPion2]) 
	continue;

      StHFSecondaryPair candidateK0Short(pion1, pion2, M_PION_PLUS, M_PION_MINUS, 
					 mIdxPicoPions[idxPion1], mIdxPicoPions[idxPion2], 
					 mPrimVtx, mBField);
      if (!mHFCuts->IsGoodSecondaryPair(candidateK0Short)) 
	continue;
      
      mPicoHFEvent->addHFSecondary(&candidateK0Short);
    }
  }

}

//-----------------------------------------------------------------------------
bool StPicoHFEventMaker::isGoodEvent() {
   mPicoEvent = mPicoDst->event();
   mPicoHFEvent->addPicoEvent(*mPicoEvent);
   
   mBField = mPicoEvent->bField();
   mPrimVtx = mPicoEvent->primaryVertex();

   return ((mPicoEvent->triggerWord() & mHFCuts->CutTriggerWord()) &&
	   mPicoEvent->ranking() > 0 &&	
	   fabs(mPicoEvent->primaryVertex().z()) < mHFCuts->CutVzMax() &&
	   fabs(mPicoEvent->primaryVertex().z() - mPicoEvent->vzVpd()) < mHFCuts->CutVzVpdVzMax());
}

//-----------------------------------------------------------------------------
float StPicoHFEventMaker::getTofBeta(StPicoTrack const * const trk) const {
   if (Int_t const index2tof = trk->bTofPidTraitsIndex() >= 0) {
     if (StPicoBTofPidTraits const* tofPid = mPicoDst->btofPidTraits(index2tof))
       return tofPid->btofBeta();
   }
   
   return  0.;
}

