void tof() {
  cout << "Hello." << endl << endl;


  //  TFile F("../ROOTfiles/hms_replay_303_bc.root");
  TFile F("../ROOTfiles/hms_replay_303.root");
  TTree* T = F.Get("T");

  Double_t fptime_1x;
  Double_t fptime_2x;
  Double_t fptime_1y;
  Double_t fptime_2y;
  Double_t gtrbeta;
  Double_t betanotrk;

  T->SetBranchAddress("H.hod.1x.fptime", &fptime_1x);
  T->SetBranchAddress("H.hod.2x.fptime", &fptime_2x);
  T->SetBranchAddress("H.hod.1y.fptime", &fptime_1y);
  T->SetBranchAddress("H.hod.2y.fptime", &fptime_2y);
  T->SetBranchAddress("H.gtr.beta", &gtrbeta);
  T->SetBranchAddress("H.hod.betanotrack", &betanotrk);

  TH1F *fp1x = new TH1F("fpt1x", "", 100, 0., 50);
  TH1F *fp2x = new TH1F("fpt2x", "", 100, 0., 50);
  TH1F *fp1y = new TH1F("fpt1y", "", 100, 0., 50);
  TH1F *fp2y = new TH1F("fpt2y", "", 100, 0., 50);

  TH1F *beta = new TH1F("betais", "", 300, 0., 1.4);
  TH1F *betant = new TH1F("betaisnt", "", 300, 0., 1.4);


  Long64_t nEvents = T->GetEntries();
  //  for (Long64_t iEvent; iEvent<10; ++iEvent) {
  for (Long64_t iEvent; iEvent<nEvents; ++iEvent) {
    if ((iEvent+1)%10000 == 0) cout << "Event: " << iEvent+1 << endl;
    T->GetEntry(iEvent);

    //    cout << "iEvent and fptime_1x:  " << " " << iEvent << " " << fptime_1x << endl;

    fp1x->Fill(fptime_1x);
    fp2x->Fill(fptime_2x);
    fp1y->Fill(fptime_1y);
    fp2y->Fill(fptime_2y);

    beta->Fill(gtrbeta);
    betant->Fill(betanotrk);

  }

  gStyle->SetOptStat(kFALSE);
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPalette(1,0);


  //  **************************************************************************

  TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
  c1->Divide(2,2);
  c1->cd(1);
  fp1x->SetLabelSize(0.045, "x");
  fp1x->SetLabelSize(0.045, "y");
  fp1x->GetXaxis()->SetTitleSize(0.055);
  fp1x->GetYaxis()->SetTitleSize(0.055);
  fp1x->GetXaxis()->CenterTitle(1);
  fp1x->GetXaxis()->SetTitleOffset(0.8);
  fp1x->GetXaxis()->SetTitle("fptime_1x");
  fp1x->SetFillStyle(3009);
  fp1x->SetFillColor(kRed-7);
  fp1x->Draw();

  TF1 *gaus1 = new TF1("gaus1","gaus",12.,21.);
  gaus1->SetLineColor(kAzure);
  gaus1->SetLineStyle(2);
  gaus1->SetLineWidth(5);
  fp1x->Fit("gaus1","N R");
  gaus1->Draw("same"); 

  Float_t mean = gaus1->GetParameter(1);
  Float_t sigma = gaus1->GetParameter(2);

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);

  leg = new TLegend(0.6,0.75,0.75,0.85);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.045);
  leg->SetTextColor(kAzure);
  leg->SetBorderSize(0);
  leg->AddEntry(fp1x,legis_1,"");
  leg->AddEntry(fp1x,legis_2,"");
  leg->Draw("same"); 

  c1->cd(2);
  fp2x->SetLabelSize(0.045, "x");
  fp2x->SetLabelSize(0.045, "y");
  fp2x->GetXaxis()->SetTitleSize(0.055);
  fp2x->GetYaxis()->SetTitleSize(0.055);
  fp2x->GetXaxis()->CenterTitle(1);
  fp2x->GetXaxis()->SetTitleOffset(0.8);
  fp2x->GetXaxis()->SetTitle("fptime_2x");
  fp2x->SetFillStyle(3009);
  fp2x->SetFillColor(kRed-7);
  fp2x->Draw();

  TF1 *gaus2 = new TF1("gaus2","gaus",12.,21.);
  gaus2->SetLineColor(kAzure);
  gaus2->SetLineStyle(2);
  gaus2->SetLineWidth(5);
  fp2x->Fit("gaus2","N R");
  gaus2->Draw("same"); 

  Float_t mean = gaus2->GetParameter(1);
  Float_t sigma = gaus2->GetParameter(2);

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);

  leg = new TLegend(0.6,0.75,0.75,0.85);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.045);
  leg->SetTextColor(kAzure);
  leg->SetBorderSize(0);
  leg->AddEntry(fp2x,legis_1,"");
  leg->AddEntry(fp2x,legis_2,"");
  leg->Draw("same"); 

  c1->cd(3);
  fp1y->SetLabelSize(0.045, "x");
  fp1y->SetLabelSize(0.045, "y");
  fp1y->GetXaxis()->SetTitleSize(0.055);
  fp1y->GetYaxis()->SetTitleSize(0.055);
  fp1y->GetXaxis()->CenterTitle(1);
  fp1y->GetXaxis()->SetTitleOffset(0.8);
  fp1y->GetXaxis()->SetTitle("fptime_1y");
  fp1y->SetFillStyle(3009);
  fp1y->SetFillColor(kRed-7);
  fp1y->Draw();


  TF1 *gaus3 = new TF1("gaus3","gaus",12.,21.);
  gaus3->SetLineColor(kAzure);
  gaus3->SetLineStyle(2);
  gaus3->SetLineWidth(5);
  fp2x->Fit("gaus3","N R");
  gaus3->Draw("same"); 

  Float_t mean = gaus3->GetParameter(1);
  Float_t sigma = gaus3->GetParameter(2);

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);

  leg = new TLegend(0.6,0.75,0.75,0.85);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.045);
  leg->SetTextColor(kAzure);
  leg->SetBorderSize(0);
  leg->AddEntry(fp1y,legis_1,"");
  leg->AddEntry(fp1y,legis_2,"");
  leg->Draw("same");

  c1->cd(4);
  fp2y->SetLabelSize(0.045, "x");
  fp2y->SetLabelSize(0.045, "y");
  fp2y->GetXaxis()->SetTitleSize(0.055);
  fp2y->GetYaxis()->SetTitleSize(0.055);
  fp2y->GetXaxis()->CenterTitle(1);
  fp2y->GetXaxis()->SetTitleOffset(0.8);
  fp2y->GetXaxis()->SetTitle("fptime_2y");
  fp2y->SetFillStyle(3009);
  fp2y->SetFillColor(kRed-7);
  fp2y->Draw();

  TF1 *gaus4 = new TF1("gaus4","gaus",12.,21.);
  gaus4->SetLineColor(kAzure);
  gaus4->SetLineStyle(2);
  gaus4->SetLineWidth(5);
  fp2x->Fit("gaus4","N R");
  gaus4->Draw("same"); 

  Float_t mean = gaus4->GetParameter(1);
  Float_t sigma = gaus4->GetParameter(2);

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);

  leg = new TLegend(0.6,0.75,0.75,0.85);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.045);
  leg->SetTextColor(kAzure);
  leg->SetBorderSize(0);
  leg->AddEntry(fp2y,legis_1,"");
  leg->AddEntry(fp2y,legis_2,"");
  leg->Draw("same");

  c1->Print("tof_fptime_ac_tol_10.pdf");

  // ******************************************************************

  TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
  c2->cd();
  beta->GetXaxis()->SetTitleSize(0.055);
  beta->GetYaxis()->SetTitleSize(0.055);
  beta->GetXaxis()->CenterTitle(1);
  beta->GetXaxis()->SetTitleOffset(0.8);
  beta->GetXaxis()->SetTitle("beta_good_track");
  beta->SetFillStyle(3009);
  beta->SetFillColor(kRed-7);
  beta->Draw();

  TF1 *gaus = new TF1("gaus","gaus",0.96,1.073);
  gaus->SetLineColor(kAzure);
  gaus->SetLineStyle(2);
  gaus->SetLineWidth(5);
  beta->Fit("gaus","N R");
  gaus->Draw("same"); 

  Float_t mean = gaus->GetParameter(1);
  Float_t sigma = gaus->GetParameter(2);

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);

  leg = new TLegend(0.15,0.75,0.3,0.85);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.045);
  leg->SetTextColor(kAzure);
  leg->SetBorderSize(0);
  leg->AddEntry(beta,legis_1,"");
  leg->AddEntry(beta,legis_2,"");
  leg->Draw("same");


  c2->Print("tof_beta_ac_tol_10.pdf");

  // ********************************************************

  TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
  c3->cd();
  betant->GetXaxis()->SetTitleSize(0.055);
  betant->GetYaxis()->SetTitleSize(0.055);
  betant->GetXaxis()->CenterTitle(1);
  betant->GetXaxis()->SetTitleOffset(0.8);
  betant->GetXaxis()->SetTitle("beta_no_track");
  betant->SetFillStyle(3009);
  betant->SetFillColor(kRed-7);
  betant->Draw();

  TF1 *gausn = new TF1("gausn","gaus",0.96,1.073);
  gausn->SetLineColor(kAzure);
  gausn->SetLineStyle(2);
  gausn->SetLineWidth(5);
  betant->Fit("gausn","N R");
  gausn->Draw("same"); 

  Float_t mean = gausn->GetParameter(1);
  Float_t sigma = gausn->GetParameter(2);

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);

  leg = new TLegend(0.15,0.75,0.3,0.85);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.045);
  leg->SetTextColor(kAzure);
  leg->SetBorderSize(0);
  leg->AddEntry(betant,legis_1,"");
  leg->AddEntry(betant,legis_2,"");
  leg->Draw("same");


  c3->Print("tof_betant_ac_tol_10.pdf");

  cout << endl << "Bye." << endl;
}
