#ifndef StKaonPion_hh
#define StKaonPion_hh
#ifdef __ROOT__

#include "TObject.h"
#include "TClonesArray.h"
#include "StLorentzVectorF.hh"

class StPicoTrack;
class StPicoEvent;

class StKaonPion : public TObject {
 public:
  StKaonPion();
  StKaonPion(StKaonPion*);
  StKaonPion(StPicoTrack*, StPicoTrack*,unsigned short,unsigned short);
  ~StKaonPion(); // please keep this non-virtual and NEVER inherit from this class 
  void clear();

  // while ROOT types are not necessary here they are used for consistency with StPicoDst
  unsigned short   kaonIdx() const { return mKaonIdx;}
  unsigned short   pionIdx() const { return mPionIdx;}
  Float_t m()    const { return (Float_t)mM/10000.;}
  Float_t pt()   const { return (Float_t)mPt/1000.;}
  Float_t eta()   const { return (Float_t)mEta/1000.;}
  Float_t phi()   const { return (Float_t)mPhi/1000.;}
  Float_t openAngle() const { return (Float_t)mOpenAngle/1000.;}
  Float_t cosThetaStar() const { return (Float_t)mCosThetaStar/100.;}
  StLorentzVectorF lorentzVector();
          
 protected:
  unsigned short  mKaonIdx; // index of track in StPicoDstEvent
  unsigned short  mPionIdx;

  UShort_t mM; // mass * 10000. : 0.1 MeV precision
  UShort_t mPt; // pT * 1000.  : MeV precision
  Short_t mEta; // eta * 1000.
  Short_t mPhi; // phi * 1000.

  Short_t mOpenAngle; // openAngle * 1000.
  Char_t mCosThetaStar; // cosThetaStar * 100.

  ClassDef(StKaonPion,1)
};


#endif
#endif

