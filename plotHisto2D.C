#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <utility>
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

void plotHisto2D(string fname, string hname, double xmax=200.0, double ymax=200.0) {

  using namespace std;

  // Before everything, set gStyle
  //gStyle->SetPalette(1);
  gStyle->SetOptStat(1111);
  gStyle->SetStatX(0.9);
  gStyle->SetStatY(0.9);
  gStyle->SetStatW(0.15);
  gStyle->SetStatH(0.15);
  
  // Open the plot, and plot it.
  char name[256];
  sprintf(name, "%s", fname.c_str());
  
  TFile* f = TFile::Open(name);

  sprintf(name, "%s", hname.c_str());
  TH2D* massJet = (TH2D*) f->Get(name);
  TCanvas* theCanvas = new TCanvas();
  massJet->Draw();
  
  // The "look beautiful" part...
  massJet->GetXaxis()->SetRangeUser(0,xmax);
//  massJet->GetXaxis()->SetTitle("mass 1^{st} jet (GeV)");
  massJet->GetYaxis()->SetRangeUser(0,ymax);
//  massJet->GetYaxis()->SetTitle("mass 2^{nd} jet (GeV)");
  theCanvas->SetGrid();
  massJet->Draw("colz");
  massJet->Draw("colz");

  sprintf(name,"%s.png",fname.c_str());
  theCanvas->SaveAs(name);
}
