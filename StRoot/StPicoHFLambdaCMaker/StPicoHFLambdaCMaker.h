#ifndef StPicoHFLambdaCMaker_h
#define StPicoHFLambdaCMaker_h

#include "StPicoHFMaker/StPicoHFMaker.h"

/* **************************************************
 *  Class for HF LambdaC analysis
 *
 *  Authors:  Xin Dong (xdong@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder (jmthader@lbl.gov)   
 *            Michael Lomnitz (mlomitz@lbl.gov)
 *
 * **************************************************
 */

class TTree;
class TFile;
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
  int readCandidates();
  int analyseCandidates();

  // -- private members --------------------------

  unsigned int mDecayChannel;

  TNtuple*  mNtupleSecondary;
  TNtuple*  mNtupleTertiary;

  ClassDef(StPicoHFLambdaCMaker, 1)
};

#endif
