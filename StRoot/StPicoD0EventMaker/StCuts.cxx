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

#include <string>

namespace cuts
{
   // path to lists of triggers prescales
   // lists are obtained from http://www.star.bnl.gov/protected/common/common2014/trigger2014/plots_au200gev/
   prescalesFilesDirectoryName = "./run14AuAu200GeVPrescales";
   //event
   StriggerWord = 0x1F; //first five bits see http://rnc.lbl.gov/~xdong/SoftHadron/picoDst.html
   vz = 6.0;// cm.
   vzVpdVz = 3.0; // 3 cm.

   //tracking
   nHitsFit = 15;
   requireHFT = true;

   //pions
   nSigmaPion = 3.0;

   //kaons
   nSigmaKaon = 2.5;

   // kaonPion pair cuts
   cosTheta = 0; // minimum
   dcaDaughters = 0.0200; // maximum
   decayLength = 0.0030; // minimum
   minMass = 1.6;
   maxMass = 2.1;
}
