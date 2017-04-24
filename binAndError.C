void binAndError(char* histoName) {
  TH1F* h = (TH1F*)_file0->Get(histoName);
  int nbins = h->GetNbinsX();
  for(int i = 1; i<= nbins; ++i) {
    printf("%g +/- %g\n",h->GetBinContent(i),h->GetBinError(i));
  }
  return;
}
