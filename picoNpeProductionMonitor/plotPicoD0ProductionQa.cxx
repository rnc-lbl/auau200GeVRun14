/* **************************************************
 *  A class to create and save Npe production QA
 *  histograms.
 *
 *  Authors:  **Kunsu OH        (kunsuoh@gmail.com)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  **Code Maintainer
 * **************************************************
 */


#ifndef __CINT__
#include <iostream>
#include <string>

#include "TFile.h"

#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TColor.h"
#include "Rtypes.h"
#endif

using namespace std;

void setGraphicsStyle();
void drawQaHistograms(string filename);
void setStyle(TH1F* h,Color_t, string ytitle);

int main(int argc,char** argv)
{
  string filename = argv[1];
  drawQaHistograms(filename);
  return 0;
}

void drawQaHistograms(string filename)
{
  TFile* fInput = new TFile(filename.c_str());
  TH1F* h1TotalEventsInRun         = (TH1F*)fInput->Get("mh1TotalEventsInRun");
  TH1F* h1AverageHftTracksInRun    = (TH1F*)fInput->Get("mh1TotalHftTracksInRun")->Clone("h1AverageHftTracksInRun");
  TH1F* h1AverageGRefMultInRun     = (TH1F*)fInput->Get("mh1TotalGRefMultInRun")->Clone("h1AverageGRefMultInRun");
  TH1F* h1AverageKaonsInRun        = (TH1F*)fInput->Get("mh1TotalKaonsInRun")->Clone("h1AverageKaonsInRun");
  TH1F* h1AveragePionsInRun        = (TH1F*)fInput->Get("mh1TotalPionsInRun")->Clone("h1AveragePionsInRun");
  TH1F* h1AverageNpeCandidatesInRun = (TH1F*)fInput->Get("mh1TotalNpeCandidatesInRun")->Clone("h1AverageNpeCandidatesInRun");
  TH2F* h2NKaonsVsNPions           = (TH2F*)fInput->Get("mh2NKaonsVsNPions");
  TH2F* h2KaonDcaVsPt              = (TH2F*)fInput->Get("mh2KaonDcaVsPt");
  TH2F* h2PionDcaVsPt              = (TH2F*)fInput->Get("mh2PionDcaVsPt");
  TH2F* h2CosThetaVsPt             = (TH2F*)fInput->Get("mh2CosThetaVsPt");
  TH2F* h2DcaDaughtersVsPt             = (TH2F*)fInput->Get("mh2DcaDaughtersVsPt");
  TH2F* h2InvariantMassVsPt        = (TH2F*)fInput->Get("mh2InvariantMassVsPt");

  h1AverageHftTracksInRun->Divide(h1TotalEventsInRun);
  h1AverageGRefMultInRun->Divide(h1TotalEventsInRun);
  h1AverageKaonsInRun->Divide(h1TotalEventsInRun);
  h1AveragePionsInRun->Divide(h1TotalEventsInRun);
  h1AverageNpeCandidatesInRun->Divide(h1TotalEventsInRun);

  setStyle(h1AverageHftTracksInRun,1,"<HftMatchTrack>");
  setStyle(h1AverageGRefMultInRun,1,"<gRefMult>");
  setStyle(h1AverageKaonsInRun,1,"<nKaons>");
  setStyle(h1AveragePionsInRun,1,"<nPions>");
  setStyle(h1AverageNpeCandidatesInRun,1,"<K#pi pair>");

  setGraphicsStyle();
  TCanvas* cv = new TCanvas("cv","cv",800,450);
  h1AverageHftTracksInRun->Draw(); cv->SaveAs("h1AverageHftTracksInRun.png");
  h1AverageGRefMultInRun->Draw(); cv->SaveAs("h1AverageGRefMultInRun.png");
  h1AverageKaonsInRun->Draw(); cv->SaveAs("h1AverageKaonsInRun.png");
  h1AveragePionsInRun->Draw(); cv->SaveAs("h1AveragePionsInRun.png");
  h1AverageNpeCandidatesInRun->Draw(); cv->SaveAs("h1AverageNpeCandidatesInRun.png");
  h2InvariantMassVsPt->Draw("colz"); cv->SaveAs("h2InvariantMassVsPt.png");
  cv->SetLogz();
  h2NKaonsVsNPions->Draw("colz"); cv->SaveAs("h2NKaonsVsNPions.png");
  h2KaonDcaVsPt->Draw("colz"); cv->SaveAs("h2KaonDcaVsPt.png");
  h2PionDcaVsPt->Draw("colz"); cv->SaveAs("h2PionDcaVsPt.png");
  h2PionDcaVsPt->Draw("colz"); cv->SaveAs("h2PionDcaVsPt.png");
  h2CosThetaVsPt->Draw("colz"); cv->SaveAs("h2CosThetaVsPt.png");
  h2DcaDaughtersVsPt->Draw("colz"); cv->SaveAs("h2DcaDaughtersVsPt.png");
  delete cv;

  fInput->Close();
}

void setStyle(TH1F* h,Color_t color,string title)
{
  h->GetYaxis()->SetTitle(title.c_str());
  h->SetMarkerStyle(20);
  h->SetLineColor(color);
  h->SetMarkerColor(color);
}

void setGraphicsStyle()
{
    // **************************** Set graphic style ***************************************
    gStyle->SetEndErrorSize(5);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(111);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetPalette(1);
    gStyle->SetCanvasColor(10);
    gStyle->SetFrameFillColor(10);
    gStyle->SetFillColor(4);
    TGaxis::SetMaxDigits(4);
    gStyle->SetPadTopMargin(0.055);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadRightMargin(0.115);
    gStyle->SetPadLeftMargin(0.115);
    gStyle->SetLabelSize(0.035,"X");
    gStyle->SetLabelSize(0.035,"Y");
    gStyle->SetTitleSize(0.035,"X");
    gStyle->SetTitleSize(0.035,"Y");
    gStyle->SetTitleOffset(0.6,"X");
    gStyle->SetTitleOffset(0.6,"Y");
    gStyle->SetTitleFontSize(0.1);
    gStyle->SetTitleFont(132, "X");
    gStyle->SetTitleFont(132, "Y");
    gStyle->SetTitleFont(132, "Z");
    gStyle->SetTitleOffset(1.2, "Y");
    gStyle->SetStatFont(132);
    gStyle->SetStatFontSize(0.035);
    gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.87);

    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t reds[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t greens[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blues[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    Int_t  FI = TColor::CreateGradientColorTable(NRGBs, stops, reds, greens, blues, NCont);
    gStyle->SetNumberContours(NCont);
    gStyle->UseCurrentStyle();
    // **************************************************************************************
}

