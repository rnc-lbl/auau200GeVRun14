#ifndef StHFPair_hh
#define StHFPair_hh

/* **************************************************
 *  Generic class calculating and storing pairs in HF analysis
 *  Allows to combine:
 *  - two particles, using
 *      StHFPair(StPicoTrack const * particle1, StPicoTrack const * particle2, ...
 *  - a particle and another pair, using
 *      StHFPair(StPicoTrack const * particle1, StHFPair * particle2, ...
 *    - in the current implementation the incoming pair is seen as having charge = 0
 *    - after determining the vertex of particle and incoming pair, the 
 *      decay vertex (tertiary vertex) of incoming particle can be updated
 *
 * **************************************************
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

class StHFPair : public TObject
{
 public:
  StHFPair();
  StHFPair(StHFPair const *);

  StHFPair(StPicoTrack const * particle1, StPicoTrack const * particle2, 
	   float p1MassHypo, float p2MassHypo,
	   unsigned short p1Idx, unsigned short p2Idx,
	   StThreeVectorF const & vtx, float bField);

  StHFPair(StPicoTrack const * particle1, StHFPair * particle2, 
	   float p1MassHypo, float p2MassHypo,
	   unsigned short p1Idx, unsigned short p2Idx,
	   StThreeVectorF const & vtx, float bField);

  ~StHFPair() {;}
  

  StLorentzVectorF const & lorentzVector() const;
  float m()    const;
  float pt()   const;
  float eta()  const;
  float phi()  const;
  float pointingAngle() const;
  float pointingAngle(StThreeVectorF const & vtx2) const;
  float decayLength() const;
  float decayLength(StThreeVectorF const & vtx2) const;
  float particle1Dca() const;
  float particle1Dca(StPicoTrack const * p1track, StThreeVectorF const & vtx2) const;
  float particle2Dca() const;
  float particle2Dca(StPicoTrack const * p1track, StThreeVectorF const & vtx2) const;
  unsigned short particle1Idx() const;
  unsigned short particle2Idx() const;
  float dcaDaughters() const;
  float cosThetaStar() const;
  float v0x() const;
  float v0y() const;
  float v0z() const;
  float px() const;
  float py() const;
  float pz() const;

 private:
  StHFPair(StHFPair const &);
  StHFPair& operator=(StHFPair const &);
  StLorentzVectorF mLorentzVector; 

  float mPointingAngle;
  float mDecayLength;
  float mParticle1Dca;
  float mParticle2Dca;

  unsigned short  mParticle1Idx; // index of track in StPicoDstEvent 
  unsigned short  mParticle2Idx; // index of track in StPicoDstEvent for particle, idx in tertiary vertex array for pair 

  float mDcaDaughters;
  float mCosThetaStar;

  float mV0x; // reconstructed vertex pos
  float mV0y;
  float mV0z;
  
  ClassDef(StHFPair,1)
};
inline StLorentzVectorF const & StHFPair::lorentzVector() const { return mLorentzVector;}
inline float StHFPair::m()    const { return mLorentzVector.m();}
inline float StHFPair::pt()   const { return mLorentzVector.perp();}
inline float StHFPair::eta()  const { return mLorentzVector.pseudoRapidity();}
inline float StHFPair::phi()  const { return mLorentzVector.phi();}
inline float StHFPair::px()   const { return mLorentzVector.px();}
inline float StHFPair::py()   const { return mLorentzVector.py();}
inline float StHFPair::pz()   const { return mLorentzVector.pz();}
inline float StHFPair::pointingAngle() const { return mPointingAngle;}
inline float StHFPair::decayLength()   const { return mDecayLength;}
inline float StHFPair::particle1Dca()  const { return mParticle1Dca;}
inline float StHFPair::particle2Dca()  const { return mParticle2Dca;}
inline unsigned short StHFPair::particle1Idx() const { return mParticle1Idx;}
inline unsigned short StHFPair::particle2Idx() const { return mParticle2Idx;}
inline float StHFPair::dcaDaughters() const { return mDcaDaughters;}
inline float StHFPair::cosThetaStar() const { return mCosThetaStar;}
inline float StHFPair::v0x() const { return mV0x;}
inline float StHFPair::v0y() const { return mV0y;}
inline float StHFPair::v0z() const { return mV0z;}
#endif

