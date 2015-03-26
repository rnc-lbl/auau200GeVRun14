#include <TSystem>

#include "../StPicoHFMaker/StHFCuts.h"

class StMaker;
class StChain;
class StPicoDstMaker;

StChain *chain;

void runPicoHFLambdaCMaker(const Char_t *inputFile="test.list", const Char_t *outputFile="test.root")
{ 
  //Check STAR Library. Please set SL_version to the original star library used in the production from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL15c";
  string env_SL = getenv ("STAR");
  if(env_SL.find(SL_version)==string::npos)
  {
      cout<<"Environment Star Library does not match the requested library in runPicoD0EventMaker.C. Exiting..."<<endl;
      exit(1);
  }

  Int_t nEvents = 10000000;
	
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();
  
  gSystem->Load("StPicoDstMaker");
  gSystem->Load("StPicoHFMaker");
  gSystem->Load("StPicoHFLambdaCMaker");

  chain = new StChain();
  
  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0,inputFile,"picoDstMaker");
  StPicoHFLambdaCMaker* picoHFLambdaCMaker = new StPicoHFLambdaCMaker("picoHFLambdaCMaker", picoDstMaker, outputFile);
  StHFCuts* hfCuts = StHFCuts::Instance();

  // -- use secondary decay
  picoHFLambdaCMaker->SetDecayMode(StPicoHFEvent::secondaryPair);
  
  // -- use triplet decay
  //  picoHFLambdaCMaker->SetDecayMode(StPicoHFEvent::triplet);
  
  // -- write trees
  picoHFLambdaCMaker->SetMakerMode(StPicoHFEventMaker::write);

  // ---------------------------------------------------
  // -- Set Base cuts for HF analysis
  hfCuts->SetCutVzMax(20.);
  hfCuts->SetCutVzVpdVzMax(10.);
  
  hfCuts->SetCutNHitsFitMax(15); 
  hfCuts->SetCutRequireHFT(true);
  hfCuts->SetCutNHitsFitnHitsMax(0.52);
  // ---------------------------------------------------
  picoHFLambdaCMaker->SetHFBaseCuts(hfCuts);

  chain->Init();
  cout<<"chain->Init();"<<endl;
  int total = picoDstMaker->chain()->GetEntries();
  cout << " Total entries = " << total << endl;
  if(nEvents>total) nEvents = total;

  for (Int_t i=0; i<nEvents; i++)
    {
      if(i%10000==0)
	cout << "Working on eventNumber " << i << endl;
      
      chain->Clear();
      int iret = chain->Make(i);
      
      if (iret) { cout << "Bad return code!" << iret << endl; break;}
      
      total++;
    }
  
  cout << "****************************************** " << endl;
  cout << "Work done... now its time to close up shop!"<< endl;
  cout << "****************************************** " << endl;
  chain->Finish();
  cout << "****************************************** " << endl;
  cout << "total number of events  " << nEvents << endl;
  cout << "****************************************** " << endl;
  
  delete chain;
}

