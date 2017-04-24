#include "TH1.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 

void histogramFromFile(TH1* histo, const std::string fileName) {
  std::string line;
  ifstream myfile (fileName.c_str());
  if (myfile.is_open()) {
    std::cout << "File opened" << std::endl;    
    int i=1;
    {
      while ( getline (myfile,line) )
	{
	  if(i > histo->GetNbinsX()) std::cout << "ERROR: surpassed number of histogram bins" << std::endl;
	  double content = atof(line.c_str());
	  histo->SetBinContent(i,content);
	  std::cout << content << std::endl;
	  ++i;
	}
      myfile.close();
    }
  }
  else
    std::cout << "File NOT opened" << std::endl;

}
