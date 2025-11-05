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
// ---------------------------------------------------------------------------
//
// class LogicalCrystalVolume
//
// Class description:
//
// Specialised logical volume for the description of crystals.
// The object can be created only with an extended material
// with a crystal extension. The class handles the orientation
// of the crystal axes wrt the solid to which the crystal is attached.
//

// History:
//
// 21-04-16, created by E.Bagli
//
// ---------------------------------------------------------------------------
#ifndef LOGICALCRYSTALVOLUME_HH
#define G4LOGICALCRYSTALVOLUME_HH 1

#include <vector>
#include "G4LogicalVolume.hh"

class LogicalCrystalVolume : public G4LogicalVolume
{
  public:

    LogicalCrystalVolume(G4VSolid* pSolid,
                           G4Material* pMaterial,
                           const G4String& name,
                           G4FieldManager* pFieldMgr=0,
                           G4VSensitiveDetector* pSDetector=0,
                           G4UserLimits* pULimits=0,
                           G4bool optimise=true);

    ~LogicalCrystalVolume();
    
    G4bool IsExtended() const { return true; }
      // Return true if it is not a base-class object.

    
    void SetVerbose(G4int aInt) { verboseLevel = aInt; }

    static G4bool IsLattice(G4LogicalVolume* aLV);

  private:
    
    G4int verboseLevel;

    static std::vector<G4LogicalVolume*> fLCVvec;
};

#endif
