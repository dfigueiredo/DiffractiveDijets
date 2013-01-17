#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TKey.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

//
// Efficiency and Systematic Plotter
//
////////////////////////////////////


void EffApplication(){
  Systematic("histo_effTriggerMultijetsRunB_RefDijet50U_And30U.root","file1.root","file2.root");
}


void EachCutEfficiency(TString name, bool logscale){

  TFile *l1  = TFile::Open(name);
  gStyle->SetOptStat(0);

  TCanvas* c1 = new TCanvas("c1","c1",700,500);
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9931857,0.9949239,NULL,"brNDC");

  if(logscale) c1->SetLogy(1);

  TH1F* h_without_cuts = (TH1F*)l1->Get("Events_without_cuts");
  TH1F* h_Trigger = (TH1F*)l1->Get("Events_with_trigger");
  TH1F* h_Triggerpresel = (TH1F*)l1->Get("Events_with_trigger_presel");
  TH1F* h_Triggerpreselvertex = (TH1F*)l1->Get("Events_with_trigger_presel_vertex");
  TH1F* h_step4_4 = (TH1F*)l1->Get("Events_All_step4_4");
  TH1F* h_step4_3 = (TH1F*)l1->Get("Events_All_step4_3");
  TH1F* h_step4_2 = (TH1F*)l1->Get("Events_All_step4_2");
  TH1F* h_step4_1 = (TH1F*)l1->Get("Events_All_step4_1");

  TH1F *ratiopresel = h_Trigger->Clone(); 
  ratiopresel->SetName("RatioPreSel");
  ratiopresel->Divide(h_Triggerpresel,h_Trigger,1.,1.,"B");
  ratiopresel->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiopresel->GetYaxis()->SetTitleOffset(1.1);
  ratiopresel->GetYaxis()->SetTitleSize(0.03);
  ratiopresel->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiopresel->GetXaxis()->SetRangeUser(0,0.8);
  ratiopresel->SetTitle("Efficiency, Cut: #sum E_HF^{+}<30 GeV and #sum E_HF^{-}<30 GeV");
  ratiopresel->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_presel.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_presel.C"));

  TH1F *ratiovertex = h_Trigger->Clone();
  ratiovertex->SetName("RatioVertex"); 
  ratiovertex->Divide(h_Triggerpreselvertex,h_Trigger,1.,1.,"B");
  ratiovertex->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiovertex->GetYaxis()->SetTitleOffset(1.1);
  ratiovertex->GetYaxis()->SetTitleSize(0.03);
  ratiovertex->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiovertex->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex");
  ratiovertex->GetXaxis()->SetRangeUser(0,0.8);
  ratiovertex->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_vertex.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_vertex.C"));

  TH1F *ratiostep4_4 = h_Trigger->Clone(); 
  ratiostep4_4->SetName("RatioStep4_4"); 
  ratiostep4_4->Divide(h_step4_4,h_Trigger,1.,1.,"B");
  ratiostep4_4->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiostep4_4->GetYaxis()->SetTitleOffset(1.1);
  ratiostep4_4->GetYaxis()->SetTitleSize(0.03);
  ratiostep4_4->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiostep4_4->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex and |#eta_{pf(max,min)}|<4");
  ratiostep4_4->GetXaxis()->SetRangeUser(0,0.8);
  ratiostep4_4->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_4.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_4.C"));

  TH1F *ratiostep4_3 = h_Trigger->Clone(); 
  ratiostep4_3->SetName("RatioStep4_3"); 
  ratiostep4_3->Divide(h_step4_3,h_Trigger,1.,1.,"B");
  ratiostep4_3->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiostep4_3->GetYaxis()->SetTitleOffset(1.1);
  ratiostep4_3->GetYaxis()->SetTitleSize(0.03);
  ratiostep4_3->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiostep4_3->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex and |#eta_{pf(max,min)}|<3");
  ratiostep4_3->GetXaxis()->SetRangeUser(0,0.8);
  ratiostep4_3->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_3.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_3.C"));

  TH1F *ratiostep4_2 = h_Trigger->Clone(); 
  ratiostep4_2->SetName("RatioStep4_2"); 
  ratiostep4_2->Divide(h_step4_2,h_Trigger,1.,1.,"B");
  ratiostep4_2->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiostep4_2->GetYaxis()->SetTitleOffset(1.1);
  ratiostep4_2->GetYaxis()->SetTitleSize(0.03);
  ratiostep4_2->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiostep4_2->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex and |#eta_{pf(max,min)}|<2");
  ratiostep4_2->GetXaxis()->SetRangeUser(0,0.8);
  ratiostep4_2->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_2.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_2.C"));

  TH1F *ratiostep4_1 = h_Trigger->Clone(); 
  ratiostep4_1->SetName("RatioStep4_1"); 
  ratiostep4_1->Divide(h_step4_1,h_Trigger,1.,1.,"B");
  ratiostep4_1->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiostep4_1->GetYaxis()->SetTitleOffset(1.1);
  ratiostep4_1->GetYaxis()->SetTitleSize(0.03);
  ratiostep4_1->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiostep4_1->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex and |#eta_{pf(max,min)}|<1");
  ratiostep4_1->GetXaxis()->SetRangeUser(0,0.8);
  ratiostep4_1->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_1.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_1.C"));

  TString filenameroot(name);
  File = new TFile(filenameroot.TString::ReplaceAll("histo_effCuts","eff"),"RECREATE");
  File->cd();
  ratiopresel->Write();
  ratiovertex->Write();
  ratiostep4_4->Write();
  ratiostep4_3->Write();
  ratiostep4_2->Write();
  ratiostep4_1->Write();
  File->Close();

}

