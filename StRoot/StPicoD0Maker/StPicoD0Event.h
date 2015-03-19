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

   // while ROOT types are not necessary here they are used for consistency with StPicoDst
   TClonesArray* kaonPion()   const { return mKaonPion;}
   Int_t         nKaonPion()  const { return mNKaonPion;}

   // for variables from StPicoEvent
   Int_t   runId()   const { return mRunId; }
   Int_t   eventId() const { return mEventId; }

private:
   // variables from StPicoEvent
   Int_t   mRunId;           // run number
   Int_t   mEventId;         // event number
   Int_t   mNKaonPion;       // number of stored pairs

   TClonesArray*        mKaonPion;
   static TClonesArray* fgKaonPion;

   ClassDef(StPicoD0Event, 1)
};

#endif
