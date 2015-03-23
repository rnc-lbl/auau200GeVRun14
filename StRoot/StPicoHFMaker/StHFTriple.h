#ifndef StHFTriple_hh
#define StHFTriple_hh
#ifdef __ROOT__

#include "TObject.h"
#include "TClonesArray.h"
#include "StLorentzVectorF.hh"

class StPicoTrack;
class StPicoEvent;

class StHFTriple : public TObject
{
 public:
  StHFTriple();
  StHFTriple(StHFTriple const *);
  StHFTriple(StPicoTrack const * particle1, StPicoTrack const * particle2, 
	   float particle1MassHypo, float particle2MassHypo,
	   unsigned short particle1Idx, unsigned short particle2Idx,
	   StThreeVectorF const & vtx, float bField);
  ~StHFTriple() {;}

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
  StHFTriple(StHFTriple const &);
  StHFTriple& operator=(StHFTriple const &);
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

  ClassDef(StHFTriple,1)
};

inline float StHFTriple::m()    const { return mLorentzVector.m();}
inline float StHFTriple::pt()   const { return mLorentzVector.perp();}
inline float StHFTriple::eta()  const { return mLorentzVector.pseudoRapidity();}
inline float StHFTriple::phi()  const { return mLorentzVector.phi();}
inline float StHFTriple::pointingAngle() const { return mPointingAngle;}
inline float StHFTriple::decayLength()   const { return mDecayLength;}
inline float StHFTriple::particle1Dca()  const { return mParticle1Dca;}
inline float StHFTriple::particle2Dca()  const { return mParticle2Dca;}
inline unsigned short   StHFTriple::particle1Idx() const { return mParticle1Idx;}
inline unsigned short   StHFTriple::particle2Idx() const { return mParticle2Idx;}
inline float StHFTriple::dcaDaughters() const { return static_cast<float>(mDcaDaughters)/10000.;}
inline float StHFTriple::cosThetaStar() const { return static_cast<float>(mCosThetaStar/100.);}

#endif
#endif

