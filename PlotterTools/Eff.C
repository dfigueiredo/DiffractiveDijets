void Eff(){
	MakePlot("histo_zerobias_Before.root",1);
}


void MakePlot(TString name,bool logscale){

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
   File = new TFile(filenameroot.TString::ReplaceAll("histo_zerobias","eff"),"RECREATE");
   File->cd();
   ratiopresel->Write();
   ratiovertex->Write();
   ratiostep4_4->Write();
   ratiostep4_3->Write();
   ratiostep4_2->Write();
   ratiostep4_1->Write();
   File->Close();

}



