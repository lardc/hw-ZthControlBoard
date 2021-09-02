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
	//ZwGPIO_WritePin(PIN_U_GATE_SEL, FALSE);
	ZwGPIO_WritePin(PIN_MCB_SYNC, FALSE);
	ZwGPIO_WritePin(PIN_LED2, FALSE);
	ZwGPIO_WritePin(PIN_SB_IM_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_TSP_CS, TRUE);
	ZwGPIO_WritePin(PIN_DCB_LDAC, TRUE);
	ZwGPIO_WritePin(PIN_MCB_LDAC, TRUE);
	ZwGPIO_WritePin(PIN_SB_TK1_CS, TRUE);
	ZwGPIO_WritePin(PIN_PS_CTRL, FALSE);
	ZwGPIO_WritePin(PIN_LED1, FALSE);
	ZwGPIO_WritePin(PIN_MCB_DCB_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_TCOOL2_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_IH_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_TCOOL1_CS, TRUE);
	ZwGPIO_WritePin(PIN_SB_TK2_CS, TRUE);
	ZwGPIO_WritePin(PIN_EEPROM_CS, TRUE);
	ZwGPIO_WritePin(PIN_SYNC_OSC, FALSE);
	ZwGPIO_WritePin(PIN_REG_RCLK, FALSE);

   	// Configure pins to output
	ZwGPIO_PinToOutput(PIN_U_GATE_SEL);
	ZwGPIO_PinToOutput(PIN_MCB_SYNC);
	ZwGPIO_PinToOutput(PIN_LED2);
	ZwGPIO_PinToOutput(PIN_SB_IM_CS);
	ZwGPIO_PinToOutput(PIN_SB_TSP_CS);
	ZwGPIO_PinToOutput(PIN_DCB_LDAC);
	ZwGPIO_PinToOutput(PIN_MCB_LDAC);
	ZwGPIO_PinToOutput(PIN_SB_TK1_CS);
	ZwGPIO_PinToOutput(PIN_PS_CTRL);
	ZwGPIO_PinToOutput(PIN_LED1);
	ZwGPIO_PinToOutput(PIN_MCB_DCB_CS);
	ZwGPIO_PinToOutput(PIN_SB_TCOOL2_CS);
	ZwGPIO_PinToOutput(PIN_SB_IH_CS);
	ZwGPIO_PinToOutput(PIN_SB_TCOOL1_CS);
	ZwGPIO_PinToOutput(PIN_SB_TK2_CS);
	ZwGPIO_PinToOutput(PIN_EEPROM_CS);
	ZwGPIO_PinToOutput(PIN_SYNC_OSC);
	ZwGPIO_PinToOutput(PIN_REG_RCLK);

	// Configure pins to input
	ZwGPIO_PinToInput(PIN_T_TR2, FALSE, 0);
	ZwGPIO_PinToInput(PIN_T_REC, FALSE, 0);
	ZwGPIO_PinToInput(PIN_WATER, FALSE, 0);
	ZwGPIO_PinToInput(PIN_T_TR1, FALSE, 0);
}
// ----------------------------------------

void ZbGPIO_SwitchLED1(Boolean Set)
{
	ZwGPIO_WritePin(PIN_LED1, Set);
}
// ----------------------------------------

void ZbGPIO_SwitchLED2(Boolean Set)
{
	ZwGPIO_WritePin(PIN_LED2, Set);
}
// ----------------------------------------

void ZbGPIO_ToggleLED1()
{
	ZwGPIO_TogglePin(PIN_LED1);
}
// ----------------------------------------

void ZbGPIO_ToggleLED2()
{
	ZwGPIO_TogglePin(PIN_LED2);
}
// ----------------------------------------

void ZbGPIO_IGBT_SetUGate20V()
{
	ZwGPIO_WritePin(PIN_U_GATE_SEL, FALSE);
}
// ----------------------------------------

void ZbGPIO_IGBT_SetUGate15V()
{
	ZwGPIO_WritePin(PIN_U_GATE_SEL, TRUE);
}
// ----------------------------------------

void ZbGPIO_MCB_Sync(Boolean State)
{
	ZwGPIO_WritePin(PIN_MCB_SYNC, State);
}
// ----------------------------------------

void ZbGPIO_SB_Im_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_SB_IM_CS, State);
}
// ----------------------------------------

void ZbGPIO_SB_TSP_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_SB_TSP_CS, State);
}
// ----------------------------------------

void ZbGPIO_DCB_LDAC(Boolean State)
{
	ZwGPIO_WritePin(PIN_DCB_LDAC, State);
}
// ----------------------------------------

void ZbGPIO_MCB_LDAC(Boolean State)
{
	ZwGPIO_WritePin(PIN_MCB_LDAC, State);
}
// ----------------------------------------

void ZbGPIO_LowPowerSupplyControl(Boolean State)
{
	ZwGPIO_WritePin(PIN_PS_CTRL, State);
}
// ----------------------------------------

void ZbGPIO_DCB_MCB_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_MCB_DCB_CS, State);
}
// ----------------------------------------

void ZbGPIO_SB_Tcool1_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_SB_TCOOL1_CS, State);
}
// ----------------------------------------

void ZbGPIO_SB_Tcool2_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_SB_TCOOL2_CS, State);
}
// ----------------------------------------

void ZbGPIO_SB_Ih_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_SB_IH_CS, State);
}
// ----------------------------------------

void ZbGPIO_SB_Tc1_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_SB_TK2_CS, State);
}
// ----------------------------------------

void ZbGPIO_SB_Tc2_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_SB_TK2_CS, State);
}
// ----------------------------------------

void ZbGPIO_EEPROM_CS(Boolean State)
{
	ZwGPIO_WritePin(PIN_EEPROM_CS, State);
}
// ----------------------------------------

void ZbGPIO_SyncOscilloscope(Boolean State)
{
	ZwGPIO_WritePin(PIN_SYNC_OSC, State);
}
// ----------------------------------------

void ZbGPIO_RegisterRCLK(Boolean State)
{
	ZwGPIO_WritePin(PIN_REG_RCLK, State);
}
// ----------------------------------------

Boolean ZbGPIO_IsTransformer1Overheated()
{
	if(ZwGPIO_ReadPin(PIN_T_TR1))
		return TRUE;
	else
		return FALSE;
}
// ----------------------------------------

Boolean ZbGPIO_IsTransformer2Overheated()
{
	if(ZwGPIO_ReadPin(PIN_T_TR2))
		return TRUE;
	else
		return FALSE;
}
// ----------------------------------------

Boolean ZbGPIO_IsRectifierOverheated()
{
	return ZwGPIO_ReadPin(PIN_T_REC);
}
// ----------------------------------------

Boolean ZbGPIO_IsWater()
{
	return ZwGPIO_ReadPin(PIN_WATER);
}
// ----------------------------------------
// No more.
