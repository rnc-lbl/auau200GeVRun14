#include "StarClassLibrary/StThreeVectorF.hh"
#include "StarRoot/MTrack.h"
#include "StiMaker/StKFVerticesCollection.h"
#include "StEvent/StDcaGeometry.h"

#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoKFVertexFitter.h"

using namespace std;

StThreeVectorF StPicoKFVertexFitter::primaryVertexRefit(StPicoDst const* const picoDst,
    std::vector<int> const& tracksToRemove) const
{
   vector<int> goodTracks;

   // make a list of good tracks to be used in the KFVertex fit
   for (unsigned int iTrk = 0; iTrk < picoDst->numberOfTracks(); ++iTrk)
   {
      StPicoTrack* gTrack = (StPicoTrack*)picoDst->track(iTrk);
      if (! gTrack) continue;

      bool rejectTrak = false;
      for (size_t  j = 0; j < tracksToRemove.size(); ++j)
      {
        if (tracksToRemove[j] == gTrack->id())
        {
          rejectTrak = true;
          break;
        }
      }

      if(rejectTrak) continue;
      goodTracks.push_back(iTrk);
   }

   // fill an array of KFParticles
   KFParticle* particles[goodTracks.size()];

   for (size_t iTrk = 0; iTrk < goodTracks.size(); ++iTrk)
   {
      StPicoTrack* gTrack = (StPicoTrack*)picoDst->track(goodTracks[iTrk]);

      StDcaGeometry dcaG = gTrack->dcaGeometry();
      Double_t xyzp[6], CovXyzp[21];
      dcaG.GetXYZ(xyzp, CovXyzp);
      MTrack track;
      track.SetParameters(xyzp);
      track.SetCovarianceMatrix(CovXyzp);
      track.SetNDF(1);
      track.SetID(gTrack->id());
      track.SetCharge(dcaG.charge());

      Int_t pdg = dcaG.charge() > 1 ? 211 : -211; // assume all tracks are pions.

      particles[iTrk] = new KFParticle(track, pdg);
   }

   TArrayC Flag(goodTracks.size());
   KFVertex aVertex;
   aVertex.ConstructPrimaryVertex((const KFParticle **) particles, goodTracks.size(),
                                  (Bool_t*) Flag.GetArray(), TMath::Sqrt(StAnneling::Chi2Cut() / 2));

   // clean up
   for(size_t iTrk = 0; iTrk < goodTracks.size(); ++iTrk) delete particles[iTrk];

   StThreeVectorF kfVertex(-999.,-999.,-999.);

   if (aVertex.GetX())
   {
     kfVertex.set(aVertex.GetX(), aVertex.GetY(), aVertex.GetZ());
   }

   return kfVertex;
}
