
#include "FreeRTOS.h"
#include "semphr.h"
#include "output_ports.h"
#include "LCD_State.h"

extern SemaphoreHandle_t xTurnRightSemaphore;
extern SemaphoreHandle_t xTurnLeftSemaphore;
extern SemaphoreHandle_t xFastStopSemaphore;
extern xQueueHandle eventQueue;
extern SemaphoreHandle_t xDisplaySemaphore;
extern char* stateForDisplay;
extern int state[6];

typedef void (*StateFunction)(int, int);
extern StateFunction stateMachines[21];

void vMainHandlerTask( void *pvParameters ){
for( ;; )
{
	int event;
	xQueueReceive(eventQueue,&event,portMAX_DELAY );
	stateMachines[state[0]](event, 0);
}
}

void vTurnRightHandlerTask( void *pvParameters )
{
xSemaphoreTake( xTurnRightSemaphore, 0 );
for( ;; )
{
xSemaphoreTake( xTurnRightSemaphore, portMAX_DELAY );
	turnRight();
}
}

void vTurnLeftHandlerTask( void *pvParameters )
{
	
xSemaphoreTake( xTurnLeftSemaphore, 0 );
for( ;; )
{
xSemaphoreTake( xTurnLeftSemaphore, portMAX_DELAY );
	turnLeft();
}
}
void vFastStopHandlerTask( void *pvParameters )
{
xSemaphoreTake( xFastStopSemaphore, 0 );
for( ;; )
{
xSemaphoreTake( xFastStopSemaphore, portMAX_DELAY );
	fastStop();
}
}


void vClearDisplayHandlerTask( void *pvParameters )
{
	
xSemaphoreTake( xDisplaySemaphore, 0 );
for( ;; )
{
xSemaphoreTake( xDisplaySemaphore, portMAX_DELAY );
	displayString(stateForDisplay);
}
}


