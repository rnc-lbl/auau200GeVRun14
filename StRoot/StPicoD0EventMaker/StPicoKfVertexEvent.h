#ifndef StPicoKfVertexEvent__h
#define StPicoKfVertexEvent__h

/* **************************************************
 *  A class to save event information with KF vertex.
 *
 *  Authors:  **Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  **Code Maintainer
 *
 * **************************************************
 */

#include "StarClassLibrary/StThreeVectorF.hh"

class TTree;
class TFile;
class StPicoEvent;

class StPicoKfVertexEvent
{
public:
   StPicoKfVertexEvent(char const*);
   void addEvent(StPicoEvent const&,StThreeVectorF const&);
   void closeFile();

private:

   int   mRunId;
   int   mEventId;
   int   mRefMult;
   int   mGRefMult;
   float mVx;
   float mVy;
   float mVz;
   float mKfVx;
   float mKfVy;
   float mKfVz;

   TFile* mOutputFile;
   TTree* mTree;
};

#endif
