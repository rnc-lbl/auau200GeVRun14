#ifndef StPicoD0EventMaker_h
#define StPicoD0EventMaker_h

#include "StMaker.h"

class TTree;
class TFile;
class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoD0Event;

class StPicoD0EventMaker : public StMaker 
{
  public:
    StPicoD0EventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName);
    virtual ~StPicoD0EventMaker();
    
    virtual Int_t Init();
    virtual Int_t Make();
    virtual void  Clear(Option_t *opt="");
    virtual Int_t Finish();
    
  private:
    bool  isGoodEvent();
    bool  isGoodTrack(StPicoTrack*);
    bool  isPion(StPicoTrack*);
    bool  isKaon(StPicoTrack*);

    StPicoDstMaker* mPicoDstMaker;
    StPicoDst*      mPicoDst;
    StPicoEvent*    mPicoEvent;
    
    TFile* mOutputFile;
    TTree* mTree;
    StPicoD0Event* mPicoD0Event;

    ClassDef(StPicoD0EventMaker, 1)
};

#endif
