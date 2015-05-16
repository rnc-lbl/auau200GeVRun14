#ifndef StElectronTrack_hh
#define StElectronTrack_hh
#ifdef __ROOT__

/* **************************************************
 *  A specialized track class for storing electron index
 *  
 *
 *
 *  Authors:  **Kunsu OH        (kunsuoh@gmail.com)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  **Code Maintainer
 * **************************************************
 */

#include "TObject.h"
#include "TClonesArray.h"

class StPicoTrack;
class StPicoEvent;

class StElectronTrack : public TObject
{
public:
    StElectronTrack();
    StElectronTrack(StElectronTrack const *);
    StElectronTrack(StPicoTrack const * const electron, unsigned short electronIdx);
    ~StElectronTrack() {}// please keep this non-virtual and NEVER inherit from this class
    
    unsigned short   electronIdx() const;	// non-photonic electron candidates idx
    
private:
    // disable copy constructor and assignment operator by making them private (once C++11 is available in STAR you can use delete specifier instead)
    StElectronTrack(StElectronTrack const &);
    StElectronTrack& operator=(StElectronTrack const &);
    
    unsigned short  mElectronIdx; // index of track in StPicoDstEvent

    ClassDef(StElectronTrack,1)
};
inline unsigned short   StElectronTrack::electronIdx() const { return mElectronIdx;}

#endif
#endif

