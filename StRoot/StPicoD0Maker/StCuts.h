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
	float const dca = 2.0;

	//pions 
	float const nSigmaPion = 3.0;
	float const pionPt = 0.2;
	float const pionEta = 1.0;

	//kaons 
	float const nSigmaKaon = 2.0;
	float const kaonPt = 0.2;
	float const kaonEta = 1.0;
}
#endif
