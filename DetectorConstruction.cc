#include "DetectorConstruction.hh"
#include "PhantomSD.hh"
#include "WaterSD.hh"
//#include "MagneticField.hh"
#include "CellParameterisation.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"
#include "G4PSFlatSurfaceFlux.hh"
#include "G4MultiFunctionalDetector.hh"

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>

#include "spline.h"

// Magnetic Field Manager
/*
G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;
*/
// Detector Construction Logical Intialization

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fMessenger(0),
  fVisAttributes(),
  fDriftLogical(0),
  fLeaf1ALogical(0),
  fLeaf1BLogical(0),
  fLeaf2ALogical(0),
  fCellLogical(0)
{
 // DefineCommands();
}

DetectorConstruction::~DetectorConstruction()
{
    delete fMessenger;
    
    for (G4int i=0; i<G4int(fVisAttributes.size()); ++i) 
    {
      delete fVisAttributes[i];
    }  
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Construct materials
    ConstructMaterials();
    G4Material* air = G4Material::GetMaterial("G4_AIR");
    G4Material* argonGas = G4Material::GetMaterial("G4_Ar");
    G4Material* scintillator 
      = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    G4Material* tung = G4Material::GetMaterial("G4_W");
    G4Material* lead = G4Material::GetMaterial("G4_Pb");
    G4Material* CsI = G4Material::GetMaterial("G4_CESIUM_IODIDE");
    G4Material* water = G4Material::GetMaterial("G4_WATER");

    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;
  
    // Geometry

    // experimental hall (world volume)
    G4VSolid* worldSolid 
      = new G4Box("worldBox",7.*m,7.*m,7.*m);
    G4LogicalVolume* worldLogical
      = new G4LogicalVolume(worldSolid,air,"worldLogical");
    G4VPhysicalVolume* worldPhysical
      = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,
                          false,0,checkOverlaps);

    // drift chamber
    G4VSolid* driftSolid 
      = new G4Box("driftBox",2.1*m,2.5*m,2.1*m);
    fDriftLogical
      = new G4LogicalVolume(driftSolid,air,"driftLogical");
        new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),fDriftLogical,
                          "driftPhysical",worldLogical,
                          false,0,checkOverlaps);
  //////////////////// 
  // Leaf Structure //
  ////////////////////


    G4double voxelMatWidth = 200.*cm; // Width of voxel detector frame

    G4double maxLeafY = 200.*cm; // Determines max size of leaf
    G4double maxLeafX = voxelMatWidth/2;

    G4int leafCnt  = 10; // Number of leafs in group

    G4int grpCnt = 3;  // Number of leaf groups on each side

    G4double leafX = maxLeafX*.5; // Dimensions of leaf (left/right of beam axis)
    G4double leafY = maxLeafY/(grpCnt*leafCnt);  // up/down in beam axis
    G4double leafZ = 20.*cm; // along beam axis

    G4double moveX = 2.*cm; // Movement of leaf group 

    G4double leafGrpY = leafY*leafCnt; // Y length of leaf group

    G4double leafWorldX = 100.*cm; // Leaf structure position from the drift volume (Move L|R)
    G4double leafWorldZ = 100.*cm;  // Can move the structure closer to SD's

//Call Spline//

  std::ifstream data1("cTestRight.txt");
  if (!data1) {
    std::cerr << "PANIC: input file couldn't be opened" << std::endl;
  }
  else {
    double a=0., b=0.;
    while(data1 >> a >> b) {
      vRightLeafY.push_back(b);
      vRightLeafX.push_back(a);
    }
    tk::spline f;
    f.set_points(vRightLeafY, vRightLeafX);
    double c=0.;
    double d=0.;
    for (G4int i=0; i<leafCnt; i++){
	c = i*1.0/leafCnt;
	d = f(c);
	cInterValue1.push_back(d);
    }
  }

  std::ifstream data2("cTestLeft.txt");
  if (!data2) {
    std::cerr << "PANIC: input file couldn't be opened" << std::endl;
  }
  else {
    double a=0., b=0.;
    while(data2 >> a >> b) {
      vLeftLeafY.push_back(b);
      vLeftLeafX.push_back(a);
    }
    tk::spline g;
    g.set_points(vLeftLeafY, vLeftLeafX);
    double c=0.;
    double d=0.;
    for (G4int i=0; i<leafCnt; i++){
        c = i*1.0/leafCnt;
        d = g(c);
        cInterValue2.push_back(d);
    }
  }

