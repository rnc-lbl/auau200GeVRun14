#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TString.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StKaonPion.h"

#include "StPicoD0Hists.h"

ClassImp(StPicoD0Hists)

//-----------------------------------------------------------------------
StPicoD0Hists::StPicoD0Hists(TString fileBaseName) : mOutFile(NULL), mh2VzVsVzVpd(NULL),
  mh2VrVsVz(NULL), mh1TotalEventsInRun(NULL), mh1TotalHftTracksInRun(NULL), mh1TotalRefMultInRun(NULL),
  mh1TotalD0CandidatesInRun(NULL), mh2KaonDcaVsPt(NULL), mh2PionDcaVsPt(NULL), mh2CosThetaVsPt(NULL)
{
  mOutFile = new TFile(Form("%s.picoD0.hists.root",fileBaseName.Data()),"RECREATE");
}

//-----------------------------------------------------------------------
void StPicoD0Hists::addEvent(StPicoEvent const & picoEvent)
{
}
//---------------------------------------------------------------------
void StPicoD0Hists::addKaonPion(StKaonPion const* const kp, StPicoTrack const* const kaon, StPicoTrack const* const pion)
{
}
//---------------------------------------------------------------------
void StPicoD0Hists::closeFile()
{
  mOutFile->Close();
}
