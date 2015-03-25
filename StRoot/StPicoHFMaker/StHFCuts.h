#ifndef StHFCUTS_H
#define StHFCUTS_H
#ifdef __ROOT__

/* **************************************************
 *  Cut class for HF analysis
 *  - used as a singlton
 *
 *  Authors:
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

  void SetCutVzMax(float f)            { mVzMax = f; }
  void SetCutVzVpdVzMax(float f)       { mVzVpdVzMax = f; }

  void SetCutNHitsFitMax(int i)        { mNHitsFitMax = i; }
  void SetCutRequireHFT(bool b)        { mRequireHFT = b; }
  void SetCutNHitsFitnHitsMax(float f) { mNHitsFitnHitsMax = f; }

  void SetCutNSigmaPion(float f)       { mNSigmaPion = f; }
  void SetCutPionPt(float f)           { mPionPt = f; }
  void SetCutPionEta(float f)          { mPionEta = f; }
  
  void SetCutNSigmaKaon(float f)       { mNSigmaKaon = f; }
  void SetCutKaonPt(float f)           { mKaonPt = f; }
  void SetCutKaonEta(float f)          { mKaonEta = f; }

  void SetCutNSigmaProton(float f)     { mNSigmaProton = f; }
  void SetCutProtonPt(float f)         { mProtonPt = f; }
  void SetCutProtonEta(float f)        { mProtonEta = f; }

  void SetCutPrimPair(float dcaDaughtersMax, float decayLengthMin, float massMin, float massMax)  {
    mPrimDcaDaughtersMax = dcaDaughtersMax; mPrimDecayLengthMin = decayLengthMin;
    mPrimMassMin = massMin; mPrimMassMax = massMax; }

  void SetCutSecondaryPair(float dcaDaughtersMax, float decayLengthMin, float massMin, float massMax)  {
    mSecondDcaDaughtersMax = dcaDaughtersMax; mSecondDecayLengthMin = decayLengthMin;
    mSecondMassMin = massMin; mSecondMassMax = massMax; }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
  
  const float& CutVzMax()                 const { return mVzMax; }
  const float& CutVzVpdVzMax()            const { return mVzVpdVzMax; }

  const int& CutNHitsFitMax()             const { return mNHitsFitMax; }
  const bool& CutRequireHFT()             const { return mRequireHFT; }
  const float& CutNHitsFitnHitsMax()      const { return mNHitsFitnHitsMax; }

  const float& CutNSigmaPion()            const { return mNSigmaPion; }
  const float& CutPionPt()                const { return mPionPt; }
  const float& CutPionEta()               const { return mPionEta; }
  
  const float& CutNSigmaKaon()            const { return mNSigmaKaon; }
  const float& CutKaonPt()                const { return mKaonPt; }
  const float& CutKaonEta()               const { return mKaonEta; }

  const float& CutNSigmaProton()          const { return mNSigmaProton; }
  const float& CutProtonPt()              const { return mProtonPt; }
  const float& CutProtonEta()             const { return mProtonEta; }

  const float& CutPrimDcaDaughtersMax()   const { return mPrimDcaDaughtersMax; }
  const float& CutPrimDecayLengthMin()    const { return mPrimDecayLengthMin; }
  const float& CutPrimMassMin()           const { return mPrimMassMin; }
  const float& CutPrimMassMax()           const { return mPrimMassMax; }

  const float& CutSecondDcaDaughtersMax() const { return mSecondDcaDaughtersMax; }
  const float& CutSecondDecayLengthMin()  const { return mSecondDecayLengthMin; }
  const float& CutSecondMassMin()         const { return mSecondMassMin; }
  const float& CutSecondMassMax()         const { return mSecondMassMax; }

  const float& MassHypothesisParticle1()        const { return mMassHypo[0]; }
  const float& MassHypothesisParticle2()        const { return mMassHypo[1]; }
  const float& MassHypothesisParticle3()        const { return mMassHypo[2]; }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  bool IsGoodEvent();

  bool IsGoodTrack(StPicoTrack const *trk) const;



  //  bool IsPionTPC(StPicoTrack const *trk)
  // bool IsPionTOF(StPicoTrack const *trk, float const & bTofBeta)
  

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

  // -- tracking
  int   mNHitsFitMax;
  bool  mRequireHFT;
  float mNHitsFitnHitsMax; // applied in StPicoDstMaker for Run14 data, no need to reapply here
  
  float mMassHypo[3];

  // ------------------------------------------
  // -- Basic cuts for particles
  //    too fill particle vectors
  // ------------------------------------------
  // -- pions
  float mNSigmaPion;
  float mPionPt;
  float mPionEta;
  
  // -- kaons
  float mNSigmaKaon;
  float mKaonPt;
  float mKaonEta;

  // -- protons
  float mNSigmaProton;
  float mProtonPt;
  float mProtonEta;

  // ------------------------------------------
  // -- Pair cuts for primary pair
  // ------------------------------------------
  float mPrimDcaDaughtersMax;
  float mPrimDecayLengthMin; 
  float mPrimMassMin;
  float mPrimMassMax;

  // ------------------------------------------
  // -- Pair cuts for secondary pair
  // ------------------------------------------
  float mSecondDcaDaughtersMax;
  float mSecondDecayLengthMin; 
  float mSecondMassMin;
  float mSecondMassMax;

  ClassDef(StHFCuts,1)
};

#endif
#endif
