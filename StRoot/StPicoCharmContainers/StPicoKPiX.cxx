#include <limits>
#include <algorithm>
#include <iostream>

#ifdef __ROOT__
#include "StPicoKPiX.h"

#include "StarClassLibrary/StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "StPicoDstMaker/StPicoTrack.h"

ClassImp(StPicoKPiX)

StPicoKPiX::StPicoKPiX(): mKaonMomAtDca{}, mPionMomAtDca{}, mXMomAtDca{},
                          mKaonPionDca{std::numeric_limits<float>::max()},
                          mKaonXDca{std::numeric_limits<float>::max()},
                          mPionXDca{std::numeric_limits<float>::max()},
                          mPointingAngle(std::numeric_limits<float>::max()), 
                          mDecayLength(std::numeric_limits<float>::min()),
                          mKaonDca(std::numeric_limits<float>::min()), 
                          mPionDca(std::numeric_limits<float>::min()),
                          mXDca(std::numeric_limits<float>::min()),
                          mKaonIdx(std::numeric_limits<unsigned short>::max()), 
                          mPionIdx(std::numeric_limits<unsigned short>::max()),
                          mXIdx(std::numeric_limits<unsigned short>::max())
{
}

//------------------------------------
StPicoKPiX::StPicoKPiX(StPicoTrack const* const kaon, StPicoTrack const* const pion, StPicoTrack const* const x,
                       unsigned short const kIdx, unsigned short const pIdx, unsigned short xIdx,
                       StThreeVectorF const& vtx, float const bField) : StPicoKPiX()
{
   if (!kaon || !pion || !x ||
       kaon->id() == pion->id() || 
       kaon->id() == x->id() ||
       pion->id() == x->id())
   {
      return;
   }

   mKaonIdx = kIdx;
   mPionIdx = pIdx;
   mXIdx    = xIdx;

   /// local variables prefixes:
   ///   k is for kaon
   ///   p is for pion
   ///   x is for the third particle

   StPhysicalHelixD kHelix = kaon->dcaGeometry().helix();
   StPhysicalHelixD pHelix = pion->dcaGeometry().helix();
   StPhysicalHelixD xHelix = x->dcaGeometry().helix();

   // move origins of helices to the primary vertex origin
   kHelix.moveOrigin(kHelix.pathLength(vtx));
   pHelix.moveOrigin(pHelix.pathLength(vtx));
   xHelix.moveOrigin(xHelix.pathLength(vtx));

   // use straight lines approximation to get point of DCA
   StPhysicalHelixD const kStraightLine(kHelix.momentum(bField * kilogauss), kHelix.origin(), 0, kaon->charge());
   StPhysicalHelixD const pStraightLine(pHelix.momentum(bField * kilogauss), pHelix.origin(), 0, pion->charge());
   StPhysicalHelixD const xStraightLine(xHelix.momentum(bField * kilogauss), xHelix.origin(), 0, x->charge());

   pair<double, double> const sskp = kStraightLine.pathLengths(pStraightLine);
   pair<double, double> const sskx = kStraightLine.pathLengths(xStraightLine);
   pair<double, double> const sspx = pStraightLine.pathLengths(xStraightLine);

   StThreeVectorF const kAtDcaToP = kStraightLine.at(sskp.first);
   StThreeVectorF const pAtDcaToK = pStraightLine.at(sskp.second);
   StThreeVectorF const kAtDcaToX = kStraightLine.at(sskx.first);
   StThreeVectorF const xAtDcaToK = xStraightLine.at(sskx.second);
   StThreeVectorF const pAtDcaToX = pStraightLine.at(sspx.first);
   StThreeVectorF const xAtDcaToP = xStraightLine.at(sspx.second);

   StThreeVectorF const v0 = ( kAtDcaToP + pAtDcaToK + kAtDcaToX + xAtDcaToK + pAtDcaToX + xAtDcaToK ) / 6.;
   mKaonMomAtDca  = kHelix.momentumAt(kHelix.pathLength(v0), bField * kilogauss);
   mPionMomAtDca  = pHelix.momentumAt(pHelix.pathLength(v0), bField * kilogauss);
   mXMomAtDca     = xHelix.momentumAt(xHelix.pathLength(v0), bField * kilogauss);

   mKaonPionDca = (kAtDcaToP - pAtDcaToK).mag();
   mKaonXDca    = (kAtDcaToX - xAtDcaToK).mag();
   mPionXDca    = (pAtDcaToX - xAtDcaToP).mag();

   // calculate pointing angle and decay length
   StThreeVectorF const vtxToV0 = v0 - vtx;
   mPointingAngle = vtxToV0.angle(mKaonMomAtDca + mPionMomAtDca + mXMomAtDca);
   mDecayLength = vtxToV0.mag();

   // calculate DCA of tracks to primary vertex
   mKaonDca = (kHelix.origin() - vtx).mag();
   mPionDca = (pHelix.origin() - vtx).mag();
   mXDca    = (xHelix.origin() - vtx).mag();
}

StLorentzVectorF StPicoKPiX::fourMom(double const xMassHypothesis) const
{
  return {mKaonMomAtDca+mPionMomAtDca+mXMomAtDca,
          mKaonMomAtDca.massHypothesis(M_KAON_MINUS)+
          mPionMomAtDca.massHypothesis(M_PION_PLUS)+
          mXMomAtDca.massHypothesis(xMassHypothesis)};
}

StLorentzVectorF StPicoKPiX::kaonPionFourMom() const
{
  return {mKaonMomAtDca+mPionMomAtDca, 
          mKaonMomAtDca.massHypothesis(M_KAON_MINUS)+
          mPionMomAtDca.massHypothesis(M_PION_PLUS)};
}

StLorentzVectorF StPicoKPiX::kaonXFourMom(double const xMassHypothesis) const
{
  return {mKaonMomAtDca+mXMomAtDca, 
          mKaonMomAtDca.massHypothesis(M_KAON_MINUS)+
          mXMomAtDca.massHypothesis(xMassHypothesis)};
}

StLorentzVectorF StPicoKPiX::pionXFourMom(double const xMassHypothesis) const
{
  return {mPionMomAtDca+mXMomAtDca, 
          mPionMomAtDca.massHypothesis(M_KAON_MINUS)+
          mXMomAtDca.massHypothesis(xMassHypothesis)};
}

float StPicoKPiX::dcaDaughters() const
{
  return std::max({mKaonPionDca,mKaonXDca,mPionXDca});
}

#endif // __ROOT__
