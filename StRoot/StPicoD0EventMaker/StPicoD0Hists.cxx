#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TString.h"
#include "../StPicoDstMaker/StPicoEvent.h"
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
  TH1::SetDefaultSumw2();
  mh1TotalEventsInRun = new TH1F("mh1TotalEventsInRun","totalEventsInRun;runIndex",nRuns+1,0,nRuns+1);
  mh1TotalHftTracksInRun = new TH1F("mh1TotalHftTracksInRun","totalHftTracksInRun;runIndex;totalHftTracksInRun",nRuns+1,0,nRuns+1);
  mh1TotalGRefMultInRun = new TH1F("mh1TotalGRefMultInRun","totalGRefMultInRun;runIndex;totalGRefMultInRun",nRuns+1,0,nRuns+1);
  mh1TotalKaonsInRun = new TH1F("mh1TotalKaonsInRun","totalKaonsInRun;runIndex;totalKaonsInRun",nRuns+1,0,nRuns+1);
  mh1TotalPionsInRun = new TH1F("mh1TotalPionsInRun","totalPionsInRun;runIndex;totalPionsInRun",nRuns+1,0,nRuns+1);
  mh1TotalD0CandidatesInRun = new TH1F("mh1TotalD0CandidatesInRun","totalD0CandidatesInRun;runIndex:totalD0CandidatesInRun",nRuns+1,0,nRuns+1);
  mh2NKaonsVsNPions = new TH2F("mh2NKaonsVsNPions","nKaonsVsNPions;nPions;nKaons",1000,0,1000,300,0,300);
  mh2KaonDcaVsPt = new TH2F("mh2KaonDcaVsPt","kaonDcaVsPt;p_{T}(k#pi)(GeV/c);dca(cm)",120,0,12,50,0,0.05);
  mh2PionDcaVsPt = new TH2F("mh2PionDcaVsPt","pionDcaVsPt;p_{T}(k#pi)(GeV/c);dca(cm)",120,0,12,50,0,0.05);
  mh2InvariantMassVsPt = new TH2F("mh2InvariantMassVsPt","invariantMassVsPt;p_{T}(K#pi)(GeV/c);m_{K#pi}(GeV/c^{2})",120,0,12,50,1.6,2.1);
}
StPicoD0Hists::~StPicoD0Hists()
{
  delete mPrescales;
  // note that histograms are owned by mOutFile. They will be destructed 
  // when the file is closed.
}
//-----------------------------------------------------------------------
void StPicoD0Hists::addEvent(StPicoEvent const& picoEvent,StPicoD0Event const & picoD0Even,unsigned int const nHftTracks)
{
  int runIndex = mPrescales->runIndex(picoD0Event.runId());
  mh1TotalEventsInRun->Fill(runIndex);
  mh1TotalEventsInRun->Fill(runIndex,nHftTracks);
  mh1TotalRefMultInRun->Fill(runIndex,picoEvent.grefMult());
  mh1TotalKaonsInRun->Fill(runIndex,picoD0Event.nKaons());
  mh1TotalPionsInRun->Fill(runIndex,picoD0Event.nPions());
  mh1TotalD0CandidatesInRun->Fill(runIndex,picoD0Event.nKaonPion());
  mh2NKaonsVsNPions->Fill(picoD0Event.nPions(),picoD0Event.nKaons());
}
//---------------------------------------------------------------------
void StPicoD0Hists::addKaonPion(StKaonPion const* const kp, bool const fillMass)
{
  mh2KaonDcaVsPt->Fill(kp->pt(),kp->kaonDca());
  mh2PionDcaVsPt->Fill(kp->pt(),kp->pionDca());
  if(fillMass) mh2InvariantMassVsPt->Fill(kp->pt(),kp->m());
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
