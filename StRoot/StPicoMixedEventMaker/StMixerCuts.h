#ifndef StMixerCuts_H
#define StMixerCuts_H
/* **************************************************
 *  Cuts namespace.
 *
 *  Authors:  Michael Lomnitz (mrlomnitz@lbl.gov)
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
  float const cosTheta = 0.995;
  float const dcaDaughters = 0.005;
  float const massMin = 1.6;
  float const massMax = 2.1;  
  float const decayMin = 0.003;
  float const decayMax = 9999.;
}
#endif
