#include <vector>
#include <cmath>
#include <algorithm>

#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "StarClassLibrary/StThreeVectorF.hh"
#include "StarClassLibrary/StLorentzVectorF.hh"
#include "StarClassLibrary/StPhysicalHelixD.hh"
#include "StPicoDstMaker/StPicoDst.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoBTofPidTraits.h"

#include "StPicoCharmContainers/StPicoD0Event.h"
#include "StPicoCharmContainers/StKaonPion.h"
#include "StPicoCharmContainers/StPicoD0QaHists.h"
#include "StPicoCharmMakerCuts.h"

#include "StPicoCharmMaker.h"

ClassImp(StPicoCharmMaker)

StPicoCharmMaker::StPicoCharmMaker(char const* makerName, StPicoDstMaker* picoMaker, char const* fileBaseName)
   : StMaker(makerName), mPicoDstMaker(picoMaker), mPicoEvent(nullptr), mPicoD0Hists(nullptr),
     mD0File(nullptr), mD0Tree(nullptr), mPicoD0Event(nullptr) 
{
   TString baseName(fileBaseName);

   baseName.ReplaceAll(".root","");
   mD0File = new TFile(Form("%s.picoD0.root",baseName.Data()), "RECREATE");
   mD0File->SetCompressionLevel(1);
   int BufSize = (int)pow(2., 16.);
   int Split = 1;
   mD0Tree = new TTree("T", "T", BufSize);
   mD0Tree->SetAutoSave(1000000); // autosave every 1 Mbytes
   mPicoD0Event = new StPicoD0Event();
   mD0Tree->Branch("dEvent", "StPicoD0Event", &mPicoD0Event, BufSize, Split);

   mPicoD0Hists = new StPicoD0QaHists(baseName.Data(), charmMakerCuts::prescalesFilesDirectoryName);
}

StPicoCharmMaker::~StPicoCharmMaker()
{
   /* mTree is owned by mD0File directory, it will be destructed once
    * the file is closed in ::Finish() */
   delete mPicoD0Hists;
}

Int_t StPicoCharmMaker::Init()
{
   return kStOK;
}

Int_t StPicoCharmMaker::Finish()
{
   mD0File->cd();
   mD0File->Write();
   mD0File->Close();
   mPicoD0Hists->closeFile();
   return kStOK;
}

void StPicoCharmMaker::Clear(Option_t *opt)
{
   mPicoD0Event->clear("C");
}

Int_t StPicoCharmMaker::Make()
{
   if (!mPicoDstMaker)
   {
      LOG_WARN << " No PicoDstMaker! Skip! " << endm;
      return kStWarn;
   }

   StPicoDst const * picoDst = mPicoDstMaker->picoDst();
   if (!picoDst)
   {
      LOG_WARN << " No PicoDst! Skip! " << endm;
      return kStWarn;
   }

   mPicoEvent = picoDst->event();

   unsigned int nHftTracks = 0;
   
   if (isGoodTrigger() && isGoodEvent())
   {
      UInt_t nTracks = picoDst->numberOfTracks();

      std::vector<unsigned short> idxPicoKaons;
      std::vector<unsigned short> idxPicoPions;

      StThreeVectorF const pVtx = mPicoEvent->primaryVertex();

      for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
      {
         StPicoTrack* trk = picoDst->track(iTrack);
         if(!trk) continue;

         if (!isGoodTrack(trk)) continue;
         ++nHftTracks;

         if (isPion(trk)) idxPicoPions.push_back(iTrack);
         if (isKaon(trk)) idxPicoKaons.push_back(iTrack);

      } // .. end tracks loop

      mPicoD0Event->nKaons(idxPicoKaons.size());
      mPicoD0Event->nPions(idxPicoPions.size());

      float const bField = mPicoEvent->bField();
      for (unsigned short ik = 0; ik < idxPicoKaons.size(); ++ik)
      {
        StPicoTrack const * kaon = picoDst->track(idxPicoKaons[ik]);

        // make Kπ pairs
        for (unsigned short ip = 0; ip < idxPicoPions.size(); ++ip)
        {
          if (idxPicoKaons[ik] == idxPicoPions[ip]) continue;

          StPicoTrack const * pion = picoDst->track(idxPicoPions[ip]);

          StKaonPion kaonPion(*kaon, *pion, idxPicoKaons[ik], idxPicoPions[ip], pVtx, bField);

          if (!isGoodPair(kaonPion)) continue;

          if(isGoodMass(kaonPion)) mPicoD0Event->addKaonPion(kaonPion);

          bool fillMass = isGoodQaPair(kaonPion,*kaon,*pion);
          // bool fillMass = isGoodQaPair(&kaonPion,*kaon,*pion);
          bool unlike = kaon->charge() * pion->charge() < 0 ? true : false;

          if(fillMass || unlike) mPicoD0Hists->addKaonPion(&kaonPion,fillMass, unlike);

        } // .. end make Kπ pairs
      } // .. end of kaons loop
   } //.. end of good event fill

   mPicoD0Event->addPicoEvent(*mPicoEvent);
   mPicoD0Hists->addEvent(*mPicoEvent,*mPicoD0Event,nHftTracks);

   // This should never be inside the good event block
   // because we want to save header information about all events, good or bad
   mD0Tree->Fill();
   mPicoD0Event->clear("C");

   return kStOK;
}

