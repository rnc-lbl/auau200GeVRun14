#include <vector>

#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

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
StPicoHFEventMaker::StPicoHFEventMaker(char const* name, StPicoDstMaker* picoMaker, 
				       char const* outputBaseFileName,  char const* inputHFListHFtree = "") :
  StMaker(name), mPicoDst(NULL), mHFCuts(NULL), mPicoHFEvent(NULL), mBField(0.), mOutList(NULL),
  mDecayMode(StPicoHFEvent::kTwoParticleDecay), mMakerMode(StPicoHFEventMaker::kAnalyse), 
  mOuputFileBaseName(outputBaseFileName), mInputFileName(inputHFListHFtree),
  mPicoDstMaker(picoMaker), mPicoEvent(NULL), mTree(NULL), mHFChain(NULL), mEventCounter(0), 
  mOutputFileTree(NULL), mOutputFileList(NULL) {
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
 
  // -- READ ------------------------------------
  if (mMakerMode == StPicoHFEventMaker::kRead) {

    if (!mHFChain) {
      mHFChain = new TChain("T");
      std::ifstream listOfFiles(mInputFileName.Data());
      if (listOfFiles.is_open()) {
	std::string file;
	while (getline(listOfFiles, file)) {
	  LOG_INFO << " StPicoHFEventMaker - Adding :" << file << endm;
	  mHFChain->Add(file.c_str());
	}
      }
      else { 
	LOG_ERROR << " StPicoHFEventMaker - Could not open list of files. ABORT!" << endm;
	return kStErr;
      }
    } // if (mMakerMode == StPicoHFEventMaker::kRead) {

    mHFChain->GetBranch("hfEvent")->SetAutoDelete(kFALSE);
    mHFChain->SetBranchAddress("hfEvent", &mPicoHFEvent);
  }
  
  // -- file which holds list of histograms
  mOutputFileList = new TFile(Form("%s.%s.root", mOuputFileBaseName.Data(), GetName()), "RECREATE");
  mOutputFileList->SetCompressionLevel(1);

  if (mMakerMode == StPicoHFEventMaker::kWrite) {
    mOutputFileTree = new TFile(Form("%s.picoHFtree.root", mOuputFileBaseName.Data()), "RECREATE");
    mOutputFileTree->SetCompressionLevel(1);
    mOutputFileTree->cd();

    // -- create OutputTree
    int BufSize = (int)pow(2., 16.);
    int Split = 1;
    if (!mTree) 
      mTree = new TTree("T", "T", BufSize);
    mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
    mTree->Branch("hfEvent", "StPicoHFEvent", &mPicoHFEvent, BufSize, Split);
  } // if (mMakerMode == StPicoHFEventMaker::kWrite) {

  // -- disable automatic adding of objects to file
  bool oldStatus = TH1::AddDirectoryStatus();
  TH1::AddDirectory(false);

  // -- add list which holds all histograms  
  mOutList = new TList();
  mOutList->SetOwner(true);

  // -- create event stat histograms
  initializeEventStats();

  // -- call method of daughter class
  InitHF();

  TH1::AddDirectory(oldStatus);

  // -- reset event to be in a defined state
  resetEvent();

  return kStOK;
}

// _________________________________________________________
Int_t StPicoHFEventMaker::Finish() {
  // -- Inhertited from StMaker 
  //    NOT TO BE OVERWRITTEN by daughter class
  //    daughter class should implement FinishHF()

  mOutputFileTree->cd();
  mOutputFileTree->Write();
  mOutputFileTree->Close();

  mOutputFileList->cd();
  mOutList->Write(mOutList->GetName(), TObject::kSingleKey);

  // -- call method of daughter class
  FinishHF();

  mOutputFileList->Close();

  return kStOK;
}

// _________________________________________________________
void StPicoHFEventMaker::resetEvent() {
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

  resetEvent();
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
    LOG_WARN << " StPicoHFEventMaker - No PicoDstMaker! Skip! " << endm;
    return kStWarn;
  }

  mPicoDst = mPicoDstMaker->picoDst();
  if (!mPicoDst) {
    LOG_WARN << " StPicoHFEventMaker - No PicoDst! Skip! " << endm;
    return kStWarn;
  }
  
  // -- read in HF tree
  if (mMakerMode == StPicoHFEventMaker::kRead) {
    mHFChain->GetEntry(mEventCounter++);

    if (mPicoHFEvent->runId() != mPicoDst->event()->runId() || mPicoHFEvent->eventId() != mPicoDst->event()->eventId()) {
      LOG_ERROR <<" StPicoHFEventMaker - !!!!!!!!!!!! ATTENTION !!!!!!!!!!!!!"<<endm;
      LOG_ERROR <<" StPicoHFEventMaker - SOMETHING TERRIBLE JUST HAPPENED. StPicoEvent and StPicoHFEvent are not in sync."<<endm;
      exit(1);
    }
  } // if (mMakerMode == StPicoHFEventMaker::kRead) {
  

  Int_t iReturn = kStOK;
  
  if (setupEvent()) {
    UInt_t nTracks = mPicoDst->numberOfTracks();
    
    // -- Fill vectors of particle types
    if (mMakerMode == StPicoHFEventMaker::kWrite || mMakerMode == StPicoHFEventMaker::kAnalyse) {
      for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack) {
	StPicoTrack* trk = mPicoDst->track(iTrack);
	
	if (!trk || !mHFCuts->isGoodTrack(trk)) continue;
	
	float const beta = getTofBeta(trk);
	if (isPion(trk, beta))   mIdxPicoPions.push_back(iTrack);   // isPion method to be implemented by daughter class
	if (isKaon(trk, beta))   mIdxPicoKaons.push_back(iTrack);   // isKaon method to be implemented by daughter class
	if (isProton(trk, beta)) mIdxPicoProtons.push_back(iTrack); // isProton method to be implemented by daughter class
      
      } // .. end tracks loop
    } // if (mMakerMode == StPicoHFEventMaker::kWrite || mMakerMode == StPicoHFEventMaker::kAnalyse) {

    // -- call method of daughter class
    iReturn = MakeHF();
    
  } // if (setupEvent()) {
  
  // -- save information about all events, good or bad
  if (mMakerMode == StPicoHFEventMaker::kWrite)
    mTree->Fill();
  
  // -- reset event to be in a defined state
  resetEvent();
  
  return (kStOK && iReturn);
}

// _________________________________________________________
void StPicoHFEventMaker::createTertiaryK0Shorts() {
  // -- Create candidate for tertiary K0shorts
  //    only store pairs with opposite charge

  for (unsigned short idxPion1 = 0; idxPion1 < mIdxPicoPions.size(); ++idxPion1) {
    StPicoTrack const * pion1 = mPicoDst->track(mIdxPicoPions[idxPion1]);

    for (unsigned short idxPion2 = idxPion1+1 ; idxPion2 < mIdxPicoPions.size(); ++idxPion2) {
      StPicoTrack const * pion2 = mPicoDst->track(mIdxPicoPions[idxPion2]);      

      if (mIdxPicoPions[idxPion1] == mIdxPicoPions[idxPion2]) 
	continue;

      StHFPair candidateK0Short(pion1, pion2, M_PION_PLUS, M_PION_MINUS, 
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
