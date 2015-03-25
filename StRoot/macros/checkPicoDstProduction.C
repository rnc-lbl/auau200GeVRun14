/*
 * File:   checkProduction.C
 * Author: mustafa
 *
 * This macro is to check the intergrity of PicoDst production.
 */

#include <iostream>

#include "TFile.h"
#include "TTree.h"`
#include "TUnixSystem.h"


using namespace std;

void deleteFile(TString filename);

void checkPicoDstProduction(TString picoFileName, int nMuDstEvents, float numberOfEventsRatio = 0.95)
{
   if (!nMuDstEvents)
   {
      cout << "FILE IS EMPTY" << endl;
   }

   TFile fPicoDst(picoFileName.Data());

   // Check if the root file is zombie
   if (fPicoDst.IsZombie())
   {
      cout << "WARNING - ZMOBIE FILE: " << picoFileName << endl;
      fPicoDst.Close();
      deleteFile(picoFileName);
      return;
   }

   // Get the number of events stored in the etree
   TTree* picoDst = (TTree*)fPicoDst.Get("PicoDst");

   int nPicoEvents = 0;
   if (picoDst)
   {
      nPicoEvents = picoDst->GetEntriesFast();

      if (static_cast<float>(nPicoEvents) / nMuDstEvents < numberOfEventsRatio)
      {
         fPicoDst.Close();
         cout << "WARNING - LOW NUMBER OF EVENTS: " << picoFileName << endl;
         deleteFile(picoFileName);
         return;
      }
   }
   else
   {
      cout << "WARNING - TREE DOES NOT EXIST: " << picoFileName << endl;
      deleteFile(picoFileName);
      return;
   }

   return;
}

void deleteFile(TString filename)
{
  TString command = "rm -f " + filename;
  cout<<command<<endl;
  gSystem->Exec(command.Data());
}
