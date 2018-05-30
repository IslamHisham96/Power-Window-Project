#include "Timers.h"



void configureAutoTimer(void (*autoTimerHandler)(void))
{
	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	 ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_A_ONE_SHOT);
	 TimerIntRegister(TIMER0_BASE, TIMER_A, autoTimerHandler);
	 ROM_IntEnable(INT_TIMER0A);
   ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	
}

void enableAutoTimer(void)
{
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet());
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
}

void configureEngineTimer(void (*engineTimerHandler)(void))
{
	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	 ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_A_ONE_SHOT);
	 TimerIntRegister(TIMER1_BASE, TIMER_A, engineTimerHandler);
	 ROM_IntEnable(INT_TIMER1A);
   ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	
}

void enableEngineTimer(void)
{
	ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, 15*ROM_SysCtlClockGet()); // ROM_SysCtlClockGet() = 1s
	ROM_TimerEnable(TIMER1_BASE, TIMER_A);
}
