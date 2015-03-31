#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>

#include "TH1F.h"

#include "../StPicoDstMaker/StPicoConstants.h"
#include "StPicoPrescales.h"

using namespace std;

ClassImp(StPicoPrescales);

StPicoPrescales::StPicoPrescales(string prescalesFilesDirectoryName): mPrescalesFilesDirectoryName(prescalesFilesDirectoryName)
{

  for(int i=0;i<nTrigger;++i)
  {
    mTriggersIds.push_back(Pico::mTriggerId[i]);
    readList(mTriggersIds.back());
  }

  for(int i=0;i<nTriggerMtd;++i)
  {
    mTriggersIds.push_back(Pico::mTriggerIdMtd[i]);
    readList(mTriggersIds.back());
  }

   mLastQuery = mTable.end();
}
//___________________________________________
void StPicoPrescales::readList(unsigned int trgId)
{
   stringstream st;
   st << trgId;
   string listFileName = mPrescalesFilesDirectoryName + "/" + st.str() + ".txt";
   cout << "Reading prescale values for trigger " << trgId << endl;
   cout << "From list " << listFileName << endl;

   //Open list
   ifstream runs(listFileName.c_str());

   while (!runs.eof())
   {
      string line;
      int run;
      float prescale;

      getline(runs, line);
      if (line == "\0" || line == "\n") continue;

      size_t firstSpace = line.find(" ");
      size_t lastSpace = line.rfind(" ");

      istringstream runBuffer(line.substr(0, firstSpace));
      istringstream prescaleBuffer(line.substr(lastSpace + 1));

      runBuffer >> run;
      prescaleBuffer >> prescale;

      map<unsigned int, vecPrescales>::iterator it = mTable.find(run);

      if (it == mTable.end())
      {
         vecPrescales vec(mTriggersIds.size(), -1);
         vec[trgId] = prescale;
         mTable.insert(pair<unsigned int, vecPrescales>(run, vec));
      }
      else
      {
         if (it->second.at(trgId) == -1) it->second.at(trgId) = prescale;
         else
         {
            cout << "Two prescale values for same run and same trigger." << endl;
            cout << "Run= " << run << " Trigger= " << mTriggersIds[trgId] << " StPicoPrescales= " << it->second.at(trgId) << " " << prescale << endl;
         }
      }
   }

   runs.close();
}

//__________________________________
float StPicoPrescales::getPrescale(unsigned int run, unsigned int trg)
{
   if(trg > mTriggersIds.size())
   {
     cout << "StPicoPrescales requested triggers doesn't exist. See StTRIGGERS.h for triggers definition." << endl;
     return -1;
   }

   if (mLastQuery != mTable.end() && run == mLastQuery->first) return mLastQuery->second.at(trg);
   else
   {
      map<unsigned int, vecPrescales>::iterator it = mTable.find(run);

      if (it != mTable.end())
      {
         mLastQuery = it;
         return it->second.at(trg);
      }
      else
      {
         cout << "StPicoPrescales::GetPrescale: No prescale values available for run " << run << ". Skip it." << endl;
         return -1;
      }
   }
}

//__________________________________
int StPicoPrescales::getNumberOfRuns() const
{
   return mTable.size();
}
//__________________________________
void StPicoPrescales::fillPrescalesHist(TH1F* hist, unsigned int trg)
{
   if(!hist) return;

   if(trg > mTriggersIds.size())
   {
     cout << "StPicoPrescales requested triggers doesn't exist. See StTRIGGERS.h for triggers definition." << endl;
     return;
   }

   for (map<unsigned int,vecPrescales>::iterator it = mTable.begin(); it != mTable.end(); ++it)
   {
      hist->Fill(std::distance(mTable.begin(), it), it->second.at(trg));
   }
}
//___________________________________
unsigned int StPicoPrescales::runIndex(unsigned int run)
{
   if (mLastQuery != mTable.end() && run == mLastQuery->first) return std::distance(mTable.begin(), mLastQuery);
   else
   {
      map<unsigned int, vecPrescales>::iterator it = mTable.find(run);
      mLastQuery = it;
      return std::distance(mTable.begin(), it);
   }
}

//___________________________________
bool StPicoPrescales::runExists(unsigned int run)
{
   if (mLastQuery != mTable.end() && run == mLastQuery->first) return true;
   else
   {
      map<unsigned int, vecPrescales>::iterator it = mTable.find(run);

      if (it != mTable.end())
      {
         mLastQuery = it;
         return true;
      }
      else return false;
   }
}

