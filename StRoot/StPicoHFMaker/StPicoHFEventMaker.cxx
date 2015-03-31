#include <vector>

#include "TTree.h"
#include "TFile.h"

#include "phys_constants.h"
#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"
#include "../StPicoDstMaker/StPicoDst.h"
#include "../StPicoDstMaker/StPicoDstMaker.h"
#include "../StPicoDstMaker/StPicoEvent.h"
#include "../StPicoDstMaker/StPicoTrack.h"
#include "../StPicoDstMaker/StPicoBTofPidTraits.h"

#include "StHFCuts.h"
#include "StPicoHFEvent.h"
#include "StPicoHFEventMaker.h"
#include "StHFPair.h"
#include "StHFTriplet.h"

ClassImp(StPicoHFEventMaker)

// _________________________________________________________
StPicoHFEventMaker::StPicoHFEventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName)
: StMaker(name), mPicoDst(NULL), mHFCuts(NULL), mPicoHFEvent(NULL), mBField(0.), 
  mDecayMode(StPicoHFEvent::kTwoParticleDecay), mMakerMode(StPicoHFEventMaker::kAnalyse), 
  mTree(NULL), mOutList(NULL),
  mPicoDstMaker(picoMaker), mPicoEvent(NULL), mOutputFile(NULL) {
  // -- constructor
}

// _________________________________________________________
StPicoHFEventMaker::~StPicoHFEventMaker() {
   // -- destructor 
  
  if (mHFCuts)
    delete mHFCuts;
  mHFCuts = NULL;

  /* mTree is owned by mOutputFile directory, it will be destructed once
   * the file is closed in ::Finish() */
}

// _________________________________________________________
Int_t StPicoHFEventMaker::Init() {
  // -- Inhertited from StMaker 
  //    NOT TO BE OVERWRITTEN by daughter class
  //    daughter class should implement InitHF()

  // -- check for cut class
  if (!mHFCuts)
    mHFCuts = new StHFCuts;
  
  // -- create HF event - using the proper decay mode to initialize
  mPicoHFEvent = new StPicoHFEvent(mDecayMode);

  mOutputFile = new TFile(GetName(), "RECREATE");
  mOutputFile->SetCompressionLevel(1);

  if (mMakerMode == StPicoHFEventMaker::kWrite) {
    // -- create OutputTree
    int BufSize = (int)pow(2., 16.);
    int Split = 1;
    if (!mTree) 
      mTree = new TTree("T", "T", BufSize);
    mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
    mTree->Branch("dEvent", "StPicoHFEvent", &mPicoHFEvent, BufSize, Split);
  }

  // -- add List which holds all histograms
  bool oldStatus = TH1::AddDirectoryStatus();
  TH1::AddDirectory(false);
  
  mOutList = new TList();
  mOutList->SetOwner(true);

  TH1::AddDirectory(oldStatus);

  // -- create event stat histograms
  initializeEventStats();

  // -- call method of daughter class
  InitHF();

  // -- reset event to be in a defined state
  reset();

  return kStOK;
}

// _________________________________________________________
Int_t StPicoHFEventMaker::Finish() {
  // -- Inhertited from StMaker 
  //    NOT TO BE OVERWRITTEN by daughter class
  //    daughter class should implement FinishHF()

  mOutputFile->cd();
  mOutputFile->Write();
  mOutList->Write(mOutList->GetName(), TObject::kSingleKey);

  // -- call method of daughter class
  FinishHF();

  mOutputFile->Close();

  return kStOK;
}

// _________________________________________________________
void StPicoHFEventMaker::reset() {
  // -- reset event

  mIdxPicoPions.clear();
  mIdxPicoKaons.clear();
  mIdxPicoProtons.clear();
  
  mPicoHFEvent->clear("C");
}

// _________________________________________________________
void StPicoHFEventMaker::Clear(Option_t *opt) {
  // -- Inhertited from StMaker 
  //    NOT TO BE OVERWRITTEN by daughter class
  //    daughter class should implement ClearHF()

  // -- call method of daughter class
  ClearHF();

  reset();
}

// _________________________________________________________
Int_t StPicoHFEventMaker::Make() {
  // -- Inhertited from StMaker 
  //    NOT TO BE OVERWRITTEN by daughter class
  //    daughter class should implement MakeHF()
  // -- isPion, isKaon, isProton methods are to be 
  //    implemented by daughter class (
  //    -> methods of StHFCuts can and should be used

  if (!mPicoDstMaker) {
    LOG_WARN << " No PicoDstMaker! Skip! " << endm;
    return kStWarn;
  }

  mPicoDst = mPicoDstMaker->picoDst();
  if (!mPicoDst) {
    LOG_WARN << " No PicoDst! Skip! " << endm;
    return kStWarn;
  }
  
  Int_t iReturn = kStOK;
  
  if (setupEvent()) {
    UInt_t nTracks = mPicoDst->numberOfTracks();
    
    for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack) {
      StPicoTrack* trk = mPicoDst->track(iTrack);

      if (!trk || !mHFCuts->isGoodTrack(trk)) continue;

      float const beta = getTofBeta(trk);
      if (isPion(trk, beta))   mIdxPicoPions.push_back(iTrack);   // isPion method to be implemented by daughter class
      if (isKaon(trk, beta))   mIdxPicoKaons.push_back(iTrack);   // isKaon method to be implemented by daughter class
      if (isProton(trk, beta)) mIdxPicoProtons.push_back(iTrack); // isProton method to be implemented by daughter class
      
    } // .. end tracks loop

    // -- call method of daughter class
    iReturn = MakeHF();
    
  } // .. end of good event fill
  
  // -- save information about all events, good or bad
  if (mMakerMode == StPicoHFEventMaker::kWrite)
    mTree->Fill();
  
  // -- reset event to be in a defined state
  reset();
  
  return (kStOK && iReturn);
}

