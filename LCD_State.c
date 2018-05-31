#include "LCD_State.h"
#include "lcd.h"

void init_LCD_output(void)
{
	// Port D
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
	{
	}
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
	// Port B
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
	{
	}
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	LCD_init();
}


void clear(void)
{
	LCD_command(0x0F);          /* turn on display, cursor blinking */
	LCD_command(0x0F);          /* turn on display, cursor blinking */
		LCD_command(1);       /* clear display */
		LCD_command(0x80);    /* lcd cursor location */	
}

void displayString(char *s)
{
	clear();
	int len,i;
	delayMs(20);
	len = strlen(s);
	for (i = 0; i < len; i++)
	{
		LCD_data(s[i]);
	}
}

