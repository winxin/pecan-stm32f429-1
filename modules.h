#ifndef __MODULES_H__
#define __MODULES_H__

#include "position.h"
#include "image.h"
#include "tracking.h"
#include "log.h"
#include "sgp4.h"
#include "config.h"
#include "types.h"

#define MODULE_POSITION(CONF)	{chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048+2048), (CONF)->name, NORMALPRIO, modulePOS,      (CONF)); (CONF)->active=true; }
#define MODULE_IMAGE(CONF)		{chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(4096+2048), (CONF)->name, NORMALPRIO, moduleIMG,      (CONF)); (CONF)->active=true; }
#define MODULE_LOG(CONF)		{chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(1024+2048), (CONF)->name, NORMALPRIO, moduleLOG,      (CONF)); (CONF)->active=true; }
#define MODULE_TRACKING(CYCLE)	 chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(3072+2048), "Tracking",   NORMALPRIO, moduleTRACKING, NULL  );
#define MODULE_RADIO()			 chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048+2048), "Radio",      HIGHPRIO,   moduleRADIO,    NULL  );

#define initEssentialModules() { \
	chMtxObjectInit(&interference_mtx); \
	chMtxObjectInit(&camera_mtx); \
	MODULE_TRACKING(CYCLE_TIME); /* Tracker data input */ \
	MODULE_RADIO(); /* Tracker data output */ \
}

extern char *SMODE_STRING[];
extern char *MOULATION_STRING[];
extern char *PROTOCOL_STRING[];
#define VAL2SMODE(v) SMODE_STRING[v]			/* Returns sleep as string */
#define VAL2MOULATION(v) MOULATION_STRING[v]	/* Returns modulation as string */
#define VAL2PROTOCOL(v) PROTOCOL_STRING[v]		/* Returns protocol as string */

extern mutex_t interference_mtx;	// HF interference mutex (needed to exclude radio from HF sensitiv components [Camera])

extern systime_t watchdog_radio;	// Last update time for module RADIO
extern systime_t watchdog_tracking;	// Last update time for module TRACKING

#endif
