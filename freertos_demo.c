//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS example.
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inputs.h"
#include "output_ports.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "StateMachinesFunctions.h"
#include "StateNames.h"
#include "TaskHandlers.h"
#include "Timers.h"
#include "LCD_State.h"




typedef void (*StateFunction)(int, int);
StateFunction stateMachines[22];



SemaphoreHandle_t xTurnRightSemaphore;
SemaphoreHandle_t xTurnLeftSemaphore;
SemaphoreHandle_t xFastStopSemaphore;



xSemaphoreHandle g_pUARTSemaphore;
xQueueHandle eventQueue;
bool p_enable = true;




void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    while(1)
    {
    }
}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
		//
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************


void
Window_Up(void){
	turnRight();
}
void
Window_Down(void){
	turnLeft();
}
void
Window_Stop(void){
	fastStop();
}

void
Window_Handler(void){
	//for PortA
	static TickType_t xTimeISRLastExecuted_A = 0;
	
	//initializations
	//char ss[40];
	TickType_t xTimeNow_A, xTimeBetweenInterrupts_A;
	portBASE_TYPE xHigherPriorityTaskWoken_A = pdFALSE;
	int32_t event=0;
	
	//delay by counting ticks
	xTimeNow_A = xTaskGetTickCountFromISR();
	xTimeBetweenInterrupts_A = xTimeNow_A - xTimeISRLastExecuted_A;
	
	
//			sprintf(ss,"%d\n",xTaskGetTickCountFromISR());
//			UARTprintf(ss);
	
	//UARTprintf("entered_an't\n");
	if(xTimeBetweenInterrupts_A > 200){
	UARTprintf("entered_a\n");
	//int32_t s;
	//char ss[40];
	uint32_t status = GPIOIntStatus(DRIVER_PORT, true);
	if (status & OBSTACLE){
		event = OBSTACLE_EVENT;
		UARTprintf("obstacle\n");
	}
	else if(status & DRIVER_UP & DRIVER_DOWN){
		//Window_Stop();
		event = DRIVER_NEUTRAL_EVENT;
	}
	else if(status & DRIVER_UP){
		
	//UARTprintf("derp\n");
	//	Window_Up();
		if(GPIOPinRead(DRIVER_PORT,DRIVER_UP) == 0){
		//UARTprintf("pressed\n");
		event = DRIVER_UP_EVENT;
		}
		else{
		//UARTprintf("released\n");
		event = DRIVER_NEUTRAL_EVENT;
		}
		UARTprintf("DU\n");
	}
	else if(status & DRIVER_DOWN){
	//UARTprintf("herp\n");
		//s = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_4| GPIO_PIN_5 |GPIO_PIN_6 );
		//sprintf(ss,"%d\n",s);
		//UARTprintf(ss);
			//Window_Down();
		//s = GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_4| GPIO_PIN_5 |GPIO_PIN_6 );
		//sprintf(ss,"%d\n",s);
		//UARTprintf(ss);
		if(GPIOPinRead(DRIVER_PORT,DRIVER_DOWN) == 0){
		//UARTprintf("pressed\n");
		event = DRIVER_DOWN_EVENT;
		}
		else{
		//UARTprintf("released\n");
		event = DRIVER_NEUTRAL_EVENT;
		}
		UARTprintf("DD\n");
	}
	else if(status & LOCK){
		//p_enable = !p_enable;
		event = LOCK_EVENT;
		UARTprintf("LOCK\n");
	}
	
	else if(status & PASSENGER_UP & PASSENGER_DOWN){
		//if(p_enable)
		//Window_Stop();
		event = PASSENGER_NEUTRAL_EVENT;
	}
	
	else if(status & PASSENGER_UP){
		//if(p_enable)
		//Window_Up();
		if(GPIOPinRead(PASSENGER_PORT,PASSENGER_UP) == 0){
		//UARTprintf("pressed\n");
		event = PASSENGER_UP_EVENT;
		}
		else{
		//UARTprintf("released\n");
		event = PASSENGER_NEUTRAL_EVENT;
		}
		UARTprintf("PU\n");
	}
	
	else if(status & PASSENGER_DOWN){
		//if(p_enable)
		//Window_Down();
		if(GPIOPinRead(PASSENGER_PORT,PASSENGER_DOWN) == 0){
		//UARTprintf("pressed\n");
		event = PASSENGER_DOWN_EVENT;
		}
		else{
		//UARTprintf("released\n");
		event = PASSENGER_NEUTRAL_EVENT;
		}
		UARTprintf("PD\n");
	}
	xQueueSendToFrontFromISR(eventQueue,&event,&xHigherPriorityTaskWoken_A);
}
	GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	xTimeISRLastExecuted_A = xTimeNow_A;
	//GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
}
void
Engine_Handler(void){
	//for PortC
	static TickType_t xTimeISRLastExecuted_C = 0;
	
	//initializations
	//char ss[40];
	TickType_t xTimeNow_C, xTimeBetweenInterrupts_C;
	portBASE_TYPE xHigherPriorityTaskWoken_C = pdFALSE;
	int32_t event = 0;
	
	//delay by counting ticks
	xTimeNow_C = xTaskGetTickCountFromISR();
	xTimeBetweenInterrupts_C = xTimeNow_C - xTimeISRLastExecuted_C;
//			sprintf(ss,"%d\n",xTaskGetTickCountFromISR());
//			UARTprintf(ss);
	if(xTimeBetweenInterrupts_C > 200){
	UARTprintf("entered_c\n");
	uint32_t status = GPIOIntStatus(ENGINE_PORT, true);
	if(status & ENGINE){
		event = ENGINE_EVENT;
		UARTprintf("ENG\n");
	}
	xQueueSendToFrontFromISR(eventQueue,&event,&xHigherPriorityTaskWoken_C);
}
	GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_6);
	xTimeISRLastExecuted_C = xTimeNow_C;
}

