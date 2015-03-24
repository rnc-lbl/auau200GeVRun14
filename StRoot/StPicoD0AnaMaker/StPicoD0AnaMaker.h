#ifndef StPicoD0AnaMaker_h
#define StPicoD0AnaMaker_h

#include "TChain.h"
#include "StMaker.h"

class TString;
class TFile;
class TNtuple;
class StPicoD0Event;
class StPicoDstMaker;

class StPicoD0AnaMaker : public StMaker
{
  public:
    StPicoD0AnaMaker(char const * name, char const * inputFilesList, char const * outName,StPicoDstMaker* picoDstMaker);
    virtual ~StPicoD0AnaMaker();

    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();

    int getEntries() const;
    
  private:
    StPicoD0AnaMaker() {}

    StPicoDstMaker* mPicoDstMaker;
    StPicoD0Event* mPicoD0Event;

    TString mOutFileName;
    TString mInputFileList;
    TFile* mOutputFile;
    TChain* mChain;
    TNtuple* mNtuple;
    int mEventCounter;

    ClassDef(StPicoD0AnaMaker, 1)
};

inline int StPicoD0AnaMaker::getEntries() const 
{
  return mChain? mChain->GetEntriesFast() : 0;
}
#endif
