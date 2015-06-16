#include <limits>

#include "TTree.h"
#include "TH2F.h"

#include "StPicoEventMixer.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDst.h"

#include "StPicoHFMaker/StHFCuts.h"
#include "StPicoMixedEventMaker.h"

#include "StMixerEvent.h"
#include "StMixerPair.h"
#include "StMixerTriplet.h"

StPicoEventMixer::StPicoEventMixer(): 
  mEvents(NULL), mEventsBuffer(std::numeric_limits<int>::min()), filledBuffer(0)
{
  setEventBuffer(3);
  mVtx = new TH2F("bgVtx","Vertex pos;vertex x;vertex y",250,-2.5,2.5,250,-2.5,2.5);
  mFgVtx = new TH2F("fgVtx","Vertex pos;vertex x;vertex y",250,-2.5,2.5,250,-2.5,2.5);
  mForeground = new TH2F("fgMass","Foreground Invariant mass(K#pi);p_{T}(K#pi)(GeV/c),Mass_{K#pi}(GeV/c^{2})",150,0,15,50,1.6,2.1);
  mBackground = new TH2F("bgMass","Mixed Event Invariant mass(K#pi);p_{T}(K#pi)(GeV/c),Mass_{K#pi}(GeV/c^{2})",150,0,15,50,1.6,2.1);
  //int BufSize = (int)pow(2., 16.);
  //ntp_ME = new TNtuple("ntp_ME","MixedEvent Tree","dca1:dca2:dcaDaughters:"		       
  //"theta_hs:decayL_hs:pt_hs:mass_hs:eta_hs:phi_hs:",BufSize);
  /*ntp_ME = new TTree("ntp_ME","MixedEvent",BufSize);
  ntp_ME ->SetAutoSave(1000000);
  ntp_ME->Branch("dca1",&dca1,"dca1/F");
  ntp_ME->Branch("dca1",&dca2,"dca2/F");
  ntp_ME->Branch("dcaDaughters",&dcaDaughters,"dcaDaughters/F");
  ntp_ME->Branch("theta_hs",&theta_hs,"theta_hs/F");
  ntp_ME->Branch("decayL_hs",&decayL_hs,"decayL_hs/F");
  ntp_ME->Branch("pt_hs",&pt_hs,"pt_hs/F");
  ntp_ME->Branch("mass_hs",&mass_hs,"mass_hs/F");
  ntp_ME->Branch("eta_hs",&eta_hs,"eta_hs/F");
  ntp_ME->Branch("phi_hs",&phi_hs,"phi_hs/F");*/
}

