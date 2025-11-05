//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// 
//------------------ G4GammaConversion physics process -------------------------
//                   by Michel Maire, 24 May 1996
//
// Modified by Michel Maire and Vladimir Ivanchenko
//
// -----------------------------------------------------------------------------

#include "GammaConversionCrystal.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4BetheHeitlerModel.hh"
#include "G4PairProductionRelModel.hh"
#include "G4Electron.hh"
#include "G4EmParameters.hh"

/// Новые модели рожедения электрон-позитронных пар
#include "BetheHeitlerModel.hh"
#include "PairProductionRelModel.hh"
#include "G4BetheHeitlerModel.hh"
#include "G4PairProductionRelModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

GammaConversionCrystal::GammaConversionCrystal(const G4String& processName,
  G4ProcessType type):G4VEmProcess (processName, type),
    isInitialised(false)
{
  SetMinKinEnergy(2.0*electron_mass_c2);
  SetProcessSubType(fGammaGeneralProcess);
  SetStartFromNullFlag(true);
  SetBuildTableFlag(true);
  SetSecondaryParticle(G4Electron::Electron());
  SetLambdaBinning(220);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GammaConversionCrystal::~GammaConversionCrystal()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GammaConversionCrystal::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GammaConversionCrystal::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    G4EmParameters* param = G4EmParameters::Instance();
    G4double emin = std::max(param->MinKinEnergy(), 2*electron_mass_c2);
    G4double emax = param->MaxKinEnergy();

    SetMinKinEnergy(emin);
    /// It is used new model BetheHeitlerModel
     if(!EmModel(0)) { SetEmModel(new BetheHeitlerModel()); }
    //if(!EmModel(0)) { SetEmModel(new G4BetheHeitlerModel()); }
    EmModel(0)->SetLowEnergyLimit(emin);
    G4double energyLimit = std::min(EmModel(0)->HighEnergyLimit(), 80*GeV);
    EmModel(0)->SetHighEnergyLimit(energyLimit);
    AddEmModel(1, EmModel(0));

    if(emax > energyLimit) {
      /// It is used new model PairProductionRelModel
       if(!EmModel(1)) { SetEmModel(new PairProductionRelModel()); }
      //if(!EmModel(1)) { SetEmModel(new G4PairProductionRelModel()); }
      EmModel(1)->SetLowEnergyLimit(energyLimit);
      EmModel(1)->SetHighEnergyLimit(emax);
      AddEmModel(1, EmModel(1));
    }
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double GammaConversionCrystal::MinPrimaryEnergy(const G4ParticleDefinition*,
					     const G4Material*)
{
  return 2*electron_mass_c2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GammaConversionCrystal::PrintInfo()
{}         

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GammaConversionCrystal::ProcessDescription(std::ostream& out) const
{
  out << "  Gamma conversion for crystal";
  G4VEmProcess::ProcessDescription(out);
}
//G4double GammaConversionCrystal::PostStepGetPhysicalInteractionLength(
//                             const G4Track& track,
//                             G4double   previousStepSize,
//                             G4ForceCondition* condition)
//{
//    *condition = NotForced;
//    G4double x = DBL_MAX;
//    G4LogicalVolume* aLV = track.GetVolume()->GetLogicalVolume();
//    G4LogicalVolume* aNLV = track.GetNextVolume()->GetLogicalVolume();

//    if(LogicalCrystalVolume::IsLattice(aLV) == true &&
//            LogicalCrystalVolume::IsLattice(aNLV) == true){
//        return G4VEmProcess::PostStepGetPhysicalInteractionLength(track,previousStepSize,condition);
//    }
//    else{
//        x= DBL_MAX;
//        theNumberOfInteractionLengthLeft = -1.0;
//        currentInteractionLength = DBL_MAX;

//    }
//    return x;
//}

//G4VParticleChange* GammaConversionCrystal::PostStepDoIt(const G4Track& track,
//                                              const G4Step& step)
//{
//    G4LogicalVolume* aLV = track.GetVolume()->GetLogicalVolume();
//    G4LogicalVolume* aNLV = track.GetNextVolume()->GetLogicalVolume();

//    if(LogicalCrystalVolume::IsLattice(aLV) == true &&
//            LogicalCrystalVolume::IsLattice(aNLV) == true){
//        return G4VEmProcess::PostStepDoIt(track,step);
//    }
//    else
//    {
//        theNumberOfInteractionLengthLeft = -1.0;
//        mfpKinEnergy = DBL_MAX;
//        fParticleChange.InitializeForPostStep(track);
//        return &fParticleChange;
//    }
//}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... 
