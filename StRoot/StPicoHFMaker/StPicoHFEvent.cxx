#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"

#include "StPicoHFEvent.h"
#include "StHFPair.h"
#include "StHFSecondaryPair.h"
#include "StHFTriplet.h"

ClassImp(StPicoHFEvent)

TClonesArray *StPicoHFEvent::fgHFPrimaryArray = 0;
TClonesArray *StPicoHFEvent::fgHFSecondaryArray = 0;

//-----------------------------------------------------------------------
StPicoHFEvent::StPicoHFEvent() : mRunId(-1), mEventId(-1), mNHFPrimary(0), mNHFSecondary(0), 
				 mHFPrimaryArray(NULL), mHFSecondaryArray(NULL) 
{
  // -- Default constructor

   if (!fgHFPrimaryArray) fgHFPrimaryArray = new TClonesArray("StHFPair");
   mHFPrimaryArray = fgHFPrimaryArray;
}

//-----------------------------------------------------------------------
StPicoHFEvent::StPicoHFEvent(unsigned int mode) : mRunId(-1), mEventId(-1), mNHFPrimary(0), mNHFSecondary(0), 
						  mHFPrimaryArray(NULL), mHFSecondaryArray(NULL) 
{
  // -- Constructor with mode selection
  if (mode == StPicoHFEvent::secondPair) {
    if (!fgHFPrimaryArray) fgHFPrimaryArray = new TClonesArray("StHFPair");
    mHFPrimaryArray = fgHFPrimaryArray;

    if (!fgHFSecondaryArray) fgHFSecondaryArray = new TClonesArray("StHFSecondaryPair");
    mHFSecondaryArray = fgHFSecondaryArray;
  }
  else if (mode == StPicoHFEvent::triplet) {
    if (!fgHFPrimaryArray) fgHFPrimaryArray = new TClonesArray("StHFTriplet");
    mHFPrimaryArray = fgHFPrimaryArray;
  }
  else {
    if (!fgHFPrimaryArray) fgHFPrimaryArray = new TClonesArray("StHFPair");
    mHFPrimaryArray = fgHFPrimaryArray;
  }
}

//-----------------------------------------------------------------------
void StPicoHFEvent::addPicoEvent(StPicoEvent const & picoEvent)
{
   // StPicoEvent variables
   mRunId   = picoEvent.runId();
   mEventId = picoEvent.eventId();
}

//-----------------------------------------------------------------------
void StPicoHFEvent::clear(char const *option)
{
   mHFPrimaryArray->Clear(option);
   if (mHFSecondaryArray)
     mHFSecondaryArray->Clear(option);

   mRunId   = -1;
   mEventId = -1;
   mNHFPrimary   = 0;
   mNHFSecondary = 0;
}

//---------------------------------------------------------------------
void StPicoHFEvent::addHFPrimary(StHFPair const* t) {
   TClonesArray &primaryArray = *mHFPrimaryArray;
   new(primaryArray[mNHFPrimary++]) StHFPair(t);
}

//---------------------------------------------------------------------
void StPicoHFEvent::addHFPrimary(StHFTriplet const* t) {
   TClonesArray &primaryArray = *mHFPrimaryArray;
   new(primaryArray[mNHFPrimary++]) StHFTriplet(t);
}

//---------------------------------------------------------------------
void StPicoHFEvent::addHFSecondary(StHFSecondaryPair const* t) {
   TClonesArray &secondaryArray = *mHFSecondaryArray;
   new(secondaryArray[mNHFSecondary++]) StHFSecondaryPair(t);
}
