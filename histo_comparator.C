void histo_comparator(TH1* histo1, TH1* histo2, bool normalized) {

  histo1->Draw();
  histo2->Draw();
  
  histo1->SetLineWidth(2);
  histo2->SetLineWidth(2);
  histo1->SetLineColor(kRed);
  histo2->SetLineColor(kBlack);
  histo1->SetFillColor(kRed);
  histo2->SetFillColor(kBlack);
  histo1->SetFillStyle(3004);
  histo2->SetFillStyle(3005);

  if(normalized) {
    histo1->Scale(1.0/histo1->Integral());
    histo2->Scale(1.0/histo2->Integral());
  }
  histo1->Draw();
  histo2->Draw("same");
   
  TLegend* leg = new TLegend(0.7, 0.75, 0.88, 0.88);
  leg->AddEntry(histo1,"Histo1","f");
  leg->AddEntry(histo2,"Histo2","f");
  leg->SetFillColor(0);
  leg->Draw();
}
