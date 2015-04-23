#ifndef StHFCUTS_H
#define StHFCUTS_H

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
#include "StBTofUtil/StV0TofCorrection.h"

class StPicoTrack;
class StPicoEvent;
class StPicoDst;

class StHFPair;
class StHFTriplet;

class StHFCuts : public TNamed
{
 public:
  
  StHFCuts();
  StHFCuts(const Char_t *name);
  ~StHFCuts();
  
  void init();

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  bool isGoodEvent(StPicoDst const * const picoDst, int *aEventCuts);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   

  bool isGoodRun(StPicoEvent const *picoEvent) const;

  bool isGoodTrack(StPicoTrack const *trk) const;
   
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  // -- PID
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  
  enum eHFPID {kPion, kKaon, kProton, kHFPIDMax};

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  // -- TOF PID
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  bool isTPCHadron(StPicoTrack const * const trk, int pidFlag) const;

  bool isTPCPion(StPicoTrack const *trk) const;
  bool isTPCKaon(StPicoTrack const *trk) const;
  bool isTPCProton(StPicoTrack const *trk) const;

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  // -- TOF PID
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --   
  bool isTOFHadronPID(StPicoTrack const *trk, float const & tofBeta, int pidFlag) const;
  bool isTOFHadron(StPicoTrack const *trk, float const & tofBeta, int pidFlag) const;
  bool isHybridTOFHadron(StPicoTrack const *trk, float const & tofBeta, int pidFlag) const;

  // -- Is TOF particle in ptot range
  //    if track has no TOF information - return false
  //    use for 
  //      - primary hadrons 
  //      - secondarys from charm decays (as an approximation)
  bool isTOFPion(StPicoTrack const *trk) const;
  bool isTOFKaon(StPicoTrack const *trk) const;
  bool isTOFProton(StPicoTrack const *trk) const;

  bool isTOFPion(StPicoTrack const *trk,   float const & tofBeta) const;
  bool isTOFKaon(StPicoTrack const *trk,   float const & tofBeta) const;
  bool isTOFProton(StPicoTrack const *trk, float const & tofBeta) const;

  // -- Is TOF particle in ptot range
  //    if track has no TOF information - return true
  //    use for 
  //      - primary hadrons 
  //      - secondarys from charm decays (as an approximation)
  bool isHybridTOFPion(StPicoTrack const *trk) const;
  bool isHybridTOFKaon(StPicoTrack const *trk) const;
  bool isHybridTOFProton(StPicoTrack const *trk) const;

  bool isHybridTOFPion(StPicoTrack const *trk,   float const & tofBeta) const;
  bool isHybridTOFKaon(StPicoTrack const *trk,   float const & tofBeta) const;
  bool isHybridTOFProton(StPicoTrack const *trk, float const & tofBeta) const;

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

  void setCutPrimaryDCAtoVtxMax(float f);

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 

  void setCutPtRange(float min, float max, int pidFlag);
  void setCutTPCNSigma(float f, int pidFlag);
  void setCutTOFNSigma(float f, int pidFlag);
  void setCutTOFDeltaOneOverBeta(float f, int pidFlag);
  void setCutPtotRangeTOF(float min, float max, int pidFlag);
  void setCutPtotRangeHybridTOF(float min, float max, int pidFlag);

  void setCutPionPtRange(float min, float max);
  void setCutTPCNSigmaPion(float f);
  void setCutTOFNSigmaPion(float f);
  void setCutTOFDeltaOneOverBetaPion(float f);
  void setCutPionPtotRangeTOF(float min, float max);
  void setCutPionPtotRangeHybridTOF(float min, float max);

  void setCutKaonPtRange(float min, float max);
  void setCutTPCNSigmaKaon(float f);
  void setCutTOFNSigmaKaon(float f);
  void setCutTOFDeltaOneOverBetaKaon(float f);
  void setCutKaonPtotRangeTOF(float min, float max);
  void setCutKaonPtotRangeHybridTOF(float min, float max);

  void setCutProtonPtRange(float min, float max);
  void setCutTPCNSigmaProton(float f);
  void setCutTOFNSigmaProton(float f);
  void setCutTOFDeltaOneOverBetaProton(float f);
  void setCutProtonPtotRangeTOF(float min, float max);
  void setCutProtonPtotRangeHybridTOF(float min, float max);

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

  // -- calculate beta of track -- basic calculation
  float getTofBetaBase(StPicoTrack const* const trk) const;

  // -- calculate beta of track -- for primary particles
  float getTofBeta(StPicoTrack const* const trk) const;

  // -- calculate corrected beta of track -- for secondary particles
  float getTofBeta(StPicoTrack const * const trk, 
		   StLorentzVectorF const & secondaryMother, StThreeVectorF const & secondaryVtx) const; 
  
