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

class TH1F;
class TH2F;
class TFile;
class TString;
class StPicoEvent;
class StPicoTrack;
class StKaonPion;


class StPicoD0Hists
{
  public:
   StPicoD0Hists(TString fileBaseName);
   ~StPicoD0Hists();
   void addEvent(StPicoEvent const & picoEvent);
   void addKaonPion(StKaonPion const*, StPicoTrack const* kaon, StPicoTrack const* pion);
   void closeFile();

  private:
   StPicoD0Hists(){}

   TFile* mOutFile;
   TH2F* mh2VzVsVzVpd;
   TH2F* mh2VrVsVz;
   TH1F* mh1TotalEventsInRun;
   TH1F* mh1TotalHftTracksInRun;
   TH1F* mh1TotalRefMultInRun;
   TH1F* mh1TotalKaonsInRun;
   TH1F* mh1TotalPionsInRun;
   TH1F* mh1TotalD0CandidatesInRun;
   TH2F* mh2NKaonsVsNPions;
   TH2F* mh2KaonDcaVsPt;
   TH2F* mh2PionDcaVsPt;
   TH2F* mh2CosThetaVsPt;
   TH2F* mh2InvariantMassVsPt;

   ClassDef(StPicoD0Hists, 1)
};
#endif
