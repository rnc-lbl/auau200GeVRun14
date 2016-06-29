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

#include "TString.h"

#include "StChain/StMaker.h"
#include "StarClassLibrary/StThreeVectorF.hh"

class TTree;
class TFile;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoD0Event;
class StKaonPion;
class StPicoKPiXEvent;
class StPicoKPiX;
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
    
    void  makeD0(bool m=true);
    void  makeKaonPionPion(bool m=true);
    void  makeKaonPionKaon(bool m=true);
    void  makeKaonPionProton(bool m=true);

  private:
    bool  isGoodEvent() const;
    bool  isGoodTrigger() const;
    bool  isGoodTrack(StPicoTrack const&, StThreeVectorF const& pVtx) const;
    bool  isPion(StPicoTrack const&) const;
    bool  isKaon(StPicoTrack const&) const;
    bool  isProton(StPicoTrack const&) const;
    bool  isGoodD0Pair(StKaonPion const&) const;
    bool  isGoodKPiX(StPicoKPiX const&) const;
    bool  isGoodD0Mass(StKaonPion const&) const;
    bool  isGoodKPiXMass(double mass) const;
    int   getD0PtIndex(StKaonPion const& kp) const;
    bool  isGoodQaPair(StKaonPion const&, StPicoTrack const&,StPicoTrack const&);

    StPicoDstMaker*  mPicoDstMaker;
    StPicoEvent*     mPicoEvent;
    StPicoD0QaHists* mPicoD0Hists;

    TString mBaseName;

    TFile* mD0File;
    TTree* mD0Tree;
    StPicoD0Event*   mPicoD0Event;

    TFile* mKPiXFile;
    TTree* mKPiXTree;
    StPicoKPiXEvent* mPicoKPiXEvent;

    bool mMakeD0 = true;
    bool mMakeKaonPionPion = true;
    bool mMakeKaonPionKaon = true;
    bool mMakeKaonPionProton = true;

    ClassDef(StPicoCharmMaker, 0)
};
inline void StPicoCharmMaker::makeD0(bool m)             { mMakeD0 = m; }
inline void StPicoCharmMaker::makeKaonPionPion(bool m)   { mMakeKaonPionPion = m; }
inline void StPicoCharmMaker::makeKaonPionKaon(bool m)   { mMakeKaonPionKaon = m; }
inline void StPicoCharmMaker::makeKaonPionProton(bool m) { mMakeKaonPionProton = m; }
#endif
