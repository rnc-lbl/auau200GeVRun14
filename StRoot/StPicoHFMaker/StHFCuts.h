#ifndef StHFCUTS_H
#define StHFCUTS_H
#ifdef __ROOT__

/* **************************************************
 *  Cut class for HF analysis
 *  - used as a singlton
 *
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)   
 *
 * **************************************************
 */

#include "TObject.h"

class StPicoTrack;

class StHFPair;
class StHFSecondaryPair;
class StHFTriplet;

class StHFCuts : public TObject
{
 public:
  
  static StHFCuts* Instance();
  
  ~StHFCuts() {;}
  
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  void SetCutVzMax(float f)            { mVzMax            = f; }
  void SetCutVzVpdVzMax(float f)       { mVzVpdVzMax       = f; }
  void SetCutTriggerWord(UShort_t us)  { mTriggerWord      = us; }

  void SetCutNHitsFitMax(int i)        { mNHitsFitMax      = i; }
  void SetCutRequireHFT(bool b)        { mRequireHFT       = b; }
  void SetCutNHitsFitnHitsMax(float f) { mNHitsFitnHitsMax = f; }

  /* void SetCutNSigmaPion(float f)       { mNSigmaPion = f; } */
  /* void SetCutPionPt(float f)           { mPionPt = f; } */
  /* void SetCutPionEta(float f)          { mPionEta = f; } */
  
  /* void SetCutNSigmaKaon(float f)       { mNSigmaKaon = f; } */
  /* void SetCutKaonPt(float f)           { mKaonPt = f; } */
  /* void SetCutKaonEta(float f)          { mKaonEta = f; } */

  /* void SetCutNSigmaProton(float f)     { mNSigmaProton = f; } */
  /* void SetCutProtonPt(float f)         { mProtonPt = f; } */
  /* void SetCutProtonEta(float f)        { mProtonEta = f; } */

  void SetCutPrimaryPair(float dcaDaughtersMax, float decayLengthMin, float cosThetaMin, 
			 float massMin, float massMax)  {
    mPrimaryDcaDaughtersMax = dcaDaughtersMax; mPrimaryDecayLengthMin = decayLengthMin;
    mPrimaryCosThetaMin = cosThetaMin;
    mPrimaryMassMin = massMin; mPrimaryMassMax = massMax; }

  void SetCutSecondaryPair(float dcaDaughtersMax, float decayLengthMin, float cosThetaMin, 
			 float massMin, float massMax)  {
    mSecondaryDcaDaughtersMax = dcaDaughtersMax; mSecondaryDecayLengthMin = decayLengthMin;
    mSecondaryCosThetaMin = cosThetaMin;
    mSecondaryMassMin = massMin; mSecondaryMassMax = massMax; }

  void SetCutTriplet(float dcaDaughters12Max, float dcaDaughters23Max, float dcaDaughters31Max, 
		     float decayLengthMin, float cosThetaMin, float massMin, float massMax)  {
    mTripletDcaDaughters12Max = dcaDaughters12Max; mTripletDcaDaughters23Max = dcaDaughters23Max; mTripletDcaDaughters31Max = dcaDaughters31Max; 
    mTripletDecayLengthMin = decayLengthMin; mTripletCosThetaMin = cosThetaMin;
    mTripletMassMin = massMin; mTripletMassMax = massMax; }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
  
  const float&    CutVzMax()                    const { return mVzMax; }
  const float&    CutVzVpdVzMax()               const { return mVzVpdVzMax; }
  const UShort_t& CutTriggerWord()              const { return mTriggerWord; }

  const int&      CutNHitsFitMax()              const { return mNHitsFitMax; }
  const bool&     CutRequireHFT()               const { return mRequireHFT; }
  const float&    CutNHitsFitnHitsMax()         const { return mNHitsFitnHitsMax; }

  /* const float& CutNSigmaPion()            const { return mNSigmaPion; } */
  /* const float& CutPionPt()                const { return mPionPt; } */
  /* const float& CutPionEta()               const { return mPionEta; } */
  
  /* const float& CutNSigmaKaon()            const { return mNSigmaKaon; } */
  /* const float& CutKaonPt()                const { return mKaonPt; } */
  /* const float& CutKaonEta()               const { return mKaonEta; } */

