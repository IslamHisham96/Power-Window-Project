#include "inputs.h"

void init_input(void)
{
		ROM_SysCtlPeripheralEnable(BUTTONS_GPIO_PERIPH);

    //
    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    //
    HWREG(BUTTONS_GPIO_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(BUTTONS_GPIO_BASE + GPIO_O_CR) |= 0x01;
    HWREG(BUTTONS_GPIO_BASE + GPIO_O_LOCK) = 0;

    //
    // Set each of the button GPIO pins as an input with a pull-up.
    //
    ROM_GPIODirModeSet(BUTTONS_GPIO_BASE, ALL_BUTTONS, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(BUTTONS_GPIO_BASE, ALL_BUTTONS,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//
// Wait for the GPIOA module to be ready.
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
{
}SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//
// Wait for the GPIOA module to be ready.
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
{
}
		GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	ROM_GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_STRENGTH_6MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
	ROM_GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		//ROM_GPIODirModeSet(GPIO_PORTC_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_DIR_MODE_IN);
}
void portf_int(void (*pfnIntHandler)(void)){
		GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);		// Disable interrupt for PF4 (in case it was enabled)
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);		// Clear pending interrupts for PF4
		GPIOIntRegister(GPIO_PORTF_BASE, pfnIntHandler);		// Register our handler function for port F
		GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0,
		GPIO_FALLING_EDGE);				// Configure PF4 for falling edge trigger
		GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4| GPIO_PIN_0);		// Enable interrupt for PF4
}
//driver and passneger up and down
void porta_int(void (*pfnIntHandler)(void)){
		/*GPIOIntRegister(GPIO_PORTA_BASE, pfnIntHandler);		// Register our handler function for port A
		GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);		// Clear pending interrupts
		GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6, GPIO_RISING_EDGE);
		GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
		GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);		// Enable interrupt
	*/
		GPIOIntDisable(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);		// Disable interrupt for PF4 (in case it was enabled)
		GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);		// Clear pending interrupts for PF4
		GPIOIntRegister(GPIO_PORTA_BASE, pfnIntHandler);		// Register our handler function for port F
		GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_4 , GPIO_FALLING_EDGE);		
		GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_BOTH_EDGES);		// Configure PF4 for falling edge trigger
		GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);		// Enable interrupt for PF4
}
void portc_int(void (*pfnIntHandler)(void)){
	/*GPIOIntRegister(GPIO_PORTC_BASE, pfnIntHandler);		// Register our handler function for port C
		GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);		// Clear pending interrupts
		GPIOIntTypeSet(GPIO_PORTC_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_RISING_EDGE);
		GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);		// Enable interrupt
	*/
	GPIOIntDisable(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);		// Disable interrupt for PF4 (in case it was enabled)
		GPIOIntClear(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);		// Clear pending interrupts for PF4
		GPIOIntRegister(GPIO_PORTC_BASE, pfnIntHandler);		// Register our handler function for port F
		GPIOIntTypeSet(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5, GPIO_FALLING_EDGE);		
		GPIOIntTypeSet(GPIO_PORTC_BASE,GPIO_PIN_6, GPIO_BOTH_EDGES);		// Configure PF4 for falling edge trigger
		GPIOIntEnable(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);		// Enable interrupt for PF4
}
