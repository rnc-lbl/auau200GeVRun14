/* *********************************************************************   
 *  ROOT macro - Toy Monte Carlo Simulation of Particle Decay Kinematics   
 *   
 *  Authors:  Mustafa Mustafa (mmustafa@lbl.gov)   
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

//.. decay the particles and fill the histogram ....
void Decay_Fill(int const kf, int const mdme, TLorentzVector* b, float const phi, double const weight, TClonesArray& ptl);
//.. get kinematics ....
void get_kinematics(double& pt, double& eta, double& phi, double& px, double& py, double& pz);
TLorentzVector smearMom(TLorentzVector const& b,TF1 const * const fMomResolution);


void BookObjects();
void Write();

TPythia6Decayer* pydecay;
TNtuple* nt;
TFile* result;
TF1* fKaonMomResolution = 0;
TF1* fPionMomResolution = 0;

float const accp_eta = 1;
//============== main  program ==================
void pythia(int npart = 100)
{
   gRandom->SetSeed();
   BookObjects();

   pydecay = TPythia6Decayer::Instance();
   pydecay->Init();

   //.. kinetic variables for each ptcls.
   TLorentzVector* b_d = new TLorentzVector;

   //.. holder of decay particles ...
   TClonesArray ptl("TParticle", 10);

   double pt = -999, eta = -999, phi = -999, px = -999, py = -999, pz = -999;

   for (int idc = 673; idc < 736; idc++) TPythia6::Instance()->SetMDME(idc, 1, 0); // turn all D+ decay channels off
   TPythia6::Instance()->SetMDME(719, 1, 1); // turn  on : D+ -->K-pi+pi-

   for (int ipart = 0; ipart < npart; ipart++)
   {
      if (!(ipart % 100000))
         cout << "____________ ipart = " << ipart << " ________________" << endl;

      get_kinematics(pt, eta, phi, px, py, pz);

      double E_d0 = sqrt(px * px + py * py + pz * pz + M_D_PLUS * M_D_PLUS);
      b_d->SetPxPyPzE(px, py, pz, E_d0);

      Decay_Fill(411, 719, b_d, phi, 1, ptl);
      Decay_Fill(-411, 719, b_d, phi, 1, ptl);
   }

   Write();
}
//.. decay the particles and fill the histogram
void Decay_Fill(int const kf, int const mdme, TLorentzVector* const b, float const phi, double const weight, TClonesArray& ptl)
{
   pydecay->Decay(kf, b);
   pydecay->ImportParticles(&ptl);

   TLorentzVector kMom;
   TLorentzVector p1Mom;
   TLorentzVector p2Mom;

   int nTrk = ptl.GetEntriesFast();
   for (int iTrk = 0; iTrk < nTrk; ++iTrk)
   {
      TParticle* ptl0 = (TParticle*)ptl.At(iTrk);

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
   ptl.Clear();

   // smear and get total momentum
   TLorentzVector kRMom = smearMom(kMom,fKaonMomResolution);
   TLorentzVector p1RMom = smearMom(p1Mom,fPionMomResolution);
   // TLorentzVector p2RMom = smearMom(p2Mom,fPionMomResolution);
   // TLorentzVector rMom = kRMom + p1RMom + p2RMom;
   TLorentzVector rMom = kRMom + p1RMom;

   // save
   float arr[100];
   int iArr = 0;
   arr[iArr++] = kf;
   arr[iArr++] = b->M();
   arr[iArr++] = b->Perp();
   arr[iArr++] = b->PseudoRapidity();
   arr[iArr++] = b->Rapidity();
   arr[iArr++] = phi;

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
//.. produce decay kinematics ....
void get_kinematics(double& pt, double& eta, double& phi, double& px, double& py, double& pz)
{
   pt = gRandom->Uniform(0, 10);
   eta = gRandom->Uniform(-accp_eta, accp_eta);
   phi = TMath::TwoPi() * gRandom->Rndm();
   px = pt * cos(phi);
   py = pt * sin(phi);
   pz = pt * sinh(eta);
}
TLorentzVector smearMom(TLorentzVector const& b,TF1 const * const fMomResolution)
{
  float const pt = b.Perp();
  float const sPt = gRandom->Gaus(pt,1.5*pt*fMomResolution->Eval(pt));

  TLorentzVector sMom;
  sMom.SetXYZM(sPt*cos(b.Phi()),sPt*sin(b.Phi()),sPt*sinh(b.PseudoRapidity()),b.M());
  return sMom;
}
//___________
void BookObjects()
{
   result = new TFile("out.root", "recreate");
   result->cd();
   nt = new TNtuple("nt", "", "pid:m:pt:eta:y:phi:" // MC D+
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
}
//___________
void Write()
{
   result->cd();
   nt->Write();
   result->Close();
}
