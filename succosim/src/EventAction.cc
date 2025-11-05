#include <G4SystemOfUnits.hh>
#include <G4SDManager.hh>
#include <G4THitsMap.hh>
#include <G4Event.hh>

#include "EventAction.hh"
#include "Analysis.hh"
#include "CustomHit.hh"

using namespace std;

// EventAction::EndOfEventAction, executed at the end of each event
void EventAction::EndOfEventAction(const G4Event* event)
{
    // load the sensitive detector manager (set verbosity in DetectorConstruction.cc)
    G4SDManager* sdm = G4SDManager::GetSDMpointer();
	
    // load the analysis manager for data output (set verbosity in RunAction.cc)
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();

    // get the set of all the data collections for the current event
    G4HCofThisEvent* hcofEvent = event->GetHCofThisEvent();
    if(!hcofEvent) return;
	
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // implement cast of the data collections, operations on them and ntuple filling here, or... 
	
    // get data collections
    // G4int fPhCalEDepId_TL = sdm->GetCollectionID("PhCalTest_TL_SD/VolumeEDep");  // photon calorimeter
    // G4int fPhCalEDepId_TR = sdm->GetCollectionID("PhCalTest_TR_SD/VolumeEDep");  // photon calorimeter
    // G4int fPhCalEDepId_CL = sdm->GetCollectionID("PhCalTest_CL_SD/VolumeEDep");  // photon calorimeter
    // G4int fPhCalEDepId_CC = sdm->GetCollectionID("PhCalTest_CC_SD/VolumeEDep");  // photon calorimeter, central channel
    // G4int fPhCalEDepId_CR = sdm->GetCollectionID("PhCalTest_CR_SD/VolumeEDep");  // photon calorimeter
    // G4int fPhCalEDepId_BL = sdm->GetCollectionID("PhCalTest_BL_SD/VolumeEDep");  // photon calorimeter
    // G4int fPhCalEDepId_BR = sdm->GetCollectionID("PhCalTest_BR_SD/VolumeEDep");  // photon calorimeter
    G4int fTgtId = sdm->GetCollectionID("Tgt_SD/VolumeEDep");  // crystal
    // G4int fTgtSideId = sdm->GetCollectionID("TgtSide_SD/VolumeEDep");  // crystal, side
    // G4int fTrackerId0 = sdm->GetCollectionID("Tracker_SD_0/VolumeTracking");  // tracking module 0
    // G4int fTrackerId1 = sdm->GetCollectionID("Tracker_SD_1/VolumeTracking");  // tracking module 1
    // VolumeEDepHitsCollection* hitCollectionPhCal_TL = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_TL));
    // VolumeEDepHitsCollection* hitCollectionPhCal_TR = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_TR));
    // VolumeEDepHitsCollection* hitCollectionPhCal_CL = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_CL));
    // VolumeEDepHitsCollection* hitCollectionPhCal_CC = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_CC));
    // VolumeEDepHitsCollection* hitCollectionPhCal_CR = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_CR));
    // VolumeEDepHitsCollection* hitCollectionPhCal_BL = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_BL));
    // VolumeEDepHitsCollection* hitCollectionPhCal_BR = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_BR));
    VolumeEDepHitsCollection* hitCollectionTgt = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fTgtId));
    // VolumeEDepHitsCollection* hitCollectionTgtSide = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fTgtSideId));
    // VolumeTrackingHitsCollection* hitCollectionTracker0 = dynamic_cast<VolumeTrackingHitsCollection*>(hcofEvent->GetHC(fTrackerId0));
    // VolumeTrackingHitsCollection* hitCollectionTracker1 = dynamic_cast<VolumeTrackingHitsCollection*>(hcofEvent->GetHC(fTrackerId1));
	
    // event number to output ntuple
    // if ((hitCollectionTracker0) || (hitCollectionTracker1))
    {analysis->FillNtupleDColumn(0, 0, event->GetEventID());}
	
    // get tracking system data collection
    // 1 hit per particle, per step & per tracking plane
    // --> consider only hits whose energy deposit is over threshold
    // --> for hits belonging to the same particle, compute mean between all steps transverse positions
    // --> different particles are treated separately (and increase the hit counter)
    // G4double thresholdTrackerEDep = 50 * keV;
	
    // --> module 0
    // if (hitCollectionTracker0)
    // {
		//STORM22
		G4double eInit = event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy(); // MATTIA 2024: event initial energy is always scored
		
    //     G4int lastTrackId0 = -1;
    //     G4int NStep0 = 1;
    //     G4int NHits0 = 0;
    //     G4double horsa0 = -9999.0*cm;
    //     G4double versa0 = -9999.0*cm;
    //     for (auto hit: *hitCollectionTracker0->GetVector())
    //     {
    //         if (hit->GetEDep()>thresholdTrackerEDep)
    //         {
    //             if(hit->GetTrackId() != lastTrackId0)
    //             {
    //                 NHits0+=1;
    //                 NStep0=1;
    //                 horsa0=hit->GetX()[0];
    //                 versa0=hit->GetX()[1];
					
	//				//STORM22
	//				eInit=hit->GetEInit();
    //             }
    //             else
    //             {
    //                 NStep0+=1;
    //                 horsa0+=hit->GetX()[0];
    //                 versa0+=hit->GetX()[1];
					
	// 				//STORM22
	// 				eInit=hit->GetEInit();
    //             }
    //             lastTrackId0 = hit->GetTrackId();
    //         }
    //     }
    //     horsa0 = horsa0 / NStep0;
    //     versa0 = versa0 / NStep0;
    //     analysis->FillNtupleDColumn(0, 1, NHits0);
    //     analysis->FillNtupleDColumn(0, 2, NHits0);  // columns 1 & 2 are identical -- same silicon layer
    //     analysis->FillNtupleDColumn(0, 5, horsa0 / cm);
    //     analysis->FillNtupleDColumn(0, 6, versa0 / cm);
		
		//STORM22
		analysis->FillNtupleDColumn(0, 2, eInit / MeV); // MATTIA 2024: reshaping output root files
    // }
    // else
    // {
    //     analysis->FillNtupleDColumn(0, 1, 0);
    //     analysis->FillNtupleDColumn(0, 2, 0);
    //     analysis->FillNtupleDColumn(0, 5, -9999.0 / cm);
    //     analysis->FillNtupleDColumn(0, 6, -9999.0 / cm);
		
	// 	//STORM22
	// 	analysis->FillNtupleDColumn(0, 18, -9999.0 / MeV);
    // }
	
    // --> module 1
    // if (hitCollectionTracker1)
    // {
    //     G4int lastTrackId1 = -1;
    //     G4int NStep1 = 1;
    //     G4int NHits1 = 0;
    //     G4double horsa1 = -9999.0*cm;
    //     G4double versa1 = -9999.0*cm;
    //     for (auto hit: *hitCollectionTracker1->GetVector())
    //     {
    //         if (hit->GetEDep()>thresholdTrackerEDep)
    //         {
    //             if(hit->GetTrackId() != lastTrackId1)
    //                 {
    //                 NHits1+=1;
    //                 NStep1=1;
    //                 horsa1=hit->GetX()[0];
    //                 versa1=hit->GetX()[1];
    //             }
    //             else
    //             {
    //                 NStep1+=1;
    //                 horsa1+=hit->GetX()[0];
    //                 versa1+=hit->GetX()[1];
    //             }
    //             lastTrackId1 = hit->GetTrackId();
    //         }
    //     }
    //     horsa1 = horsa1 / NStep1;
    //     versa1 = versa1 / NStep1;
    //     analysis->FillNtupleDColumn(0, 3, NHits1);
    //     analysis->FillNtupleDColumn(0, 4, NHits1);  // columns 3 & 4 are identical -- same silicon layer
    //     analysis->FillNtupleDColumn(0, 7, horsa1 / cm);
    //     analysis->FillNtupleDColumn(0, 8, versa1 / cm);
    // }
    // else
    // {
    //     analysis->FillNtupleDColumn(0, 3, 0);
    //     analysis->FillNtupleDColumn(0, 4, 0);
    //     analysis->FillNtupleDColumn(0, 7, -9999.0 / cm);
    //     analysis->FillNtupleDColumn(0, 8, -9999.0 / cm);
    // }
	
    // get photon calorimeter data collection
    // 1 hit per particle & per step --> sum everything for the current event
	// G4int colPhCalEDep = 9;
    // if (hitCollectionPhCal_TL)
    // {
    //     G4double PhCalEDepTot_TL = 0.0;
    //     for (auto hit: *hitCollectionPhCal_TL->GetVector())
    //     {PhCalEDepTot_TL += hit->GetEDep();}
    //     analysis->FillNtupleDColumn(0, colPhCalEDep+0, PhCalEDepTot_TL / MeV);
    // }
    // else
    // {analysis->FillNtupleDColumn(0, colPhCalEDep+0, 0.0);}
    // if (hitCollectionPhCal_TR)
    // {
    //     G4double PhCalEDepTot_TR = 0.0;
    //     for (auto hit: *hitCollectionPhCal_TR->GetVector())
    //     {PhCalEDepTot_TR += hit->GetEDep();}
    //     analysis->FillNtupleDColumn(0, colPhCalEDep+1, PhCalEDepTot_TR / MeV);
    // }
    // else
    // {analysis->FillNtupleDColumn(0, colPhCalEDep+1, 0.0);}
    // if (hitCollectionPhCal_CL)
    // {
    //     G4double PhCalEDepTot_CL = 0.0;
    //     for (auto hit: *hitCollectionPhCal_CL->GetVector())
    //     {PhCalEDepTot_CL += hit->GetEDep();}
    //     analysis->FillNtupleDColumn(0, colPhCalEDep+2, PhCalEDepTot_CL / MeV);
    // }
    // else
    // {analysis->FillNtupleDColumn(0, colPhCalEDep+2, 0.0);}
    // if (hitCollectionPhCal_CC)
    // {
    //     G4double PhCalEDepTot_CC = 0.0;
    //     for (auto hit: *hitCollectionPhCal_CC->GetVector())
    //     {PhCalEDepTot_CC += hit->GetEDep();}
    //     analysis->FillNtupleDColumn(0, colPhCalEDep+3, PhCalEDepTot_CC / MeV);
    // }
    // else
    // {analysis->FillNtupleDColumn(0, colPhCalEDep+3, 0.0);}
    // if (hitCollectionPhCal_CR)
    // {
    //     G4double PhCalEDepTot_CR = 0.0;
    //     for (auto hit: *hitCollectionPhCal_CR->GetVector())
    //     {PhCalEDepTot_CR += hit->GetEDep();}
    //     analysis->FillNtupleDColumn(0, colPhCalEDep+4, PhCalEDepTot_CR / MeV);
    // }
    // else
    // {analysis->FillNtupleDColumn(0, colPhCalEDep+4, 0.0);}
    // if (hitCollectionPhCal_BL)
    // {
    //     G4double PhCalEDepTot_BL = 0.0;
    //     for (auto hit: *hitCollectionPhCal_BL->GetVector())
    //     {PhCalEDepTot_BL += hit->GetEDep();}
    //     analysis->FillNtupleDColumn(0, colPhCalEDep+5, PhCalEDepTot_BL / MeV);
    // }
    // else
    // {analysis->FillNtupleDColumn(0, colPhCalEDep+5, 0.0);}
	// if (hitCollectionPhCal_BR)
    // {
    //     G4double PhCalEDepTot_BR = 0.0;
    //     for (auto hit: *hitCollectionPhCal_BR->GetVector())
    //     {PhCalEDepTot_BR += hit->GetEDep();}
    //     analysis->FillNtupleDColumn(0, colPhCalEDep+6, PhCalEDepTot_BR / MeV);
    // }
    // else
    // {analysis->FillNtupleDColumn(0, colPhCalEDep+6, 0.0);}
    
    // get crystal data collection
    // 1 hit per particle & per step --> sum everything for the current event
	G4int colTgtEDep = 1; // MATTIA 2024: reshaping output root files
    if (hitCollectionTgt)
    {
        G4double PhTgtTot= 0.0;
        for (auto hit: *hitCollectionTgt->GetVector())
        {PhTgtTot += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colTgtEDep, PhTgtTot / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colTgtEDep, 0.0);}

    // get side crystal data collection
    // 1 hit per particle & per step --> sum everything for the current event
	// G4int colTgtSideEDep = 17;
    // if (hitCollectionTgtSide)
    // {
    //     G4double PhTgtSideTot= 0.0;
    //     for (auto hit: *hitCollectionTgtSide->GetVector())
    //     {PhTgtSideTot += hit->GetEDep();}
    //     analysis->FillNtupleDColumn(0, colTgtSideEDep, PhTgtSideTot / MeV);
    // }
    // else
    // {analysis->FillNtupleDColumn(0, colTgtSideEDep, 0.0);}

    // add event to ntuple
    analysis->AddNtupleRow(0);

    // --------------------------------------------------
    // ...uncomment this line for the test event action (implemented in include/TestMode.cc)
    // EndOfEventScoringTest(event, sdm, hcofEvent, analysis);
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
}
