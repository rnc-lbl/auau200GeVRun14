#ifndef StHFPair_hh
#define StHFPair_hh
#ifdef __ROOT__

/* **************************************************
 *  Generic class calculating and storing primary pairs in HF analysis
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
class StHFSecondaryPair;

class StHFPair : public TObject
{
 public:
  StHFPair();
  StHFPair(StHFPair const *);

  StHFPair(StPicoTrack const * particle1, StPicoTrack const * particle2, 
	   float p1MassHypo, float p2MassHypo,
	   unsigned short particle1Idx, unsigned short particle2Idx,
	   StThreeVectorF const & vtx, float bField);

  StHFPair(StPicoTrack const * particle1, StHFSecondaryPair const * particle2, 
	   StPicoTrack const * secondaryP1, StPicoTrack const * secondaryP2,
	   float p1MassHypo, float p2MassHypo,
	   unsigned short particle1Idx, unsigned short particle2Idx,
	   StThreeVectorF const & vtx, float bField);

  ~StHFPair() {;}

  StLorentzVectorF const & lorentzVector() const;
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
  StHFPair(StHFPair const &);
  StHFPair& operator=(StHFPair const &);
  StLorentzVectorF mLorentzVector; // this owns four float only

  float mPointingAngle;
  float mDecayLength;
  float mParticle1Dca;
  float mParticle2Dca;

  unsigned short  mParticle1Idx; // index of track in StPicoDstEvent
  unsigned short  mParticle2Idx;

  float mDcaDaughters;
  float mCosThetaStar; 

  ClassDef(StHFPair,1)
};
inline StLorentzVectorF const & StHFPair::lorentzVector() const { return mLorentzVector;}
inline float StHFPair::m()    const { return mLorentzVector.m();}
inline float StHFPair::pt()   const { return mLorentzVector.perp();}
inline float StHFPair::eta()  const { return mLorentzVector.pseudoRapidity();}
inline float StHFPair::phi()  const { return mLorentzVector.phi();}
inline float StHFPair::pointingAngle() const { return mPointingAngle;}
inline float StHFPair::decayLength()   const { return mDecayLength;}
inline float StHFPair::particle1Dca()  const { return mParticle1Dca;}
inline float StHFPair::particle2Dca()  const { return mParticle2Dca;}
inline unsigned short   StHFPair::particle1Idx() const { return mParticle1Idx;}
inline unsigned short   StHFPair::particle2Idx() const { return mParticle2Idx;}
inline float StHFPair::dcaDaughters() const { return mDcaDaughters;}
inline float StHFPair::cosThetaStar() const { return mCosThetaStar;}

#endif
#endif

