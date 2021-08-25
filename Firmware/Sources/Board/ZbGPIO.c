// -----------------------------------------
// Board-specific GPIO functions
// ----------------------------------------

// Header
#include "ZbGPIO.h"
#include "SysConfig.h"

// Functions
//
void ZbGPIO_Init()
{
	// Reset to default state
	ZwGPIO_WritePin(PIN_LED_1, FALSE);
	ZwGPIO_WritePin(PIN_LED_2, FALSE);

   	// Configure pins
   	ZwGPIO_PinToOutput(PIN_LED_1);
   	ZwGPIO_PinToOutput(PIN_LED_2);
}
// ----------------------------------------

void ZbGPIO_SwitchLED1(Boolean Set)
{
	ZwGPIO_WritePin(PIN_LED_1, Set);
}
// ----------------------------------------

void ZbGPIO_SwitchLED2(Boolean Set)
{
	ZwGPIO_WritePin(PIN_LED_2, Set);
}
// ----------------------------------------

void ZbGPIO_ToggleLED1()
{
	ZwGPIO_TogglePin(PIN_LED_1);
}
// ----------------------------------------

void ZbGPIO_T_DUT_CS(Boolean Set)
{
	ZwGPIO_WritePin(PIN_T_DUT_CS, Set);
}
// ----------------------------------------

void ZbGPIO_T_COOL_CS(Boolean Set)
{
	ZwGPIO_WritePin(PIN_T_COOL_CS, Set);
}
// ----------------------------------------

void ZbGPIO_IMC_CS(Boolean Set)
{
	ZwGPIO_WritePin(PIN_IMC_CS, Set);
}
// ----------------------------------------

void ZbGPIO_IHC_CS(Boolean Set)
{
	ZwGPIO_WritePin(PIN_IHC_CS, Set);
}
// ----------------------------------------

void ZbGPIO_DAC_LDAC(Boolean Set)
{
	ZwGPIO_WritePin(PIN_DAC_LDAC, Set);
}
// ----------------------------------------

void ZbGPIO_SYNC(Boolean Set)
{
	ZwGPIO_WritePin(PIN_SYNC, Set);
}
// ----------------------------------------

void ZbGPIO_DateDrv_LDAC(Boolean Set)
{
	ZwGPIO_WritePin(PIN_GATE_DRV_LDAC, Set);
}
// ----------------------------------------

void ZbGPIO_MCB_LDAC(Boolean Set)
{
	ZwGPIO_WritePin(PIN_MCB_LDAC, Set);
}
// ----------------------------------------

void ZbGPIO_ControlMCB(Boolean Set)
{
	ZwGPIO_WritePin(PIN_MCB_SYNC, Set);
}
// ----------------------------------------
// No more.
