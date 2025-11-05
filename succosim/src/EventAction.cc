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
    G4int fPhCalEDepId_00 = sdm->GetCollectionID("PhCalTest_00_SD/VolumeEDep");  // photon calorimeter
    G4int fPhCalEDepId_01 = sdm->GetCollectionID("PhCalTest_01_SD/VolumeEDep");  // photon calorimeter
    G4int fPhCalEDepId_02 = sdm->GetCollectionID("PhCalTest_02_SD/VolumeEDep");  // photon calorimeter
    G4int fPhCalEDepId_10 = sdm->GetCollectionID("PhCalTest_10_SD/VolumeEDep");  // photon calorimeter
    G4int fPhCalEDepId_11 = sdm->GetCollectionID("PhCalTest_11_SD/VolumeEDep");  // photon calorimeter, central channel
    G4int fPhCalEDepId_12 = sdm->GetCollectionID("PhCalTest_12_SD/VolumeEDep");  // photon calorimeter
    G4int fPhCalEDepId_20 = sdm->GetCollectionID("PhCalTest_20_SD/VolumeEDep");  // photon calorimeter
    G4int fPhCalEDepId_21 = sdm->GetCollectionID("PhCalTest_21_SD/VolumeEDep");  // photon calorimeter
    G4int fPhCalEDepId_22 = sdm->GetCollectionID("PhCalTest_22_SD/VolumeEDep");  // photon calorimeter
    G4int fTgtId = sdm->GetCollectionID("Tgt_SD/VolumeEDep");  // crystal
    G4int fTrackerId0 = sdm->GetCollectionID("Tracker_SD_0/VolumeTracking");  // tracking module 0
    G4int fTrackerId1 = sdm->GetCollectionID("Tracker_SD_1/VolumeTracking");  // tracking module 1
    G4int fTrackerId2_0 = sdm->GetCollectionID("Tracker_SD_2_0/VolumeTracking");  // tracking module 2 -- 0th
    G4int fTrackerId2_1 = sdm->GetCollectionID("Tracker_SD_2_1/VolumeTracking");  // tracking module 2 -- 1st
    G4int fTrackerId3_0 = sdm->GetCollectionID("Tracker_SD_3_0/VolumeTracking");  // tracking module 3 -- 0th
    G4int fTrackerId3_1 = sdm->GetCollectionID("Tracker_SD_3_1/VolumeTracking");  // tracking module 3 -- 1st
    VolumeEDepHitsCollection* hitCollectionPhCal_00 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_00));
    VolumeEDepHitsCollection* hitCollectionPhCal_01 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_01));
    VolumeEDepHitsCollection* hitCollectionPhCal_02 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_02));
    VolumeEDepHitsCollection* hitCollectionPhCal_10 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_10));
    VolumeEDepHitsCollection* hitCollectionPhCal_11 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_11));
    VolumeEDepHitsCollection* hitCollectionPhCal_12 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_12));
    VolumeEDepHitsCollection* hitCollectionPhCal_20 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_20));
    VolumeEDepHitsCollection* hitCollectionPhCal_21 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_21));
    VolumeEDepHitsCollection* hitCollectionPhCal_22 = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fPhCalEDepId_22));
    VolumeEDepHitsCollection* hitCollectionTgt = dynamic_cast<VolumeEDepHitsCollection*>(hcofEvent->GetHC(fTgtId));
    VolumeTrackingHitsCollection* hitCollectionTracker0 = dynamic_cast<VolumeTrackingHitsCollection*>(hcofEvent->GetHC(fTrackerId0));
    VolumeTrackingHitsCollection* hitCollectionTracker1 = dynamic_cast<VolumeTrackingHitsCollection*>(hcofEvent->GetHC(fTrackerId1));
    VolumeTrackingHitsCollection* hitCollectionTracker2_0 = dynamic_cast<VolumeTrackingHitsCollection*>(hcofEvent->GetHC(fTrackerId2_0));
    VolumeTrackingHitsCollection* hitCollectionTracker2_1 = dynamic_cast<VolumeTrackingHitsCollection*>(hcofEvent->GetHC(fTrackerId2_1));
    VolumeTrackingHitsCollection* hitCollectionTracker3_0 = dynamic_cast<VolumeTrackingHitsCollection*>(hcofEvent->GetHC(fTrackerId3_0));
    VolumeTrackingHitsCollection* hitCollectionTracker3_1 = dynamic_cast<VolumeTrackingHitsCollection*>(hcofEvent->GetHC(fTrackerId3_1));
	
    // event number to output ntuple
    if ((hitCollectionTracker0) || (hitCollectionTracker1))
    {analysis->FillNtupleDColumn(0, 0, event->GetEventID());}
	
    // get tracking system data collection
    // 1 hit per particle, per step & per tracking plane
    // --> consider only hits whose energy deposit is over threshold
    // --> for hits belonging to the same particle, compute mean between all steps transverse positions
    // --> different particles are treated separately (and increase the hit counter)
    G4double thresholdTrackerEDep = 50 * keV;
	
    // --> module 0
    if (hitCollectionTracker0)
    {
        G4int lastTrackId0 = -1;
        G4int NStep0 = 1;
        G4int NHits0 = 0;
        G4double horsa0 = -9999.0*cm;
        G4double versa0 = -9999.0*cm;
        for (auto hit: *hitCollectionTracker0->GetVector())
        {
            if (hit->GetEDep()>thresholdTrackerEDep)
            {
                if(hit->GetTrackId() != lastTrackId0)
                {
                    NHits0+=1;
                    NStep0=1;
                    horsa0=hit->GetX()[0];
                    versa0=hit->GetX()[1];
                }
                else
                {
                    NStep0+=1;
                    horsa0+=hit->GetX()[0];
                    versa0+=hit->GetX()[1];
                }
                lastTrackId0 = hit->GetTrackId();
            }
        }
        horsa0 = horsa0 / NStep0;
        versa0 = versa0 / NStep0;
        analysis->FillNtupleDColumn(0, 1, NHits0);
        analysis->FillNtupleDColumn(0, 2, NHits0);  // columns 1 & 2 are identical -- same silicon layer
        analysis->FillNtupleDColumn(0, 9, horsa0 / cm);
        analysis->FillNtupleDColumn(0, 10, versa0 / cm);
    }
    else
    {
        analysis->FillNtupleDColumn(0, 1, 0);
        analysis->FillNtupleDColumn(0, 2, 0);
        analysis->FillNtupleDColumn(0, 9, -9999.0 / cm);
        analysis->FillNtupleDColumn(0, 10, -9999.0 / cm);
    }
	
    // --> module 1
    if (hitCollectionTracker1)
    {
        G4int lastTrackId1 = -1;
        G4int NStep1 = 1;
        G4int NHits1 = 0;
        G4double horsa1 = -9999.0*cm;
        G4double versa1 = -9999.0*cm;
        for (auto hit: *hitCollectionTracker1->GetVector())
        {
            if (hit->GetEDep()>thresholdTrackerEDep)
            {
                if(hit->GetTrackId() != lastTrackId1)
                    {
                    NHits1+=1;
                    NStep1=1;
                    horsa1=hit->GetX()[0];
                    versa1=hit->GetX()[1];
                }
                else
                {
                    NStep1+=1;
                    horsa1+=hit->GetX()[0];
                    versa1+=hit->GetX()[1];
                }
                lastTrackId1 = hit->GetTrackId();
            }
        }
        horsa1 = horsa1 / NStep1;
        versa1 = versa1 / NStep1;
        analysis->FillNtupleDColumn(0, 3, NHits1);
        analysis->FillNtupleDColumn(0, 4, NHits1);  // columns 3 & 4 are identical -- same silicon layer
        analysis->FillNtupleDColumn(0, 11, horsa1 / cm);
        analysis->FillNtupleDColumn(0, 12, versa1 / cm);
    }
    else
    {
        analysis->FillNtupleDColumn(0, 3, 0);
        analysis->FillNtupleDColumn(0, 4, 0);
        analysis->FillNtupleDColumn(0, 11, -9999.0 / cm);
        analysis->FillNtupleDColumn(0, 12, -9999.0 / cm);
    }
	
    // --> module 2 -- 0th
    if (hitCollectionTracker2_0)
    {
        G4int lastTrackId2_0 = -1;
        G4int NStep2_0 = 1;
        G4int NHits2_0 = 0;
        G4double horsa2 = -9999.0*cm;
        for (auto hit: *hitCollectionTracker2_0->GetVector())
        {
            if (hit->GetEDep()>thresholdTrackerEDep)
            {
                if(hit->GetTrackId() != lastTrackId2_0)
                {
                    NHits2_0+=1;
                    NStep2_0=1;
                    horsa2=hit->GetX()[0];
                }
                else
                {
                    NStep2_0+=1;
                    horsa2+=hit->GetX()[0];
                }
                lastTrackId2_0 = hit->GetTrackId();
            }
        }
        horsa2 = horsa2 / NStep2_0;
        analysis->FillNtupleDColumn(0, 5, NHits2_0);
        analysis->FillNtupleDColumn(0, 13, horsa2 / cm);
    }
    else
    {
        analysis->FillNtupleDColumn(0, 5, 0);
        analysis->FillNtupleDColumn(0, 13, -9999.0 / cm);
    }
	
    // --> module 2 -- 1st
    if (hitCollectionTracker2_1)
    {
        G4int lastTrackId2_1 = -1;
        G4int NStep2_1 = 1;
        G4int NHits2_1 = 0;
        G4double versa2 = -9999.0*cm;
        for (auto hit: *hitCollectionTracker2_1->GetVector())
        {
            if (hit->GetEDep()>thresholdTrackerEDep)
            {
                if(hit->GetTrackId() != lastTrackId2_1)
                {
                    NHits2_1+=1;
                    NStep2_1=1;
                    versa2=hit->GetX()[1];
                }
                else
                {
                    NStep2_1+=1;
                    versa2+=hit->GetX()[1];
                }
                lastTrackId2_1 = hit->GetTrackId();
            }
        }
        versa2 = versa2 / NStep2_1;
        analysis->FillNtupleDColumn(0, 6, NHits2_1);
        analysis->FillNtupleDColumn(0, 14, versa2 / cm);
    }
    else
    {
        analysis->FillNtupleDColumn(0, 6, 0);
        analysis->FillNtupleDColumn(0, 14, -9999.0 / cm);
    }
	
    // --> module 3 -- 0th
    if (hitCollectionTracker3_0)
    {
        G4int lastTrackId3_0 = -1;
        G4int NStep3_0 = 1;
        G4int NHits3_0 = 0;
        G4double horsa3 = -9999.0*cm;
        for (auto hit: *hitCollectionTracker3_0->GetVector())
        {
            if (hit->GetEDep()>thresholdTrackerEDep)
            {
                if(hit->GetTrackId() != lastTrackId3_0)
                {
                    NHits3_0+=1;
                    NStep3_0=1;
                    horsa3=hit->GetX()[0];
                }
                else
                {
                    NStep3_0+=1;
                    horsa3+=hit->GetX()[0];
                }
                lastTrackId3_0 = hit->GetTrackId();
            }
        }
        horsa3 = horsa3 / NStep3_0;
        analysis->FillNtupleDColumn(0, 7, NHits3_0);
        analysis->FillNtupleDColumn(0, 15, horsa3 / cm);
    }
    else
    {
        analysis->FillNtupleDColumn(0, 7, 0);
        analysis->FillNtupleDColumn(0, 15, -9999.0 / cm);
    }
	
    // --> module 3 -- 1st
    if (hitCollectionTracker3_1)
    {
        G4int lastTrackId3_1 = -1;
        G4int NStep3_1 = 1;
        G4int NHits3_1 = 0;
        G4double versa3 = -9999.0*cm;
        for (auto hit: *hitCollectionTracker3_1->GetVector())
        {
            if (hit->GetEDep()>thresholdTrackerEDep)
            {
                if(hit->GetTrackId() != lastTrackId3_1)
                {
                    NHits3_1+=1;
                    NStep3_1=1;
                    versa3=hit->GetX()[1];
                }
                else
                {
                    NStep3_1+=1;
                    versa3+=hit->GetX()[1];
                }
                lastTrackId3_1 = hit->GetTrackId();
            }
        }
        versa3 = versa3 / NStep3_1;
        analysis->FillNtupleDColumn(0, 8, NHits3_1);
        analysis->FillNtupleDColumn(0, 16, versa3 / cm);
    }
    else
    {
        analysis->FillNtupleDColumn(0, 8, 0);
        analysis->FillNtupleDColumn(0, 16, -9999.0 / cm);
    }
	
    // get photon calorimeter data collection
    // 1 hit per particle & per step --> sum everything for the current event
	G4int colPhCalEDep = 17;
    if (hitCollectionPhCal_00)
    {
        G4double PhCalEDepTot_00 = 0.0;
        for (auto hit: *hitCollectionPhCal_00->GetVector())
        {PhCalEDepTot_00 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+0, PhCalEDepTot_00 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+0, 0.0);}
    if (hitCollectionPhCal_01)
    {
        G4double PhCalEDepTot_01 = 0.0;
        for (auto hit: *hitCollectionPhCal_01->GetVector())
        {PhCalEDepTot_01 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+1, PhCalEDepTot_01 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+1, 0.0);}
    if (hitCollectionPhCal_02)
    {
        G4double PhCalEDepTot_02 = 0.0;
        for (auto hit: *hitCollectionPhCal_02->GetVector())
        {PhCalEDepTot_02 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+2, PhCalEDepTot_02 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+2, 0.0);}
    if (hitCollectionPhCal_10)
    {
        G4double PhCalEDepTot_10 = 0.0;
        for (auto hit: *hitCollectionPhCal_10->GetVector())
        {PhCalEDepTot_10 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+3, PhCalEDepTot_10 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+3, 0.0);}
    if (hitCollectionPhCal_11)
    {
        G4double PhCalEDepTot_11 = 0.0;
        for (auto hit: *hitCollectionPhCal_11->GetVector())
        {PhCalEDepTot_11 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+4, PhCalEDepTot_11 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+4, 0.0);}
    if (hitCollectionPhCal_12)
    {
        G4double PhCalEDepTot_12 = 0.0;
        for (auto hit: *hitCollectionPhCal_12->GetVector())
        {PhCalEDepTot_12 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+5, PhCalEDepTot_12 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+5, 0.0);}
	if (hitCollectionPhCal_20)
    {
        G4double PhCalEDepTot_20 = 0.0;
        for (auto hit: *hitCollectionPhCal_20->GetVector())
        {PhCalEDepTot_20 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+6, PhCalEDepTot_20 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+6, 0.0);}
    if (hitCollectionPhCal_21)
    {
        G4double PhCalEDepTot_21 = 0.0;
        for (auto hit: *hitCollectionPhCal_21->GetVector())
        {PhCalEDepTot_21 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+7, PhCalEDepTot_21 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+7, 0.0);}
    if (hitCollectionPhCal_22)
    {
        G4double PhCalEDepTot_22 = 0.0;
        for (auto hit: *hitCollectionPhCal_22->GetVector())
        {PhCalEDepTot_22 += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colPhCalEDep+8, PhCalEDepTot_22 / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colPhCalEDep+8, 0.0);}
    
    // get crystal data collection
    // 1 hit per particle & per step --> sum everything for the current event
	G4int colTgtEDep = 26;
    if (hitCollectionTgt)
    {
        G4double PhTgtTot= 0.0;
        for (auto hit: *hitCollectionTgt->GetVector())
        {PhTgtTot += hit->GetEDep();}
        analysis->FillNtupleDColumn(0, colTgtEDep, PhTgtTot / MeV);
    }
    else
    {analysis->FillNtupleDColumn(0, colTgtEDep, 0.0);}

    // add event to ntuple
    analysis->AddNtupleRow(0);

    // --------------------------------------------------
    // ...uncomment this line for the test event action (implemented in include/TestMode.cc)
    // EndOfEventScoringTest(event, sdm, hcofEvent, analysis);
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
}
