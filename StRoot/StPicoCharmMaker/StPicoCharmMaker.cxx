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
#include "phys_constants.h"

#include "StPicoCharmContainers/StPicoD0Event.h"
#include "StPicoCharmContainers/StKaonPion.h"
#include "StPicoCharmContainers/StPicoD0QaHists.h"
#include "StPicoCharmContainers/StPicoKPiXEvent.h"
#include "StPicoCharmContainers/StPicoKPiX.h"

#include "StPicoCharmMakerCuts.h"
#include "StPicoCharmMaker.h"

ClassImp(StPicoCharmMaker)

StPicoCharmMaker::StPicoCharmMaker(char const* makerName, StPicoDstMaker* picoMaker, char const* fileBaseName)
   : StMaker(makerName), mPicoDstMaker(picoMaker), mPicoEvent(nullptr), mPicoD0Hists(nullptr), mBaseName(fileBaseName),
     mD0File(nullptr), mD0Tree(nullptr), mPicoD0Event(nullptr),
     mKPiXFile(nullptr), mKPiXTree(nullptr), mPicoKPiXEvent(nullptr)
{
   mBaseName.ReplaceAll(".root","");
}

StPicoCharmMaker::~StPicoCharmMaker()
{
   /* mTree is owned by mD0File directory, it will be destructed once
    * the file is closed in ::Finish() */
   delete mPicoD0Hists;
}

Int_t StPicoCharmMaker::Init()
{
  int const BufSize = (int)pow(2., 16.);
  int const Split = 1;

  if(mMakeD0)
  {
    mD0File = new TFile(Form("%s.picoD0.root", mBaseName.Data()), "RECREATE");
    mD0File->SetCompressionLevel(1);
    mD0Tree = new TTree("T", "T", BufSize);
    mD0Tree->SetAutoSave(1000000); // autosave every 1 Mbytes
    mPicoD0Event = new StPicoD0Event();
    mD0Tree->Branch("dEvent", "StPicoD0Event", &mPicoD0Event, BufSize, Split);

    mPicoD0Hists = new StPicoD0QaHists(mBaseName.Data(), charmMakerCuts::prescalesFilesDirectoryName);
  }

  if(mMakeKaonPionPion || mMakeKaonPionKaon || mMakeKaonPionProton)
  {
    mKPiXFile = new TFile(Form("%s.picoKPiX.root", mBaseName.Data()), "RECREATE");
    mKPiXFile->SetCompressionLevel(1);
    mKPiXTree = new TTree("KPiXTree", "T", BufSize);
    mKPiXTree->SetAutoSave(1000000); // autosave every 1 Mbytes
    mPicoKPiXEvent = new StPicoKPiXEvent();
    mKPiXTree->Branch("kPiXEvent", "StPicoKPiXEvent", &mPicoKPiXEvent, BufSize, Split);
  }

  return kStOK;
}

Int_t StPicoCharmMaker::Finish()
{
  if(mMakeD0)
  {
    mD0File->Write();
    mD0File->Close();
    mPicoD0Hists->closeFile();
  }

  if(mKPiXFile)
  {
   mKPiXFile->Write();
   mKPiXFile->Close();
  }

   return kStOK;
}

