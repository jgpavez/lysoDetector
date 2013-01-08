#include "globals.hh"
#include "lysoDetectorSD.hh"
#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"
#include "lysoDetectorAnalysis.hh"


lysoDetectorSD::lysoDetectorSD(const G4String& name, lysoDetectorRunAction* runaction)
:G4VSensitiveDetector(name),
  runAction(runaction),
  energy(0.),
  time(0.),
  pos(G4ThreeVector(0.,0.,0.)),
  mom(G4ThreeVector(0.,0.,0.)),
  point(0)
{
}

lysoDetectorSD::~lysoDetectorSD(){}

void lysoDetectorSD::Initialize(G4HCofThisEvent*){}

G4bool lysoDetectorSD::ProcessHits(G4Step* step, G4TouchableHistory*){
  G4cout << "Processing Hit " << std::endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhoton()){
    point = step->GetPreStepPoint();
    pos = point->GetPosition();
    mom = point->GetMomentumDirection();
    if ( std::abs(pos(2)) == 22.51*mm && pos(2)*mom(2)>0){
	energy = point->GetKineticEnergy();
	time = point->GetGlobalTime();
  	analysisManager->FillH1(1, energy/eV);
	analysisManager->FillH1(2,time/ns);
    }		
 
  }
  return true;

}	

void lysoDetectorSD::EndOfEvent(G4HCofThisEvent* ){}
