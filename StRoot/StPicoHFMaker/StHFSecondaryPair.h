#ifndef StHFSecondaryPair_hh
#define StHFSecondaryPair_hh
#ifdef __ROOT__

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

  unsigned short mDcaDaughters; // dcaDaughters * 10000. 1 μm precision
  char           mCosThetaStar; // cosThetaStar * 100.

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
inline float StHFSecondaryPair::dcaDaughters() const { return static_cast<float>(mDcaDaughters)/10000.;}
inline float StHFSecondaryPair::cosThetaStar() const { return static_cast<float>(mCosThetaStar/100.);}

#endif
#endif
