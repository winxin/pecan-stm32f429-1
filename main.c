#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include "ptime.h"
#include "config.h"
#include "debug.h"
#include "modules.h"
#include "padc.h"
#include "pi2c.h"
#include "pac1720.h"
#include "bme280.h"
#include "sd.h"

static virtual_timer_t vt;
uint32_t counter = 0;
uint32_t error = 0;

static const WDGConfig wdgcfg = {
	STM32_IWDG_PR_256,
	STM32_IWDG_RL(10000),
	STM32_IWDG_WIN_DISABLED
};

/**
  * LED blinking routine
  * RED LED blinks: One or more modules crashed (software watchdog)
  * GREEN LED blinks: I'm alive! (STM32 crashed if not blinking)
  * YELLOW LED: Camera takes a photo (See image.c)
  */
static void led_cb(void *led_sw) {
	// Switch LEDs
	palWritePad(PORT(LED_3GREEN), PIN(LED_3GREEN), (bool)led_sw);	// Show I'M ALIVE
	if(error) {
		palWritePad(PORT(LED_1RED), PIN(LED_1RED), (bool)led_sw);	// Show error
	} else {
		palSetPad(PORT(LED_1RED), PIN(LED_1RED));	// Shut off error
	}

	led_sw = (void*)!led_sw; // Set next state

	chSysLockFromISR();
	chVTSetI(&vt, MS2ST(500), led_cb, led_sw);
	chSysUnlockFromISR();
}

/**
  * Main routine is starting up system, runs the software watchdog (module monitoring), controls LEDs
  */
int main(void) {
	halInit();					// Startup HAL
	chSysInit();				// Startup RTOS

	palClearPad(PORT(LED_4GREEN), PIN(LED_4GREEN)); // Show I'M ALIVE

	DEBUG_INIT();				// Debug Init (Serial debug port, LEDs)
	TRACE_INFO("MAIN > Startup");

	pi2cInit();					// Startup I2C
	initEssentialModules();		// Startup required modules (input/output modules)
	initModules();				// Startup optional modules (eg. POSITION, LOG, ...)
	pac1720_init();				// Startup current measurement
	initSD();					// Startup SD

	chThdSleepMilliseconds(100);

	// Initialize LED timer
	chVTObjectInit(&vt);
	chVTSet(&vt, MS2ST(500), led_cb, 0);

	chThdSleepMilliseconds(1000);

	// Initialize Watchdog
	wdgStart(&WDGD1, &wdgcfg);
	wdgReset(&WDGD1);

	while(true) {
		// Print time
		if(counter % 10 == 0)
		PRINT_TIME("MAIN");

		// Watchdog reset
		wdgReset(&WDGD1);

		// Software watchdog FIXME
		/*for(uint8_t i=0; i<moduleCount; i++) {
			if(ST2S(chVTGetSystemTimeX() - modules[i]->lastCycle) <= (uint32_t)modules[0]->cycle + CYCLE_TIME) {
				TRACE_INFO("MAIN > Module %s OK", modules[i]->name);
				error = 0;
			} else {
				TRACE_ERROR("MAIN > Module %s CRASHED", modules[i]->name);
				error = 1; // Let red LED blink in virtual timer
			}
		}*/

		chThdSleepMilliseconds(1000);
		counter++;
	}
}