void StPicoEventMixer::finish(){
  mVtx->Write();
  mFgVtx->Write();
  mForeground -> Write();
  mBackground -> Write();
  //ntp_ME->Write("anyName",TObject::kSingleKey);
  //ntp_ME->Write();
  return;
}
bool StPicoEventMixer::addPicoEvent(StPicoDst const* const picoDst, StHFCuts const* const mHFCuts)
{
  int nTracks = picoDst->numberOfTracks();
  StThreeVectorF pVertex = picoDst->event()->primaryVertex();
  StMixerEvent* event = new StMixerEvent(pVertex, picoDst->event()->bField());

  bool isTpcPi = false;
  bool isTofPi = false;
  bool isTpcK = false;
  bool isTofK = false;
  //Event.setNoTracks( nTracks );
  for( int iTrk = 0; iTrk < nTracks; ++iTrk){
    StPicoTrack const* trk = picoDst->track(iTrk);
    if( !mHFCuts->isGoodTrack(trk)  || isCloseTrack(*trk,pVertex)) continue;
    if( mHFCuts->isTPCPion(trk)){
      isTpcPi = true;
      isTofPi = true;
      isTpcK = false;
      isTofK = false;
      StMixerTrack mTrack(pVertex, picoDst->event()->bField(), *trk, isTpcPi, isTofPi, isTpcK, isTofK);
      event->addPion(mTrack);      
    }
    if( mHFCuts->isTPCKaon(trk)){
      isTpcPi = false;
      isTofPi = false;
      isTpcK = true;
      isTofK = true;
      StMixerTrack mTrack(pVertex, picoDst->event()->bField(), *trk, isTpcPi, isTofPi, isTpcK, isTofK);
      event->addKaon(mTrack);
    }
  } 
  if ( event->getNoPions() > 0 ||  event->getNoKaons() > 0){
    mEvents.push_back(event);
    filledBuffer+=1;
  }
  else {
    delete event;
    return false;
  }
  //Returns true if need to do mixing, false if buffer has space still
  if ( filledBuffer == mEventsBuffer - 1 )
    return true;
  return false;
}  
void StPicoEventMixer::mixEvents(StHFCuts *mHFCuts){

  //cout<<"Mixing events"<<endl;
  //-------
  size_t const nEvent = mEvents.size();
  int const nTracksEvt1 = mEvents.at(0)->getNoPions();
  //Template for D0 studies
  for( size_t iEvt2 = 0; iEvt2 < nEvent; iEvt2++){
    int const nTracksEvt2 = mEvents.at(iEvt2)->getNoKaons();
    if( iEvt2 == 0 )
      mFgVtx->Fill(mEvents.at(0)->vertex().x(),mEvents.at(0)->vertex().y());
    else
      mVtx->Fill(mEvents.at(0)->vertex().x(),mEvents.at(0)->vertex().y());
    for( int iTrk2 = 0; iTrk2 < nTracksEvt2; iTrk2++){
      
      for( int iTrk1 = 0; iTrk1 < nTracksEvt1; iTrk1++){

	if( mEvents.at(0)->pionAt(iTrk1).charge() == mEvents.at(iEvt2)->kaonAt(iTrk2).charge() ) 
	  continue;
	StMixerPair pair(mEvents.at(0)->pionAt(iTrk1), mEvents.at(iEvt2)->kaonAt(iTrk2),
					    StHFCuts::kPion, StHFCuts::kKaon,
					    mEvents.at(0)->vertex(), mEvents.at(iEvt2)->vertex(),
					    mEvents.at(0)->field() );
	if( mHFCuts->isGoodMixerPair(&pair) ){
	  if(iEvt2 == 0)
	    fillFG(&pair);
	  else
	    fill(&pair);
	}
      } //second event track loop
    } //first event track loop 
  } //loop over second events
  --filledBuffer;
  delete mEvents.at(0)	;
  mEvents.erase(mEvents.begin());
  return;
} 
// _________________________________________________________
bool StPicoEventMixer::isMixerPion(StMixerTrack const& track){
  short info = track.getTrackInfo();
  //TPC pion
  if( (info & 2) >> 1 != 1) return false;
  //TOF pion
  if( (info & 4) >> 2 != 1) return false;
  return true;
}
// _________________________________________________________
bool StPicoEventMixer::isMixerKaon(StMixerTrack const& track){
  short info = track.getTrackInfo();
  //TPC Kaon
  if( (info & 8) >> 3 != 1) return false;
  //TOF Kaon
  if( (info & 16) >> 4 != 1) return false;
  return true;
}
// _________________________________________________________
void StPicoEventMixer::fill(StMixerPair const* const pair){
  mBackground -> Fill(pair->pt(),pair->m());
  
  /*  dca1 = pair->particle1Dca();
  dca2 = pair->particle1Dca();
  dcaDaughters = pair->dcaDaughters();
  theta_hs = pair->pointingAngle();
  decayL_hs = pair->decayLength();
  pt_hs = pair->pt();
  mass_hs = pair->m();
  eta_hs = pair->eta();
  phi_hs = pair->phi();
  ntp_ME->Fill();
  */
  return;
}
// _________________________________________________________
void StPicoEventMixer::fillFG(StMixerPair const* const pair){
  mForeground -> Fill(pair->pt(),pair->m());
  return;
}

bool StPicoEventMixer::isCloseTrack(StPicoTrack const& trk, StThreeVectorF const& pVtx){
  StPhysicalHelixD helix = trk.dcaGeometry().helix();
  helix.moveOrigin(helix.pathLength(pVtx));
  if( (helix.origin()-pVtx).mag() > 0.008 ) return false;
  return true;
}
