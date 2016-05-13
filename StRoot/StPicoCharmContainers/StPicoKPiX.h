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
  StPicoKPiX(StPicoTrack const* kaon, StPicoTrack const* pion, StPicoTrack const* x,
             unsigned short kIdx,unsigned short pIdx, unsigned short xIdx,
             StThreeVectorF const& vtx, float bField);
  ~StPicoKPiX() {}// please keep this non-virtual and NEVER inherit from this class 

  StLorentzVectorF fourMom(double xMassHypothesis) const;
  StLorentzVectorF kaonPionFourMom() const;
  StLorentzVectorF kaonXFourMom(double xMassHypothesis) const;
  StLorentzVectorF pionXFourMom(double xMassHypothesis) const;

  StThreeVectorF const& kaonMomAtDca() const;
  StThreeVectorF const& pionMomAtDca() const;
  StThreeVectorF const& xMomAtDca()    const;

  float kaonPionDca() const;
  float kaonXDca()    const;
  float pionXDca()    const;
  float pointingAngle() const;
  float decayLength()   const;
  float kaonDca() const;
  float pionDca() const;
  float xDca()    const;

  unsigned short   kaonIdx() const;
  unsigned short   pionIdx() const;
  unsigned short   xIdx() const;

  float dcaDaughters() const;
  float perpDcaToVtx() const;
          
 private:
  // disable copy constructor and assignment operator by making them private 
  // StPicoKPiX(StPicoKPiX const &);
  // StPicoKPiX& operator=(StPicoKPiX const &);

  StThreeVectorF mKaonMomAtDca;
  StThreeVectorF mPionMomAtDca;
  StThreeVectorF mXMomAtDca;

  float mKaonPionDca;
  float mKaonXDca;
  float mPionXDca;
  float mPointingAngle;
  float mDecayLength;

  float mKaonDca;
  float mPionDca;
  float mXDca;

  unsigned short  mKaonIdx; // index of track in StPicoDstEvent
  unsigned short  mPionIdx;
  unsigned short  mXIdx;

  ClassDef(StPicoKPiX,1)
};
inline StThreeVectorF const& StPicoKPiX::kaonMomAtDca() const { return mKaonMomAtDca;}
inline StThreeVectorF const& StPicoKPiX::pionMomAtDca() const { return mPionMomAtDca;}
inline StThreeVectorF const& StPicoKPiX::xMomAtDca()    const { return mXMomAtDca;}
inline float StPicoKPiX::kaonPionDca()   const { return mKaonPionDca;}
inline float StPicoKPiX::kaonXDca()      const { return mKaonXDca;}
inline float StPicoKPiX::pionXDca()      const { return mPionXDca;}
inline float StPicoKPiX::pointingAngle() const { return mPointingAngle;}
inline float StPicoKPiX::decayLength()   const { return mDecayLength;}
inline float StPicoKPiX::kaonDca() const { return mKaonDca;}
inline float StPicoKPiX::pionDca() const { return mPionDca;}
inline float StPicoKPiX::xDca()    const { return mXDca;}
inline unsigned short   StPicoKPiX::kaonIdx() const { return mKaonIdx;}
inline unsigned short   StPicoKPiX::pionIdx() const { return mPionIdx;}
inline unsigned short   StPicoKPiX::xIdx()    const { return mXIdx;}
inline float StPicoKPiX::perpDcaToVtx() const { return mDecayLength*std::sin(mPointingAngle);}
#endif
#endif
