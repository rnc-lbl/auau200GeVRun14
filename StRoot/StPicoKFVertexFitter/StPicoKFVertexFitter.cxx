#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TNtuple.h"

#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StiMaker/StKFVerticesCollection.h"
#include "StPicoKFVertexFitter.h"
#include "MTrack.h"

ClassImp(StPicoKFVertexFitter)

StPicoKFVertexFitter::StPicoKFVertexFitter(StThreeVectorF *kfVertex,StPicoDstMaker* picoDstMaker): 
mKFVertex(kfVertex),d0Daughters(NULL),mPicoDstMaker(picoDstMaker)
{
//  d0Daughters.clear();
  dcaG = new StDcaGeometry();
}

StPicoKFVertexFitter::StPicoKFVertexFitter(StThreeVectorF *kfVertex,vector<int>& daughter,StPicoDstMaker* picoDstMaker): 
mKFVertex(kfVertex),d0Daughters(daughter),mPicoDstMaker(picoDstMaker)
{
//  d0Daughters = daughter;
  dcaG = new StDcaGeometry();
}

//-----------------------------------------------------------------------------
StPicoKFVertexFitter::~StPicoKFVertexFitter()
{
   /*  */
  delete dcaG;
}
//-----------------------------------------------------------------------------
int StPicoKFVertexFitter::primaryVertexRefit() 
{

  picoDst = mPicoDstMaker->picoDst();
  int nTracks = picoDst->numberOfTracks();
  int N = 0;//nTracks - d0Daughters.size();
  for (int i=0; i < nTracks; i++) {
    StPicoTrack *gTrack = (StPicoTrack*)picoDst->track(i);
    if (! gTrack) continue;
    Int_t kg = gTrack->id();

    bool flagDd0DaughtersCand = false;
    for (vector<int>::size_type j=0; j < d0Daughters.size(); j++) {
      if (d0Daughters[j] == kg) 
      {
        flagDd0DaughtersCand = 1;
      }
    }
    if (flagDd0DaughtersCand == 1) continue;
    N++;
  }
  KFParticle *particles[N];
  //  KFParticle **particles = new KFParticle*[N];
  //  StKFVertexMaker fitter;
  Int_t NGoodGlobals = 0;
  for (int i=0; i < nTracks; i++) {
    StPicoTrack *gTrack = (StPicoTrack*)picoDst->track(i);
    if (! gTrack) continue;
    dcaG->set(gTrack->params(),gTrack->errMatrix());
    if (! dcaG) continue;
    Int_t kg = gTrack->id();

    bool flagDd0DaughtersCand = false;
    for (vector<int>::size_type j=0; j < d0Daughters.size(); j++) {
      if (d0Daughters[j] == kg) 
      {
        flagDd0DaughtersCand = 1;
      }
    }
    if (flagDd0DaughtersCand == 1) continue;


    // particles[NGoodGlobals] = fitter.AddTrackAt(dcaG,kg);
    if (! dcaG) return 0;
    Double_t xyzp[6], CovXyzp[21];
    dcaG->GetXYZ(xyzp,CovXyzp);
    static MTrack track;
    track.SetParameters(xyzp);
    track.SetCovarianceMatrix(CovXyzp);
    track.SetNDF(1);
    //    track.SetChi2(GlobalTracks_mChiSqXY[k]);
    track.SetID(kg);
    Int_t q   = 1;
    Int_t pdg = 211;
    if (dcaG->charge() < 0) {
      q = -1;
      pdg = -211;
    } 
    track.SetCharge(q);
    particles[NGoodGlobals] = new KFParticle(track, pdg);
    particles[NGoodGlobals]->SetID(kg);
    //////////////////
    NGoodGlobals++;
  }
  TArrayC Flag(N);
  KFVertex aVertex;
  aVertex.ConstructPrimaryVertex((const KFParticle **) particles, N,
      (Bool_t*) Flag.GetArray(),TMath::Sqrt(StAnneling::Chi2Cut()/2));
  //  delete [] particles;
  for(int i=0;i<N;++i)
  {
    //    particles[i]->Clear();
    delete particles[i];
  }

  if(aVertex.GetX()==0) return 0;
  mKFVertex->set(aVertex.GetX(),aVertex.GetY(),aVertex.GetZ());
  return 1;
}
//-----------------------------------------------------------------------------

