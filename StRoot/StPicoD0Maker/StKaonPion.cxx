#include <limits>

#ifdef __ROOT__
#include "StKaonPion.h"

#include "TMath.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoConstants.h"

const float KAONMASS = 0.493677;
const float PIONMASS = 0.13957;

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
StKaonPion::StKaonPion(StPicoTrack const * const kaon, StPicoTrack const * const pion,unsigned short const kIdx,unsigned short const pIdx, StThreeVectorF const & pVtx, float const & bField) : mKaonIdx(kIdx), mPionIdx(pIdx)
{
  clear();
  if (!kaon || !pion) return;

  StLorentzVectorF kFourMom(kaon->pMom(),kaon->pMom().massHypothesis(KAONMASS));
  StLorentzVectorF pFourMom(pion->pMom(),pion->pMom().massHypothesis(PIONMASS));
  mLorentzVector = kFourMom + pFourMom;

 
  StLorentzVectorF kpFourMomReverse(-mLorentzVector.px(), -mLorentzVector.py(), -mLorentzVector.pz(), mLorentzVector.e());
  StLorentzVectorF kMomStar = kFourMom.boost(kpFourMomReverse);

  mCosThetaStar = static_cast<char>((TMath::Nint(TMath::Cos(kMomStar.vect().angle(mLorentzVector.vect())) * 100. )));
}
//------------------------------------
StKaonPion::~StKaonPion()
{
}
//------------------------------------
void StKaonPion::clear()
{
  mKaonIdx = std::numeric_limits<unsigned short>::max();
  mPionIdx = std::numeric_limits<unsigned short>::max();

  mCosThetaStar = std::numeric_limits<char>::min();
};
#endif // __ROOT__
