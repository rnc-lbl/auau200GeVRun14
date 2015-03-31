#ifndef StPicoHFLambdaCMaker_h
#define StPicoHFLambdaCMaker_h

#include "../StPicoHFMaker/StPicoHFEventMaker.h"

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
class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoHFEvent;

class StHFPair;
class StHFTriplet;
class StHFCuts;

class StPicoHFLambdaCMaker : public StPicoHFEventMaker 
{
 public:
  StPicoHFLambdaCMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName);
  virtual ~StPicoHFLambdaCMaker();
  
  virtual Int_t InitHF();
  virtual Int_t MakeHF();
  //    virtual Int_t ClearHF();
  virtual Int_t FinishHF();
  
  void setDecayChannel(unsigned int u) { mDecayChannel = u; }

  enum eDecayChannel {kPionKaonProton, kProtonK0short, kPionLambda};

 protected:
  virtual bool  isPion(StPicoTrack const*, float const & bTofBeta) const;
  virtual bool  isKaon(StPicoTrack const*, float const & bTofBeta) const;
  virtual bool  isProton(StPicoTrack const*, float const & bTofBeta) const;
  
 private:
  
  unsigned int mDecayChannel;

  ClassDef(StPicoHFLambdaCMaker, 1)
};

#endif