// _________________________________________________________
void StPicoHFEventMaker::createTertiaryK0Shorts() {
  // -- Create candidate for tertiary K0shorts

  for (unsigned short idxPion1 = 0; idxPion1 < mIdxPicoPions.size(); ++idxPion1) {
    StPicoTrack const * pion1 = mPicoDst->track(mIdxPicoPions[idxPion1]);

    for (unsigned short idxPion2 = idxPion1+1 ; idxPion2 < mIdxPicoPions.size(); ++idxPion2) {
      StPicoTrack const * pion2 = mPicoDst->track(mIdxPicoPions[idxPion2]);      

      if (mIdxPicoPions[idxPion1] == mIdxPicoPions[idxPion2]) 
	continue;

      StHFPair candidateK0Short(pion1, pion2, 
				M_PION_PLUS, M_PION_MINUS, 
				mIdxPicoPions[idxPion1], mIdxPicoPions[idxPion2], 
				mPrimVtx, mBField);
      if (!mHFCuts->isGoodTertiaryVertexPair(candidateK0Short)) 
	continue;
      
      mPicoHFEvent->addHFTertiaryVertexPair(&candidateK0Short);
    }
  }
}

// _________________________________________________________
bool StPicoHFEventMaker::setupEvent() {
  // -- fill members from pico event, check for good eventa and fill event statistics

  mPicoEvent = mPicoDst->event();
  mPicoHFEvent->addPicoEvent(*mPicoEvent);
  
  mBField = mPicoEvent->bField();
  mPrimVtx = mPicoEvent->primaryVertex();
  
  int aEventStat[mHFCuts->eventStatMax()];
  
  bool bResult = mHFCuts->isGoodEvent(mPicoEvent, aEventStat);

  // -- fill event statistics histograms
  fillEventStats(aEventStat);

  return bResult;
}

// _________________________________________________________
float StPicoHFEventMaker::getTofBeta(StPicoTrack const * const trk) const {
  // -- provide beta of TOF for pico track

  if (Int_t const index2tof = trk->bTofPidTraitsIndex() >= 0) {
    if (StPicoBTofPidTraits const* tofPid = mPicoDst->btofPidTraits(index2tof))
      return tofPid->btofBeta();
  }
  
  return  0.;
}

// _________________________________________________________
void StPicoHFEventMaker::initializeEventStats() {
  // -- Initialize event statistics histograms
  
  const char *aEventCutNames[]   = {"all", "bad run", "trigger", "#it{v}_{z}", "#it{v}_{z}-#it^{VPD}_{z}", "centrality", "accepted"};

  mOutList->Add(new TH1F("hEventStat0","Event cut statistics 0;Event Cuts;Events", mHFCuts->eventStatMax(), -0.5, mHFCuts->eventStatMax()-0.5));
  TH1F *hEventStat0 = static_cast<TH1F*>(mOutList->Last());

  mOutList->Add(new TH1F("hEventStat1","Event cut statistics 1;Event Cuts;Events", mHFCuts->eventStatMax(), -0.5, mHFCuts->eventStatMax()-0.5));
  TH1F *hEventStat1 = static_cast<TH1F*>(mOutList->Last());

  for (unsigned int ii = 0; ii < mHFCuts->eventStatMax()-1; ii++) {
    hEventStat0->GetXaxis()->SetBinLabel(ii+1, aEventCutNames[ii]);
    hEventStat1->GetXaxis()->SetBinLabel(ii+1, aEventCutNames[ii]);
  }

  //  hEventStat0->GetXaxis()->SetBinLabel(fHEventStatMax, Form("Centrality [0-%s]%%", aCentralityMaxNames[9-1]));
  //  hEventStat1->GetXaxis()->SetBinLabel(fHEventStatMax, Form("Centrality [0-%s]%%", aCentralityMaxNames[9-1]));
}

//________________________________________________________________________
void StPicoHFEventMaker::fillEventStats(int *aEventStat) {
  // -- Fill event statistics 

  TH1F *hEventStat0 = static_cast<TH1F*>(mOutList->FindObject("hEventStat0"));
  TH1F *hEventStat1 = static_cast<TH1F*>(mOutList->FindObject("hEventStat1"));

  for (unsigned int idx = 0; idx < mHFCuts->eventStatMax() ; ++idx) {
    if (!aEventStat[idx])
      hEventStat0->Fill(idx);
  }
  
  for (unsigned int idx = 0; idx < mHFCuts->eventStatMax(); ++idx) {
    if (aEventStat[idx])
      break;
    hEventStat1->Fill(idx);
  }
}
