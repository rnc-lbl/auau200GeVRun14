/*
 * File:   checkProduction.C
 * Author: mustafa
 *
 * This macro is to check the intergrity of prorductions.
 */

#include <iostream>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TUnixSystem.h"


using namespace std;

enum fileState {Zombie = -1, NoTree = -2, CannotOpenFile=-3};

void deleteFile(TString filename);
int getNumberOfEvents(TString filename, TString treeName);

void checkProduction(TString picoFileName, TString muFileName,
                            float numberOfEventsRatio = 0.95, TString picoTreeName = "PicoDst", TString muTreeName = "MuDst")
{
   int nMuDstEvents = getNumberOfEvents(muFileName, muTreeName);

   if (nMuDstEvents > 0)
   {
      int nPicoEvents = getNumberOfEvents(picoFileName, picoTreeName);

      if (nPicoEvents > 0)
      {
         if (static_cast<float>(nPicoEvents) / nMuDstEvents < numberOfEventsRatio)
         {
            cerr<<'\n';
            cerr<< "nEvents  = " << nMuDstEvents <<" and nProducedEvents = " << nPicoEvents <<endl;
            cerr << "WARNING - LOW NUMBER OF EVENTS: " << picoFileName << endl;
            deleteFile(picoFileName);
            return;
         }
         else
         {
           cerr<< " FILE IS GOOD: "<<endl;
           cerr<< "nEvents  = " << nMuDstEvents <<" and nProducedEvents = " << nPicoEvents <<endl;
         }
      }
      else
      {
        deleteFile(picoFileName);
        return;
      }
   }
   else if (nMuDstEvents == 0)
   {
      cerr << "MuDst FILE IS EMPTY" << endl;
   }
   else
   {
      cerr << "MuDst FILE IS NOT ACCESSIBLE OR TREE DOES NOT EXIST" << endl;
      deleteFile(picoFileName);
   }

   return;
}

void deleteFile(TString filename)
{
   TString command = "rm -f " + filename;
   cerr<<'\n';
   cerr << command << endl;
   gSystem->Exec(command.Data());
}

int getNumberOfEvents(TString filename, TString treeName)
{
   TFile* file = TFile::Open(filename.Data());

   if(!file)
   {
      cerr<<'\n';
      cerr << "WARNING - CANNOT OPEN FILE: " << filename << endl;
      return CannotOpenFile;

   }

   // Check if the root file is zombie
   if (file->IsZombie())
   {
      cerr<<'\n';
      cerr << "WARNING - ZMOBIE FILE: " << filename << endl;
      file->Close();
      return Zombie;
   }

   // Get the number of events stored in the etree
   TTree* tree = (TTree*)file->Get(treeName.Data());

   if (tree)
   {
      int nEvents = tree->GetEntriesFast();
      file->Close();
      return nEvents;
   }
   else
   {
      cerr<<'\n';
      cerr << "WARNING - TREE DOES NOT EXIST: " << filename << endl;
      file->Close();
      return NoTree;
   }
}
