#include <G4SystemOfUnits.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4SDManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4UniformMagField.hh>
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
#include <G4ChordFinder.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4Box.hh>

#include "DetectorConstruction.hh"
#include "CustomSD.hh"

#include <G4Tubs.hh>
#include <G4SubtractionSolid.hh>

#include "LogicalCrystalVolume.hh"  // added to exploit modified Geant4

// DetectorConstruction::Construct, i.e. where the setup geometry is implemented
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // NIST database
    G4NistManager* nist = G4NistManager::Instance();
	
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // define the world and all the setup stuff (materials, volumes) here, or...
    
    // different operation regimes (complete with settings in ConstructSDandField)
    G4bool bCrys = true; // crystal placement switch
	
	/*
    // colors
    G4VisAttributes* grey = new G4VisAttributes(true, G4Colour::Grey());
    G4VisAttributes* cyan = new G4VisAttributes(true, G4Colour::Cyan());
	G4VisAttributes* green = new G4VisAttributes(true, G4Colour::Green());
	G4VisAttributes* white = new G4VisAttributes(true, G4Colour::White());
	G4VisAttributes* invisible = new G4VisAttributes(false);
	G4VisAttributes* brown = new G4VisAttributes(true, G4Colour::Brown());
    G4VisAttributes* blue = new G4VisAttributes(true, G4Colour::Blue());

    // off-the-shelf materials (from NIST)
    G4Material* air = nist->FindOrBuildMaterial("G4_AIR"); // air
    G4Material* silicon = nist->FindOrBuildMaterial("G4_Si");  // silicon
    G4Material* aluminum = nist->FindOrBuildMaterial("G4_Al");  // aluminum
    G4Material* iron = nist->FindOrBuildMaterial("G4_Fe"); // iron 
    G4Material* pwo = nist->FindOrBuildMaterial("G4_PbWO4"); // lead tungstate
    G4Material* bgo = nist->FindOrBuildMaterial("G4_BGO"); // BGO
    G4Material* ss = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"); // stainless steel
	G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic"); // vacuum
	G4Material* lead = nist->FindOrBuildMaterial("G4_Pb"); // lead
	G4Material* plastic = nist->FindOrBuildMaterial("G4_POLYSTYRENE"); // polystirene, for scintillators 
	*/
	
    // colors
    G4VisAttributes* grey = new G4VisAttributes(true, G4Colour::Grey());
	G4VisAttributes* green = new G4VisAttributes(true, G4Colour::Green());
	G4VisAttributes* invisible = new G4VisAttributes(false);
	G4VisAttributes* brown = new G4VisAttributes(true, G4Colour::Brown());
    G4VisAttributes* blue = new G4VisAttributes(true, G4Colour::Blue());

    // off-the-shelf materials (from NIST)
    // G4Material* air = nist->FindOrBuildMaterial("G4_AIR"); // air
    // G4Material* silicon = nist->FindOrBuildMaterial("G4_Si");  // silicon
    // G4Material* aluminum = nist->FindOrBuildMaterial("G4_Al");  // aluminum
    // G4Material* iron = nist->FindOrBuildMaterial("G4_Fe"); // iron 
    G4Material* pwo = nist->FindOrBuildMaterial("G4_PbWO4"); // lead tungstate
	// G4Material* leadGlass = nist->FindOrBuildMaterial("G4_GLASS_LEAD"); // lead glass
    // G4Material* ss = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"); // stainless steel
	G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic"); // vacuum

    // world
    G4double worldSizeX = 1 * m;
    G4double worldSizeY = 1 * m;
    G4double worldSizeZ = 1 * m;
    G4VSolid* worldBox = new G4Box("World_Solid", worldSizeX / 2, worldSizeY / 2, worldSizeZ / 2);

    G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, vacuum, "World_Logical"); // MATTIA 2024: world now is vacuum
    G4VisAttributes* visAttrWorld = new G4VisAttributes();
    visAttrWorld->SetVisibility(false);
    worldLog->SetVisAttributes(visAttrWorld);

    G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr, {}, worldLog, "World", nullptr, false, 0);
	
    // detector positions, general
    // G4double zTrackerSiliCentre0 = 0 * cm;  // silicon tracking module 0 longitudinal centre (4.5 cm long)
    // G4double zTrackerSiliCentre1 = zTrackerSiliCentre0 + 15.90 * m;  // silicon tracking module 1 longitudinal centre (4.5 cm long)
    // G4double zMagnet = zTrackerSiliCentre0 + 67*cm + 511*cm + 0.5*m + 1.5*m;  // bending magnet longitudinal centre
    // G4double zTgt = zTrackerSiliCentre1 + 1.57*m;  // target longitudinal centre
    // G4double zPhCaloFront = zTgt + 0.29 * m;  // photon calorimeter longitudinal front
    // G4double zPipe0Front = zTrackerSiliCentre0 + 67 * cm; // pipe 0 (between tracking module 0 & magnet) longitudinal front
    // G4double zPipe1Front = zMagnet + 1.5*m + 0.5*m; // pipe 1 (between magnet & tracking module 1) longitudinal front
    // G4double dzPipe0 = (zMagnet - 1.5*m - 0.5*m) - zPipe0Front; // pipe 0 length
    // G4double dzPipe1 = (zTrackerSiliCentre1 - 50 * cm) - zPipe1Front; // pipe 1 length
    // G4double xECaloShift = 10.1*cm; // electron calorimeter shift off beam nominal path
    G4double zTgt = 25*cm;  // target longitudinal centre
	
	// target
    G4double tgtThickness = 2*zTgt; // MATTIA 2024: new target dimensions
    G4double tgtWidth = 2*zTgt;
    G4double tgtHeight = 2*zTgt;
    G4VSolid* tgtBox = new G4Box("Tgt_Solid", tgtWidth / 2, tgtHeight / 2, tgtThickness / 2);
    
    if(bChanneling){
	    LogicalCrystalVolume* tgtLog_channeling = new LogicalCrystalVolume(tgtBox, pwo, "Tgt_Logical");
	    tgtLog_channeling->SetVisAttributes(blue);
		
		if(bCrys){new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTgt), tgtLog_channeling, "Tgt", worldLog, false, 0);};
		
	}else{
	    G4LogicalVolume* tgtLog = new G4LogicalVolume(tgtBox, pwo, "Tgt_Logical");
	    tgtLog->SetVisAttributes(blue);
		
		if(bCrys){new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTgt), tgtLog, "Tgt", worldLog, false, 0);};
	}
	
	// side target
	// G4LogicalVolume* tgtLog_side = new G4LogicalVolume(tgtBox, pwo, "TgtSide_Logical");
	// if(bCrys){new G4PVPlacement(nullptr, G4ThreeVector(3.2*cm, 0, zTgt), tgtLog_side, "TgtSide", worldLog, false, 0);};
	
    // bending magnet
    // G4double magnetYokeThickness = 2 * m;
    // G4double magnetYokeWidth = 1.75 * m;
    // G4double magnetYokeHeight = 1.12 * m;
    // G4double magnetFieldWidth = 1.12 * m;
	// G4double magnetFieldHeight = 110 * mm;
    // G4VSolid* magnetYokeBox_0 = new G4Box("magnetYokeOuter_Solid", magnetYokeWidth / 2, magnetYokeHeight / 2, magnetYokeThickness / 2);
	// G4VSolid* magnetYokeBox_I = new G4Box("magnetYokeInner_Solid", magnetFieldWidth / 2, magnetFieldHeight / 2, magnetYokeThickness / 2 + 0.1*mm);
	// G4SubtractionSolid* magnetYokeBox = new G4SubtractionSolid("magnetYoke_Solid", magnetYokeBox_0, magnetYokeBox_I);
    // G4VSolid* magnetFieldBox = new G4Box("magnetField_Solid", magnetFieldWidth / 2, magnetFieldHeight / 2, magnetYokeThickness / 2);
	
    // G4LogicalVolume* magnetYokeLog = new G4LogicalVolume(magnetYokeBox, iron, "magnetYoke_Logical");
    // magnetYokeLog->SetVisAttributes(green);
    // G4LogicalVolume* magnetFieldLog = new G4LogicalVolume(magnetFieldBox, air, "magnetField_Logical");
    // G4VisAttributes* visAttrField = new G4VisAttributes();
    // visAttrField->SetVisibility(false);
    // magnetFieldLog->SetVisAttributes(visAttrField);
	
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zMagnet), magnetYokeLog, "magnetYoke", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zMagnet), magnetFieldLog, "magnetField", worldLog, false, 0);
	
    // silicon tracking system
    // G4double trackerSiliThickness = 300 * um;
    // G4double trackerSiliWidth = 384 * 50 * um;
    // G4double trackerSiliHeight = 384 * 50 * um;
    // G4VSolid* trackerSiliBox = new G4Box("TrackerSili_Solid", trackerSiliWidth / 2, trackerSiliHeight / 2, trackerSiliThickness / 2);
    // G4double trackerSiliBigThickness = 410 * um;
    // G4double trackerSiliBigWidth = 384 * 242 * um;
    // G4double trackerSiliBigHeight = 384 * 242 * um;
    // G4VSolid* trackerSiliBigBox = new G4Box("TrackerSiliBig_Solid", trackerSiliBigWidth / 2, trackerSiliBigHeight / 2, trackerSiliBigThickness / 2);
    // G4double trackerAlThickness = 10 * um;
    // G4double trackerAlWidth = 2 * cm;
    // G4double trackerAlBigHeight = 10 * cm;
    // G4double trackerAlBigWidth = 10 * cm;
    // G4double trackerAlHeight = 2 * cm;
    // G4VSolid* trackerAlBox = new G4Box("TrackerAl_Solid", trackerAlWidth / 2, trackerAlHeight / 2, trackerAlThickness / 2);
    // G4VSolid* trackerAlBigBox = new G4Box("TrackerAlBig_Solid", trackerAlBigWidth / 2, trackerAlBigHeight / 2, trackerAlThickness / 2);
	// G4SubtractionSolid* trackerAlSideFrontBox = new G4SubtractionSolid("TrackerAlBoxSideFront_Solid", new G4Box("", 20 * cm, 20 * cm, 1 * mm), new G4Box("", trackerAlWidth / 2, trackerAlHeight / 2, 1.1*mm));
	// G4SubtractionSolid* trackerAlSideFrontBigBox = new G4SubtractionSolid("TrackerAlBoxSideFrontBig_Solid", new G4Box("", 20 * cm, 20 * cm, 1 * mm), new G4Box("", trackerAlBigWidth / 2, trackerAlBigHeight / 2, 1.1*mm));
	// G4VSolid* trackerAlSideBackBox = new G4Box("TrackerAlBoxSideBack_Solid", 20 * cm, 20 * cm, 1 * mm);
	
    // G4LogicalVolume* trackerSiliLog0 = new G4LogicalVolume(trackerSiliBox, silicon, "TrackerSili_Logical_0");  // tracker 0 is small
    // G4LogicalVolume* trackerSiliLog1 = new G4LogicalVolume(trackerSiliBox, silicon, "TrackerSili_Logical_1");  // tracker 1 is small
    // G4LogicalVolume* trackerAlLog = new G4LogicalVolume(trackerAlBox, aluminum, "TrackerAl_Logical");
    // G4LogicalVolume* trackerAlBigLog = new G4LogicalVolume(trackerAlBigBox, aluminum, "TrackerAlBig_Logical");
    // G4LogicalVolume* trackerAlSideFrontLog = new G4LogicalVolume(trackerAlSideFrontBox, aluminum, "TrackerAlSideFrontBox_Logical");
	// G4LogicalVolume* trackerAlSideFrontBigLog = new G4LogicalVolume(trackerAlSideFrontBigBox, aluminum, "TrackerAlSideFrontBigBox_Logical");
	// G4LogicalVolume* trackerAlSideBackLog = new G4LogicalVolume(trackerAlSideBackBox, aluminum, "TrackerAlBoxSideBack_Logical");
    // trackerSiliLog0->SetVisAttributes(grey);
    // trackerSiliLog1->SetVisAttributes(grey);
    // trackerAlLog->SetVisAttributes(grey);
    // trackerAlBigLog->SetVisAttributes(grey);
    // trackerAlSideFrontLog->SetVisAttributes(grey);
    // trackerAlSideFrontBigLog->SetVisAttributes(grey);
    // trackerAlSideBackLog->SetVisAttributes(grey);
	
    // G4double zTrackerAlFrontCentre0 = zTrackerSiliCentre0 - (2.25*cm - trackerAlThickness / 2);
    // G4double zTrackerAlBackCentre0 = zTrackerSiliCentre0 + (2.25*cm - trackerAlThickness / 2);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerSiliCentre0), trackerSiliLog0, "TrackerSili_0", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre0), trackerAlLog, "TrackerSili_0_Al_Front", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre0), trackerAlLog, "TrackerSili_0_Al_Rear", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre0), trackerAlSideFrontLog, "TrackerSili_0_Al_Front", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre0), trackerAlSideBackLog, "TrackerSili_0_Al_Rear", worldLog, false, 0);
    // G4double zTrackerAlFrontCentre1 = zTrackerSiliCentre1 - (2.25*cm - trackerAlThickness / 2);
    // G4double zTrackerAlBackCentre1 = zTrackerSiliCentre1 + (2.25*cm - trackerAlThickness / 2);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerSiliCentre1), trackerSiliLog1, "TrackerSili_1", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre1), trackerAlLog, "TrackerSili_1_Al_Front", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre1), trackerAlLog, "TrackerSili_1_Al_Rear", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre1), trackerAlSideFrontLog, "TrackerSili_1_Al_Front", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre1), trackerAlSideBackLog, "TrackerSili_1_Al_Rear", worldLog, false, 0);
	
    // electromagnetic calorimeter
    // G4double caloPhThickness = 40 * cm;
    // G4double caloPhWidthChannel = 10 * cm;
    // G4double caloPhHeightChannel = 10 * cm;
    // G4VSolid* caloPhChannelBox = new G4Box("PhCalTest_Solid", caloPhWidthChannel / 2, caloPhHeightChannel / 2, caloPhThickness / 2);
	
    // G4LogicalVolume* caloPhLog_TL = new G4LogicalVolume(caloPhChannelBox, leadGlass, "PhCalTest_Logical_TL");
    // G4LogicalVolume* caloPhLog_TR = new G4LogicalVolume(caloPhChannelBox, leadGlass, "PhCalTest_Logical_TR");
    // G4LogicalVolume* caloPhLog_CL = new G4LogicalVolume(caloPhChannelBox, leadGlass, "PhCalTest_Logical_CL");
    // G4LogicalVolume* caloPhLog_CC = new G4LogicalVolume(caloPhChannelBox, leadGlass, "PhCalTest_Logical_CC");
    // G4LogicalVolume* caloPhLog_CR = new G4LogicalVolume(caloPhChannelBox, leadGlass, "PhCalTest_Logical_CR");
    // G4LogicalVolume* caloPhLog_BL = new G4LogicalVolume(caloPhChannelBox, leadGlass, "PhCalTest_Logical_BL");
    // G4LogicalVolume* caloPhLog_BR = new G4LogicalVolume(caloPhChannelBox, leadGlass, "PhCalTest_Logical_BR");
    // caloPhLog_TL->SetVisAttributes(brown);
    // caloPhLog_TR->SetVisAttributes(brown);
    // caloPhLog_CL->SetVisAttributes(brown);
    // caloPhLog_CC->SetVisAttributes(brown);
    // caloPhLog_CR->SetVisAttributes(brown);
    // caloPhLog_BL->SetVisAttributes(brown);
    // caloPhLog_BR->SetVisAttributes(brown);
	
    // new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPhCaloFront + caloPhThickness / 2), caloPhLog_CC, "PhCalTestCC", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(-xECaloShift, 0, zPhCaloFront + caloPhThickness / 2), caloPhLog_CL, "PhCalTestCL", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(xECaloShift, 0, zPhCaloFront + caloPhThickness / 2), caloPhLog_CR, "PhCalTestCR", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(-xECaloShift/2, xECaloShift, zPhCaloFront + caloPhThickness / 2), caloPhLog_TL, "PhCalTestTL", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(xECaloShift/2, xECaloShift, zPhCaloFront + caloPhThickness / 2), caloPhLog_TR, "PhCalTestTR", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(-xECaloShift/2, -xECaloShift, zPhCaloFront + caloPhThickness / 2), caloPhLog_BL, "PhCalTestBL", worldLog, false, 0);
    // new G4PVPlacement(nullptr, G4ThreeVector(xECaloShift/2, -xECaloShift, zPhCaloFront + caloPhThickness / 2), caloPhLog_BR, "PhCalTestBR", worldLog, false, 0);
	
	// beam pipes
	// G4double pi = 3.1415926535;
	// G4double pipeOuterRadius = 9*cm;
	// G4double pipeInnerRadius = 8*cm;
    // G4VSolid* pipe0Solid = new G4Tubs("Pipe0_Solid", pipeInnerRadius, pipeOuterRadius, dzPipe0/2, 0, 2*pi);
    // G4VSolid* pipe1Solid = new G4Tubs("Pipe1_Solid", pipeInnerRadius, pipeOuterRadius, dzPipe1/2, 0, 2*pi);
	// G4VSolid* pipe0VacuumSolid = new G4Tubs("Pipe0Vacuum_Solid", 0, pipeInnerRadius, (dzPipe0-2*cm)/2, 0, 2*pi);
	// G4VSolid* pipe1VacuumSolid = new G4Tubs("Pipe1Vacuum_Solid", 0, pipeInnerRadius, (dzPipe1-2*cm)/2, 0, 2*pi);
	
    // G4LogicalVolume* pipe0Log = new G4LogicalVolume(pipe0Solid, ss, "Pipe0_Logical");
    // G4LogicalVolume* pipe1Log = new G4LogicalVolume(pipe1Solid, ss, "Pipe1_Logical");
    // G4LogicalVolume* pipe0VacuumLog = new G4LogicalVolume(pipe0VacuumSolid, vacuum, "Pipe0Vacuum_Logical");
    // G4LogicalVolume* pipe1VacuumLog = new G4LogicalVolume(pipe1VacuumSolid, vacuum, "Pipe1Vacuum_Logical");
    // pipe0Log->SetVisAttributes(grey);
    // pipe1Log->SetVisAttributes(grey);
    // pipe0VacuumLog->SetVisAttributes(invisible);
    // pipe1VacuumLog->SetVisAttributes(invisible);
	
	// new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPipe0Front + dzPipe0/2), pipe0Log, "Pipe0", worldLog, false, 0);
	// new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPipe1Front + dzPipe1/2), pipe1Log, "Pipe1", worldLog, false, 0);
	// new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPipe0Front + dzPipe0/2), pipe0VacuumLog, "Pipe0Vacuum", worldLog, false, 0);
	// new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPipe1Front + dzPipe1/2), pipe1VacuumLog, "Pipe1Vacuum", worldLog, false, 0);

    // --------------------------------------------------
    // ...uncomment this line for the test setup (implemented in include/TestMode.cc) 
    // G4VPhysicalVolume* worldPhys = SetupTest(nist);
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
    // print list of defined material
    G4cout << "-----" << G4endl;
    G4cout << "| DetectorConstruction.cc: material list" << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    G4cout << "-----" << G4endl;
	
    return worldPhys;
}

