// Header
//
#include "ZthDUTControlBoard.h"

// Include
//
#include "SysConfig.h"
#include "ZbBoard.h"
#include "Global.h"
#include "ConvertUtils.h"

// Variable
//
DCB_OutputType	ZthDCBOutputType = VOLTAGE_SOURCE;

// Functions
//
void ZthDCB_CurrentSet(_iq Current)
{
	if(ZthDCBOutputType == CURRENT_SOURCE)
		ZbDAC_Write(CONVERT_IgToDAC(Current) | DAC_WRITE_A, &ZbGPIO_DCB_LDAC, FALSE);
}
// ----------------------------------------

void ZthDCB_VoltageSet(DCB_GateVoltage Voltage)
{
	if(ZthDCBOutputType == VOLTAGE_SOURCE)
	{
		switch(Voltage)
		{
			case GATE_VOLTGE_0V:
				ZbDAC_Write(DAC_MIN_VALUE | DAC_WRITE_B, &ZbGPIO_DCB_LDAC, FALSE);
				break;

			case GATE_VOLTGE_15V:
				ZbGPIO_IGBT_SetUGate15V();
				ZbDAC_Write((DAC_MAX_VALUE | DAC_WRITE_B), &ZbGPIO_DCB_LDAC, FALSE);
				break;

			case GATE_VOLTGE_20V:
				ZbGPIO_IGBT_SetUGate20V();
				ZbDAC_Write((DAC_MAX_VALUE | DAC_WRITE_B), &ZbGPIO_DCB_LDAC, FALSE);
				break;
		}
	}
}
// ----------------------------------------

void ZthDCB_SwitchOutput(DCB_OutputType Source)
{
	switch(Source)
	{
		case CURRENT_SOURCE:
			ZthDCBOutputType = CURRENT_SOURCE;
			ZbDAC_Write(DAC_MIN_VALUE | DAC_WRITE_B, &ZbGPIO_DCB_LDAC, FALSE);
			break;

		case VOLTAGE_SOURCE:
			ZthDCBOutputType = VOLTAGE_SOURCE;
			ZbDAC_Write((DAC_MIN_VALUE & DAC_WRITE_A), &ZbGPIO_DCB_LDAC, FALSE);
			break;
	}
}
// ----------------------------------------




