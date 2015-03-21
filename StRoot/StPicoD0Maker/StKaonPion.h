#ifndef StKaonPion_hh
#define StKaonPion_hh
#ifdef __ROOT__

#include "TObject.h"
#include "TClonesArray.h"
#include "StLorentzVectorF.hh"

class StPicoTrack;
class StPicoEvent;

class StKaonPion : public TObject
{
 public:
  StKaonPion();
  StKaonPion(StKaonPion const *);
  StKaonPion(StPicoTrack const * const, StPicoTrack const * const ,unsigned short const,unsigned short const,
             StThreeVectorF const &, float const &);
  ~StKaonPion(); // please keep this non-virtual and NEVER inherit from this class 
  void clear();

  unsigned short   kaonIdx() const { return mKaonIdx;}
  unsigned short   pionIdx() const { return mPionIdx;}
  float m()    const { return mLorentzVector.m();}
  float pt()   const { return mLorentzVector.perp();}
  float eta()  const { return mLorentzVector.pseudoRapidity();}
  float phi()  const { return mLorentzVector.phi();}
  float dcaDaughters() const { return static_cast<float>(mDcaDaughters)/10000.;}
  float cosThetaStar() const { return static_cast<float>(mCosThetaStar/100.);}
  StLorentzVectorF const & lorentzVector() const { return mLorentzVector;}
          
 private:
  StKaonPion& operator=(StKaonPion const &); // disable assignment operator (once C++11 is available in STAR you can use delete specifier instead)
  StLorentzVectorF mLorentzVector; // this owns four float only

  float mPointingAngle;
  float mDecayLength;
  float mKaonDca;
  float mPionDca;

  unsigned short  mKaonIdx; // index of track in StPicoDstEvent
  unsigned short  mPionIdx;

  unsigned short mDcaDaughters; // dcaDaughters * 10000. 1 μm precision
  char mCosThetaStar; // cosThetaStar * 100.

  ClassDef(StKaonPion,1)
};


#endif
#endif

