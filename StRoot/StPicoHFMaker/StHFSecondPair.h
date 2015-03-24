#ifndef StHFSecondPair_hh
#define StHFSecondPair_hh
#ifdef __ROOT__

#include "TObject.h"
#include "TClonesArray.h"
#include "StLorentzVectorF.hh"

class StPicoTrack;
class StPicoEvent;

class StHFSecondPair : public TObject
{
 public:
  StHFSecondPair();
  StHFSecondPair(StHFSecondPair const *);

  StHFSecondPair(StPicoTrack const * particle1, StPicoTrack const * particle2, 
	   float p1MassHypo, float p2MassHypo,
	   unsigned short particle1Idx, unsigned short particle2Idx,
	   StThreeVectorF const & vtx, float bField);

  StHFSecondPair(StPicoTrack const * particle1, StHFSecondPair const * particle2, 
	   float p1MassHypo, float p2MassHypo,
	   unsigned short particle1Idx, unsigned short particle2Idx,
	   StThreeVectorF const & vtx, float bField);

  ~StHFSecondPair() {;}

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
  StHFSecondPair(StHFSecondPair const &);
  StHFSecondPair& operator=(StHFSecondPair const &);
  StLorentzVectorF mLorentzVector; // this owns four float only

  float mPointingAngle;
  float mDecayLength;
  float mParticle1Dca;
  float mParticle2Dca;

  unsigned short  mParticle1Idx; // index of track in StPicoDstEvent
  unsigned short  mParticle2Idx;

  unsigned short mDcaDaughters; // dcaDaughters * 10000. 1 μm precision
  char           mCosThetaStar; // cosThetaStar * 100.

  ClassDef(StHFSecondPair,1)
};

inline float StHFSecondPair::m()    const { return mLorentzVector.m();}
inline float StHFSecondPair::pt()   const { return mLorentzVector.perp();}
inline float StHFSecondPair::eta()  const { return mLorentzVector.pseudoRapidity();}
inline float StHFSecondPair::phi()  const { return mLorentzVector.phi();}
inline float StHFSecondPair::pointingAngle() const { return mPointingAngle;}
inline float StHFSecondPair::decayLength()   const { return mDecayLength;}
inline float StHFSecondPair::particle1Dca()  const { return mParticle1Dca;}
inline float StHFSecondPair::particle2Dca()  const { return mParticle2Dca;}
inline unsigned short   StHFSecondPair::particle1Idx() const { return mParticle1Idx;}
inline unsigned short   StHFSecondPair::particle2Idx() const { return mParticle2Idx;}
inline float StHFSecondPair::dcaDaughters() const { return static_cast<float>(mDcaDaughters)/10000.;}
inline float StHFSecondPair::cosThetaStar() const { return static_cast<float>(mCosThetaStar/100.);}

#endif
#endif

