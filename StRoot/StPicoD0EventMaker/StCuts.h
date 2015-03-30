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
   //event
   UShort_t triggerWord = 0x1F; //first five bits see http://rnc.lbl.gov/~xdong/SoftHadron/picoDst.html
   float const vz = 6.0;// cm.
   float const vzVpdVz = 3.0; // 3 cm.

   //tracking
   int const nHitsFit = 15;
   bool const requireHFT = true;

   //pions
   float const nSigmaPion = 3.0;

   //kaons
   float const nSigmaKaon = 2.5;

   // kaonPion pair cuts
   float const cosTheta = 0; // minimum
   float const dcaDaughters = 0.0200; // maximum
   float const decayLength = 0.0030; // minimum
   float const minMass = 1.6;
   float const maxMass = 2.1;
}
#endif
