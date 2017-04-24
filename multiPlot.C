void customDraw(TH1* h, double xmin, double xmax, double statX, bool log) {
  gStyle->SetStatX(statX);
  if(xmin<xmax)
    h->GetXaxis()->SetRangeUser(xmin,xmax);
  if(log)
    gPad->SetLogy();
  h->Draw();
}


void multiPlot(string histoname, int rebinfactor=1,
	       double xmin=0.0, double xmax=-1.0, double statX=0.98,
	       bool log=false) {
  TCanvas* cv;
  TH1* h;
  TH1* h0;
  TH1* h1;
  TH1* h2;
  TH1* h3;

  _file0->GetObject(histoname.c_str(),h0);
  _file1->GetObject(histoname.c_str(),h1);
  _file2->GetObject(histoname.c_str(),h2);
  _file3->GetObject(histoname.c_str(),h3);
  
  h0->Rebin(rebinfactor);
  h1->Rebin(rebinfactor);
  h2->Rebin(rebinfactor);
  h3->Rebin(rebinfactor);
  
  cv = new TCanvas("cv",histoname.c_str(),1.2*800,1.2*600);
  cv->Divide(2,2);
  cv->cd(1);
  customDraw(h0,xmin,xmax,statX,log);
  cv->cd(2);
  customDraw(h1,xmin,xmax,statX,log);
  cv->cd(3);
  customDraw(h2,xmin,xmax,statX,log);
  cv->cd(4);
  customDraw(h3,xmin,xmax,statX,log);
}

void overlayPlot(string histoname, int rebinfactor=1,
	       double xmin=0.0, double xmax=-1.0, double statX=0.98,
		 bool log=false, bool norm=true) {
  TCanvas* cv;
  TH1* h;
  TH1* h0;
  TH1* h1;
  TH1* h2;
  TH1* h3;

  _file0->GetObject(histoname.c_str(),h0);
  _file1->GetObject(histoname.c_str(),h1);
  _file2->GetObject(histoname.c_str(),h2);
  _file3->GetObject(histoname.c_str(),h3);
  
  h0->Rebin(rebinfactor);
  h1->Rebin(rebinfactor);
  h2->Rebin(rebinfactor);
  h3->Rebin(rebinfactor);

  cv = new TCanvas("cv",histoname.c_str(),800,800);
  h0->SetLineColor(kBlack);
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  h3->SetLineColor(kGreen+2);

  h0->SetLineWidth(2);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h3->SetLineWidth(2);
  
  double integral=0;
  if(norm) {
    integral = h0->Integral();
    h0->Scale(1.0/integral);
    integral = h1->Integral();
    h1->Scale(1.0/integral);
    integral = h2->Integral();
    h2->Scale(1.0/integral);
    integral = h3->Integral();
    h3->Scale(1.0/integral);
  }

  h0->SetTitle("");
  h1->SetTitle("");
  h2->SetTitle("");
  h3->SetTitle("");
  if(norm) {
    h0->GetYaxis()->SetTitle("Normalized Distribution");
    h0->GetYaxis()->CenterTitle();
  }

  h0->Draw();
  h1->Draw("SAME");
  h2->Draw("SAME");
  h3->Draw("SAME");

  TLegend* leg = new TLegend(0.66,0.762,0.915,0.908);
  leg->AddEntry(h0,"CT10");
  leg->AddEntry(h1,"CTEQ6ll");
  leg->AddEntry(h2,"MSTW2008");
  leg->AddEntry(h3,"NNPDF21");
  leg->SetFillColor(kWhite);
  leg->Draw();

  makeCMSPreliminaryTop(8);
}

