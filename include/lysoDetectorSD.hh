
#ifndef lysoDetectorSD_h
#define lysoDetectorSD_h 1

#include "globals.hh"
#include "lysoDetectorRunAction.hh"
#include "G4VSensitiveDetector.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4HCofThisEvent.hh"

class lysoDetectorSD : public G4VSensitiveDetector
{
public:
        lysoDetectorSD(const G4String&, lysoDetectorRunAction*);
        ~lysoDetectorSD();

        void   Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void   EndOfEvent(G4HCofThisEvent*);

private:
        lysoDetectorRunAction* runAction;
        G4double energy;
        G4double time;
        G4ThreeVector pos;
        G4ThreeVector mom;
        G4StepPoint* point;

};

#endif

