#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"

#include "StPicoD0Event.h"
#include "StKaonPion.h"

ClassImp(StPicoD0Event)

TClonesArray *StPicoD0Event::fgKaonPionArray = 0;

//-----------------------------------------------------------------------
StPicoD0Event::StPicoD0Event() : mRunId(-1), mEventId(-1), mNKaonPion(0), mKaonPionArray(NULL)
{
  if (!fgKaonPionArray) fgKaonPionArray = new TClonesArray("StKaonPion");
  mKaonPionArray = fgKaonPionArray; 
}

//-----------------------------------------------------------------------
void StPicoD0Event::addPicoEvent(StPicoEvent const & picoEvent)
{
  // StPicoEvent variables
  mRunId = picoEvent.runId();
  mEventId = picoEvent.eventId();
}

//-----------------------------------------------------------------------
void StPicoD0Event::clear(char const *option)
{
  mKaonPionArray->Clear(option);
  mRunId = -1;
  mEventId = -1;
  mNKaonPion = 0;
}
//---------------------------------------------------------------------
void StPicoD0Event::addKaonPion(StKaonPion* t)
{
  TClonesArray &kaonPionArray = *mKaonPionArray;
  new(kaonPionArray[mNKaonPion++]) StKaonPion(t);
}
