#ifndef StMixerCuts_H
#define StMixerCuts_H
/* **************************************************
 * 
 * Cuts namespace.
 *
 * **************************************************
 *
 *  Initial Authors:  
 *         ** Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  ** Code Maintainer
 *
 * **************************************************
 */

#include "Rtypes.h"
#include <string>

#include "phys_constants.h"
#include "SystemOfUnits.h"

#include "StMixerCuts.h"

namespace mxeCuts
{
  enum mePid {kKaon, kPion, kProton};
  float const pidMass[3] = { M_KAON_PLUS, M_PION_PLUS, M_PROTON};
  
  //Event
  UShort_t const triggerWord = 0x1F; //first five bits see http://rnc.lbl.gov/~xdong/SoftHadron/picoDst.html
  float const maxVz = 6.0;// cm.
  float const vzVpdVz = 3.0; // 3 cm.
  float const Verror = 1.0e-5; // 
  float const Vrcut = 2.0; // 
  
  //Tracking
  float const minPt = 1.2;
  float const nHitsFit = 20;
  float const nHitsFit_nHitsPos = 0.52;
  float const dca2pVtx = 0.008;
  bool const mRequireHft = true;
  //PID
  // pions
  float const nSigmaPion = 3.0;
  float const pTofBetaDiff = 0.03;
  // kaons
  float const nSigmaKaon = 2.0;
  float const kTofBetaDiff = 0.03;
  
  //Topology
  float const massMin = 0;
  float const massMax = 2.5;
  int const massBins = 250;
  int   const nPtBins = 5;
  float const PtEdge[nPtBins+1] = {0., 1., 2., 3., 5., 10.};
  //Ultimate1
  //float const dcaV0ToPv[nPtBins] = {0.0062, 0.0047, 0.0040, 0.0041, 0.0042};
  //float const decayLength[nPtBins] = {0.0149, 0.0205, 0.0216, 0.0233, 0.0282};
  //float const cosTheta[nPtBins] = {0.0000, 0.0000, 0.0000, 0.0000, 0.0000};//0.995
  //float const dcaDaughters[nPtBins] = {0.0082, 0.0070, 0.0056, 0.0065, 0.0065}; //0.0050;
  //float const kDca[nPtBins] = {0.0123, 0.0097, 0.0091, 0.0075, 0.0053};//0.008, // minimum
  //float const pDca[nPtBins] = {0.0109, 0.0108, 0.0100, 0.0074, 0.0067};//0.008
  //Ultimate 2
  float const dcaV0ToPv[nPtBins] = {0.0061, 0.0049, 0.0038, 0.0038, 0.0040};
  float const decayLength[nPtBins] = {0.0145, 0.0181, 0.0212, 0.0247, 0.0259};
  float const cosTheta[nPtBins] = {0.0000, 0.0000, 0.0000, 0.0000, 0.0000};//0.995
  float const dcaDaughters[nPtBins] = {0.0084, 0.0066, 0.0057, 0.0050, 0.0060}; //0.0050;
  float const kDca[nPtBins] = {0.0103, 0.0091, 0.0095, 0.0079, 0.0058};//0.008, // minimum
  float const pDca[nPtBins] = {0.0110, 0.0111, 0.0086, 0.0081, 0.0062};//0.008
}
#endif
