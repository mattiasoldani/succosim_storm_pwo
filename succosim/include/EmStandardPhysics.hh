#ifndef EmStandardPhysics_h
#define EmStandardPhysics_h 1
 
#include "G4VPhysicsConstructor.hh"
#include "globals.hh"
#include "G4EmParticleList.hh"
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
///
/// \brief The EmStandardPhysics class класс электроммагнитных взаимодействий.
/// За основу взят стандартный класс Geant4, изменены некоторые физические процессы
/// которые меняются в ориентированном кристалле
/// This is class of standard electromagnetic interactions.
/// The standard  G4EmStandardPhysics class was taking as basic. The some classes
/// were changed which were changed into oriented crystal.
///
class EmStandardPhysics : public G4VPhysicsConstructor
{
public:

  explicit EmStandardPhysics(G4int ver=0, const G4String& name="");
 
  virtual ~EmStandardPhysics();

  virtual void ConstructParticle();
    /// The changing processes in crystal.
    /// Bremsstrahlung for e- and e+ and pair production for gamma
  virtual void ConstructProcess();
 
 private:
   G4int  verbose;
   G4EmParticleList partList;
 };


 
 //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
 #endif
