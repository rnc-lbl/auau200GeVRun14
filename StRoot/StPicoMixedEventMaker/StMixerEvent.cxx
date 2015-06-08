#include "StMixerEvent.h"

#include <limits>
StMixerEvent::StMixerEvent() :  mVtx(StThreeVectorF()), mEventTracks(0),
				mBField(std::numeric_limits<float>::quiet_NaN()),mNoTracks(std::numeric_limits<int>::quiet_NaN())
{
}
StMixerEvent::StMixerEvent(StMixerEvent *t) : mVtx(t->mVtx), mEventTracks(t->mEventTracks),
				mBField(t->mBField), mNoTracks(t->mNoTracks)
{
}
StMixerEvent::StMixerEvent(StThreeVectorF vtx, float b) :  mVtx(StThreeVectorF()), mEventTracks(0),
				mBField(std::numeric_limits<float>::quiet_NaN()),mNoTracks(std::numeric_limits<int>::quiet_NaN())
{
  mVtx = vtx;
  mBField = b;
  
}
void StMixerEvent::addTrack(StMixerTrack *t)
{
  mEventTracks.push_back(*t);
  mNoTracks++;
  return;
}