void StPicoCharmMaker::Clear(Option_t *opt)
{
  if(mMakeD0) mPicoD0Event->clear("C");
  if(mKPiXFile) mPicoKPiXEvent->clear("C");
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
      std::vector<unsigned short> idxPicoProtons;

      StThreeVectorF const pVtx = mPicoEvent->primaryVertex();

      for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
      {
         StPicoTrack* trk = picoDst->track(iTrack);
         if(!trk) continue;

         if (!isGoodTrack(*trk, pVtx)) continue;
         ++nHftTracks;

         if (isPion(*trk)) idxPicoPions.push_back(iTrack);
         if (isKaon(*trk)) idxPicoKaons.push_back(iTrack);
         if (isProton(*trk)) idxPicoProtons.push_back(iTrack);

      } // .. end tracks loop

      if(mMakeD0)
      {
        mPicoD0Event->nKaons(idxPicoKaons.size());
        mPicoD0Event->nPions(idxPicoPions.size());
      }

      float const bField = mPicoEvent->bField();
      for (size_t iK0 = 0; iK0 < idxPicoKaons.size(); ++iK0)
      {
        StPicoTrack const* kaon0 = picoDst->track(idxPicoKaons[iK0]);

        for (size_t iPi0 = 0; iPi0 < idxPicoPions.size(); ++iPi0)
        {
          if (idxPicoKaons[iK0] == idxPicoPions[iPi0]) continue;
          StPicoTrack const* pion0 = picoDst->track(idxPicoPions[iPi0]);

          // make Kπ pairs
          StKaonPion kaonPion(*kaon0, *pion0, idxPicoKaons[iK0], idxPicoPions[iPi0], pVtx, bField);

          if (mMakeD0 && isGoodD0Pair(kaonPion))
          {
            if(isGoodD0Mass(kaonPion)) mPicoD0Event->addKaonPion(kaonPion);

            bool const fillMass = isGoodQaPair(kaonPion,*kaon0,*pion0);
            bool const unlike = kaon0->charge() * pion0->charge() < 0 ? true : false;

            if(fillMass || unlike) mPicoD0Hists->addKaonPion(&kaonPion,fillMass, unlike);
          }

          if(kaonPion.dcaDaughters() > charmMakerCuts::dcaDaughters) continue;

          // make Kππ
          if(mMakeKaonPionPion)
          {
            for(size_t iPi1 = iPi0+1; iPi1 < idxPicoPions.size(); ++iPi1)
            {
              if (idxPicoKaons[iK0] == idxPicoPions[iPi1]) continue;
              StPicoTrack const* pion1 = picoDst->track(idxPicoPions[iPi1]);

              StPicoKPiX kaonPionPion(*kaon0, *pion0, *pion1, idxPicoKaons[iK0], idxPicoPions[iPi0], idxPicoPions[iPi1], pVtx, bField);

              if(isGoodKPiX(kaonPionPion) && isGoodKPiXMass(kaonPionPion.fourMom(M_PION_PLUS).m())) mPicoKPiXEvent->addKPiX(kaonPionPion);
            }
          }

          // make KπK
          if(mMakeKaonPionKaon)
          {
            for(size_t iK1 = iK0+1; iK1 < idxPicoKaons.size(); ++iK1)
            {
              if (idxPicoKaons[iK1] == idxPicoPions[iPi0]) continue;
              StPicoTrack const* kaon1 = picoDst->track(idxPicoKaons[iK1]);

              StPicoKPiX kaonPionKaon(*kaon0, *pion0, *kaon1, idxPicoKaons[iK0], idxPicoPions[iPi0], idxPicoKaons[iK1], pVtx, bField);

              if(isGoodKPiX(kaonPionKaon) && isGoodKPiXMass(kaonPionKaon.fourMom(M_KAON_MINUS).m())) mPicoKPiXEvent->addKPiX(kaonPionKaon);
            }
          }

          // make KπP
          if(mMakeKaonPionProton)
          {
            for(size_t iP = 0; iP < idxPicoProtons.size(); ++iP)
            {
              if (idxPicoProtons[iP] == idxPicoPions[iPi0]) continue;
              StPicoTrack const* proton = picoDst->track(idxPicoProtons[iP]);

              StPicoKPiX kaonPionProton(*kaon0, *pion0, *proton, idxPicoKaons[iK0], idxPicoPions[iPi0], idxPicoProtons[iP], pVtx, bField);

              if(isGoodKPiX(kaonPionProton) && isGoodKPiXMass(kaonPionProton.fourMom(M_PROTON).m())) mPicoKPiXEvent->addKPiX(kaonPionProton);
            }
          }
        } // .. end make Kπ pairs
      } // .. end of kaons loop
   } //.. end of good event fill

   if(mMakeD0)
   {
     mPicoD0Event->addPicoEvent(*mPicoEvent);
     mPicoD0Hists->addEvent(*mPicoEvent,*mPicoD0Event,nHftTracks);
     mD0Tree->Fill();
     mPicoD0Event->clear("C");
   }

   if(mKPiXFile)
   {
     mPicoKPiXEvent->addPicoEvent(*mPicoEvent);
     mKPiXTree->Fill();
     mPicoKPiXEvent->clear("C");
   }

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

bool StPicoCharmMaker::isGoodTrack(StPicoTrack const& trk, StThreeVectorF const& pVtx) const
{
   return (!charmMakerCuts::requireHFT || trk.isHFTTrack()) &&
          trk.gPt()      >= charmMakerCuts::minPt  &&
          fabs(trk.dca(pVtx))  >  charmMakerCuts::minDca &&
          trk.nHitsFit() >= charmMakerCuts::nHitsFit &&
          fabs(trk.gMom(pVtx, mPicoEvent->bField()).pseudoRapidity()) <= charmMakerCuts::eta;
}

bool StPicoCharmMaker::isPion(StPicoTrack const& trk) const
{
   return fabs(trk.nSigmaPion()) < charmMakerCuts::nSigmaPion;
}

bool StPicoCharmMaker::isKaon(StPicoTrack const& trk) const
{
   return fabs(trk.nSigmaKaon()) < charmMakerCuts::nSigmaKaon;
}

bool StPicoCharmMaker::isProton(StPicoTrack const& trk) const
{
  return fabs(trk.nSigmaProton()) < charmMakerCuts::nSigmaProton;
}

bool StPicoCharmMaker::isGoodD0Pair(StKaonPion const& kp) const
{
   return std::cos(kp.pointingAngle()) > charmMakerCuts::cosTheta &&
          kp.decayLength() > charmMakerCuts::decayLength &&
          kp.dcaDaughters() < charmMakerCuts::dcaDaughters;
}

bool StPicoCharmMaker::isGoodKPiX(StPicoKPiX const& kpx) const
{
   return std::cos(kpx.pointingAngle()) > charmMakerCuts::cosTheta &&
          kpx.decayLength() > charmMakerCuts::decayLength &&
          kpx.dcaDaughters() < charmMakerCuts::dcaDaughters;
}

bool StPicoCharmMaker::isGoodD0Mass(StKaonPion const& kp) const
{
   return kp.m() > charmMakerCuts::minD0Mass && kp.m() < charmMakerCuts::maxD0Mass;
}

bool StPicoCharmMaker::isGoodKPiXMass(double const mass) const
{
   return mass > charmMakerCuts::minKPiXMass && mass < charmMakerCuts::maxKPiXMass;
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
