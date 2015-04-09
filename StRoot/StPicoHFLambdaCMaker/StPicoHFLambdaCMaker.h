#ifndef StPicoHFLambdaCMaker_h
#define StPicoHFLambdaCMaker_h

#include "StPicoHFMaker/StPicoHFMaker.h"

/* **************************************************
 *  Class for HF LambdaC analysis
 * --------------------------------------------------
 * 
 *  For more info look also in the .h files in StPicoHFMaker/
 *     StPicoHFMaker/StPicoHFMaker.h      <-- Base Class for analysis
 *     StPicoHFMaker/StPicoHFEvent.h      <-- Holds candidates for one event (written to Tree)
 *     StPicoHFMaker/StHFCuts.h           <-- Cuts, can be set in run macro
 *     StPicoHFMaker/StHFPair.h           <-- Holds a pair candidate of a two body decay
 *     StPicoHFMaker/StHFTriplet.h        <-- Holds a triplet of a three body decay
 *
 *  Usage:
 *   - Implement
 *        InitHF()
 *        MakeHF()
 *        ClearHF()
 *        FinishHF()
 *
 *  - Do not ovewrite Init, Make, Clear, Finish which are inhertited from StPicoHFMaker via StMaker 

 *  - Set StHFCuts class via setHFBaseCuts(...) in run macro
 *
 *  - Set use mode of StPicoHFMaker class  via setMakerMode(...)
 *     use enum of StPicoHFMaker::eMakerMode
 *      StPicoHFMaker::kAnalyse - don't write candidate trees, just fill histograms
 *      StPicoHFMaker::kWrite   - write candidate trees
 *      StPicoHFMaker::kRead    - read candidate trees and fill histograms
 *
 *  - Set decay mode of analysis via setDecayMode(...)
 *     use enum of StPicoHFEvent::eHFEventMode (see there for more info)
 *      StPicoHFEvent::kTwoParticleDecay,
 *      StPicoHFEvent::kThreeParticleDecay
 *      StPicoHFEvent::kTwoAndTwoParticleDecay
 *
 *  - Implement these track selection methods used to fill vectors for 'good' identified particles
 *      (methods from StHFCuts utility class can/should be used)
 *       isPion
 *       isKaon
 *       isProton
 *
 *  --------------------------------------------------
 *  Authors:  Xin Dong (xdong@lbl.gov)
 *            Michael Lomnitz (mlomitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder (jmthader@lbl.gov)   
 *
 * **************************************************
 */

class TNtuple;

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoHFEvent;

class StHFPair;
class StHFTriplet;
class StHFCuts;

class StPicoHFLambdaCMaker : public StPicoHFMaker 
{
 public:
  StPicoHFLambdaCMaker(char const* name, StPicoDstMaker* picoMaker, char const* outputBaseFileName,  
		       char const* inputHFListHFtree);
  virtual ~StPicoHFLambdaCMaker();
  
  virtual Int_t InitHF();
  virtual Int_t MakeHF();
  //    virtual Int_t ClearHF();
  virtual Int_t FinishHF();
  
  void setDecayChannel(unsigned int u) { mDecayChannel = u; }

  enum eDecayChannel {kPionKaonProton, kProtonK0short, kPionLambda};

 protected:
  virtual bool isPion(StPicoTrack const*, float const & bTofBeta) const;
  virtual bool isKaon(StPicoTrack const*, float const & bTofBeta) const;
  virtual bool isProton(StPicoTrack const*, float const & bTofBeta) const;
  
 private:
  bool isPion(StPicoTrack const*) const;
  bool isKaon(StPicoTrack const*) const;
  bool isProton(StPicoTrack const*) const;
  
  int createCandidates();
  int analyseCandidates();

  // -- private members --------------------------

  unsigned int mDecayChannel;

  TNtuple*  mNtupleSecondary;
  TNtuple*  mNtupleTertiary;

  ClassDef(StPicoHFLambdaCMaker, 1)
};

#endif
