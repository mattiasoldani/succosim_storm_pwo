//MATTIA 2024: implemented custom commands to be executed from macros

#include "CustomConfigs.hh"

using namespace std;

CustomConfigs::CustomConfigs(){

    fMessenger = new G4GenericMessenger(this, "/custom/");
    fMessenger->SetGuidance("Custom commands for macros.");
    fMessenger->DeclareMethod("setBChanneling", &CustomConfigs::SetBChanneling)
        .SetStates(G4State_PreInit, G4State_Idle)
        .SetGuidance("Set bChanneling.");

}

CustomConfigs::~CustomConfigs(){
    delete fMessenger;
    delete fConf;
}

CustomConfigs* CustomConfigs::fConf = 0;
CustomConfigs* CustomConfigs::GetCustomConfigs(){
  if(!fConf){fConf = new CustomConfigs;}
  return fConf;
}

void CustomConfigs::Print(){
    cout << "-----" << endl;
    cout << "| custom settings:" << endl;
    cout << "| - is channeling enabled? " << this->GetBChanneling() << endl;
    cout << "-----" << endl;
}