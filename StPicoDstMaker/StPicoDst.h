#ifndef StPicoDst_h
#define StPicoDst_h


class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoV0;

#include "TObject.h"
#include "TClonesArray.h"
#include "StPicoArrays.h"

class StPicoDst : public TObject {
public:
  /// constructor
  StPicoDst();
  /// set the pointers to the TClonesArrays
  static void set(StPicoDstMaker* maker);
  /// set the pointers to the TClonesArrays
  static void set(TClonesArray**, TClonesArray**);
  /// resets the pointers to the TClonesArrays to 0
  static void unset();

protected:
  /// array of TClonesArrays
  static TClonesArray** picoArrays;
  /// array of TClonesArrays for the stuff inherited from the StPicoV0
  static TClonesArray** picoV0Arrays;

public:
  /// returns pointer to the n-th TClonesArray 
  static TClonesArray* picoArray(int type) { return picoArrays[type]; }
  /// returns pointer to the n-th TClonesArray from the picoV0 arrays
  static TClonesArray* picoV0Array(int type) { return picoV0Arrays[type]; }

  /// returns pointer to current StPicoEvent (class holding the event wise information)
  static StPicoEvent* event() { return (StPicoEvent*)picoArrays[picoEvent]->UncheckedAt(0); }
  /// return pointer to i-th track 
  static StPicoTrack* track(int i) { return (StPicoTrack*)picoArrays[picoTrack]->UncheckedAt(i); }
  /// returns pointer to the i-th picoV0Ks
  static StPicoV0*    ks(int i)    { return (StPicoV0*)picoV0Arrays[picoV0Ks]->UncheckedAt(i); }
  /// returns pointer to the i-th picoV0L
  static StPicoV0* lambda(int i)   { return (StPicoV0*)picoV0Arrays[picoV0L]->UncheckedAt(i); }
  /// returns pointer to the i-th picoV0Lbar
  static StPicoV0*   lbar(int i)   { return (StPicoV0*)picoV0Arrays[picoV0Lbar]->UncheckedAt(i); }  

  static unsigned int numberOfTracks() { return picoArrays[picoTrack]->GetEntries(); }
  static unsigned int numberOfKs()     { return picoV0Arrays[picoV0Ks]->GetEntries(); }
  static unsigned int numberOfLambda() { return picoV0Arrays[picoV0L]->GetEntries(); }
  static unsigned int numberOfLbar()   { return picoV0Arrays[picoV0Lbar]->GetEntries(); }
  
  virtual void Print(Option_t *option = "") const; ///< Print basic event info
  static void printTracks();
  static void printKs();
  static void printLambda();
  static void printLbar();
        
  friend class StPicoDstMaker;

  ClassDef(StPicoDst,1)
};

#endif
