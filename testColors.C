void testColors()
{
   TH1F *histo1 = new TH1F("histo1", "histo1", 20, -5, 5);
   histo1->FillRandom("gaus");
   TCanvas *cv = new TCanvas("cv", "cv", 800, 800);

   // Call colors by number
   for (int i = 0; i != 15; ++i) {
      histo1->SetFillColor(SPcolors[i]);
      histo1->Draw();
      cv->SaveAs(TString::Format("test%i.pdf", i));
   }

   // Call colors by name
   histo1->SetFillColor(SPcolors[SPdred]);
   histo1->Draw();
   cv->SaveAs("test_dred.pdf");
}
