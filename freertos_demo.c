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


#define DRIVER_UP_EVENT 0
#define DRIVER_DOWN_EVENT 1
#define PASSENGER_UP_EVENT 2
#define PASSENGER_DOWN_EVENT 3
#define PASSENGER_NEUTRAL_EVENT 4
#define DRIVER_NEUTRAL_EVENT 5
#define TIMER_TICK_EVENT 6
#define LOCK_EVENT 7
#define LIMIT_UP_EVENT 8
#define LIMIT_DOWN_EVENT 9
#define ENGINE_EVENT 10




SemaphoreHandle_t xTurnRightSemaphore;
SemaphoreHandle_t xTurnLeftSemaphore;
SemaphoreHandle_t xFastStopSemaphore;
int stateDepth = 3;
int state[6];

typedef void (*StateFunction)(int, int);

StateFunction stateMachines[2];


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
		default:
				if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void passengerUPSM(int event, int depth){

	
	switch(event){
		
		case LIMIT_UP_EVENT:
				state[depth] = passengerNeutral;
				stateDepth = 3;
		
				xSemaphoreGive(xFastStopSemaphore);
				break;

		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void iniPassengerUPSM(int event, int depth){

	
	switch(event){
	
		case TIMER_TICK_EVENT:
			 state[depth] = manualPassengerUp;
				break;
		case PASSENGER_NEUTRAL_EVENT:
				state[depth] = autoPassengerUp;
				break;


		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void manualPassengerUPSM(int event, int depth){

	switch(event){

		case PASSENGER_NEUTRAL_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				state[depth - 1] = passengerNeutral;
				stateDepth = 3;
				break;
		
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
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

		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}

void manualPassengerDownSM(int event, int depth){

	switch(event){
	

		case PASSENGER_NEUTRAL_EVENT:
				xSemaphoreGive(xFastStopSemaphore);
				state[depth - 1] = passengerNeutral;
				stateDepth = 3;
				break;
		
		default:
			if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}

}


void emergencyDownSM(int event, int depth){
	
	switch(event){
	
		//case TIME_PASSED:
				//passengerNeutral state
				//break;
		default:
				if(depth + 1 < stateDepth) stateMachines[state[depth + 1]](event, depth + 1);
		
	}
	
}







//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>FreeRTOS Example (freertos_demo)</h1>
//!
//! This application demonstrates the use of FreeRTOS on Launchpad.
//!
//! The application blinks the user-selected LED at a user-selected frequency.
//! To select the LED press the left button and to select the frequency press
//! the right button.  The UART outputs the application status at 115,200 baud,
//! 8-n-1 mode.
//!
//! This application utilizes FreeRTOS to perform the tasks in a concurrent
//! fashion.  The following tasks are created:
//!
//! - An LED task, which blinks the user-selected on-board LED at a
//!   user-selected rate (changed via the buttons).
//!
//! - A Switch task, which monitors the buttons pressed and passes the
//!   information to LED task.
//!
//! In addition to the tasks, this application also uses the following FreeRTOS
//! resources:
//!
//! - A Queue to enable information transfer between tasks.
//!
//! - A Semaphore to guard the resource, UART, from access by multiple tasks at
//!   the same time.
//!
//! - A non-blocking FreeRTOS Delay to put the tasks in blocked state when they
//!   have nothing to do.
//!
//! For additional details on FreeRTOS, refer to the FreeRTOS web page at:
//! http://www.freertos.org/
//
//*****************************************************************************


//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
xSemaphoreHandle g_pUARTSemaphore;
xQueueHandle eventQueue;
bool p_enable = true;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
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
	if(xTimeBetweenInterrupts_A > 200){
	UARTprintf("entered_a\n");
	//int32_t s;
	//char ss[40];
	uint32_t status = GPIOIntStatus(DRIVER_PORT, true);
	if(status & DRIVER_UP & DRIVER_DOWN){
		//Window_Stop();
		event = DRIVER_NEUTRAL_EVENT;
	}
	else if(status & DRIVER_UP){
		
	//UARTprintf("derp\n");
	//	Window_Up();
		event = DRIVER_UP_EVENT;
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
		event = DRIVER_DOWN_EVENT;
	}
	else if(status & LOCK){
		//p_enable = !p_enable;
		event = LOCK_EVENT;
	}
	
	else if(status & PASSENGER_UP & PASSENGER_DOWN){
		//if(p_enable)
		//Window_Stop();
		event = PASSENGER_NEUTRAL_EVENT;
	}
	
	else if(status & PASSENGER_UP){
		//if(p_enable)
		//Window_Up();
		event = PASSENGER_UP_EVENT;
	}
	
	else if(status & PASSENGER_DOWN){
		//if(p_enable)
		//Window_Down();
		event = PASSENGER_DOWN_EVENT;
	}
	xQueueSendToFrontFromISR(eventQueue,&event,&xHigherPriorityTaskWoken_A);
}
	GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	xTimeISRLastExecuted_A = xTimeNow_A;
	//GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
}
void
Limit_Handler(void){
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
	uint32_t status = GPIOIntStatus(LIMIT_PORT, true);
	if(status & LIMIT_UP & LIMIT_DOWN){
		//***************************************************************
		//Is this legal??
		//***************************************************************
	}
	else if(status & LIMIT_UP){
		event = LIMIT_UP_EVENT;
	}
	else if(status & LIMIT_DOWN){
		event = LIMIT_DOWN_EVENT;
	}
	else if(status & ENGINE){
		event = ENGINE_EVENT;
	}
	xQueueSendToFrontFromISR(eventQueue,&event,&xHigherPriorityTaskWoken_C);
}
	GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
	xTimeISRLastExecuted_C = xTimeNow_C;
}
void
Main_Task(void * pvParameters)
{
	
}





int
main(void)
{
	
	
	//Semaphores creation to synchronize Actuator Tasks with Main Task
	vSemaphoreCreateBinary( xTurnRightSemaphore );
	vSemaphoreCreateBinary( xTurnLeftSemaphore );
	vSemaphoreCreateBinary( xFastStopSemaphore );
	
	//Creation of Actuator Tasks with priority higher than Main Task
	xTaskCreate( vTurnRightHandlerTask, "vTurnRightHandlerTask", 240, NULL, 3, NULL );
	xTaskCreate( vTurnLeftHandlerTask, "vTurnLeftHandlerTask", 240, NULL, 3, NULL );
	xTaskCreate( vFastStopHandlerTask, "vFastStopHandlerTask", 240, NULL, 3, NULL );
    //
    // Set the clocking to run at 50 MHz from the PLL.
    //
    //ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |  SYSCTL_OSC_MAIN);
    ConfigureUART();
	
		//create event queue with size 1 to send events (may be changed)
		eventQueue = xQueueCreate(1,sizeof(uint32_t));
    //
    // Print demo introduction.
    //
    UARTprintf("\n\nWelcome to the EK-TM4C123GXL FreeRTOS Demo!\n");
		init_input();
		init_output();
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
