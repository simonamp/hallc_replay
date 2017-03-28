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
  histo->GetXaxis()->SetRangeUser(0.,800.);
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
  TF1 *gaus = new TF1("gaus","gaus",min,max);
  gaus->SetLineColor(kBlack);
  gaus->SetLineStyle(2);
  gaus->SetLineWidth(5);
  histo->Fit("gaus","N R");
  gaus->Draw("same"); 

  Float_t mean = gaus->GetParameter(1);
  Float_t sigma = gaus->GetParameter(2);
  Float_t npe = (mean-3*sigma);

  Double_t maxis = histo->GetMaximum();

  TString legis_1, legis_2, legis_3;
  legis_1 = Form("mean = %4.3f", mean);
  legis_2 = Form("sigma = %4.3f", sigma);
  legis_3 = Form("mean-3*sigma = %4.3f", npe);

  leg = new TLegend(0.30,0.7,0.70,0.93);
  leg->SetFillColor(-1);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.055);
  leg->SetTextColor(kBlack);
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

  TH1F *histo7 = new TH1F("SHMS_Quartz_neg_7","",1000,0.,1000.);      
  TH1F *histo8 = new TH1F("SHMS_Quartz_neg_8","",1000,0.,1000.);      
  TH1F *histo9 = new TH1F("SHMS_Quartz_neg_9","",1000,0.,1000.);      
  TH1F *histo10 = new TH1F("SHMS_Quartz_neg_10","",1000,0.,1000.);      
  TH1F *histo11 = new TH1F("SHMS_Quartz_neg_11","",1000,0.,1000.);      
  TH1F *histo12 = new TH1F("SHMS_Quartz_neg_12","",1000,0.,1000.);      
  TH1F *histo13 = new TH1F("SHMS_Quartz_neg_13","",1000,0.,1000.);      
  TH1F *histo14 = new TH1F("SHMS_Quartz_neg_14","",1000,0.,1000.);      
  TH1F *histo15 = new TH1F("SHMS_Quartz_neg_15","",1000,0.,1000.);      
  
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
	    
	    //	    if (adc1bar == 6 && (track_y[0] + track_yp[0] * 276.35) > 15. && (track_y[0] + track_yp[0] * 276.35) < 24. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    if (adc1bar == 6 && (track_y[0] + track_yp[0] * 276.35) > 15. && (track_y[0] + track_yp[0] * 276.35) < 24. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      Double_t adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo7->Fill(adc1val);

	      //	    } else if (adc1bar == 7 && (track_y[0] + track_yp[0] * 276.35) > 9. && (track_y[0] + track_yp[0] * 276.35) < 19. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    } else if (adc1bar == 7 && (track_y[0] + track_yp[0] * 276.35) > 9. && (track_y[0] + track_yp[0] * 276.35) < 19. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo8->Fill(adc1val);

	      //	    } else if (adc1bar == 8 && (track_y[0] + track_yp[0] * 276.35) > 5. && (track_y[0] + track_yp[0] * 276.35) < 14. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    } else if (adc1bar == 8 && (track_y[0] + track_yp[0] * 276.35) > 5. && (track_y[0] + track_yp[0] * 276.35) < 14. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo9->Fill(adc1val);

	      //	    } else if (adc1bar == 9 && (track_y[0] + track_yp[0] * 276.35) > 0. && (track_y[0] + track_yp[0] * 276.35) < 7. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    } else if (adc1bar == 9 && (track_y[0] + track_yp[0] * 276.35) > 0. && (track_y[0] + track_yp[0] * 276.35) < 7. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo10->Fill(adc1val);

	      //	    } else if (adc1bar == 10 && (track_y[0] + track_yp[0] * 276.35) > -4. && (track_y[0] + track_yp[0] * 276.35) < 4. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    } else if (adc1bar == 10 && (track_y[0] + track_yp[0] * 276.35) > -4. && (track_y[0] + track_yp[0] * 276.35) < 4. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo11->Fill(adc1val);

	      //	    } else if (adc1bar == 11 && (track_y[0] + track_yp[0] * 276.35) > -10. && (track_y[0] + track_yp[0] * 276.35) < -1. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    } else if (adc1bar == 11 && (track_y[0] + track_yp[0] * 276.35) > -10. && (track_y[0] + track_yp[0] * 276.35) < -1. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo12->Fill(adc1val);

	      //	    } else if (adc1bar == 12 && (track_y[0] + track_yp[0] * 276.35) > -15. && (track_y[0] + track_yp[0] * 276.35) < -6. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    } else if (adc1bar == 12 && (track_y[0] + track_yp[0] * 276.35) > -15. && (track_y[0] + track_yp[0] * 276.35) < -6. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo13->Fill(adc1val);

	      //	    } else if (adc1bar == 13 && (track_y[0] + track_yp[0] * 276.35) > -19. && (track_y[0] + track_yp[0] * 276.35) < -11. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    } else if (adc1bar == 13 && (track_y[0] + track_yp[0] * 276.35) > -19. && (track_y[0] + track_yp[0] * 276.35) < -11. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo14->Fill(adc1val);

	      //	    } else if (adc1bar == 14 && (track_y[0] + track_yp[0] * 276.35) > -24. && (track_y[0] + track_yp[0] * 276.35) < 17. && (track_x[0] + track_xp[0] * 276.35) < 0.){
	    } else if (adc1bar == 14 && (track_y[0] + track_yp[0] * 276.35) > -24. && (track_y[0] + track_yp[0] * 276.35) < 17. && (track_x[0] + track_xp[0] * 276.35) > 0.){
	      Double_t adc1val = 0.;
	      adc1val = adc_values[ip][0][4][adc1_ihit] * 1.0/4096.*1000;
	      histo15->Fill(adc1val);

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
  //  formatit(histo7,110.,600.,"SHMS_S2Y_7Neg"); // less than
  formatit(histo7,20.,280.,"SHMS_S2Y_7Neg"); // more than

  c3->cd(2);
  //  formatit(histo8,180.,410.,"SHMS_S2Y_8Neg"); // less than
  formatit(histo8,110.,384.,"SHMS_S2Y_8Neg"); // more than

  c3->cd(3);
  //  formatit(histo9,75.,400,"SHMS_S2Y_9Neg"); // less than
  formatit(histo9,7.,240,"SHMS_S2Y_9Neg"); // more than

  c3->cd(4);
  //  formatit(histo10,80.,380.,"SHMS_S2Y_10Neg"); // less than
  formatit(histo10,20.,280.,"SHMS_S2Y_10Neg"); // more than

  c3->cd(5);
  //  formatit(histo11,200.,540.,"SHMS_S2Y_11Neg"); // less than   
  formatit(histo11,160.,422.,"SHMS_S2Y_11Neg"); // more than   

  c3->cd(6);
  //  formatit(histo12,200,510,"SHMS_S2Y_12Neg"); // less than
  formatit(histo12,100,377,"SHMS_S2Y_12Neg"); // more than

  c3->cd(7);
  //  formatit(histo13,110,280,"SHMS_S2Y_13Neg"); // less than
  formatit(histo11,130,423,"SHMS_S2Y_13Neg"); // more than

  c3->cd(8);
  //  formatit(histo14,300,680,"SHMS_S2Y_14Neg"); // less than
  formatit(histo14,100,410,"SHMS_S2Y_14Neg"); // more than

  c3->cd(9);
  //  formatit(histo15,40,120,"SHMS_S2Y_15Neg"); // less than
  formatit(histo15,40,120,"SHMS_S2Y_15Neg"); // more than

  c3->Print("488_fresh_quartz_pint_ampl_neg_withc_xgt0.png");
  c3->Print("488_fresh_replay_quartz_pint_ampl_neg_withc_xgt0.pdf");

  return 0;
  
}
