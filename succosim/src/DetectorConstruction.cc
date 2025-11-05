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

    // world
    G4double worldSizeX = 10 * m;
    G4double worldSizeY = 10 * m;
    G4double worldSizeZ = 80 * m;
    G4VSolid* worldBox = new G4Box("World_Solid", worldSizeX / 2, worldSizeY / 2, worldSizeZ / 2);

    G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, air, "World_Logical");
    G4VisAttributes* visAttrWorld = new G4VisAttributes();
    visAttrWorld->SetVisibility(false);
    worldLog->SetVisAttributes(visAttrWorld);

    G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr, {}, worldLog, "World", nullptr, false, 0);
	
    // detector positions, general
    G4double zTrackerSiliCentre0 = 0 * cm;  // silicon tracking module 0 longitudinal centre (4.5 cm long)
    G4double zTrackerSiliCentre1 = 13.65 * m;  // silicon tracking module 1 longitudinal centre (4.5 cm long)
    G4double zTrackerSiliCentre2 = zTrackerSiliCentre1 + 5.25 * m + (2.25 + 2.4) * cm;  // silicon tracking module 2 longitudinal centre (4.8 cm long)
    G4double zTrackerSiliCentre3 = zTrackerSiliCentre2 + 12.11 * m + 4.8 * cm;  // silicon tracking module 3 longitudinal centre (4.8 cm long)
    G4double zMagnet = zTrackerSiliCentre2 + 20 * cm + 1 * m;  // bending magnet longitudinal centre
    G4double zTgt = zTrackerSiliCentre1 + 2.25 * cm + 41 * cm;  // target longitudinal centre
    G4double zPhCaloFront = zTrackerSiliCentre3 + 2.4 * cm + 42 * cm + 2 * cm + 54.5 * cm;  // photon calorimeter longitudinal front
    G4double zPipe0Front = zTrackerSiliCentre0 + 2.25 * cm + 3.65 * m; // pipe 0 (between tracking modules 0 & 1) longitudinal front
    G4double zPipe1Front = zTgt + 90 * cm; // pipe 1 (between target & tracking module 2) longitudinal front
    G4double dzPipe0 = (zTrackerSiliCentre1 - 2.25 * cm - 44.5 * cm) - zPipe0Front; // pipe 0 length
    G4double dzPipe1 = (zTrackerSiliCentre2 - 2.4 * cm - 20 * cm) - zPipe1Front; // pipe 1 length
    G4double zECaloFront = zPhCaloFront - (54.5-5) * cm;  // electron calorimeter longitudinal front
    G4double xECaloShift = (3.5 + 14 + 11.5/2)*cm; // electron calorimeter shift off beam nominal path
	G4double zOutCounter = zTrackerSiliCentre2 - 2.4*cm -10*cm; // output multiplicity counter longitudinal centre
	G4double zVeto = zTrackerSiliCentre3 - 2.4*cm -1*cm; // veto scintillator longitudinal centre
	
    // target -- 1X0
    G4double tgtThickness = 0.8903 * cm;
    G4double tgtWidth = 3*0.8903 * cm;
    G4double tgtHeight = 3*0.8903 * cm;
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
    
    /*// target -- 2X0
    G4double tgtThickness = 2*0.8903 * cm;
    G4double tgtWidth = 0.8903 * cm;
    G4double tgtHeight = 3*0.8903 * cm;
    G4VSolid* tgtBox = new G4Box("Tgt_Solid", tgtWidth / 2, tgtHeight / 2, tgtThickness / 2);
    
    if(bChanneling){
	    LogicalCrystalVolume* tgtLog_channeling = new LogicalCrystalVolume(tgtBox, pwo, "Tgt_Logical");
	    tgtLog_channeling->SetVisAttributes(blue);
		
		if(bCrys){new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTgt), tgtLog_channeling, "Tgt", worldLog, false, 0);};
		
	}else{
	    G4LogicalVolume* tgtLog = new G4LogicalVolume(tgtBox, pwo, "Tgt_Logical");
	    tgtLog->SetVisAttributes(blue);
		
		if(bCrys){new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTgt), tgtLog, "Tgt", worldLog, false, 0);};
	}*/
	
    // bending magnet
    G4double magnetYokeThickness = 2 * m;
    G4double magnetYokeWidth = 1.75 * m;
    G4double magnetYokeHeight = 1.12 * m;
    G4double magnetFieldWidth = 1.12 * m;
	G4double magnetFieldHeight = 110 * mm;
    G4VSolid* magnetYokeBox_0 = new G4Box("magnetYokeOuter_Solid", magnetYokeWidth / 2, magnetYokeHeight / 2, magnetYokeThickness / 2);
	G4VSolid* magnetYokeBox_I = new G4Box("magnetYokeInner_Solid", magnetFieldWidth / 2, magnetFieldHeight / 2, magnetYokeThickness / 2 + 0.1*mm);
	G4SubtractionSolid* magnetYokeBox = new G4SubtractionSolid("magnetYoke_Solid", magnetYokeBox_0, magnetYokeBox_I);
    G4VSolid* magnetFieldBox = new G4Box("magnetField_Solid", magnetFieldWidth / 2, magnetFieldHeight / 2, magnetYokeThickness / 2);
	
    G4LogicalVolume* magnetYokeLog = new G4LogicalVolume(magnetYokeBox, iron, "magnetYoke_Logical");
    magnetYokeLog->SetVisAttributes(green);
    G4LogicalVolume* magnetFieldLog = new G4LogicalVolume(magnetFieldBox, air, "magnetField_Logical");
    G4VisAttributes* visAttrField = new G4VisAttributes();
    visAttrField->SetVisibility(false);
    magnetFieldLog->SetVisAttributes(visAttrField);
	
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zMagnet), magnetYokeLog, "magnetYoke", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zMagnet), magnetFieldLog, "magnetField", worldLog, false, 0);
	
    // silicon tracking system
    G4double trackerSiliThickness = 300 * um;
    G4double trackerSiliWidth = 384 * 50 * um;
    G4double trackerSiliHeight = 384 * 50 * um;
    G4VSolid* trackerSiliBox = new G4Box("TrackerSili_Solid", trackerSiliWidth / 2, trackerSiliHeight / 2, trackerSiliThickness / 2);
    G4double trackerSiliBigThickness = 410 * um;
    G4double trackerSiliBigWidth = 384 * 242 * um;
    G4double trackerSiliBigHeight = 384 * 242 * um;
    G4VSolid* trackerSiliBigBox = new G4Box("TrackerSiliBig_Solid", trackerSiliBigWidth / 2, trackerSiliBigHeight / 2, trackerSiliBigThickness / 2);
    G4double trackerAlThickness = 10 * um;
    G4double trackerAlWidth = 2 * cm;
    G4double trackerAlBigHeight = 10 * cm;
    G4double trackerAlBigWidth = 10 * cm;
    G4double trackerAlHeight = 2 * cm;
    G4VSolid* trackerAlBox = new G4Box("TrackerAl_Solid", trackerAlWidth / 2, trackerAlHeight / 2, trackerAlThickness / 2);
    G4VSolid* trackerAlBigBox = new G4Box("TrackerAlBig_Solid", trackerAlBigWidth / 2, trackerAlBigHeight / 2, trackerAlThickness / 2);
	G4SubtractionSolid* trackerAlSideFrontBox = new G4SubtractionSolid("TrackerAlBoxSideFront_Solid", new G4Box("", 20 * cm, 20 * cm, 1 * mm), new G4Box("", trackerAlWidth / 2, trackerAlHeight / 2, 1.1*mm));
	G4SubtractionSolid* trackerAlSideFrontBigBox = new G4SubtractionSolid("TrackerAlBoxSideFrontBig_Solid", new G4Box("", 20 * cm, 20 * cm, 1 * mm), new G4Box("", trackerAlBigWidth / 2, trackerAlBigHeight / 2, 1.1*mm));
	G4VSolid* trackerAlSideBackBox = new G4Box("TrackerAlBoxSideBack_Solid", 20 * cm, 20 * cm, 1 * mm);
	
    G4LogicalVolume* trackerSiliLog0 = new G4LogicalVolume(trackerSiliBox, silicon, "TrackerSili_Logical_0");  // tracker 0 is small
    G4LogicalVolume* trackerSiliLog1 = new G4LogicalVolume(trackerSiliBox, silicon, "TrackerSili_Logical_1");  // tracker 1 is small
    G4LogicalVolume* trackerSiliLog2_0 = new G4LogicalVolume(trackerSiliBigBox, silicon, "TrackerSili_Logical_2_0");  // tracker 2 is big & double -- 0th
    G4LogicalVolume* trackerSiliLog2_1 = new G4LogicalVolume(trackerSiliBigBox, silicon, "TrackerSili_Logical_2_1");  // tracker 2 is big & double -- 1st
    G4LogicalVolume* trackerSiliLog3_0 = new G4LogicalVolume(trackerSiliBigBox, silicon, "TrackerSili_Logical_3_0");  // tracker 3 is big & double -- 0th
    G4LogicalVolume* trackerSiliLog3_1 = new G4LogicalVolume(trackerSiliBigBox, silicon, "TrackerSili_Logical_3_1");  // tracker 3 is big & double -- 1st
    G4LogicalVolume* trackerAlLog = new G4LogicalVolume(trackerAlBox, aluminum, "TrackerAl_Logical");
    G4LogicalVolume* trackerAlBigLog = new G4LogicalVolume(trackerAlBigBox, aluminum, "TrackerAlBig_Logical");
    G4LogicalVolume* trackerAlSideFrontLog = new G4LogicalVolume(trackerAlSideFrontBox, aluminum, "TrackerAlSideFrontBox_Logical");
	G4LogicalVolume* trackerAlSideFrontBigLog = new G4LogicalVolume(trackerAlSideFrontBigBox, aluminum, "TrackerAlSideFrontBigBox_Logical");
	G4LogicalVolume* trackerAlSideBackLog = new G4LogicalVolume(trackerAlSideBackBox, aluminum, "TrackerAlBoxSideBack_Logical");
    trackerSiliLog0->SetVisAttributes(grey);
    trackerSiliLog1->SetVisAttributes(grey);
    trackerSiliLog2_0->SetVisAttributes(grey);
    trackerSiliLog2_1->SetVisAttributes(grey);
    trackerAlLog->SetVisAttributes(grey);
    trackerAlBigLog->SetVisAttributes(grey);
    trackerAlSideFrontLog->SetVisAttributes(grey);
    trackerAlSideFrontBigLog->SetVisAttributes(grey);
    trackerAlSideBackLog->SetVisAttributes(grey);
	
    G4double zTrackerAlFrontCentre0 = zTrackerSiliCentre0 - (2.25*cm - trackerAlThickness / 2);
    G4double zTrackerAlBackCentre0 = zTrackerSiliCentre0 + (2.25*cm - trackerAlThickness / 2);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerSiliCentre0), trackerSiliLog0, "TrackerSili_0", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre0), trackerAlLog, "TrackerSili_0_Al_Front", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre0), trackerAlLog, "TrackerSili_0_Al_Rear", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre0), trackerAlSideFrontLog, "TrackerSili_0_Al_Front", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre0), trackerAlSideBackLog, "TrackerSili_0_Al_Rear", worldLog, false, 0);
    G4double zTrackerAlFrontCentre1 = zTrackerSiliCentre1 - (2.25*cm - trackerAlThickness / 2);
    G4double zTrackerAlBackCentre1 = zTrackerSiliCentre1 + (2.25*cm - trackerAlThickness / 2);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerSiliCentre1), trackerSiliLog1, "TrackerSili_1", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre1), trackerAlLog, "TrackerSili_1_Al_Front", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre1), trackerAlLog, "TrackerSili_1_Al_Rear", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre1), trackerAlSideFrontLog, "TrackerSili_1_Al_Front", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre1), trackerAlSideBackLog, "TrackerSili_1_Al_Rear", worldLog, false, 0);
    G4double zTrackerAlFrontCentre2 = zTrackerSiliCentre2 - (2.4*cm - trackerAlThickness / 2);
    G4double zTrackerAlBackCentre2 = zTrackerSiliCentre2 + (2.4*cm - trackerAlThickness / 2);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerSiliCentre2-0.5*cm), trackerSiliLog2_0, "TrackerSili_2_0", worldLog, false, 0);  // tracker 2 sensors are separated by 1 cm
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerSiliCentre2+0.5*cm), trackerSiliLog2_1, "TrackerSili_2_1", worldLog, false, 0);  // tracker 2 sensors are separated by 1 cm
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre2), trackerAlBigLog, "TrackerSili_2_Al_Front", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre2), trackerAlBigLog, "TrackerSili_2_Al_Rear", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre2), trackerAlSideFrontBigLog, "TrackerSili_2_Al_Front", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre2), trackerAlSideBackLog, "TrackerSili_2_Al_Rear", worldLog, false, 0);
    G4double zTrackerAlFrontCentre3 = zTrackerSiliCentre3 - (2.4*cm - trackerAlThickness / 2);
    G4double zTrackerAlBackCentre3 = zTrackerSiliCentre3 + (2.4*cm - trackerAlThickness / 2);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerSiliCentre3-0.5*cm), trackerSiliLog3_0, "TrackerSili_3_0", worldLog, false, 0);  // tracker 3 sensors are separated by 1 cm
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerSiliCentre3+0.5*cm), trackerSiliLog3_1, "TrackerSili_3_1", worldLog, false, 0);  // tracker 3 sensors are separated by 1 cm
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre3), trackerAlBigLog, "TrackerSili_3_Al_Front", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre3), trackerAlBigLog, "TrackerSili_3_Al_Rear", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlFrontCentre3), trackerAlSideFrontBigLog, "TrackerSili_3_Al_Front", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zTrackerAlBackCentre3), trackerAlSideBackLog, "TrackerSili_3_Al_Rear", worldLog, false, 0);
	
    // photon omogeneous (BGO) calorimeter
    G4double caloPhThickness = 23 * cm;
    G4double caloPhWidthChannel = 2.1 * cm;
    G4double caloPhHeightChannel = 2.1 * cm;
	G4double caloPhGap = 1 * mm;
	G4double caloPhTransvDelta = caloPhHeightChannel + caloPhGap;
    G4VSolid* caloPhChannelBox = new G4Box("PhCalTest_Solid", caloPhWidthChannel / 2, caloPhHeightChannel / 2, caloPhThickness / 2);
	
    G4LogicalVolume* caloPhLog_00 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_00_Logical");
    G4LogicalVolume* caloPhLog_01 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_01_Logical");
    G4LogicalVolume* caloPhLog_02 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_02_Logical");
    G4LogicalVolume* caloPhLog_10 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_10_Logical");
    G4LogicalVolume* caloPhLog_11 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_11_Logical"); // central channel
    G4LogicalVolume* caloPhLog_12 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_12_Logical");
    G4LogicalVolume* caloPhLog_20 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_20_Logical");
    G4LogicalVolume* caloPhLog_21 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_21_Logical");
    G4LogicalVolume* caloPhLog_22 = new G4LogicalVolume(caloPhChannelBox, bgo, "PhCalTest_22_Logical");
    caloPhLog_00->SetVisAttributes(white);
    caloPhLog_01->SetVisAttributes(white);
    caloPhLog_02->SetVisAttributes(white);
    caloPhLog_10->SetVisAttributes(white);
    caloPhLog_11->SetVisAttributes(white); // central channel
    caloPhLog_12->SetVisAttributes(white);
    caloPhLog_20->SetVisAttributes(white);
    caloPhLog_21->SetVisAttributes(white);
    caloPhLog_22->SetVisAttributes(white);
	
    new G4PVPlacement(nullptr, G4ThreeVector(-caloPhTransvDelta, -caloPhTransvDelta, zPhCaloFront + caloPhThickness / 2), caloPhLog_00, "PhCalTest_00", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, -caloPhTransvDelta, zPhCaloFront + caloPhThickness / 2), caloPhLog_01, "PhCalTest_01", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(caloPhTransvDelta, -caloPhTransvDelta, zPhCaloFront + caloPhThickness / 2), caloPhLog_02, "PhCalTest_02", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(-caloPhTransvDelta, 0, zPhCaloFront + caloPhThickness / 2), caloPhLog_10, "PhCalTest_10", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPhCaloFront + caloPhThickness / 2), caloPhLog_11, "PhCalTest_11", worldLog, false, 0); // central channel
    new G4PVPlacement(nullptr, G4ThreeVector(caloPhTransvDelta, 0, zPhCaloFront + caloPhThickness / 2), caloPhLog_12, "PhCalTest_12", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(-caloPhTransvDelta, caloPhTransvDelta, zPhCaloFront + caloPhThickness / 2), caloPhLog_20, "PhCalTest_20", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0, caloPhTransvDelta, zPhCaloFront + caloPhThickness / 2), caloPhLog_21, "PhCalTest_21", worldLog, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(caloPhTransvDelta, caloPhTransvDelta, zPhCaloFront + caloPhThickness / 2), caloPhLog_22, "PhCalTest_22", worldLog, false, 0);
    
    // electron sampling (scintillator & lead) calorimeter
    G4double caloThicknessLayerAbs = 1.5 * mm;
    G4double caloThicknessLayerSci = 4.0 * mm;
    G4double caloWidth = 11.5 * cm;
    G4double caloHeight = 11.5 * cm;
    G4VSolid* absBox = new G4Box("ECalTest_Abs_Solid", caloWidth / 2, caloHeight / 2, caloThicknessLayerAbs / 2);
    G4VSolid* sciBox = new G4Box("ECalTest_Sci_Solid", caloWidth / 2, caloHeight / 2, caloThicknessLayerSci / 2);

    G4LogicalVolume* absLog = new G4LogicalVolume(absBox, lead, "ECalTest_Abs_Logical");
    G4LogicalVolume* sciLog = new G4LogicalVolume(sciBox, plastic, "ECalTest_Sci_Logical");
    absLog->SetVisAttributes(brown);
    sciLog->SetVisAttributes(cyan);

    G4int numberOfLayersSci = 70;
    G4double minZSci = zECaloFront + caloThicknessLayerSci / 2;
    G4int numberOfLayersAbs = 69;
    G4double minZAbs = zECaloFront + caloThicknessLayerSci + caloThicknessLayerAbs / 2;
    for (int i = 0; i < numberOfLayersSci; i++)
    {
        G4ThreeVector scintillatorPosition = G4ThreeVector(xECaloShift, 0, minZSci + i*(caloThicknessLayerAbs+caloThicknessLayerSci));
        ostringstream sName; sName << "ECalTest_Sci_" << i;
        new G4PVPlacement(nullptr, scintillatorPosition, sciLog, sName.str(), worldLog, false, i);
    }
    for (int i = 0; i < numberOfLayersAbs; i++)
    {
        G4ThreeVector absorberPosition = G4ThreeVector(xECaloShift, 0, minZAbs + i*(caloThicknessLayerAbs+caloThicknessLayerSci));
        ostringstream aName; aName << "ECalTest_Abs_" << i;
        new G4PVPlacement(nullptr, absorberPosition, absLog, aName.str(), worldLog, false, i);
    }
	
	// calorimeter box
	G4double dxCalosBoxSmall = 55*cm;
	G4double dxCalosBoxBig = 100*cm;
	G4double dyCalosBox = 40*cm;
	G4double dzCalosBox = 1*mm;
	G4VSolid* calosBoxFrontSolid = new G4Box("calosBoxFront_Solid", dxCalosBoxSmall / 2, dyCalosBox / 2, dzCalosBox / 2);
	G4VSolid* calosBoxSideSolid = new G4Box("calosBoxSide_Solid", dxCalosBoxBig / 2, dyCalosBox / 2, dzCalosBox / 2);
	
	G4LogicalVolume* calosBoxFrontLog = new G4LogicalVolume(calosBoxFrontSolid, aluminum, "CalosBoxFront_Logical");
	G4LogicalVolume* calosBoxSideLog = new G4LogicalVolume(calosBoxSideSolid, aluminum, "CalosBoxSide_Logical");
	calosBoxFrontLog->SetVisAttributes(grey);
	calosBoxSideLog->SetVisAttributes(grey);
	
	new G4PVPlacement(nullptr, G4ThreeVector(xECaloShift/2, 10*cm, zECaloFront-5*cm), calosBoxFrontLog, "CalosBoxFront", worldLog, false, 0);
    G4RotationMatrix* rotationMatrixHorsa = new G4RotationMatrix();
    rotationMatrixHorsa->rotateY(90.*deg);
	new G4PVPlacement(rotationMatrixHorsa, G4ThreeVector(xECaloShift/2-dxCalosBoxSmall/2, 10*cm, zECaloFront-5*cm+dxCalosBoxBig/2), calosBoxSideLog, "CalosBoxSide", worldLog, false, 0);
	new G4PVPlacement(rotationMatrixHorsa, G4ThreeVector(xECaloShift/2+dxCalosBoxSmall/2, 10*cm, zECaloFront-5*cm+dxCalosBoxBig/2), calosBoxSideLog, "CalosBoxSide", worldLog, false, 0);
	
	// various scintillators
	G4double dxScintiCavoto = 20*cm;
	G4double dyScintiCavoto = 10*cm;
	G4double dzScintiCavoto = 1*cm;
	G4VSolid* scintiCavotoSolid = new G4Box("scintiCavoto_Solid", dxScintiCavoto / 2, dyScintiCavoto / 2, dzScintiCavoto / 2);
	
	G4LogicalVolume* scintiCavotoLog = new G4LogicalVolume(scintiCavotoSolid, plastic, "scintiCavoto_Logical");
	scintiCavotoLog->SetVisAttributes(cyan);
	
	new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zOutCounter), scintiCavotoLog, "OutCounter", worldLog, false, 0);
	new G4PVPlacement(nullptr, G4ThreeVector(-2*cm, 0, zVeto), scintiCavotoLog, "OutCounter", worldLog, false, 0);
	
	// beam pipes
	G4double pi = 3.1415926535;
	G4double pipeOuterRadius = 9*cm;
	G4double pipeInnerRadius = 8*cm;
    G4VSolid* pipe0Solid = new G4Tubs("Pipe0_Solid", pipeInnerRadius, pipeOuterRadius, dzPipe0/2, 0, 2*pi);
    G4VSolid* pipe1Solid = new G4Tubs("Pipe1_Solid", pipeInnerRadius, pipeOuterRadius, dzPipe1/2, 0, 2*pi);
	G4VSolid* pipe0VacuumSolid = new G4Tubs("Pipe0Vacuum_Solid", 0, pipeInnerRadius, (dzPipe0-2*cm)/2, 0, 2*pi);
	G4VSolid* pipe1VacuumSolid = new G4Tubs("Pipe1Vacuum_Solid", 0, pipeInnerRadius, (dzPipe1-2*cm)/2, 0, 2*pi);
	
    G4LogicalVolume* pipe0Log = new G4LogicalVolume(pipe0Solid, ss, "Pipe0_Logical");
    G4LogicalVolume* pipe1Log = new G4LogicalVolume(pipe1Solid, ss, "Pipe1_Logical");
    G4LogicalVolume* pipe0VacuumLog = new G4LogicalVolume(pipe0VacuumSolid, vacuum, "Pipe0Vacuum_Logical");
    G4LogicalVolume* pipe1VacuumLog = new G4LogicalVolume(pipe1VacuumSolid, vacuum, "Pipe1Vacuum_Logical");
    pipe0Log->SetVisAttributes(grey);
    pipe1Log->SetVisAttributes(grey);
    pipe0VacuumLog->SetVisAttributes(invisible);
    pipe1VacuumLog->SetVisAttributes(invisible);
	
	new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPipe0Front + dzPipe0/2), pipe0Log, "Pipe0", worldLog, false, 0);
	new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPipe1Front + dzPipe1/2), pipe1Log, "Pipe1", worldLog, false, 0);
	new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPipe0Front + dzPipe0/2), pipe0VacuumLog, "Pipe0Vacuum", worldLog, false, 0);
	new G4PVPlacement(nullptr, G4ThreeVector(0, 0, zPipe1Front + dzPipe1/2), pipe1VacuumLog, "Pipe1Vacuum", worldLog, false, 0);

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
    G4bool bField = true; // magnetic field switch
	
    // bending magnet field
    G4double magnField = bField ? 1.9 : 0.0 * tesla;
    G4UniformMagField* bendingField = new G4UniformMagField(G4ThreeVector(0., magnField, 0.));
    G4LogicalVolume* fieldLog = G4LogicalVolumeStore::GetInstance()->GetVolume("magnetField_Logical");
    G4FieldManager* fieldManager = new G4FieldManager(bendingField);
    fieldLog->SetFieldManager(fieldManager, true);
	
    // SD -- photon calorimeter
    VolumeEDepSD* gammaCalSD_00 = new VolumeEDepSD("PhCalTest_00_SD");
    SetSensitiveDetector("PhCalTest_00_Logical", gammaCalSD_00);
    sdm->AddNewDetector(gammaCalSD_00);
    VolumeEDepSD* gammaCalSD_01 = new VolumeEDepSD("PhCalTest_01_SD");
    SetSensitiveDetector("PhCalTest_01_Logical", gammaCalSD_01);
    sdm->AddNewDetector(gammaCalSD_01);
    VolumeEDepSD* gammaCalSD_02 = new VolumeEDepSD("PhCalTest_02_SD");
    SetSensitiveDetector("PhCalTest_02_Logical", gammaCalSD_02);
    sdm->AddNewDetector(gammaCalSD_02);
    VolumeEDepSD* gammaCalSD_10 = new VolumeEDepSD("PhCalTest_10_SD");
    SetSensitiveDetector("PhCalTest_10_Logical", gammaCalSD_10);
    sdm->AddNewDetector(gammaCalSD_10);
    VolumeEDepSD* gammaCalSD_11 = new VolumeEDepSD("PhCalTest_11_SD");
    SetSensitiveDetector("PhCalTest_11_Logical", gammaCalSD_11);
    sdm->AddNewDetector(gammaCalSD_11);
    VolumeEDepSD* gammaCalSD_12 = new VolumeEDepSD("PhCalTest_12_SD");
    SetSensitiveDetector("PhCalTest_12_Logical", gammaCalSD_12);
    sdm->AddNewDetector(gammaCalSD_12);
    VolumeEDepSD* gammaCalSD_20 = new VolumeEDepSD("PhCalTest_20_SD");
    SetSensitiveDetector("PhCalTest_20_Logical", gammaCalSD_20);
    sdm->AddNewDetector(gammaCalSD_20);
    VolumeEDepSD* gammaCalSD_21 = new VolumeEDepSD("PhCalTest_21_SD");
    SetSensitiveDetector("PhCalTest_21_Logical", gammaCalSD_21);
    sdm->AddNewDetector(gammaCalSD_21);
    VolumeEDepSD* gammaCalSD_22 = new VolumeEDepSD("PhCalTest_22_SD");
    SetSensitiveDetector("PhCalTest_22_Logical", gammaCalSD_22);
    sdm->AddNewDetector(gammaCalSD_22);
    
    // SD -- crystal
    VolumeEDepSD* tgtSD_22 = new VolumeEDepSD("Tgt_SD");
    SetSensitiveDetector("Tgt_Logical", tgtSD_22);
    sdm->AddNewDetector(tgtSD_22);
	
    // SD -- tracking system (1 sensitive detector per tracking plane)
    VolumeTrackingSD* trackerSD0 = new VolumeTrackingSD("Tracker_SD_0");
    SetSensitiveDetector("TrackerSili_Logical_0", trackerSD0);
    sdm->AddNewDetector(trackerSD0);
    VolumeTrackingSD* trackerSD1 = new VolumeTrackingSD("Tracker_SD_1");
    SetSensitiveDetector("TrackerSili_Logical_1", trackerSD1);
    sdm->AddNewDetector(trackerSD1);
    VolumeTrackingSD* trackerSD2_0 = new VolumeTrackingSD("Tracker_SD_2_0");
    SetSensitiveDetector("TrackerSili_Logical_2_0", trackerSD2_0);
    sdm->AddNewDetector(trackerSD2_0);
    VolumeTrackingSD* trackerSD2_1 = new VolumeTrackingSD("Tracker_SD_2_1");
    SetSensitiveDetector("TrackerSili_Logical_2_1", trackerSD2_1);
    sdm->AddNewDetector(trackerSD2_1);
    VolumeTrackingSD* trackerSD3_0 = new VolumeTrackingSD("Tracker_SD_3_0");
    SetSensitiveDetector("TrackerSili_Logical_3_0", trackerSD3_0);
    sdm->AddNewDetector(trackerSD3_0);
    VolumeTrackingSD* trackerSD3_1 = new VolumeTrackingSD("Tracker_SD_3_1");
    SetSensitiveDetector("TrackerSili_Logical_3_1", trackerSD3_1);
    sdm->AddNewDetector(trackerSD3_1);
	
    // --------------------------------------------------
    // ...uncomment this line for the test sensitive detectors (implemented in include/TestMode.cc)
    // SDTest(sdm);
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
}

// ============================================================
// ============================================================
// implement custom methods here

