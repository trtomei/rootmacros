#include <iostream>
#include <cmath>
#include <TMath.h>

void SMconstants(double GF=1.16637E-5, double alpha=1.0/137.036, double mz=91.1876, double mH=125.0) {

  double v = sqrt(1.0/(sqrt(2.0)*GF));
  double pi = TMath::Pi();
  double e = sqrt(4.0*alpha*pi);
  double thetaW = 0.5*TMath::ASin(v*e/mz);
  double g = e/sin(thetaW);
  double gprime = e/cos(thetaW);
  double lambda = ((mH/v)*(mH/v))/2.0;


  printf("v = %g GeV\n"
	 "g = %g\n"
	 "gprime = %g\n"
	 "lambda = %g\n",v,g,gprime,lambda);

  double mW = v*g/2.0;
  printf("Cross check: massW = %g GeV\n",mW);

  return;
}

double EtaFromTheta(double theta) {
  double eta = -1.0 * TMath::Log(TMath::Tan(theta/2.0));
  return eta;
}

double ThetaFromEta(double eta) {
  double theta = 2.0 * TMath::ATan(TMath::Exp(-eta));
  return theta;
}

double metresFromGeV(double GeV){
  double valueIneV = 1E9*GeV;
  double convFactor = 8.06554429E5;
  double invMetres = valueIneV*convFactor;
  double metres = 1.0/invMetres;
  return metres;
}

double ZZToLLLL()
{
  double ZToE = 0.03363;
  double ZToM = 0.03366;
  double result = (ZToE+ZToM)*(ZToE+ZToM);
  return result;
}

double ZZToLLQQ()
{
  double ZToE = 0.03363;
  double ZToM = 0.03366;
  double ZToHadrons = 0.6991;
  double result = (ZToE+ZToM)*ZToHadrons*2;
  return result;
}

double ZZToLLQQWithTaus()
{
  double ZToE = 0.03363;
  double ZToM = 0.03366;
  double ZToT = 0.03367;
  double ZToHadrons = 0.6991;
  double result = (ZToE+ZToM+ZToT)*ZToHadrons*2;
  return result;
}

double ZZToNNQQ()
{
  double ZToN = 0.2;
  double ZToHadrons = 0.6991;
  double result = ZToN*ZToHadrons*2;
  return result;
}

double WWToLLNN()
{
  double WToE = 0.1075;
  double WToM = 0.1057;
  double WToHadrons = 0.6760;
  double result = (WToE+WToM)*(WToE+WToM);
  return result;
}

double WWToLNQQ()
{
  double WToE = 0.1075;
  double WToM = 0.1057;
  double WToHadrons = 0.6760;
  double result = (WToE+WToM)*WToHadrons*2;
  return result;
}

double WWToLNQQWithTau()
{
  double WToE = 0.1075;
  double WToM = 0.1057;
  double WToT = 0.1125;
  double WToHadrons = 0.6760;
  double result = (WToE + WToM + WToT)*WToHadrons*2;
  return result;
}

double WWToLNQQWithTauLeptonic()
{
  double WToE = 0.1075;
  double WToM = 0.1057;
  double WToT = 0.1125;
  double TtoE = 0.1785;
  double TtoM = 0.1736;
  double WToHadrons = 0.6760;
  double result = (WToE + WToM + WToT*TtoE + WToT*TtoM)*WToHadrons*2;
  return result;
}

double weightedAverage(double x1, double s1, double x2, double s2) {
  double w1 = 1.0/(s1*s1);
  double w2 = 1.0/(s2*s2);
  double xm = (x1 * w1 + x2 * w2)/(w1+w2);
  double sm = sqrt(1/(w1+w2));
  std::cout << "Weighted average: " << xm << " +/- " << sm << std::endl;
  return sm;
}

double lifetimeFromSigma(double sigmaInGeV) {
    double sigmaIneV = sigmaInGeV * 1E9;
    double hbar = 6.582119569E-16; // eV*second;
    double c = TMath::C(); // meters/second
    double tau = hbar/sigmaIneV;
    double tauc = tau*c;
    double taucInmm = tauc*1000;
    std::cout << "tau = " << tau << std::endl;
    std::cout << "tauc in meters = " << tauc << std::endl;
    std::cout << "tauc in mm = " << taucInmm << std::endl;
    return tau;
}

double sigmaFromTOF(double TOFInmm) {
    double hbar = 6.582119569E-16; // eV*second;
    double c = TMath::C(); // meters/second;
    double tauc = TOFInmm/1000;
    double tau = tauc/c;
    double sigmaIneV = hbar/tau;
    double sigmaInGeV = sigmaIneV / 1E9;
    return sigmaInGeV;
}
