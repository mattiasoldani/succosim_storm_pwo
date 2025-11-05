#include <G4SystemOfUnits.hh>
#include <G4String.hh>

#include "RunAction.hh"
#include "Analysis.hh"

using namespace std;

// RunAction, class created at run start, with RunAction::EndOfRunAction executed at the end of the run

RunAction::RunAction() :  G4UserRunAction()
{
    // load the analysis manager for data output
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    analysis->SetVerboseLevel(1);  // set analysis manager verbosity here
  
    // create output ntuple
    analysis->SetFirstNtupleId(0);
    analysis->CreateNtuple("outData", "output data");
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // create the ntuple columns (remember the order, it is needed to fill them) here, or...
    // e.g. analysis->CreateNtupleDColumn("NEvent");
	
    analysis->CreateNtupleDColumn("NEvent");  // 0
    analysis->CreateNtupleDColumn("Tracker_NHit_X_0");  // 1
    analysis->CreateNtupleDColumn("Tracker_NHit_Y_0");  // 2 (this is gonna be identical to the previous -- same silicon layer)
    analysis->CreateNtupleDColumn("Tracker_NHit_X_1");  // 3
    analysis->CreateNtupleDColumn("Tracker_NHit_Y_1");  // 4 (this is gonna be identical to the previous -- same silicon layer)
    analysis->CreateNtupleDColumn("Tracker_NHit_X_2");  // 5
    analysis->CreateNtupleDColumn("Tracker_NHit_Y_2");  // 6
    analysis->CreateNtupleDColumn("Tracker_NHit_X_3");  // 7
    analysis->CreateNtupleDColumn("Tracker_NHit_Y_3");  // 8
    analysis->CreateNtupleDColumn("Tracker_X_0");  // 9
    analysis->CreateNtupleDColumn("Tracker_Y_0");  // 10
    analysis->CreateNtupleDColumn("Tracker_X_1");  // 11
    analysis->CreateNtupleDColumn("Tracker_Y_1");  // 12
    analysis->CreateNtupleDColumn("Tracker_X_2");  // 13
    analysis->CreateNtupleDColumn("Tracker_Y_2");  // 14
    analysis->CreateNtupleDColumn("Tracker_X_3");  // 15
    analysis->CreateNtupleDColumn("Tracker_Y_3");  // 16
    analysis->CreateNtupleDColumn("GammaCal_EDep_00");  // 17
    analysis->CreateNtupleDColumn("GammaCal_EDep_01");
    analysis->CreateNtupleDColumn("GammaCal_EDep_02");
    analysis->CreateNtupleDColumn("GammaCal_EDep_10");
    analysis->CreateNtupleDColumn("GammaCal_EDep_11");
    analysis->CreateNtupleDColumn("GammaCal_EDep_12");
    analysis->CreateNtupleDColumn("GammaCal_EDep_20");
    analysis->CreateNtupleDColumn("GammaCal_EDep_21");
    analysis->CreateNtupleDColumn("GammaCal_EDep_22"); // 25
    analysis->CreateNtupleDColumn("Crystal_EDep"); // 26
  
    // --------------------------------------------------
    // ...uncomment this line for the test ntuple columns (implemented in include/TestMode.cc)
    // OutputNtupleTest(analysis);
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    analysis->FinishNtuple(0);
	
    // open output file
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // choose output file name here --> file will have extension .root and and will be in ./out_data/
    G4String outFileName = "OutData";
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    analysis->OpenFile("./out_data/"+outFileName+".root");
}

RunAction::~RunAction()
{
    // write output file & close it
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    analysis->Write();
    analysis->CloseFile();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    // retrieve the number of events produced in the run
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    if (IsMaster())
    {
        G4cout << "-----" << G4endl;
        G4cout << "| RunAction.cc: end of run --> generated events: " << nofEvents << G4endl;
        G4cout << "-----" << G4endl;
    }
}
