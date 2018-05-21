#include "StateNames.h"
#include "inputs.h"
#include "semphr.h"

extern SemaphoreHandle_t xTurnRightSemaphore;
extern SemaphoreHandle_t xTurnLeftSemaphore;
extern SemaphoreHandle_t xFastStopSemaphore;


int stateDepth = 3;
int state[6];

typedef void (*StateFunction)(int, int);

extern StateFunction stateMachines[20];

void iniDriverDownSM (int event, int depth){

	switch(event){
	
		case TIMER_TICK_EVENT:
			state[stateDepth - 1] = manualDriverDown;
			break;
		case DRIVER_NEUTRAL_EVENT:
			state[stateDepth - 1] = autoDriverDown;
			break;

	}

}
void manualDriverDownSM (int event, int depth){
	stateDepth = 0;
	state[stateDepth++] = safe;
	switch(event){

		case LIMIT_DOWN_EVENT:
			xSemaphoreGive(xFastStopSemaphore);
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;

	}

}
void iniDriverUpSM (int event, int depth){

	switch(event){
	
		case TIMER_TICK_EVENT:
			state[stateDepth - 1] = manualDriverUp;
			break;
		case DRIVER_NEUTRAL_EVENT:
			state[stateDepth - 1] = autoDriverUp;
			break;

	}

}
void manualDriverUpSM (int event, int depth){
	stateDepth = 0;
	state[stateDepth++] = safe;
	switch(event){

		case LIMIT_UP_EVENT:
			xSemaphoreGive(xFastStopSemaphore);
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;
	}

}
void driverDownSM (int event, int depth){
	stateDepth = 0;
	state[stateDepth++] = safe;
	switch(event){
	
		case DRIVER_UP_EVENT:
			xSemaphoreGive(xTurnRightSemaphore);
			state[stateDepth++] = driverUp;
			state[stateDepth++] = manualDriverUp;
			break;
		case LIMIT_DOWN_EVENT:
			xSemaphoreGive(xFastStopSemaphore);
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void driverUpSM (int event, int depth){
	stateDepth = 0;
	state[stateDepth++] = safe;
	switch(event){
	
		case DRIVER_DOWN_EVENT:
			xSemaphoreGive(xTurnLeftSemaphore);
			state[stateDepth++] = driverDown;
			state[stateDepth++] = manualDriverDown;
			break;
		case LIMIT_UP_EVENT:
			xSemaphoreGive(xFastStopSemaphore);
			state[stateDepth++] = driverNeutral;
			state[stateDepth++] = passengerNeutral;
			break;
		case OBSTACLE_EVENT:
			xSemaphoreGive(xTurnLeftSemaphore);
			//start timer
			state[stateDepth++] = emergencyDown;
			break;
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void safeSM(int event, int depth){

	state[depth] = safe;
	switch(event){
	
		case ENGINE_EVENT:
			break;
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void driverNeutralSM(int event, int depth){

	state[depth] = driverNeutral;
	
	switch(event){
	
		case DRIVER_UP_EVENT:
				//driverUPSM(event, depth);
				break;
		case DRIVER_DOWN_EVENT:
				//driverDownSM(event, depth);
				break;
		default:
				if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void passengerNeutralSM(int event, int depth){

	state[depth] = passengerNeutral;
	
	switch(event){
	
		case PASSENGER_UP_EVENT:
				state[depth] = passengerUp;
				state[depth + 1] = iniPassengerUp;
				stateDepth = 4;
		
				xSemaphoreGive( xTurnRightSemaphore);
				//start timer
				break;
		case PASSENGER_DOWN_EVENT:
				state[depth] = passengerDown;
				state[depth + 1] = iniPassengerDown;
				stateDepth = 4;
		
				xSemaphoreGive( xTurnLeftSemaphore);
				//start timer
				break;

	}

}

void passengerUpSM(int event, int depth){

	
	switch(event){
		
		case LIMIT_UP_EVENT:
				state[depth] = passengerNeutral;
				stateDepth = 3;
		
				xSemaphoreGive(xFastStopSemaphore);
				break;
		case OBSTACLE_EVENT:
			xSemaphoreGive(xTurnLeftSemaphore);
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
	
		case TIMER_TICK_EVENT:
			 state[depth] = manualPassengerUp;
				break;
		case PASSENGER_NEUTRAL_EVENT:
				state[depth] = autoPassengerUp;
				break;

	}

}

void manualPassengerUpSM(int event, int depth){

	switch(event){

		case PASSENGER_NEUTRAL_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				state[depth - 1] = passengerNeutral;
				stateDepth = 3;
				break;
	
	}

}

void passengerDownSM(int event, int depth){

	switch(event){
		
		case LIMIT_DOWN_EVENT:
				state[depth] = passengerNeutral;
				stateDepth = 3;
				xSemaphoreGive(xFastStopSemaphore);
				break;

		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void iniPassengerDownSM(int event, int depth){

	switch(event){
	
		case TIMER_TICK_EVENT:
			 state[depth] = manualPassengerDown;
				break;
		case PASSENGER_NEUTRAL_EVENT:
				state[depth] = autoPassengerDown;
				break;
	}

}

void manualPassengerDownSM(int event, int depth){

	switch(event){
	

		case PASSENGER_NEUTRAL_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				state[depth - 1] = passengerNeutral;
				stateDepth = 3;
				break;
			
	}

}


void emergencyDownSM(int event, int depth){
	
	switch(event){
	
		case TIMER_TICK_EVENT:
		case LIMIT_DOWN_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				stateDepth = 0;
				state[stateDepth++] = safe;
				state[stateDepth++] = driverNeutral;
				state[stateDepth++] = passengerNeutral;
				break;

	}
	
}

