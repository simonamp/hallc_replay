#include <iostream>
#include <fstream>
#include <string>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TMatrixD.h>
#include <TROOT.h>
#include <TMatrixDBase.h>
#include <TMath.h>
#include <TChain.h>
#include <TH1F.h>
#include <TF1.h>
#include <cstdlib>

using namespace std;

static const double degtorad = 3.141592653589793/180.;

void formatit(TH2F *histo, Double_t min, Double_t max, TString bar){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.15);
  gPad->SetTopMargin(0.17);
  gPad->SetBottomMargin(7.03);
  
  histo->GetXaxis()->SetTitleOffset(1.);
  //    histo->GetYaxis()->SetRangeUser(0.,40.);
  histo->GetXaxis()->SetRangeUser(0.,600.);
  histo->SetLabelSize(0.045, "x");
  histo->SetLabelSize(0.045, "y");
  histo->GetXaxis()->SetTitleSize(0.055);
  histo->GetYaxis()->SetTitleSize(0.055);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(0.9);
  histo->GetYaxis()->SetTitleOffset(1.4);
  histo->GetXaxis()->SetTitle("Top TDC time (ns)");
  histo->GetYaxis()->SetTitle("Bottom TDC time (ns)");
  histo->SetLineColor(kOrange+10);
  histo->SetLineWidth(2);
  histo->Draw("colz");  

  /*  Double_t maxis = histo->GetMaximum();

  TF1 *gaus = new TF1("gaus","gaus",min,max);
  gaus->SetLineColor(kAzure);
  gaus->SetLineStyle(2);
  gaus->SetLineWidth(5);
  histo->Fit("gaus","N R");
  gaus->Draw("same"); 

  Float_t mean = gaus->GetParameter(1);
  Float_t sigma = gaus->GetParameter(2);
  Float_t npe = (mean-3*sigma);

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.2f", mean);
  legis_2 = Form("sigma = %4.2f", sigma);
  legis_3 = Form("mean-3*sigma = %4.2f", npe); */

  leg = new TLegend(0.24,0.65,0.54,0.9);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.055);
  leg->SetTextColor(kPink);
  leg->SetBorderSize(0);
  leg->AddEntry(histo,bar,"");
  leg->Draw("same"); 

  TLine *line = new TLine(-150.,-190.,-150.,10.);
  line->SetLineColor(kRed-5);
  line->SetLineWidth(5);
  line->SetLineStyle(3);
  line->Draw(); 
  TLine *line2 = new TLine(-190.,-150.,-110.,-150.);
  line2->SetLineColor(kRed-5);
  line2->SetLineWidth(5);
  line2->SetLineStyle(3);
  line2->Draw(); 
}

