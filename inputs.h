#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "drivers/buttons.h"
#include "FreeRTOS.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "drivers/buttons.h"
#define DRIVER_PORT GPIO_PORTA_BASE
#define LOCK_PORT GPIO_PORTA_BASE
#define PASSENGER_PORT GPIO_PORTA_BASE
#define LIMIT_PORT GPIO_PORTC_BASE
#define ENGINE_PORT GPIO_PORTC_BASE


#define DRIVER_UP GPIO_PIN_5
#define DRIVER_DOWN GPIO_PIN_6
#define PASSENGER_UP GPIO_PIN_3
#define PASSENGER_DOWN GPIO_PIN_2
#define LOCK GPIO_PIN_7
#define LIMIT_UP GPIO_PIN_4
#define LIMIT_DOWN GPIO_PIN_5
#define ENGINE GPIO_PIN_6


void init_input(void);
void portf_int(void (*pfnIntHandler)(void));
void porta_int(void (*pfnIntHandler)(void));
void portc_int(void (*pfnIntHandler)(void));
