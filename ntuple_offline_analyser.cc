#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1F.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisL1UpgradeDataFormat.h"

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

  std::cout << "No. hw entries: " << hwTree->GetEntriesFast()  << std::endl;
  std::cout << "No. emu entries: " << emuTree->GetEntriesFast() << std::endl;

  //TCanvas * hwCanvas = new TCanvas("hwCanvas", "hwCanvas");

  TH1F * hwJetEt = new TH1F("hwJetEt", "hwJetEt", 500, 0, 1000);

  for (unsigned int i = 0; i < hwTree->GetEntriesFast(); ++i) {
    hwTree->GetEntry(i);
    // std::cout << "Jets in event " << i << ": " << hw->jetEt.size() << std::endl;

    for (unsigned int j = 0; j < hw->jetEt.size(); ++j) {
      std::cout << hw->jetBx[j] << std::endl;
      if (hw->jetBx[j] == 0) {
        std::cout << "Adding a jet with energy: " << hw->jetEt[j] << std::endl;
        hwJetEt->Fill(hw->jetEt[j]);
      }
    }
  }
    
  hwJetEt->Draw("ap");
  // hwCanvas->SaveAs("hwJets.pdf");
}
