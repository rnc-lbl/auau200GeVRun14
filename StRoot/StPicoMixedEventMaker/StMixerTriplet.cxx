#include <limits>
#include <cmath>

#include "StMixerTriplet.h"
#include "StMixerTrack.h"
#include "StPhysicalHelixD.hh"
#include "SystemOfUnits.h"

ClassImp(StMixerTriplet)

// _________________________________________________________
StMixerTriplet::StMixerTriplet(): mLorentzVector(StLorentzVectorF()), mDecayVertex(StThreeVectorF()),
  mPointingAngle(std::numeric_limits<float>::quiet_NaN()), mDecayLength(std::numeric_limits<float>::quiet_NaN()),
  mParticle1Dca(std::numeric_limits<float>::quiet_NaN()), mParticle2Dca(std::numeric_limits<float>::quiet_NaN()), mParticle3Dca(std::numeric_limits<float>::quiet_NaN()),
  mParticle1Mom(StThreeVectorF()), mParticle2Mom(StThreeVectorF()), mParticle3Mom(StThreeVectorF()),
  mDcaDaughters12(std::numeric_limits<float>::max()), mDcaDaughters23(std::numeric_limits<float>::max()), 
  mDcaDaughters31(std::numeric_limits<float>::max()), mCosThetaStar(std::numeric_limits<float>::quiet_NaN()) {
}

// _________________________________________________________
StMixerTriplet::StMixerTriplet(StMixerTriplet const * t) : mLorentzVector(t->mLorentzVector), mDecayVertex(t->mDecayVertex),
   mPointingAngle(t->mPointingAngle), mDecayLength(t->mDecayLength),
   mParticle1Dca(t->mParticle1Dca), mParticle2Dca(t->mParticle2Dca), mParticle3Dca(t->mParticle3Dca),
   mParticle1Mom(t->mParticle1Mom), mParticle2Mom(t->mParticle2Mom), mParticle3Mom(t->mParticle3Mom),
   mDcaDaughters12(t->mDcaDaughters12), mDcaDaughters23(t->mDcaDaughters23),
   mDcaDaughters31(t->mDcaDaughters31), mCosThetaStar(t->mCosThetaStar) {
}

