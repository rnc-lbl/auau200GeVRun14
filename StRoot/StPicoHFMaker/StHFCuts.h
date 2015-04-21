#ifndef StHFCUTS_H
#define StHFCUTS_H
#ifdef __ROOT__

/* **************************************************
 *  Cut class for HF analysis
 *  - Base class for cuts 
 *
 *  Initial Authors:  
 *            Xin Dong        (xdong@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *          **Jochen Thaeder  (jmthader@lbl.gov)   
 *
 *  Contributing Authors
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Guannan Xie     (guannanxie@lbl.gov)
 *
 *  ** Code Maintainer
 *
 * **************************************************
 */

#include <vector>

#include "TNamed.h"
#include "TString.h"
#include "StLorentzVectorF.hh" 

class StPicoTrack;
class StPicoEvent;

class StHFPair;
class StHFTriplet;

class StHFCuts : public TNamed
{
 public:
  
  StHFCuts();
  StHFCuts(const Char_t *name);
  ~StHFCuts() {;}
  
  void init();

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  bool isGoodEvent(StPicoEvent const * const event, int *aEventCuts);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  bool isGoodRun(StPicoEvent const *picoEvent) const;

  bool isGoodTrack(StPicoTrack const *trk) const;
   
  bool isTPCPion(StPicoTrack const *trk) const;
  bool isTPCKaon(StPicoTrack const *trk) const;
  bool isTPCProton(StPicoTrack const *trk) const;

  // -- Is TOF particle (if track has no TOF information - return false)
  bool isTOFPion(StPicoTrack const *trk) const;
  bool isTOFKaon(StPicoTrack const *trk) const;
  bool isTOFProton(StPicoTrack const *trk) const;

  // -- Is TOF particle (if track has no TOF information - return true)
  bool isHybridTOFPion(StPicoTrack const *trk) const;
  bool isHybridTOFKaon(StPicoTrack const *trk) const;
  bool isHybridTOFProton(StPicoTrack const *trk) const;

  bool isTOFPion(StPicoTrack const *trk,   float const & tofBeta) const;
  bool isTOFKaon(StPicoTrack const *trk,   float const & tofBeta) const;
  bool isTOFProton(StPicoTrack const *trk, float const & tofBeta) const;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  bool isClosePair(StHFPair const & pair) const;

  bool isGoodSecondaryVertexPair(StHFPair const & pair) const;
  bool isGoodTertiaryVertexPair(StHFPair const & pair) const;
  bool isGoodSecondaryVertexTriplet(StHFTriplet const & triplet) const;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  
  const unsigned int&  eventStatMax()  const { return mEventStatMax; }

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  // -- SETTER for CUTS
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  void setBadRunListFileName(const char* fileName);

  void setCutVzMax(float f);
  void setCutVzVpdVzMax(float f);
  void setCutTriggerWord(UShort_t us);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  void setCutNHitsFitMax(int i);
  void setCutRequireHFT(bool b);
  void setCutNHitsFitnHitsMax(float f);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  void setCutTPCNSigmaPion(float f);
  void setCutPionPt(float min, float max);
  void setCutTOFNSigmaPion(float f);
  void setCutTOFDeltaOneOverBetaPion(float f);
  void setCutPionPtTOF(float min, float max);

  void setCutTPCNSigmaKaon(float f);
  void setCutKaonPt(float min, float max);
  void setCutTOFNSigmaKaon(float f);
  void setCutTOFDeltaOneOverBetaKaon(float f);
  void setCutKaonPtTOF(float min, float max);

  void setCutTPCNSigmaProton(float f);
  void setCutProtonPt(float min, float max);
  void setCutTOFNSigmaProton(float f);
  void setCutTOFDeltaOneOverBetaProton(float f);
  void setCutProtonPtTOF(float min, float max);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
  
  void setCutSecondaryPair(float dcaDaughtersMax, float decayLengthMin, float decayLengthMax, 
			   float cosThetaMin, float massMin, float massMax); 
  
  void setCutTertiaryPair(float dcaDaughtersMax, float decayLengthMin, float decayLengthMax, 
			  float cosThetaMin, float massMin, float massMax); 
  
