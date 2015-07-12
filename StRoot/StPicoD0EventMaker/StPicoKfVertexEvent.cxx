#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoKfVertexEvent.h"

StPicoKfVertexEvent::StPicoKfVertexEvent(char const* fileBaseName): mOutputFile(NULL), mTree(NULL)
{
   TString baseName(fileBaseName);
   mOutputFile = new TFile(Form("%s.kfVertex.root", fileBaseName), "RECREATE");
   mOutputFile->SetCompressionLevel(1);
   int BufSize = (int)pow(2., 16.);
   mTree = new TTree("kfEvent", "event information and kfVertex", BufSize);
   mTree->SetAutoSave(1000000); // autosave every 1 Mbytes

   mTree->Branch("mRunId   ", &mRunId   , "mRunId/I");
   mTree->Branch("mEventId ", &mEventId , "mEventId/I");
   mTree->Branch("mRefMult ", &mRefMult , "mRefMult/I");
   mTree->Branch("mGRefMult", &mGRefMult, "mGRefMult/I");
   mTree->Branch("mVx      ", &mVx      , "mVx/F");
   mTree->Branch("mVy      ", &mVy      , "mVy/F");
   mTree->Branch("mVz      ", &mVz      , "mVz/F");
   mTree->Branch("mKfVx    ", &mKfVx    , "mKfVx/F");
   mTree->Branch("mKfVy    ", &mKfVy    , "mKfVy/F");
   mTree->Branch("mKfVz    ", &mKfVz    , "mKfVz/F");
}

void StPicoKfVertexEvent::closeFile()
{
  mOutputFile->cd();
  mOutputFile->Write();
  mOutputFile->Close();
}

void StPicoKfVertexEvent::addEvent(StPicoEvent const& picoEvent,StThreeVectorF const& kfVertex)
{
  mRunId    = picoEvent.runId();
  mEventId  = picoEvent.eventId();
  mRefMult  = picoEvent.refMult();
  mGRefMult = picoEvent.grefMult();
  mVx       = picoEvent.primaryVertex().x();
  mVy       = picoEvent.primaryVertex().y();
  mVz       = picoEvent.primaryVertex().z();
  mKfVx     = kfVertex.x();
  mKfVy     = kfVertex.y();
  mKfVz     = kfVertex.z();

  mTree->Fill();
}
