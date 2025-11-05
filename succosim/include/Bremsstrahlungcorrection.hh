#ifndef BREMSSTRAHLUNGCORRECTION_H
#define BREMSSTRAHLUNGCORRECTION_H
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
///
/// \brief The BremsstrahlungCorrection class It is used for accumulate correction
/// functions of Bremsstralung
///
class BremsstrahlungCorrection
{
public:
    BremsstrahlungCorrection();
    /// PWO are old coefficients, PWO_001 are for axis 001 PWO
    enum axis_material { PWO=0,W111=1,PWO_001=2,W111_electron=7,W111_positron=4,W111_3mrad_electron=5,W111_3mrad_positron=6, PWO_100=3 }; // edited by Mattia
    //enum axis_material { PWO=0,W111=1,PWO_001=2,W111_electron=3,W111_positron=4,W111_3mrad_electron=5,W111_3mrad_positron=6};
    G4double  ComputeCorrection(G4double eKineticEnergy);
    void set_axis(axis_material a);
protected:
    axis_material axis;
    G4double ComputeCorrectionPWO(G4double eKineticEnergy);
    G4double ComputeCorrectionPWO_001(G4double eKineticEnergy);
    G4double ComputeCorrectionW_111_electron(G4double eKineticEnergy);
    G4double ComputeCorrectionW_111_positron(G4double eKineticEnergy);
    G4double ComputeCorrectionW_111_3mrad_electron(G4double eKineticEnergy);
    G4double ComputeCorrectionW_111_3mrad_positron(G4double eKineticEnergy);
    G4double ComputeCorrectionPWO_100(G4double eKineticEnergy);  // added by Mattia
	
};

#endif // BREMSSTRAHLUNGCORRECTION_H
