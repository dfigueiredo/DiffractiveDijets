void PlotterAll(){

//	MakePlot("pTJet1_PU_Complete_step2",1,0);
//	MakePlot("pTJet1_PU_Complete_JetsAtEta2_step3",1,1);
//	MakePlot("pTJet1_PU_Complete_JetsAtEta2_step4_4",1,0);
//	MakePlot("etaJet1_PU_Complete_JetsAtEta2_step3",0,1);
//	MakePlot("etaJet2_PU_Complete_JetsAtEta2_step3",0,0);
//	MakePlot("phiJet1_PU_Complete_JetsAtEta2_step3",0,1);
//	MakePlot("phiJet2_PU_Complete_JetsAtEta2_step3",0,0);
//	MakePlot("deltaPtJets_PU_Complete_JetsAtEta2_step3",1,1);
//	MakePlot("deltaPhiJets_PU_Complete_JetsAtEta2_step3",1,0);
//	MakePlot("deltaEtaJets_PU_Complete_JetsAtEta2_step3",1,0);
	MakePlot("RJJ_PU_Complete_JetsAtEta2_step3",1,1);
//	MakePlot("RJJ_PU_Complete_JetsAtEta2_step4_4",1,0);
//	MakePlot("RJJ_PU_Complete_JetsAtEta2_step4_3",1,0);
//	MakePlot("deltaEtamaxminPF_PU_Complete_JetsAtEta2_step3",1,0);
//	MakePlot("deltaEtamaxminPF_PU_Complete_JetsAtEta2_step4_4",1,0);
//	MakePlot("deltaEtamaxminPF_PU_Complete_JetsAtEta2_step4_3",1,0);
//	MakePlot("deltaPtJets_PU_Complete_JetsAtEta2_step4_3",1,0);
//	MakePlot("deltaPhiJets_PU_Complete_JetsAtEta2_step4_3",1,0);
//	MakePlot("deltaEtaJets_PU_Complete_JetsAtEta2_step4_3",1,0);
//	MakePlot("pfetamax_PU_Complete_JetsAtEta2_step3",1,0);
//	MakePlot("pfetamax_PU_Complete_JetsAtEta2_step4_4",1,0);
//	MakePlot("pfetamax_PU_Complete_JetsAtEta2_step4_3",1,0);
//	MakePlot("pfetamin_PU_Complete_JetsAtEta2_step3",1,1);
//	MakePlot("pfetamin_PU_Complete_JetsAtEta2_step4_4",1,0);
//	MakePlot("pfetamin_PU_Complete_JetsAtEta2_step4_3",1,0);
//	MakePlot("deltaEtamaxminPF_PU_Complete_JetsAtEta2_step4_3",1,0);
//	MakePlot("sumEHFpfplus_PU_Complete_JetsAtEta2_step3",1,0);

}

void MakePlot(TString name,bool logscale,bool AutoNorma){

        gStyle->SetOptStat(0);
	TFile *l1  = TFile::Open("histo_multijet_HLTExclDiJet30U_AND_HFPreSel_GoodVertex_Allcorrected.root");
	TFile *l2  = TFile::Open("histomultijet_qcd15to3000_flat_tunez2_SL_presel_vertex.root");
	TFile *l3  = TFile::Open("histomultijet_exhume_SL_presel_vertex.root");

	TCanvas* c1 = new TCanvas(name,name);
	TLegend* leg = new TLegend(0.7597956,0.822335,0.9931857,0.9949239,NULL,"brNDC");

        double weight = 0.;

	if(logscale) c1->SetLogy(1);

	if(AutoNorma) {
           TH1F* h_data = (TH1F*)l1->Get("pfetamin_PU_Complete_JetsAtEta2_step3");
           TH1F* h_mc = (TH1F*)l2->Get("pfetamin_PU_Complete_JetsAtEta2_step3");
	   double intmc = 0.;
	   double intdata = 0.;
           TAxis *axisData = h_data->GetXaxis();
           int bminData = axisData->FindBin(-5);
           int bmaxData = axisData->FindBin(-2);  
           TAxis *axisMC = h_mc->GetXaxis();
           int bminMC = axisMC->FindBin(-5);
           int bmaxMC = axisMC->FindBin(-2);           
           intdata = h_data->Integral(bminData,bmaxData); 
           intmc = h_mc->Integral(bminMC,bmaxMC);
	   weight = intdata/intmc;
           cout << "Weight: " << weight << endl;
        }
        else weight = 1.;

	TH1F* h_1 = (TH1F*)l1->Get(name);
	h_1->SetLineColor(kBlack);
	h_1->SetMarkerStyle(20);
	h_1->SetMarkerSize(1.4);
	h_1->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_1,"Data 2010","p");

	TH1F* h_2 = (TH1F*)l2->Get(name);
	h_2->SetLineColor(kRed);
	h_2->SetLineWidth(3);
        h_2->Scale(weight);
        h_2->SetFillColor(kRed-4);
        h_2->SetFillStyle(3020);
	h_2->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_2,"PYTHIA 6","l");


	TH1F* h_3 = (TH1F*)l3->Get(name);
	h_3->SetLineColor(kBlue);
	h_3->SetLineWidth(3);
	h_3->GetYaxis()->SetTitle("N Events");
        h_3->SetFillColor(40);
        h_3->SetFillStyle(3002);
	leg->AddEntry(h_3,"ExHuMe GG","l");

	h_2->Draw("histo");
	h_3->Draw("histosames");
        h_1->Draw("epsames");
	leg->Draw("histosames");

	c1->SaveAs(name+"_"+TString(".png"));
	c1->SaveAs(name+"_"+TString(".pdf"));
	c1->SaveAs(name+"_"+TString(".C"));

}



