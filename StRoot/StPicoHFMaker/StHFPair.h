#ifndef StHFPair_hh
#define StHFPair_hh
#ifdef __ROOT__

#include "TObject.h"
#include "TClonesArray.h"
#include "StLorentzVectorF.hh"

class StPicoTrack;
class StPicoEvent;

class StHFPair : public TObject
{
 public:
  StHFPair();
  StHFPair(StHFPair const *);
  StHFPair(StPicoTrack const * particle1, StPicoTrack const * particle2, 
	   float particle1MassHypo, float particle2MassHypo,
	   unsigned short particle1Idx, unsigned short particle2Idx,
	   StThreeVectorF const & vtx, float bField);
  ~StHFPair() {;}

  StLorentzVectorF const & lorentzVector() const { return mLorentzVector;}
  float m()    const;
  float pt()   const;
  float eta()  const;
  float phi()  const;
  float poitingAngle() const;
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

  float mParticle1MassHypo;   // Mass hypothesis - particle 1 - M_KAON_PLUS
  float mParticle2MassHypo;   // Mass Hypothesis - particle 2 - M_PION_PLUS

  float mPointingAngle;
  float mDecayLength;
  float mParticle1Dca;
  float mParticle2Dca;

  unsigned short  mParticle1Idx; // index of track in StPicoDstEvent
  unsigned short  mParticle2Idx;

  unsigned short mDcaDaughters; // dcaDaughters * 10000. 1 μm precision
  char mCosThetaStar; // cosThetaStar * 100.

  ClassDef(StHFPair,1)
};

inline float StHFPair::m()    const { return mLorentzVector.m();}
inline float StHFPair::pt()   const { return mLorentzVector.perp();}
inline float StHFPair::eta()  const { return mLorentzVector.pseudoRapidity();}
inline float StHFPair::phi()  const { return mLorentzVector.phi();}
inline float StHFPair::pointingAngle() const { return mPointingAngle;}
inline float StHFPair::decayLength() const { return mDecayLength;}
inline float StHFPair::particle1Dca() const { return mParticle1Dca;}
inline float StHFPair::particle2Dca() const { return mParticle2Dca;}
inline unsigned short   StHFPair::particle1Idx() const { return mParticle1Idx;}
inline unsigned short   StHFPair::particle2Idx() const { return mParticle2Idx;}
inline float StHFPair::dcaDaughters() const { return static_cast<float>(mDcaDaughters)/10000.;}
inline float StHFPair::cosThetaStar() const { return static_cast<float>(mCosThetaStar/100.);}

#endif
#endif

