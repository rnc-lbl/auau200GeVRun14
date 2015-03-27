#ifndef CUTS_H
#define CUTS_H

namespace cuts
{
   // tracking
   int nHitsFit = 20;

   //pions
   float const nSigmaPion = 3.0;

   //kaons
   float const nSigmaKaon = 2.0;

   // kaonPion pair cuts
   float const cosTheta = 0.90; // minimum
   float const dcaDaughters = 0.008; // maximum
   float const decayLength = 0.0030; // minimum
   float const minMass = 1.6;
   float const maxMass = 2.1;
}
#endif
