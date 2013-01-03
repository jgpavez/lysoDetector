
#ifndef lysoDetectorEventAction_h
#define lysoDetectorEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4THitsMap.hh"
#include "globals.hh"

class lysoDetectorSteppingAction;


class lysoDetectorEventAction : public G4UserEventAction
{
  public:
    lysoDetectorEventAction();
    virtual ~lysoDetectorEventAction();

    static lysoDetectorEventAction* Instance();
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    G4double GetSum(G4THitsMap<G4double>*) const;
    G4THitsMap<G4double>* GetHitsCollection(const G4String&, const G4Event*) const;
    void Reset();

    G4double GetEnergySum() const { return fEnergySum; }

  private:
    static lysoDetectorEventAction* fgInstance;
    G4double fEnergySum;

};

#endif
