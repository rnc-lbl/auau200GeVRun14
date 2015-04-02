#ifndef CUTS_H
#define CUTS_H

/* **************************************************
 *  Cuts namespace.
 *
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)   
 *
 * **************************************************
 */


namespace cuts
{
   // tracking
   extern int const nHitsFit;

   //pions
   extern float const nSigmaPion;

   //kaons
   extern float const nSigmaKaon;

   // kaonPion pair cuts
   extern float const cosTheta;
   extern float const dcaDaughters;
   extern float const decayLength;
   extern float const minMass;
   extern float const maxMass;
}
#endif
