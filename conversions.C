#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include "TF1.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TMath.h"
#include "Math/Interpolator.h"
#include "Math/QuantFuncMathCore.h"

class  MyFunctionObject {
public:
  std::vector<double> X;
  std::vector<double> Y;
  TGraph* g;
  ROOT::Math::Interpolator* eng;
  MyFunctionObject() {
  }
  
  MyFunctionObject(const TGraph* theGraph) {
    g = (TGraph*)theGraph->Clone("internalGraph");
    int numPoints = g->GetN();
    double thisX;
    double thisY;
    
    for(int i=0; i!=numPoints; ++i) {
      g->GetPoint(i,thisX,thisY);
      X.push_back(thisX);
      Y.push_back(thisY);
    }
    
    eng = new ROOT::Math::Interpolator(X,Y);
  }
  
  double operator() (double *x, double *p) {
    return eng->Eval(x[0]);
  }
  
private:
};

// TH1 --> TGraph
TGraph* TGraphfromTH1(TH1* h) {
  int nbinsx = h->GetNbinsX();
  TGraph* g = new TGraph(nbinsx);
  
  for(int i=0; i!=nbinsx; ++i) {
    double x = h->GetBinCenter(i+1);
    double y = h->GetBinContent(i+1);
    g->SetPoint(i,x,y);
  }

  return g;
}

// TH1 --> TGraph
TGraphAsymmErrors* GarwoodfromTH1(TH1* h) {
  const double alpha = 1 - 0.6827;
  int nbinsx = h->GetNbinsX();
  TGraphAsymmErrors* g = new TGraphAsymmErrors(nbinsx);

  for(int i=0; i!=nbinsx; ++i) {
    double x = h->GetBinCenter(i+1);
    double y = h->GetBinContent(i+1);
    double L =  (y==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,y,1.));
    double U =  ROOT::Math::gamma_quantile_c(alpha/2,y+1,1) ;
    g->SetPoint(i,x,y);
    g->SetPointEYlow(i, y-L);
    g->SetPointEYhigh(i, U-y);
  }

  return g;
}

// TGraph --> TH1
TH1F* TH1fromTGraph(TGraph* g) {
  TH1F* h = (TH1F*)g->GetHistogram()->Clone("histo");
  int npoints = g->GetN();
  double* xvalues = g->GetX();
  double* yvalues = g->GetY();
  
  for(int i=0; i!=npoints; ++i) {
    h->Fill(xvalues[i],yvalues[i]);
  }

  return h;
}


TH1F* TH1fromTGraph(TGraph* g, double xlow, double xup, int nbins, bool makeIntegerBins) {
  TH1F* h = new TH1F("histo","histo",nbins,xlow,xup);
  int npoints = g->GetN();
  double* xvalues = g->GetX();
  double* yvalues = g->GetY();
  
  if(nbins < npoints) {
    cout << "WARNING: nbins < npoints" << endl;
  }
 
  for(int i=0; i!=npoints; ++i) {
    double y = yvalues[i];
    int ibin = h->FindBin(xvalues[i]);
    if(ibin >= nbins) continue;
    if(makeIntegerBins) y = TMath::Nint(y);
    h->SetBinContent(ibin,y);
  }

  return h;
}
    
TH1F* TH1fromTGraph(TGraph* g, const TH1F* histo, bool makeIntegerBins) {
  TH1F* h = (TH1F*)histo->Clone("histoclone");
  int nbins = h->GetNbinsX();
  int npoints = g->GetN();
  double* xvalues = g->GetX();
  double* yvalues = g->GetY();
  
  if(nbins < npoints) {
    cout << "WARNING: nbins < npoints" << endl;
  }
 
  for(int i=0; i!=npoints; ++i) {
    double y = yvalues[i];
    int ibin = h->FindBin(xvalues[i]);
    if(ibin > nbins) continue;
    if(makeIntegerBins) y = TMath::Nint(y);
    cout << "Adding point " << i << " with content " << y << endl;
    h->SetBinContent(ibin,y);
  }

  return h;
}

// TGraph --> TF1
TF1* TF1fromTGraph(TGraph* g) {
  MyFunctionObject* myObj = new MyFunctionObject(g);
  
  int npoints = g->GetN();
  double minPoint = g->GetX()[0];
  double maxPoint = g->GetX()[npoints-1];
  
  TF1* myFunc = new TF1("funca",myObj,1.05*minPoint,0.95*maxPoint,0,"MyFunctionObject");
  
  return myFunc;
}  