  // -- calculate corrected beta of track -- for tertiary particles
  float getTofBeta(StPicoTrack const * const trk, 
		   StLorentzVectorF const & secondaryMother, StThreeVectorF const & secondaryVtx, 
		   StLorentzVectorF const & tertiaryMother,  StThreeVectorF const & tertiaryVtx) const;
  
  const float& getHypotheticalMass(int pidFlag)           const;

 private:
  
  StHFCuts(StHFCuts const &);       
  StHFCuts& operator=(StHFCuts const &); 

  StV0TofCorrection* mTOFCorr;  // TOF correction

  StThreeVectorF    mPrimVtx;   // primary vertex of current event
  const StPicoDst*  mPicoDst;   //! ptr to picoDst

  unsigned int mEventStatMax;   // number of event cuts
  
  float        mTOFResolution;  // TOF resolution = 0.013

  // -- bad run list
  TString mBadRunListFileName;
  std::vector<int> mVecBadRunList;

  // -- event cuts
  float mVzMax;
  float mVzVpdVzMax;
  UShort_t mTriggerWord;  // first five bits see http://rnc.lbl.gov/~xdong/SoftHadron/picoDst.html

  // -- tracking
  int   mNHitsFitMax;
  bool  mRequireHFT;
  float mNHitsFitnHitsMax; // applied in StPicoDstMaker for Run14 data, no need to reapply here

  float mPrimaryDCAtoVtxMax;         // used for primary selection for TOF Beta recalculation

  // -- acceptance - per particle type [eHFPID]
  float mPtRange[3][2];

  // -- PID cuts - per particle type [eHFPID]
  float mHypotheticalMass[3];        // hypothetical mass
  float mHypotheticalMass2[3];       // hypothetical mass squared

  float mTPCNSigmaMax[3];
  float mTOFDeltaOneOverBetaMax[3]; 

