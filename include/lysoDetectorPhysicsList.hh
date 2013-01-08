#ifndef lysoDetectorPhysicsList_h 
#define lysoDetectorPhysicsList_h 1

#include "G4VUserPhysicsList.hh"

class lysoDetectorPhysicsList: public G4VUserPhysicsList
{

public:

	lysoDetectorPhysicsList();
	~lysoDetectorPhysicsList();

private:

	void ConstructParticle();
	void ConstructProcess();
	void SetCuts();

	void ConstructEM();
	void ConstructOp();
	void ConstructScintillation();

};

#endif
