#ifndef StMixerEvent_hh
#define StMixerEvent_hh
#include <math.h>
#include <vector>
#include "StThreeVectorF.hh"
#include "StMixerTrack.h"
/* **************************************************
 *
 * Event class used for mixed event buffer, stripped down 
 * to minimum information neede to reconstruct the helix
 * and basic track information. Currently include:
 * 1) primVtx
 * 2) B-Field
 * 3) MixerTrack array
 *
 * **************************************************
 *
 *  Initial Authors:  
 *         ** Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  ** Code Maintainer
 *
 * **************************************************
 */

class StMixerTrack;

class StMixerEvent{
 public:
  StMixerEvent();
  StMixerEvent(StMixerEvent *);
  StMixerEvent(StThreeVectorF, float);
  ~StMixerEvent(){;};
  void addTrack(StMixerTrack *);
  void setPos( float const, float const, float const);
  void setField( float const );
  void setNoTracks( int const );
  int getNoTracks();
  StMixerTrack trackAt(int const); 
  StThreeVectorF const & vertex() const;
  double const field() const;
 private:
  StThreeVectorF mVtx;
  std::vector <StMixerTrack  > mEventTracks;
  float mBField;
  int mNoTracks;
};
inline void StMixerEvent::setPos( float const vx, float const vy, float const vz){
  mVtx = StThreeVectorF(vx, vy, vz);
}
inline void StMixerEvent::setField( float const field ){ mBField = field; }
inline void StMixerEvent::setNoTracks( int const noTracks ){ mNoTracks = noTracks; }
inline int StMixerEvent::getNoTracks(){ return mEventTracks.size(); }
inline StMixerTrack StMixerEvent::trackAt(int const counter) { return(mEventTracks.at(counter));} 
inline StThreeVectorF const & StMixerEvent::vertex() const { return mVtx; }
inline double const StMixerEvent::field() const {return mBField; }
#endif
