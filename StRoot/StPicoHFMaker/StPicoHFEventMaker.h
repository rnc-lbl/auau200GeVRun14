#ifndef StPicoHFEventMaker_h
#define StPicoHFEventMaker_h

#include "StMaker.h"
#include "StLorentzVectorF.hh"

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

class StPicoHFEventMaker : public StMaker 
{
  public:
    StPicoHFEventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName);
    virtual ~StPicoHFEventMaker();
    
    virtual Int_t InitHF()   { return kStOK; }
    virtual Int_t MakeHF()   { return kStOK; }
    virtual Int_t FinishHF() { return kStOK; }

    virtual void  Clear(Option_t *opt="");

    void SetHFBaseCuts(StHFCuts* cuts) { mHFCuts = cuts; }

    enum eMakerMode {analyse, write};
    
    void SetMakerMode(unsigned short us) { mMakerMode = us; }
    void SetDecayMode(unsigned short us) { mDecayMode = us; }

  protected:
    virtual bool  isPion(StPicoTrack const*, float const & bTofBeta) const   { return true; }
    virtual bool  isKaon(StPicoTrack const*, float const & bTofBeta) const   { return true; }
    virtual bool  isProton(StPicoTrack const*, float const & bTofBeta) const { return true; }

    void CreateSecondaryK0Short();

    float getTofBeta(StPicoTrack const*) const;

    // -- members ------------------------

    StPicoDst      *mPicoDst;

    StHFCuts       *mHFCuts;

    StPicoHFEvent  *mPicoHFEvent;

    float           mBField;
    StThreeVectorF  mPrimVtx;

    unsigned int    mDecayMode; // 
    unsigned int    mMakerMode; // 

    std::vector<unsigned short> mIdxPicoPions;
    std::vector<unsigned short> mIdxPicoKaons;
    std::vector<unsigned short> mIdxPicoProtons;
   
  private:
    Int_t Init();
    Int_t Make();
    Int_t Finish();
    
    void  Reset();

    bool  isGoodEvent();
    bool  isGoodTrack(StPicoTrack const*) const;
    
    // -- members ------------------------

    StPicoDstMaker* mPicoDstMaker;

    StPicoEvent*    mPicoEvent;

    TFile* mOutputFile;
    TTree* mTree;

    ClassDef(StPicoHFEventMaker, 1)
};

#endif