bool StPicoCharmMaker::isGoodEvent() const
{
   return fabs(mPicoEvent->primaryVertex().z()) < charmMakerCuts::vz &&
          fabs(mPicoEvent->primaryVertex().z() - mPicoEvent->vzVpd()) < charmMakerCuts::vzVpdVz;
}

bool StPicoCharmMaker::isGoodTrigger() const
{
  for(auto trg: charmMakerCuts::triggers)
  {
    if(mPicoEvent->isTrigger(trg)) return true;
  }

  return false;
}

bool StPicoCharmMaker::isGoodTrack(StPicoTrack const * const trk) const
{
   return (!charmMakerCuts::requireHFT || trk->isHFTTrack()) && 
          trk->nHitsFit() >= charmMakerCuts::nHitsFit;
}
bool StPicoCharmMaker::isPion(StPicoTrack const * const trk) const
{
   return fabs(trk->nSigmaPion()) < charmMakerCuts::nSigmaPion;
}
bool StPicoCharmMaker::isKaon(StPicoTrack const * const trk) const
{
   return fabs(trk->nSigmaKaon()) < charmMakerCuts::nSigmaKaon;
}
bool StPicoCharmMaker::isGoodPair(StKaonPion const & kp) const
{
   return std::cos(kp.pointingAngle()) > charmMakerCuts::cosTheta &&
          kp.decayLength() > charmMakerCuts::decayLength &&
          kp.dcaDaughters() < charmMakerCuts::dcaDaughters;
}

bool StPicoCharmMaker::isGoodMass(StKaonPion const & kp) const
{
   return kp.m() > charmMakerCuts::minMass && kp.m() < charmMakerCuts::maxMass;
}

int StPicoCharmMaker::getD0PtIndex(StKaonPion const& kp) const
{
   for (int i = 0; i < charmMakerCuts::nPtBins; i++)
   {
      if ((kp.pt() >= charmMakerCuts::PtBinsEdge[i]) && (kp.pt() < charmMakerCuts::PtBinsEdge[i + 1]))
         return i;
   }
   return charmMakerCuts::nPtBins - 1;
}

bool  StPicoCharmMaker::isGoodQaPair(StKaonPion const& kp, StPicoTrack const& kaon,StPicoTrack const& pion)
{
  int tmpIndex = getD0PtIndex(kp);

  return pion.nHitsFit() >= charmMakerCuts::qaNHitsFit && kaon.nHitsFit() >= charmMakerCuts::qaNHitsFit &&
         fabs(kaon.nSigmaKaon()) < charmMakerCuts::qaNSigmaKaon && 
         cos(kp.pointingAngle()) > charmMakerCuts::qaCosTheta[tmpIndex] &&
         kp.pionDca() > charmMakerCuts::qaPDca[tmpIndex] && kp.kaonDca() > charmMakerCuts::qaKDca[tmpIndex] &&
         kp.dcaDaughters() < charmMakerCuts::qaDcaDaughters[tmpIndex] &&
         kp.decayLength() > charmMakerCuts::qaDecayLength[tmpIndex] &&
         fabs(kp.lorentzVector().rapidity()) < charmMakerCuts::qaRapidityCut &&
         ((kp.decayLength()) * sin(kp.pointingAngle())) < charmMakerCuts::qaDcaV0ToPv[tmpIndex];
}
