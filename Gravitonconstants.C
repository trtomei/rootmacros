#include <cmath>
#include <TMath.h>

double RSWidth(double c, double mass) {
  double pi = TMath::Pi();
  double x1 = 3.83;
  double cx1 = c*x1;
  double width = 291*cx1*cx1*mass/(960*pi);
  return width;
}

double RSWidthAccurate(double c, double mass) {
  double pi = TMath::Pi();
  double x1 = 3.830716;
  double cx1 = c*x1;
  double mW = 80.384;
  double mZ = 90.1876;
  double mtop = 172.9;
  
  double rVW = mW*mW/(mass*mass);
  double rVZ = mZ*mZ/(mass*mass);
  double rFtop = mtop*mtop/(mass*mass);
  
  double kappa = sqrt(2)*x1*c/mass;

  double sigmaGToWW = 1.0*(kappa*kappa*mass*mass*mass)/(80*pi)*sqrt(1-4*rVW)*(13.0/12.0+14.0*rVW/3.0+4.0*rVW*rVW);
  double sigmaGToZZ = 0.5*(kappa*kappa*mass*mass*mass)/(80*pi)*sqrt(1-4*rVZ)*(13.0/12.0+14.0*rVZ/3.0+4.0*rVZ*rVZ);
  double sigmaGToVV = sigmaGToWW + sigmaGToZZ;

  double sigmaGToTopTop = 3.0*(kappa*kappa*mass*mass*mass)/(320*pi)*pow(1-4*rFtop,1.5)*(1+8.0*rFtop/3.0);
  double sigmaGToLightQuarks = 3.0*(kappa*kappa*mass*mass*mass)/(320*pi)*5.0; // 5 light quarks
  double sigmaGToLeptons = (kappa*kappa*mass*mass*mass)/(320*pi)*6.0; // 6 leptons;
  double sigmaGToFermions = sigmaGToTopTop+sigmaGToLightQuarks+sigmaGToLeptons;

  double sigmaGToGluonGluon = (kappa*kappa*mass*mass*mass)/(20*pi);
  double sigmaGToGammaGamma = (kappa*kappa*mass*mass*mass)/(160*pi);
  
  double width = sigmaGToVV + sigmaGToFermions + sigmaGToGluonGluon+sigmaGToGammaGamma;
  return width;
}

double ADPSWidth(double c, double mass) {
  double pi = TMath::Pi();
  double x1 = 3.83;
  double cx1 = c*x1;
  double width = 13*cx1*cx1*mass/(960*pi);
  return width;
}

double CFromADPSWidth(double width, double mass) {
  double pi = TMath::Pi();
  double x1 = 3.83;
  double numer = 960*pi*width;
  double denom = 13*x1*x1*mass;
  double c = sqrt(numer/denom);
  return c;
}