// TH1 --> TF1
TF1* TF1fromTH1(TH1* h) {
  TGraph* g = TGraphfromTH1(h);
  TF1* f = TF1fromTGraph(g);
  return f;
}

// TH1 (integral) --> TF1
TF1* TF1fromTH1Integral(TH1* h) {
  
  int nbins = h->GetNbinsX();
  double* integralArray = h->GetIntegral();
  TGraph* g = new TGraph(nbins);

  for(int i=0; i!=nbins; ++i) {
    double x = h->GetBinCenter(i+1);
    double y = integralArray[i];
    g->SetPoint(i,x,y);
  }

  MyFunctionObject* myObj = new MyFunctionObject(g);
  
  int npoints = g->GetN();
  double minPoint = g->GetX()[0];
  double maxPoint = g->GetX()[npoints-1];
  
  TF1* myFunc = new TF1("funca",myObj,1.05*minPoint,0.95*maxPoint,0,"MyFunctionObject");
  
  return myFunc;
}  

// TH1 (integral) --> TGraph
TGraph* TGraphfromTH1Integral(const TH1* hreal) {
  
  TH1* h = (TH1*)hreal->Clone("clone");
  int nbins = h->GetNbinsX();
  double* integralArray = h->GetIntegral();
  TGraph* g = new TGraph(nbins);

  for(int i=0; i!=nbins; ++i) {
    double x = h->GetBinCenter(i+1);
    double y = integralArray[i];
    g->SetPoint(i,x,y);
  }
  
  h->Delete();
  return g;
}

// Efficiency as function of the cut in histogram
TGraph* efficiencyCut(const TH1* hreal) {
  
  TH1* h = (TH1*)hreal->Clone("clone");
  int nbins = h->GetNbinsX();
  double* integralArray = h->GetIntegral();
  TGraph* g = new TGraph(nbins);

  for(int i=0; i!=nbins; ++i) {
    double x = h->GetBinCenter(i+1);
    double y = 1.0-integralArray[i];
    g->SetPoint(i,x,y);
  }
  
  h->Delete();
  return g;
}

// Scale graph

void scaleGraph(TGraph* g, double scale) {
  int npoints = g->GetN();
  for(int i=0; i!=npoints; ++i) {
    double x = g->GetX()[i]; 
    double y = g->GetY()[i];
    double newY = y*scale;
    g->SetPoint(i,x,newY);
  }
  g->Print();
}

// integer graph

void integerGraph(TGraph* g) {
  int npoints = g->GetN();
  for(int i=0; i!=npoints; ++i) {
    double x = g->GetX()[i]; 
    double y = g->GetY()[i];
    double newY = TMath::Nint(y);
    g->SetPoint(i,x,newY);
  }
  g->Print();
}

// Histogram Utils

void histogramPrint(const TH1* histo) {
  int nbins = histo->GetNbinsX();
  for(int i=1; i<=nbins; ++i) {
    std::cout <<  histo->GetBinContent(i) << std::endl;
  }
}

void detailedHistogramPrint(const TH1* histo) {
  int nbins = histo->GetNbinsX();
  for(int i=1; i<=nbins; ++i) {
    double content = histo->GetBinContent(i);
    double lowEdge = histo->GetBinLowEdge(i);
    double highEdge = lowEdge+histo->GetBinWidth(i);
    std::cout << "Bin " << i << " [" << lowEdge << "," << 
      highEdge << "]\t" << histo->GetBinContent(i) << std::endl;
  }
}

void graphPrint(const TGraph* graph) {
  int nbins = graph->GetN();
  for(int i=0; i!=nbins; ++i) {
    std::cout <<  graph->GetX()[i] << "\t" << graph->GetY()[i]  << std::endl;
  }
}

void histogramDensityToCounts(TH1* histo) {
  int nbins = histo->GetNbinsX();
  for(int i=1; i<=nbins; ++i) {
    double binContent = histo->GetBinContent(i);
    double binWidth   = histo->GetBinWidth(i);
    double newBinContent = binContent*binWidth;
    histo->SetBinContent(i,newBinContent);
  }
}

