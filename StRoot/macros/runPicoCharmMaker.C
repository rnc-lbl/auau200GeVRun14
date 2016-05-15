/* **************************************************
 *
 *  Authors:  **Mustafa Mustafa (mmustafa@lbl.gov)
 *
 *  **Code Maintainer
 *
 * **************************************************
 */


#include <TSystem>

class StMaker;
class StChain;
class StPicoDstMaker;


StChain *chain;
void runPicoCharmMaker(const Char_t *inputFile, const Char_t *outputFile="test.root")
{ 
  //Check STAR Library. Please set SL_version to the original star library used in the production from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL16d";
  string env_SL = getenv ("STAR");
  if(env_SL.find(SL_version)==string::npos)
  {
      cout<<"Environment Star Library does not match the requested library in runPicoCharmMaker.C. Exiting..."<<endl;
      exit(1);
  }

  Int_t nEvents = 1e5;
	
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();

	gSystem->Load("StPicoDstMaker");
  gSystem->Load("StPicoPrescales");

  // KFVertexFitter dependancies
  gSystem->Load("StTpcDb");
  gSystem->Load("StDbUtilities");
  gSystem->Load("Sti");
  gSystem->Load("StiUtilities");
  gSystem->Load("StSsdDbMaker");
  gSystem->Load("StSvtDbMaker");
  gSystem->Load("StiMaker");
  // ---

  gSystem->Load("StPicoCharmContainers");
  gSystem->Load("StPicoCharmMaker");

	chain = new StChain();

	StPicoDstMaker*   picoDstMaker   = new StPicoDstMaker(0,inputFile,"picoDstMaker");
  StPicoCharmMaker* picoCharmMaker = new StPicoCharmMaker("picoCharmEvent",picoDstMaker,outputFile);
  picoCharmMaker->makeD0(false);
  picoCharmMaker->makeKaonPionPion(false);
  picoCharmMaker->makeKaonPionKaon(false);
  picoCharmMaker->makeKaonPionProton(true);

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

	  ++total;
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
