#ifndef StPicoHFEventMaker_h
#define StPicoHFEventMaker_h

#include "StMaker.h"

/* **************************************************
 *  Base class for HF analysis
 *
 *  Usage:
 *
 *
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

class StPicoHFEventMaker : public StMaker 
{
  public:
    StPicoHFEventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName);
    virtual ~StPicoHFEventMaker();
    
    virtual Int_t InitHF();
    virtual Int_t MakeHF();
    virtual void  Clear(Option_t *opt="");
    virtual Int_t FinishHF();


  protected:
    virtual bool  isPion(StPicoTrack const*, float const & bTofBeta) const;
    virtual bool  isKaon(StPicoTrack const*, float const & bTofBeta) const;
    virtual bool  isProton(StPicoTrack const*, float const & bTofBeta) const;

    virtual bool  isGoodPair(StHFPair const &) const;
    virtual bool  isGoodTriple(StHFTriplet const &) const;
   
  private:
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();

    void  Reset();

    bool  isGoodEvent();
    bool  isGoodTrack(StPicoTrack const*) const;
 
    float getTofBeta(StPicoTrack const*) const;

    StPicoDstMaker* mPicoDstMaker;
    StPicoDst*      mPicoDst;
    StPicoEvent*    mPicoEvent;

    StHFCuts *mHFCuts;
    
    TFile* mOutputFile;
    TTree* mTree;
    StPicoHFEvent* mPicoHFEvent;

    float mBField;
    StThreeVectorF mPrimVtx;

    std::vector<unsigned short> mIdxPicoKaons;
    std::vector<unsigned short> mIdxPicoPions;
    std::vector<unsigned short> mIdxPicoProtons;


    
    ClassDef(StPicoHFEventMaker, 1)
};

#endif
