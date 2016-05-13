#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPad.h"


void create_plot(
  TH1D * hw,
  TH1D * emu,
  int runNo,
  char * dataset,
  char * xLabel,
  char * exportPath,
  int rebin = 1,
  int energy = 13  
  ) {

  TLatex n;
  n.SetNDC();
  n.SetTextFont(52);
  n.SetTextSize(0.04);
  
  TCanvas* canv = new TCanvas("canv","canvas");

  // TPad* pad1 = new TPad("pJetEt","pJetEt",0,0.05,1,1); 
  TPad* pad1 = new TPad("mainPad","mainPad",0,0.3,1,1); 

  TPad* pad2 = new TPad("ratioPad","ratioPad",0,0.05,1,0.3);
  
  TPad* overlayPad = new TPad("pInv","pInv", 0,0.05,1,0.3);
  overlayPad->SetFillStyle(0);

  leg = new TLegend(0.72,0.85,0.91,1);
  leg->SetFillColor(0);
  leg->SetNColumns(2);
  leg->AddEntry(hw,"Hardware", "p");//"l");
  leg->AddEntry(emu,"Emulator", "l");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);

  // hw->Rebin(rebin);
  // emu->Rebin(rebin);

  hw->SetMarkerStyle(21);
  hw->SetMarkerColor(1);
  hw->SetMarkerSize(0.5);
  emu->SetLineColor(kRed);
  hw->GetYaxis()->SetTitle("Number of candidates");
  hw->GetYaxis()->SetTitleSize(0.055);
  hw->GetYaxis()->SetTitleOffset(0.80);
  hw->GetXaxis()->SetLabelSize(0);
  hw->GetYaxis()->SetTickSize(0.01);

  emu->GetXaxis()->SetLabelSize(0);
  
  pad1->SetBottomMargin(0.02);
  pad1->SetGridx();
  
  pad1->Draw();
  pad1->cd();
  
  hw->SetStats(0);  
  emu->SetStats(0);
 
  if (emu->GetMaximum() > hw->GetMaximum()) {
	hw->SetMaximum(1.1*emu->GetMaximum());
  }
   
  TH1D* ratioHist = (TH1D*)hw->DrawCopy("p");
 
  //THStack * stack = new THStack("hs", "");
  //stack->Add(hw, "p");
  //stack->Add(emu, "same");
  //hw->Draw("p");
  
  emu->Draw("same");

  /*
  stack->Draw("nostack");
  stack->GetXaxis()->SetLabelSize(0);

  stack->GetYaxis()->SetTitle("Number of candidates");
  stack->GetYaxis()->SetTitleSize(0.045);
  stack->GetYaxis()->SetTitleOffset(0.95);
  stack->GetXaxis()->SetLabelSize(0);
  stack->GetYaxis()->SetTickSize(0.01);
  */
  
  leg->Draw();
  stringstream caption;
  caption << "#bf{CMS Preliminary}: Run " << runNo << ", #sqrt{s} = " << energy
		  << " TeV, " << dataset;
  n.DrawLatex(0.1, 0.915, caption.str().c_str());
  
  canv->cd();
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.30);
  pad2->Draw();
  pad2->cd();
  pad2->SetGridy();
  pad2->SetGridx();
  hw->Divide(emu);
  hw->GetYaxis()->SetTitle("Ratio HW/EM");

  stringstream labelText;
  labelText << "Level-1 Trigger " << xLabel;
  hw->GetYaxis()->CenterTitle();
  hw->GetXaxis()->SetTitle(labelText.str().c_str());

  hw->GetYaxis()->SetTitleSize(0.11);
  hw->GetYaxis()->SetTitleOffset(0.35);
  hw->GetYaxis()->SetLabelSize(0.09);

  hw->GetYaxis()->SetLabelOffset(0.006);
  hw->GetYaxis()->SetNdivisions(40407);
  
  hw->GetXaxis()->SetTitleSize(0.1);
  hw->GetXaxis()->SetTitleOffset(1.5);
  hw->GetXaxis()->SetLabelOffset(0.04);
  hw->GetXaxis()->SetLabelSize(0.10);

  hw->SetMinimum(0.76);
  hw->SetMaximum(1.24);
  hw->Draw("p");
  canv->cd();
  overlayPad->Draw();
  overlayPad->cd();
  
  TLine* unity = new TLine(0.1,0.65,0.9,0.65);
  unity->SetLineColor(kBlue);
  unity->Draw();

  stringstream pathStream;
  pathStream << "compHwEmu/" << exportPath;
  canv->SaveAs(pathStream.str().c_str());

  delete canv;
}


