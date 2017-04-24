/*************************************************
  Automatically plots histograms from two files
  onto the same plot and saves them.
  It looks for ALL histograms in the first file
  and plots the corresponding histogram in the 2nd
  file onto the sample plot.
  
  Can be run from a bash prompt as well:
    root -b -l -q "plotHistogramsTogether.C(\"fileA.root\",\"fileB.root\")"
    root -b -l -q "plotHistogramsTogether.C(\"fileA.root\",\"fileB.root\",\"Signal\",\"Background\",10,2,1,1,5)"

  Michael B. Anderson
  Sept 5, 2008
*************************************************/

#include <string.h>
#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "Riostream.h"

// Accesable everywhere
TObject *obj;
TFile   *sourceFile1, *sourceFile2;
TString label1, label2;
TString outputFolder,outputFilename;
TCanvas *canvasDefault;
Float_t scale1, scale2;
Int_t rebinNumber;
bool forceNonLog;
bool showStatsBoxes;
bool isRealData;

// *******************************************
// Variables
TString imageType = "pdf";
int outputWidth   = 800;
int outputHeight  = 800;
bool yAxisLogScale = false;
// End of Variables
// *******************************************


void histogramTitles(TH1* theHisto) {
  TString* theName = new TString(theHisto->GetName());
  if(theName->Contains("mass",TString::kIgnoreCase)){
    theHisto->GetXaxis()->SetTitle("mass (GeV)");
  }
  if(theName->Contains("pt",TString::kIgnoreCase)){
    theHisto->GetXaxis()->SetTitle("p_{T} (GeV)");
  }
  if(theName->Contains("eta",TString::kIgnoreCase)){
    theHisto->GetXaxis()->SetTitle("#eta");
  }
  if(theName->Contains("phi",TString::kIgnoreCase)){
    theHisto->GetXaxis()->SetTitle("#phi");
  }
  if(theName->Contains("mass",TString::kIgnoreCase) && theName->Contains("jet",TString::kIgnoreCase)) {
    theHisto->GetXaxis()->SetTitle("Jet mass (GeV)");
  }
  if(theName->Contains("pt",TString::kIgnoreCase) && theName->Contains("jet",TString::kIgnoreCase)) {
    theHisto->GetXaxis()->SetTitle("Jet p_{T} (GeV)");
  }
  if(theName->Contains("eta",TString::kIgnoreCase) && theName->Contains("jet",TString::kIgnoreCase)) {
    theHisto->GetXaxis()->SetTitle("Jet #eta");
  }
  if(theName->Contains("phi",TString::kIgnoreCase) && theName->Contains("jet",TString::kIgnoreCase)) {
    theHisto->GetXaxis()->SetTitle("Jet #phi");
  }
  if(theName->Contains("MET",TString::kIgnoreCase)) {
    theHisto->GetXaxis()->SetTitle("MET (GeV)");
  }

  TString* theDirName = new TString(theHisto->GetDirectory()->GetName());
  if(theDirName->Contains("plotJets")) {
    if(theName->Contains("Z_pt")) {
      theHisto->SetName("jet_pt");
      theHisto->SetTitle("jet_pt");
    }     
    if(theName->Contains("Z_eta")) {
      theHisto->SetName("jet_eta");
      theHisto->SetTitle("jet_eta");
    } 
    if(theName->Contains("Z_mass")) {
      theHisto->SetName("jet_mass");
      theHisto->SetTitle("jet_mass");
    }     
  }
  theHisto->SetTitle("");
}

