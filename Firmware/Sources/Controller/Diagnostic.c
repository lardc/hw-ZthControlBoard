// Header
//
#include "Diagnostic.h"
//
#include "Controller.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "ZbBoard.h"
#include "ZthDUTControlBoard.h"
#include "ZthMCurrentBoard.h"
#include "ZthSensingBoard.h"
#include "ZthProtectionBoard.h"
#include "ZbGPIO.h"
#include "ConvertUtils.h"

// Functions prototypes
//
void DIAG_SaveToRegisters(_iq Data);


// Functions
//
Boolean DIAG_Process(Int16U Command)
{
	volatile _iq Result;

	switch (Command)
	{
		case ACT_DBG_READ_IM:
			DIAG_SaveToRegisters(MEASURE_Im());
			break;

		case ACT_DBG_READ_IH:
			DIAG_SaveToRegisters(MEASURE_Ih());
			break;

		case ACT_DBG_READ_TSP:
			DIAG_SaveToRegisters(MEASURE_TSP());
			break;

		case ACT_DBG_READ_TCASE1:
			DIAG_SaveToRegisters(MEASURE_Tcase1());
			break;

		case ACT_DBG_READ_TCASE2:
			DIAG_SaveToRegisters(MEASURE_Tcase2());
			break;

		case ACT_DBG_READ_TCOOL1:
			DIAG_SaveToRegisters(MEASURE_Tcool1());
			break;

		case ACT_DBG_READ_TCOOL2:
			DIAG_SaveToRegisters(MEASURE_Tcool2());
			break;

		case ACT_DBG_READ_PROTECT_DATA:
			DataTable[REG_DBG] = ZthPB_FaultCheck();
			break;

		case ACT_DBG_GATE_CURRENT:
			ZthDCB_SwitchOutput(CURRENT_SOURCE);
			ZbDAC_Write((DataTable[REG_DBG] | DAC_WRITE_A), &ZbGPIO_DCB_LDAC, FALSE);
			break;

		case ACT_DBG_GATE_VOLTAGE_15V:
			ZthDCB_SwitchOutput(VOLTAGE_SOURCE);
			ZthDCB_VoltageSet(GATE_VOLTGE_15V);
			break;

		case ACT_DBG_GATE_VOLTAGE_20V:
			ZthDCB_SwitchOutput(VOLTAGE_SOURCE);
			ZthDCB_VoltageSet(GATE_VOLTGE_20V);
			break;

		case ACT_DBG_IM:
			ZbDAC_Write((DataTable[REG_DBG] | DAC_WRITE_A), &ZbGPIO_MCB_LDAC, FALSE);
			break;

		case ACT_DBG_REG:
			ZbDAC_Write(DataTable[REG_DBG], &ZbGPIO_RegisterRCLK, TRUE);
			DELAY_US(10000);
			ZbDAC_Write(BIT15, &ZbGPIO_RegisterRCLK, TRUE);
			break;

		case ACT_DBG_LPS_CTRL:
			ZbGPIO_LowPowerSupplyControl(DataTable[REG_DBG]);
			break;

		case ACT_DBG_SYNC_OSC:
			ZbGPIO_SyncOscilloscope(TRUE);
			DELAY_US(1000);
			ZbGPIO_SyncOscilloscope(FALSE);
			break;

		case ACT_DBG_READ_PROTECTION:
			DataTable[REG_DBG] = ZthPB_FaultCheck();
			break;

		case ACT_IH_SET_RANGE:
			ZbGPIO_SB_Ih_Range(DataTable[REG_DBG]);
			CONVERT_IhSetRangeParams(DataTable[REG_DBG]);
			break;

		default:
			return FALSE;
	}

	return TRUE;
}
// ----------------------------------------

void DIAG_SaveToRegisters(_iq Data)
{
	DataTable[REG_DBG_WHOLE] = _IQint(Data);
	DataTable[REG_DBG_FRACTION] = _IQint(_IQmpy(Data - _IQI(DataTable[REG_DBG_WHOLE]), _IQI(10)));
}
// ----------------------------------------
