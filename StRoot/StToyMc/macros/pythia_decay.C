/* *********************************************************************   
 *  ROOT macro - Toy Monte Carlo Simulation of Particle Decay Kinematics   
 *  Example for D+/D- --> Kpipi
 *   
 *  Authors:  Mustafa Mustafa (mmustafa@lbl.gov)   
 *            **Michael Lomnitz (mrlomnitz@lbl.gov)
 *
 *  ** Code Maintainer
 *   
 * *********************************************************************
*/

#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TH1F.h"
#include "TNtuple.h"
#include "TF1.h"
#include "TClonesArray.h"
#include "TPythia6.h"
#include "TPythia6Decayer.h"
#include "TRandom.h"
#include "TParticle.h"
#include "TGraph.h"
#include "TMath.h"
#include "phys_constants.h"

using namespace std;

void setDecayChannels(int const mdme);
void decay(int const kf, TLorentzVector* b, TClonesArray& daughters);
void fill(int const kf, TLorentzVector* b, double const weight, TClonesArray& daughters);
void getKinematics(TLorentzVector& b,double const mass);
TLorentzVector smearMom(TLorentzVector const& b,TF1 const * const fMomResolution);
void bookObjects();
void write();

TPythia6Decayer* pydecay;
TNtuple* nt;
TFile* result;

TF1* fKaonMomResolution = 0;
TF1* fPionMomResolution = 0;
TF1* fWeightFunction = 0;

string outFileName = "Dpm.root";
std::pair<int,int> const decayChannels(673,736);
std::pair<float,float> const momentumRange(0,10);
float const acceptanceEta = 1.0;

//============== main  program ==================
void pythia_decay(int npart = 100)
{
   gRandom->SetSeed();
   bookObjects();

   pydecay = TPythia6Decayer::Instance();
   pydecay->Init();

   setDecayChannels(719); // D+/D- --> Kpipi

   TLorentzVector* b_d = new TLorentzVector;
   TClonesArray ptl("TParticle", 10);
   for (int ipart = 0; ipart < npart; ipart++)
   {
      if (!(ipart % 100000))
         cout << "____________ ipart = " << ipart << " ________________" << endl;

      getKinematics(*b_d,M_D_PLUS);

      decay(411,b_d,ptl);
      fill(411, b_d, fWeightFunction->Eval(b_d->Perp()), ptl);
      decay(-411,b_d,ptl);
      fill(-411, b_d, fWeightFunction->Eval(b_d->Perp()), ptl);
   }

   write();
}

void setDecayChannels(int const mdme)
{
   for (int idc = decayChannels.first; idc < decayChannels.second+1; idc++) TPythia6::Instance()->SetMDME(idc, 1, 0);
   TPythia6::Instance()->SetMDME(mdme, 1, 1);
}

void decay(int const kf, TLorentzVector* const b, TClonesArray& daughters)
{
   pydecay->Decay(kf, b);
   pydecay->ImportParticles(&daughters);
}

