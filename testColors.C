void testColors() {
  int lightcolors[5]={TColor::GetColor(134,210,251), // CMS blue
		      TColor::GetColor(228,26,28),   // Extendable orange-red 
		      TColor::GetColor(116,196,118), // Extendable green
		      TColor::GetColor(208,123,197), // Optimized purple
		      TColor::GetColor(206,170,100)}; // Optimized orangish

  TH1F* histo1 = new TH1F("histo1","histo1",20,0,5);
  TH1F* histo2 = new TH1F("histo2","histo2",20,0,5);
  TH1F* histo3 = new TH1F("histo3","histo3",20,0,5);
  TH1F* histo4 = new TH1F("histo4","histo4",20,0,5);
  TH1F* histo5 = new TH1F("histo5","histo5",20,0,5);

  histo1->FillRandom("expo",20000);
  histo2->FillRandom("expo",10000);
  histo3->FillRandom("expo",5000);
  histo4->FillRandom("expo",2500);
  histo5->FillRandom("expo",1300);
  
  histo1->SetFillColor(lightcolors[0]);
  histo2->SetFillColor(lightcolors[1]);
  histo3->SetFillColor(lightcolors[2]);
  histo4->SetFillColor(lightcolors[3]);
  histo5->SetFillColor(lightcolors[4]);

  THStack* hs = new THStack("hs","stack");
  hs->Add(histo5);
  hs->Add(histo4);
  hs->Add(histo3);
  hs->Add(histo2);
  hs->Add(histo1);
  
  hs->Draw();
}