// End Spline //

  // Right Side Leafs

  G4VSolid* leaf1ASolid
      = new G4Box("leafBox",leafX,leafY,leafZ);
    fLeaf1ALogical
      = new G4LogicalVolume(leaf1ASolid,tung,"leaf1ALogical");
    for (G4int i=0;i<leafCnt;i++)
        {
        G4double y1 = (i-floor((leafCnt-1)/2))*leafY*2;
	    G4double x1 = cInterValue1.at(i)*cm;

        new G4PVPlacement(0,G4ThreeVector(-leafWorldX + x1,y1,leafWorldZ),fLeaf1ALogical,
                          "leaf1APhysical",fDriftLogical,
                          false,i,checkOverlaps);
	}

  // Left Side Leafs 

    G4VSolid* leaf2ASolid
      = new G4Box("leafBox",leafX,leafY,leafZ);
    fLeaf2ALogical
      = new G4LogicalVolume(leaf2ASolid,tung,"leaf2ALogical");
    for (G4int i=0;i<leafCnt;i++)
        {
        G4double y1 = (i-floor((leafCnt-1)/2))*leafY*2;
	G4double x1 = cInterValue2.at(i)*cm;

        new G4PVPlacement(0,G4ThreeVector(leafWorldX+x1,y1,leafWorldZ),fLeaf2ALogical,
                          "leaf2APhysical",fDriftLogical,
                          false,i,checkOverlaps);
        }

    ///////////////////////
    // Phantom Cell Wall //
    ///////////////////////

    // Phantom Cell main solid
    G4VSolid* mainCellSolid
      = new G4Box("MainCellBox",2.0*m,2.0*m,1.*cm);
    G4LogicalVolume* mainCellLogical
      = new G4LogicalVolume(mainCellSolid,CsI,"MainCellLogical");
    new G4PVPlacement(0,G4ThreeVector(0.,0.,1.96*m),mainCellLogical,
                      "MainCellPhysical",fDriftLogical,
                      false,0,checkOverlaps);

    // Phantom Cells
    G4VSolid* cellSolid
      = new G4Box("cellBox",20.*cm,20.*cm,1.*cm);
    fCellLogical
      = new G4LogicalVolume(cellSolid,CsI,"cellLogical");
    G4VPVParameterisation* cellParam = new CellParameterisation();
    new G4PVParameterised("cellPhysical",fCellLogical,mainCellLogical,
                          kXAxis,100,cellParam);

    // Restructered Phantom (Water voxels) 
   G4double voxelThickness = 4.*cm;
   G4double voxelSize      = 2.*cm;
   G4double z1 		   = 1.9*m;

   for(int i = 0;i<1600;i++) {
     char name[256];
     sprintf(name,"waterBox%d",i+1);
     G4VSolid* waterSolid = new G4Box(name,voxelSize,voxelSize,voxelThickness);
     sprintf(name,"waterLogical%d",i+1);
     fWaterLogical[i] = new G4LogicalVolume(waterSolid,water,name);
     sprintf(name,"waterPhysical%d",i+1);

     G4int column = i / 40;
     G4int row = i % 40;

     G4double x1 = (column-19)*voxelSize*2 - voxelSize;
     G4double y1 = (row-19)*voxelSize*2 - voxelSize;
     new G4PVPlacement(0,G4ThreeVector(x1,y1,z1),fWaterLogical[i],name,
                      fDriftLogical,false,i,/*checkOverlaps*/0); 
   }  

  //////////////////
  // MFD Geometry // //Work in progress...maybe...
  //////////////////


    // visualization attributes ------------------------------------------------
    
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    visAttributes->SetVisibility(false);
    worldLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
  
    visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    fDriftLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    fLeaf1ALogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    fLeaf2ALogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0.0));
    visAttributes->SetVisibility(false);
    fVisAttributes.push_back(visAttributes);
/*
    visAttributes = new G4VisAttributes(G4Colour(0.9, 0.9, 0.9));
    phantomLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
*/
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.8888,0.0));
    visAttributes->SetVisibility(false);
    mainCellLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.0));
    fCellLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    // return the world physical volume
  return worldPhysical;
}

   // Construct sensitive detectors and magnetic field

void DetectorConstruction::ConstructSDandField()
{
  // Attach MFD to scoring solid
/*  
  G4String MFDname = "MFDet";
  G4String psName; //Primitive Scorer
  G4String fltName; //Filter
*/
  // MultiFunctionalDetector

//  G4MultiFunctionalDetector* MFDet= new G4MultiFunctionalDetector(MFDname);
     
  // Sensitive detectors
   
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
//  SDman->AddNewDetector( MFDet );
//  fWaterLogical->SetSensitiveDetector(MFDet);
  G4String SDname;
 
  //CsI Phantom
  G4VSensitiveDetector* phantom
    = new PhantomSD(SDname="/Phantom");
  SDman->AddNewDetector(phantom);
  fCellLogical->SetSensitiveDetector(phantom);

  //Water Phantom
  for(int i = 0;i<1600;i++) {
   char name[256];
   sprintf(name,"waterDetector%d",i+1);
   G4VSensitiveDetector* waterDetector
     = new WaterSD(SDname=name);
   SDman->AddNewDetector(waterDetector);
   fWaterLogical[i]->SetSensitiveDetector(waterDetector);
  }
/*
  //Water Phantom MFD
  for(int i = 0;i<100;i++) {
   char name[256];
   sprintf(name,"waterDetMFD%d",i+1);
   G4String psName(name);
   G4PSFlatSurfaceFlux* waterDetMFD
     = new G4PSFlatSurfaceFlux(psName,fFlux_In);
   MFDet->RegisterPrimitive(waterDetMFD);
  }
*/
   // Magnetic field
/*
   fMagneticField = new MagneticField();
   fFieldMgr = new G4FieldManager();
   fFieldMgr->SetDetectorField(fMagneticField);
   fFieldMgr->CreateChordFinder(fMagneticField);
   G4bool forceToAllDaughters = true;
   fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

   // Register the field and its manager for deleting
   G4AutoDelete::Register(fMagneticField);
   G4AutoDelete::Register(fFieldMgr);
*/
}

void DetectorConstruction::ConstructMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    // Air 
    nistManager->FindOrBuildMaterial("G4_AIR");
  
    // Argon gas
    nistManager->FindOrBuildMaterial("G4_Ar");

    // Scintillator
    // (PolyVinylToluene, C_9H_10)
    nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    // Tungsten
    nistManager->FindOrBuildMaterial("G4_W");

    // Cesium Iodide
    nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");

    // Lead
    nistManager->FindOrBuildMaterial("G4_Pb");
    
    // Water
    nistManager->FindOrBuildMaterial("G4_WATER");
    
    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


