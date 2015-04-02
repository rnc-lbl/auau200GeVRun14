
/* **************************************************
 *   Run LambdaC Maker in different modes
 *
 *
 *   
 * -- different modes to use the StPicoHFEventMaker class
 *    - StPicoHFEventMaker::kAnalyse - don't write candidate trees, just fill histograms
 *        inputFile : fileList of PicoDst files or single picoDst file
 *        outputFile: baseName for outfile 
 *    - StPicoHFEventMaker::kWrite   - write candidate trees
 *        inputFile : path to single picoDist file
 *        outputFile: baseName for outfile 
 *    - StPicoHFEventMaker::kRead    - read candidate trees and fill histograms
 *        inputFile : fileList of PicoDst files
 *        outputFile: baseName for outfile 
 *
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *            Jochen Thaeder  (jmthader@lbl.gov)
 *
 * **************************************************
 */

#include <TSystem>
#include "TChain.h"

#include "../StPicoHFMaker/StHFCuts.h"



class StMaker;
class StChain;
class StPicoDstMaker;

StChain *chain;



void runPicoHFLambdaCMaker(const Char_t *inputFile="test.file", const Char_t *outputFile="test.root") { 
  //Check STAR Library. Please set SL_version to the original star library used in the production from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL15c";
  string env_SL = getenv ("STAR");
  if (env_SL.find(SL_version)==string::npos) {
      cout<<"Environment Star Library does not match the requested library in runPicoHFLambdaCMaker.C. Exiting..."<<endl;
      exit(1);
  }

  Int_t nEvents = 10000000;
	
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();
  
  gSystem->Load("StPicoDstMaker");
  gSystem->Load("StPicoHFMaker");
  gSystem->Load("StPicoHFLambdaCMaker");

  chain = new StChain();

  // ========================================================================================
  Int_t makerMode = StPicoHFEventMaker::kAnalyse;
  // ========================================================================================
  
  TString sInputFile(inputFile);
  TString sInputListHF("");  

  if (makerMode == StPicoHFEventMaker::kAnalyse) {
    if (!sInputFile.Contains(".list") && !sInputFile.Contains(".root")) {
      cout << "No input list or picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFEventMaker::kWrite) {
    if (!sInputFile.Contains(".root")) {
      cout << "No input picoDst root file provided! Exiting..." << endl;
      exit(1);
    }

    // JMT buildoutfileName
  }
  else if (makerMode == StPicoHFEventMaker::kRead) {
   if (!sInputFile.Contains(".list")) {
      cout << "No input list provided! Exiting..." << endl;
      exit(1);
   }
   
   // JMT get input list for HF trees
   // JMT buildoutfileName - use base path
  }
  else {
    cout << "Unknown makerMode! Exiting..." << endl;
    exit(1);
  }
  
  // ========================================================================================
  Int_t makerMode = StPicoHFEventMaker::kAnalyse;
  // ========================================================================================

  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0, inputFile, "picoDstMaker");
  StPicoHFLambdaCMaker* picoHFLambdaCMaker = new StPicoHFLambdaCMaker("picoHFLambdaCMaker", picoDstMaker, outputFile, sInputListHF);
  picoHFLambdaCMaker->SetMakerMode(makeMode);

  StHFCuts* hfCuts = new StHFCust("lambdaCBaseCuts", "lambdaCBaseCuts");
  picoHFLambdaCMaker->SetHFBaseCuts(hfCuts);

  // ---------------------------------------------------
  // -- Set Base cuts for HF analysis
  hfCuts->SetCutVzMax(6.);
  hfCuts->SetCutVzVpdVzMax(3.);
  hfCuts->SetCutTriggerWord(0x1F);

  hfCuts->SetCutNHitsFitMax(15); 
  hfCuts->SetCutRequireHFT(true);
  hfCuts->SetCutNHitsFitnHitsMax(0.52);
  // ---------------------------------------------------


#if 0
  // -- Lc -> p + Ks0 , Ks0 -> pi+ + pi-
  picoHFLambdaCMaker->SetDecayMode(StPicoHFEvent::kTwoAndTwoParticleDecay);


  hfCuts->SetCutTriplet(0.02, 0.02, 0.02, 0.003, 0., 2.0, 2.5); 

#else
  // -- Lc -> p + K- + pi+
  picoHFLambdaCMaker->SetDecayMode(StPicoHFEvent::kThreeParticleDecay);
  


#endif





  // ========================================================================================

  // ========================================================================================

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

