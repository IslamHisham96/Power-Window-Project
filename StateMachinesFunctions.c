#include "StateNames.h"
#include "inputs.h"
#include "semphr.h"
#include "Timers.h"
#include "LCD_State.h"

extern SemaphoreHandle_t xTurnRightSemaphore;
extern SemaphoreHandle_t xTurnLeftSemaphore;
extern SemaphoreHandle_t xFastStopSemaphore;


int stateDepth = 3;
int state[6] = {safe, driverNeutral, passengerNeutral,0,0,0};

typedef void (*StateFunction)(int, int);

extern StateFunction stateMachines[21];

void autoDriverUpSM(int event, int depth){}
void autoDriverDownSM(int event, int depth){}
void autoPassengerUpSM(int event, int depth){}
void autoPassengerDownSM(int event, int depth){}


	
void dyingSM(int event, int depth){

	switch(event){
	
		case ENGINE_TIMER_TICK_EVENT:
			state[stateDepth - 1] = dead;
			displayString("dead");
			break;

	}

}
void deadSM(int event, int depth){

	switch(event){
	
		case ENGINE_EVENT:
			displayString("passengerNeutral");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;

	}

}
	
void iniDriverDownSM (int event, int depth){

	switch(event){
	
		case AUTO_TIMER_TICK_EVENT:
			displayString("manualDriverDown");
			state[stateDepth - 1] = manualDriverDown;
			break;
		case DRIVER_NEUTRAL_EVENT:
			displayString("autoDriverDown");
			state[stateDepth - 1] = autoDriverDown;
			break;

	}

}
void manualDriverDownSM (int event, int depth){
	
	switch(event){

		case DRIVER_NEUTRAL_EVENT:
			xSemaphoreGive(xFastStopSemaphore);
			displayString("passengerNeutral");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;

	}

}
void iniDriverUpSM (int event, int depth){

	switch(event){
	
		case AUTO_TIMER_TICK_EVENT:
			displayString("manualDriverUp");
			state[stateDepth - 1] = manualDriverUp;
			break;
		case DRIVER_NEUTRAL_EVENT:
			displayString("autoDriverUp");
			state[stateDepth - 1] = autoDriverUp;
			break;

	}

}
void manualDriverUpSM (int event, int depth){
	
	switch(event){

		case DRIVER_NEUTRAL_EVENT:
			xSemaphoreGive(xFastStopSemaphore);
			displayString("passengerNeutral");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;
	}

}
void driverDownSM (int event, int depth){
	
	switch(event){
	
		case DRIVER_UP_EVENT:
			xSemaphoreGive(xTurnRightSemaphore);
			displayString("iniDriverUp");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverUp;
			state[stateDepth++] = iniDriverUp;
			//start timer
			enableAutoTimer();
			break;
		case LIMIT_DOWN_EVENT:
			xSemaphoreGive(xFastStopSemaphore);
			displayString("passengerNeutral");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void driverUpSM (int event, int depth){
	
	switch(event){
	
		case DRIVER_DOWN_EVENT:
			xSemaphoreGive(xTurnLeftSemaphore);
			displayString("iniDriverDown");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverDown;
			state[stateDepth++] = iniDriverDown;
			//start timer
			enableAutoTimer();		
			break;
		case LIMIT_UP_EVENT:
			xSemaphoreGive(xFastStopSemaphore);
			displayString("passengerNeutral");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;
		case OBSTACLE_EVENT:
			xSemaphoreGive(xTurnLeftSemaphore);
			displayString("emergencyDown");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = emergencyDown;
			//start timer
			enableObstacleTimer();
			break;
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void safeSM(int event, int depth){

	
	switch(event){
	
		case ENGINE_EVENT:
			displayString("dying");
			stateDepth = 0;
			state[stateDepth++] = dying;
			//start timer
			enableEngineTimer();
			break;
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void driverNeutralSM(int event, int depth){

	
	switch(event){
	
		case DRIVER_UP_EVENT:
				xSemaphoreGive(xTurnRightSemaphore);
				displayString("iniDriverUp");
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverUp;
				state[stateDepth++] = iniDriverUp;
				//start timer
				enableAutoTimer();
				break;
		case DRIVER_DOWN_EVENT:
				xSemaphoreGive(xTurnLeftSemaphore);
				displayString("iniDriverDown");
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverDown;
				state[stateDepth++] = iniDriverDown;
				//start timer
				enableAutoTimer();
				break;
		default:
				if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void passengerNeutralSM(int event, int depth){

	
	switch(event){
	
		case PASSENGER_UP_EVENT:
				xSemaphoreGive(xTurnRightSemaphore);		
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerUp;
				state[stateDepth++] = iniPassengerUp;
				displayString("iniPassengerUp");
				//start timer
				enableAutoTimer();
				break;
		case PASSENGER_DOWN_EVENT:
				xSemaphoreGive(xTurnLeftSemaphore);
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerDown;
				state[stateDepth++] = iniPassengerDown;
				displayString("iniPassengerDown");
				//start timer
				enableAutoTimer();
				break;

	}

}

void passengerUpSM(int event, int depth){

	
	switch(event){
		
		case PASSENGER_DOWN_EVENT:
				xSemaphoreGive(xTurnLeftSemaphore);
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerDown;
				state[stateDepth++] = iniPassengerDown;
				//start timer
				enableAutoTimer();
				break;
		case LIMIT_UP_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerNeutral;
				displayString("passengerNeutral");
				break;
		case OBSTACLE_EVENT:
				xSemaphoreGive(xTurnLeftSemaphore);
				displayString("emergencyDown");
				stateDepth = 2;
				state[stateDepth-1] = emergencyDown;
				//start timer
				enableObstacleTimer();
				break;

		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void iniPassengerUpSM(int event, int depth){

	
	switch(event){
	
		case AUTO_TIMER_TICK_EVENT:
				displayString("manualPassengerUp");
			  state[depth] = manualPassengerUp;
				break;
		case PASSENGER_NEUTRAL_EVENT:
				displayString("autoPassengerUp");
				state[depth] = autoPassengerUp;
				break;

	}

}

void manualPassengerUpSM(int event, int depth){

	switch(event){

		case PASSENGER_NEUTRAL_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				displayString("passengerNeutral");
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerNeutral;
				break;
	
	}

}

void passengerDownSM(int event, int depth){

	switch(event){

		case PASSENGER_UP_EVENT:
				xSemaphoreGive(xTurnRightSemaphore);
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerUp;
				state[stateDepth++] = iniPassengerUp;
				//start timer
				enableAutoTimer();
				break;
		
		case LIMIT_DOWN_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerNeutral;
				displayString("passengerNeutral");
				break;

		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void iniPassengerDownSM(int event, int depth){

	switch(event){
	
		case AUTO_TIMER_TICK_EVENT:
			 displayString("manualPassengerDown");
			 state[depth] = manualPassengerDown;
				break;
		case PASSENGER_NEUTRAL_EVENT:
				displayString("autoPassengerDown");
				state[depth] = autoPassengerDown;
				break;
	}

}

void manualPassengerDownSM(int event, int depth){

	switch(event){
	

		case PASSENGER_NEUTRAL_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				displayString("passengerNeutral");
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerNeutral;
				break;
			
	}

}


void emergencyDownSM(int event, int depth){
	
	switch(event){
	
		case OBSTACLE_TIMER_TICK_EVENT:
		case LIMIT_DOWN_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				displayString("passengerNeutral");
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerNeutral;
				break;

	}
	
}