  /* const float& CutNSigmaProton()          const { return mNSigmaProton; } */
  /* const float& CutProtonPt()              const { return mProtonPt; } */
  /* const float& CutProtonEta()             const { return mProtonEta; } */

  const float&    CutPrimaryDcaDaughtersMax()   const { return mPrimaryDcaDaughtersMax; }
  const float&    CutPrimaryDecayLengthMin()    const { return mPrimaryDecayLengthMin; }
  const float&    CutPrimaryCosThetaMin()       const { return mPrimaryCosThetaMin; }
  const float&    CutPrimaryMassMin()           const { return mPrimaryMassMin; }
  const float&    CutPrimaryMassMax()           const { return mPrimaryMassMax; }

  const float&    CutSecondaryDcaDaughtersMax() const { return mSecondaryDcaDaughtersMax; }
  const float&    CutSecondaryDecayLengthMin()  const { return mSecondaryDecayLengthMin; }
  const float&    CutSecondaryCosThetaMin()     const { return mSecondaryCosThetaMin; }
  const float&    CutSecondaryMassMin()         const { return mSecondaryMassMin; }
  const float&    CutSecondaryMassMax()         const { return mSecondaryMassMax; }

  const float&    CutTripletDcaDaughters12Max()   const { return mTripletDcaDaughters12Max; }
  const float&    CutTripletDcaDaughters23Max()   const { return mTripletDcaDaughters23Max; }
  const float&    CutTripletDcaDaughters31Max()   const { return mTripletDcaDaughters31Max; }
  const float&    CutTripletDecayLengthMin()    const { return mTripletDecayLengthMin; }
  const float&    CutTripletCosThetaMin()       const { return mTripletCosThetaMin; }
  const float&    CutTripletMassMin()           const { return mTripletMassMin; }
  const float&    CutTripletMassMax()           const { return mTripletMassMax; }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  bool IsGoodTrack(StPicoTrack const *trk) const;

  bool IsGoodPrimaryPair(StHFPair const & pair) const;
  bool IsGoodSecondaryPair(StHFSecondaryPair const & pair) const;
  bool IsGoodTriplet(StHFTriplet const & triplet) const;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

 private:
  
  StHFCuts();  
  StHFCuts(StHFCuts const &);       
  StHFCuts& operator=(StHFCuts const &); 

  static StHFCuts* mStHFCuts;

  // -- event cuts
  float mVzMax;
  float mVzVpdVzMax;
  UShort_t mTriggerWord;  // first five bits see http://rnc.lbl.gov/~xdong/SoftHadron/picoDst.html

  // -- tracking
  int   mNHitsFitMax;
  bool  mRequireHFT;
  float mNHitsFitnHitsMax; // applied in StPicoDstMaker for Run14 data, no need to reapply here
  
  // ------------------------------------------
  // -- Basic cuts for particles
  //    too fill particle vectors
  // ------------------------------------------
  /* // -- pions */
  /* float mNSigmaPion; */
  /* float mPionPt; */
  /* float mPionEta; */
  
  /* // -- kaons */
  /* float mNSigmaKaon; */
  /* float mKaonPt; */
  /* float mKaonEta; */

  /* // -- protons */
  /* float mNSigmaProton; */
  /* float mProtonPt; */
  /* float mProtonEta; */

  // ------------------------------------------
  // -- Pair cuts for primary pair
  // ------------------------------------------
  float mPrimaryDcaDaughtersMax;
  float mPrimaryDecayLengthMin; 
  float mPrimaryCosThetaMin;
  float mPrimaryMassMin;
  float mPrimaryMassMax;

  // ------------------------------------------
  // -- Pair cuts for secondary pair
  // ------------------------------------------
  float mSecondaryDcaDaughtersMax;
  float mSecondaryDecayLengthMin; 
  float mSecondaryCosThetaMin;
  float mSecondaryMassMin;
  float mSecondaryMassMax;

  // ------------------------------------------
  // -- Cuts of triplet
  // ------------------------------------------
  float mTripletDcaDaughters12Max;
  float mTripletDcaDaughters23Max;
  float mTripletDcaDaughters31Max;
  float mTripletDecayLengthMin; 
  float mTripletCosThetaMin;
  float mTripletMassMin;
  float mTripletMassMax;

  ClassDef(StHFCuts,1)
};

#endif
#endif
