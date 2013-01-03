

#include "lysoDetectorEventAction.hh"

#include "lysoDetectorRunAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "lysoDetectorAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>

lysoDetectorEventAction* lysoDetectorEventAction::fgInstance = 0;


lysoDetectorEventAction* lysoDetectorEventAction::Instance()
{
  return fgInstance;
}

lysoDetectorEventAction::lysoDetectorEventAction():
			G4UserEventAction(),
			fEnergySum(0.)
{
  fgInstance = this;
}

lysoDetectorEventAction::~lysoDetectorEventAction()
{
  fgInstance = 0;
}


G4THitsMap<G4double> * lysoDetectorEventAction::GetHitsCollection(const G4String& hcName,
                                                        const G4Event* event) const
{
  G4int hcID
        = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
  G4THitsMap<G4double>* hitsCollection
        = static_cast<G4THitsMap<G4double>*>(
                event->GetHCofThisEvent()->GetHC(hcID));

  if ( !hitsCollection){
        G4cerr << "Cannot access hitsCollection" << hcName << G4endl;
        exit(1);
  }
  return hitsCollection;

}

G4double lysoDetectorEventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0.;
  std::map <G4int, G4double*>::iterator it;
  for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++){
        G4cout << "energy" << *(it->second) << std::endl;
        sumValue += *(it->second);
  }
  return sumValue;
}

void lysoDetectorEventAction::BeginOfEventAction(const G4Event* event)
{
  G4int eventNb = event->GetEventID();
  G4cout << "\n---> Begin of event: " << eventNb << G4endl;

}

void lysoDetectorEventAction::EndOfEventAction(const G4Event* event)
{
  fEnergySum = GetSum(GetHitsCollection("Absorber/Edep", event));
  // G4cout << "Energia " << fEnergySum << std::endl;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(1, fEnergySum);
}

void lysoDetectorEventAction::Reset()
{
  fEnergySum = 0.;

}

