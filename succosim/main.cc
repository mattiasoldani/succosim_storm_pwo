#ifdef G4MULTITHREADED
    #include <G4MTRunManager.hh>
    using RunManager = G4MTRunManager;
#else
    #include <G4RunManager.hh>
    using RunManager = G4RunManager;
#endif

#ifdef G4VIS_USE
    #include <G4VisExecutive.hh>
#endif
#ifdef G4UI_USE
    #include <G4UIExecutive.hh>
#endif

#include <G4String.hh>
#include <G4UImanager.hh>
#include <vector>

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// add new physics lists here
// e.g. #include <FTFP_BERT.hh>
#include <QGSP_BERT.hh>
#include "FTFP_BERT.hh"  // added to exploit modified Geant4
#include "EmStandardPhysics.hh"  // added to exploit modified Geant4
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "Analysis.hh"
#include "PhysicsList.hh"
#include "TestMode.cc"

// MATTIA 2024: initialise the scoring manager
#include <G4ScoringManager.hh>

// MATTIA 2024: custom commands for macros
#include "CustomConfigs.hh"

using namespace std;

int main(int argc, char** argv)
{
    cout << "-----" << endl;
    cout << "| main.cc: let's start!" << endl;
    cout << "-----" << endl;

    // MATTIA 2024: load user interface manager here
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // MATTIA 2024: custom commands for macros - load manager
    CustomConfigs* conf = CustomConfigs::GetCustomConfigs();

    // find execution type (interactive or not) and detect all the macros given as executable arguments
    vector<G4String> macros;
    bool interactive = false;
    if  (argc == 1)
    {
        interactive = true;
    }
    else if (argc == 2) // MATTIA 2024: case added to handle pre-run initialisation
    {
        interactive = true;
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + argv[1]);

    }
    {
        for (int i = 1; i < argc; i++)
        {
            G4String arg = argv[i];
            if (arg == "-i" || arg == "--interactive")
            {
                interactive = true;
                G4String command = "/control/execute ";
                UImanager->ApplyCommand(command + argv[1]);
                // MATTIA 2024: added to handle pre-run initialisation
                continue;
            }
            else
            {
                // macros.push_back(arg); // MATTIA 2024: modified to handle pre-run initialisation
                if (i==1){
                    G4String command = "/control/execute ";
                    UImanager->ApplyCommand(command + argv[1]);
                }
                else{macros.push_back(arg);}
            }
        }
    }

    // MATTIA 2024: custom commands for macros - print parameters
    conf->Print();

	// load run manager
    auto runManager = new RunManager();
    runManager->SetVerboseLevel(0);  // <<< set run manager verbosity here

    // MATTIA 2024: initialise the scoring manager
    G4ScoringManager* scorer = G4ScoringManager::GetScoringManager();

    // if in graphical mode, start visualisation
    #ifdef G4VIS_USE
        G4VisManager* visManager = new G4VisExecutive("Quiet");  // <<< set visualisation manager verbosity here
        visManager->SetVerboseLevel(0);  // <<< set visualisation manager verbosity here
        visManager->Initialize();
    #endif

    // load physics list, detector construction and action initialisation
    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    // set physics list name here: (new ...()), set PhysicsList for the custom list
    // e.g. runManager->SetUserInitialization(new FTFP_BERT());
	// note: this whole part has been modified to exploit modified Geant4
	G4bool bChanneling = conf->GetBChanneling();  // enable/disable oriented crystal physics here // MATTIA 2024: tried to add external setting for channeling physics, but it was useless
	DetectorConstruction* modifiedDetectorConstruction = new DetectorConstruction();
	modifiedDetectorConstruction->SetChanneling(bChanneling);
	if(bChanneling){
		G4VModularPhysicsList* modifiedPhysicsList = new FTFP_BERT(0);
		modifiedPhysicsList->ReplacePhysics(new EmStandardPhysics());
		runManager->SetUserInitialization(modifiedPhysicsList);
	}else{
		runManager->SetUserInitialization(new QGSP_BERT());
	}
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    runManager->SetUserInitialization(modifiedDetectorConstruction);
	//runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new ActionInitialization());

    // load user interface manager and program executor
    #ifdef G4UI_USE
        G4UIExecutive* ui = nullptr;
        if (interactive)
        {
            ui = new G4UIExecutive(argc, argv);
        }
    #endif
    // G4UImanager* UImanager = G4UImanager::GetUIpointer();
    // MATTIA 2024: user interface manager loading moved above

    // execute all the macros given as executable arguments
    for (auto macro : macros)
    {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macro);
    }

    // if in interactive mode, open it; moreover, if in graphical mode, execute the default macro(s) for graphical mode startup
    #ifdef G4UI_USE
        if (interactive)
        {
            if (ui->IsGUI())
            {
                // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
                // select macro to be executed at graphical mode startup
                // e.g. UImanager->ApplyCommand("/control/execute macros/gui.mac");
                UImanager->ApplyCommand("/control/execute macros/gui.mac");
                // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            }
            else
            {
                UImanager->ApplyCommand("/run/initialize");
            }
            ui->SessionStart();
            delete ui;
        }
    #endif

    // at the end of the session, delete the run manager
    delete runManager;
	
    cout << "-----" << endl;
    cout << "| main.cc: done, goodbye!" << endl;
    cout << "-----" << endl;

    return 0;
}
