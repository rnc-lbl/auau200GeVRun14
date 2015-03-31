#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TString.h"
#include "../StPicoDstMaker/StPicoTrack.h"
#include "../StPicoPrescales/StPicoPrescales.h"
#include "StPicoD0Event.h"
#include "StKaonPion.h"
#include "StCuts.h"

#include "StPicoD0Hists.h"

ClassImp(StPicoD0Hists)

//-----------------------------------------------------------------------
StPicoD0Hists::StPicoD0Hists(TString fileBaseName) : mPrescales(NULL), mOutFile(NULL),
  mh1TotalEventsInRun(NULL), mh1TotalHftTracksInRun(NULL), mh1TotalRefMultInRun(NULL),
  mh1TotalD0CandidatesInRun(NULL), mh2KaonDcaVsPt(NULL), mh2PionDcaVsPt(NULL)
{
  mPrescales = new StPicoPrescales(cuts::prescalesFilesDirectoryName);

  mOutFile = new TFile(Form("%s.picoD0.hists.root",fileBaseName.Data()),"RECREATE");

  int nRuns = mPrescales->numberOfRuns();
  mh1TotalEventsInRun = new TH1F("mh1TotalEventsInRun","totalEventsInRun;runIndex",nRuns+1,0,nRuns+1);
  mh1TotalHftTracksInRun = new TH1F("mh1TotalHftTracksInRun","totalHftTracksInRun;runIndex",nRuns+1,0,nRuns+1);
  mh1TotalRefMultInRun = new TH1F("mh1TotalRefMultInRun","totalRefMultInRun;runIndex",nRuns+1,0,nRuns+1);
  mh1TotalKaonsInRun = new TH1F("mh1TotalKaonsInRun","totalKaonsInRun;runIndex",nRuns+1,0,nRuns+1);
  mh1TotalPionsInRun = new TH1F("mh1TotalPionsInRun","totalPionsInRun;runIndex",nRuns+1,0,nRuns+1);
  mh1TotalD0CandidatesInRun = new TH1F("mh1TotalD0CandidatesInRun","totalD0CandidatesInRun;runIndex",nRuns+1,0,nRuns+1);
  mh2NKaonsVsNPions = new TH2F("mh2NKaonsVsNPions","nKaonsVsNPions;nKaons;nPions",300,0,300,1000,0,1000);
  mh2KaonDcaVsPt = new TH2F("mh2KaonDcaVsPt","kaonDcaVsPt;p_{T}(k#pi)(GeV/c);dca(cm)",120,0,12,50,0,0.05);
  mh2PionDcaVsPt = new TH2F("mh2PionDcaVsPt","pionDcaVsPt;p_{T}(k#pi)(GeV/c);dca(cm)",120,0,12,50,0,0.05);
  mh2InvariantMassVsPt = new TH2F("mh2InvariantMassVsPt","invariantMassVsPt;p_{T}(K#pi)(GeV/c);m_{K#pi}(GeV/c^{2})",120,0,12,50,1.6,2.1);
}
StPicoD0Hists::~StPicoD0Hists()
{
  delete mPrescales;
  // note that histograms are owned by mOutFile. They will be destructed 
  // when the file if closed.
}
//-----------------------------------------------------------------------
void StPicoD0Hists::addEvent(StPicoD0Event const & picoD0Event)
{
  int runIndex = mPrescales->runIndex(picoD0Event.runId());
  mh1TotalEventsInRun->Fill(runIndex);
}
//---------------------------------------------------------------------
void StPicoD0Hists::addKaonPion(StKaonPion const* const kp, StPicoTrack const* const kaon, StPicoTrack const* const pion)
{
}
//---------------------------------------------------------------------
void StPicoD0Hists::closeFile()
{
  mOutFile->cd();
  mh1TotalEventsInRun->Write();
  mh1TotalHftTracksInRun->Write();
  mh1TotalRefMultInRun->Write();
  mh1TotalKaonsInRun->Write();
  mh1TotalPionsInRun->Write();
  mh1TotalD0CandidatesInRun->Write();
  mh2NKaonsVsNPions->Write();
  mh2KaonDcaVsPt->Write();
  mh2PionDcaVsPt->Write();
  mh2InvariantMassVsPt->Write();
  mOutFile->Close();
}