int main(){

TFile *myfile;
  Int_t run_num = 464;

  TString froot, filename, limitsfile, selectlimitsfile;
  //    froot = Form("../../ROOTfiles/phodo_replay_%d.root",run_num);
  froot = Form("/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/shms_replay_%d_-1.root",run_num);

  file = new TFile(froot, "READ");
  
  this_tree = new TTree;
  this_tree = (TTree *)file->Get("T");

  const UInt_t nplanes = 4;
  const UInt_t nsides = 2;
  const UInt_t maxbars  = 21;
  const UInt_t nsignals = 2;
  const UInt_t nadcsignals = 7;
  const UInt_t ntdcsignals = 2;
  const UInt_t maxhits = 128*16;

  const TString plane_names[nplanes] = {"1x", "1y", "2x", "2y"};
  const UInt_t  nbars[nplanes] = {13, 13, 14, 21};
  const TString sides[nsides] = {"neg", "pos"};
  const TString signals[nsides] = {"Adc", "Tdc"};
  const TString adc_signals[nadcsignals] = {"Ped", "PedRaw", "PulseInt", "PulseIntRaw", 
					    "PulseAmp", "PulseAmpRaw", "PulseTimeRaw"};
  const TString tdc_signals[ntdcsignals] = {"Time", "TimeRaw"};
  
  Int_t nadchits[nplanes][nsides];
  Int_t ntdchits[nplanes][nsides];

  Double_t adc_paddles[nplanes][nsides][maxhits];
  Double_t tdc_paddles[nplanes][nsides][maxhits];

  Double_t adc_values[nplanes][nsides][nadcsignals][maxhits];
  Double_t tdc_values[nplanes][nsides][ntdcsignals][maxhits];  
  
  for(UInt_t ip = 0; ip < nplanes; ip++) {
    
    for(UInt_t iside = 0; iside < nsides; iside++) {
      
      for(UInt_t ibar = 0; ibar < nbars[ip]; ibar++) {
      }
      
      for(UInt_t isignal = 0; isignal < nsignals; isignal++) {
	
	if(signals[isignal] == "Adc") {  
	  TString adc_base_name = "P.hod."+
	    plane_names[ip]+"."+sides[iside]+signals[isignal];
	  TString adc_ndata_name = "Ndata."+adc_base_name+"Counter";
	  T->SetBranchAddress(adc_ndata_name, &nadchits[ip][iside]);
	  TString adc_paddle_name = adc_base_name+"Counter";
	  T->SetBranchAddress(adc_paddle_name, &adc_paddles[ip][iside][0]);
	  for (UInt_t iadcsignal = 0; iadcsignal < nadcsignals; iadcsignal++) {
	    if (adc_signals[iadcsignal] == "Ped") {
	      TString pedlist_name = adc_base_name+"Ped";
	      T->SetBranchAddress(pedlist_name, &adc_values[ip][iside][iadcsignal][0]);
	    }
	    if (adc_signals[iadcsignal] == "PedRaw") {
	      TString pedrawlist_name = adc_base_name+"PedRaw";
	      T->SetBranchAddress(pedrawlist_name, &adc_values[ip][iside][iadcsignal][0]);
	    }
	    if (adc_signals[iadcsignal] == "PulseInt") {   
	      TString intlist_name = adc_base_name+"PulseInt";
	      T->SetBranchAddress(intlist_name, &adc_values[ip][iside][iadcsignal][0]);
	    }
	    if (adc_signals[iadcsignal] == "PulseIntRaw") {
	      TString intrawlist_name = adc_base_name+"PulseIntRaw";
	      T->SetBranchAddress(intrawlist_name, &adc_values[ip][iside][iadcsignal][0]);
	    }
	    if (adc_signals[iadcsignal] == "PulseAmp") {   
	      TString amplist_name = adc_base_name+"PulseAmp";
	      T->SetBranchAddress(amplist_name, &adc_values[ip][iside][iadcsignal][0]);
	    }
	    if (adc_signals[iadcsignal] == "PulseAmpRaw") {
	      TString amprawlist_name = adc_base_name+"PulseAmpRaw";
	      T->SetBranchAddress(amprawlist_name, &adc_values[ip][iside][iadcsignal][0]);
	    }
	    if (adc_signals[iadcsignal] == "PulseTimeRaw") {
	      TString ptrawlist_name = adc_base_name+"PulseTimeRaw";
	      T->SetBranchAddress(ptrawlist_name, &adc_values[ip][iside][iadcsignal][0]);
	    }
	  }
	}
	
	if(signals[isignal] == "Tdc") {  
	  TString tdc_base_name = "P.hod."+
	    plane_names[ip]+"."+sides[iside]+signals[isignal];
	  TString tdc_ndata_name = "Ndata."+tdc_base_name+"Counter";
	  T->SetBranchAddress(tdc_ndata_name, &ntdchits[ip][iside]);
	  TString tdc_paddle_name = tdc_base_name+"Counter";
	  T->SetBranchAddress(tdc_paddle_name, &tdc_paddles[ip][iside][0]);
	  for (UInt_t itdcsignal = 0; itdcsignal < ntdcsignals; itdcsignal++) {
	    if (tdc_signals[itdcsignal] == "Time") {
	      TString tdclist_name = tdc_base_name+"Time";
	      T->SetBranchAddress(tdclist_name, &tdc_values[ip][iside][itdcsignal][0]);
	    }
	    if (tdc_signals[itdcsignal] == "TimeRaw") {
	      TString tdcrawlist_name = tdc_base_name+"TimeRaw";
	      T->SetBranchAddress(tdcrawlist_name, &tdc_values[ip][iside][itdcsignal][0]);
	    }
	  }
	}
	
      }
    }
  }


  Int_t nentries = (Int_t)this_tree->GetEntries();
  
  //  delete histo7, histo8, histo9, histo10, histo11, histo12, histo13, histo8, histo15;

  TH2F *histo7 = new TH2F("SHMS_Quartz_pos_7","",80,-190,-110.,80,-190,-110.);      
  TH2F *histo8 = new TH2F("SHMS_Quartz_pos_8","",80,-190,-110.,80,-190.,-110.);      
  TH2F *histo9 = new TH2F("SHMS_Quartz_pos_9","",80,-190,-110.,80,-190.,-110.);      
  TH2F *histo10 = new TH2F("SHMS_Quartz_pos_10","",80,-190,-110.,80,-190.,-110.);      
  TH2F *histo11 = new TH2F("SHMS_Quartz_pos_11","",80,-190,-110.,80,-190.,-110.);      
  TH2F *histo12 = new TH2F("SHMS_Quartz_pos_12","",80,-190,-110.,80,-190.,-110.);      
  TH2F *histo13 = new TH2F("SHMS_Quartz_pos_13","",80,-190,-110.,80,-190.,-110.);      
  TH2F *histo14 = new TH2F("SHMS_Quartz_pos_14","",80,-190,-110.,80,-190.,-110.);      
  TH2F *histo15 = new TH2F("SHMS_Quartz_pos_15","",80,-190,-110.,80,-190.,-110.);      


  
  //  for(Int_t i=0; i < nentries; i++){
  for(Int_t i=0; i < 100000; i++){
  //  for(Int_t i=0; i < 2000; i++){
    
    this_tree->GetEntry(i);
    
    for(UInt_t ip = 0; ip < nplanes; ip++) {

      if (ip == 3){ 
	
	for(Int_t tdc1_ihit = 0; tdc1_ihit < ntdchits[ip][1]; tdc1_ihit++)  {
	  UInt_t tdc1bar = TMath::Nint(tdc_paddles[ip][1][tdc1_ihit]) - 1;
	  
	  if (tdc1bar == 6){
	    Int_t countit = 0;
	    Double_t tdc1val = 0.;
	    Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	    
	    for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	      UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	      if (tdc2bar == 6){
		Double_t tdc2val = 0.;
		Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
		Double_t diff_pos_neg = tdc1val - tdc2val;
		histo7->Fill(tdc2val,tdc1val);
		//	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
		countit++;
		//		cout << "twice here?  " << countit << endl; 
	      }
	    }
	} else if (tdc1bar == 7){
	  Int_t countit = 0;
	  Double_t tdc1val = 0.;
	  Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	  
	  for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	    UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	    if (tdc2bar == 7){
	      Double_t tdc2val = 0.;
	      Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
	      Double_t diff_pos_neg = tdc1val - tdc2val;
	      histo8->Fill(tdc2val,tdc1val);
	      //	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
	      countit++;
	      //	      cout << "twice here?  " << countit << endl; 
	    }
	  }
	}  else if (tdc1bar == 8){
	  Int_t countit = 0;
	  Double_t tdc1val = 0.;
	  Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	  
	  for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	    UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	    if (tdc2bar == 8){
	      Double_t tdc2val = 0.;
	      Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
	      Double_t diff_pos_neg = tdc1val - tdc2val;
	      histo9->Fill(tdc2val,tdc1val);
	      //	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
	      countit++;
	      //	      cout << "twice here?  " << countit << endl; 
	    }
	  }
	} else if (tdc1bar == 9){
	  Int_t countit = 0;
	  Double_t tdc1val = 0.;
	  Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	  
	  for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	    UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	    if (tdc2bar == 9){
	      Double_t tdc2val = 0.;
	      Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
	      Double_t diff_pos_neg = tdc1val - tdc2val;
	      histo10->Fill(tdc2val,tdc1val);
	      //	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
	      countit++;
	      //	      cout << "twice here?  " << countit << endl; 
	    }
	  }
	} else if (tdc1bar == 10){
	  Int_t countit = 0;
	  Double_t tdc1val = 0.;
	  Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	  
	  for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	    UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	    if (tdc2bar == 10){
	      Double_t tdc2val = 0.;
	      Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
	      Double_t diff_pos_neg = tdc1val - tdc2val;
	      histo11->Fill(tdc2val,tdc1val);
	      //	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
	      countit++;
	      //	      cout << "twice here?  " << countit << endl; 
	    }
	  }
	} else if (tdc1bar == 11){
	  Int_t countit = 0;
	  Double_t tdc1val = 0.;
	  Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	  
	  for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	    UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	    if (tdc2bar == 11){
	      Double_t tdc2val = 0.;
	      Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
	      Double_t diff_pos_neg = tdc1val - tdc2val;
	      histo12->Fill(tdc2val,tdc1val);
	      //	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
	      countit++;
	      //	      cout << "twice here?  " << countit << endl; 
	    }
	  }
	} else if (tdc1bar == 12){
	  Int_t countit = 0;
	  Double_t tdc1val = 0.;
	  Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	  
	  for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	    UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	    if (tdc2bar == 12){
	      Double_t tdc2val = 0.;
	      Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
	      Double_t diff_pos_neg = tdc1val - tdc2val;
	      histo13->Fill(tdc2val,tdc1val);
	      //	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
	      countit++;
	      //	      cout << "twice here?  " << countit << endl; 
	    }
	  }
	} else if (tdc1bar == 13){
	  Int_t countit = 0;
	  Double_t tdc1val = 0.;
	  Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	  
	  for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	    UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	    if (tdc2bar == 13){
	      Double_t tdc2val = 0.;
	      Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
	      Double_t diff_pos_neg = tdc1val - tdc2val;
	      histo14->Fill(tdc2val,tdc1val);
	      //	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
	      countit++;
	      //	      cout << "twice here?  " << countit << endl; 
	    }
	  }
	} else if (tdc1bar == 14){
	    Int_t countit = 0;
	    Double_t tdc1val = 0.;
	    Double_t tdc1val = tdc_values[ip][1][0][tdc1_ihit]*0.1;
	    
	    for(Int_t tdc2_ihit = 0; tdc2_ihit < ntdchits[ip][0]; tdc2_ihit++)  {
	      UInt_t tdc2bar = TMath::Nint(tdc_paddles[ip][0][tdc2_ihit]) - 1;
	      if (tdc2bar == 14){
		Double_t tdc2val = 0.;
		Double_t tdc2val = tdc_values[ip][0][0][tdc2_ihit]*0.1;
		Double_t diff_pos_neg = tdc1val - tdc2val;
		histo15->Fill(tdc2val,tdc1val);
		//	      cout << " " << tdc2val << " " << diff_pos_neg << endl;
		countit++;
		//		cout << "twice here?  " << countit << endl; 
	      }
	    }
	  } else {} 
	}
      }
    }
  }
  
  
  gStyle->SetOptStat(kFALSE);
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPalette(1,0);
  
  TCanvas *c3 = new TCanvas("c3","",900,900);
  
  c3->Divide(3,3);
  c3->cd(1);
  formatit(histo7,160.,400.,"SHMS_Quartz_7");
  c3->cd(2);
  formatit(histo8,220.,450.,"SHMS_Quartz_8");
  c3->cd(3);
  formatit(histo9,100.,260.,"SHMS_Quartz_9");
  c3->cd(4);
  formatit(histo10,180.,400.,"SHMS_Quartz_10");
  c3->cd(5);
  formatit(histo11,180.,400.,"SHMS_Quartz_11");   
  c3->cd(6);
  formatit(histo12,100.,300.,"SHMS_Quartz_12");
  c3->cd(7);
  formatit(histo13,100.,320.,"SHMS_Quartz_13");
  c3->cd(8);
  formatit(histo14,100.,300.,"SHMS_Quartz_14");
  c3->cd(9);
  formatit(histo15,2.5,8.5,"SHMS_Quartz_15"); 

  c3->Print("464_quartz_tdc_time_bottom.png");
  c3->Print("464_quartz_tdc_time_bottom.pdf"); 

  return 0;
  
}
