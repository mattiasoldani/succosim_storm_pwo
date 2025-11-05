#include <G4SystemOfUnits.hh>
#include <G4ParticleTable.hh>
#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <Randomize.hh>
#include <math.h>

#include "PrimaryGeneratorAction.hh"

using namespace std;

// PrimaryGeneratorAction::PrimaryGeneratorAction, i.e. where the particle gun is defined and the beam features that are constant throughout the run are set
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    // define the particle gun
    fGun = new G4ParticleGun();
	
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // set the beam features that are constant throughout the run here, or...
    
    // constant features: particle type, energy
    G4ParticleDefinition* myParticle;
    myParticle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fGun->SetParticleDefinition(myParticle);
    //G4double energy = 120 * GeV;
    //fGun->SetParticleEnergy(energy);

    // --------------------------------------------------
    // ...uncomment this line for the test beam (implemented in include/TestMode.cc)
    // BeamFeaturesFxdTest(fGun);
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{delete fGun;}

// PrimaryGeneratorAction::GeneratePrimaries, i.e. where the beam features that change at each event are set and the particles are shot
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // set the beam features that change at each event here, or...
    
    // randomised features: source position (transverse, uniform in a square, whereas longitudinal is fixed), momentum direction
    G4double zFixed = -200 * um;
    G4double xRnd = (3*mm) * (G4UniformRand()-0.5);
    G4double yRnd = (8*mm) * (G4UniformRand()-0.5);
    G4ThreeVector gunPosition = G4ThreeVector(xRnd, yRnd, zFixed);
    fGun->SetParticlePosition(gunPosition);
    G4double thRndX = G4RandGauss::shoot(0, 100E-6);
    G4double thRndY = G4RandGauss::shoot(0, 100E-6);
    G4ThreeVector momentumDirection = G4ThreeVector(thRndX, thRndY, 1 - pow(pow(thRndX, 2) + pow(thRndY, 2), 0.5));
    fGun->SetParticleMomentumDirection(momentumDirection);
	G4double energy = (120*GeV) + (0.035*120*GeV*G4RandGauss::shoot(0, 1));  // energy smearing: dp/p = 3.5%
	fGun->SetParticleEnergy(energy);

    // --------------------------------------------------
    // ...uncomment this line for the test beam (implemented in include/TestMode.cc)
    // BeamFeaturesRndTest(fGun);
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
    // shot the event primary particle
    fGun->GeneratePrimaryVertex(anEvent);
}
