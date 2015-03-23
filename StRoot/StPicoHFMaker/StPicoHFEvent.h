#ifndef StPicoHFEvent__h
#define StPicoHFEvent__h

class StPicoEvent;

#include "TObject.h"
#include "TClonesArray.h"

#include "StKaonPion.h"

class StPicoHFEvent : public TObject
{
public:
   StPicoHFEvent();
   ~StPicoHFEvent(){ clear("C");}
   void    clear(char const *option = "");
   void    addPicoEvent(StPicoEvent const & picoEvent);
   void    addKaonPion(StKaonPion const*);

   TClonesArray const * kaonPionArray()   const { return mKaonPionArray;}
   int         nKaonPion()  const;

   // for variables from StPicoEvent
   Int_t   runId()   const;
   Int_t   eventId() const;

private:
   // some variables below are kept in ROOT types to match the same ones in StPicoEvent
   Int_t   mRunId;           // run number
   Int_t   mEventId;         // event number
   int   mNKaonPion;       // number of stored pairs

   TClonesArray*        mKaonPionArray;
   static TClonesArray* fgKaonPionArray;

   ClassDef(StPicoHFEvent, 1)
};

inline int   StPicoHFEvent::nKaonPion()  const { return mNKaonPion;}
inline Int_t StPicoHFEvent::runId()   const { return mRunId; }
inline Int_t StPicoHFEvent::eventId() const { return mEventId; }
#endif
