#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>
#include <algorithm>
#include <vector>


void Script_Leaf_Analysis()
{

  //Open and create root file

  TFile *f = new TFile("total1600_2.root");
  TFile *g = new TFile("script_output_2.root","recreate");

  if (!f) { return; }

  TTree *out =(TTree*)f->Get("out");

/*
  //Branch Names

  Double_t = PhantomEnergy, WaterHitX, WaterHitY, WaterHitZ, WaterHitE, WaterHitT;
  Int_t    = WaterHitDet;
  Double_t = TotalWaterEnergy;

  //Get Branches

  TBranch *b_PhantomEnergy = out->GetBranch("PhantomEnergy");
  TBranch *b_WaterHitX = out->GetBranch("WaterHitX");
  TBranch *b_WaterHitY = out->GetBranch("WaterHitY");
  TBranch *b_WaterHitZ = out->GetBranch("WaterHitZ");
  TBranch *b_WaterHitE = out->GetBranch("WaterHitE");
  TBranch *b_WaterHitT = out->GetBranch("WaterHitT");
  TBranch *b_WaterHitDet = out->GetBranch("WaterHitDet");
  TBranch *b_TotalWaterEnergy = out->GetBranch("TotalWaterEnergy");
*/

  //Leaf names

  Double_t PhantomEnergy, WaterHitX, WaterHitY, WaterHitZ, WaterHitE, WaterHitT;
  Int_t    WaterHitDet;
  Double_t TotalWaterEnergy;

  //Get Associated Leaves

  TLeaf *l_PhantomEnergy = out->GetLeaf("PhantomEnergy");
  TLeaf *l_WaterHitX = out->GetLeaf("WaterHitX");
  TLeaf *l_WaterHitY = out->GetLeaf("WaterHitY");
  TLeaf *l_WaterHitZ = out->GetLeaf("WaterHitZ");
  TLeaf *l_WaterHitE = out->GetLeaf("WaterHitE");
  TLeaf *l_WaterHitT = out->GetLeaf("WaterHitT");
  TLeaf *l_WaterHitDet = out->GetLeaf("WaterHitDet");
  TLeaf *l_TotalWaterEnergy = out->GetLeaf("TotalWaterEnergy");

  //Set Addresses

  l_PhantomEnergy    -> SetAddress(&PhantomEnergy);
  l_WaterHitX        -> SetAddress(&WaterHitX);
  l_WaterHitY        -> SetAddress(&WaterHitY);
  l_WaterHitZ	     -> SetAddress(&WaterHitZ);
  l_WaterHitE	     -> SetAddress(&WaterHitE);
  l_WaterHitT	     -> SetAddress(&WaterHitT);
  l_WaterHitDet	     -> SetAddress(&WaterHitDet);
  l_TotalWaterEnergy -> SetAddress(&TotalWaterEnergy);

  //Histograms

  int n = out->GetEntries();

  double vox_num = 1600.;

  TH1D *hTotalWaterEnergy = new TH1D("hTotalWaterEnergy","Total Energy Deposited in Water Voxels (MeV)", 200, 0., 10.);
  TH2D *hTotWatE_WatHitDet = new TH2D("hTotWatE_WatHitDet","",200,0.,10.,200,0,vox_num);
  TH2D *hHitsTotal = new TH2D("hHitsTotal","Total Hits on Voxel #", 200, 0., vox_num, 200, 0., 1000.);
  TH2D *hParticleFluence = new TH2D("hParticleFluence","Particle Fluence of Voxel (cm^-2)", 200, 0., vox_num, 200, 0., 60.);

  TH2D *hXyDet = new TH2D("xyDet","",200,-2000.,2000.,200,-2000.,2000.);

  
  //Fill Histograms

  for (int i=0; i < n; ++i) {
    out->GetEntry(i);
    if (TotalWaterEnergy == 0) continue;
    hTotalWaterEnergy->Fill(TotalWaterEnergy);
    hTotWatE_WatHitDet->Fill(TotalWaterEnergy,WaterHitDet);
    hXyDet->Fill(WaterHitX,WaterHitY);
    }

  ///////////////////////
  //Fluence Calculation//
  ///////////////////////

  double energyHit = 0.;
  double totalEnergyHit = 0.;
  
  double voxel_Length = 4.; //*cm
  double voxel_Width  = 4.; //*cm
  double voxel_Area = voxel_Length*voxel_Width; //*cm^2

  int vox_cnt = 1600; //# of water voxels

  std::vector<int> x1;
  std::vector<int> x2;

  for (int i=0; i < n; ++i){
    out->GetEntry(i);
    x1.push_back(WaterHitDet);
  }

  //Particle Hit Counter Start--------------------

  int hitsTotal = 0;
  double pFluence = 0.; // Particle Fluence (cm^-2)
  double eFluence = 0.; // Energy Fluence (MeV/cm^2)

  for (int i=1; i < vox_cnt+1; ++i){
    for (std::vector<int>::iterator it = x1.begin() ; it != x1.end(); ++it) {
	if (*it == i) ++hitsTotal;
    }
  hHitsTotal->Fill(i,hitsTotal);
  x2.push_back(hitsTotal);

  pFluence = hitsTotal/voxel_Area; // Calculate Particle Fluence
  hParticleFluence->Fill(i,pFluence);
  std::cout << "Particle Fluence (cm^-2): " << pFluence << std::endl;
  pFluence = 0.;
  hitsTotal = 0;
  }

  //Particle Hit Counter End---------------------

  //Histogram Options

  hHitsTotal->SetMarkerStyle(33);
  hParticleFluence->SetMarkerStyle(33);

  hXyDet->SetOption("colz");

  //Write

  hTotalWaterEnergy->Write();
  hTotWatE_WatHitDet->Write();
  hHitsTotal->Write();
  hParticleFluence->Write();
  hXyDet->Write();

  //Close File

  f->Close();
}
  
