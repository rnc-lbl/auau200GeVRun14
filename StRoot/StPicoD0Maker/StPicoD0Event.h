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
   void    addKaonPion(StKaonPion const*);

   TClonesArray const * kaonPionArray()   const;
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

   ClassDef(StPicoD0Event, 1)
};

inline TClonesArray const * StPicoD0Event::kaonPionArray()   const { return mKaonPionArray;}
inline int   StPicoD0Event::nKaonPion()  const { return mNKaonPion;}
inline Int_t StPicoD0Event::runId()   const { return mRunId; }
inline Int_t StPicoD0Event::eventId() const { return mEventId; }
#endif
