
#include "lysoDetectorPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"


lysoDetectorPrimaryGeneratorAction* lysoDetectorPrimaryGeneratorAction::fgInstance = 0;

const lysoDetectorPrimaryGeneratorAction* lysoDetectorPrimaryGeneratorAction::Instance()
{

  return fgInstance;
}


lysoDetectorPrimaryGeneratorAction::lysoDetectorPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  //fParticleGun->SetParticleEnergy(6.*MeV);

  fgInstance = this;
}


lysoDetectorPrimaryGeneratorAction::~lysoDetectorPrimaryGeneratorAction()
{
  delete fParticleGun;
  fgInstance = 0;
}

void lysoDetectorPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double envSizeXY = 0;
  G4double envSizeZ = 0;
  G4LogicalVolume* envLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
  G4Box* envBox = NULL;
  if ( envLV ) envBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  if ( envBox ) {
    envSizeXY = envBox->GetXHalfLength()*2.;
    envSizeZ = envBox->GetZHalfLength()*2.;
  }
  else  {
    G4cerr << "Envelope volume of box shape not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

  G4double size = 0.8;
  G4double x0 = 2.5 * envSizeXY;
  G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double z0 = size * 0.8 * (G4UniformRand()-0.5);


  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1,0,0));
  fParticleGun->SetParticleEnergy(511.*keV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

