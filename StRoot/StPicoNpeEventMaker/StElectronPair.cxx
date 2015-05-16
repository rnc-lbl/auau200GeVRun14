#include <limits>
#include <cmath>

#ifdef __ROOT__
#include "StElectronPair.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "SystemOfUnits.h"
#include "StPicoDstMaker/StPicoTrack.h"

ClassImp(StElectronPair)


StElectronPair::StElectronPair():
mElectronIdx(std::numeric_limits<unsigned short>::quiet_NaN()), mPartnerIdx(std::numeric_limits<unsigned short>::quiet_NaN()),
mMass(std::numeric_limits<unsigned short>::quiet_NaN()),
mPairDca(std::numeric_limits<float>::quiet_NaN()),
mPositionX(std::numeric_limits<short>::quiet_NaN()),
mPositionY(std::numeric_limits<short>::quiet_NaN()),
mPositionZ(std::numeric_limits<short>::quiet_NaN())
{
}
//------------------------------------
StElectronPair::StElectronPair(StElectronPair const * t) :
mElectronIdx(t->mElectronIdx), mPartnerIdx(t->mPartnerIdx),
mMass(t->mMass),
mPairDca(t->mPairDca),
mPositionX(t->mPositionX),
mPositionY(t->mPositionY),
mPositionZ(t->mPositionZ)
{
}
//------------------------------------
StElectronPair::StElectronPair(StPicoTrack const * const electron, StPicoTrack const * const partner,
                               unsigned short const electronIdx, unsigned short const partnerIdx,
                               StThreeVectorF const & vtx, float const bField) :
mElectronIdx(electronIdx), mPartnerIdx(partnerIdx),
mMass(std::numeric_limits<unsigned short>::quiet_NaN()),
mPairDca(std::numeric_limits<float>::quiet_NaN()),
mPositionX(std::numeric_limits<short>::quiet_NaN()),
mPositionY(std::numeric_limits<short>::quiet_NaN()),
mPositionZ(std::numeric_limits<short>::quiet_NaN())
{
    if ((!electron || !partner) || (electron->id() == partner->id()))
    {
        mElectronIdx = std::numeric_limits<unsigned short>::quiet_NaN();
        mPartnerIdx = std::numeric_limits<unsigned short>::quiet_NaN();
        return;
    }
    
    
    // to be used for testing with preview II pico production
    StPhysicalHelixD electronHelix = electron->dcaGeometry().helix();
    StPhysicalHelixD partnerHelix = partner->dcaGeometry().helix();
    
    // move origins of helices to the primary vertex origin
    //    electronHelix.moveOrigin(electronHelix.pathLength(vtx));
    //    partnerHelix.moveOrigin(partnerHelix.pathLength(vtx));
    
    // use straight lines approximation to get point of DCA of electron-partner pair
    //    StThreeVectorF const electronMom = electronHelix.momentum(bField * kilogauss);
    //    StThreeVectorF const partnerMom = partnerHelix.momentum(bField * kilogauss);
    //    StPhysicalHelixD const kStraightLine(electronMom, electronHelix.origin(), 0, electron->charge());
    //    StPhysicalHelixD const pStraightLine(partnerMom, partnerHelix.origin(), 0, partner->charge());
    
    //    pair<double, double> const ss = kStraightLine.pathLengths(pStraightLine);
    //    StThreeVectorF const kAtDcaTopartner = kStraightLine.at(ss.first);
    //    StThreeVectorF const pAtDcaToelectron = pStraightLine.at(ss.second);
    
    // normal method
    pair<double,double> ss = electronHelix.pathLengths(partnerHelix);
    StThreeVectorD kAtDcaToPartner = electronHelix.at(ss.first);
    StThreeVectorD pAtDcaToElectron = partnerHelix.at(ss.second);
    
    // calculate DCA of partner to electron at their DCA
    StThreeVectorD VectorDca = kAtDcaToPartner - pAtDcaToElectron;
    mPairDca = (float)VectorDca.mag();
    
    // calculate Lorentz vector of electron-partner pair
    StThreeVectorF const electronMomAtDca = electronHelix.momentumAt(ss.first, bField * kilogauss);
    StThreeVectorF const partnerMomAtDca = partnerHelix.momentumAt(ss.second, bField * kilogauss);
    
    StLorentzVectorF const electronFourMom(electronMomAtDca, electronMomAtDca.massHypothesis(M_ELECTRON));
    StLorentzVectorF const partnerFourMom(partnerMomAtDca, partnerMomAtDca.massHypothesis(M_ELECTRON));
    StLorentzVectorF const epairFourMom = electronFourMom + partnerFourMom;
    mMass = (unsigned short)(epairFourMom.m()*1000);
    
    StThreeVectorD Position = kAtDcaToPartner + pAtDcaToElectron;
    mPositionX = (short)(Position.x()/2*100.0);
    mPositionY = (short)(Position.y()/2*100.0);
    mPositionZ = (short)(Position.z()/2*100.0);
    
}
#endif // __ROOT__