  void setCutSecondaryTriplet(float dcaDaughters12Max, float dcaDaughters23Max, float dcaDaughters31Max, 
			      float decayLengthMin, float decayLengthMax, 
			      float cosThetaMin, float massMin, float massMax);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  // -- GETTER for single CUTS
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  const float&    cutVzMax()                    const;
  const float&    cutVzVpdVzMax()               const;
  const UShort_t& cutTriggerWord()              const;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  const int&      cutNHitsFitMax()              const;
  const bool&     cutRequireHFT()               const;
  const float&    cutNHitsFitnHitsMax()         const;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  const float&    cutTPCNSigmaPion()            const;
  const float&    cutTOFDeltaOneOverBetaPion()  const;
  const float     cutTOFNSigmaPion()            const;
  const float&    cutPionPtMin()                const;
  const float&    cutPionPtMax()                const;
  const float&    cutPionPtTOFMin()             const;
  const float&    cutPionPtTOFMax()             const;

  const float&    cutTPCNSigmaKaon()            const;
  const float&    cutTOFDeltaOneOverBetaKaon()  const;
  const float     cutTOFNSigmaKaon()            const;
  const float&    cutKaonPtMin()                const;
  const float&    cutKaonPtMax()                const;
  const float&    cutKaonPtTOFMin()             const;
  const float&    cutKaonPtTOFMax()             const;

  const float&    cutTPCNSigmaProton()          const;
  const float&    cutTOFDeltaOneOverBetaProton()const;
  const float     cutTOFNSigmaProton()          const;
  const float&    cutProtonPtMin()              const;
  const float&    cutProtonPtMax()              const;
  const float&    cutProtonPtTOFMin()           const;
  const float&    cutProtonPtTOFMax()           const;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  const float&    cutSecondaryPairDcaDaughtersMax()       const;
  const float&    cutSecondaryPairDecayLengthMin()        const;
  const float&    cutSecondaryPairDecayLengthMax()        const;
  const float&    cutSecondaryPairCosThetaMin()           const;
  const float&    cutSecondaryPairMassMin()               const;
  const float&    cutSecondaryPairMassMax()               const;

  const float&    cutTertiaryPairDcaDaughtersMax()        const;
  const float&    cutTertiaryPairDecayLengthMin()         const;
  const float&    cutTertiaryPairDecayLengthMax()         const;
  const float&    cutTertiaryPairCosThetaMin()            const;
  const float&    cutTertiaryPairMassMin()                const;
  const float&    cutTertiaryPairMassMax()                const;

  const float&    cutSecondaryTripletDcaDaughters12Max()  const;
  const float&    cutSecondaryTripletDcaDaughters23Max()  const;
  const float&    cutSecondaryTripletDcaDaughters31Max()  const;
  const float&    cutSecondaryTripletDecayLengthMin()     const;
  const float&    cutSecondaryTripletDecayLengthMax()     const;
  const float&    cutSecondaryTripletCosThetaMin()        const;
  const float&    cutSecondaryTripletMassMin()            const;
  const float&    cutSecondaryTripletMassMax()            const;

  // -- caluclate beta of track
  const float getTofBeta(StPicoTrack const*) const;

 private:
  
  StHFCuts(StHFCuts const &);       
  StHFCuts& operator=(StHFCuts const &); 

  StThreeVectorF mPrimVtx;      // primary vertex of current event

  unsigned int mEventStatMax;   // number of event cuts
  
  float mTOFResolution;         // TOF resolution = 0.013

  // -- bad run list
  TString          mBadRunListFileName;
  std::vector<int> mVecBadRunList;

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
 
  // -- pions
  float mTPCNSigmaPionMax;
  float mTOFDeltaOneOverBetaPionMax;
  float mPionPtMin;
  float mPionPtMax;
  float mPionPtTOFMin;
  float mPionPtTOFMax;

  // -- kaons
  float mTPCNSigmaKaonMax;
  float mTOFDeltaOneOverBetaKaonMax;
  float mKaonPtMin;
  float mKaonPtMax;
  float mKaonPtTOFMin;
  float mKaonPtTOFMax;