void
Limit_Handler(void){
	//for PortF
	static TickType_t xTimeISRLastExecuted_F = 0;
	
	//initializations
	//char ss[40];
	TickType_t xTimeNow_F, xTimeBetweenInterrupts_F;
	portBASE_TYPE xHigherPriorityTaskWoken_F = pdFALSE;
	int32_t event = 0;
	
	//delay by counting ticks
	xTimeNow_F = xTaskGetTickCountFromISR();
	xTimeBetweenInterrupts_F = xTimeNow_F - xTimeISRLastExecuted_F;
//			sprintf(ss,"%d\n",xTaskGetTickCountFromISR());
//			UARTprintf(ss);
	if(xTimeBetweenInterrupts_F > 200){
	UARTprintf("entered_f\n");
	uint32_t status = GPIOIntStatus(LIMIT_PORT, true);
	if(status & LIMIT_UP & LIMIT_DOWN){
		//***************************************************************
		//Is this legal??
		//***************************************************************
	}
	else if(status & LIMIT_UP){
		event = LIMIT_UP_EVENT;
		UARTprintf("LU\n");
	}
	else if(status & LIMIT_DOWN){
		event = LIMIT_DOWN_EVENT;
		UARTprintf("LD\n");
	}
	xQueueSendToFrontFromISR(eventQueue,&event,&xHigherPriorityTaskWoken_F);
}
	
	GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	xTimeISRLastExecuted_F = xTimeNow_F;
}


void autoTimerHandler(void)
{
	ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	int32_t event = AUTO_TIMER_TICK_EVENT;
	portBASE_TYPE xHigherPriorityTaskWoken_AutoTimer = pdFALSE;	
	UARTprintf("timer_auto\n");
	xQueueSendToFrontFromISR(eventQueue,&event,&xHigherPriorityTaskWoken_AutoTimer);
	
}

void engineTimerHandler(void)
{
	ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	int32_t event = ENGINE_TIMER_TICK_EVENT;
	portBASE_TYPE xHigherPriorityTaskWoken_EngineTimer = pdFALSE;
	UARTprintf("timer_engine\n");
	xQueueSendToFrontFromISR(eventQueue,&event,&xHigherPriorityTaskWoken_EngineTimer);
	
}	

void obstacleTimerHandler(void)
{
	ROM_TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	int32_t event = OBSTACLE_TIMER_TICK_EVENT;
	portBASE_TYPE xHigherPriorityTaskWoken_ObstacleTimer = pdFALSE;
	UARTprintf("timer_obstacle\n");
	xQueueSendToFrontFromISR(eventQueue,&event,&xHigherPriorityTaskWoken_ObstacleTimer);	
	
}

