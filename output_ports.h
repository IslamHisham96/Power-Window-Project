#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "drivers/buttons.h"
#include "FreeRTOS.h"

#define MOTOR_PORT GPIO_PORTB_BASE

#define MOTOR_ENABLE GPIO_PIN_4
#define MOTOR_INPUT1 GPIO_PIN_5
#define MOTOR_INPUT2 GPIO_PIN_6

void init_output(void);
void turnRight(void);
void turnLeft(void);
void enable(void);
void disable(void);
void fastStop(void);