void compHwEmu_new (
  int runNo = 0, char * dataset = "", bool useEventDisplay = false
  ) {

  const unsigned int evtToDisplay = 18;

  TFile* inFileHw = new TFile("l1tCalo_2016_simHistos.root");
  TFile* inFileEm = new TFile("l1tCalo_2016_simHistos.root");

  // gStyle->SetOptStat(111111);

  // Jets
  TH1D* hwMPJetEt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpjet/et");
  TH1D* emMPJetEt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpjet/et");
  TH1D* hwJetEt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/jet/et");
  TH1D* emJetEt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/jet/et");

  TH1D* hwMPJetEta = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpjet/eta");
  TH1D* emMPJetEta = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpjet/eta");
  TH1D* hwJetEta = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/jet/eta");
  TH1D* emJetEta = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/jet/eta");

  TH1D* hwMPJetPhi = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpjet/phi");
  TH1D* emMPJetPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpjet/phi");
  TH1D* hwJetPhi = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/jet/phi");
  TH1D* emJetPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/jet/phi");

  // MP sums
  TH1D* hwMPSumEt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpsumet/et");
  TH1D* emMPSumEt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpsumet/et");

  TH1D* hwMPSumEtx = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpsummetx/et");
  TH1D* emMPSumEtx = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpsummetx/et");

  TH1D* hwMPSumEty = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpsummety/et");
  TH1D* emMPSumEty = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpsummety/et");

  TH1D* hwMPSumHt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpsumht/et");
  TH1D* emMPSumHt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpsumht/et");

  TH1D* hwMPSumHtx = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpsummhtx/et");
  TH1D* emMPSumHtx = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpsummhtx/et");

  TH1D* hwMPSumHty = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpsummhty/et");
  TH1D* emMPSumHty = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpsummhty/et");
  
  // Demux sums
  
  TH1D* hwSumEt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/sumet/et");
  TH1D* emSumEt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/sumet/et");

  TH1D* hwSumMet = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/summet/et");
  TH1D* emSumMet = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/summet/et");

  TH1D* hwSumHt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/sumht/et");
  TH1D* emSumHt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/sumht/et");

  TH1D* hwSumMht = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/summht/et");
  TH1D* emSumMht = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/summht/et");
  
  // Sum phi's
  
  TH1D* hwMetPhi = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/summet/phi");
  TH1D* emMetPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/summet/phi");

  TH1D* hwMhtPhi = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/summht/phi");
  TH1D* emMhtPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/summht/phi");
  

  // Sorts

  TH1D* hwSortMP = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/sortMP");
  TH1D* emSortMP = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/sortMP");

  TH1D* hwSort = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/sort");
  TH1D* emSort = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/sort");

  // EG Et

  TH1D* hwMPEgEt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpeg/et");
  TH1D* emMPEgEt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpeg/et");
  TH1D* hwEgEt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/eg/et");
  TH1D* emEgEt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/eg/et");


  // EG eta
  TH1D* hwMPEgEta = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpeg/eta");
  TH1D* emMPEgEta = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpeg/eta");
  TH1D* hwEgEta = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/eg/eta");
  TH1D* emEgEta = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/eg/eta");

  // EG phi
  TH1D* hwMPEgPhi = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mpeg/phi");
  TH1D* emMPEgPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mpeg/phi");
  TH1D* hwEgPhi = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/eg/phi");
  TH1D* emEgPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/eg/phi");

 

  // Tau Et

  TH1D* hwMPTauEt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mptau/et");
  TH1D* emMPTauEt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mptau/et");
  TH1D* hwTauEt = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/tau/et");
  TH1D* emTauEt = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/tau/et");


  // Tau eta
  TH1D* hwMPTauEta = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mptau/eta");
  TH1D* emMPTauEta = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mptau/eta");
  TH1D* hwTauEta = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/tau/eta");
  TH1D* emTauEta = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/tau/eta");

  // Tau phi
  TH1D* hwMPTauPhi = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/mptau/phi");
  TH1D* emMPTauPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/mptau/phi");
  TH1D* hwTauPhi = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/tau/phi");
  TH1D* emTauPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/tau/phi");

  TH1D* hwMinBiasHFp0 = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/minbiashfp0/et");
  TH1D* hwMinBiasHFm0 = (TH1D*)inFileHw->Get("l1tCaloStage2HwHistos/minbiashfm0/et");
  // TH1D* emTauPhi = (TH1D*)inFileEm->Get("l1tStage2CaloAnalyzer/tau/phi");

  TH2F * HFCorrHist = new TH2F("HFCorrHist", "HFCorrHist",
							100, 1, 0,
							100, 1, 0);
  // mhtHist->SetTitle(";MHT; MHT emu");
  cout << hwMinBiasHFp0->GetEntries() << endl;

  hwMinBiasHFp0->Draw();

  return;

  
  if (useEventDisplay) {

	stringstream filename;
	stringstream title;

	// stringstream treeobj;
	
	for (unsigned int i = 1; i <= evtToDisplay; ++i) {

	  char treeobj [255];
	  sprintf(treeobj, "l1tCaloStage2HwHistos/Events/1-%d/Tower", i);

	  //treeobj << "l1tCaloStage2HwHistos/Events/1-" << i << "/Tower";
	  TH1F* evtHisto = (TH1F *) inFileHw->Get(
		//treeobj.str().c_str()
		treeobj
	  );	

	  TCanvas * canv = new TCanvas("canv", "canvas");

	  title << "Event " << i << " towers (hw)";
	  evtHisto->SetTitle(title.str().c_str());
	  evtHisto->GetXaxis()->SetTitle("#eta regions, i#eta");
	  evtHisto->GetYaxis()->SetTitle("#phi regions, i#phi");
	  // evtHisto->SetGridx();
	  
	  evtHisto->Draw("text");

	  // filename << "compHwEmu/hwEvt-towers-" << i << ".pdf";

	  if ( i == 1) {
		// canv->SaveAs(filename.str().c_str());
		canv->Print("compHwEmu/hwEvt-towers.pdf(", "pdf");
	  } else if ( i == evtToDisplay) {
		canv->Print("compHwEmu/hwEvt-towers.pdf)", "pdf");
	  } else {
		canv->Print("compHwEmu/hwEvt-towers.pdf", "pdf");
	  }

	  filename.str("");
	  title.str("");
	  // treeobj.str("");
	}

	stringstream filename;
	stringstream title;
	// stringstream treeobj;
	
	for (unsigned int i = 1; i <= evtToDisplay; ++i) {

	  char treeobj [255];
	  sprintf(treeobj, "l1tStage2CaloAnalyzer/Events/1-%d/Tower", i);

	  //treeobj << "l1tStage2CaloAnalyzer/Events/1-" << i << "/Tower";
	  TH1F* evtHisto = (TH1F *) inFileHw->Get(
		// treeobj.str().c_str()
		treeobj
	  );

	  TCanvas * canv = new TCanvas("canv", "canvas");

	  title << "Event " << i << " towers (emu)";
	  evtHisto->SetTitle(title.str().c_str());
	  evtHisto->GetXaxis()->SetTitle("#eta regions, i#eta");
	  evtHisto->GetYaxis()->SetTitle("#phi regions, i#phi");
	  
	  evtHisto->Draw("text");

	  // filename << "compHwEmu/hwEvt-towers-" << i << ".pdf";

	  if ( i == 1) {
		// canv->SaveAs(filename.str().c_str());
		canv->Print("compHwEmu/emuEvt-towers.pdf(", "pdf");
	  } else if ( i == evtToDisplay) {
		canv->Print("compHwEmu/emuEvt-towers.pdf)", "pdf");
	  } else {
		canv->Print("compHwEmu/emuEvt-towers.pdf", "pdf");
	  }

	  filename.str("");
	  title.str("");
	  // treeobj.str("");
	}
  }

  /*
  create_plot(
	hwMPEgEt,
	emMPEgEt,
	runNo, dataset, "e/#gamma iE_{T}", "Egs/EgEt.pdf", 6
	);

  return;
  */
  // ========================== jets start ========================
  // plot MP jet Et
  create_plot(
	hwMPJetEt,
	emMPJetEt,
	runNo, dataset, "Jet iE_{T}", "Jets/JetEt.pdf", 20
	);

  //return;
  // plot MP jet eta
  create_plot(
	hwMPJetEta,
	emMPJetEta,
	runNo, dataset, "Jet i#eta", "Jets/JetEta.pdf"
	);

  // plot MP jet phi
  create_plot(
	hwMPJetPhi,
	emMPJetPhi,
	runNo, dataset, "Jet i#phi", "Jets/JetPhi.pdf"
	);

  // plot demux jet Et
  create_plot(
	hwJetEt,
	emJetEt,
	runNo, dataset, "Jet iE_{T}", "DemuxJets/JetEt.pdf", 20
	);

  // plot demux jet eta
  create_plot(
	hwJetEta,
	emJetEta,
	runNo, dataset, "Jet i#eta", "DemuxJets/JetEta.pdf"
	);
  
  // plot demux jet phi
  create_plot(
	hwJetPhi,
	emJetPhi,
	runNo, dataset, "Jet i#phi", "DemuxJets/JetPhi.pdf"
	);

  // =========================== jets end =========================
  
  // ======================== MP sums start ========================
  // plot MP sum Et
  create_plot(
	hwMPSumEt,
	emMPSumEt,
	runNo, dataset, "Jet iE_{T}", "MPSums/MPSumEt.pdf", 40
	);
  
  // plot MP sum Etx
  create_plot(
	hwMPSumEtx,
	emMPSumEtx,
	runNo, dataset, "Jet iE_{T,x}", "MPSums/MPSumEtx.pdf", 20
	);

  // plot MP sum Ety
  create_plot(
	hwMPSumEty,
	emMPSumEty,
	runNo, dataset, "Jet iE_{T,y}", "MPSums/MPSumEty.pdf", 20
	);

  // plot MP sum Ht
    create_plot(
	hwMPSumHt,
	emMPSumHt,
	runNo, dataset, "Jet iH_{T}", "MPSums/MPSumHt.pdf", 40
	);
  
  // plot MP sum Htx
  create_plot(
	hwMPSumHtx,
	emMPSumHtx,
	runNo, dataset, "Jet iH_{T,x}", "MPSums/MPSumHtx.pdf", 20
	);

  // plot MP sum Hty
  create_plot(
	hwMPSumHty,
	emMPSumHty,
	runNo, dataset, "Jet iH_{T,y}", "MPSums/MPSumHty.pdf", 20
	);  
  // ========================= MP sums end ========================
  
  // ======================== demux sums start ========================
    // plot demux sum Et
  create_plot(
	hwSumEt,
	emSumEt,
	runNo, dataset, "iE_{T}", "DemuxSums/DemSumEt.pdf", 40
	);
  
  // plot demux sum Met
  create_plot(
	hwSumMet,
	emSumMet,
	runNo, dataset, "iMET", "DemuxSums/DemSumMet.pdf", 20
	);
    
  // plot demux sum Met phi
  create_plot(
	hwMetPhi,
	emMetPhi,
	runNo, dataset, "MET i#phi", "DemuxSums/DemMetPhi.pdf"
	);
  // plot demux sum Ht
  create_plot(
	hwSumHt,
	emSumHt,
	runNo, dataset, "iH_{T}", "DemuxSums/DemSumHt.pdf", 40
	);
  
  // plot demux sum Mht
  create_plot(
	hwSumMht,
	emSumMht,
	runNo, dataset, "iMHT", "DemuxSums/DemSumMht.pdf", 20
	);
    
  // plot demux sum Mht phi
  create_plot(
	hwMhtPhi,
	emMhtPhi,
	runNo, dataset, "MHT i#phi", "DemuxSums/DemMhtPhi.pdf"
	);
  // ========================= demux sums end ========================

  // ======================== e/gamma start ======================

  // plot MP e/g Et
  create_plot(
	hwMPEgEt,
	emMPEgEt,
	runNo, dataset, "e/#gamma iE_{T}", "Egs/EgEt.pdf", 4
	);

  // plot MP e/g eta
  create_plot(
	hwMPEgEta,
	emMPEgEta,
	runNo, dataset, "e/#gamma i#eta", "Egs/EgEta.pdf"
	);
  
  // plot MP e/g phi
  create_plot(
	hwMPEgPhi,
	emMPEgPhi,
	runNo, dataset, "e/#gamma i#phi", "Egs/EgPhi.pdf"
	);

  // plot demux e/g Et
  create_plot(
	hwEgEt,
	emEgEt,
	runNo, dataset, "e/#gamma iE_{T}", "DemuxEgs/EgEt.pdf", 4
	);

  // plot demux e/g eta
  create_plot(
	hwEgEta,
	emEgEta,
	runNo, dataset, "e/#gamma i#eta", "DemuxEgs/EgEta.pdf"
	);

  // plot demux e/g phi
  create_plot(
	hwEgPhi,
	emEgPhi,
	runNo, dataset, "e/#gamma i#phi", "DemuxEgs/EgPhi.pdf"
	);
  // ======================== e/gamma end ========================

  // ========================= tau start =========================

  // plot MP tau Et
  create_plot(
	hwMPTauEt,
	emMPTauEt,
	runNo, dataset, "#tau iE_{T}", "Taus/TauEt.pdf", 4
	);

  // plot MP tau eta
  create_plot(
	hwMPTauEta,
	emMPTauEta,
	runNo, dataset, "#tau i#eta", "Taus/TauEta.pdf"
	);

  // plot MP tau phi
  create_plot(
	hwMPTauPhi,
	emMPTauPhi,
	runNo, dataset, "#tau i#phi", "Taus/TauPhi.pdf"
	);
  
  // plot demux tau Et
  create_plot(
	hwTauEt,
	emTauEt,
	runNo, dataset, "#tau iE_{T}", "DemuxTaus/TauEt.pdf", 4
	);

  // plot demux tau eta
  create_plot(
	hwTauEta,
	emTauEta,
	runNo, dataset, "#tau i#eta", "DemuxTaus/TauEta.pdf"
	);
  
  // plot demux tau phi
  create_plot(
	hwTauPhi,
	emTauPhi,
	runNo, dataset, "#tau i#phi", "DemuxTaus/TauPhi.pdf"
	);
  // ========================== tau end ==========================

};
