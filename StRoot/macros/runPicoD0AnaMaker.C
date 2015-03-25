void runPicoD0AnaMaker(TString d0list,TString picolist,TString outFileName)
{
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();

	gSystem->Load("StPicoDstMaker");
	gSystem->Load("StPicoD0Maker");
	gSystem->Load("StPicoD0AnaMaker");

  chain = new StChain();
	StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0,picolist,"picoDstMaker");
	StPicoD0AnaMaker*  picoD0AnaMaker = new StPicoD0AnaMaker("picoD0AnaMaker",d0list,outFileName.Data(),picoDstMaker);

  chain->Init();
  int nEntries = picoD0AnaMaker->getEntries();
  for(int iEvent = 0; iEvent<nEntries; ++iEvent)
  {
    chain->Clear();
    chain->Make();
    int iret = chain->Make();
	  if (iret) { cout << "Bad return code!" << iret << endl; break;}
  }

  chain->Finish();
  delete chain;
}