void TriggerEfficiency(TString file1, TString output1){

  TFile *l1  = TFile::Open(file1);

  TH1F* h1_without_cuts = (TH1F*)l1->Get("Events_without_cuts");
  TH1F* h1_RefTrigger = (TH1F*)l1->Get("Events_with_RefTrigger");
  TH1F* h1_RefTriggerCutEta4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta4");
  TH1F* h1_RefTriggerCutEta3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta3");
  TH1F* h1_RefTriggerCutEta2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta2");
  TH1F* h1_RefTriggerCutEta1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta1");
  TH1F* h1_RefTriggerCutAndTriggerEta4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");
  TH1F* h1_RefTriggerCutAndTriggerEta3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F* h1_RefTriggerCutAndTriggerEta2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F* h1_RefTriggerCutAndTriggerEta1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");

  //Eff1
  TH1F *h1_ratiorefer = h1_RefTrigger->Clone(); 
  h1_ratiorefer->Divide(h1_RefTrigger,h1_RefTrigger,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta4 = h1_RefTriggerCutAndTriggerEta4->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta4->Divide(h1_RefTriggerCutAndTriggerEta4,h1_RefTriggerCutEta4,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta3 = h1_RefTriggerCutAndTriggerEta3->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta3->Divide(h1_RefTriggerCutAndTriggerEta3,h1_RefTriggerCutEta3,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta2 = h1_RefTriggerCutAndTriggerEta2->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta2->Divide(h1_RefTriggerCutAndTriggerEta2,h1_RefTriggerCutEta2,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta1 = h1_RefTriggerCutAndTriggerEta1->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta1->Divide(h1_RefTriggerCutAndTriggerEta1,h1_RefTriggerCutEta1,1.,1.,"B");


  File = new TFile(output1,"RECREATE");
  File->cd();
  h1_ratiorefer->Write();
  h1_ratioRefTriggerCutAndTriggerEta4->Write();
  h1_ratioRefTriggerCutAndTriggerEta3->Write();
  h1_ratioRefTriggerCutAndTriggerEta2->Write();
  h1_ratioRefTriggerCutAndTriggerEta1->Write();
  File->Close();

}

void TriggerEfficiencyMerge(TString file1, TString file2, TString output){

  TFile *l1  = TFile::Open(file1);
  TFile *l2  = TFile::Open(file2);

  TH1F* h1_without_cuts = (TH1F*)l1->Get("Events_without_cuts");
  TH1F* h1_RefTrigger = (TH1F*)l1->Get("Events_with_RefTrigger");
  TH1F* h1_RefTriggerCutEta4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta4");
  TH1F* h1_RefTriggerCutEta3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta3");
  TH1F* h1_RefTriggerCutEta2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta2");
  TH1F* h1_RefTriggerCutEta1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta1");
  TH1F* h1_RefTriggerCutAndTriggerEta4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");
  TH1F* h1_RefTriggerCutAndTriggerEta3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F* h1_RefTriggerCutAndTriggerEta2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F* h1_RefTriggerCutAndTriggerEta1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");

  TH1F* h2_without_cuts = (TH1F*)l2->Get("Events_without_cuts");
  TH1F* h2_RefTrigger = (TH1F*)l2->Get("Events_with_RefTrigger");
  TH1F* h2_RefTriggerCutEta4 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLine_eta4");
  TH1F* h2_RefTriggerCutEta3 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLine_eta3");
  TH1F* h2_RefTriggerCutEta2 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLine_eta2");
  TH1F* h2_RefTriggerCutEta1 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLine_eta1");
  TH1F* h2_RefTriggerCutAndTriggerEta4 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");
  TH1F* h2_RefTriggerCutAndTriggerEta3 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F* h2_RefTriggerCutAndTriggerEta2 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F* h2_RefTriggerCutAndTriggerEta1 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");

  //Efficiency File1 Calculation
  TH1F *h1_ratiorefer = h1_RefTrigger->Clone(); 
  h1_ratiorefer->Divide(h1_RefTrigger,h1_RefTrigger,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta4 = h1_RefTriggerCutAndTriggerEta4->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta4->Divide(h1_RefTriggerCutAndTriggerEta4,h1_RefTriggerCutEta4,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta3 = h1_RefTriggerCutAndTriggerEta3->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta3->Divide(h1_RefTriggerCutAndTriggerEta3,h1_RefTriggerCutEta3,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta2 = h1_RefTriggerCutAndTriggerEta2->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta2->Divide(h1_RefTriggerCutAndTriggerEta2,h1_RefTriggerCutEta2,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta1 = h1_RefTriggerCutAndTriggerEta1->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta1->Divide(h1_RefTriggerCutAndTriggerEta1,h1_RefTriggerCutEta1,1.,1.,"B");


  //Efficiency File2 Calculation
  TH1F *h2_ratiorefer = h2_RefTrigger->Clone(); 
  h2_ratiorefer->Divide(h2_RefTrigger,h2_RefTrigger,1.,1.,"B");

  TH1F *h2_ratioRefTriggerCutAndTriggerEta4 = h2_RefTriggerCutAndTriggerEta4->Clone(); 
  h2_ratioRefTriggerCutAndTriggerEta4->Divide(h2_RefTriggerCutAndTriggerEta4,h2_RefTriggerCutEta4,1.,1.,"B");

  TH1F *h2_ratioRefTriggerCutAndTriggerEta3 = h2_RefTriggerCutAndTriggerEta3->Clone(); 
  h2_ratioRefTriggerCutAndTriggerEta3->Divide(h2_RefTriggerCutAndTriggerEta3,h2_RefTriggerCutEta3,1.,1.,"B");

  TH1F *h2_ratioRefTriggerCutAndTriggerEta2 = h2_RefTriggerCutAndTriggerEta2->Clone(); 
  h2_ratioRefTriggerCutAndTriggerEta2->Divide(h2_RefTriggerCutAndTriggerEta2,h2_RefTriggerCutEta2,1.,1.,"B");

  TH1F *h2_ratioRefTriggerCutAndTriggerEta1 = h2_RefTriggerCutAndTriggerEta1->Clone(); 
  h2_ratioRefTriggerCutAndTriggerEta1->Divide(h2_RefTriggerCutAndTriggerEta1,h2_RefTriggerCutEta1,1.,1.,"B");

  // Efficiency1*Efficiency2 Calculation
  h1_ratiorefer->Multiply(h2_ratiorefer);
  h1_ratioRefTriggerCutAndTriggerEta4->Multiply(h2_ratioRefTriggerCutAndTriggerEta4);
  h1_ratioRefTriggerCutAndTriggerEta3->Multiply(h2_ratioRefTriggerCutAndTriggerEta3);
  h1_ratioRefTriggerCutAndTriggerEta2->Multiply(h2_ratioRefTriggerCutAndTriggerEta2);
  h2_RefTrigger->Draw();
  h1_ratioRefTriggerCutAndTriggerEta1->Multiply(h2_ratioRefTriggerCutAndTriggerEta1);

  File = new TFile(output,"RECREATE");
  File->cd();
  h1_ratiorefer->Write();
  h1_ratioRefTriggerCutAndTriggerEta4->Write();
  h1_ratioRefTriggerCutAndTriggerEta3->Write();
  h1_ratioRefTriggerCutAndTriggerEta2->Write();
  h1_ratioRefTriggerCutAndTriggerEta1->Write();
  File->Close();

}

void Systematic(TString filein, TString outplus, TString outminus){

  TFile *l1  = TFile::Open(filein);

  //Load Histograms
  TH1F *heff_trigger4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");
  TH1F *heff_trigger4plus = heff_trigger4->Clone();
  TH1F *heff_trigger4minus = heff_trigger4->Clone(); 

  TH1F *heff_trigger3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F *heff_trigger3plus = heff_trigger3->Clone();
  TH1F *heff_trigger3minus = heff_trigger3->Clone(); 

  TH1F *heff_trigger2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F *heff_trigger2plus = heff_trigger2->Clone();
  TH1F *heff_trigger2minus = heff_trigger2->Clone(); 

  TH1F *heff_trigger1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");
  TH1F *heff_trigger1plus = heff_trigger1->Clone();
  TH1F *heff_trigger1minus = heff_trigger1->Clone();  

  int nBins = heff_trigger4->GetNbinsX();
  int binFirst = heff_trigger4->FindFirstBinAbove(0,1);
  int binLast = heff_trigger4->FindLastBinAbove(0,1);

  for(Int_t ibin = 0; ibin <= nBins; ++ibin){
  float binError4 = heff_trigger4->GetBinError(ibin);
  float binContent4 = heff_trigger4->GetBinContent(ibin);
  float sigmaplus4 = binContent4 + binError4;
  float sigmaminus4 = binContent4 - binError4;
  heff_trigger4plus->SetBinContent(ibin,sigmaplus4);
  heff_trigger4minus->SetBinContent(ibin,sigmaminus4);

  float binError3 = heff_trigger3->GetBinError(ibin);
  float binContent3 = heff_trigger3->GetBinContent(ibin);
  float sigmaplus3 = binContent3 + binError3;
  float sigmaminus3 = binContent3 - binError3;
  heff_trigger3plus->SetBinContent(ibin,sigmaplus3);
  heff_trigger3minus->SetBinContent(ibin,sigmaminus3);

  float binError2 = heff_trigger2->GetBinError(ibin);
  float binContent2 = heff_trigger2->GetBinContent(ibin);
  float sigmaplus2 = binContent2 + binError2;
  float sigmaminus2 = binContent2 - binError2;
  heff_trigger2plus->SetBinContent(ibin,sigmaplus2);
  heff_trigger2minus->SetBinContent(ibin,sigmaminus2);

  float binError1 = heff_trigger1->GetBinError(ibin);
  float binContent1 = heff_trigger1->GetBinContent(ibin);
  float sigmaplus1 = binContent1 + binError1;
  float sigmaminus1 = binContent1 - binError1;
  heff_trigger1plus->SetBinContent(ibin,sigmaplus1);
  heff_trigger1minus->SetBinContent(ibin,sigmaminus1);
  }

  File1 = new TFile(outplus,"RECREATE");
  File1->cd();
  heff_trigger4plus->Write();
  heff_trigger3plus->Write();
  heff_trigger2plus->Write();
  heff_trigger1plus->Write();
  File1->Close();

  File2 = new TFile(outminus,"RECREATE");
  File2->cd();
  heff_trigger4minus->Write();
  heff_trigger3minus->Write();
  heff_trigger2minus->Write();
  heff_trigger1minus->Write();
  File2->Close();

}

