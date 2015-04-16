
/* **************************************************
 *   Run StPicoHFMyAnaMaker in different modes
 * --------------------------------------------------
 *   
 *  - Different modes to use the  class
 *    - StPicoHFMaker::kAnalyze - don't write candidate trees, just fill histograms
 *        inputFile : fileList of PicoDst files or single picoDst file
 *        outputFile: baseName for outfile 
 *    - StPicoHFMaker::kWrite   - write candidate trees
 *        inputFile : path to single picoDist file
 *        outputFile: baseName for outfile 
 *    - StPicoHFMaker::kRead    - read candidate trees and fill histograms
 *        inputFile : fileList of PicoDst files
 *        outputFile: baseName for outfile 
 *
 * --------------------------------------------------
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

void runPicoHFMyAnaMaker(const Char_t *inputFile="test.list", const Char_t *outputFile="outputBaseName",  unsigned int makerMode = 0 /*kAnalyze*/, 
			 const Char_t * badRunListFileName = "picoList_bad_MB.list") { 
  // -- Check STAR Library. Please set SL_version to the original star library used in the production 
  //    from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL15c";
  string env_SL = getenv ("STAR");
  if (env_SL.find(SL_version)==string::npos) {
      cout<<"Environment Star Library does not match the requested library in runPicoHFMyAnaMaker.C. Exiting..."<<endl;
      exit(1);
  }

  Int_t nEvents = 10000000;
	
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();
  
  gSystem->Load("StPicoDstMaker");
  gSystem->Load("StPicoPrescales");
  gSystem->Load("StPicoHFMaker");
  gSystem->Load("StPicoHFMyAnaMaker");

  chain = new StChain();

  // ========================================================================================
  //makerMode    = StPicoHFMaker::kAnalyze;
  // ========================================================================================
  
  cout << "Maker Mode    " << makerMode << endl;
  cout << "Decay Channel " << decayChannel << endl; 

  TString sInputFile(inputFile);
  TString sInputListHF("");  

  if (makerMode == StPicoHFMaker::kAnalyze) {
    if (!sInputFile.Contains(".list") && !sInputFile.Contains("picoDst.root")) {
      cout << "No input list or picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFMaker::kWrite) {
    if (!sInputFile.Contains("picoDst.root")) {
      cout << "No input picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFMaker::kRead) {
   if (!sInputFile.Contains(".list")) {
      cout << "No input list provided! Exiting..." << endl;
      exit(1);
   }

   // -- prepare filelist for picoDst from hfTrees
   sInputListHF = sInputFile;
   sInputFile = "tmpPico.list";
   TString command = "sed 's|^.*hfTree|/project/projectdirs/starprod/picodsts/Run14/AuAu/200GeV/physics/P15ic|g' "
     + sInputListHF + " > " + sInputFile;
   gSystem->Exec(command.Data());
   command = "sed -i 's|picoHFtree|picoDst|g' " + sInputFile;
   gSystem->Exec(command.Data());
  }
  else {
    cout << "Unknown makerMode! Exiting..." << endl;
    exit(1);
  }
  
  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0, sInputFile, "picoDstMaker");
  StPicoHFMyAnaMaker* picoHFMyAnaMaker = new StPicoHFMyAnaMaker("picoHFMyAnaMaker", picoDstMaker, outputFile, sInputListHF);
  picoHFMyAnaMaker->setMakerMode(makerMode);

  StHFCuts* hfCuts = new StHFCuts("hfBaseCuts");
  picoHFMyAnaMaker->setHFBaseCuts(hfCuts);

  // ---------------------------------------------------
  // -- Set Base cuts for HF analysis

  // -- File name of bad run list
  hfCuts->setBadRunListFileName(badRunListFileName);

  // -- ADD USER CUTS HERE ----------------------------

  // hfCuts->setCutVzMax(6.);
  // hfCuts->setCutVzVpdVzMax(3.);
  // hfCuts->setCutTriggerWord(0x1F);

  // hfCuts->setCutNHitsFitMax(15); 
  // hfCuts->setCutRequireHFT(true);
  // hfCuts->setCutNHitsFitnHitsMax(0.52);
  // ---------------------------------------------------

  // -- Channel1
  picoHFMyAnaMaker->setDecayMode(StPicoHFEvent::kTwoParticleDecay);
  picoHFMyAnaMaker->setDecayChannel(StPicoHFMyAnaMaker::kChannel1);

  // -- ADD USER CUTS HERE ----------------------------


  // ========================================================================================

  // ========================================================================================

  chain->Init();
  cout << "chain->Init();" << endl;
  int total = picoDstMaker->chain()->GetEntries();
  cout << " Total entries = " << total << endl;
  if(nEvents>total) nEvents = total;

  for (Int_t i=0; i<nEvents; i++) {
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

  // -- clean up if in read mode
  if (makerMode == StPicoHFMaker::kRead)
    gSystem->Exec(Form("rm -f %s", sInputFile.Data()));
}