// _________________________________________________________
StMixerTriplet::StMixerTriplet(StMixerTrack const * const particle1, StMixerTrack const * const particle2, StMixerTrack const * const particle3,
			       float p1MassHypo, float p2MassHypo, float p3MassHypo,
			       StThreeVectorF const & vtx, float const bField) :   mLorentzVector(StLorentzVectorF()), mDecayVertex(StThreeVectorF()),
  mPointingAngle(std::numeric_limits<float>::quiet_NaN()), mDecayLength(std::numeric_limits<float>::quiet_NaN()),
  mParticle1Dca(std::numeric_limits<float>::quiet_NaN()), mParticle2Dca(std::numeric_limits<float>::quiet_NaN()),
  mParticle1Mom(StThreeVectorF()), mParticle2Mom(StThreeVectorF()), mParticle3Mom(StThreeVectorF()),
  mDcaDaughters12(std::numeric_limits<float>::max()), mDcaDaughters23(std::numeric_limits<float>::max()),
 mDcaDaughters31(std::numeric_limits<float>::max()), mCosThetaStar(std::numeric_limits<float>::quiet_NaN()) {
  // -- Create pair out of 2 tracks
  //     prefixes code:
  //      p1 means particle 1
  //      p2 means particle 2
  //      pair means particle1-particle2  pair
  
  mParticle1Mom = particle1->gMom();
  mParticle2Mom = particle2->gMom();
  mParticle3Mom = particle3->gMom();

  StThreeVectorF const origin(0.0 ,0.0 , 0.0);
  
  StPhysicalHelixD p1Helix(particle1->gMom(), origin, particle1->charge(), bField); 
  StPhysicalHelixD p2Helix(particle2->gMom(), origin, particle2->charge(), bField); 
  StPhysicalHelixD p3Helix(particle3->gMom(), origin, particle3->charge(), bField); 
   // -- move origins of helices to the primary vertex origin
  p1Helix.moveOrigin(p1Helix.pathLength(vtx));
  p2Helix.moveOrigin(p2Helix.pathLength(vtx));
  p3Helix.moveOrigin(p3Helix.pathLength(vtx));
  
  // -- use straight lines approximation to get point of DCA of particle1-particle2 pair
  StThreeVectorF const p1Mom = p1Helix.momentum(bField * kilogauss);
  StThreeVectorF const p2Mom = p2Helix.momentum(bField * kilogauss);
  StThreeVectorF const p3Mom = p3Helix.momentum(bField * kilogauss);
  
  StPhysicalHelixD const p1StraightLine(p1Mom, p1Helix.origin(), 0, particle1->charge());
  StPhysicalHelixD const p2StraightLine(p2Mom, p2Helix.origin(), 0, particle2->charge());
  StPhysicalHelixD const p3StraightLine(p3Mom, p3Helix.origin(), 0, particle3->charge());
  
  pair<double, double> const ss12 = p1StraightLine.pathLengths(p2StraightLine);
  StThreeVectorF const p1AtDcaToP2 = p1StraightLine.at(ss12.first);
  StThreeVectorF const p2AtDcaToP1 = p2StraightLine.at(ss12.second);

  pair<double, double> const ss23 = p2StraightLine.pathLengths(p3StraightLine);
  StThreeVectorF const p2AtDcaToP3 = p2StraightLine.at(ss23.first);
  StThreeVectorF const p3AtDcaToP2 = p3StraightLine.at(ss23.second);
  
  pair<double, double> const ss31 = p3StraightLine.pathLengths(p1StraightLine);
  StThreeVectorF const p3AtDcaToP1 = p3StraightLine.at(ss31.first);
  StThreeVectorF const p1AtDcaToP3 = p1StraightLine.at(ss31.second);
  
  // -- calculate DCA of particle1 to particl2 at their DCA
  mDcaDaughters12 = (p1AtDcaToP2 - p2AtDcaToP1).mag();
  
  // -- calculate DCA of particle2 to particl3 at their DCA
  mDcaDaughters23 = (p2AtDcaToP3 - p3AtDcaToP2).mag();
  
  // -- calculate DCA of particle3 to particle1 at their DCA
  mDcaDaughters31 = (p3AtDcaToP1 - p1AtDcaToP3).mag();
  
  // -- calculate decay vertex (secondary)
  StThreeVectorF mDecayVertex = ( p1AtDcaToP2 + p2AtDcaToP1 + p2AtDcaToP3 + p3AtDcaToP2 + p3AtDcaToP1 + p1AtDcaToP3 ) / 6.0;
  
  // -- constructing mother daughter four momentum. Need helix (not straight line) for each daughter
  double const p1AtV0 = p1Helix.pathLength( mDecayVertex );
  StThreeVectorF const p1MomAtDca = p1Helix.momentumAt(p1AtV0 ,  bField * kilogauss);

  double const p2AtV0 = p2Helix.pathLength( mDecayVertex );
  StThreeVectorF const p2MomAtDca = p2Helix.momentumAt(p2AtV0 ,  bField * kilogauss);
  
  double const p3AtV0 = p3Helix.pathLength( mDecayVertex );
  StThreeVectorF const p3MomAtDca = p3Helix.momentumAt(p3AtV0 ,  bField * kilogauss);
  
  StLorentzVectorF const p1FourMom(p1MomAtDca, p1MomAtDca.massHypothesis(p1MassHypo));
  StLorentzVectorF const p2FourMom(p2MomAtDca, p2MomAtDca.massHypothesis(p2MassHypo));
  StLorentzVectorF const p3FourMom(p3MomAtDca, p3MomAtDca.massHypothesis(p3MassHypo));
  
  mLorentzVector = p1FourMom + p2FourMom + p3FourMom;
   
  // -- calculate cosThetaStar
  //    ->> Lomnitz: Need to rethink theta star
  StLorentzVectorF const tripletFourMomReverse(-mLorentzVector.px(), -mLorentzVector.py(), -mLorentzVector.pz(), mLorentzVector.e());
  StLorentzVectorF const p1FourMomStar = p1FourMom.boost(tripletFourMomReverse);
  mCosThetaStar = std::cos(p1FourMomStar.vect().angle(mLorentzVector.vect()));
  
  // -- calculate pointing angle and decay length
  StThreeVectorF const vtxToV0 = mDecayVertex - vtx;
  mPointingAngle = vtxToV0.angle(mLorentzVector.vect());
  mDecayLength = vtxToV0.mag();
  
  // --- calculate DCA of tracks to primary vertex
  mParticle1Dca = (p1Helix.origin() - vtx).mag();
  mParticle2Dca = (p2Helix.origin() - vtx).mag();
  mParticle3Dca = (p3Helix.origin() - vtx).mag();
}


