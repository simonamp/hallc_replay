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


void formatit(TH2F *histo, Double_t min, Double_t max, TString bar, Double_t xc, Double_t yc, Double_t rx, Double_t ry){

  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(0.11);
  gPad->SetRightMargin(0.01);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(7.03);
  
  //  histo->GetXaxis()->SetTitleOffset(1.);
  //  histo->GetYaxis()->SetRangeUser(0.,140.);
  //  histo->GetXaxis()->SetRangeUser(0.,140.);
  //histo->SetLabelSize(0.055, "x");
  //histo->SetLabelSize(0.055, "y");
  //histo->GetXaxis()->SetTitleSize(0.065);
  //histo->GetYaxis()->SetTitleSize(0.065);
  //histo->GetXaxis()->CenterTitle(1);
  //histo->GetXaxis()->SetTitleOffset(0.8);
  //histo->GetXaxis()->SetTitle("charge from pulse integral (pC)");
  //histo->SetLineColor(kOrange+10);
  //histo->SetLineWidth(2);
  histo->Draw("colz");
  /*  TF1 *gaus = new TF1("gaus","gaus",min,max);
  gaus->SetLineColor(kBlack);
  gaus->SetLineStyle(2);
  gaus->SetLineWidth(5);
  histo->Fit("gaus","N R");
  gaus->Draw("same"); 

  Float_t mean = gaus->GetParameter(1);
  Float_t sigma = gaus->GetParameter(2);
  Float_t npe = (mean/sigma)*(mean/sigma);

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);
  legis_3 = Form("n.p.e = %4.3f", npe); */

  //leg = new TLegend(0.45,0.7,0.75,0.93);
  //leg->SetFillColor(-1);
  //leg->SetFillStyle(0);
  //leg->SetTextSize(0.055);
  //leg->SetTextColor(kBlack);
  //leg->SetBorderSize(0);
  //leg->AddEntry(histo,bar,"");
//  leg->AddEntry(histo,legis_1,"");
//  leg->AddEntry(histo,legis_2,"");
//  leg->AddEntry(histo,legis_3,"");
//leg->Draw("same");

  ell_ch3 = new TEllipse(xc,yc,rx,ry,0.,360.,135.);
  ell_ch3->SetLineColor(2);
  ell_ch3->SetLineStyle(1);
  ell_ch3->SetLineWidth(2);
  ell_ch3->SetFillStyle(0);
  ell_ch3->Draw("same");
}

