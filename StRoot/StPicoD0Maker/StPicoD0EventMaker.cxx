#include <vector>
#include "StPicoD0EventMaker.h"
#include "StCuts.h"

#include "StRoot/StPicoDstMaker/StPicoDst.h"
#include "StRoot/StPicoDstMaker/StPicoTrack.h"
#include "StRoot/StPicoDstMaker/StPicoEvent.h"
#include "StRoot/StPicoDstMaker/StPicoDstMaker.h"
// #include "StRoot/StPicoDstMaker/StPicoTrigger.h"
#include "TTree.h"
#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"
#include "TFile.h"

ClassImp(StPicoD0EventMaker)

//-----------------------------------------------------------------------------
StPicoD0EventMaker::StPicoD0EventMaker(const char* name, StPicoDstMaker *picoMaker, const char* outName)
   : StMaker(name)
{
   mPicoDstMaker = picoMaker;
   mPicoDst = 0;
   mPicoEvent = 0;
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
   /*  */
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
   mPicoD0Event->clear();
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

      bool bKaon = 0;
      bool bPion = 0;

      for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
      {
         StPicoTrack* trk = mPicoDst->track(iTrack);

         if (!trk || !isGoodTrack(trk)) continue;

         bKaon = 0;
         bPion = 0;

         if (isPion(trk))
         {
            idxPicoPions.push_back(iTrack);
            bPion = true;
         }

         if (isKaon(trk))
         {
            idxPicoKaons.push_back(iTrack);
            bKaon = true;
         }
      } // .. end tracks loop

      float const bField = mPicoEvent->bField();
      StThreeVectorF const pVtx = mPicoEvent->primaryVertex();

      for (unsigned short ik = 0; ik < idxPicoKaons.size(); ++ik)
      {
         StPicoTrack* kaon = mPicoDst->track(idxPicoKaons[ik]);

         // make Kπ pairs
         for (unsigned short ip = 0; ip < idxPicoPions.size(); ++ip)
         {
            if (idxPicoKaons[ik] == idxPicoPions[ip]) continue;

            StPicoTrack* pion = mPicoDst->track(idxPicoPions[ip]);
            if (pion->pMom().perp() <= 0.2 || fabs(pion->pMom().pseudoRapidity()) >= cuts::pionEta) continue; // for the Kπ pair we need both to have pT>0.2

            StKaonPion* kaonPion = new StKaonPion(kaon, pion,idxPicoKaons[ik],idxPicoPions[ip],pVtx,bField);

            if (kaonPion->m() <= 0.48 || kaonPion->m() > 2.5)
            {
               delete kaonPion;
               continue;
            }

            mPicoD0Event->addKaonPion(kaonPion);
            delete kaonPion;
         } // .. end make Kπ pairs
      } // .. end of kaons loop


      idxPicoKaons.clear();
      idxPicoPions.clear();
   } //.. end of good event fill

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
bool StPicoD0EventMaker::isGoodTrack(StPicoTrack* trk)
{
   if (trk->nHitsFit() >= cuts::nHitsFit) return true;

   return false;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isPion(StPicoTrack* trk)
{
   // no cut on Eta because the soft pion can have any eta
   if (trk->pMom().perp() >= cuts::pionPt && fabs(trk->nSigmaPion()) < cuts::nSigmaPion) return true;

   return false;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isKaon(StPicoTrack* trk)
{
   if (trk->pMom().perp() >= cuts::kaonPt  && fabs(trk->nSigmaKaon()) < cuts::nSigmaKaon
         && fabs(trk->pMom().pseudoRapidity()) < cuts::kaonEta) return true;

   return false;
}
