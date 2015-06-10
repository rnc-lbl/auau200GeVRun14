#include "StMixerEvent.h"

#include <limits>
StMixerEvent::StMixerEvent() :  mVtx(StThreeVectorF()),
				mBField(std::numeric_limits<float>::quiet_NaN()),mNoTracks(std::numeric_limits<int>::quiet_NaN())
{
}
StMixerEvent::StMixerEvent(StMixerEvent *t) : mVtx(t->mVtx), mEventKaons(t->mEventKaons), mEventPions(t->mEventPions),
				mBField(t->mBField), mNoTracks(t->mNoTracks)
{
}
StMixerEvent::StMixerEvent(StThreeVectorF vtx, float b) :  mVtx(StThreeVectorF()),
				mBField(std::numeric_limits<float>::quiet_NaN()),mNoTracks(std::numeric_limits<int>::quiet_NaN())
{
  mVtx = vtx;
  mBField = b;
  
}
void StMixerEvent::addPion(StMixerTrack t)
{
  mEventPions.push_back(t);
  mNoTracks++;
  return;
}
void StMixerEvent::addKaon(StMixerTrack t)
{
  mEventKaons.push_back(t);
  mNoTracks++;
  return;
}
