#ifndef StPicoMixedEventMaker_h
#define StPicoMixedEventMaker_h

#include "StMaker.h"

/* **************************************************
 *  Base class for Mixed Event cosntructions
 *  
 *  - Usage: Implement specific decay in daughter, i.e. 2 or three body decay
 * 
 *  - Methods from StHFCyts utility class can/should be used
 *
 * **************************************************
 *
 *  Initial Authors:
 *            Michael Lomnitz  (mrlomnitz@lbl.gov)
 *            Mustaga Mustafa  (mmustafa@lbl.gov)
 *
 *  ** Code Maintainer
 * 
 *
 * **************************************************  
 */

class TTree; //Need tod ecide if will be saving TTree, NTuple or histos
class TFile;
class TChain;

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoEventMixer;
class StHFCuts;

class StPicoMixedEventMaker : public StMaker 
{
  public:
    StPicoMixedEventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outputBaseFileName,  
		       char const* inputHFListHFtree);
    virtual ~StPicoMixedEventMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    virtual void  Clear(Option_t *opt="");
    void setHFBaseCuts(StHFCuts *cuts);
    Int_t SetCategories();
    StHFCuts *mHFCuts;
 private:
    int categorize(StPicoDst const *);
    StPicoDst      *mPicoDst;
    StPicoDstMaker* mPicoDstMaker;      
    StPicoEvent*    mPicoEvent;         
    StPicoEventMixer* mPicoEventMixer; //Needs to be generalized to have mixer per category bin

    TString         mOuputFileBaseName; 
    TString         mInputFileName;     

    int             mRunId;
    int             mEventCounter;

    bool LoadEventPlaneCorr(Int_t const run);
                                        
    TTree*          mTree;

    TFile*          mOutputFileTree; 

    ClassDef(StPicoMixedEventMaker, 1)
};
inline void StPicoMixedEventMaker::setHFBaseCuts(StHFCuts *cuts)  {mHFCuts = cuts; }
#endif
