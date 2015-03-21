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
  

StKaonPion::StKaonPion()
{
  clear();
}
//------------------------------------
StKaonPion::StKaonPion(StKaonPion const * t)
{
  mKaonIdx = t->mKaonIdx;
  mPionIdx = t->mPionIdx;
  mLorentzVector = t->mLorentzVector;
  mCosThetaStar = t->mCosThetaStar;
}
//------------------------------------
StKaonPion::StKaonPion(StPicoTrack const * const kaon, StPicoTrack const * const pion,unsigned short const kIdx,unsigned short const pIdx, StThreeVectorF const & vtx, float const & bField) : mKaonIdx(kIdx), mPionIdx(pIdx)
{
  if (!kaon || !pion) 
  {
    clear();
    return;
  }

  if (kaon->id() == pion->id())
  {
    clear();
    return;
  }

  StPhysicalHelixD kHelix = kaon->dcaGeometry().helix();
  StPhysicalHelixD pHelix = pion->dcaGeometry().helix();

  // move origins of helices to the primary vertex origin
  kHelix.moveOrigin(kHelix.pathLength(vtx));
  pHelix.moveOrigin(pHelix.pathLength(vtx));

  // straight line approximation good enough!
  StThreeVectorF kMom = kHelix.momentum(bField*kilogauss);
  StThreeVectorF pMom = pHelix.momentum(bField*kilogauss);
  StPhysicalHelixD kStraightLine(kMom, kHelix.origin(), 0, kaon->charge());
  StPhysicalHelixD pStraightLine(pMom, pHelix.origin(), 0, pion->charge());

  pair<double,double> ss = kStraightLine.pathLengths(pStraightLine);
  StThreeVectorF kAtDcaToPion = kStraightLine.at(ss.first);
  StThreeVectorF pAtDcaToKaon = pStraightLine.at(ss.second);

  float dcaDaughters = (kAtDcaToPion - pAtDcaToKaon).mag();
  mDcaDaughters = (dcaDaughters*10000.) > std::numeric_limits<unsigned short>::max() ? 
    std::numeric_limits<unsigned short>::max() : static_cast<unsigned short>(std::round(mDcaDaughters*10000.));

  StThreeVectorF kMomAtDca = kHelix.momentumAt(ss.first,bField*kilogauss);
  StThreeVectorF pMomAtDca = pHelix.momentumAt(ss.second,bField*kilogauss);

  StLorentzVectorF kFourMom(kMomAtDca,kMomAtDca.massHypothesis(M_KAON_PLUS));
  StLorentzVectorF pFourMom(pMomAtDca,pMomAtDca.massHypothesis(M_PION_PLUS));

  mLorentzVector = kFourMom + pFourMom;
 
  // calculate cosThetaStar
  StLorentzVectorF kpFourMomReverse(-mLorentzVector.px(), -mLorentzVector.py(), -mLorentzVector.pz(), mLorentzVector.e());
  StLorentzVectorF kFourMomStar = kFourMom.boost(kpFourMomReverse);
  mCosThetaStar = static_cast<char>( std::round( std::cos(kFourMomStar.vect().angle(mLorentzVector.vect())) * 100. ));

  // calculate pointing angle and decay length
  StThreeVectorF v0 = ( kAtDcaToPion + pAtDcaToKaon) * 0.5;
  mPointingAngle = (v0-vtx).angle(mLorentzVector.vect());
  mDecayLength = (v0-vtx).mag();

  // calculate DCA of tracks to primary vertex
  mKaonDca = (kHelix.origin() - vtx).mag();
  mPionDca = (pHelix.origin() - vtx).mag();
}
//------------------------------------
StKaonPion::~StKaonPion()
{
}
//------------------------------------
void StKaonPion::clear()
{
  mPointingAngle = std::numeric_limits<float>::quiet_NaN();
  mDecayLength = std::numeric_limits<float>::quiet_NaN();
  mKaonDca = std::numeric_limits<float>::quiet_NaN();
  mPionDca = std::numeric_limits<float>::quiet_NaN();

  mKaonIdx = std::numeric_limits<unsigned short>::max();
  mPionIdx = std::numeric_limits<unsigned short>::max();

  mDcaDaughters = std::numeric_limits<unsigned short>::max();
  mCosThetaStar = std::numeric_limits<char>::min();
};
#endif // __ROOT__