void makeHistogramInteger(TH1* histo) {
  int nbins = histo->GetNbinsX();
  for(int i=1; i<=nbins; ++i) {
    double binContent = histo->GetBinContent(i);
    int newBinContent = TMath::Nint(binContent);
    histo->SetBinContent(i,newBinContent);
  }
}

void fillHistogramWithFunction(TF1* func, TH1* histo) {
  int nbins = histo->GetNbinsX();
  for(int i=1; i<=nbins; ++i) {
    double binWidth = histo->GetBinWidth(i);
    double binLowEdge = histo->GetBinLowEdge(i);
    double binHighEdge = binLowEdge+binWidth;
    double integral = func->Integral(binLowEdge,binHighEdge)/binWidth;
    histo->SetBinContent(i,integral);
  }
}

TGraph* errorGraph(TGraphAsymmErrors* ge, int errorSide, bool negativeErrors) {
  if(errorSide==0) {
    std::cout << "NOOO, up or down!" << std::endl;
    return 0;
  }

  int npoints = ge->GetN();
  TGraph* g = new TGraph(npoints);

  for(int i=0; i!=npoints; ++i) {
    double x = ge->GetX()[i];
    double y = ge->GetY()[i];
    double error;
    if(!negativeErrors) {
      if(errorSide==1) {
	error = ge->GetErrorYhigh(i);
	y = y+error;
      }
      if(errorSide==-1) {
	error = ge->GetErrorYlow(i);
	y = y-error;
      }
      g->SetPoint(i,x,y);
    }
    else { // special place in hell for these people
      if(errorSide==1) {
	error = ge->GetErrorYlow(i);
	y = y-error;
      }
      if(errorSide==-1) {
	error = ge->GetErrorYhigh(i);
	y = y+error;
      }
      g->SetPoint(i,x,y);

    }
  }

  return g;
}

/// Snips pBegin points from the beginning of the graph and pEnd points from the end.
TGraph* snipGraph(TGraph* go, int pBegin, int pEnd) {
  int nold = go->GetN();
  int nnew = nold-(pBegin+pEnd);
  TGraph* gn = new TGraph(nnew);
  
  for(int i=pBegin; i!=(nold-pEnd); ++i) {
    int thisI = i-pBegin;
    double x = go->GetX()[i];
    double y = go->GetY()[i];
    gn->SetPoint(thisI,x,y);
  }
  
  return gn;
}

void normalizeHistogram(TH1* histo) {
  double integral = histo->Integral();
  histo->Scale(1.0/integral);
}

void renameHistogram(TH1* histo, TString oldS, TString newS) {
  TString oldName = histo->GetName();
  oldName.ReplaceAll(oldS,newS);
  histo->SetName(oldName.Data());
}

void histogramPrintWithErrors (TH1* histo) {
  //binLow(GeV)    binHigh(GeV)    dsdm(pb/GeV)    errLow_dsdm(pb/GeV)     errUp_dsdm(pb/GeV)
  histo->SetBinErrorOption(TH1::kPoisson);
  int nBinsX = histo->GetNbinsX();
  int theBin=0;
  for (int i=0; i!=nBinsX; ++i) {
    theBin=i+1;
    double xbinLowEdge = histo->GetBinLowEdge(theBin);
    double width = histo->GetBinWidth(theBin);
    double xbinHighEdge = xbinLowEdge+width;
    double binContent = histo->GetBinContent(theBin);
    double binErrorLow = histo->GetBinErrorLow(theBin);
    double binErrorUp = histo->GetBinErrorUp(theBin);
    std::cout << xbinLowEdge << " "
	      << xbinHighEdge << " "
	      << binContent << " "
	      << binErrorLow  << " "
	      << binErrorUp << std::endl;
  }
}

TH1F* updateHistogramFromFile(TH1F* histo, const char* inputFile, const char* name) {
  TH1F* hclone = (TH1F*)histo->Clone(name);
  int nbins = hclone->GetNbinsX();
  hclone->Reset();
  ifstream in;
  double theContent;
  in.open(inputFile);
  for(int i=0; i!=nbins; ++i) {
    int theBin = i+1;
    in >> theContent;
    hclone->SetBinContent(theBin,theContent);
  }

  return hclone;
}
