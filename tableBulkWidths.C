// This macro implements Eq. 10 of hep-ph/0701186
// for the width of the bulk graviton
// Run it in ROOT with .x tableBulkWidths

double bulkWidth(double c, double mass) {
  double pi = TMath::Pi();
  double x1 = 3.83;
  double cx1 = c*x1;
  double width = 13*cx1*cx1*mass/(960*pi);
  return width;
}

double tableBulkWidths(){
  double C[3] = {0.01,0.05,0.2};
  double M[3] = {600,1000,1500};
  printf("\nMasses and widths in GeV\n");
  for (int i=0;i!=3;++i) {
    for(int j=0;j!=3;++j) {
      double c = C[i];
      double mass = M[j];
      double width = bulkWidth(c,mass);
      printf("c = %g, mass = %g, width = %2.4f\n",c,mass,width);
    }
    printf("\n");
  }
}