void plotHistogramsTogether(TString fileName1,
                            TString fileName2,
                            TString fileLabel1 = "",
                            TString fileLabel2 = "",
                            Float_t fileScale1 = -1.0,
                            Float_t fileScale2 = -1.0,
                            bool showStats = false,
			    bool nonLog = false,
			    Int_t rebin = 1,
			    bool isData = true) {

  // If file labels were not given as argument,
  // use the filename as a label
  if (fileLabel1 == "") {
    fileLabel1 = fileName1;
    fileLabel2 = fileName2;
    fileLabel1.ReplaceAll(".root","");
    fileLabel1.ReplaceAll(".root","");
  }
  label1 = fileLabel1;
  label2 = fileLabel2;
  rebinNumber = rebin;
  forceNonLog = nonLog;
  isRealData = isData;

  // Set the scale of the histograms.
  // If they are < 0.0, they will be area normalized
  scale1 = fileScale1;
  scale2 = fileScale2;
  showStatsBoxes = showStats;

  sourceFile1 = TFile::Open( fileName1 );
  sourceFile2 = TFile::Open( fileName2 );

  outputFolder = "HistogramsTogether/"; // Blank to use current directory,
                                          // or, for a specific dir type
                                          // something like "images/"

  gSystem->MakeDirectory(outputFolder);

  canvasDefault = new TCanvas("canvasDefault","testCanvas",outputWidth,outputHeight);

  // This function will plot all histograms from 
  // file1 against matching histogram from file2
  recurseOverKeys(sourceFile1);

  sourceFile1->Close();
  sourceFile2->Close();

  TString currentDir = gSystem->pwd();
  cout << "Done. See images in:" << endl << currentDir << "/" << outputFolder << endl;
}

void recurseOverKeys( TDirectory *target1 ) {

  // Figure out where we are
  TString path( (char*)strstr( target1->GetPath(), ":" ) );
  path.Remove( 0, 2 );

  sourceFile1->cd( path );
  
  TDirectory *current_sourcedir = gDirectory;

  TKey *key;
  TIter nextkey(current_sourcedir->GetListOfKeys());

  while (key = (TKey*)nextkey()) {

    obj = key->ReadObj();

    // Check if this is a 1D histogram or a directory
    if (obj->IsA()->InheritsFrom("TH1F")) {

      // **************************
      // Plot & Save this Histogram
      TH1F *htemp1, *htemp2;

      htemp1 = (TH1F*)obj;
      TString histName = htemp1->GetName();

      if (path != "") {
        sourceFile2->GetObject(path+"/"+histName, htemp2);
      } else {
      sourceFile2->GetObject(histName, htemp2);
      }

      outputFilename=histName;
      plot2Histograms(htemp1, htemp2, outputFolder+path+"/"+outputFilename+"."+imageType);

    } else if ( obj->IsA()->InheritsFrom( "TDirectory" ) ) {
      // it's a subdirectory

      cout << "Found subdirectory " << obj->GetName() << endl;
      gSystem->MakeDirectory(outputFolder+path+"/"+obj->GetName());

      // obj is now the starting point of another round of merging
      // obj still knows its depth within the target file via
      // GetPath(), so we can still figure out where we are in the recursion
      recurseOverKeys( (TDirectory*)obj );

      } // end of IF a TDirectory 
  }
}

