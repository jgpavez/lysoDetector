#include "lysoDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

lysoDetectorConstruction::lysoDetectorConstruction()
: G4VUserDetectorConstruction()
{ }


lysoDetectorConstruction::~lysoDetectorConstruction()
{ }


G4VPhysicalVolume* lysoDetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();

  G4int ncomponents;
  G4double crystal_sizeXY = 4*mm, crystal_sizeZ = 45*mm;


  G4double prelude_density = 7.4*g/cm3;
  G4Material* prelude = new G4Material("prelude", prelude_density, ncomponents=4);
  prelude->AddElement(nist->FindOrBuildElement("Lu"),71*perCent);
  prelude->AddElement(nist->FindOrBuildElement("Si"),7*perCent);
  prelude->AddElement(nist->FindOrBuildElement("O"), 18*perCent);
  prelude->AddElement(nist->FindOrBuildElement("Y"), 4*perCent);

  G4Material* scincillator = new G4Material("scincillator", prelude_density ,ncomponents=2);
  scincillator->AddMaterial(prelude,99.81*perCent);
  scincillator->AddElement(nist->FindOrBuildElement("Ce"), 0.19*perCent);

  G4bool checkOverlaps = true;


  G4double world_sizeXYZ = 6*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld =
    new G4Box("World",                       
       0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          
                        world_mat,           
                        "World");            

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                    
                      G4ThreeVector(),      
                      logicWorld,            
                      "World",             
                      0,                     
                      false,               
                      0,                    
                      checkOverlaps);        


  G4Box* solidEnv =
    new G4Box("Envelope",                   
        0.5*crystal_sizeXY, 0.5*crystal_sizeXY, 0.5*crystal_sizeZ); 

  G4LogicalVolume* logicEnv =
    new G4LogicalVolume(solidEnv,            
                        scincillator,           
                        "Envelope");         

  new G4PVPlacement(0,                       
                    G4ThreeVector(),         
                    logicEnv,                
                    "Envelope",             
                    logicWorld,              
                    false,                  
                    0,                      
                    checkOverlaps);          

  G4Box* solidDetector =
        new G4Box("Detector", 0.5*crystal_sizeXY, 0.5*crystal_sizeXY, 1.*mm);

  G4LogicalVolume* logicDetectorRight =
        new G4LogicalVolume(solidDetector,
                        nist->FindOrBuildMaterial("G4_Al"),
                        "RightDetector");
  G4LogicalVolume* logicDetectorLeft =
        new G4LogicalVolume(solidDetector,
                        nist->FindOrBuildMaterial("G4_Al"),
                        "LeftDetector");


  new G4PVPlacement(0,
                G4ThreeVector(0.,0.,0.5*crystal_sizeZ),
                logicDetectorRight,
                "DetectorRight",
                logicWorld,
                false,
                checkOverlaps);
  new G4PVPlacement(0,
                G4ThreeVector(0.,0.,-0.5*crystal_sizeZ),
                logicDetectorLeft,
                "DetectorLeft",
                logicWorld,
                false,
                checkOverlaps);


  G4MultiFunctionalDetector* absDetector
        = new G4MultiFunctionalDetector("Absorber");
  G4VPrimitiveScorer* primitive;
  primitive = new G4PSEnergyDeposit("Edep");

  absDetector->RegisterPrimitive(primitive);
  G4SDManager::GetSDMpointer()->AddNewDetector(absDetector);

//  logicEnv->SetSensitiveDetector(absDetector);

  logicDetectorRight->SetSensitiveDetector(absDetector);
  logicDetectorLeft->SetSensitiveDetector(absDetector);

  return physWorld;
}

  

