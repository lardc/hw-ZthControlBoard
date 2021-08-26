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
	ZwGPIO_WritePin(PIN_U_GATE_SEL, FALSE);
	ZwGPIO_WritePin(PIN_SYNC_IM, FALSE);
	ZwGPIO_WritePin(PIN_LED2, FALSE);
	ZwGPIO_WritePin(PIN_SB_IM_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_TSP_CS, TRUE);
	ZwGPIO_WritePin(PIN_DCB_LDAC, TRUE);
	ZwGPIO_WritePin(PIN_MCB_LDAC, TRUE);
	ZwGPIO_WritePin(PIN_SB_TK1_CS, TRUE);
	ZwGPIO_WritePin(PIN_PS_CTRL, FALSE);
	ZwGPIO_WritePin(PIN_LED1, FALSE);
	ZwGPIO_WritePin(PIN_MCB_DCB_SYNC, FALSE);
	ZwGPIO_WritePin(PIN_SB_TCOOL2_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_IH_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_TCOOL1_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_TK2_CS, TRUE);
	ZwGPIO_WritePin(PIN_MEM_CS, TRUE);
	ZwGPIO_WritePin(PIN_SYNC_OSC, FALSE);
	ZwGPIO_WritePin(PIN_REG_RCLK, FALSE);

   	// Configure pins to output
	ZwGPIO_PinToOutput(PIN_U_GATE_SEL);
	ZwGPIO_PinToOutput(PIN_SYNC_IM);
	ZwGPIO_PinToOutput(PIN_LED2);
	ZwGPIO_PinToOutput(PIN_SB_IM_CS);
	ZwGPIO_PinToOutput(PIN_SB_TSP_CS);
	ZwGPIO_PinToOutput(PIN_DCB_LDAC);
	ZwGPIO_PinToOutput(PIN_MCB_LDAC);
	ZwGPIO_PinToOutput(PIN_SB_TK1_CS);
	ZwGPIO_PinToOutput(PIN_PS_CTRL);
	ZwGPIO_PinToOutput(PIN_LED1);
	ZwGPIO_PinToOutput(PIN_MCB_DCB_SYNC);
	ZwGPIO_PinToOutput(PIN_SB_TCOOL2_CS);
	ZwGPIO_PinToOutput(PIN_SB_IH_CS);
	ZwGPIO_PinToOutput(PIN_SB_TCOOL1_CS);
	ZwGPIO_PinToOutput(PIN_SB_TK2_CS);
	ZwGPIO_PinToOutput(PIN_MEM_CS);
	ZwGPIO_PinToOutput(PIN_SYNC_OSC);
	ZwGPIO_PinToOutput(PIN_REG_RCLK);

	// Configure pins to input
	ZwGPIO_PinToInput(PIN_T_TR2);
	ZwGPIO_PinToInput(PIN_T_REC);
	ZwGPIO_PinToInput(PIN_WATER);
	ZwGPIO_PinToInput(PIN_T_TR1);
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
