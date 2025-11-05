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
//
// Implementation of G4LogicalCrystalVolume
//
// 21-04-16, created by E.Bagli
// 
// --------------------------------------------------------------------

#include "LogicalCrystalVolume.hh"
#include "G4ExtendedMaterial.hh"
#include "G4CrystalExtension.hh"
#include "G4VMaterialExtension.hh"

std::vector<G4LogicalVolume*> LogicalCrystalVolume::fLCVvec;

// --------------------------------------------------------------------

LogicalCrystalVolume::
LogicalCrystalVolume(G4VSolid* pSolid, G4Material* pMaterial,
                       const G4String& name, G4FieldManager* pFieldMgr,
                       G4VSensitiveDetector* pSDetector,
                       G4UserLimits* pULimits, G4bool optimise)
: G4LogicalVolume(pSolid,pMaterial,name,pFieldMgr,pSDetector,pULimits,optimise),
 verboseLevel(0)
{
   fLCVvec.push_back(this);
}

// --------------------------------------------------------------------

LogicalCrystalVolume::~LogicalCrystalVolume()
{
   fLCVvec.erase( std::remove(fLCVvec.begin(),fLCVvec.end(), this ),
                  fLCVvec.end() );
}

// --------------------------------------------------------------------

G4bool LogicalCrystalVolume::IsLattice(G4LogicalVolume* aLV)
{
  return std::find(fLCVvec.begin(), fLCVvec.end(), aLV) != fLCVvec.end();
}


// --------------------------------------------------------------------
