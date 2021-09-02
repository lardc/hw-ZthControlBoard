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
void ZbGPIO_ToggleLED2();
void ZbGPIO_IGBT_SetUGate20V();
void ZbGPIO_IGBT_SetUGate15V();
void ZbGPIO_MCB_Sync(Boolean State);
void ZbGPIO_SB_Im_CS(Boolean State);
void ZbGPIO_SB_TSP_CS(Boolean State);
void ZbGPIO_DCB_LDAC(Boolean State);
void ZbGPIO_MCB_LDAC(Boolean State);
void ZbGPIO_LowPowerSupplyControl(Boolean State);
void ZbGPIO_DCB_MCB_CS(Boolean State);
void ZbGPIO_SB_Tcool1_CS(Boolean State);
void ZbGPIO_SB_Tcool2_CS(Boolean State);
void ZbGPIO_SB_Ih_CS(Boolean State);
void ZbGPIO_SB_Tc1_CS(Boolean State);
void ZbGPIO_SB_Tc2_CS(Boolean State);
void ZbGPIO_EEPROM_CS(Boolean State);
void ZbGPIO_SyncOscilloscope(Boolean State);
void ZbGPIO_RegisterRCLK(Boolean State);
Boolean ZbGPIO_IsTransformer1Overheated();
Boolean ZbGPIO_IsTransformer2Overheated();
Boolean ZbGPIO_IsRectifierOverheated();
Boolean ZbGPIO_IsWater();

#endif // __ZBGPIO_H
