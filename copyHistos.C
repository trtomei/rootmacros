void copyHistos(TString oldFileName, TString newFileName, TString regex, bool match=true){
  TFile *f1 = TFile::Open(oldFileName.Data());
  TFile *f2 = TFile::Open(newFileName.Data(), "RECREATE");
  TIter next(f1->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    TClass *cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TH1")) continue;
    TH1 *h = (TH1*)key->ReadObj();
    TPRegexp pr(regex);
    if(match) {
      if(pr.MatchB(h->GetName())) {
	std::cout << "Histogram " << h->GetName() << " MATCHED." << std::endl;
	TString hname = h->GetName();
	hname.ReplaceAll(".lhe.hepmc.root","").ReplaceAll("i2hdm_mh1scan_h1h1j_13tev_q_ptj","CMS_EXO_16_013_");
	h->SetName(hname.Data());
	h->Write();
      }
      else {
	std::cout << "Histogram " << h->GetName() << " DID NOT match." << std::endl;
      }
    }// Close if negate
    else {
      if(!pr.MatchB(h->GetName())) {
	std::cout << "Histogram " << h->GetName() << " DID NOT match (copy)." << std::endl;
	TString hname = h->GetName();
	hname.ReplaceAll(".lhe.hepmc.root","").ReplaceAll("i2hdm_mh1scan_h1h1j_13tev_q_ptj","CMS_EXO_12_048_");
	h->SetName(hname.Data());
	h->Write();
      }
      else {
	std::cout << "Histogram " << h->GetName() << " MATCHED (no copy)." << std::endl;
      }
    }// Close else
  } // Close while
  f2->Close();
  f1->Close();
}
