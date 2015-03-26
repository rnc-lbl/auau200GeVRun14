void runPicoD0AnaMaker(TString d0list,TString outFileName)
{
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();

	gSystem->Load("StPicoDstMaker");
	gSystem->Load("StPicoD0EventMaker");
	gSystem->Load("StPicoD0AnaMaker");

  chain = new StChain();

  // create list of picoDst files
  TString command = "sed 's/hft\\\/d0tree/picodsts/g' "+d0list+" >correspondingPico.list";
  gSystem->Exec(command.Data());
	StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0,"correspondingPico.list","picoDstMaker");
	StPicoD0AnaMaker*  picoD0AnaMaker = new StPicoD0AnaMaker("picoD0AnaMaker",d0list,outFileName.Data(),picoDstMaker);
  
  // delete list of picos
  command = "rm -f correspondingPico.list";
  gSystem->Exec(command.Data());

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