void fill(int const kf, TLorentzVector* b, double const weight, TClonesArray& daughters)
{
   TLorentzVector kMom;
   TLorentzVector p1Mom;
   TLorentzVector p2Mom;

   int nTrk = daughters.GetEntriesFast();
   for (int iTrk = 0; iTrk < nTrk; ++iTrk)
   {
      TParticle* ptl0 = (TParticle*)daughters.At(iTrk);

      switch(abs(ptl0->GetPdgCode()))
      {
        case 321:
          ptl0->Momentum(kMom);
          break;
        case 211:
          if(!p1Mom.P()) ptl0->Momentum(p1Mom);
          else ptl0->Momentum(p2Mom);
          break;
        default:
          break;
      }
   }
   daughters.Clear();

   // smear and get total momentum
   TLorentzVector kRMom = smearMom(kMom,fKaonMomResolution);
   TLorentzVector p1RMom = smearMom(p1Mom,fPionMomResolution);
   TLorentzVector p2RMom = smearMom(p2Mom,fPionMomResolution);
   TLorentzVector rMom = kRMom + p1RMom + p2RMom;

   // save
   float arr[100];
   int iArr = 0;
   arr[iArr++] = kf;
   arr[iArr++] = weight;
   arr[iArr++] = b->M();
   arr[iArr++] = b->Perp();
   arr[iArr++] = b->PseudoRapidity();
   arr[iArr++] = b->Rapidity();
   arr[iArr++] = b->Phi();

   arr[iArr++] = rMom.M();
   arr[iArr++] = rMom.Perp();
   arr[iArr++] = rMom.PseudoRapidity();
   arr[iArr++] = rMom.Rapidity();
   arr[iArr++] = rMom.Phi();

   arr[iArr++] = kMom.M();
   arr[iArr++] = kMom.Perp();
   arr[iArr++] = kMom.PseudoRapidity();
   arr[iArr++] = kMom.Rapidity();
   arr[iArr++] = kMom.Phi();

   arr[iArr++] = kRMom.M();
   arr[iArr++] = kRMom.Perp();
   arr[iArr++] = kRMom.PseudoRapidity();
   arr[iArr++] = kRMom.Rapidity();
   arr[iArr++] = kRMom.Phi();

   arr[iArr++] = p1Mom.M();
   arr[iArr++] = p1Mom.Perp();
   arr[iArr++] = p1Mom.PseudoRapidity();
   arr[iArr++] = p1Mom.Rapidity();
   arr[iArr++] = p1Mom.Phi();

   arr[iArr++] = p1RMom.M();
   arr[iArr++] = p1RMom.Perp();
   arr[iArr++] = p1RMom.PseudoRapidity();
   arr[iArr++] = p1RMom.Rapidity();
   arr[iArr++] = p1RMom.Phi();

   arr[iArr++] = p2Mom.M();
   arr[iArr++] = p2Mom.Perp();
   arr[iArr++] = p2Mom.PseudoRapidity();
   arr[iArr++] = p2Mom.Rapidity();
   arr[iArr++] = p2Mom.Phi();

   arr[iArr++] = p2RMom.M();
   arr[iArr++] = p2RMom.Perp();
   arr[iArr++] = p2RMom.PseudoRapidity();
   arr[iArr++] = p2RMom.Rapidity();
   arr[iArr++] = p2RMom.Phi();
   
   nt->Fill(arr);
}

void getKinematics(TLorentzVector& b,double const mass)
{
   float const pt = gRandom->Uniform(momentumRange.first,momentumRange.second);
   float const eta = gRandom->Uniform(-acceptanceEta,acceptanceEta);
   float const phi = TMath::TwoPi() * gRandom->Rndm();

   b.SetXYZM(pt * cos(phi),pt * sin(phi),pt * sinh(eta), mass);
}

TLorentzVector smearMom(TLorentzVector const& b,TF1 const * const fMomResolution)
{
  float const pt = b.Perp();
  float const sPt = gRandom->Gaus(pt,pt*fMomResolution->Eval(pt));

  TLorentzVector sMom;
  sMom.SetXYZM(sPt*cos(b.Phi()),sPt*sin(b.Phi()),sPt*sinh(b.PseudoRapidity()),b.M());
  return sMom;
}
//___________
void bookObjects()
{
   result = new TFile(outFileName.c_str(), "recreate");
   result->cd();

   nt = new TNtuple("nt", "", "pid:w:m:pt:eta:y:phi:" // MC D+
                              "rM:rPt:rEta:rY:rPhi:" // Rc D+
                              "kM:kPt:kEta:kY:kPhi:" // MC Kaon
                              "kRM:kRPt:kREta:kRY:kRPhi:" // Rc Kaon
                              "p1M:p1Pt:p1Eta:p1Y:p1Phi:" // MC Pion1
                              "p1RM:p1RPt:p1REta:p1RY:p1RPhi:" // Rc Pion1
                              "p2M:p2Pt:p2Eta:p2Y:p2Phi:" // MC Pion2
                              "p2RM:p2RPt:p2REta:p2RY:p2RPhi"); // Rc Pion2

   TFile f("momentum_resolution.root");
   fPionMomResolution = (TF1*)f.Get("fPion")->Clone("fPionMomResolution");
   fKaonMomResolution = (TF1*)f.Get("fKaon")->Clone("fKaonMomResolution");
   f.Close();

   TFile fPP("pp200_spectra.root");
   fWeightFunction = (TF1*)fPP.Get("run12/f1Levy")->Clone("fWeightFunction");
   fPP.Close();
}
//___________
void write()
{
   result->cd();
   nt->Write();
   result->Close();
}
