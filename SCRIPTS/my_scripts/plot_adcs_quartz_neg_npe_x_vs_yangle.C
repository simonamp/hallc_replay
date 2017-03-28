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
  gPad->SetLeftMargin(0.11);
  gPad->SetRightMargin(0.11);
  gPad->SetTopMargin(0.03);
  gPad->SetBottomMargin(7.03);
  
  //  histo->GetXaxis()->SetTitleOffset(1.);
  //    histo->GetYaxis()->SetRangeUser(0.,40.);
  //  histo->GetXaxis()->SetRangeUser(0.,200.);
  /*  histo->SetLabelSize(0.055, "x");
  histo->SetLabelSize(0.055, "y");
  histo->GetXaxis()->SetTitleSize(0.065);
  histo->GetYaxis()->SetTitleSize(0.065);
  histo->GetXaxis()->CenterTitle(1);
  histo->GetXaxis()->SetTitleOffset(0.8);
  histo->GetXaxis()->SetTitle("charge from pulse integral (pC)");
  histo->SetLineColor(kOrange+10);
  histo->SetLineWidth(2); */
  histo->Draw("colz");  
  /*  TF1 *gaus = new TF1("gaus","gaus",min,max);
  gaus->SetLineColor(kAzure);
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
  legis_3 = Form("n.p.e = %4.3f", npe);

  leg = new TLegend(0.45,0.7,0.75,0.93);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.055);
  leg->SetTextColor(kAzure);
  leg->SetBorderSize(0);
  leg->AddEntry(histo,bar,"");
  leg->AddEntry(histo,legis_1,"");
  leg->AddEntry(histo,legis_2,"");
  leg->AddEntry(histo,legis_3,"");
  leg->Draw("same"); */
}

int main(){

TFile *myfile;
 Int_t run_num = 488;
// Int_t run_num = 393;

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
  
  //TH1F *histo = 0;
  //  TH1F *histo = new TH1F("bla","",4097,0.,4096);      
  TH2F *histo7 = new TH2F("SHMS_Quartz_neg_7","",280,-70.,70.,100,-0.04.,0.04);      
  TH2F *histo8 = new TH2F("SHMS_Quartz_neg_8","",280,-70.,70.,100,-0.04,0.04);      
  TH2F *histo9 = new TH2F("SHMS_Quartz_neg_9","",280,-70.,70.,100,-0.04,0.04);      
  TH2F *histo10 = new TH2F("SHMS_Quartz_neg_10","",280,-70.,70.,100,-0.04,0.04);      
  TH2F *histo11 = new TH2F("SHMS_Quartz_neg_11","",280,-70.,70.,100,-0.04,0.04);      
  TH2F *histo12 = new TH2F("SHMS_Quartz_neg_12","",280,-70.,70.,100,-0.04,0.04);      
  TH2F *histo13 = new TH2F("SHMS_Quartz_neg_13","",280,-70.,70.,100,-0.04,0.04);      
  TH2F *histo14 = new TH2F("SHMS_Quartz_neg_14","",280,-70.,70.,100,-0.04,0.04);      
  TH2F *histo15 = new TH2F("SHMS_Quartz_neg_15","",280,-70.,70.,100,-0.04,0.04);      



  
  for(Int_t i=0; i < nentries; i++){
    //  for(Int_t i=0; i < 400000; i++){
    //      for(Int_t i=0; i < 10; i++){
    
    this_tree->GetEntry(i);
    
    if (nTracksx == 1 && nTracksy ==1){
      
      for(UInt_t ip = 0; ip < nplanes; ip++) {
	//
	
	if (ip == 3){
	  
	  for(Int_t adc1_ihit = 0; adc1_ihit < nadchits[ip][0]; adc1_ihit++)  {
	    UInt_t adc1bar = TMath::Nint(adc_paddles[ip][0][adc1_ihit]) - 1;
	    
	    if (adc1bar == 6){
	      //	if (ip == 3 && adc1bar == 6 && adc_values[ip][1][2][adc1_ihit] > 0.){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      Double_t adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo7->Fill(xis,track_yp[0],adc1val);
	      //	      histo7nw->Fill(xis,yis);
	      //	      histo7dv->Divide(histo7,histo7nw);
	    } else if (adc1bar == 7){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo8->Fill(xis,track_yp[0],adc1val);
	      //	      histo8->Fill(xis,yis);
	      //	  histo8->Divide(histo8nw);
	    } else if (adc1bar == 8){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo9->Fill(xis,track_yp[0],adc1val);
	    } else if (adc1bar == 9){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo10->Fill(xis,track_yp[0],adc1val);
	    } else if (adc1bar == 10){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo11->Fill(xis,track_yp[0],adc1val);
	    } else if (adc1bar == 11){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo12->Fill(xis,track_yp[0],adc1val);
	    } else if (adc1bar == 12){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo13->Fill(xis,track_yp[0],adc1val);
	    } else if (adc1bar == 13){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo14->Fill(xis,track_yp[0],adc1val);
	    } else if (adc1bar == 14){
	      Double_t adc1val = 0.;
	      Double_t xis = 0;
	      Double_t yis = 0;
	      adc1val = adc_values[ip][0][2][adc1_ihit] * 0.02;
	      Double_t xis = track_x[0] + track_xp[0] * 276.35;
	      Double_t yis = track_y[0] + track_yp[0] * 276.35;
	      histo15->Fill(xis,track_yp[0],adc1val);
	    } 
	    else{
	    }; 
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
  //  formatit(histo7,60.,130.,"SHMS_Quartz_7Pos");
  formatit(histo7,45.,120.,"SHMS_Quartz_7Neg");
  c3->cd(2);
  //  formatit(histo8,80.,160.,"SHMS_Quartz_8Pos");
  formatit(histo8,25.,60.,"SHMS_Quartz_8Neg");
  c3->cd(3);
  //  formatit(histo9,40.,85,"SHMS_Quartz_9Pos");
  formatit(histo9,42.,120,"SHMS_Quartz_9Neg");
  c3->cd(4);
  //  formatit(histo10,60.,160.,"SHMS_Quartz_10Pos");
  formatit(histo10,30.,110.,"SHMS_Quartz_10Neg");
  c3->cd(5);
  //  formatit(histo11,58.,120.,"SHMS_Quartz_11Pos");   
  formatit(histo11,45.,110.,"SHMS_Quartz_11Neg");   
  c3->cd(6);
  //  formatit(histo12,40,90,"SHMS_Quartz_12Pos");
  formatit(histo12,38,130,"SHMS_Quartz_12Neg");
  c3->cd(7);
  //  formatit(histo13,40,100,"SHMS_Quartz_13Pos");
  formatit(histo13,40,125,"SHMS_Quartz_13Neg");
  c3->cd(8);
  //  formatit(histo14,40,95,"SHMS_Quartz_14Pos");
  formatit(histo14,50,150,"SHMS_Quartz_14Neg");
  c3->cd(9);
  formatit(histo15,25,85,"SHMS_Quartz_15Neg"); 

  c3->Print("488_fresh_quartz_pint_npe_neg_x_vs_yangle.png");
  c3->Print("488_fresh_replay_quartz_pint_npe_neg_x_vs_yangle.pdf");

  return 0;
  
}
