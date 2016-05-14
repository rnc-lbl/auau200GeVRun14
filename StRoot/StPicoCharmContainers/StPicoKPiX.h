#ifndef StPicoKPiX_hh
#define StPicoKPiX_hh
#ifdef __ROOT__

/* **************************************************
 *  A specialized container for calculating K-π-hadron
 *  lorentz vector and topological decay parameters 
 *  and storing them.
 *
 *  Authors:  Xin Dong (xdong@lbl.gov),
 *          **Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  ** Code Maintainer
 *
 * **************************************************
 */
#include <cmath>

#include "TObject.h"
#include "StarClassLibrary/StLorentzVectorF.hh"
#include "StarClassLibrary/StThreeVectorF.hh"

class StPicoTrack;

class StPicoKPiX : public TObject
{
 public:
  StPicoKPiX();
  StPicoKPiX(StPicoTrack const* kaon, StPicoTrack const* pion, StPicoTrack const* xaon,
             unsigned short kIdx,unsigned short pIdx, unsigned short xIdx,
             StThreeVectorF const& vtx, float bField);
  ~StPicoKPiX() {}// please keep this non-virtual and NEVER inherit from this class 

  StLorentzVectorF fourMom(double xMassHypothesis) const;
  StLorentzVectorF kaonPionFourMom() const;
  StLorentzVectorF kaonXaonFourMom(double xMassHypothesis) const;
  StLorentzVectorF pionXaonFourMom(double xMassHypothesis) const;

  StThreeVectorF const& kaonMomAtDca() const;
  StThreeVectorF const& pionMomAtDca() const;
  StThreeVectorF const& xaonMomAtDca() const;

  float kaonPionDca() const;
  float kaonXaonDca() const;
  float pionXaonDca() const;

  float kaonDca() const;
  float pionDca() const;
  float xaonDca() const;

  float pointingAngle() const;
  float decayLength()   const;

  unsigned short   kaonIdx() const;
  unsigned short   pionIdx() const;
  unsigned short   xaonIdx() const;

  float dcaDaughters() const;
  float perpDcaToVtx() const;
          
 private:
  // disable copy constructor and assignment operator by making them private 
  // StPicoKPiX(StPicoKPiX const &);
  // StPicoKPiX& operator=(StPicoKPiX const &);

  StThreeVectorF mKaonMomAtDca;
  StThreeVectorF mPionMomAtDca;
  StThreeVectorF mXaonMomAtDca;

  float mKaonPionDca;
  float mKaonXaonDca;
  float mPionXaonDca;

  float mKaonDca;
  float mPionDca;
  float mXaonDca;

  float mPointingAngle;
  float mDecayLength;

  unsigned short  mKaonIdx; // index of track in StPicoDstEvent
  unsigned short  mPionIdx;
  unsigned short  mXaonIdx;

  ClassDef(StPicoKPiX,1)
};
inline StThreeVectorF const& StPicoKPiX::kaonMomAtDca() const { return mKaonMomAtDca;}
inline StThreeVectorF const& StPicoKPiX::pionMomAtDca() const { return mPionMomAtDca;}
inline StThreeVectorF const& StPicoKPiX::xaonMomAtDca() const { return mXaonMomAtDca;}
inline float StPicoKPiX::kaonPionDca()   const { return mKaonPionDca;}
inline float StPicoKPiX::kaonXaonDca()   const { return mKaonXaonDca;}
inline float StPicoKPiX::pionXaonDca()   const { return mPionXaonDca;}
inline float StPicoKPiX::kaonDca() const { return mKaonDca;}
inline float StPicoKPiX::pionDca() const { return mPionDca;}
inline float StPicoKPiX::xaonDca() const { return mXaonDca;}
inline float StPicoKPiX::pointingAngle() const { return mPointingAngle;}
inline float StPicoKPiX::decayLength()   const { return mDecayLength;}
inline unsigned short   StPicoKPiX::kaonIdx() const { return mKaonIdx;}
inline unsigned short   StPicoKPiX::pionIdx() const { return mPionIdx;}
inline unsigned short   StPicoKPiX::xaonIdx() const { return mXaonIdx;}
inline float StPicoKPiX::perpDcaToVtx() const { return mDecayLength*std::sin(mPointingAngle);}
#endif
#endif
