#include <vector>
#include "TF1.h" 
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TString.h"
#include "Math/Interpolator.h"

class  MyFunctionObject {
public:
  std::vector<double> X;
  std::vector<double> Y;
  TGraph* g;
  ROOT::Math::Interpolator* eng;

  // use constructor to customize your function object                                                                                                      
  MyFunctionObject() {
  }

  MyFunctionObject(const TGraph* theGraph) {
    g = (TGraph*)theGraph->Clone("internalGraph");
    printf("Graph cloned\n");
    int numPoints = g->GetN();
    double thisX;
    double thisY;

    for(int i=0; i!=numPoints; ++i) {
      g->GetPoint(i,thisX,thisY);
      X.push_back(thisX);
      Y.push_back(thisY);
    }
    printf("Vectors set\n");

    eng = new ROOT::Math::Interpolator(X,Y);
  }
  
  double operator() (double *x, double *p) {
    return eng->Eval(x[0]);
  }

private:
};

double integrateHistogram(const TH1* h, double fraction) {
  
  int nbinsx = h->GetNbinsX();
  double histoIntegral = h->Integral();
  TGraph* g = new TGraph(nbinsx-1);

  
  for(int i=1; i!=nbinsx; ++i) {
    double inte = h->Integral(i,nbinsx);
    double frac = inte/histoIntegral;
    double xpoint = h->GetBinCenter(i);
    g->SetPoint(i-1,xpoint,frac);
  }

  MyFunctionObject* myObj = new MyFunctionObject(g); 
  
  double minPoint = g->GetX()[0];
  double maxPoint = g->GetX()[nbinsx-2];
  printf("%g\t%g\t(min and max)\n",minPoint,maxPoint);

  TF1* myFunc = new TF1("funca",myObj,1.05*minPoint,0.95*maxPoint,0,"MyFunctionObject");
  
  double value = myFunc->GetX(fraction);
  double xcheck = myFunc->Eval(value);
  //myFunc->Draw();
  printf("Cross check: %g\n",xcheck);

  return value;
}
