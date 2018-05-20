
#include "FreeRTOS.h"
#include "semphr.h"
#include "output_ports.h"

extern SemaphoreHandle_t xTurnRightSemaphore;
extern SemaphoreHandle_t xTurnLeftSemaphore;
extern SemaphoreHandle_t xFastStopSemaphore;

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