  // -- protons
  float mTPCNSigmaProtonMax;
  float mTOFDeltaOneOverBetaProtonMax;
  float mProtonPtMin;
  float mProtonPtMax;
  float mProtonPtTOFMin;
  float mProtonPtTOFMax;
  
  // ------------------------------------------
  // -- Pair cuts for secondary pair
  // ------------------------------------------
  float mSecondaryPairDcaDaughtersMax;
  float mSecondaryPairDecayLengthMin; 
  float mSecondaryPairDecayLengthMax; 
  float mSecondaryPairCosThetaMin;
  float mSecondaryPairMassMin;
  float mSecondaryPairMassMax;

  // ------------------------------------------
  // -- Pair cuts tertiary pair
  // ------------------------------------------
  float mTertiaryPairDcaDaughtersMax;
  float mTertiaryPairDecayLengthMin; 
  float mTertiaryPairDecayLengthMax; 
  float mTertiaryPairCosThetaMin;
  float mTertiaryPairMassMin;
  float mTertiaryPairMassMax;

  // ------------------------------------------
  // -- Cuts of secondary triplet
  // ------------------------------------------
  float mSecondaryTripletDcaDaughters12Max;
  float mSecondaryTripletDcaDaughters23Max;
  float mSecondaryTripletDcaDaughters31Max;
  float mSecondaryTripletDecayLengthMin; 
  float mSecondaryTripletDecayLengthMax; 
  float mSecondaryTripletCosThetaMin;
  float mSecondaryTripletMassMin;
  float mSecondaryTripletMassMax;

  ClassDef(StHFCuts,1)
};

inline void StHFCuts::setBadRunListFileName(const char* fileName) { mBadRunListFileName = fileName; }

inline void StHFCuts::setCutVzMax(float f)            { mVzMax            = f; }
inline void StHFCuts::setCutVzVpdVzMax(float f)       { mVzVpdVzMax       = f; }
inline void StHFCuts::setCutTriggerWord(UShort_t us)  { mTriggerWord      = us; }

inline void StHFCuts::setCutNHitsFitMax(int i)        { mNHitsFitMax      = i; }
inline void StHFCuts::setCutRequireHFT(bool b)        { mRequireHFT       = b; }
inline void StHFCuts::setCutNHitsFitnHitsMax(float f) { mNHitsFitnHitsMax = f; }

inline void StHFCuts::setCutTPCNSigmaPion(float f)             { mTPCNSigmaPionMax = f; }
inline void StHFCuts::setCutTOFNSigmaPion(float f)             { mTOFDeltaOneOverBetaPionMax = f*mTOFResolution;}
inline void StHFCuts::setCutTOFDeltaOneOverBetaPion(float f)   { mTOFDeltaOneOverBetaPionMax = f;}
inline void StHFCuts::setCutPionPt(float min, float max)       { mPionPtMin  = min; mPionPtMax  = max; }
inline void StHFCuts::setCutPionPtTOF(float min, float max)    { mPionPtTOFMin = min; mPionPtTOFMax = max; }
inline void StHFCuts::setCutTPCNSigmaKaon(float f)             { mTPCNSigmaKaonMax = f; }
inline void StHFCuts::setCutTOFNSigmaKaon(float f)             { mTOFDeltaOneOverBetaKaonMax = f*mTOFResolution;}
inline void StHFCuts::setCutTOFDeltaOneOverBetaKaon(float f)   { mTOFDeltaOneOverBetaKaonMax = f;}
inline void StHFCuts::setCutKaonPt(float min, float max)       { mKaonPtMin  = min; mKaonPtMax  = max; }
inline void StHFCuts::setCutKaonPtTOF(float min, float max)    { mKaonPtTOFMin = min; mKaonPtTOFMax = max; }
inline void StHFCuts::setCutTPCNSigmaProton(float f)           { mTPCNSigmaProtonMax = f; }
inline void StHFCuts::setCutTOFNSigmaProton(float f)           { mTOFDeltaOneOverBetaProtonMax = f*mTOFResolution;}
inline void StHFCuts::setCutTOFDeltaOneOverBetaProton(float f) { mTOFDeltaOneOverBetaProtonMax = f;}
inline void StHFCuts::setCutProtonPt(float min, float max)     { mProtonPtMin  = min; mProtonPtMax  = max; }
inline void StHFCuts::setCutProtonPtTOF(float min, float max)  { mProtonPtTOFMin = min; mProtonPtTOFMax = max; }