void plot2Histograms(TH1* htemp1, TH1* htemp2, TString filename) {

  // Make sure histograms exist
  if ( !htemp2 ) {
    cout << "Histogram missing from 2nd file: " << htemp1->GetName() << endl;
    return;
  }

  // Rebin
  if(rebinNumber != 1) {
    htemp1->Rebin(rebinNumber);
    htemp2->Rebin(rebinNumber);
  }
    
  // Scale by given factor.
  // If given factor is negative, area normalize
  if (scale1 > 0.0) {
    htemp1->Scale(scale1);
  } else {
    Double_t integral = htemp1->Integral();
    if (integral>0.0) htemp1->Scale(1/integral);
  }
  if (scale2 > 0.0) {
    htemp2->Scale(scale2);
  } else {
    Double_t integral = htemp2->Integral();
    if (integral>0.0) htemp2->Scale(1/integral);
  }
  
  // Set the histogram colors & lines
  htemp1->SetLineColor(kBlack);
  htemp2->SetLineColor(kRed);
  htemp1->SetLineWidth(2);
  htemp2->SetLineWidth(2);

  // Turn off stats
  if (!showStatsBoxes) {
    gStyle->SetOptStat(0);
  }

  // Fix histogram titles
  histogramTitles(htemp1);
  TString title = htemp1->GetTitle();
  htemp2->SetName(htemp1->GetName());

  // Create TStack but we will draw without stacking
  THStack *tempStack = new THStack();
  tempStack->Add(htemp1,"sames");
  tempStack->Add(htemp2,"sames");

  // Find minimum and maximum.
  double max1 = htemp1->GetBinContent(htemp1->GetMaximumBin());
  double max2 = htemp2->GetBinContent(htemp2->GetMaximumBin());
  double max;
  if (max1 > max2) max = max1;
  if (max2 > max1) max = max2;
 
  if(max1 == 0.0) return;
  if(max2 == 0.0) return;

  int nbins1 = htemp1->GetNbinsX();
  int nbins2 = htemp2->GetNbinsX();
  double min1 = 99999999.9;
  double min2 = 99999999.9;
  double min;

  for (int ibin = 1; ibin != nbins1; ibin++) {
    if ((htemp1->GetBinContent(ibin) < min1) && (htemp1->GetBinContent(ibin) != 0.0))
      min1 = htemp1->GetBinContent(ibin);
  }
  for (int ibin = 1; ibin != nbins1; ibin++) {
    if ((htemp2->GetBinContent(ibin) < min2) && (htemp2->GetBinContent(ibin) != 0.0))
      min2 = htemp1->GetBinContent(ibin);
  }
  if(min1 < min2) min = min1;
  if(min2 < min1) min = min1;
  
  // Some logic for log or non log scale
  if(max > 1000*min) yAxisLogScale=true;
  if(max < 1000*min) yAxisLogScale=false;
  if(forceNonLog) yAxisLogScale=false;
  
  // Draw the histogram and titles
  tempStack->Draw();
  tempStack->GetHistogram()->GetYaxis()->SetRangeUser(0.5*min,1.1*max);
  if(scale1 < 0.0 && scale2 < 0.0) {
    tempStack->GetHistogram()->GetYaxis()->SetTitle("Normalized Distribution");
  } 

  // Set the histogram colors & lines  
  tempStack->Draw("hist nostack");
  tempStack->SetTitle(title);
  tempStack->GetXaxis()->SetTitle(htemp1->GetXaxis()->GetTitle());


  // Draw the legend
  TLegend *infoBox = new TLegend(0.75, 0.83, 0.99, 0.99, "");
  infoBox->AddEntry(htemp1,label1,"L");
  infoBox->AddEntry(htemp2,label2,"L");
  infoBox->SetShadowColor(0);  // 0 = transparent
  infoBox->SetFillColor(kWhite); 
  infoBox->Draw();

  // Place the stats boxes to be non-overlapping
  if (showStatsBoxes) {
    canvasDefault->SetRightMargin(0.2);
    canvasDefault->Update();
    TPaveStats *st1 = (TPaveStats*)htemp1->GetListOfFunctions()->FindObject("stats"); 
    TPaveStats *st2 = (TPaveStats*)htemp2->GetListOfFunctions()->FindObject("stats");
    st1->SetX1NDC(.79); st1->SetX2NDC(.99);
    st1->SetY1NDC(.6);  st1->SetY2NDC(.8);
    st2->SetX1NDC(.79); st2->SetX2NDC(.99);
    st2->SetY1NDC(.38); st2->SetY2NDC(.58);
    canvasDefault->Modified();
  }

  // Set log y axis
  if (yAxisLogScale) canvasDefault->SetLogy(1);
  // Make the CMS Preliminary thing
  makeCMSPreliminaryTop(8);
  // Save the canvas
  canvasDefault->SaveAs(filename);

}