int main(){
  
  TFile *myfile;
  Int_t run_num = 488;
  
  
  TString froot, filename, limitsfile, selectlimitsfile;
  //    froot = Form("../../ROOTfiles/phodo_replay_%d.root",run_num);
  froot = Form("/net/cdaq/cdaql3data/cdaq/hallc-online/ROOTfiles/shms_replay_%d_-1.root",run_num);
  //  froot = Form("../../../../mkjones/hallc_replay/ROOTfiles/phodo_replay_%d.root",run_num);
  
  file = new TFile(froot, "READ");
  
  this_tree = new TTree;
  this_tree = (TTree *)file->Get("T");

  Int_t nTracksx;
  Int_t nTracksy;
  Double_t track_x[1000];
  Double_t track_y[1000];
  Double_t track_xp[1000];
  Double_t track_yp[1000];


  T->SetBranchAddress("Ndata.P.dc.x", &nTracksx);
  T->SetBranchAddress("Ndata.P.dc.y", &nTracksy);
  T->SetBranchAddress("P.dc.x", track_x);        
  T->SetBranchAddress("P.dc.y", track_y);        
  T->SetBranchAddress("P.dc.xp", track_xp);      
  T->SetBranchAddress("P.dc.yp", track_yp);    


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

  Double_t pdctrkb;

  //  T->SetBranchAddress("pdc_trk_beta", pdctrkb);
  
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

  TH2F *histo7 = new TH2F("SHMS_Quartz_neg_7","",400.,0.,400.,400.,0.,400.);      
  TH2F *histo8 = new TH2F("SHMS_Quartz_neg_8","",400.,0.,400.,400.,0.,400.);      
  TH2F *histo9 = new TH2F("SHMS_Quartz_neg_9","",400.,0.,400.,400.,0.,400.);      
  TH2F *histo10 = new TH2F("SHMS_Quartz_neg_10","",400.,0.,400.,400.,0.,400.);      
  TH2F *histo11 = new TH2F("SHMS_Quartz_neg_11","",400.,0.,400.,400.,0.,400.);      
  TH2F *histo12 = new TH2F("SHMS_Quartz_neg_12","",400.,0.,400.,400.,0.,400.);      
  TH2F *histo13 = new TH2F("SHMS_Quartz_neg_13","",400.,0.,400.,400.,0.,400.);      
  TH2F *histo14 = new TH2F("SHMS_Quartz_neg_14","",400.,0.,400.,400.,0.,400.);      
  TH2F *histo15 = new TH2F("SHMS_Quartz_neg_15","",400.,0.,400.,400.,0.,400.);      
  
  for(Int_t i=0; i < nentries; i++){
  //  for(Int_t i=0; i < 400000; i++){
    //    for(Int_t i=0; i < 10; i++){
    
    this_tree->GetEntry(i);
    
    if (nTracksx == 1 && nTracksy ==1){
      
      for(UInt_t ip = 0; ip < nplanes; ip++) {
	//
	
	if (ip == 3){
	  
	  for(Int_t adc1_ihit = 0; adc1_ihit < nadchits[ip][0]; adc1_ihit++)  {
	    UInt_t adc1bar = TMath::Nint(adc_paddles[ip][0][adc1_ihit]) - 1;
	    
	    for(Int_t adc2_ihit = 0; adc2_ihit < nadchits[ip][1]; adc2_ihit++)  {
	      UInt_t adc2bar = TMath::Nint(adc_paddles[ip][1][adc2_ihit]) - 1;
	      
	      if (adc1bar == 6 && adc2bar == 6 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//		if (adc1bar == 6 && adc2bar == 6 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		Double_t adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		Double_t adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo7->Fill(adc1val,adc2val);
		
	      } else if (adc1bar == 7 && adc2bar == 7 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//	} else if (adc1bar == 7 && adc2bar == 7 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo8->Fill(adc1val,adc2val);
		
	      } else if (adc1bar == 8 && adc2bar == 8 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//	} else if (adc1bar == 8 && adc2bar == 8 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo9->Fill(adc1val,adc2val);
		
	      } else if (adc1bar == 9 && adc2bar == 9 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//	} else if (adc1bar == 9 && adc2bar == 9 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo10->Fill(adc1val,adc2val);
		
	      } else if (adc1bar == 10 && adc2bar == 10 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//	} else if (adc1bar == 10 && adc2bar == 10 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo11->Fill(adc1val,adc2val);
		
	      } else if (adc1bar == 11 && adc2bar == 11 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//	} else if (adc1bar == 11 && adc2bar == 11 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo12->Fill(adc1val,adc2val);
		
	      } else if (adc1bar == 12 && adc2bar == 12 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//	} else if (adc1bar == 12 && adc2bar == 12 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo13->Fill(adc1val,adc2val);
		
	      } else if (adc1bar == 13 && adc2bar == 13 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//	} else if (adc1bar == 13 && adc2bar == 13 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo14->Fill(adc1val,adc2val);
		
	      } else if (adc1bar == 14 && adc2bar == 14 && (track_x[0] + track_xp[0] * 276.35) < 0.){
		//	} else if (adc1bar == 14 && adc2bar == 14 && (track_x[0] + track_xp[0] * 276.35) > 0.){
		Double_t adc1val = 0.;
		adc1val = adc_values[ip][0][6][adc1_ihit] * 0.0625;
		Double_t adc2val = 0.;
		adc2val = adc_values[ip][1][6][adc2_ihit] * 0.0625;
		histo15->Fill(adc1val,adc2val);
		
	      } 
	      else{
	      }; 
	      
	    }
	  }
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
  formatit(histo7,30.,190.,"SHMS_S2Y_7Neg", 73.,77., 10., 27.); // less than
  //  formatit(histo7,30.,190.,"SHMS_S2Y_7Neg", 73.,72., 10., 27.); // more than


  c3->cd(2);
  formatit(histo8,70.,155.,"SHMS_S2Y_8Neg", 75.,73., 10., 27.); // less than
  //  formatit(histo8,70.,155.,"SHMS_S2Y_8Neg", 75.,68., 10., 27.); // more than


  c3->cd(3);
  formatit(histo9,20.,140,"SHMS_S2Y_9Neg", 73.,79., 9., 26.); // less than
  //  formatit(histo9,20.,140,"SHMS_S2Y_9Neg", 73.,74., 9., 26.); // more than


  c3->cd(4);
  formatit(histo10,25.,160.,"SHMS_S2Y_10Neg", 74.,77., 10., 26.); // less than
  //  formatit(histo10,25.,160.,"SHMS_S2Y_10Neg", 74.,72., 10., 26.); // more than


  c3->cd(5);
  formatit(histo11,68.,180.,"SHMS_S2Y_11Neg", 71.,78., 10., 26.); // less than   
  //  formatit(histo11,68.,180.,"SHMS_S2Y_11Neg", 71.,74., 10., 26.); // more than   


  c3->cd(6);
  formatit(histo12,70,170,"SHMS_S2Y_12Neg", 71.,78., 10., 26.); // less than
  //  formatit(histo12,70,170,"SHMS_S2Y_12Neg", 71.,73., 10., 26.); // more than


  c3->cd(7);
  formatit(histo13,35,88,"SHMS_S2Y_13Neg", 72.,77., 10., 26.); // less than
  //  formatit(histo13,35,88,"SHMS_S2Y_13Neg", 72.,72., 10., 26.); // more than


  c3->cd(8);
  formatit(histo14,100,210,"SHMS_S2Y_14Neg", 73.,75., 10., 26.); // less than
  //  formatit(histo14,100,210,"SHMS_S2Y_14Neg", 73.,68., 10., 26.); // more than


  c3->cd(9);
  formatit(histo15,40,185,"SHMS_S2Y_15Neg", 73.,69., 10., 26.); // less than
  //  formatit(histo15,40,185,"SHMS_S2Y_15Neg", 73.,61., 10., 26.); // more than

  
  //    c3->Print("488_fresh_quartz_pint_adcrawtime_neg_withc_xlt0_noneiny_zoomin.png");
  //    c3->Print("488_fresh_replay_quartz_pint_adcrawtime_neg_withc_xlt0_noneiny_zoomin.pdf");
  c3->Print("488_fresh_quartz_pint_adcrawtime_neg_withc_xlt0_noneiny.png");
  c3->Print("488_fresh_replay_quartz_pint_adcrawtime_neg_withc_xlt0_noneiny.pdf");
  
  return 0;
  
}
