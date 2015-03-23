#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"

#include "StPicoHFEvent.h"
#include "StKaonPion.h"

ClassImp(StPicoHFEvent)

TClonesArray *StPicoHFEvent::fgKaonPionArray = 0;

//-----------------------------------------------------------------------
StPicoHFEvent::StPicoHFEvent() : mRunId(-1), mEventId(-1), mNKaonPion(0), mKaonPionArray(NULL)
{
   if (!fgKaonPionArray) fgKaonPionArray = new TClonesArray("StKaonPion");
   mKaonPionArray = fgKaonPionArray;
}

//-----------------------------------------------------------------------
void StPicoHFEvent::addPicoEvent(StPicoEvent const & picoEvent)
{
   // StPicoEvent variables
   mRunId = picoEvent.runId();
   mEventId = picoEvent.eventId();
}

//-----------------------------------------------------------------------
void StPicoHFEvent::clear(char const *option)
{
   mKaonPionArray->Clear(option);
   mRunId = -1;
   mEventId = -1;
   mNKaonPion = 0;
}
//---------------------------------------------------------------------
void StPicoHFEvent::addKaonPion(StKaonPion const* t)
{
   TClonesArray &kaonPionArray = *mKaonPionArray;
   new(kaonPionArray[mNKaonPion++]) StKaonPion(t);
}
