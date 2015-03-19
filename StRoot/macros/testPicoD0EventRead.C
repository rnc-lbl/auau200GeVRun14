void testPicoD0EventRead(TString filename)
{
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();

	gSystem->Load("StPicoDstMaker");
	gSystem->Load("StPicoD0Maker");

	TFile* f = new TFile(filename.Data());
	TTree* T = (TTree*)f->Get("T");
	StPicoD0Event* event = new StPicoD0Event();
	T->SetBranchAddress("dEvent",&event);

	// TFile ff("read_test.root","RECREATE");

	StKaonPion* pp = 0;

	for(Int_t i=0;i<100000;i++)
	{
		T->GetEntry(i);

		TClonesArray* arrKPi = event->kaonPion();

		for(int idx=0;idx<event->nKaonPion();++idx)
		{
			pp = (StKaonPion*)arrKPi->At(idx);

      cout<<pp->m()<<endl;
		}
	}

	// ff.Close();
}
