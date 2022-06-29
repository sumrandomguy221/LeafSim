#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"

#include <vector>

class MagneticField;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;

/// Detector construction

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    
    virtual void ConstructSDandField();

    void ConstructMaterials();
    
private:
    void DefineCommands();

    G4GenericMessenger* fMessenger;
    
  std::vector<double> vLeftLeafX;
  std::vector<double> vLeftLeafY;

  std::vector<double> vRightLeafX;
  std::vector<double> vRightLeafY;

  std::vector<double> cInterValue1;
  std::vector<double> cInterValue2;

    std::vector<G4VisAttributes*> fVisAttributes;
    
//    static G4ThreadLocal MagneticField* fMagneticField;
//    static G4ThreadLocal G4FieldManager* fFieldMgr;
  
    G4LogicalVolume* fDriftLogical;
    G4LogicalVolume* fLeaf1ALogical;
    G4LogicalVolume* fLeaf1BLogical;
    G4LogicalVolume* fLeaf1CLogical;
    G4LogicalVolume* fLeaf2ALogical;
    G4LogicalVolume* fLeaf2BLogical;
    G4LogicalVolume* fLeaf2CLogical;
    G4LogicalVolume* fCellLogical;
    G4LogicalVolume* fWaterLogical[1600];
};

#endif
