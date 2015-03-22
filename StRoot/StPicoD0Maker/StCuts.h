#ifndef CUTS_H
#define CUTS_H

namespace cuts
{
	//event
	float const vz = 100.0;
	float const vzVpdVz = 6.0;

	//tracking
	int const nHitsFit = 20;
  bool const requireHFT = true;
	float const nHitsFitnHitsMax = 0.52; // applied in StPicoDstMaker for Run14 data, no need to reapply here

	//pions 
	float const nSigmaPion = 3.0;
	float const pionPt = 0.2;

	//kaons 
	float const nSigmaKaon = 2.0;
	float const kaonPt = 0.2;

  // kaonPion pair cuts
  float const dcaDaughters = 0.0200; // maximum
  float const decayLength = 0.0030; // minimum
  float const minMass = 1.6;
  float const maxMass = 2.1;
}
#endif
