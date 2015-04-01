#ifndef StPicoD0Hists__h
#define StPicoD0Hists__h

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

#include "TObject.h"

class TH1F;
class TH2F;
class TFile;
class TString;
class StPicoPrescales;
class StPicoEvent;
class StPicoD0Event;
class StKaonPion;


class StPicoD0Hists: public TObject
{
  public:
   StPicoD0Hists(TString fileBaseName);
   virtual ~StPicoD0Hists();
   void addEvent(StPicoEvent const &, StPicoD0Event const &,unsigned int const nHftTracks);
   void addKaonPion(StKaonPion const*), bool fillMass;
   void closeFile();

  private:
   StPicoD0Hists(){}

   StPicoPrescales* mPrescales;
   TFile* mOutFile;
   // TH2F* mh2VzVsVzVpd;
   // TH2F* mh2VrVsVz;
   TH1F* mh1TotalEventsInRun;
   TH1F* mh1TotalHftTracksInRun;
   TH1F* mh1TotalGRefMultInRun;
   TH1F* mh1TotalKaonsInRun;
   TH1F* mh1TotalPionsInRun;
   TH1F* mh1TotalD0CandidatesInRun;
   TH2F* mh2NKaonsVsNPions;
   TH2F* mh2KaonDcaVsPt;
   TH2F* mh2PionDcaVsPt;
   // TH2F* mh2CosThetaVsPt;
   TH2F* mh2InvariantMassVsPt;

   ClassDef(StPicoD0Hists, 1)
};
#endif
