#ifndef StPicoHFEventMaker_h
#define StPicoHFEventMaker_h

#include "StMaker.h"

// -----------------------------------------------
//   Base class for HF analysis
//   - Run 14 - 200 GeV AuAu 
// -----------------------------------------------

class TTree;
class TFile;
class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoHFEvent;
class StKaonPion;

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

    virtual bool  isGoodPair(StKaonPion const &) const;
    virtual bool  isGoodTriple(StKaonPion const &) const;
   
  private:
    Int_t Init();
    Int_t Make();
    Int_t Finish();

    Int_t Reset();

    bool  isGoodEvent();
    bool  isGoodTrack(StPicoTrack const*) const;
 
    float getTofBeta(StPicoTrack const*) const;

    StPicoDstMaker* mPicoDstMaker;
    StPicoDst*      mPicoDst;
    StPicoEvent*    mPicoEvent;
    
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
