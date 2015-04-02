#ifndef StPicoHFMaker_h
#define StPicoHFMaker_h

#include "StMaker.h"
#include "StLorentzVectorF.hh"

/* **************************************************
 *  Base class for HF analysis
 *
 *  Usage:
 *  - Implement in daughter class
 *     InitHF()
 *     MakeHF()
 *     ClearHF()
 *     FinishHF()
 *  - Do not ovewrite Init, Make, Clear, Finish which are inhertited from StMaker
 *  - Set StHFCuts class via setHFBaseCuts(...) in run macro
 *
 *  - Set decay mode of analysis via setDecayMode(...)
 *     use enum of StPicoHFEvent::eHFEventMode (see there for more info)
 *      StPicoHFEvent::kTwoParticleDecay, 
 *      StPicoHFEvent::kThreeParticleDecay
 *      StPicoHFEvent::kTwoAndTwoParticleDecay 
 *
 *  - Set use mode of StPicoHFMaker class  via setMakerMode(...)
 *     use enum of StPicoHFMaker::eMakerMode 
 *      StPicoHFMaker::kAnalyse - don't write candidate trees, just fill histograms
 *      StPicoHFMaker::kWrite   - write candidate trees
 *      StPicoHFMaker::kRead    - read candidate trees and fill histograms
 *
 *  - Implement in daughter class, methods from StHFCuts utility class can/should be used
 *     methods are used to fill vectors for 'good' identified particles
 *     isPion
 *     isKaon
 *     isProton
 *
 * **************************************************
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)   
 *
 * **************************************************
 */

class TTree;
class TFile;
class TChain;

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoHFEvent;
class StHFPair;
class StHFTriplet;
class StHFCuts;

class StPicoHFMaker : public StMaker 
{
  public:
    StPicoHFMaker(char const* name, StPicoDstMaker* picoMaker, char const* outputBaseFileName,  
		       char const* inputHFListHFtree);
    virtual ~StPicoHFMaker();
    
    // -- TO BE IMPLEMENTED BY DAUGHTER CLASS
    virtual Int_t InitHF()                  { return kStOK; }
    virtual Int_t MakeHF()                  { return kStOK; }
    virtual void  ClearHF(Option_t *opt="") { return; }
    virtual Int_t FinishHF()                { return kStOK; }

    void setHFBaseCuts(StHFCuts* cuts);
    void setMakerMode(unsigned short us);
    void setDecayMode(unsigned short us);

    // -- different modes to use the StPicoHFMaker class
    //    - kAnalyse - don't write candidate trees, just fill histograms
    //    - kWrite   - write candidate trees
    //    - kRead    - read candidate trees and fill histograms
    enum eMakerMode {kAnalyse, kWrite, kRead};

    // -- TO BE IMPLEMENTED BY DAUGHTER CLASS
    virtual bool  isPion(StPicoTrack const*, float const & bTofBeta) const   { return true; }
    virtual bool  isKaon(StPicoTrack const*, float const & bTofBeta) const   { return true; }
    virtual bool  isProton(StPicoTrack const*, float const & bTofBeta) const { return true; }

  protected:

    void  createTertiaryK0Shorts();

    unsigned int isDecayMode();
    unsigned int isMakerMode();

    float getTofBeta(StPicoTrack const*) const;

    // -- protected members ------------------------

    StPicoDst      *mPicoDst;

    StHFCuts       *mHFCuts;

    StPicoHFEvent  *mPicoHFEvent;

    float           mBField;
    StThreeVectorF  mPrimVtx;

    TList          *mOutList;

    std::vector<unsigned short> mIdxPicoPions;
    std::vector<unsigned short> mIdxPicoKaons;
    std::vector<unsigned short> mIdxPicoProtons;

  private:
    // -- Inhertited from StMaker 
    //    NOT TO BE OVERWRITTEN by daughter class
    //    daughter class should implement xxxHF()
    Int_t Init();
    Int_t Make();
    void  Clear(Option_t *opt="");
    Int_t Finish();
    
    void  resetEvent();
    bool  setupEvent();
    
    void  initializeEventStats();
    void  fillEventStats(int *aEventStat);

    // -- private members ------------------------

    unsigned int    mDecayMode; // use enum of StPicoHFEvent::eHFEventMode
    unsigned int    mMakerMode; // use enum of StPicoEventMaker::eMakerMode

    TString         mOuputFileBaseName; // base name for output files
                                        //   for tree     -> <mOuputFileBaseName>.picoHFtree.root
                                        //   for histList -> <mOuputFileBaseName>.GetName().root

    TString         mInputFileName;     // filename of input list of HF trees (needs to be in the 
                                        // same order as the picoDstList

    StPicoDstMaker* mPicoDstMaker;      // ptr to picoDst maker

    StPicoEvent*    mPicoEvent;         // ptr to picoDstEvent

    TTree*          mTree;              // tree holding "mPicoHFEvent" for writing only

    TChain*         mHFChain;           // chain to read in HF tree
    int             mEventCounter;      // n Processed events in chain

    TFile*          mOutputFileTree;    // ptr to file saving the HFtree
    TFile*          mOutputFileList;    // ptr to file saving the list of histograms
    ClassDef(StPicoHFMaker, 1)
};

inline void StPicoHFMaker::setHFBaseCuts(StHFCuts* cuts)   { mHFCuts = cuts; }
inline void StPicoHFMaker::setMakerMode(unsigned short us) { mMakerMode = us; }
inline void StPicoHFMaker::setDecayMode(unsigned short us) { mDecayMode = us; }

inline unsigned int StPicoHFMaker::isDecayMode()           { return mDecayMode; }
inline unsigned int StPicoHFMaker::isMakerMode()           { return mMakerMode; }
#endif
