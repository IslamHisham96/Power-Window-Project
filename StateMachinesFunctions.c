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

extern StateFunction stateMachines[22];

void autoDriverUpSM(int event, int depth){}
void autoDriverDownSM(int event, int depth){}
void autoPassengerUpSM(int event, int depth){}
void autoPassengerDownSM(int event, int depth){}


	
void dyingSM(int event, int depth){

	switch(event){
	
		case TIMER_TICK_EVENT:
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
			displayString("manualDriverUp");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverUp;
			state[stateDepth++] = manualDriverUp;
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
			displayString("manualDriverDown");
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = driverDown;
			state[stateDepth++] = manualDriverDown;
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
			//start timer
			stateDepth = 0;
			state[stateDepth++] = safe;
			state[stateDepth++] = emergencyDown;
			break;
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void safeSM(int event, int depth){

	
	switch(event){
	
		case ENGINE_EVENT:
			displayString("dying");
		//start timer
			stateDepth = 0;
			state[stateDepth++] = dying;
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
				//start timer
				enableAutoTimer();
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverUp;
				state[stateDepth++] = iniDriverUp;
				break;
		case DRIVER_DOWN_EVENT:
				xSemaphoreGive(xTurnLeftSemaphore);
				displayString("iniDriverDown");
				//start timer
				enableAutoTimer();
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverDown;
				state[stateDepth++] = iniDriverDown;
				break;
		default:
				if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void passengerNeutralSM(int event, int depth){

	
	switch(event){
	
		case PASSENGER_UP_EVENT:
				state[depth] = passengerUp;
				state[depth + 1] = iniPassengerUp;
				stateDepth++;
				xSemaphoreGive( xTurnRightSemaphore);
				displayString("iniPassengerUp");
				//start timer
				enableAutoTimer();
				break;
		case PASSENGER_DOWN_EVENT:
				state[depth] = passengerDown;
				state[depth + 1] = iniPassengerDown;
				stateDepth++;
				xSemaphoreGive( xTurnLeftSemaphore);
				displayString("iniPassengerDown");
				//start timer
				enableAutoTimer();
				break;

	}

}

void passengerUpSM(int event, int depth){

	
	switch(event){
		
		case LIMIT_UP_EVENT:
				state[depth] = passengerNeutral;
				stateDepth--;
				xSemaphoreGive(xFastStopSemaphore);
				displayString("passengerNeutral");
				break;
		case OBSTACLE_EVENT:
			xSemaphoreGive(xTurnLeftSemaphore);
			displayString("emergencyDown");
			//start timer
			stateDepth = 2;
			state[stateDepth-1] = emergencyDown;
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
				state[depth - 1] = passengerNeutral;
				stateDepth--;
				break;
	
	}

}

void passengerDownSM(int event, int depth){

	switch(event){
		
		case LIMIT_DOWN_EVENT:
				state[depth] = passengerNeutral;
				stateDepth--;
				xSemaphoreGive(xFastStopSemaphore);
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
				state[depth - 1] = passengerNeutral;
				stateDepth--;
				break;
			
	}

}


void emergencyDownSM(int event, int depth){
	
	switch(event){
	
		case TIMER_TICK_EVENT:
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

