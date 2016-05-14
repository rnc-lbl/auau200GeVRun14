#ifndef StPicoCharmMaker_h
#define StPicoCharmMaker_h

/* **************************************************
 *  A Maker that reads StPicoEvents' and creates 
 *  StPicoD0Events and StPicoKPiXEvents
 *
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            **Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  **Code Maintainer
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
class StPicoD0QaHists;

class StPicoCharmMaker : public StMaker 
{
  public:
    StPicoCharmMaker(char const* makerName, StPicoDstMaker* picoMaker, char const* fileBaseName);
    virtual ~StPicoCharmMaker();
    
    virtual Int_t Init();
    virtual Int_t Make();
    virtual void  Clear(Option_t *opt="");
    virtual Int_t Finish();
    
  private:
    bool  isGoodEvent() const;
    bool  isGoodTrigger() const;
    bool  isGoodTrack(StPicoTrack const*) const;
    bool  isPion(StPicoTrack const*) const;
    bool  isKaon(StPicoTrack const*) const;
    bool  isGoodPair(StKaonPion const&) const;
    bool  isGoodMass(StKaonPion const&) const;
    int   getD0PtIndex(StKaonPion const& kp) const;
    bool  isGoodQaPair(StKaonPion const&, StPicoTrack const&,StPicoTrack const&);

    StPicoDstMaker* mPicoDstMaker;
    StPicoEvent*    mPicoEvent;
    StPicoD0QaHists*  mPicoD0Hists;

    TFile* mD0File;
    TTree* mD0Tree;
    StPicoD0Event* mPicoD0Event;

    ClassDef(StPicoCharmMaker, 0)
};
#endif
