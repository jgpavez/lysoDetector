

#include "lysoDetectorRunAction.hh"
#include "lysoDetectorPrimaryGeneratorAction.hh"
#include "lysoDetectorAnalysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

lysoDetectorRunAction::lysoDetectorRunAction()
: G4UserRunAction()
{
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;
  const G4double picogray  = 1.e-12*gray;

  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);
}

lysoDetectorRunAction::~lysoDetectorRunAction()
{}

void lysoDetectorRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  G4RunManager::GetRunManager()->SetRandomNumberStore(false);



  G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

  G4String fileName = "hist";
  analysisManager->OpenFile(fileName);
  analysisManager->SetFirstHistoId(1);
  analysisManager->CreateH1("1","Energy Distribution",1000,1.5,4.5);
  analysisManager->CreateH1("2","Photon Time Distribution",1000,-50,350); 
}

void lysoDetectorRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int nofEvents = aRun->GetNumberOfEvent();

  const G4ParticleGun* particleGun
    = lysoDetectorPrimaryGeneratorAction::Instance()->GetParticleGun();
  G4String particleName
    = particleGun->GetParticleDefinition()->GetParticleName();
  G4double particleEnergy = particleGun->GetParticleEnergy();

  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << " The run consists of " << nofEvents << " "<< particleName << " of "
     <<   G4BestUnit(particleEnergy,"Energy")
     << G4endl;


  analysisManager->Write();
  analysisManager->CloseFile();

  delete G4AnalysisManager::Instance();
}

