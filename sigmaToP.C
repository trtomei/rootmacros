#include "Math/ProbFuncMathCore.h"

double sigmaToP(double sigma) {
  double p = ROOT::Math::normal_cdf(sigma)-ROOT::Math::normal_cdf(-sigma);
  printf("Probability in between -%g sigma and +%g sigma: %g\n",sigma,sigma,p);
  return p;
}

double sigmaToPvalue(double sigma) {
  double p = 1.0-ROOT::Math::normal_cdf(sigma);
  return p;
}
