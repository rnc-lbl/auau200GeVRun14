#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"

#include "StPicoNpeEvent.h"
#include "StElectronPair.h"
#include "StElectronTrack.h"

ClassImp(StPicoNpeEvent)

TClonesArray *StPicoNpeEvent::fgElectronPairArray = 0;
TClonesArray *StPicoNpeEvent::fgElectronArray = 0;

//-----------------------------------------------------------------------
StPicoNpeEvent::StPicoNpeEvent() : mRunId(-1), mEventId(-1), mNElectronTrack(0), mNElectronPair(0), mNElectrons(0), mNPartners(0), mElectronPairArray(NULL), mElectronArray(NULL)
{
    if (!fgElectronPairArray) fgElectronPairArray = new TClonesArray("StElectronPair");
    mElectronPairArray = fgElectronPairArray;

    if (!fgElectronArray) fgElectronArray = new TClonesArray("StElectronTrack");
    mElectronArray = fgElectronArray;
}

//-----------------------------------------------------------------------
void StPicoNpeEvent::addPicoEvent(StPicoEvent const & picoEvent)
{
    // StPicoEvent variables
    mRunId = picoEvent.runId();
    mEventId = picoEvent.eventId();
}

//-----------------------------------------------------------------------
void StPicoNpeEvent::clear(char const *option)
{
    mElectronPairArray->Clear(option);
    mElectronArray->Clear(option);
    mRunId = -1;
    mEventId = -1;
    mNElectronPair = 0;
    mNElectronTrack = 0;
    mNElectrons = 0;
    mNPartners = 0;
}
//---------------------------------------------------------------------
void StPicoNpeEvent::addElectronPair(StElectronPair const* t)
{
    TClonesArray &electronPairArray = *mElectronPairArray;
    new(electronPairArray[mNElectronPair++]) StElectronPair(t);
}
//---------------------------------------------------------------------
void StPicoNpeEvent::addElectron(StElectronTrack const* t)
{
    TClonesArray &electronArray = *mElectronArray;
    new(electronArray[mNElectronTrack++]) StElectronTrack(t);
}
