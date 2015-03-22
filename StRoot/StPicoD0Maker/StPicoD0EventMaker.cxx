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
#include "StPicoD0Event.h"
#include "StPicoD0EventMaker.h"
#include "StCuts.h"

ClassImp(StPicoD0EventMaker)

//-----------------------------------------------------------------------------
StPicoD0EventMaker::StPicoD0EventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName)
   : StMaker(name), mPicoDstMaker(picoMaker), mPicoDst(NULL), mPicoEvent(NULL)
{
   mPicoD0Event = new StPicoD0Event();

   mOutputFile = new TFile(outName, "RECREATE");
   mOutputFile->SetCompressionLevel(1);
   int BufSize = (int)pow(2., 16.);
   int Split = 1;
   mTree = new TTree("T", "T", BufSize);
   mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
   mTree->Branch("dEvent", "StPicoD0Event", &mPicoD0Event, BufSize, Split);
}

//-----------------------------------------------------------------------------
StPicoD0EventMaker::~StPicoD0EventMaker()
{
   /* mTree is owned by mOutputFile directory, it will be destructed once
    * the file is closed in ::Finish() */
}

//-----------------------------------------------------------------------------
Int_t StPicoD0EventMaker::Init()
{
   return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoD0EventMaker::Finish()
{
   mOutputFile->cd();
   mOutputFile->Write();
   mOutputFile->Close();
   return kStOK;
}
//-----------------------------------------------------------------------------
void StPicoD0EventMaker::Clear(Option_t *opt)
{
   mPicoD0Event->clear("C");
}

//-----------------------------------------------------------------------------
Int_t StPicoD0EventMaker::Make()
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

      std::vector<unsigned short> idxPicoKaons;
      std::vector<unsigned short> idxPicoPions;

      for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
      {
         StPicoTrack* trk = mPicoDst->track(iTrack);

         if (!trk || !isGoodTrack(trk)) continue;

         float const beta = getTofBeta(trk);

         if (isPion(trk, beta)) idxPicoPions.push_back(iTrack);
         if (isKaon(trk, beta)) idxPicoKaons.push_back(iTrack);

      } // .. end tracks loop

      float const bField = mPicoEvent->bField();
      StThreeVectorF const pVtx = mPicoEvent->primaryVertex();

      for (unsigned short ik = 0; ik < idxPicoKaons.size(); ++ik)
      {
         StPicoTrack const * kaon = mPicoDst->track(idxPicoKaons[ik]);

         // make Kπ pairs
         for (unsigned short ip = 0; ip < idxPicoPions.size(); ++ip)
         {
            if (idxPicoKaons[ik] == idxPicoPions[ip]) continue;

            StPicoTrack const * pion = mPicoDst->track(idxPicoPions[ip]);

            StKaonPion kaonPion(kaon, pion, idxPicoKaons[ik], idxPicoPions[ip], pVtx, bField);

            if (!isGoodPair(kaonPion)) continue;

            mPicoD0Event->addKaonPion(&kaonPion);
         } // .. end make Kπ pairs
      } // .. end of kaons loop

      idxPicoKaons.clear();
      idxPicoPions.clear();
   } //.. end of good event fill

   // This should never be inside the good event block
   // because we want to save information about all events, good or bad
   mTree->Fill();
   mPicoD0Event->clear("C");

   return kStOK;
}

//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isGoodEvent()
{
   mPicoEvent = mPicoDst->event();
   mPicoD0Event->addPicoEvent(*mPicoEvent);

   // cuts
   float vz = mPicoEvent->primaryVertex().z();
   if (!(mPicoEvent->ranking() > 0)) return false;
   if (fabs(vz) > cuts::vz) return false;

   return true;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isGoodTrack(StPicoTrack const * const trk) const
{
   // Require at least one hit on every layer of PXL and IST.
   // It is done here for tests on the preview II data.
   // The new StPicoTrack which is used in official production has a method to check this
   if (trk->nHitsFit() >= cuts::nHitsFit
         && (!cuts::requireHFT || trk->nHitsMapHFT() & 0xB)) return true;

   return false;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isPion(StPicoTrack const * const trk, float const & bTofBeta) const
{
   // no cut on Eta because the soft pion can have any eta
   if (trk->pMom().perp() >= cuts::pionPt && fabs(trk->nSigmaPion()) < cuts::nSigmaPion) return true;

   return false;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isKaon(StPicoTrack const * const trk, float const & bTofBeta) const
{
   if (trk->pMom().perp() >= cuts::kaonPt  && fabs(trk->nSigmaKaon()) < cuts::nSigmaKaon) return true;

   return false;
}
//-----------------------------------------------------------------------------
float StPicoD0EventMaker::getTofBeta(StPicoTrack const * const trk) const
{
   if (Int_t const index2tof = trk->bTofPidTraitsIndex() >= 0)
   {
      if (StPicoBTofPidTraits const* tofPid = mPicoDst->btofPidTraits(index2tof))
         return tofPid->btofBeta();
   }

   return  0.;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isGoodPair(StKaonPion const & kp) const
{
   if (kp.dcaDaughters() <  cuts::dcaDaughters &&
         kp.decayLength() > cuts::decayLength &&
         kp.m() > cuts::minMass &&
         kp.m() < cuts::maxMass) return true;

   return false;
}
