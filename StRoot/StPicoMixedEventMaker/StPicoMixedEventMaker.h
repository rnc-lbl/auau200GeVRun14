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
class StRefMultCorr;

class StPicoEventMixer;

class StPicoMixedEventMaker : public StMaker 
{
  public:
  StPicoMixedEventMaker(char const* name, StPicoDstMaker* picoMaker, StRefMultCorr* grefmultCorrUtil,
			char const* outputBaseFileName,  
			char const* inputHFListHFtree);
    virtual ~StPicoMixedEventMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    virtual void  Clear(Option_t* opt="");

    Int_t SetCategories();

 private:
    int categorize(StPicoDst const*);
    StPicoDst*      mPicoDst;
    StPicoDstMaker* mPicoDstMaker;      
    StPicoEvent*    mPicoEvent;         
    StRefMultCorr* mGRefMultCorrUtil;

    StPicoEventMixer* mPicoEventMixer[10][9]; //Needs to be generalized, have vz and centrality

    TString         mOuputFileBaseName; 
    TString         mInputFileName;     

    int             mEventCounter;

    bool loadEventPlaneCorr(int const runId);
                                        
    TTree*          mTree;
    TFile*          mOutputFileTree; 

    ClassDef(StPicoMixedEventMaker, 0)
};
#endif
