#include "output_ports.h"

void init_output(void)
{
ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
ROM_GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, GPIO_DIR_MODE_OUT);
ROM_GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, 0x50);
}

void enable(void)
{
ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x10);
}

void disable(void)
{
 ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x0);
}

void turnRight(void)
{
	enable();
ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_6, 0x20);
}


void turnLeft(void)
{
	enable();
ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_6, 0x40);
}



void fastStop(void)
{
	enable();
  ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5 | GPIO_PIN_6, 0x0);
}
