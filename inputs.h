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

#ifndef inputs
#define inputs
#define DRIVER_PORT GPIO_PORTA_BASE
#define LOCK_PORT GPIO_PORTA_BASE
#define PASSENGER_PORT GPIO_PORTA_BASE
#define LIMIT_PORT GPIO_PORTF_BASE
#define ENGINE_PORT GPIO_PORTC_BASE
#define OBSTACLE_PORT GPIO_PORTA_BASE

#define DRIVER_UP GPIO_PIN_5
#define DRIVER_DOWN GPIO_PIN_6
#define PASSENGER_UP GPIO_PIN_3
#define PASSENGER_DOWN GPIO_PIN_2
#define LOCK GPIO_PIN_7
#define LIMIT_UP GPIO_PIN_0
#define LIMIT_DOWN GPIO_PIN_4
#define ENGINE GPIO_PIN_6
#define OBSTACLE GPIO_PIN_4

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
#define OBSTACLE_EVENT 11
#define AUTO_TIMER_TICK_EVENT 12
#define OBSTACLE_TIMER_TICK_EVENT 13
#define ENGINE_TIMER_TICK_EVENT 14



void init_input(void);
void portf_int(void (*pfnIntHandler)(void));
void porta_int(void (*pfnIntHandler)(void));
void portc_int(void (*pfnIntHandler)(void));
#endif
