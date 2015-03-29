#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TNtuple.h"

#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoD0EventMaker/StPicoD0Event.h"
#include "StPicoD0EventMaker/StKaonPion.h"
#include "StPicoD0AnaMaker.h"
#include "StCuts.h"

ClassImp(StPicoD0AnaMaker)

StPicoD0AnaMaker::StPicoD0AnaMaker(char const * name,char const * inputFilesList, 
    char const * outName,StPicoDstMaker* picoDstMaker): 
  StMaker(name),mPicoDstMaker(picoDstMaker),mPicoD0Event(NULL), mOutFileName(outName), mInputFileList(inputFilesList),
   mOutputFile(NULL), mChain(NULL), mEventCounter(0), mNtuple(NULL)
{}

Int_t StPicoD0AnaMaker::Init()
{
   mPicoD0Event = new StPicoD0Event();

   mChain = new TChain("T");
   std::ifstream listOfFiles(mInputFileList.Data());
   if (listOfFiles.is_open())
   {
      std::string file;
      while (getline(listOfFiles, file))
      {
         LOG_INFO << "StPicoD0AnaMaker - Adding :" << file << endm;
         mChain->Add(file.c_str());
      }
   }
   else
   {
      LOG_ERROR << "StPicoD0AnaMaker - Could not open list of files. ABORT!" << endm;
      return kStErr;
   }

   mChain->GetBranch("dEvent")->SetAutoDelete(kFALSE);
   mChain->SetBranchAddress("dEvent", &mPicoD0Event);

   mOutputFile = new TFile(mOutFileName.Data(), "RECREATE");
   mOutputFile->cd();


   // -------------- USER VARIABLES -------------------------
   mNtuple = new TNtuple("nt", "", "pt1:pt2:flag:m:pt:eta:phi:theta:"
                         "decayL:kDca:pDca:dca12:cosThetaStar");
   return kStOK;
}
//-----------------------------------------------------------------------------
StPicoD0AnaMaker::~StPicoD0AnaMaker()
{
   /*  */
}
//-----------------------------------------------------------------------------
Int_t StPicoD0AnaMaker::Finish()
{
   LOG_INFO << " StPicoD0AnaMaker - writing data and closing output file " <<endm;
   mOutputFile->cd();
   mNtuple->Write();
   mOutputFile->Close();

   return kStOK;
}
//-----------------------------------------------------------------------------
Int_t StPicoD0AnaMaker::Make()
{
   readNextEvent();

   if (!mPicoDstMaker)
   {
      LOG_WARN << " StPicoD0AnaMaker - No PicoDstMaker! Skip! " << endm;
      return kStWarn;
   }

   StPicoDst const* picoDst = mPicoDstMaker->picoDst();

   if (!picoDst)
   {
      LOG_WARN << "StPicoD0AnaMaker - No PicoDst! Skip! " << endm;
      return kStWarn;
   }

   if(mPicoD0Event->runId() != picoDst->event()->runId() ||
       mPicoD0Event->eventId() != picoDst->event()->eventId())
   {
     LOG_ERROR <<" StPicoD0AnaMaker - !!!!!!!!!!!! ATTENTION !!!!!!!!!!!!!"<<endm;
     LOG_ERROR <<" StPicoD0AnaMaker - SOMETHING TERRIBLE JUST HAPPENED. StPicoEvent and StPicoD0Event are not in sync."<<endm;
     exit(1);
   }

   // -------------- USER ANALYSIS -------------------------
   TClonesArray const * aKaonPion = mPicoD0Event->kaonPionArray();

   for (int idx = 0; idx < aKaonPion->GetEntries(); ++idx)
   {
      // this is an example of how to get the kaonPion pairs and their corresponsing tracks
      StKaonPion const* kp = (StKaonPion*)aKaonPion->At(idx);
      if(!isGoodPair(kp)) continue;

      StPicoTrack const* kaon = picoDst->track(kp->kaonIdx());
      StPicoTrack const* pion = picoDst->track(kp->pionIdx());

      mNtuple->Fill(kaon->gPt(), pion->gPt(), kaon->charge()*pion->charge(), kp->m(), kp->pt(), kp->eta(), kp->phi(), kp->pointingAngle(),
                    kp->decayLength(), kp->kaonDca(), kp->pionDca(), kp->dcaDaughters(), kp->cosThetaStar());
   }

   return kStOK;
}
//-----------------------------------------------------------------------------
bool StPicoD0AnaMaker::isGoodPair(StKaonPion const* const kp) const
{
  if(!kp) return false;

  StPicoTrack const* kaon = mPicoDstMaker->picoDst()->track(kp->kaonIdx());
  StPicoTrack const* pion = mPicoDstMaker->picoDst()->track(kp->pionIdx());

  bool tracking = kaon->nHitsFit() > cuts::nHitsFit && pion->nHitsFit() > cuts::nHitsFit;

  return tracking && fabs(kaon->nSigmaKaon()) < cuts::nSigmaKaon &&
         fabs(pion->nSigmaPion()) < cuts::nSigmaPion && 
         kp->m() > cuts::minMass && kp->m() < cuts::maxMass &&
         std::cos(kp->pointingAngle()) > cuts::cosTheta &&
         kp->decayLength() > cuts::decayLength &&
         kp->dcaDaughters() < cuts::dcaDaughters;
}
