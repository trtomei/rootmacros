#include "TH1F.h"
#include "TEfficiency.h"

void calculateEfficiency(int npassed, int ntotal) {
  TH1F passed("passed","passed",1,-0.5,0.5);
  TH1F total("total","total",1,-0.5,0.5);
  passed.SetBinContent(1,npassed);
  total.SetBinContent(1,ntotal);
  TEfficiency eff(passed,total);
  printf("Efficiency is %e (+ %e, -%e)\n", eff.GetEfficiency(1),eff.GetEfficiencyErrorUp(1),eff.GetEfficiencyErrorLow(1));
  printf("g->SetPoint(i,m,%g\n",eff.GetEfficiency(1));
  printf("g->SetPointError(i,0,0,%g,%g\n",eff.GetEfficiencyErrorLow(1),eff.GetEfficiencyErrorUp(1));
}

void calculateEfficiency(int npassed, int ntotal, double& efficiency, double& effErrHigh, double& effErrLow) {
  TH1F passed("passed","passed",1,-0.5,0.5);
  TH1F total("total","total",1,-0.5,0.5);
  passed.SetBinContent(1,npassed);
  total.SetBinContent(1,ntotal);
  TEfficiency eff(passed,total);
  
  efficiency = eff.GetEfficiency(1);
  effErrHigh = eff.GetEfficiencyErrorUp(1);
  effErrLow = eff.GetEfficiencyErrorLow(1);
}

double efficiencyCut(const TH1* histo, int bin) {
  int nbinsx = histo->GetNbinsX();
  double total = histo->Integral();
  if(bin<1 || bin>nbinsx) return 0;
  double fraction = histo->Integral(bin,nbinsx)/total;
  return fraction;
}
