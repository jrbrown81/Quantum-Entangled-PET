{
		Double_t norm, norm2, norm3;
		Int_t nBins;
		
		TFile *file3 = new TFile("nylonSim.root");
		TH1F* sim_h=(TH1F*)file3->Get("dPhiAngle_Ewin");
		sim_h->SetTitle("scattered simulation");

		
		TFile *file4 = new TFile("nylonLab.root");
		TH1F* expt_h=(TH1F*)file4->Get("dPhiAngle_Ewin");
		expt_h->SetTitle("scattered data");
		expt_h->SetYTitle("Normalised Coincidence Count Rate");

		TFile *file5 = new TFile("b2bExpt.root");
		TH1F* direct_h=(TH1F*)file5->Get("dPhiAngle_Ewin");
		direct_h->SetTitle("back-to-back data");
		direct_h->SetYTitle("Normalised Coincidence Count Rate");
		
		TFile *file6 = new TFile("b2bSim.root");
		TH1F* directSim_h=(TH1F*)file6->Get("dPhiAngle_Ewin");
		directSim_h->SetTitle("back-to-back simulation");
		directSim_h->SetYTitle("Normalised Coincidence Count Rate");

	Float_t back2backBinCount = expt_h->GetNbinsX();
	Float_t binCut = 180. - 360./back2backBinCount;
	
	TH1F* scatExptMod_h=new TH1F("scatExptMod_h","",back2backBinCount -2,-binCut,binCut);
	TH1F* scatSimMod_h=new TH1F("scatSimMod_h","",back2backBinCount - 2,-binCut,binCut);
	for(int i=1;i<=36;i++) {
		scatExptMod_h->SetBinContent(i,expt_h->GetBinContent(i+1));			// Chops out first and last bins. 
		scatSimMod_h->SetBinContent(i,sim_h->GetBinContent(i+1));
	}

	TCanvas* test_c=new TCanvas("test_c","Test");

	//rebin nylonscatt lab by 3. Normalise to central two bins. (360/38)*2*3 deg
	TH1F* scatExptModRebin_h=(TH1F*)scatExptMod_h->Clone("scatter data");
	scatExptModRebin_h->Rebin(3);
	nBins = scatExptModRebin_h->GetNbinsX();
	cout << "exptBins: " << nBins << endl;
	scatExptModRebin_h->SetStats(0);
	norm3=scatExptModRebin_h->GetBinContent(nBins/2);
	norm3+=scatExptModRebin_h->GetBinContent(nBins/2+1);
	norm3=norm3/2;
	scatExptModRebin_h->Scale(1./norm3);


	//Rebin nylonscatt sim by 2. Equivalent normalisation two cental, half of neighbours. 
	TH1F* scatSimModRebin_h=(TH1F*)scatSimMod_h->Clone("scatter simulation");
	scatSimModRebin_h->Rebin(2);
	nBins = scatSimModRebin_h->GetNbinsX();
	cout << "simBins: " << nBins << endl;
	scatSimModRebin_h->SetStats(0);
	norm3=scatSimModRebin_h->GetBinContent(nBins/2);
	norm3+=scatSimModRebin_h->GetBinContent(nBins/2+1);
	norm3+=0.5*scatSimModRebin_h->GetBinContent(nBins/2-1);
	norm3+=0.5*scatSimModRebin_h->GetBinContent(nBins/2+2);
	norm3=norm3/3;
	scatSimModRebin_h->Scale(1./norm3);

	// normalising to two central bins of nylonScatt lab. 
	// 360/(38*3*2) deg. 
	// back to back bins cover 20deg

	Double_t nBinsToCover = 360.*6./(38.*20);
	Double_t normalFrac = (nBinsToCover - 2)/2;


	TH1F* directExptNorm4_h=(TH1F*)direct_h->Clone("directExptNorm4_h");
	nBins = directExptNorm4_h->GetNbinsX();
	directExptNorm4_h->SetStats(0); 
	norm3 = directExptNorm4_h->GetBinContent(nBins/2);
	norm3+= directExptNorm4_h->GetBinContent(nBins/2 +1);
	norm3+= normalFrac*directExptNorm4_h->GetBinContent(nBins/2 +2);
	norm3+= normalFrac*directExptNorm4_h->GetBinContent(nBins/2 -2);
	norm3=norm3/nBinsToCover;
	directExptNorm4_h->Scale(1./norm3);


	TH1F* directSimNorm4_h=(TH1F*)directSim_h->Clone("directSimNorm4_h");
	nBins = directSimNorm4_h->GetNbinsX();
	directSimNorm4_h->SetStats(0);
	norm3 = directSimNorm4_h->GetBinContent(nBins/2);
	norm3+= directSimNorm4_h->GetBinContent(nBins/2 +1);
	norm3+= 0.421052632*directSimNorm4_h->GetBinContent(nBins/2 +2);
	norm3+= 0.421052632*directSimNorm4_h->GetBinContent(nBins/2 -2);
	norm3=norm3/2.842105263;
	directSimNorm4_h->Scale(1./norm3);





	
//	test_c->cd(4);
	scatExptModRebin_h->GetXaxis()->SetTitle("#Delta#phi (degrees)");
	scatExptModRebin_h->SetYTitle(expt_h->GetYaxis()->GetTitle());
	scatExptModRebin_h->SetLineColor(1);
	scatExptModRebin_h->SetMarkerColor(1);
	scatExptModRebin_h->SetMarkerStyle(21);
	scatExptModRebin_h->SetBit( TH1::kNoTitle, true );
	scatExptModRebin_h->GetXaxis()->SetLabelSize(0.05);
	scatExptModRebin_h->GetXaxis()->SetTitleSize(0.05);
	scatExptModRebin_h->GetXaxis()->SetTitleOffset(0.9);
	scatExptModRebin_h->GetYaxis()->SetLabelSize(0.05);
	scatExptModRebin_h->GetYaxis()->SetTitleSize(0.05);
	scatExptModRebin_h->GetYaxis()->SetTitleOffset(0.9);
	scatExptModRebin_h->Draw("e1"); 
	scatSimModRebin_h->SetLineColor(4); 
	scatSimModRebin_h->SetFillColor(4); 
	scatSimModRebin_h->Draw("e3 same"); 

	
	directExptNorm4_h->SetMarkerSize(0.5);
	directExptNorm4_h->SetLineColor(2);
	directExptNorm4_h->SetMarkerColor(2);
	directExptNorm4_h->Draw("e1 same");
	scatExptModRebin_h->Draw("e1 same");
	directSimNorm4_h->SetLineColor(2);
	directSimNorm4_h->SetFillColor(2);
	directSimNorm4_h->Draw("e3 same");




	auto legend=new TLegend(0.1,0.7,0.45,0.9);
	legend->AddEntry(scatExptModRebin_h,"scatter data","lep");
	legend->AddEntry(scatSimModRebin_h,"scatter simulation","l");
	legend->AddEntry(directExptNorm4_h,"back-to-back data","lep"); 
	legend->AddEntry(directSimNorm4_h,"back-to-back simulation","lp"); 
	legend->Draw();

}
