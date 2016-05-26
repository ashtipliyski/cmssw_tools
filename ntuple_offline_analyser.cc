#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"


void create_plot(
  TH1D * hw,
  TH1D * emu,
  int runNo,
  const char * dataset,
  const char * xLabel,
  const char * exportPath,
  int rebin = 1,
  int energy = 13  
  ) {

  // define latex container to hold the title
  TLatex n;
  n.SetNDC();
  n.SetTextFont(52);
  n.SetTextSize(0.04);

  // create canvas that will hold each plot
  TCanvas* canv = new TCanvas("canv","canvas");

  // top pad (comparison)
  TPad* pad1 = new TPad("mainPad","mainPad",0,0.3,1,1);
  // bottom (resuduals) pad
  TPad* pad2 = new TPad("ratioPad","ratioPad",0,0.05,1,0.3);

  // pad to contain trendline for hw-emu ratio of 1
  TPad* overlayPad = new TPad("pInv","pInv", 0,0.05,1,0.3);
  overlayPad->SetFillStyle(0);

  // create legend that will describe appearance of data points
  TLegend * leg = new TLegend(0.72,0.85,0.91,1);
  leg->SetFillColor(0);
  leg->SetNColumns(2);
  leg->AddEntry(hw,"Hardware", "p");//"l");
  leg->AddEntry(emu,"Emulator", "l");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);

  // optionally reduce segmentation in x to improve visibility of some plots
  hw->Rebin(rebin);
  emu->Rebin(rebin);

  // hw->SetTitle("");
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
   
  hw->DrawCopy();
  emu->Draw("same");
  
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

void ntuple_offline_analyser () {

  TFile * file = new TFile("L1Ntuples.root");
  if (file->IsZombie()) {
    std::cout << "Error opening file." << std::endl;
    exit(-1);
  }
  
  TTree * hwTree = (TTree*) file->Get("l1UpgradeTree/L1UpgradeTree");
  if (!hwTree) {
    std::cout << "Error: cannot open upgrade tree" << std::endl;
    return;
  }

  TTree * emuTree = (TTree*) file->Get("l1UpgradeEmuTree/L1UpgradeTree");
  if (!emuTree) {
    std::cout << "Error: cannot open emu tree" << std::endl;
    return;
  }
  
  std::cout << "Trees opened!" << std::endl;


  L1Analysis::L1AnalysisL1UpgradeDataFormat * hw =
    new L1Analysis::L1AnalysisL1UpgradeDataFormat();
  L1Analysis::L1AnalysisL1UpgradeDataFormat * emu =
    new L1Analysis::L1AnalysisL1UpgradeDataFormat();

  hwTree->SetBranchAddress("L1Upgrade", &hw);
  emuTree->SetBranchAddress("L1Upgrade", &emu);

  // std::cout << "No. hw entries: " << hwTree->GetEntriesFast()  << std::endl;
  // std::cout << "No. emu entries: " << emuTree->GetEntriesFast() << std::endl;

  TCanvas * hwCanvas = new TCanvas("hwCanvas", "");

  TH1D * hwJetEt = new TH1D("hwJetEt", "", 50, 1, 0);

  for (unsigned int i = 0; i < hwTree->GetEntriesFast(); ++i) {
    hwTree->GetEntry(i);

    for (unsigned int j = 0; j < hw->jetEt.size(); ++j) {
      if (hw->jetBx[j] == 0) {
        hwJetEt->Fill(hw->jetEt[j]);
      }
    }
  }
    
  hwJetEt->Draw();

  TH1D * emuJetEt = new TH1D("emuJetEt", "", 50, 1, 0);

  for (unsigned int i = 0; i < emuTree->GetEntriesFast(); ++i) {
    emuTree->GetEntry(i);

    for (unsigned int j = 0; j < emu->jetEt.size(); ++j) {
      if (emu->jetBx[j] == 0) {
        emuJetEt->Fill(emu->jetEt[j]);
      }
    }
  }
    
  emuJetEt->Draw();

  create_plot(
	hwJetEt, emuJetEt, 272798, "Zero Bias", "Jet iE_{T}", "jetEt.pdf"
	);
  
}


