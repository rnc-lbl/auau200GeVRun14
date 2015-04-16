#include <cmath>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TString.h"
#include "../StPicoDstMaker/StPicoEvent.h"
#include "../StPicoPrescales/StPicoPrescales.h"
#include "StPicoHFEvent.h"
#include "StHFPair.h"
#include "StHFTriplet.h"

#include "StHFHists.h"

class StPicoPrescales;
ClassImp(StHFHists)


StHFHists::StHFHists() : TNamed("StHFHists", "StHFHists"),
  mEventList(NULL), mPairList(NULL), mTripletList(NULL), mPrescales(NULL), mNRuns(0) {
}


StHFHists::StHFHists(const char* name) : TNamed(name, name),
  mEventList(NULL),  mPairList(NULL),  mTripletList(NULL), mPrescales(NULL), mNRuns(0){
}


StHFHists::~StHFHists()
{

  if (mPrescales)
    delete mPrescales;
  mPrescales = NULL;
  // note that histograms are owned by mOutFile. They will be destructed 
  // when the file is closed.
}

void StHFHists::init (TList * outList){
  // -- init method to set up internal lists /hists

  // path to lists of triggers prescales
  // lists are obtained from http://www.star.bnl.gov/protected/common/common2014/trigger2014/plots_au200gev/
  const char * prescalesFilesDirectoryName = "./run14AuAu200GeVPrescales";
  mPrescales = new StPicoPrescales(prescalesFilesDirectoryName); // fix dir name
  mNRuns = mPrescales->numberOfRuns();
  
  // -- event list
  outList->Add(new TList);
  mEventList = static_cast<TList*>(outList->Last());
  mEventList->SetOwner(kTRUE);
  mEventList->SetName("baseHFEventHists");
  // -- create event hists
  mEventList->Add(new TH1F("mh1TotalEventsInRun","totalEventsInRun;runIndex;totalEventsInRun",mNRuns+1,0,mNRuns+1));
  //  mEventList->Add(new TH1F("mh1TotalHftTracksInRun","totalHftTracksInRun;runIndex;totalHftTracksInRun",mNRuns+1,0,mNRuns+1));
  mEventList->Add(new TH1F("mh1TotalGRefMultInRun","totalGRefMultInRun;runIndex;totalGRefMultInRun",mNRuns+1,0,mNRuns+1));
  mEventList->Add(new TH1F("mh1TotalHFSecondaryVerticesInRun","totalHFSecondaryVerticesInRun;runIndex;totalHFSecondaryVerticesInRun",mNRuns+1,0,mNRuns+1));
  mEventList->Add(new TH1F("mh1TotalHFTertiaryVerticesInRun","totalHFTertiaryVerticesInRun;runIndex;totalHFTertiaryVerticesInRun",mNRuns+1,0,mNRuns+1));
  mEventList->Add(new TH2F("mh2NHFSecondaryVsNHFTertiary","nHFSecondaryVsnHFTertiary;nHFTertiary;nHFSecondary",1000,0,1000,300,0,300));

  // -- pair histogram list
  outList->Add(new TList);
  mPairList = static_cast<TList*>(outList->Last());
  mPairList->SetOwner(kTRUE);
  mPairList->SetName("baseHFPairHists");
  // -- create pair candidate hists
  mPairList->Add(new TH2F("mh2PairParticle1DcaVsPt","pairParticle1DcaVsPt;p_{T}(pair)(GeV/c));pairParticle1Dca(cm)",120,0,12,200,0,0.02));
  mPairList->Add(new TH2F("mh2PairParticle2DcaVsPt","pairParticle2DcaVsPt;p_{T}(pair)(GeV/c));pairParticle2Dca(cm)",120,0,12,200,0,0.02));
  mPairList->Add(new TH2F("mh2PairCosThetaStarVsPt","pairCosThetaStarVsPt;p_{T}(pair)(GeV/c));cos(#theta)",120,0,12,500,0,1.0));
  mPairList->Add(new TH2F("mh2PairDcaDaughtersVsPt","pairDcaDaughtersVsPt;p_{T}(pair)(GeV/c));dcaDaughters(cm)",120,0,12,200,0,0.02));
  mPairList->Add(new TH2F("mh2PairDecayLenghtVsPt","pairDecayLenghtVsPt;p_{T}(pair)(GeV/c));pairDecayLenght(cm)",120,0,12,500,0,0.1));
   
  // -- triplet histogram list
  outList->Add(new TList);
  mTripletList = static_cast<TList*>(outList->Last());
  mTripletList->SetOwner(kTRUE);
  mTripletList->SetName("baseHFTripletHists");
  // -- create triplet candidate hists
  mTripletList->Add(new TH2F("mh2TripletParticle1DcaVsPt","tripletParticle1DcaVsPt;p_{T}(triplet)(GeV/c));dcaDaughters(cm)",120,0,12,200,0,0.02));
  mTripletList->Add(new TH2F("mh2TripletParticle2DcaVsPt","tripletParticle2DcaVsPt;p_{T}(triplet)(GeV/c));dcaDaughters(cm)",120,0,12,200,0,0.02));
  mTripletList->Add(new TH2F("mh2TripletParticle3DcaVsPt","tripletParticle3DcaVsPt;p_{T}(triplet)(GeV/c));dcaDaughters(cm)",120,0,12,200,0,0.02));
  mTripletList->Add(new TH2F("mh2TripletCosThetaStarVsPt","tripletCosThetaStarVsPt;p_{T}(triplet)(GeV/c));cos(#theta)",120,0,12,500,0,1.0));
  mTripletList->Add(new TH2F("mh2TripletDcaDaughters12VsPt","tripletDcaDaughters12VsPt;p_{T}(triplet)(GeV/c));dcaDaughters(cm)",120,0,12,200,0,0.02));
  mTripletList->Add(new TH2F("mh2TripletDcaDaughters23VsPt","tripletDcaDaughters23VsPt;p_{T}(triplet)(GeV/c));dcaDaughters(cm)",120,0,12,200,0,0.02));
  mTripletList->Add(new TH2F("mh2TripletDcaDaughters31VsPt","tripletDcaDaughters31VsPt;p_{T}(triplet)(GeV/c));dcaDaughters(cm)",120,0,12,200,0,0.02));
  mTripletList->Add(new TH2F("mh2TripletDecayLenghtVsPt","tripletDecayLenghtVsPt;p_{T}(triplet)(GeV/c));tripletDecayLenght(cm)",120,0,12,500,0,0.1));
}


