
#ifndef lysoDetectorConstruction_h
#define lysoDetectorConstruction_h

#include "lysoDetectorRunAction.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class lysoDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    lysoDetectorConstruction(lysoDetectorRunAction* runAction);
    virtual ~lysoDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
  private:
    lysoDetectorRunAction *runAction;

};

#endif