  float mPtotRangeTOF[3][2];         // momentum range [min,max], where TOF PID is applied
  float mPtotRangeHybridTOF[3][2];   // momentum range [min,max], where Hybrid TOF PID is applied

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

inline void StHFCuts::setCutVzMax(float f)              { mVzMax            = f; }
inline void StHFCuts::setCutVzVpdVzMax(float f)         { mVzVpdVzMax       = f; }
inline void StHFCuts::setCutTriggerWord(UShort_t us)    { mTriggerWord      = us; }

inline void StHFCuts::setCutNHitsFitMax(int i)          { mNHitsFitMax      = i; }
inline void StHFCuts::setCutRequireHFT(bool b)          { mRequireHFT       = b; }
inline void StHFCuts::setCutNHitsFitnHitsMax(float f)   { mNHitsFitnHitsMax = f; }

inline void StHFCuts::setCutPrimaryDCAtoVtxMax(float f) { mPrimaryDCAtoVtxMax = f; }

inline void StHFCuts::setCutPtRange(float min, float max, int pidFlag)            { mPtRange[pidFlag][0] = min; 
                                                                                    mPtRange[pidFlag][1] = max; }
inline void StHFCuts::setCutTPCNSigma(float f, int pidFlag)                       { mTPCNSigmaMax[pidFlag] = f; }
inline void StHFCuts::setCutTOFNSigma(float f, int pidFlag)                       { mTOFDeltaOneOverBetaMax[pidFlag] = f*mTOFResolution;}
inline void StHFCuts::setCutTOFDeltaOneOverBeta(float f, int pidFlag)             { mTOFDeltaOneOverBetaMax[pidFlag] = f;}
inline void StHFCuts::setCutPtotRangeTOF(float min, float max, int pidFlag)       { mPtotRangeTOF[pidFlag][0] = min; 
                                                                                    mPtotRangeTOF[pidFlag][1] = max; }
inline void StHFCuts::setCutPtotRangeHybridTOF(float min, float max, int pidFlag) { mPtotRangeHybridTOF[pidFlag][0] = min; 
                                                                                    mPtotRangeHybridTOF[pidFlag][1] = max; }

inline void StHFCuts::setCutPionPtRange(float min, float max)              { setCutPtRange(min, max, StHFCuts::kPion); }
inline void StHFCuts::setCutTPCNSigmaPion(float f)                         { setCutTPCNSigma(f, StHFCuts::kPion); }
inline void StHFCuts::setCutTOFNSigmaPion(float f)                         { setCutTOFNSigma(f, StHFCuts::kPion); }
inline void StHFCuts::setCutTOFDeltaOneOverBetaPion(float f)               { setCutTOFDeltaOneOverBeta(f, StHFCuts::kPion); }
inline void StHFCuts::setCutPionPtotRangeTOF(float min, float max)         { setCutPtotRangeTOF(min, max, StHFCuts::kPion); }
inline void StHFCuts::setCutPionPtotRangeHybridTOF(float min, float max)   { setCutPtotRangeHybridTOF(min, max, StHFCuts::kPion); }

inline void StHFCuts::setCutKaonPtRange(float min, float max)              { setCutPtRange(min, max, StHFCuts::kKaon); }
inline void StHFCuts::setCutTPCNSigmaKaon(float f)                         { setCutTPCNSigma(f, StHFCuts::kKaon); }
inline void StHFCuts::setCutTOFNSigmaKaon(float f)                         { setCutTOFNSigma(f, StHFCuts::kKaon); }
inline void StHFCuts::setCutTOFDeltaOneOverBetaKaon(float f)               { setCutTOFDeltaOneOverBeta(f, StHFCuts::kKaon); }
inline void StHFCuts::setCutKaonPtotRangeTOF(float min, float max)         { setCutPtotRangeTOF(min, max, StHFCuts::kKaon); }
inline void StHFCuts::setCutKaonPtotRangeHybridTOF(float min, float max)   { setCutPtotRangeHybridTOF(min, max, StHFCuts::kKaon); }

inline void StHFCuts::setCutProtonPtRange(float min, float max)            { setCutPtRange(min, max, StHFCuts::kProton); }
inline void StHFCuts::setCutTPCNSigmaProton(float f)                       { setCutTPCNSigma(f, StHFCuts::kProton); }
inline void StHFCuts::setCutTOFNSigmaProton(float f)                       { setCutTOFNSigma(f, StHFCuts::kProton); }
inline void StHFCuts::setCutTOFDeltaOneOverBetaProton(float f)             { setCutTOFDeltaOneOverBeta(f, StHFCuts::kProton); }
inline void StHFCuts::setCutProtonPtotRangeTOF(float min, float max)       { setCutPtotRangeTOF(min, max, StHFCuts::kProton); }
inline void StHFCuts::setCutProtonPtotRangeHybridTOF(float min, float max) { setCutPtotRangeHybridTOF(min, max, StHFCuts::kProton); }

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

inline const float&    StHFCuts::getHypotheticalMass(int pidFlag)        const { return mHypotheticalMass[pidFlag]; }

// -- check for good hadrons in TPC - in ptRange
inline bool StHFCuts::isTPCPion(StPicoTrack const * const trk)   const {return isTPCHadron(trk, StHFCuts::kPion); }
inline bool StHFCuts::isTPCKaon(StPicoTrack const * const trk)   const {return isTPCHadron(trk, StHFCuts::kKaon); }
inline bool StHFCuts::isTPCProton(StPicoTrack const * const trk) const {return isTPCHadron(trk, StHFCuts::kProton); }

inline bool StHFCuts::isTOFPion(StPicoTrack const *trk)   const { float tofBeta = getTofBeta(trk);  
                                                                  return isTOFHadron(trk, tofBeta, StHFCuts::kPion); }
inline bool StHFCuts::isTOFKaon(StPicoTrack const *trk)   const { float tofBeta = getTofBeta(trk);  
                                                                  return isTOFHadron(trk, tofBeta, StHFCuts::kKaon); }
inline bool StHFCuts::isTOFProton(StPicoTrack const *trk) const { float tofBeta = getTofBeta(trk);  
                                                                  return isTOFHadron(trk, tofBeta, StHFCuts::kProton); }

inline bool StHFCuts::isTOFPion(StPicoTrack const *trk, float const & tofBeta)   const { return isTOFHadron(trk, tofBeta, StHFCuts::kPion); }
inline bool StHFCuts::isTOFKaon(StPicoTrack const *trk, float const & tofBeta)   const { return isTOFHadron(trk, tofBeta, StHFCuts::kKaon); }
inline bool StHFCuts::isTOFProton(StPicoTrack const *trk, float const & tofBeta) const { return isTOFHadron(trk, tofBeta, StHFCuts::kProton); }

inline bool StHFCuts::isHybridTOFPion(StPicoTrack const *trk)   const { float tofBeta = getTofBeta(trk);  
                                                                        return isHybridTOFHadron(trk, tofBeta, StHFCuts::kPion); }
inline bool StHFCuts::isHybridTOFKaon(StPicoTrack const *trk)   const { float tofBeta = getTofBeta(trk);  
                                                                        return isHybridTOFHadron(trk, tofBeta, StHFCuts::kKaon); }
inline bool StHFCuts::isHybridTOFProton(StPicoTrack const *trk) const { float tofBeta = getTofBeta(trk);  
                                                                        return isHybridTOFHadron(trk, tofBeta, StHFCuts::kProton); }

inline bool StHFCuts::isHybridTOFPion(StPicoTrack const *trk, float const & tofBeta)   const { return isHybridTOFHadron(trk, tofBeta, StHFCuts::kPion); }
inline bool StHFCuts::isHybridTOFKaon(StPicoTrack const *trk, float const & tofBeta)   const { return isHybridTOFHadron(trk, tofBeta, StHFCuts::kKaon); }
inline bool StHFCuts::isHybridTOFProton(StPicoTrack const *trk, float const & tofBeta) const { return isHybridTOFHadron(trk, tofBeta, StHFCuts::kProton); }

#endif
