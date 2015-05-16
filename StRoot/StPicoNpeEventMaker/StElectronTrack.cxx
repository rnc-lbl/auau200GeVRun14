#include <limits>
#include <cmath>

#ifdef __ROOT__
#include "StElectronTrack.h"

#include "StPicoDstMaker/StPicoTrack.h"

ClassImp(StElectronTrack)


StElectronTrack::StElectronTrack():
mElectronIdx(std::numeric_limits<unsigned short>::quiet_NaN())
{
}
//------------------------------------
StElectronTrack::StElectronTrack(StElectronTrack const * t) :
mElectronIdx(t->mElectronIdx)
{
}
//------------------------------------
StElectronTrack::StElectronTrack(StPicoTrack const * const electron, unsigned short const electronIdx) :
mElectronIdx(electronIdx)
{
    if (!electron)
    {
        mElectronIdx = std::numeric_limits<unsigned short>::quiet_NaN();
        return;
    }

}
#endif // __ROOT__
