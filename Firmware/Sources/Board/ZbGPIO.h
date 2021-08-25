// -----------------------------------------
// Board-specific GPIO functions
// ----------------------------------------

#ifndef __ZBGPIO_H
#define __ZBGPIO_H

// Include
#include "stdinc.h"
#include "ZwDSP.h"

// Functions
//
void ZbGPIO_Init();
//
void ZbGPIO_SwitchLED1(Boolean Set);
void ZbGPIO_SwitchLED2(Boolean Set);
void ZbGPIO_ToggleLED1();
void ZbGPIO_T_DUT_CS(Boolean Set);
void ZbGPIO_IMC_CS(Boolean Set);
void ZbGPIO_IHC_CS(Boolean Set);
void ZbGPIO_ControlMCB(Boolean Set);
void ZbGPIO_MCB_LDAC(Boolean Set);
void ZbGPIO_DateDrv_LDAC(Boolean Set);
void ZbGPIO_DAC_LDAC(Boolean Set);
void ZbGPIO_SYNC(Boolean Set);
void ZbGPIO_T_COOL_CS(Boolean Set);

#endif // __ZBGPIO_H
