#include <vector>

#include "TTree.h"
#include "TFile.h"
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
  mBField(0.), mDecayMode(StPicoHFEvent::pair), mMakerMode(0), mTree(NULL), mOutList(NULL),
  mPicoDstMaker(picoMaker), mPicoEvent(NULL), mOutputFile(NULL) {
  
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

  mOutputFile = new TFile(GetName(), "RECREATE");
  mOutputFile->SetCompressionLevel(1);

  if (mMakerMode == StPicoHFEventMaker::write) {
    // -- create OutputTree
    int BufSize = (int)pow(2., 16.);
    int Split = 1;
    if (!mTree) 
      mTree = new TTree("T", "T", BufSize);
    mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
    mTree->Branch("dEvent", "StPicoHFEvent", &mPicoHFEvent, BufSize, Split);
  }

  // -- Add List which holds all histograms
  bool oldStatus = TH1::AddDirectoryStatus();
  TH1::AddDirectory(false);
  
  mOutList = new TList();
  mOutList->SetOwner(true);

  TH1::AddDirectory(oldStatus);

  // -- Call method of daughter class
  InitHF();

  Reset();

  return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoHFEventMaker::Finish() {
  FinishHF();

  mOutputFile->cd();
  mOutputFile->Write();
  mOutList->Write(mOutList->GetName(), TObject::kSingleKey);
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
   if (mMakerMode == StPicoHFEventMaker::write)
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

   const int mEventStatMax = 6;
   int  aEventStat[mEventStatMax];
   
   bool bResult = mHFCuts->IsGoodEvent(mPicoEvent, aEventStat,  mEventStatMax);
   return bResult;

   /*
   return ((mPicoEvent->triggerWord() & mHFCuts->CutTriggerWord()) &&
	   mPicoEvent->ranking() > 0 &&	
	   fabs(mPicoEvent->primaryVertex().z()) < mHFCuts->CutVzMax() &&
	   fabs(mPicoEvent->primaryVertex().z() - mPicoEvent->vzVpd()) < mHFCuts->CutVzVpdVzMax());
   */
}

//-----------------------------------------------------------------------------
float StPicoHFEventMaker::getTofBeta(StPicoTrack const * const trk) const {
   if (Int_t const index2tof = trk->bTofPidTraitsIndex() >= 0) {
     if (StPicoBTofPidTraits const* tofPid = mPicoDst->btofPidTraits(index2tof))
       return tofPid->btofBeta();
   }
   
   return  0.;
}

#if 0

//________________________________________________________________________
void InitializeEventStats() {
  // -- Initialize event statistics histograms
  
  const int    eventStatMax   = 6; 
  const char *aEventCutNames[]   = {"all", "bad run", "trigger", "#it{v}_{z}", "#it{v}_{z}-#it^{VPD}_{z}", "centrality", "accepted"};

  mOutList->Add(new TH1F("hEventStat0","Event cut statistics 0;Event Cuts;Events", eventStatMax, -0.5, eventStatMax-0.5));
  TH1F *hEventStat0 = static_cast<TH1F*>(fOutList->Last());

  mOutList->Add(new TH1F("hEventStat1","Event cut statistics 1;Event Cuts;Events", eventStatMax, -0.5, eventStatMax-0.5));
  TH1F *hEventStat1 = static_cast<TH1F*>(fOutList->Last());

  for (Int_t ii=0; ii < eventStatMax-1; ii++) {
    hEventStat0->GetXaxis()->SetBinLabel(ii+1, aEventCutNames[ii]);
    hEventStat1->GetXaxis()->SetBinLabel(ii+1, aEventCutNames[ii]);
  }

  //  hEventStat0->GetXaxis()->SetBinLabel(fHEventStatMax, Form("Centrality [0-%s]%%", aCentralityMaxNames[9-1]));
  //  hEventStat1->GetXaxis()->SetBinLabel(fHEventStatMax, Form("Centrality [0-%s]%%", aCentralityMaxNames[9-1]));
}



//________________________________________________________________________
void InitializeCentralityStats() {
  // -- Initialize trigger statistics histograms
  


  fOutList->Add(new TH1F("hCentralityStat","Centrality statistics;Centrality Bins;Events", fNCentralityBins,-0.5,fNCentralityBins-0.5));
  TH1F* hCentralityStat = static_cast<TH1F*>(fOutList->Last());
  
  for (Int_t ii=0; ii < fNCentralityBins; ii++)
    hCentralityStat->GetXaxis()->SetBinLabel(ii+1, aCentralityNames[ii]);

  fOutList->Add(new TH1F("hRefMult2Stat","RefMult2 Statistics; RefMult2;Events", 351, 0., 350.));
  fOutList->Add(new TH1F("hRefMult2CorrStat","RefMult2Corr Statistics; RefMult2;Events", 351, 0., 350.));
}
#endif
