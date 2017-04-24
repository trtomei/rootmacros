/// Simple macro to calculate the prescales of ZeroBias seed
/// Inputs: number of colliding bunches, target ZeroBias rate in kHz
/// Outputs: the prescales, guaranteed to be prime and co-prime with
/// the number of colliding bunches.
/// Usage: .L calculateZeroBiasPrescales.C

bool checkCoPrimes(int nb, int ps) {
  bool result = nb > ps ? nb%ps != 0: ps%nb != 0 ; 
  return result;
}

void calculateZeroBiasPrescales(int numberOfBunches, double targetRate = 20.0) {

  double LHCcircle =  26659.0; // meters
  double LHCfrequency = TMath::C()/LHCcircle / 1000.0; // kHz
  printf("LHC frequency = %g kHz\n",LHCfrequency);
  double ZBfrequency = LHCfrequency * numberOfBunches;
  printf("ZeroBias frequency = %g kHz\n",ZBfrequency);
  
  int prescale = int(ZBfrequency/targetRate);
  prescale = TMath::NextPrime(prescale);

  // We must make them prime amongst themselves
  while(!checkCoPrimes(numberOfBunches, prescale)) {prescale = TMath::NextPrime(prescale+1);}

  printf("With prescale = %i, rate will be %4.1f kHz\n",prescale, ZBfrequency/prescale);
}
