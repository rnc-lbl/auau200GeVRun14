#ifndef StHFSecondaryPair_hh
#define StHFSecondaryPair_hh
#ifdef __ROOT__

/* **************************************************
 *  Generic class calculating and storing secondary pairs in HF analysis
 *
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)   
 *
 * **************************************************
 */

#include "TObject.h"
#include "TClonesArray.h"
#include "StLorentzVectorF.hh"

class StPicoTrack;
class StPicoEvent;

class StHFSecondaryPair : public TObject
{
 public:
  StHFSecondaryPair();
  StHFSecondaryPair(StHFSecondaryPair const *);

  StHFSecondaryPair(StPicoTrack const * particle1, StPicoTrack const * particle2, 
	   float p1MassHypo, float p2MassHypo,
	   unsigned short particle1Idx, unsigned short particle2Idx,
	   StThreeVectorF const & vtx, float bField);

  StHFSecondaryPair(StPicoTrack const * particle1, StHFSecondaryPair const * particle2, 
	   float p1MassHypo, float p2MassHypo,
	   unsigned short particle1Idx, unsigned short particle2Idx,
	   StThreeVectorF const & vtx, float bField);

  ~StHFSecondaryPair() {;}

  StLorentzVectorF const & lorentzVector() const { return mLorentzVector;}
  float m()    const;
  float pt()   const;
  float eta()  const;
  float phi()  const;
  float pointingAngle() const;
  float decayLength() const;
  float particle1Dca() const;
  float particle2Dca() const;
  unsigned short   particle1Idx() const;
  unsigned short   particle2Idx() const;
  float dcaDaughters() const;
  float cosThetaStar() const;
          
 private:
  // disable copy constructor and assignment operator by making them private (once C++11 is available in STAR you can use delete specifier instead)
  StHFSecondaryPair(StHFSecondaryPair const &);
  StHFSecondaryPair& operator=(StHFSecondaryPair const &);
  StLorentzVectorF mLorentzVector; // this owns four float only

  float mPointingAngle;
  float mDecayLength;
  float mParticle1Dca;
  float mParticle2Dca;

  unsigned short  mParticle1Idx; // index of track in StPicoDstEvent
  unsigned short  mParticle2Idx;

  float mDcaDaughters;
  float mCosThetaStar;

  ClassDef(StHFSecondaryPair,1)
};

inline float StHFSecondaryPair::m()    const { return mLorentzVector.m();}
inline float StHFSecondaryPair::pt()   const { return mLorentzVector.perp();}
inline float StHFSecondaryPair::eta()  const { return mLorentzVector.pseudoRapidity();}
inline float StHFSecondaryPair::phi()  const { return mLorentzVector.phi();}
inline float StHFSecondaryPair::pointingAngle() const { return mPointingAngle;}
inline float StHFSecondaryPair::decayLength()   const { return mDecayLength;}
inline float StHFSecondaryPair::particle1Dca()  const { return mParticle1Dca;}
inline float StHFSecondaryPair::particle2Dca()  const { return mParticle2Dca;}
inline unsigned short   StHFSecondaryPair::particle1Idx() const { return mParticle1Idx;}
inline unsigned short   StHFSecondaryPair::particle2Idx() const { return mParticle2Idx;}
inline float StHFSecondaryPair::dcaDaughters() const { return mDcaDaughters;}
inline float StHFSecondaryPair::cosThetaStar() const { return mCosThetaStar;}

#endif
#endif

