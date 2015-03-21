#ifndef StPicoD0Event__h
#define StPicoD0Event__h

class StPicoEvent;

#include "TObject.h"
#include "TClonesArray.h"

#include "StKaonPion.h"

class StPicoD0Event : public TObject
{
public:
   StPicoD0Event();
   ~StPicoD0Event(){ clear("C");}
   void    clear(char const *option = "");
   void    addPicoEvent(StPicoEvent const & picoEvent);
   void    addKaonPion(StKaonPion*);

   TClonesArray const * kaonPion()   const { return mKaonPion;}
   int         nKaonPion()  const { return mNKaonPion;}

   // for variables from StPicoEvent
   Int_t   runId()   const { return mRunId; }
   Int_t   eventId() const { return mEventId; }

private:
   // some variables below are kept in ROOT types to match the same ones in StPicoEvent
   Int_t   mRunId;           // run number
   Int_t   mEventId;         // event number
   int   mNKaonPion;       // number of stored pairs

   TClonesArray*        mKaonPion;
   static TClonesArray* fgKaonPion;

   ClassDef(StPicoD0Event, 1)
};

#endif
