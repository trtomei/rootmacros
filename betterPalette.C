//Draw color plots using different color palettes.
//Author:: Olivier Couet

#include "TStyle.h"
#include "TColor.h"
#include "TF2.h"
#include "TExec.h"
#include "TCanvas.h"
/*
void
setBetterPalette()
{
  const Int_t NRGBs = 5;
  const Int_t NCont = 63;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}
*/

void
setBetterPalette()
{
  TColor::SetPalette(53,0);
}

TCanvas* multipalette() {
  TCanvas *c3  = new TCanvas("c3","C3",0,0,600,600);
  TF2 *f3 = new TF2("f3","0.1+(1-(x-2)*(x-2))*(1-(y-2)*(y-2))",1,3,1,3);


  //f3->Draw("surf1"); 
  //TExec *ex1 = new TExec("ex1","Pal1();");
  //ex1->Draw();
  f3->Draw("surf1 same"); 

  return c3;
}
