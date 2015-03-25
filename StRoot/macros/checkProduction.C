/*
 * File:   checkProduction.C
 * Author: mustafa
 *
 * This macro is to check the intergrity of prorductions.
 */

#include <fstream>
#include <iostream>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TUnixSystem.h"


using namespace std;

enum fileState {Zombie = -1, NoTree = -2, CannotOpenFile = -3};

void deleteFile(TString filename, ofstream& os);
int getNumberOfEvents(TString filename, TString treeName, ofstream& os);

void checkProduction(TString picoFileName, TString muFileName,
                     float numberOfEventsRatio = 0.95, TString picoTreeName = "PicoDst", TString muTreeName = "MuDst")
{
   TString logFileName = picoFileName;
   logFileName = logFileName.ReplaceAll(".picoDst.root", ".nEventsCheck.log");

   ofstream logOs(logFileName.Data(),ios::out);

   logOs << '\n';
   logOs << "CHECKING OUTPUT " << endl;
   logOs << "picoFileName: " << picoFileName << endl;
   logOs << "muFileName: " << muFileName << endl;
   logOs << '\n';

   int nMuDstEvents = getNumberOfEvents(muFileName, muTreeName,logOs);

   logOs << "nMuDstEvents = " << nMuDstEvents << endl;

   if (nMuDstEvents > 0)
   {
      int nPicoEvents = getNumberOfEvents(picoFileName, picoTreeName,logOs);

      logOs << "nPicoEvents = " << nPicoEvents << endl;

      if (nPicoEvents > 0)
      {
         if (static_cast<float>(nPicoEvents) / nMuDstEvents < numberOfEventsRatio)
         {
            logOs << '\n';
            logOs << "WARNING - LOW NUMBER OF EVENTS: " << picoFileName << endl;
            deleteFile(picoFileName,logOs);
            return;
         }
         else
         {
            logOs << " FILE IS GOOD: " << endl;
         }
      }
      else
      {
         deleteFile(picoFileName,logOs);
         return;
      }
   }
   else if (nMuDstEvents == 0)
   {
      logOs << "MuDst FILE IS EMPTY" << endl;
   }
   else
   {
      logOs << "MuDst FILE IS NOT ACCESSIBLE OR TREE DOES NOT EXIST" << endl;
      deleteFile(picoFileName,logOs);
   }

   return;
}

void deleteFile(TString filename, ofstream& logOs)
{
   TString command = "rm -f " + filename;
   logOs << '\n';
   logOs << command << endl;
   gSystem->Exec(command.Data());
}

int getNumberOfEvents(TString filename, TString treeName, ofstream& logOs)
{
   TFile* file = TFile::Open(filename.Data());

   if (!file)
   {
      cerr << '\n';
      logOs << "WARNING - CANNOT OPEN FILE: " << filename << endl;
      return CannotOpenFile;

   }

   // Check if the root file is zombie
   if (file->IsZombie())
   {
      logOs << '\n';
      logOs << "WARNING - ZMOBIE FILE: " << filename << endl;
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
      logOs << '\n';
      logOs << "WARNING - TREE DOES NOT EXIST: " << filename << endl;
      file->Close();
      return NoTree;
   }
}
