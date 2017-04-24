double lambdaPiFromKmpl(double kmpl, double gravitonMass = 1000.0) {
  return gravitonMass/(kmpl*3.8317);
}

double kmplFromLambdaPi(double lambdaPi, double gravitonMass = 1000.0) {
  return gravitonMass/(lambdaPi*3.8317);
}
