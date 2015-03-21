#include <limits>
#include <cmath>

#ifdef __ROOT__
#include "StKaonPion.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoConstants.h"

ClassImp(StKaonPion)
  

StKaonPion::StKaonPion(): mLorentzVector(StLorentzVectorF()), 
                       mPointingAngle(std::numeric_limits<float>::quiet_NaN()), mDecayLength(std::numeric_limits<float>::quiet_NaN()), 
                       mKaonDca(std::numeric_limits<float>::quiet_NaN()), mPionDca(std::numeric_limits<float>::quiet_NaN()), 
                       mKaonIdx(std::numeric_limits<unsigned short>::max()), mPionIdx(std::numeric_limits<unsigned short>::max()), 
                       mDcaDaughters(std::numeric_limits<unsigned short>::max()), mCosThetaStar(std::numeric_limits<char>::min())

{
}
//------------------------------------
StKaonPion::StKaonPion(StKaonPion const * t) : mLorentzVector(t->mLorentzVector), 
                       mPointingAngle(t->mPointingAngle), mDecayLength(t->mDecayLength), 
                       mKaonDca(t->mKaonDca), mPionDca(t->mPionDca),
                       mKaonIdx(t->mKaonIdx), mPionIdx(t->mPionIdx), 
                       mDcaDaughters(t->mDcaDaughters), mCosThetaStar(t->mCosThetaStar)
{
}
//------------------------------------
StKaonPion::StKaonPion(StPicoTrack const * const kaon, StPicoTrack const * const pion,
                       unsigned short const kIdx,unsigned short const pIdx, 
                       StThreeVectorF const & vtx, float const & bField) : mLorentzVector(StLorentzVectorF()), 
                       mPointingAngle(std::numeric_limits<float>::quiet_NaN()), mDecayLength(std::numeric_limits<float>::quiet_NaN()), 
                       mKaonDca(std::numeric_limits<float>::quiet_NaN()), mPionDca(std::numeric_limits<float>::quiet_NaN()), 
                       mKaonIdx(kIdx), mPionIdx(pIdx), 
                       mDcaDaughters(std::numeric_limits<unsigned short>::max()), mCosThetaStar(std::numeric_limits<char>::min())
{
  if ((!kaon || !pion) || (kaon->id() == pion->id()))
  {
    mKaonIdx = std::numeric_limits<unsigned short>::max();
    mPionIdx = std::numeric_limits<unsigned short>::max();
    return;
  }

  /// prefixes code:
  ///   k means kaon
  ///   p means pion
  ///   kp means kaon-pion pair
  
  StPhysicalHelixD kHelix = kaon->dcaGeometry().helix();
  StPhysicalHelixD pHelix = pion->dcaGeometry().helix();

  // move origins of helices to the primary vertex origin
  kHelix.moveOrigin(kHelix.pathLength(vtx));
  pHelix.moveOrigin(pHelix.pathLength(vtx));

  // straight line approximation good enough!
  StThreeVectorF const kMom = kHelix.momentum(bField*kilogauss);
  StThreeVectorF const pMom = pHelix.momentum(bField*kilogauss);
  StPhysicalHelixD const kStraightLine(kMom, kHelix.origin(), 0, kaon->charge());
  StPhysicalHelixD const pStraightLine(pMom, pHelix.origin(), 0, pion->charge());

  pair<double,double> const ss = kStraightLine.pathLengths(pStraightLine);
  StThreeVectorF const kAtDcaToPion = kStraightLine.at(ss.first);
  StThreeVectorF const pAtDcaToKaon = pStraightLine.at(ss.second);

  // calculate DCA of kaon and pion at DCA
  float const dcaDaughters = (kAtDcaToPion - pAtDcaToKaon).mag();
  mDcaDaughters = (dcaDaughters*10000.) > std::numeric_limits<unsigned short>::max() ? 
    std::numeric_limits<unsigned short>::max() : static_cast<unsigned short>(std::round(mDcaDaughters*10000.));

  // calculate Lorentz vector of kaon-pion pair
  StThreeVectorF const kMomAtDca = kHelix.momentumAt(ss.first,bField*kilogauss);
  StThreeVectorF const pMomAtDca = pHelix.momentumAt(ss.second,bField*kilogauss);

  StLorentzVectorF const kFourMom(kMomAtDca,kMomAtDca.massHypothesis(M_KAON_PLUS));
  StLorentzVectorF const pFourMom(pMomAtDca,pMomAtDca.massHypothesis(M_PION_PLUS));

  mLorentzVector = kFourMom + pFourMom;
 
  // calculate cosThetaStar
  StLorentzVectorF const kpFourMomReverse(-mLorentzVector.px(), -mLorentzVector.py(), -mLorentzVector.pz(), mLorentzVector.e());
  StLorentzVectorF const kFourMomStar = kFourMom.boost(kpFourMomReverse);
  mCosThetaStar = static_cast<char>( std::round( std::cos(kFourMomStar.vect().angle(mLorentzVector.vect())) * 100. ));

  // calculate pointing angle and decay length
  StThreeVectorF const v0ToVtx = ( kAtDcaToPion + pAtDcaToKaon) * 0.5 - vtx;
  mPointingAngle = v0ToVtx.angle(mLorentzVector.vect());
  mDecayLength = v0ToVtx.mag();

  // calculate DCA of tracks to primary vertex
  mKaonDca = (kHelix.origin() - vtx).mag();
  mPionDca = (pHelix.origin() - vtx).mag();
}
#endif // __ROOT__
