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

void formatit(TH1F *histo, Double_t min, Double_t max, TString bar){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.11);
  gPad->SetRightMargin(0.01);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(7.03);
  
  histo->GetXaxis()->SetTitleOffset(1.);
  //    histo->GetYaxis()->SetRangeUser(0.,40.);
  histo->GetXaxis()->SetRangeUser(0.,600.);
  histo->SetLabelSize(0.055, "x");
  histo->SetLabelSize(0.055, "y");
  histo->GetXaxis()->SetTitleSize(0.065);
  histo->GetYaxis()->SetTitleSize(0.065);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(0.8);
  histo->GetXaxis()->SetTitle("pulse amplitude (mV)");
  histo->SetLineColor(kOrange+10);
  histo->SetLineWidth(2);
  histo->Draw("");  

  Double_t maxis = histo->GetMaximum();

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
  legis_3 = Form("mean-3*sigma = %4.2f", npe);

  leg = new TLegend(0.4,0.65,0.7,0.9);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.055);
  leg->SetTextColor(kAzure);
  leg->SetBorderSize(0);
  leg->AddEntry(histo,bar,"");
  leg->AddEntry(histo,legis_1,"");
  leg->AddEntry(histo,legis_2,"");
  leg->AddEntry(histo,legis_3,"");
  leg->Draw("same");

  TLine *line = new TLine(60.,0.,60.,maxis);
  line->SetLineColor(kGreen);
  line->SetLineWidth(3);
  line->Draw();
}

int main(){

TFile *myfile;
  Int_t run_num = 394;

  TString froot, filename, limitsfile, selectlimitsfile;
  froot = Form("../../ROOTfiles/phodo_replay_%d.root",run_num);

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
  
  TH1F *histo = 0;
  //  TH1F *histo = new TH1F("bla","",4097,0.,4096);      
  TH1F *histo7 = new TH1F("SHMS_Quartz_pos_7","",2000,0.,1000.);      
  TH1F *histo8 = new TH1F("SHMS_Quartz_pos_8","",2000,0.,1000.);      
  TH1F *histo9 = new TH1F("SHMS_Quartz_pos_9","",2000,0.,1000.);      
  TH1F *histo10 = new TH1F("SHMS_Quartz_pos_10","",2000,0.,1000.);      
  TH1F *histo11 = new TH1F("SHMS_Quartz_pos_11","",2000,0.,1000.);      
  TH1F *histo12 = new TH1F("SHMS_Quartz_pos_12","",2000,0.,1000.);      
  TH1F *histo13 = new TH1F("SHMS_Quartz_pos_13","",2000,0.,1000.);      
  TH1F *histo14 = new TH1F("SHMS_Quartz_pos_14","",2000,0.,1000.);      
  TH1F *histo15 = new TH1F("SHMS_Quartz_pos_15","",2000,0.,1000.);      
  
  for(Int_t i=0; i < nentries; i++){
    //    for(Int_t i=0; i < 10; i++){
    
    this_tree->GetEntry(i);
    
    for(UInt_t ip = 0; ip < nplanes; ip++) {
      //
      
      for(Int_t adc1_ihit = 0; adc1_ihit < nadchits[ip][1]; adc1_ihit++)  {
	UInt_t adc1bar = TMath::Nint(adc_paddles[ip][1][adc1_ihit]) - 1;
	
	if (ip == 3 && adc1bar == 6){
	  Double_t adc1val = 0.;
	  Double_t adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	  histo7->Fill(adc1val);
	} else if (ip == 3 && adc1bar == 7){
	  Double_t adc1val = 0.;
	  adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	  histo8->Fill(adc1val);
	} else if (ip == 3 && adc1bar == 8){
	  Double_t adc1val = 0.;
	  adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	  histo9->Fill(adc1val);
	} else if (ip == 3 && adc1bar == 9){
	  Double_t adc1val = 0.;
	  adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	  histo10->Fill(adc1val);
	} else if (ip == 3 && adc1bar == 10){
	  Double_t adc1val = 0.;
	  adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	  histo11->Fill(adc1val);
	} else if (ip == 3 && adc1bar == 11){
	  Double_t adc1val = 0.;
	  adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	  histo12->Fill(adc1val);
	} else if (ip == 3 && adc1bar == 12){
	  Double_t adc1val = 0.;
	  adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	  histo13->Fill(adc1val);
	} else if (ip == 3 && adc1bar == 13){
	  Double_t adc1val = 0.;
	  adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	  histo14->Fill(adc1val);
	} else if (ip == 3 && adc1bar == 14){
	    Double_t adc1val = 0.;
	    adc1val = adc_values[ip][1][4][adc1_ihit]*1.0/4096.*1000;
	    histo15->Fill(adc1val);
	} 
	else{
	  }; 
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
  formatit(histo7,160.,400.,"SHMS_Quartz_7B");
  c3->cd(2);
  formatit(histo8,220.,450.,"SHMS_Quartz_8B");
  c3->cd(3);
  formatit(histo9,100.,260.,"SHMS_Quartz_9B");
  c3->cd(4);
  formatit(histo10,180.,400.,"SHMS_Quartz_10B");
  c3->cd(5);
  formatit(histo11,180.,400.,"SHMS_Quartz_11B");   
  c3->cd(6);
  formatit(histo12,100.,300.,"SHMS_Quartz_12B");
  c3->cd(7);
  formatit(histo13,100.,320.,"SHMS_Quartz_13B");
  c3->cd(8);
  formatit(histo14,100.,300.,"SHMS_Quartz_14B");
  c3->cd(9);
  formatit(histo15,2.5,8.5,"SHMS_Quartz_15B"); 

  c3->Print("394_quartz_amp_bottom.png");
  c3->Print("394_quartz_amp_bottom.pdf");

  return 0;
  
}