inline void StHFCuts::setCutSecondaryPair(float dcaDaughtersMax, float decayLengthMin, float decayLengthMax, 
					  float cosThetaMin, float massMin, float massMax)  {
  mSecondaryPairDcaDaughtersMax = dcaDaughtersMax;
  mSecondaryPairDecayLengthMin = decayLengthMin; mSecondaryPairDecayLengthMax = decayLengthMax;
  mSecondaryPairCosThetaMin = cosThetaMin;
  mSecondaryPairMassMin = massMin; mSecondaryPairMassMax = massMax; }

inline void StHFCuts::setCutTertiaryPair(float dcaDaughtersMax, float decayLengthMin, float decayLengthMax, 
					 float cosThetaMin, float massMin, float massMax)  {
  mTertiaryPairDcaDaughtersMax = dcaDaughtersMax;
  mTertiaryPairDecayLengthMin = decayLengthMin; mTertiaryPairDecayLengthMax = decayLengthMax;
  mTertiaryPairCosThetaMin = cosThetaMin;
  mTertiaryPairMassMin = massMin; mTertiaryPairMassMax = massMax; }
  
inline void StHFCuts::setCutSecondaryTriplet(float dcaDaughters12Max, float dcaDaughters23Max, float dcaDaughters31Max, 
					     float decayLengthMin, float decayLengthMax, 
					     float cosThetaMin, float massMin, float massMax)  {
  mSecondaryTripletDcaDaughters12Max = dcaDaughters12Max; mSecondaryTripletDcaDaughters23Max = dcaDaughters23Max; 
  mSecondaryTripletDcaDaughters31Max = dcaDaughters31Max; 
  mSecondaryTripletDecayLengthMin = decayLengthMin; mSecondaryTripletDecayLengthMax = decayLengthMax; 
  mSecondaryTripletCosThetaMin = cosThetaMin;
  mSecondaryTripletMassMin = massMin; mSecondaryTripletMassMax = massMax; }


inline const float&    StHFCuts::cutVzMax()                    const { return mVzMax; }
inline const float&    StHFCuts::cutVzVpdVzMax()               const { return mVzVpdVzMax; }
inline const UShort_t& StHFCuts::cutTriggerWord()              const { return mTriggerWord; }

inline const int&      StHFCuts::cutNHitsFitMax()              const { return mNHitsFitMax; }
inline const bool&     StHFCuts::cutRequireHFT()               const { return mRequireHFT; }
inline const float&    StHFCuts::cutNHitsFitnHitsMax()         const { return mNHitsFitnHitsMax; }

inline const float&    StHFCuts::cutTPCNSigmaPion()            const { return mTPCNSigmaPionMax; }
inline const float&    StHFCuts::cutTOFDeltaOneOverBetaPion()  const { return mTOFDeltaOneOverBetaPionMax; }
inline const float     StHFCuts::cutTOFNSigmaPion()            const { return mTOFDeltaOneOverBetaPionMax/mTOFResolution; }
inline const float&    StHFCuts::cutPionPtMin()                const { return mPionPtMin; }
inline const float&    StHFCuts::cutPionPtMax()                const { return mPionPtMax; }
inline const float&    StHFCuts::cutPionPtTOFMin()             const { return mPionPtTOFMin; }
inline const float&    StHFCuts::cutPionPtTOFMax()             const { return mPionPtTOFMax; }

