#include "lysoDetectorConstruction.hh"
#include "lysoDetectorSD.hh"
#include "lysoDetectorRunAction.hh"

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

lysoDetectorConstruction::lysoDetectorConstruction(lysoDetectorRunAction* runaction)
: G4VUserDetectorConstruction(),runAction(runaction)
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


  G4Material* air  = nist->FindOrBuildMaterial("G4_AIR");

  G4Material* scintillator = new G4Material("scintillator", prelude_density ,ncomponents=2);
  scintillator->AddMaterial(prelude,99.81*perCent);
  scintillator->AddElement(nist->FindOrBuildElement("Ce"), 0.19*perCent);

  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable* mpt2 = new G4MaterialPropertiesTable();


  const G4int num = 20;

  G4double ene[num]   =  {1.79*eV, 1.85*eV, 1.91*eV, 1.97*eV,

			2.04*eV, 2.11*eV, 2.19*eV, 2.27*eV,

			2.36*eV, 2.45*eV, 2.56*eV, 2.67*eV,

			2.80*eV, 2.94*eV, 3.09*eV, 3.25*eV,

			3.44*eV, 3.65*eV, 3.89*eV, 4.16*eV};



  G4double fast[num]  =  {0.01, 0.10, 0.20, 0.50,

			0.90, 1.70, 2.90, 5.00,

			8.30, 12.5, 17.0, 22.9,

			26.4, 25.6, 16.8, 4.20,

			0.30, 0.20, 0.10, 0.01};



  G4double rLyso[num] =  {1.81, 1.81, 1.81, 1.81,

			1.81, 1.81, 1.81, 1.81,

			1.81, 1.81, 1.81, 1.81,

			1.81, 1.81, 1.81, 1.81,

			1.81, 1.81, 1.81, 1.81};



  G4double rAir[num]  =  {1.00, 1.00, 1.00, 1.00,

			1.00, 1.00, 1.00, 1.00,

			1.00, 1.00, 1.00, 1.00,

			1.00, 1.00, 1.00, 1.00,

			1.00, 1.00, 1.00, 1.00};



  G4double abs[num]   =  {3.5*m, 3.5*m, 3.5*m, 3.5*m,

			3.5*m, 3.5*m, 3.5*m, 3.5*m,

			3.5*m, 3.5*m, 3.5*m, 3.5*m,

			3.5*m, 3.5*m, 3.5*m, 3.5*m,

			3.5*m, 3.5*m, 3.5*m, 3.5*m};



  mpt->AddProperty("FASTCOMPONENT", ene, fast, num);

  mpt->AddProperty("RINDEX", ene, rLyso , num);

  mpt->AddProperty("ABSLENGTH", ene, abs, num);

  mpt->AddConstProperty("SCINTILLATIONYIELD",32/keV);

  mpt->AddConstProperty("RESOLUTIONSCALE", 1);

  mpt->AddConstProperty("FASTTIMECONSTANT",41*ns);
  scintillator->SetMaterialPropertiesTable(mpt);

  mpt2->AddProperty("RINDEX" , ene, rAir , num);
  air->SetMaterialPropertiesTable(mpt2);

  G4double world_sizeXYZ = 6*cm;

  G4Box* solidWorld =
    new G4Box("World",                       
       0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          
                        air,           
                        "World");            

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                    
                      G4ThreeVector(),      
                      logicWorld,            
                      "World",             
                      0,                     
                      false,               
                      0);        


  G4Box* solidEnv =
    new G4Box("Envelope",                   
        0.5*crystal_sizeXY, 0.5*crystal_sizeXY, 0.5*crystal_sizeZ); 

  G4LogicalVolume* logicEnv =
    new G4LogicalVolume(solidEnv,            
                        scintillator,           
                        "Envelope");         

  new G4PVPlacement(0,                       
                    G4ThreeVector(),         
                    logicEnv,                
                    "Envelope",             
                    logicWorld,              
                    false,                  
                    0,                      
                    0);          

  G4double SDz = 0.15*mm;
  G4double gap = 0.01*mm;
  G4Box* solidDetector =
        new G4Box("Detector", 0.5*crystal_sizeXY, 0.5*crystal_sizeXY, SDz);

  G4LogicalVolume* logicDetector =
        new G4LogicalVolume(solidDetector,
                        air,
                        "Detector");
  //G4LogicalVolume* logicDetectorLeft =
  //      new G4LogicalVolume(solidDetector,
  //                      air,
  //                      "LeftDetector");


  new G4PVPlacement(0,
                G4ThreeVector(0.,0.,0.5*crystal_sizeZ + SDz + gap),
                logicDetector,
                "DetectorRight",
                logicWorld,
                false,
                0);
  new G4PVPlacement(0,
                G4ThreeVector(0.,0.,-0.5*crystal_sizeZ - SDz - gap),
                logicDetector,
                "DetectorLeft",
                logicWorld,
                false,
                0);

// Set sentisitive detector
  lysoDetectorSD* sd = new lysoDetectorSD("lysoDetector/SensitiveDetector",runAction);
  G4SDManager* sdm = G4SDManager::GetSDMpointer();

   


//  logicEnv->SetSensitiveDetector(sd);

  logicDetector->SetSensitiveDetector(sd);

  sdm->AddNewDetector(sd);

  return physWorld;
}

  

