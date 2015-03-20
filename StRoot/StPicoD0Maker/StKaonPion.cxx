#include <limits>

#ifdef __ROOT__
#include "StKaonPion.h"

#include "TMath.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoConstants.h"

ClassImp(StKaonPion)
  


////////////////////////////////////////////////////////////////////
StKaonPion::StKaonPion()
{
  clear();
}
////////////////////////////////////////////////////////////////////
StKaonPion::StKaonPion(StKaonPion* t) {
  clear();
  mKaonIdx = t->mKaonIdx;
  mPionIdx = t->mPionIdx;
  mM    = t->mM;
  mPt = t->mPt;
  mEta = t->mEta;
  mPhi = t->mPhi;
  mOpenAngle = t->mOpenAngle;
  mCosThetaStar = t->mCosThetaStar;
}

////////////////////////////////////////////////////////////////////
StKaonPion::StKaonPion(StPicoTrack* kaon, StPicoTrack* pion,unsigned short kIdx,unsigned short pIdx)
{
  clear();
  if (!kaon || !pion) return;

  StLorentzVectorF kFourMom(kaon->pMom(),kaon->pMom().massHypothesis(.493677));
  StLorentzVectorF pFourMom(pion->pMom(),pion->pMom().massHypothesis(.13957));
  StLorentzVectorF kpFourMom = kFourMom + pFourMom;

 
  StLorentzVectorF kpFourMomReverse(-kpFourMom.px(), -kpFourMom.py(), -kpFourMom.pz(), kpFourMom.e());
  StLorentzVectorF kMomStar = kFourMom.boost(kpFourMomReverse);

  //StThreeVectorF kRotMom(-1.*kaon->pMom().x(),-1.*kaon->pMom().y(),kaon->pMom().z());
  //StLorentzVectorF kRotFourMom(kRotMom,kRotMom.massHypothesis(.493677));
  //StLorentzVectorF kpRotFourMom = kRotFourMom + pFourMom;

  mKaonIdx = kIdx;
  mPionIdx = pIdx;

  mM = (10000.*kpFourMom.m())>Pico::USHORTMAX ? Pico::USHORTMAX : (UShort_t)(TMath::Nint(10000.*kpFourMom.m()));
  mPt = (1000.*kpFourMom.perp())>Pico::USHORTMAX ? Pico::USHORTMAX : (UShort_t)(TMath::Nint(1000.*kpFourMom.perp()));
  mEta = (Short_t)(TMath::Nint(kpFourMom.pseudoRapidity()*1000.));
  mPhi = (Short_t)(TMath::Nint(kpFourMom.phi()*1000.));

  mOpenAngle = (Short_t)(TMath::Nint(kaon->pMom().angle(pion->pMom()) * 1000.));
  mCosThetaStar = (Char_t)(TMath::Nint(TMath::Cos(kMomStar.vect().angle(kpFourMom.vect())) * 100. ));
}
//////////////////////////
StLorentzVectorF StKaonPion::lorentzVector()
{
  Float_t px = pt()*TMath::Cos(phi());
  Float_t py = pt()*TMath::Sin(phi());
  Float_t pz = pt()*TMath::SinH(eta());
  Float_t e = TMath::Sqrt(px*px+py*py+pz*pz+m()*m());

  StLorentzVectorF kpFourMom(px,py,pz,e);

  return kpFourMom;
}
////////////////////////////////////////////////////////////////////
StKaonPion::~StKaonPion() {
}

////////////////////////////////////////////////////////////////////
void StKaonPion::clear() {

  mKaonIdx = std::numeric_limits<unsigned short>::max();
  mPionIdx = std::numeric_limits<unsigned short>::max();

  mM=Pico::USHORTMAX;
  mPt=0;
  mEta=Pico::SHORTMAX;
  mPhi=Pico::SHORTMAX;
  mOpenAngle=Pico::SHORTMAX;
  mCosThetaStar=0.;
};
#endif // __ROOT__
