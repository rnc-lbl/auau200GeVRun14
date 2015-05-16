#ifndef CUTS_H
#define CUTS_H

/* **************************************************
 *  Cuts namespace.
 *
 *  Authors:  **Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  **Code Maintainer
 *
 * **************************************************
 */

#include "Rtypes.h"
#include <string>

namespace cuts
{
   extern std::string const prescalesFilesDirectoryName;
   //event
   extern UShort_t const triggerWord;
   extern float const vz;
   extern float const vzVpdVz;

   //tracking
   extern int const nHitsFit;
   extern bool const requireHFT;

   //pions
   extern float const nSigmaPion;

   //kaons
   extern float const nSigmaKaon;

   // tree kaonPion pair cuts
   extern float const cosTheta;
   extern float const dcaDaughters;
   extern float const decayLength;
   extern float const minMass;
   extern float const maxMass;

   // histograms kaonPion pair cuts
   extern float const qaNHitsFit;
   extern float const qaNSigmaKaon;
   extern float const qaCosTheta;
   extern float const qaDcaDaughters;
   extern float const qaKDca;
   extern float const qaPDca;
}
#endif
