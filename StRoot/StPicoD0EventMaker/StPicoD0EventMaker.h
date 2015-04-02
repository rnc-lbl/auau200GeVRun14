#ifndef StPicoD0EventMaker_h
#define StPicoD0EventMaker_h

/* **************************************************
 *  A Maker that reads StPicoEvents' and creates 
 *  StPicoD0Events and stores them.
 *
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)   
 *
 * **************************************************
 */

#include "StMaker.h"

class TTree;
class TFile;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoD0Event;
class StKaonPion;
class StPicoD0Hists;

class StPicoD0EventMaker : public StMaker 
{
  public:
    StPicoD0EventMaker(char const* makerName, StPicoDstMaker* picoMaker, char const* fileBaseName);
    virtual ~StPicoD0EventMaker();
    
    virtual Int_t Init();
    virtual Int_t Make();
    virtual void  Clear(Option_t *opt="");
    virtual Int_t Finish();
    
  private:
    bool  isGoodEvent();
    bool  isGoodTrack(StPicoTrack const*) const;
    bool  isPion(StPicoTrack const*) const;
    bool  isKaon(StPicoTrack const*) const;
    bool  isGoodPair(StKaonPion const &) const;
    bool  isGoodQaPair(StKaonPion const&, StPicoTrack const&,StPicoTrack const&);

    StPicoDstMaker* mPicoDstMaker;
    StPicoEvent*    mPicoEvent;
    StPicoD0Hists*  mPicoD0Hists;
    
    TFile* mOutputFile;
    TTree* mTree;
    StPicoD0Event* mPicoD0Event;

    ClassDef(StPicoD0EventMaker, 1)
};

#endif
