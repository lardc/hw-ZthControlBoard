// Header
//
#include "ZthDUTControlBoard.h"

// Include
//
#include "ZbGPIO.h"
#include "ZwUtils.h"
#include "Global.h"
#include "ConvertUtils.h"

// Variable
//
DCB_OutputType	ZthDCBOutputType = VOLTAGE_SOURCE;

// Functions
//
void ZthDCB_CurrentSet(Int16U Current)
{
	if(ZthDCBOutputType == CURRENT_SOURCE)
		ZbDAC_Write(CONVERT_Ig(Current) & DAC_WRITE_A, &ZbGPIO_DCB_LDAC);
}
// ----------------------------------------

void ZthDCB_VoltageSet(DCB_GateVoltage Voltage)
{
	if(ZthDCBOutputType == VOLTAGE_SOURCE)
	{
		switch(Voltage)
		{
			case GATE_VOLTGE_0V:
				ZbDAC_Write(DAC_V_MIN_SET | DAC_WRITE_B, &ZbGPIO_DCB_LDAC);
				break;

			case GATE_VOLTGE_15V:
				ZbGPIO_IGBT_SetUGate15V();
				ZbDAC_Write((DAC_MAX_VALUE | DAC_WRITE_B), &ZbGPIO_DCB_LDAC);
				break;

			case GATE_VOLTGE_20V:
				ZbGPIO_IGBT_SetUGate20V();
				ZbDAC_Write((DAC_MAX_VALUE | DAC_WRITE_B), &ZbGPIO_DCB_LDAC);
				break;
		}
	}
}
// ----------------------------------------

void ZthDCB_SwitchOutput(OutputType Source)
{
	switch(Source)
	{
		case CURRENT_SOURCE:
			ZthDCBOutputType = CURRENT_SOURCE;
			ZbDAC_Write(DAC_V_MIN_SET | DAC_WRITE_B, &ZbGPIO_DCB_LDAC);
			break;

		case VOLTAGE_SOURCE:
			ZthDCBOutputType = VOLTAGE_SOURCE;
			ZbDAC_Write((DAC_V_MIN_SET & DAC_WRITE_A), &ZbGPIO_DCB_LDAC);
			break;
	}
}
// ----------------------------------------




