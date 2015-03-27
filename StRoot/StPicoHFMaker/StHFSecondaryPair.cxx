#include <limits>
#include <cmath>

#ifdef __ROOT__
#include "StHFSecondaryPair.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StDcaGeometry.h" // remove in official production
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "StPicoDstMaker/StPicoTrack.h"

ClassImp(StHFSecondaryPair)


StHFSecondaryPair::StHFSecondaryPair(): mLorentzVector(StLorentzVectorF()),
   mPointingAngle(std::numeric_limits<float>::quiet_NaN()), mDecayLength(std::numeric_limits<float>::quiet_NaN()),
   mParticle1Dca(std::numeric_limits<float>::quiet_NaN()), mParticle2Dca(std::numeric_limits<float>::quiet_NaN()),
   mParticle1Idx(std::numeric_limits<unsigned short>::max()), mParticle2Idx(std::numeric_limits<unsigned short>::max()),
  mDcaDaughters(std::numeric_limits<float>::max()), mCosThetaStar(std::numeric_limits<float>::min()),
  //Added by lomnitz to initialze vertex pos
  mV0x(std::numeric_limits<float>::max()), mV0y(std::numeric_limits<float>::max()), mV0z(std::numeric_limits<float>::max())
{
}
//------------------------------------
StHFSecondaryPair::StHFSecondaryPair(StHFSecondaryPair const * t) : mLorentzVector(t->mLorentzVector),
   mPointingAngle(t->mPointingAngle), mDecayLength(t->mDecayLength),
   mParticle1Dca(t->mParticle1Dca), mParticle2Dca(t->mParticle2Dca),
   mParticle1Idx(t->mParticle1Idx), mParticle2Idx(t->mParticle2Idx),
   mDcaDaughters(t->mDcaDaughters), mCosThetaStar(t->mCosThetaStar),
   //Added by Lomnitz 
   mV0x(t->mV0x), mV0y(t->mV0y), mV0z(t->mV0z)								    
{
}

//------------------------------------
StHFSecondaryPair::StHFSecondaryPair(StPicoTrack const * const particle1, StPicoTrack const * const particle2,
		   float p1MassHypo, float p2MassHypo, 
		   unsigned short const p1Idx, unsigned short const p2Idx,
		   StThreeVectorF const & vtx, float const bField)  : 
  mLorentzVector(StLorentzVectorF()),
  mPointingAngle(std::numeric_limits<float>::quiet_NaN()), mDecayLength(std::numeric_limits<float>::quiet_NaN()),
  mParticle1Dca(std::numeric_limits<float>::quiet_NaN()), mParticle2Dca(std::numeric_limits<float>::quiet_NaN()),
  mParticle1Idx(p1Idx), mParticle2Idx(p2Idx),
  mDcaDaughters(std::numeric_limits<float>::max()), mCosThetaStar(std::numeric_limits<float>::min()),
  //Lomnitz
  mV0x(std::numeric_limits<float>::max()), mV0y(std::numeric_limits<float>::max()), mV0z(std::numeric_limits<float>::max())
{
   if ((!particle1 || !particle2) || (particle1->id() == particle2->id()))
   {
      mParticle1Idx = std::numeric_limits<unsigned short>::max();
      mParticle2Idx = std::numeric_limits<unsigned short>::max();
      return;
   }

   /// prefixes code:
   ///   p1 means particle 1
   ///   p2 means particle 2
   ///   pair means partic1 , particle2. pair

   StPhysicalHelixD p1Helix = particle1->dcaGeometry().helix();
   StPhysicalHelixD p2Helix = particle2->dcaGeometry().helix();

   // move origins of helices to the primary vertex origin
   p1Helix.moveOrigin(p1Helix.pathLength(vtx));
   p2Helix.moveOrigin(p2Helix.pathLength(vtx));

   // use straight lines approximation to get point of DCA of particle1-particle2 pair
   StThreeVectorF const p1Mom = p1Helix.momentum(bField * kilogauss);
   StThreeVectorF const p2Mom = p2Helix.momentum(bField * kilogauss);
   StPhysicalHelixD const p1StraightLine(p1Mom, p1Helix.origin(), 0, particle1->charge());
   StPhysicalHelixD const p2StraightLine(p2Mom, p2Helix.origin(), 0, particle2->charge());

   pair<double, double> const ss = p1StraightLine.pathLengths(p2StraightLine);
   StThreeVectorF const p1AtDcaToP2 = p1StraightLine.at(ss.first);
   StThreeVectorF const p2AtDcaToP1 = p2StraightLine.at(ss.second);

   // calculate DCA of particle1 to particl2 at their DCA
   mDcaDaughters = (p1AtDcaToP2 - p2AtDcaToP1).mag();
   
   // calculate Lorentz vector of particle1-particle2 pair
   StThreeVectorF const p1MomAtDca = p1Helix.momentumAt(ss.first,  bField * kilogauss);
   StThreeVectorF const p2MomAtDca = p2Helix.momentumAt(ss.second, bField * kilogauss);

   StLorentzVectorF const p1FourMom(p1MomAtDca, p1MomAtDca.massHypothesis(p1MassHypo));
   StLorentzVectorF const p2FourMom(p2MomAtDca, p2MomAtDca.massHypothesis(p2MassHypo));

   mLorentzVector = p1FourMom + p2FourMom;

   // calculate cosThetaStar
   StLorentzVectorF const pairFourMomReverse(-mLorentzVector.px(), -mLorentzVector.py(), -mLorentzVector.pz(), mLorentzVector.e());
   StLorentzVectorF const p1FourMomStar = p1FourMom.boost(pairFourMomReverse);
   mCosThetaStar = std::cos(p1FourMomStar.vect().angle(mLorentzVector.vect()));

   //Lomnitz
   StThreeVectorF const tertiaryVtx = (p1AtDcaToP2 + p2AtDcaToP1) * 0.5 ;
   mV0x = tertiaryVtx.x();
   mV0y = tertiaryVtx.y();
   mV0z = tertiaryVtx.z(); 

   // calculate pointing angle and decay length -- with respect to primary vertex - only rough estimate)
   StThreeVectorF const vtxToV0 = tertiaryVtx - vtx;
   mPointingAngle = vtxToV0.angle(mLorentzVector.vect());
   mDecayLength = vtxToV0.mag();

   // calculate DCA of tracks to primary vertexv( only rough estimate 
   mParticle1Dca = (p1Helix.origin() - vtx).mag();
   mParticle2Dca = (p2Helix.origin() - vtx).mag();
}

#endif // __ROOT__