inline const float&    StHFCuts::cutTPCNSigmaKaon()            const { return mTPCNSigmaKaonMax; }
inline const float&    StHFCuts::cutTOFDeltaOneOverBetaKaon()  const { return mTOFDeltaOneOverBetaKaonMax; }
inline const float     StHFCuts::cutTOFNSigmaKaon()            const { return mTOFDeltaOneOverBetaKaonMax/mTOFResolution; }
inline const float&    StHFCuts::cutKaonPtMin()                const { return mKaonPtMin; }
inline const float&    StHFCuts::cutKaonPtMax()                const { return mKaonPtMax; }
inline const float&    StHFCuts::cutKaonPtTOFMin()             const { return mKaonPtTOFMin; }
inline const float&    StHFCuts::cutKaonPtTOFMax()             const { return mKaonPtTOFMax; }

inline const float&    StHFCuts::cutTPCNSigmaProton()          const { return mTPCNSigmaProtonMax; }
inline const float&    StHFCuts::cutTOFDeltaOneOverBetaProton()const { return mTOFDeltaOneOverBetaProtonMax; }
inline const float     StHFCuts::cutTOFNSigmaProton()          const { return mTOFDeltaOneOverBetaProtonMax/mTOFResolution; }
inline const float&    StHFCuts::cutProtonPtMin()              const { return mProtonPtMin; }
inline const float&    StHFCuts::cutProtonPtMax()              const { return mProtonPtMax; }
inline const float&    StHFCuts::cutProtonPtTOFMin()           const { return mProtonPtTOFMin; }
inline const float&    StHFCuts::cutProtonPtTOFMax()           const { return mProtonPtTOFMax; }

inline const float&    StHFCuts::cutSecondaryPairDcaDaughtersMax()       const { return mSecondaryPairDcaDaughtersMax; }
inline const float&    StHFCuts::cutSecondaryPairDecayLengthMin()        const { return mSecondaryPairDecayLengthMin; }
inline const float&    StHFCuts::cutSecondaryPairDecayLengthMax()        const { return mSecondaryPairDecayLengthMax; }
inline const float&    StHFCuts::cutSecondaryPairCosThetaMin()           const { return mSecondaryPairCosThetaMin; }
inline const float&    StHFCuts::cutSecondaryPairMassMin()               const { return mSecondaryPairMassMin; }
inline const float&    StHFCuts::cutSecondaryPairMassMax()               const { return mSecondaryPairMassMax; }
inline const float&    StHFCuts::cutTertiaryPairDcaDaughtersMax()        const { return mTertiaryPairDcaDaughtersMax; }
inline const float&    StHFCuts::cutTertiaryPairDecayLengthMin()         const { return mTertiaryPairDecayLengthMin; }
inline const float&    StHFCuts::cutTertiaryPairDecayLengthMax()         const { return mTertiaryPairDecayLengthMax; }
inline const float&    StHFCuts::cutTertiaryPairCosThetaMin()            const { return mTertiaryPairCosThetaMin; }
inline const float&    StHFCuts::cutTertiaryPairMassMin()                const { return mTertiaryPairMassMin; }
inline const float&    StHFCuts::cutTertiaryPairMassMax()                const { return mTertiaryPairMassMax; }
inline const float&    StHFCuts::cutSecondaryTripletDcaDaughters12Max()  const { return mSecondaryTripletDcaDaughters12Max; }
inline const float&    StHFCuts::cutSecondaryTripletDcaDaughters23Max()  const { return mSecondaryTripletDcaDaughters23Max; }
inline const float&    StHFCuts::cutSecondaryTripletDcaDaughters31Max()  const { return mSecondaryTripletDcaDaughters31Max; }
inline const float&    StHFCuts::cutSecondaryTripletDecayLengthMin()     const { return mSecondaryTripletDecayLengthMin; }
inline const float&    StHFCuts::cutSecondaryTripletDecayLengthMax()     const { return mSecondaryTripletDecayLengthMax; }
inline const float&    StHFCuts::cutSecondaryTripletCosThetaMin()        const { return mSecondaryTripletCosThetaMin; }
inline const float&    StHFCuts::cutSecondaryTripletMassMin()            const { return mSecondaryTripletMassMin; }
inline const float&    StHFCuts::cutSecondaryTripletMassMax()            const { return mSecondaryTripletMassMax; }

#endif
#endif
