#ifndef __SI4X6X__H__
#define __SI4X6X__H__

#include "ch.h"
#include "hal.h"
#include "../defines.h"
#include "../modules.h"

#define RADIO_SDN_SET(radio, state)			(radio == RADIO_2M ? palWritePad(GPIOA, 12, state) : palWritePad(GPIOB, 12, state))
#define RADIO_CS_SET(radio, state)			(radio == RADIO_2M ? palWritePad(GPIOA, 11, state) : palWritePad(GPIOB, 0, state))
#define RF_GPIO0_SET(radio, state)			(radio == RADIO_2M ? palWritePad(GPIOA, 10, state) : palWritePad(GPIOE, 13, state))
#define RF_GPIO1_SET(radio, state)			(radio == RADIO_2M ? palWritePad(GPIOA, 9, state) : palWritePad(GPIOE, 14, state))
#define MOD_GPIO_SET(radio, state)			RF_GPIO1_SET(radio, state)
#define RADIO_WITHIN_MAX_PWR(radio, dBm)	(radio == RADIO_2M ? (dBm) <= RADIO1_MAX_PWR : (dBm) <= RADIO2_MAX_PWR)
#define RADIO_WITHIN_FREQ_RANGE(frequ)		((frequ) >= 119000000 && (frequ) <= 1050000000)
#define RADIO_MAX_PWR(radio)				(radio == RADIO_2M ? RADIO1_MAX_PWR : RADIO2_MAX_PWR)

void Si446x_Init(radio_t radio, modulation_t modem_type);
void Si446x_write(radio_t radio, uint8_t* txData, uint32_t len);
void sendFrequencyToSi446x(radio_t radio, uint32_t freq);
void setModemAFSK(radio_t radio);
void setModemCW(radio_t radio);
void setDeviation(radio_t radio, uint32_t deviation);
void setPowerLevel(radio_t radio, int8_t level);
void startTx(radio_t radio);
void stopTx(radio_t radio);
void radioShutdown(radio_t radio);
bool radioTune(radio_t radio, uint32_t frequency, int8_t level);
int8_t Si446x_getTemperature(radio_t radio);
uint8_t dBm2powerLvl(int32_t dBm);

#endif