int
main(void)
{

	
	//Initialize StateMachineArray
	stateMachines[ safe ] = &safeSM;
	stateMachines[ driverNeutral ] = &driverNeutralSM;
	stateMachines[ passengerDown ] = &passengerDownSM;
	stateMachines[ passengerUp ] = &passengerUpSM;
	stateMachines[ driverDown ] = &driverDownSM;
	stateMachines[ driverUp ] = &driverUpSM;
	stateMachines[ passengerNeutral ] = &passengerNeutralSM;
	stateMachines[ iniPassengerDown ] = &iniPassengerDownSM;
	stateMachines[ manualPassengerDown ] = &manualPassengerDownSM;
	stateMachines[ manualPassengerUp ] = &manualPassengerUpSM;
	stateMachines[ iniDriverDown ] = &iniDriverDownSM;
	stateMachines[ manualDriverDown ] = &manualDriverDownSM;
	stateMachines[ iniDriverUp ] = &iniDriverUpSM;
	stateMachines[ manualDriverUp ] = &manualDriverUpSM;
	stateMachines[ emergencyDown ] = &emergencyDownSM;
	stateMachines[ autoDriverUp ] = &autoDriverUpSM;
	stateMachines[ autoDriverDown ] = &autoDriverDownSM;
	stateMachines[ autoPassengerUp ] = &autoPassengerUpSM;
	stateMachines[ autoPassengerDown ] = &autoPassengerDownSM;
	stateMachines[ dying ] = &dyingSM;
	stateMachines[ dead ] = &deadSM;
	
	//Semaphores creation to synchronize Actuator Tasks with Main Task
	vSemaphoreCreateBinary( xTurnRightSemaphore );
	vSemaphoreCreateBinary( xTurnLeftSemaphore );
	vSemaphoreCreateBinary( xFastStopSemaphore );
	
	//Creation of Actuator Tasks with priority higher than Main Task
	xTaskCreate( vTurnRightHandlerTask, "vTurnRightHandlerTask", 240, NULL, 3, NULL );
	xTaskCreate( vTurnLeftHandlerTask, "vTurnLeftHandlerTask", 240, NULL, 3, NULL );
	xTaskCreate( vFastStopHandlerTask, "vFastStopHandlerTask", 240, NULL, 3, NULL );
	
	//Create Main Task to receive events and pass it to state machine
	xTaskCreate( vMainHandlerTask, "vMainHandlerTask", 240, NULL, 1, NULL );
    //
    // Set the clocking to run at 50 MHz from the PLL.
    //
    //ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |  SYSCTL_OSC_MAIN);
    ConfigureUART();
		
		//timer for automatic
		
		configureAutoTimer(autoTimerHandler);
		configureObstacleTimer(obstacleTimerHandler);
		configureEngineTimer(engineTimerHandler);
	
		//create event queue with size 1 to send events (may be changed)
		eventQueue = xQueueCreate(1,sizeof(uint32_t));
    //
    // Print demo introduction.
    //
    UARTprintf("\n\nWelcome to the EK-TM4C123GXL FreeRTOS Demo!\n");
		init_input();
		init_output();
		init_LCD_output();
		displayString("Neutral");
		porta_int(Window_Handler);
		//portf_int(Window_Handler);
    portc_int(Limit_Handler);
		//
    // Initialize the UART and configure it for 115,200, 8-N-1 operation.
    //
		
    //
    // Create a mutex to guard the UART.
    //
    //g_pUARTSemaphore = xSemaphoreCreateMutex();
		
    //
    // Create the LED task.
    //
    /*if(LEDTaskInit() != 0)
    {

        while(1)
        {
        }
    }

    //
    // Create the switch task.
    //
    if(SwitchTaskInit() != 0)
    {

        while(1)
        {
        }
    }
		*/

    //
    // Start the scheduler.  This should not return.
    //
    vTaskStartScheduler();

    //
    // In case the scheduler returns for some reason, print an error and loop
    // forever.
    //
		
			//int32_t s;
			//char ss[40];
    while(1)
    {
       //s = GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 |GPIO_PIN_6 | GPIO_PIN_7);
			//sprintf(ss,"%d\n",s);
			//UARTprintf(ss);
    }
}
