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
  mEventList(NULL), mPrescales(NULL), mNRuns(0) {
}


StHFHists::StHFHists(const char* name) : TNamed(name, name),
  mEventList(NULL), mPrescales(NULL), mNRuns(0){
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
  //std::string const prescalesFilesDirectoryName = "./run14AuAu200GeVPrescales";
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
}


//-----------------------------------------------------------------------
// addEvent
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

