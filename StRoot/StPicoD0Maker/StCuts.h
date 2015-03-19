#ifndef CUTS_H
#define CUTS_H

// pay attention that all the pairs invariant mass cuts are applied in the code
namespace cuts
{
	//event
	const float vz = 100.0;
	const float vzVpdVz = 6.0;

	//tracking
	const int nHitsFit = 15;
	const float nHitsFitnHitsMax = 0.52;
	const float dca = 2.0; // this is not applied for global tracks, of course :-)

	// hadrons
	const float hadronsGlobalPt = 0.2;
	const float hadronsDca = 3.0;

	//pions 
	const float nSigmaPion = 3.0;
	const float pionPt = 0.15; // this is used for soft pions, for other pions 0.2 is used, see code
	const float pionEta = 1.0; // this is used for all but soft pions

	//kaons 
	const float kaonPt = 0.2;
	const float kaonEta = 1.0;
	const float nSigmaKaon = 3.0;

	// electrons
	const float electronPt = 0.2;
	const float electronEta = 1.0;
	const float nSigmaElectronHigh = 5.0;
	const float nSigmaElectronLow = -5.0;
	const float electronsBeta = 0.035; // for tracks with pT< 1.0

	// partner electron cuts
	const float partnerElecNSigmaElectron = 5.0;
	const float partnerElecGlobalPt = 0.2;
}
#endif
