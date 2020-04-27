// position smearing

void randExtract()
{
	Float_t leftMargin=0.12;
	Float_t rightMargin=0.05;
	Float_t bottomMargin=0.15;
	Float_t topMargin=0.06;
	
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	gStyle->SetPadLeftMargin(leftMargin);
	gStyle->SetPadRightMargin(rightMargin);
	gStyle->SetPadTopMargin(topMargin);
	gStyle->SetPadBottomMargin(bottomMargin);
	TGaxis::SetMaxDigits(3);
		
	Float_t legTextSize=0.05;
	Float_t labelTextSize=0.06;
	Float_t titleOffset=1.0;
	Int_t nBins=100;
	Int_t fitMin=8;
	Int_t fitMax=92;
	
////////////////////////////////////////////////////////////////////////////////////////
// Calculate enhancement coefficients and scaling factors
	// enter enhancement and calculate factors from those

	// New simulation with scatter flag bug fix, April 2020
		double enh=2.541;
		double enhError=0.012;
		double enh_pol=1.;

	// cos(2phi) distribution functions
		TF1* enhFunc=new TF1("enhFunc","(1-[0])/(1+[0])*TMath::Cos(2*TMath::DegToRad()*x)+1",-180,180);
		enhFunc->SetParName(0,"enh");
		enhFunc->SetParameter(0,enh);
		TF1* enhFuncP=new TF1("enhFuncP","(1-[0])/(1+[0])*TMath::Cos(2*TMath::DegToRad()*x)+1",-180,180);
		enhFuncP->SetParName(0,"enh");
		enhFuncP->SetParameter(0,enh+enhError);
		TF1* enhFuncM=new TF1("enhFuncM","(1-[0])/(1+[0])*TMath::Cos(2*TMath::DegToRad()*x)+1",-180,180);
		enhFuncM->SetParName(0,"enh");
		enhFuncM->SetParameter(0,enh-enhError);
		TF1* polFunc=new TF1("polFunc","(1-[0])/(1+[0])*TMath::Cos(2*TMath::DegToRad()*x)+1",-180,180);
		polFunc->SetParName(0,"enh_pol");
		polFunc->SetParameter(0,enh_pol);

		TCanvas* functions_c=new TCanvas("functions_c","Enhancements Functions");
		enhFunc->Draw();
		polFunc->SetLineColor(4);
		polFunc->Draw("same");
		functions_c->BuildLegend(0.6,0.75,0.9,0.94);

	// calculate coefficients for each phi slice
		int nSlices=9;
		double phi[nSlices];

		double c_s[3][nSlices];
		double c_t[3][nSlices];

		cout << endl << "Enhancement: " << enh << " +- " << enhError << endl;
		cout << "Reconstruction coefficients" << endl
				<< "|#phi|	c_s	c_t" << endl;
		for(int i=0;i<nSlices;i++) {
			phi[i]=i*20+10;

			c_t[0][i]=(enhFunc->Integral(i*20,(i+1)*20))/20;
			c_t[1][i]=(enhFuncP->Integral(i*20,(i+1)*20))/20;
			c_t[2][i]=(enhFuncM->Integral(i*20,(i+1)*20))/20;
			c_s[0][i]=(polFunc->Integral(i*20,(i+1)*20))/20;
			cout << phi[i] << "	" << c_s[0][i] << "	" << c_t[0][i] << endl;
		}

		double K[3];
		K[0]=c_t[0][4]*c_s[0][0]-c_s[0][4]*c_t[0][0];
		K[1]=c_t[1][4]*c_s[0][0]-c_s[0][4]*c_t[1][0];
		K[2]=c_t[2][4]*c_s[0][0]-c_s[0][4]*c_t[2][0];

		cout << "K: " << K[0] << endl;
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// Read data from files to trees and fill basic histograms

	TTree* tree[3][4];
	tree[0][0]=new TTree("tree1","phi0b");
	tree[0][1]=new TTree("tree2","phi90b");
	tree[0][2]=new TTree("tree3","scatterOnlyb");
	tree[0][3]=new TTree("tree4","trueOnlyb");
	tree[1][0]=new TTree("tree5","phi0m");
	tree[1][1]=new TTree("tree6","phi90m");
	tree[1][2]=new TTree("tree7","scatterOnlym");
	tree[1][3]=new TTree("tree8","trueOnlym");
	tree[2][0]=new TTree("tree9","phi0t");
	tree[2][1]=new TTree("tree10","phi90t");
	tree[2][2]=new TTree("tree11","scatterOnlyt");
	tree[2][3]=new TTree("tree12","trueOnlyt");
// New simulations with scatter flag bug fix, April 2020
// Bottom
	tree[0][0]->ReadFile("bottom_trueRandom_0-20_3BillLORs.csv");
	tree[0][1]->ReadFile("bottom_trueRandom_80-100_3BillLORs.csv");
	tree[0][2]->ReadFile("bottom_random_onlyThetaWindow_3BillLORs.csv");
	tree[0][3]->ReadFile("bottom_true_onlyThetaWindow_3BillLORs.csv");

	float x[3], phi0[3], phi90[3], scatter[3], trues[3];
	TH1F* scatAbs_h[3];
	TH1F* scatAbsScaled_h[3];
	TH1F* trueAbs_h[3];
	TH1F* trueAbsScaled_h[3];
	TH1F* phi_h[3][nSlices];
	for(int i=0;i<1;i++) {
		tree[i][0]->SetBranchAddress("X",&x[i]);
		tree[i][0]->SetBranchAddress("Y",&phi0[i]);
		tree[i][1]->SetBranchAddress("Y",&phi90[i]);
		tree[i][2]->SetBranchAddress("Y",&scatter[i]);
		tree[i][3]->SetBranchAddress("Y",&trues[i]);
		
		scatAbs_h[i]=new TH1F(Form("scatAbs%i_h",i),"'Real' scatter profile (all phi)",nBins,0,nBins);//
		scatAbs_h[i]->GetXaxis()->SetTitle("x, pixels");
		scatAbs_h[i]->GetYaxis()->SetTitle("Intensity (arb.)");
		scatAbsScaled_h[i]=new TH1F(Form("scatAbsScaled%i_h",i),"'Real' random profile",nBins,0,nBins);//
		trueAbs_h[i]=new TH1F(Form("trueAbs%i_h",i),"'Real' true profile",nBins,0,nBins);//
		trueAbs_h[i]->GetXaxis()->SetTitle("x, pixels");
		trueAbs_h[i]->GetYaxis()->SetTitle("Intensity (arb.)");
		trueAbsScaled_h[i]=new TH1F(Form("trueAbsScaled%i_h",i),"'Real' true profile",nBins,0,nBins);
		trueAbsScaled_h[i]->GetXaxis()->SetTitle("x, pixels");
		trueAbsScaled_h[i]->GetYaxis()->SetTitle("Intensity (arb.)");
		trueAbsScaled_h[i]->GetXaxis()->SetTitleSize(labelTextSize);
		trueAbsScaled_h[i]->GetYaxis()->SetTitleSize(labelTextSize);
		
		for(int j=0;j<9;j++) {
			phi_h[i][j]=new TH1F(Form("phi%i_%d_h",i,j*20+10),Form("%d#circ #leq |#Delta#phi| #leq %d#circ",j*20,j*20+20),nBins,0,nBins);//
			phi_h[i][j]->GetXaxis()->SetTitle("x, pixels");
			phi_h[i][j]->GetYaxis()->SetTitle("Intensity (arb.)");
			phi_h[i][j]->GetXaxis()->SetTitleSize(labelTextSize);
			phi_h[i][j]->GetYaxis()->SetTitleSize(labelTextSize);
			phi_h[i][j]->GetXaxis()->SetLabelSize(labelTextSize);
			phi_h[i][j]->GetYaxis()->SetLabelSize(labelTextSize);
			phi_h[i][j]->GetYaxis()->SetTitleOffset(titleOffset);
		}
	}

	Long64_t nentries = tree[0][2]->GetEntries();
	for(int i=0;i<1;i++) {
		for (Long64_t j=0;j<nentries;j++) {
			tree[i][0]->GetEntry(j);
			phi_h[i][0]->Fill(j,phi0[i]);
			tree[i][1]->GetEntry(j);
			phi_h[i][4]->Fill(j,phi90[i]);
			tree[i][2]->GetEntry(j);
			scatAbs_h[i]->Fill(j,scatter[i]);
			tree[i][3]->GetEntry(j);
			trueAbs_h[i]->Fill(j,trues[i]);
		}
	}
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// Create canvases
	TCanvas* c1[3];
	c1[0]=new TCanvas("c1_1","Raw slices (bottom)");
	TLegend* leg1[3];
	
	for(int i=0;i<1;i++){
		c1[i]->cd();
		phi_h[i][4]->Draw("hist l");
		phi_h[i][0]->SetLineColor(3);
		phi_h[i][0]->Draw("hist l same");
		leg1[i] = new TLegend(leftMargin,0.75,leftMargin+0.35,1-topMargin);
		leg1[i]->AddEntry(phi_h[i][4],phi_h[i][4]->GetTitle());
		leg1[i]->AddEntry(phi_h[i][0],phi_h[i][0]->GetTitle());
		leg1[i]->SetTextSize(legTextSize);
		leg1[i]->Draw();
	}

// Phi 0 and 90 only method
	TH1F* scatEst0_90_h[3];
	TH1F* trueEst0_90_h[3];
	TH1F* sub_h[3];
	TH1F* scat90_h[3];
	TH1F* true90_h[3];
	
	for(int i=0;i<1;i++){
		scatEst0_90_h[i]=new TH1F(Form("scatEst0_90_h_%i",i),"Extracted random profile",nBins,0,nBins);
		trueEst0_90_h[i]=new TH1F(Form("trueEst0_90_h_%i",i),"Extracted true profile",nBins,0,nBins);
		sub_h[i]=new TH1F(Form("sub_h_%i",i),"Simple subtraction of phi90 and phi0",nBins,0,nBins);

		scat90_h[i]=new TH1F(Form("scat90_h_%i",i),"Extracted random profile scaled for 80<|#phi|<100",nBins,0,nBins);
		true90_h[i]=new TH1F(Form("true90_h_%i",i),"Extracted true profile",nBins,0,nBins);
		
		scatEst0_90_h[i]->GetXaxis()->SetTitle("x, pixels");
		scatEst0_90_h[i]->GetYaxis()->SetTitle("Intensity (arb.)");
		scatEst0_90_h[i]->SetLineColor(2);
		scatEst0_90_h[i]->GetXaxis()->SetLabelSize(labelTextSize);
		scatEst0_90_h[i]->GetYaxis()->SetLabelSize(labelTextSize);
		trueEst0_90_h[i]->GetXaxis()->SetTitle("x, pixels");
		trueEst0_90_h[i]->GetYaxis()->SetTitle("Intensity (arb.)");
		trueEst0_90_h[i]->GetXaxis()->SetTitleSize(labelTextSize);
		trueEst0_90_h[i]->GetYaxis()->SetTitleSize(labelTextSize);
		trueEst0_90_h[i]->GetXaxis()->SetLabelSize(labelTextSize);
		trueEst0_90_h[i]->GetYaxis()->SetLabelSize(labelTextSize);
		scat90_h[i]->GetXaxis()->SetTitle("x, pixels");
		scat90_h[i]->GetYaxis()->SetTitle("Intensity (arb.)");
		scat90_h[i]->GetXaxis()->SetLabelSize(labelTextSize);
		scat90_h[i]->GetYaxis()->SetLabelSize(labelTextSize);
		true90_h[i]->GetXaxis()->SetTitle("x, pixels");
		true90_h[i]->GetYaxis()->SetTitle("Intensity (arb.)");
		true90_h[i]->GetXaxis()->SetLabelSize(labelTextSize);
		true90_h[i]->GetYaxis()->SetLabelSize(labelTextSize);
		
		scatEst0_90_h[i]->Add(phi_h[i][0],phi_h[i][4],c_t[0][4],-c_t[0][0]);
		scatEst0_90_h[i]->Scale(1./K[0]);

		trueEst0_90_h[i]->Add(phi_h[i][4],phi_h[i][0],c_s[0][0],-c_s[0][4]);
		trueEst0_90_h[i]->Scale(1./K[0]);
		true90_h[i]->Add(trueEst0_90_h[i],c_t[0][4]);
		scat90_h[i]->Add(scatEst0_90_h[i],c_s[0][4]);
		
		scatAbsScaled_h[i]->Add(scatAbs_h[i],1./9);
		trueAbsScaled_h[i]->Add(trueAbs_h[i],1./9);
		
		sub_h[i]->Add(phi_h[i][4],phi_h[i][0],1,-1);
	}

/////////////////////////////////////////

	TCanvas *c2[3];
	c2[0]=new TCanvas(Form("c2_%i",0),"Random extraction, bottom (histo)");
	TLegend* leg2[3];
	
	TF1* f1[3];
		
	for(int i=0;i<1;i++) {
		c2[i]->cd();
		scatEst0_90_h[i]->GetXaxis()->SetTitleSize(labelTextSize);
		scatEst0_90_h[i]->GetYaxis()->SetTitleSize(labelTextSize);
		scatEst0_90_h[i]->GetXaxis()->SetLabelSize(labelTextSize);
		scatEst0_90_h[i]->GetYaxis()->SetLabelSize(labelTextSize);
		scatEst0_90_h[i]->GetYaxis()->SetTitleOffset(titleOffset);
		scatEst0_90_h[i]->Draw("hist");
		scatAbsScaled_h[i]->DrawCopy("hist same");
		leg2[i] = new TLegend(leftMargin,0.75,leftMargin+0.5,1-topMargin);
		leg2[i]->AddEntry(scatEst0_90_h[i],scatEst0_90_h[i]->GetTitle());
		leg2[i]->AddEntry(scatAbsScaled_h[i],scatAbsScaled_h[i]->GetTitle());
		leg2[i]->SetTextSize(legTextSize);
		leg2[i]->Draw();
	}

	TCanvas *c3[3];
	TCanvas *c4[3];
	c3[0]=new TCanvas("c3_0","Random extraction, bottom (l)");
	c4[0]=new TCanvas("c4_0","Random extraction, bottom (l)");
	TLegend* leg3[3];
	TLegend* leg6[3];
	TLatex *tl[4];
	for(int c=0;c<4;c++) {
		tl[c]=new TLatex;
		tl[c]->SetTextSize(0.08);
		tl[c]->SetNDC();
	}

	for(int i=0;i<1;i++) {
		c3[i]->cd();
		scatEst0_90_h[i]->GetYaxis()->SetMaxDigits(3);
		scatEst0_90_h[i]->DrawCopy("l hist");
		f1[i] = new TF1(Form("f1_%i",i),"pol4",fitMin,fitMax);
		scatEst0_90_h[i]->Fit(Form("f1_%i",i),"RW+","hist l");
		f1[i]->SetLineColor(1);
		f1[i]->SetLineStyle(7);
		f1[i]->Draw("same");
		scatAbsScaled_h[i]->DrawCopy("l hist same");
		leg3[i] = new TLegend(leftMargin,0.75,leftMargin+0.48,1-topMargin);
		leg3[i]->AddEntry(scatEst0_90_h[i],scatEst0_90_h[i]->GetTitle());
		leg3[i]->AddEntry(scatAbsScaled_h[i],scatAbsScaled_h[i]->GetTitle());
		leg3[i]->AddEntry(f1[i],"Fit to extracted random");
		leg3[i]->SetTextSize(legTextSize);
		leg3[i]->Draw();

		c4[i]->Divide(2,2);
		c4[i]->GetPad(1)->SetPad(0.0,0.5,0.5,1.);
		c4[i]->GetPad(2)->SetPad(0.5,0.5,1.,1.);
		c4[i]->GetPad(3)->SetPad(0.0,0.0,0.5,0.5);
		c4[i]->GetPad(4)->SetPad(0.5,0.0,1.,0.5);
		c4[i]->cd(2);
		phi_h[i][4]->Draw("hist l");
		phi_h[i][0]->Draw("hist l same");
		leg1[i]->Draw();
		c4[i]->cd(3);
		scatEst0_90_h[i]->DrawCopy("l hist");
		f1[i]->Draw("same");
		scatAbsScaled_h[i]->DrawCopy("l hist same");
		leg3[i]->Draw();
		c4[i]->cd(4);
		trueEst0_90_h[i]->SetLineColor(2);
		trueEst0_90_h[i]->DrawCopy("l hist");
		trueAbsScaled_h[i]->DrawCopy("l hist same");
		leg6[i] = new TLegend(leftMargin,0.75,leftMargin+0.4,1-topMargin);
		leg6[i]->AddEntry(trueEst0_90_h[i],trueEst0_90_h[i]->GetTitle());
		leg6[i]->AddEntry(trueAbsScaled_h[i],trueAbsScaled_h[i]->GetTitle());
		leg6[i]->SetTextSize(legTextSize);
		leg6[i]->Draw();
		
		c4[i]->cd(1);
		tl[0]->DrawLatex(0.88,0.85,"a");
		c4[i]->cd(2);
		tl[0]->DrawLatex(0.88,0.85,"b");
		c4[i]->cd(3);
		tl[0]->DrawLatex(0.88,0.85,"c");
		c4[i]->cd(4);
		tl[0]->DrawLatex(0.88,0.85,"d");
	}
	
///////////////////////////////////////////

} // FIN
