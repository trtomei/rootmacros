TGraphAsymmErrors* stripXerrors(TGraphAsymmErrors* gin) {
  TGraphAsymmErrors* gout = (TGraphAsymmErrors*)gin->Clone("clone");
  int npoints = gin->GetN();
  for(int i=0; i!=npoints; ++i) {
    double eyhigh = gin->GetErrorYhigh(i);
    double eylow = gin->GetErrorYlow(i);
    gout->SetPointError(i,0,0,eyhigh,eylow);
  }

  return gout;
}
