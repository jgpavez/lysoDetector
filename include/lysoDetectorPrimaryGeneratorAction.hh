
#ifndef lysoDetectorPrimaryGeneratorAction_h
#define lysoDetectorPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class lysoDetectorConstruction;


class lysoDetectorPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    lysoDetectorPrimaryGeneratorAction();
    virtual ~lysoDetectorPrimaryGeneratorAction();

    static const lysoDetectorPrimaryGeneratorAction* Instance();
  
    virtual void GeneratePrimaries(G4Event*);
 
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    static lysoDetectorPrimaryGeneratorAction* fgInstance;

    G4ParticleGun* fParticleGun;
};

#endif
