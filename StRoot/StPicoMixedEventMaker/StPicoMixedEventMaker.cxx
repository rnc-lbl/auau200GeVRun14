#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

#include "StThreeVectorF.hh"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoBTofPidTraits.h"
#include "StPicoPrescales/StPicoPrescales.h"
#include "StPicoHFMaker/StHFCuts.h"
#include "StPicoMixedEventMaker.h"
#include "StPicoEventMixer.h"

#include <vector>

ClassImp(StPicoMixedEventMaker)

// _________________________________________________________
StPicoMixedEventMaker::StPicoMixedEventMaker(char const* name, StPicoDstMaker* picoMaker, 
		    		     char const* outputBaseFileName,  char const* inputHFListHFtree = "") :
StMaker(name), mPicoDst(NULL), mPicoDstMaker(picoMaker),  mPicoEvent(NULL),
  mPicoEventMixer(NULL),
  mOuputFileBaseName(outputBaseFileName), mInputFileName(inputHFListHFtree),
  mRunId(0), mEventCounter(0), 
  mTree(NULL), mOutputFileTree(NULL){

  mOutputFileTree = new TFile(Form("%s.picoMEtree.root", mOuputFileBaseName.Data()), "RECREATE");
  mOutputFileTree->SetCompressionLevel(1);
  mOutputFileTree->cd();
  // -- create OutputTree
  int BufSize = (int)pow(2., 16.);
  int Split = 1;
  if (!mTree) 
    mTree = new TTree("T", "T", BufSize);
  mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
  //  mTree->Branch("mixedEvent", "StPicoMixedEvent", &mPicoEventMixer, BufSize, Split);
  
  // -- constructor
}

// _________________________________________________________
StPicoMixedEventMaker::~StPicoMixedEventMaker() {
  ;
}
// Method should load Q vector stuff from Hao, needs fixing
// _________________________________________________________
bool StPicoMixedEventMaker::LoadEventPlaneCorr(Int_t const run){
  //needs to implement, will currently break maker
  return false;
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::Init() {
  mOutputFileTree->cd();  
  mPicoEventMixer = new StPicoEventMixer();
  if(!LoadEventPlaneCorr(mRunId)){
    LOG_WARN << "Event plane calculations unavalable! Skipping"<<endm;
    return kStOk;
  }
  
  // -- reset event to be in a defined state
  //resetEvent();

  return kStOK;
}

// _________________________________________________________
Int_t StPicoMixedEventMaker::Finish() {
  // -- Inhertited from StMaker 
  //    NOT TO BE OVERWRITTEN by daughter class
  //    daughter class should implement FinishHF()
  mOutputFileTree->cd();
  mOutputFileTree->Write();
  mPicoEventMixer->FinishMixedEvent();
  mOutputFileTree->Close();

  //mOutputFileList->cd();
  //mOutputFileList->Write(mOutputFileList->GetName(), TObject::kSingleKey);

  //mOutputFileList->Close();

  return kStOK;
}
// _________________________________________________________
void StPicoMixedEventMaker::Clear(Option_t *opt){
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::Make(){
  
  if(!mPicoDstMaker){
    LOG_WARN << "No PicoDstMaker! Skipping! "<<endm;
    return kStWarn;
  }
  
  StPicoDst const * picoDst = mPicoDstMaker->picoDst();
  if (!picoDst){
    LOG_WARN << "No picoDst ! Skipping! "<<endm;
    return kStWarn;
  }
  if( mPicoEventMixer -> addPicoEvent(picoDst,mHFCuts) ==  true )
    mPicoEventMixer->mixEvents(mHFCuts);
  //mTree->Fill();

  return kStOk;
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::SetCategories(){
  return kStOk;
}    
// _________________________________________________________
int StPicoMixedEventMaker::categorize(StPicoDst const * picoDst ){
  StThreeVectorF pVertex = (picoDst->event())->primaryVertex();
  if( fabs(pVertex.z())>6.0 ) return -99;
  int bin = -6.0 + (pVertex.z()+6.0)/1.2;
  return bin;
}
