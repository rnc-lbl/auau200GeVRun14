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
   mTree->SetAutoSave(1000000); // autosave every 10 Mbytes
   mTree->Branch("dEvent", "StDmesonEvent", &mPicoD0Event, BufSize, Split);
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

      std::vector<Int_t> idPicoDstKaons;
      std::vector<Int_t> idPicoDstPions;

      bool bKaon = 0;
      bool bPion = 0;

      for (UInt_t iTrack = 0; iTrack < nTracks; iTrack++)
      {
         StPicoTrack* trk = mPicoDst->track(iTrack);

         if (!trk || !isGoodTrack(trk)) continue;

         bKaon = 0;
         bPion = 0;

         if (isPion(trk))
         {
            idPicoDstPions.push_back(iTrack);
            bPion = true;
         }

         if (isKaon(trk))
         {
            idPicoDstKaons.push_back(iTrack);
            bKaon = true;
         }
      } // .. end tracks loop

      for (UInt_t ik = 0; ik < idPicoDstKaons.size(); ik++)
      {
         StPicoTrack* kaon = mPicoDst->track(idPicoDstKaons[ik]);
         // make Kπ pairs
         for (UInt_t ip = 0; ip < idPicoDstPions.size(); ip++)
         {
            if (idPicoDstKaons[ik] == idPicoDstPions[ip]) continue;

            StPicoTrack* pion = mPicoDst->track(idPicoDstPions[ip]);
            if (pion->pMom().perp() <= 0.2 || fabs(pion->pMom().pseudoRapidity()) >= cuts::pionEta) continue; // for the Kπ pair we need both to have pT>0.2

            StKaonPion* kaonPion = new StKaonPion(kaon, pion);

            if (kaonPion->m() <= 0.48 || kaonPion->m() > 2.5)
            {
               delete kaonPion;
               continue;
            }

            mPicoD0Event->addKaonPion(kaonPion);
            delete kaonPion;
         } // .. end make Kπ pairs
      } // .. end of kaons loop


      idPicoDstKaons.clear();
      idPicoDstPions.clear();
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
   Float_t vz = mPicoEvent->primaryVertex().z();
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
