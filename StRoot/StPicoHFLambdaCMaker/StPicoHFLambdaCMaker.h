#ifndef StPicoHFLambdaCMaker_h
#define StPicoHFLambdaCMaker_h

#include "../StPicoHFMaker/StPicoHFEventMaker.h"

/* **************************************************
 *  Base class for HF analysis
 *
 *  Usage:
 *   Implement in inherited class
 *    InitHF()
 *    MakeHF()
 *
 *   To be implemented in derived class
 *       methods can be StHFCuts utility class
 *    isPion
 *    isKaon
 *    isProton
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
    virtual Int_t FinishHF();

  protected:
    virtual bool  isPion(StPicoTrack const*, float const & bTofBeta) const;
    virtual bool  isKaon(StPicoTrack const*, float const & bTofBeta) const;
    virtual bool  isProton(StPicoTrack const*, float const & bTofBeta) const;
   
  private:
    /* virtual Int_t Init(); */
    /* virtual Int_t Make(); */
    /* virtual Int_t Finish(); */

    
    ClassDef(StPicoHFLambdaCMaker, 1)
};

#endif
