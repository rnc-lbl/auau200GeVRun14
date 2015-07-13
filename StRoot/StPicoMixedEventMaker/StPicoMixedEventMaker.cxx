#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TH1.h"


#include "StarClassLibrary/StThreeVectorF.hh"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoBTofPidTraits.h"
#include "StPicoPrescales/StPicoPrescales.h"
#include "StPicoMixedEventMaker.h"
#include "StPicoEventMixer.h"
#include "StRoot/StRefMultCorr/StRefMultCorr.h"

#include <vector>

ClassImp(StPicoMixedEventMaker)

// _________________________________________________________
StPicoMixedEventMaker::StPicoMixedEventMaker(char const* name, StPicoDstMaker* picoMaker, StRefMultCorr* grefmultCorrUtil,
        char const* outputBaseFileName,  char const* inputHFListHFtree = "") :
    StMaker(name), mPicoDst(NULL), mPicoDstMaker(picoMaker),  mPicoEvent(NULL),
    mGRefMultCorrUtil(grefmultCorrUtil),
    mOuputFileBaseName(outputBaseFileName), mInputFileName(inputHFListHFtree),
    mEventCounter(0), mTree(NULL), mOutputFileTree(NULL) {

  TH1::AddDirectory(false);

    mOutputFileTree = new TFile(Form("%s.picoMEtree.root", mOuputFileBaseName.Data()), "RECREATE");
    mOutputFileTree->SetCompressionLevel(1);
    mOutputFileTree->cd();
    // -- create OutputTree
    int BufSize = (int)pow(2., 16.);
    int Split = 1;
    //if (!mTree)
      //    mTree = new TTree("T", "T", BufSize);
    //mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
    //  mTree->Branch("mixedEvent", "StPicoMixedEvent", &mPicoEventMixer, BufSize, Split);

    // -- constructor
}

// _________________________________________________________
StPicoMixedEventMaker::~StPicoMixedEventMaker() {
  cout <<"Lomnitz:: Destroying"<<endl;
  delete mGRefMultCorrUtil;
  for(int iVz =0 ; iVz < 10 ; ++iVz){
    for(int iCentrality = 0 ; iCentrality < 9 ; ++iCentrality){
      delete mPicoEventMixer[iVz][iCentrality];
    }
  }
  //mOutputFileTree->Write();
  mOutputFileTree->Close();
}
// Method should load Q vector stuff from Hao, needs fixing
// _________________________________________________________
bool StPicoMixedEventMaker::loadEventPlaneCorr(Int_t const run) {
    //needs to implement, will currently break maker
    return false;
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::Init() {
    mOutputFileTree->cd();
    for(int iVz =0 ; iVz < 10 ; ++iVz){
      for(int iCentrality = 0 ; iCentrality < 9 ; ++iCentrality){
	mPicoEventMixer[iVz][iCentrality] = new StPicoEventMixer(Form("Cent_%i_Vz_%i",iCentrality,iVz));
	mPicoEventMixer[iVz][iCentrality]->setEventBuffer(10);
      }
    }
    mGRefMultCorrUtil = new StRefMultCorr("grefmult");
    // if(!LoadEventPlaneCorr(mRunId)){
    // LOG_WARN << "Event plane calculations unavalable! Skipping"<<endm;
    // return kStOk;
    // }

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
    for(int iVz =0 ; iVz < 10 ; ++iVz){
      for(int iCentrality = 0 ; iCentrality < 9 ; ++iCentrality){
	mPicoEventMixer[iVz][iCentrality]->finish();
	//delete mPicoEventMixer[iVz][iCentrality];
      }
    }
    //mOutputFileTree->Write()

    //mOutputFileList->cd();
    //mOutputFileList->Write(mOutputFileList->GetName(), TObject::kSingleKey);

    //mOutputFileList->Close();

    return kStOK;
}
// _________________________________________________________
void StPicoMixedEventMaker::Clear(Option_t* opt) {
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::Make() {

    if(!mPicoDstMaker) {
        LOG_WARN << "No PicoDstMaker! Skipping! "<<endm;
        return kStWarn;
    }

    StPicoDst const* picoDst = mPicoDstMaker->picoDst();
    if (!picoDst) {
        LOG_WARN << "No picoDst ! Skipping! "<<endm;
        return kStWarn;
    }
    // - GRef from Guannan
    if(!mGRefMultCorrUtil) {
        LOG_WARN << " No mGRefMultCorrUtil! Skip! " << endl;
        return kStWarn;
    }
    //Lomnitz, need to fix this bs
   StThreeVectorF const pVtx = picoDst->event()->primaryVertex();
    if( fabs(pVtx.z()) >=6.0 )
      return kStOk;
    mGRefMultCorrUtil->init(picoDst->event()->runId());
    mGRefMultCorrUtil->initEvent(picoDst->event()->grefMult(),pVtx.z(),picoDst->event()->ZDCx()) ;
    int const centrality  = mGRefMultCorrUtil->getCentralityBin9();
    if(centrality < 0 || centrality >8 ) return kStOk;
    int const vz_bin = (int)((6 +pVtx.z())/1.2) ;
//     4            55-60%            30-40%
//     5            50-55%            20-30%
//     6            45-50%            10-20%
    //cout<<"Centrality: "<<centrality<<endl;
    // - - -
    if( mPicoEventMixer[vz_bin][centrality] -> addPicoEvent(picoDst) ==  true )
      mPicoEventMixer[vz_bin][centrality]->mixEvents();
    //mTree->Fill();

    return kStOk;
}
// _________________________________________________________
Int_t StPicoMixedEventMaker::SetCategories() {
    return kStOk;
}
// _________________________________________________________
int StPicoMixedEventMaker::categorize(StPicoDst const * picoDst ) {
    StThreeVectorF pVertex = (picoDst->event())->primaryVertex();
    if( fabs(pVertex.z())>6.0 ) return -99;
    int bin = -6.0 + (pVertex.z()+6.0)/1.2;
    return bin;
}
