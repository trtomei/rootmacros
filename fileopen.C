// @(#)etc:$Id: $
// Author: Axel Naumann, 2008-05-22
//
// This script gets executed when double-clicking a ROOT file.
// The file that got double clicked and opened is accessible as _file0.
void fileopen() 
{
  //new TBrowser;
  // or, to only browse the file:
  //new TBrowser(_file0);
  // or, alternatively
  new TBrowser("Browser",_file0,"ROOT Object Browser","");
} 