// DetectorConstruction::ConstructSDandField, i.e. where the sensitive detectors and magnetic fields are implemented
void DetectorConstruction::ConstructSDandField()
{
    // load the sensitive detector manager
    G4SDManager* sdm = G4SDManager::GetSDMpointer();
    sdm->SetVerboseLevel(1);  // set sensitive detector manager verbosity here

    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // create the sensitive detectors and bin them to the logical volumes here, or...
    
    // different operation regimes (complete with settings in Construct)
    G4bool bField = false; // magnetic field switch
	
    // bending magnet field
    // G4double magnField = bField ? 0.0 : 0.0 * tesla; // magnet is always off here
    // G4UniformMagField* bendingField = new G4UniformMagField(G4ThreeVector(0., magnField, 0.));
    // G4LogicalVolume* fieldLog = G4LogicalVolumeStore::GetInstance()->GetVolume("magnetField_Logical");
    // G4FieldManager* fieldManager = new G4FieldManager(bendingField);
    // fieldLog->SetFieldManager(fieldManager, true);
	
    // SD -- photon calorimeter
    // VolumeEDepSD* gammaCalSD_TL = new VolumeEDepSD("PhCalTest_TL_SD");
    // VolumeEDepSD* gammaCalSD_TR = new VolumeEDepSD("PhCalTest_TR_SD");
    // VolumeEDepSD* gammaCalSD_CL = new VolumeEDepSD("PhCalTest_CL_SD");
    // VolumeEDepSD* gammaCalSD_CC = new VolumeEDepSD("PhCalTest_CC_SD");
    // VolumeEDepSD* gammaCalSD_CR = new VolumeEDepSD("PhCalTest_CR_SD");
    // VolumeEDepSD* gammaCalSD_BL = new VolumeEDepSD("PhCalTest_BL_SD");
    // VolumeEDepSD* gammaCalSD_BR = new VolumeEDepSD("PhCalTest_BR_SD");
    // SetSensitiveDetector("PhCalTest_Logical_TL", gammaCalSD_TL);
    // SetSensitiveDetector("PhCalTest_Logical_TR", gammaCalSD_TR);
    // SetSensitiveDetector("PhCalTest_Logical_CL", gammaCalSD_CL);
    // SetSensitiveDetector("PhCalTest_Logical_CC", gammaCalSD_CC);
    // SetSensitiveDetector("PhCalTest_Logical_CR", gammaCalSD_CR);
    // SetSensitiveDetector("PhCalTest_Logical_BL", gammaCalSD_BL);
    // SetSensitiveDetector("PhCalTest_Logical_BR", gammaCalSD_BR);
    // sdm->AddNewDetector(gammaCalSD_TL);
    // sdm->AddNewDetector(gammaCalSD_TR);
    // sdm->AddNewDetector(gammaCalSD_CL);
    // sdm->AddNewDetector(gammaCalSD_CC);
    // sdm->AddNewDetector(gammaCalSD_CR);
    // sdm->AddNewDetector(gammaCalSD_BL);
    // sdm->AddNewDetector(gammaCalSD_BR);
    
    // SD -- crystal
    VolumeEDepSD* tgtSD = new VolumeEDepSD("Tgt_SD");
    SetSensitiveDetector("Tgt_Logical", tgtSD);
    sdm->AddNewDetector(tgtSD);
	
    // SD -- side crystal
    // VolumeEDepSD* tgtSD_side = new VolumeEDepSD("TgtSide_SD");
    // SetSensitiveDetector("TgtSide_Logical", tgtSD_side);
    // sdm->AddNewDetector(tgtSD_side);
	
    // SD -- tracking system (1 sensitive detector per tracking plane)
    // VolumeTrackingSD* trackerSD0 = new VolumeTrackingSD("Tracker_SD_0");
    // SetSensitiveDetector("TrackerSili_Logical_0", trackerSD0);
    // sdm->AddNewDetector(trackerSD0);
    // VolumeTrackingSD* trackerSD1 = new VolumeTrackingSD("Tracker_SD_1");
    // SetSensitiveDetector("TrackerSili_Logical_1", trackerSD1);
    // sdm->AddNewDetector(trackerSD1);
	
    // --------------------------------------------------
    // ...uncomment this line for the test sensitive detectors (implemented in include/TestMode.cc)
    // SDTest(sdm);
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
}

// ============================================================
// ============================================================
// implement custom methods here

