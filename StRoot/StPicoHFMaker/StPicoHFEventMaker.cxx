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
#include "StCuts.h"

ClassImp(StPicoHFEventMaker)

//-----------------------------------------------------------------------------
StPicoHFEventMaker::StPicoHFEventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName)
   : StMaker(name), mPicoDstMaker(picoMaker), mPicoDst(NULL), mPicoEvent(NULL)
{
   mPicoHFEvent = new StPicoHFEvent();

   mOutputFile = new TFile(outName, "RECREATE");
   mOutputFile->SetCompressionLevel(1);
   int BufSize = (int)pow(2., 16.);
   int Split = 1;
   mTree = new TTree("T", "T", BufSize);
   mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
   mTree->Branch("dEvent", "StPicoHFEvent", &mPicoHFEvent, BufSize, Split);
}

//-----------------------------------------------------------------------------
StPicoHFEventMaker::~StPicoHFEventMaker()
{
   /* mTree is owned by mOutputFile directory, it will be destructed once
    * the file is closed in ::Finish() */
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::Init()
{
  Reset();
  InitHF();
  return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::InitHF()
{
  return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::Finish()
{
  FinishHF();
  mOutputFile->cd();
  mOutputFile->Write();
  mOutputFile->Close();
  return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::Finish()
{
  return kStOK;
}

//-----------------------------------------------------------------------------
void StPicoHFEventMaker::Reset()
{
  mIdxPicoPions.clear();
  mIdxPicoKaons.clear();
  mIdxPicoKaons.clear();
  
  mPicoHFEvent->clear("C");
}


//-----------------------------------------------------------------------------
void StPicoHFEventMaker::Clear(Option_t *opt)
{
  Reset();
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::Make()
{
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

   if (isGoodEvent())
   {
      UInt_t nTracks = mPicoDst->numberOfTracks();

      for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
      {
         StPicoTrack* trk = mPicoDst->track(iTrack);

         if (!trk || !isGoodTrack(trk)) continue;

         float const beta = getTofBeta(trk);

         if (isPion(trk, beta))   mIdxPicoPions.push_back(iTrack);
         if (isKaon(trk, beta))   mIdxPicoKaons.push_back(iTrack);
         if (isProton(trk, beta)) mIdxPicoProtons.push_back(iTrack);

      } // .. end tracks loop

      In_t iReturn = MakeHF();
      
   } //.. end of good event fill

   // This should never be inside the good event block
   // because we want to save information about all events, good or bad
   mTree->Fill();

   Reset();

   return (kStOK && iReturn);
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::MakeHF()
{

  for (unsigned short ik = 0; ik < mIdxPicoKaons.size(); ++ik)
    {
      StPicoTrack const * kaon = mPicoDst->track(mIdxPicoKaons[ik]);
      
      // make Kπ pairs
      for (unsigned short ip = 0; ip < mIdxPicoPions.size(); ++ip)
	{
	  if (mIdxPicoKaons[ik] == mIdxPicoPions[ip]) continue;
	  
	  StPicoTrack const * pion = mPicoDst->track(mIdxPicoPions[ip]);
	  
	  StKaonPion kaonPion(kaon, pion, mIdxPicoKaons[ik], mIdxPicoPions[ip], mPrimVtx, mBField);
	  
	  if (!isGoodPair(kaonPion)) continue;
	  
	  mPicoHFEvent->addKaonPion(&kaonPion);
	} // .. end make Kπ pairs
    } // .. end of kaons loop
  
  return kStOK;
}

//-----------------------------------------------------------------------------
bool StPicoHFEventMaker::isGoodEvent()
{
   mPicoEvent = mPicoDst->event();
   mPicoHFEvent->addPicoEvent(*mPicoEvent);
   
   mBField = mPicoEvent->bField();
   mPrimVtx = mPicoEvent->primaryVertex();


   // cuts
   float vz = mPicoEvent->primaryVertex().z();
   if (!(mPicoEvent->ranking() > 0)) return false;
   if (fabs(vz) > hfcuts::vz) return false;

   return true;
}
//-----------------------------------------------------------------------------
bool StPicoHFEventMaker::isGoodTrack(StPicoTrack const * const trk) const
{
   // Require at least one hit on every layer of PXL and IST.
   // It is done here for tests on the preview II data.
   // The new StPicoTrack which is used in official production has a method to check this
   if (trk->nHitsFit() >= hfcuts::nHitsFit
         && (!hfcuts::requireHFT || trk->nHitsMapHFT() & 0xB)) return true;

   return false;
}
//-----------------------------------------------------------------------------
bool StPicoHFEventMaker::isPion(StPicoTrack const * const trk, float const & bTofBeta) const
{
  // add ETA cut
   if (trk->pMom().perp() >= hfcuts::pionPt && fabs(trk->nSigmaPion()) < hfcuts::nSigmaPion) return true;

   return false;
}
//-----------------------------------------------------------------------------
bool StPicoHFEventMaker::isKaon(StPicoTrack const * const trk, float const & bTofBeta) const
{
  // add ETA cut
   if (trk->pMom().perp() >= hfcuts::kaonPt  && fabs(trk->nSigmaKaon()) < hfcuts::nSigmaKaon) return true;

   return false;
}
//-----------------------------------------------------------------------------
bool StPicoHFEventMaker::isProton(StPicoTrack const * const trk, float const & bTofBeta) const
{
  // add ETA cut
   if (trk->pMom().perp() >= hfcuts::protonPt  && fabs(trk->nSigmaProton()) < hfcuts::nSigmaProton) return true;

   return false;
}
//-----------------------------------------------------------------------------
float StPicoHFEventMaker::getTofBeta(StPicoTrack const * const trk) const
{
   if (Int_t const index2tof = trk->bTofPidTraitsIndex() >= 0)
   {
      if (StPicoBTofPidTraits const* tofPid = mPicoDst->btofPidTraits(index2tof))
         return tofPid->btofBeta();
   }

   return  0.;
}
//-----------------------------------------------------------------------------
bool StPicoHFEventMaker::isGoodPair(StKaonPion const & kp) const
{
   if (kp.dcaDaughters() <  hfcuts::dcaDaughters &&
         kp.decayLength() > hfcuts::decayLength &&
         kp.m() > hfcuts::minMass &&
         kp.m() < hfcuts::maxMass) return true;

   return false;
}
//-----------------------------------------------------------------------------
bool StPicoHFEventMaker::isGoodTriple(StKaonPion const & kp) const
{
  /* if (kp.dcaDaughters() <  hfcuts::dcaDaughters &&
         kp.decayLength() > hfcuts::decayLength &&
         kp.m() > hfcuts::minMass &&
         kp.m() < hfcuts::maxMass) return true;
  */
  // FILL ME

   return false;
}
