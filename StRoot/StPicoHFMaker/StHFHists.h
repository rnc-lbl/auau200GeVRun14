#ifndef StHFHists__h
#define StHFHists__h

/* **************************************************
 *  A class to create and save D0 production QA
 *  histograms.
 *
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)   
 *
 * **************************************************
 */

#include "TNamed.h"
#include "TList.h"
#include  "../StPicoPrescales/StPicoPrescales.h"

class TH1F;
class TH2F;
class TFile;
class TString;
class StPicoPrescales;

class StPicoEvent;
class StPicoHFEvent;

class StHFPair;
class StHFTriplet;


class StHFHists: public TNamed
{
 public:
  StHFHists();
  StHFHists(const char* name);

  virtual ~StHFHists();

  void init(TList *outList);
  void fillEventHists(StPicoEvent const &, StPicoHFEvent const &);
  //  void fillEventHists(StPicoEvent const &, StPicoHFEvent const &, unsigned int const nHftTracks);
 
  
 private:
  
  TList *mEventList;
  // TList *mPairList;
  // TList *mTripletList;

  // general event hists
  StPicoPrescales* mPrescales;
 
  int mNRuns;

 
   ClassDef(StHFHists, 1)
};
#endif
