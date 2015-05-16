#ifndef CUTS_H
#define CUTS_H

/* **************************************************
 *  Cuts namespace.
 *
 *  Authors:  Kunsu OH        (kunsuoh@gmail.com)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *
 * **************************************************
 */

#include "Rtypes.h"
#include <string>

namespace cuts
{
    extern std::string const prescalesFilesDirectoryName;
    //event
    extern float const vz;
    extern float const vzVpdVz;
    
    //tracking
    extern int const nHitsFit;
    extern bool const requireHFT;
    
    // electrons
    extern float const pt;
    extern float const nSigmaElectron;

    // partner
    extern float const nSigmaPartnerElectron;
    
    // tree electron pair cuts
    extern float const pairMass;
    extern float const pairMassHigh;
    extern float const pairHighPt;
    extern float const pairDca;
    extern float const positionX;
    extern float const positionY;
    extern float const positionZ;
    
    // histograms electron pair cuts
    extern float const qaNHitsFit;
    extern float const qaNSigmaElectronMax;
    extern float const qaNSigmaElectronMin;
    extern float const qaPairDca;
    extern float const qaPairMass;
}
#endif
