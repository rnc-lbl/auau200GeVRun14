#ifndef StPicoEventMixer_hh
#define StPicoEventMixer_hh

/* **************************************************
 * Class stores event buffer used in event mixing. Mixing
 * is done automatically once buffer reaches defined maximum.
 * User should rpesonalize mixEvent() method to cosntruct 
 * desired background.
 *
 * **************************************************
 * 
 * Initial Authors:
 *          **Michael Lomnitz (mrlomnitz@lbl.gov)
 *          Musta Mustafa   (mmustafa@lbl.gov)
 *
 *  ** Code maintainer 
 *
 * **************************************************
 */

#include <vector>
#include <limits>
#include <iostream>
#include "StMixerEvent.h"
#include "StMixerTrack.h"
#include "StMixerPair.h" 
#include "TH1.h"
#include "TTree.h"
#include "StMemStat.h"

class StPicoEvent;
class StPicoTrack;
class StPicoDst;
class StMixerTrack;
class StMixerEvent;
class StMixerPair;

class StHFCuts;

class StPicoEventMixer {
 public: 
  StPicoEventMixer();
  ~StPicoEventMixer(){;};
  bool addPicoEvent(const StPicoDst *picoDst, StHFCuts *mHFCuts);
  void setEventBuffer(int buffer);
  void mixEvents(StHFCuts *mHFCuts);
  void InitMixedEvent();
  void FinishMixedEvent();
 private:
  void fill(StMixerPair const * const);
  bool isMixerPion(StMixerTrack);
  bool isMixerKaon(StMixerTrack);
  //TH1F * mBackground;
  TTree * ntp_ME;
  std::vector < StMixerEvent *> mEvents; //Needs to be generalized to more categories Using TCLones array
  StMemStat mem;
  unsigned short int mEventsBuffer; 
  unsigned short int filledBuffer;
  float dca1, dca2, dcaDaughters, theta_hs, decayL_hs;
  float pt_hs, mass_hs, eta_hs, phi_hs;

};

inline void StPicoEventMixer::setEventBuffer(int buffer){ mEventsBuffer = buffer;}
			    
    
#endif
