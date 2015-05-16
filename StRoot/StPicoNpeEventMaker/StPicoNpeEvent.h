#ifndef StPicoNpeEvent__h
#define StPicoNpeEvent__h

/* **************************************************
 *  A specialized class for storing eventwise Npe
 *  candidates.
 *
 *  Authors:  **Kunsu OH        (kunsuoh@gmail.com)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  **Code Maintainer
 * **************************************************
 */

class StPicoEvent;

#include "TObject.h"
#include "TClonesArray.h"

#include "StElectronPair.h"
#include "StElectronTrack.h"

class StPicoNpeEvent : public TObject
{
public:
    StPicoNpeEvent();
    ~StPicoNpeEvent(){ clear("C");}
    void    clear(char const *option = "");
    void    addPicoEvent(StPicoEvent const & picoEvent);
    void    addElectronPair(StElectronPair const*);
    void    addElectron(StElectronTrack const*);
    void    nElectrons(int);
    void    nPartners(int);
    
    Int_t   runId()   const;
    Int_t   eventId() const;
    TClonesArray const * electronPairArray()   const;
    TClonesArray const * electronArray()   const;
    int     nElectronPair()  const;
    int     nElectron()  const;
    int     nElectrons() const;
    int     nPartners() const;
    
private:
    // some variables below are kept in ROOT types to match the same ones in StPicoEvent
    Int_t   mRunId;           // run number
    Int_t   mEventId;         // event number
    int   mNElectronPair;       // number of stored pairs
    int   mNElectronTrack;       // number of stored electron
    int   mNElectrons;
    int   mNPartners;
    
    TClonesArray*        mElectronPairArray;
    TClonesArray*        mElectronArray;
    static TClonesArray* fgElectronPairArray;
    static TClonesArray* fgElectronArray;
    
    ClassDef(StPicoNpeEvent, 1)
};

inline void StPicoNpeEvent::nElectrons(int n) { mNElectrons = n; }
inline void StPicoNpeEvent::nPartners(int n) { mNPartners = n; }

inline TClonesArray const * StPicoNpeEvent::electronPairArray()   const { return mElectronPairArray;}
inline TClonesArray const * StPicoNpeEvent::electronArray()   const { return mElectronArray;}
inline int   StPicoNpeEvent::nElectronPair()  const { return mNElectronPair;}
inline int   StPicoNpeEvent::nElectron()  const { return mNElectronTrack;}
inline int   StPicoNpeEvent::nElectrons()  const { return mNElectrons;}
inline int   StPicoNpeEvent::nPartners()  const { return mNPartners;}
inline Int_t StPicoNpeEvent::runId()   const { return mRunId; }
inline Int_t StPicoNpeEvent::eventId() const { return mEventId; }
#endif
