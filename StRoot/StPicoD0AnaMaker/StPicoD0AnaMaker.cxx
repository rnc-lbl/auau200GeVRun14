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
#include "StPicoD0Maker/StPicoD0Event.h"
#include "StPicoD0Maker/StKaonPion.h"
#include "StPicoD0AnaMaker.h"

ClassImp(StPicoD0AnaMaker)

StPicoD0AnaMaker::StPicoD0AnaMaker(char const * name,char const * inputFilesList, char const * outName): StMaker(name),
   mPicoD0Event(NULL), mOutFileName(outName), mInputFileList(inputFilesList),
   mOutputFile(NULL), mChain(NULL), mNtuple(NULL), mEventCounter(0)
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
         mChain->Add(file.c_str());
         LOG_INFO << "StPicoD0AnaMaker - Adding :" << file << endm;
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
   mOutputFile->Close();

   return kStOK;
}
//-----------------------------------------------------------------------------
Int_t StPicoD0AnaMaker::Make()
{
   // this line need to be moved into its own function
   mChain->GetEntry(mEventCounter++);

   /*if(mPicoD0Event->eventId() != picoDst->event()->eventId())
   {
     cout<<"SOMETHING TERRIBLE JUST HAPPENED. StPicoEvent and StPicoD0Event are not in sync"<<endl;
     exit(1);
   }
   */

   TClonesArray const * aKaonPion = mPicoD0Event->kaonPionArray();

   for (int idx = 0; idx < aKaonPion->GetEntriesFast(); ++idx)
   {
      StKaonPion const* kp = (StKaonPion*)aKaonPion->At(idx);
      // StPicoTrack const* kaon = picoDst->track(kp->kaonIdx());
      // StPicoTrack const* pion = picoDst->track(kp->pionIdx());

      // mNtuple->Fill(kaon->pMom().perp(), pion->pMom().perp(), kaon->charge()*pion->charge(), kp->m(), kp->pt(), kp->eta(), kp->phi(), kp->pointingAngle(),
      mNtuple->Fill(-1,-1,-1, kp->m(), kp->pt(), kp->eta(), kp->phi(), kp->pointingAngle(),
                    kp->decayLength(), kp->kaonDca(), kp->pionDca(), kp->dcaDaughters(), kp->cosThetaStar());
   }

   return kStOK;
}
