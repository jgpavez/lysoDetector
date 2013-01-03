 

#include "lysoDetectorConstruction.hh"
#include "lysoDetectorPrimaryGeneratorAction.hh"
#include "lysoDetectorRunAction.hh"
#include "lysoDetectorEventAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BIC_EMY.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

int main(int argc, char **argv)
{

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  G4RunManager *runManager = new G4RunManager;
  
  runManager->SetUserInitialization(new lysoDetectorConstruction());

  G4VModularPhysicsList* physicsList = new QGSP_BIC_EMY;
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserAction(new lysoDetectorPrimaryGeneratorAction());

  runManager->SetUserAction(new lysoDetectorEventAction());

  runManager->SetUserAction(new lysoDetectorRunAction());

  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
    UImanager->ApplyCommand("/control/execute init.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif
  }

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
