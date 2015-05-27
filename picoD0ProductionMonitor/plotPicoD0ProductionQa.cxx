/* **************************************************
 *  A class to create and save D0 production QA
 *  histograms.
 *
 *  Authors:  Mustafa Mustafa (mmustafa@lbl.gov)
 * **************************************************
 */

#ifndef __CINT__
#include <iostream>
#include <string>
#include <utility>

#include "TFile.h"

#include "TF1.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TColor.h"
#include "Rtypes.h"
#include "TMath.h"
#include "TLatex.h"
#endif

using namespace std;

void setGraphicsStyle();
void drawQaHistograms(string filename);
void setStyle(TH1* h,Color_t, string ytitle);
void drawD0InPtBins(TCanvas*, TH2F*,int nEvents);
std::pair<float,float> getSignificance(TF1* f,float integrationSigma,float binWidth);

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
  TH1F* h1AverageD0CandidatesInRun = (TH1F*)fInput->Get("mh1TotalD0CandidatesInRun")->Clone("h1AverageD0CandidatesInRun");
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
  h1AverageD0CandidatesInRun->Divide(h1TotalEventsInRun);

  setStyle(h1AverageHftTracksInRun,1,"<HftMatchTrack>");
  setStyle(h1AverageGRefMultInRun,1,"<gRefMult>");
  setStyle(h1AverageKaonsInRun,1,"<nKaons>");
  setStyle(h1AveragePionsInRun,1,"<nPions>");
  setStyle(h1AverageD0CandidatesInRun,1,"<K#pi pair>");

  setGraphicsStyle();
  TCanvas* cv = new TCanvas("cv","cv",800,450);
  h1AverageHftTracksInRun->Draw(); cv->SaveAs("h1AverageHftTracksInRun.png");
  h1AverageGRefMultInRun->Draw(); cv->SaveAs("h1AverageGRefMultInRun.png");
  h1AverageKaonsInRun->Draw(); cv->SaveAs("h1AverageKaonsInRun.png");
  h1AveragePionsInRun->Draw(); cv->SaveAs("h1AveragePionsInRun.png");
  h1AverageD0CandidatesInRun->Draw(); cv->SaveAs("h1AverageD0CandidatesInRun.png");
  h2InvariantMassVsPt->Draw("colz"); cv->SaveAs("h2InvariantMassVsPt.png");
  cv->SetLogz();
  h2NKaonsVsNPions->Draw("colz"); cv->SaveAs("h2NKaonsVsNPions.png");
  h2KaonDcaVsPt->Draw("colz"); cv->SaveAs("h2KaonDcaVsPt.png");
  h2PionDcaVsPt->Draw("colz"); cv->SaveAs("h2PionDcaVsPt.png");
  h2PionDcaVsPt->Draw("colz"); cv->SaveAs("h2PionDcaVsPt.png");
  h2CosThetaVsPt->Draw("colz"); cv->SaveAs("h2CosThetaVsPt.png");
  h2DcaDaughtersVsPt->Draw("colz"); cv->SaveAs("h2DcaDaughtersVsPt.png");
  drawD0InPtBins(cv,h2InvariantMassVsPt,h1TotalEventsInRun->GetEntries()/1e6);
  delete cv;

  fInput->Close();
}

void drawD0InPtBins(TCanvas* cv,TH2F* h,int nEvents)
{
  float ptMin[] = {1.5,1.5,2.5,3.5,5.0,07.0};
  float ptMax[] = {12.,2.5,3.5,5.0,7.0,12.0};
  size_t nPt = sizeof(ptMin)/sizeof(ptMin[0]);

  for(size_t iPt=0; iPt < nPt; ++iPt)
  {
    h->GetXaxis()->SetRangeUser(ptMin[iPt],ptMax[iPt]);
    cv->cd();
    TH1* hProj = (TH1*)h->ProjectionY();
    setStyle(hProj,1,"Counts (per 10 MeV/c^{2})");
    hProj->GetXaxis()->SetRangeUser(1.665,2.1);
    hProj->SetMinimum(0);
    hProj->Draw();
    TF1 fun("fun","gaus+expo(3)",1.66,2.1);
    fun.SetParameters(30,1.865,0.015,0,0);
    fun.SetParLimits(2,0.008,0.030);
    fun.SetLineWidth(4);
    fun.SetLineColor(2);
    fun.SetLineStyle(2);
    hProj->Fit(&fun,"RN");
    fun.Draw("same");
    hProj->Draw("same");
    auto significance = getSignificance(&fun,2.5,hProj->GetBinWidth(1));
    TLatex latex;
    latex.SetTextFont(132);
    latex.DrawLatexNDC(0.60,0.87,"Au+Au #sqrt{s_{NN}} = 200 GeV");
    latex.DrawLatexNDC(0.60,0.81,"RHIC Run 2014");
    latex.DrawLatexNDC(0.60,0.75,Form("%1.2f < p_{T} < %1.2f",ptMin[iPt],ptMax[iPt]));
    latex.DrawLatexNDC(0.15,0.33,Form("%iM MinBias Events",nEvents));
    latex.DrawLatexNDC(0.15,0.26,Form("S/#sqrt{S+B} = %i",TMath::Nint(significance.second)));
    latex.DrawLatexNDC(0.15,0.19,Form("Counts = %i",TMath::Nint(significance.first)));

    cv->SaveAs(Form("%s_pt%i.png",h->GetName(),static_cast<int>(iPt)));
  }
}

std::pair<float,float> getSignificance(TF1* f,float integrationSigma,float binWidth)
{
  float A = f->GetParameter(0);
  float mean = f->GetParameter(1);
  float sigma = f->GetParameter(2);
  TF1 fGaus("fGaus","gaus",f->GetXmin(),f->GetXmax());
  fGaus.SetParameters(A,mean,sigma);

  float signal = fGaus.Integral(mean-integrationSigma*sigma,mean+integrationSigma*sigma)/binWidth;
  float all = f->Integral(mean-integrationSigma*sigma,mean+integrationSigma*sigma)/binWidth;

  return {signal,signal/sqrt(all)};
}

void setStyle(TH1* h,Color_t color,string title)
{
  h->GetYaxis()->SetTitle(title.c_str());
  h->SetMarkerStyle(20);
  h->SetLineColor(color);
  h->SetMarkerColor(color);
  h->SetTitleFont(132,"X");
  h->SetLabelFont(132,"X");
  h->SetTitleFont(132,"Y");
  h->SetLabelFont(132,"Y");
  h->SetMarkerSize(1.4);
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

    const Int_t NCont = 255;
    gStyle->SetNumberContours(NCont);
    gStyle->UseCurrentStyle();
    // **************************************************************************************
}

