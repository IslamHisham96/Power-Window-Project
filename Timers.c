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