//-----------------------------------------------------------------------
// fill general histograms for all events
//void StHFHists::fillEventHists(StPicoEvent const& picoEvent,StPicoHFEvent const & picoHFEvent,unsigned int const nHftTracks)
void StHFHists::fillEventHists(StPicoEvent const& picoEvent,StPicoHFEvent const & picoHFEvent)
{
  int runIndex = mPrescales->runIndex(picoHFEvent.runId());
  (static_cast<TH1F*>(mEventList->FindObject("mh1TotalEventsInRun")))->Fill(runIndex);
  //(static_cast<TH1F*>(mEventList->FindObject("mh1TotalHftTracksInRun")))->Fill(runIndex,nHftTracks);
  (static_cast<TH1F*>(mEventList->FindObject("mh1TotalGRefMultInRun")))->Fill(runIndex,picoEvent.grefMult());
  (static_cast<TH1F*>(mEventList->FindObject("mh1TotalHFSecondaryVerticesInRun")))->Fill(runIndex,picoHFEvent.nHFSecondaryVertices());
  (static_cast<TH1F*>(mEventList->FindObject("mh1TotalHFTertiaryVerticesInRun")))->Fill(runIndex,picoHFEvent.nHFTertiaryVertices());
  (static_cast<TH2F*>(mEventList->FindObject("mh2NHFSecondaryVsNHFTertiary")))->Fill(picoHFEvent.nHFTertiaryVertices(),picoHFEvent.nHFSecondaryVertices());
}

// fill general histograms for good events
void StHFHists::fillGoodEventHists(StPicoEvent const& picoEvent,StPicoHFEvent const & picoHFEvent)
{ 
  // int runIndex = mPrescales->runIndex(picoHFEvent.runId()); 
}


// fill histograms for pair candidates
void StHFHists::fillPairHists(StHFPair const* const t, bool const fillMass)
{
  (static_cast<TH2F*>(mPairList->FindObject("mh2PairParticle1DcaVsPt")))->Fill(t->pt(),t->particle1Dca());
  (static_cast<TH2F*>(mPairList->FindObject("mh2PairParticle2DcaVsPt")))->Fill(t->pt(),t->particle2Dca());
  (static_cast<TH2F*>(mPairList->FindObject("mh2PairCosThetaStarVsPt")))->Fill(t->pt(),t->cosThetaStar());
  (static_cast<TH2F*>(mPairList->FindObject("mh2PairDcaDaughtersVsPt")))->Fill(t->pt(),t->dcaDaughters());
  (static_cast<TH2F*>(mPairList->FindObject("mh2PairDecayLenghtVsPt")))->Fill(t->pt(),t->decayLength());
//vertex position histos?
}

// fill histograms for triplet candidates
void StHFHists::fillTripletHists(StHFTriplet const* const t, bool const fillMass)
{
  (static_cast<TH2F*>(mTripletList->FindObject("mh2TripletParticle1DcaVsPt")))->Fill(t->pt(),t->particle1Dca());
  (static_cast<TH2F*>(mTripletList->FindObject("mh2TripletParticle2DcaVsPt")))->Fill(t->pt(),t->particle2Dca());
  (static_cast<TH2F*>(mTripletList->FindObject("mh2TripletParticle3DcaVsPt")))->Fill(t->pt(),t->particle3Dca());
  (static_cast<TH2F*>(mTripletList->FindObject("mh2TripletCosThetaStarVsPt")))->Fill(t->pt(),t->cosThetaStar());
  (static_cast<TH2F*>(mTripletList->FindObject("mh2TripletDcaDaughters12VsPt")))->Fill(t->pt(),t->dcaDaughters12());
  (static_cast<TH2F*>(mTripletList->FindObject("mh2TripletDcaDaughters23VsPt")))->Fill(t->pt(),t->dcaDaughters23());
  (static_cast<TH2F*>(mTripletList->FindObject("mh2TripletDcaDaughters31VsPt")))->Fill(t->pt(),t->dcaDaughters31());
  (static_cast<TH2F*>(mTripletList->FindObject("mh2TripletDecayLenghtVsPt")))->Fill(t->pt(),t->decayLength());
  //vertex position histos?
}
