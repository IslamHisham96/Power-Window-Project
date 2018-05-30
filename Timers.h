#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"


void configureAutoTimer(void (*autoTimerHandler)(void));
void enableAutoTimer(void);
void configureEngineTimer(void (*engineTimerHandler)(void));
void enableEngineTimer(void);
