//MATTIA 2024: implemented custom commands to be executed from macros

#include <G4GenericMessenger.hh>	

class CustomConfigs{	

    public:

        CustomConfigs();
        ~CustomConfigs();
        static CustomConfigs* GetCustomConfigs();

        void Print();

        void SetBChanneling(){bChanneling0 = 1;}
        G4bool GetBChanneling(){return bChanneling0;}

    private:

        static CustomConfigs * fConf;
        G4GenericMessenger*	fMessenger;	

        G4bool bChanneling0;

};