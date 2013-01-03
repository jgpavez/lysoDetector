
#ifndef lysoDetectorConstruction_h
#define lysoDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class lysoDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    lysoDetectorConstruction();
    virtual ~lysoDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

};

#endif
