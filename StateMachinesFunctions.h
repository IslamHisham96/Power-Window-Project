#ifndef StateMachinesFunctions
#define StateMachinesFunctions
void safeSM (int, int); 
void driverNeutralSM (int, int);
void passengerDownSM (int, int);
void passengerUpSM (int, int);
void driverDownSM (int, int);
void driverUpSM (int, int);
void passengerneutralSM (int, int);
void iniPassengerDownSM (int, int);
void manualPassengerDownSM (int, int);
void iniPassengerUpSM (int, int);
void manualPassengerUpSM (int, int);
void iniDriverDownSM (int, int);
void manualDriverDownSM (int, int);
void iniDriverUpSM (int, int);
void manualDriverUpSM (int, int);
void passengerNeutralSM(int, int);
void emergencyDownSM (int, int);
void autoDriverUpSM(int event, int depth);
void autoDriverDownSM(int event, int depth);
void autoPassengerUpSM(int event, int depth);
void autoPassengerDownSM(int event, int depth);
void dyingSM(int event, int depth);
void deadSM(int event, int depth);
#endif
