
#ifndef lysoDetectorRunAction_h
#define lysoDetectorRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class lysoDetectorRunAction : public G4UserRunAction
{

  public: 
    lysoDetectorRunAction();
    virtual ~lysoDetectorRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

};

#endif
