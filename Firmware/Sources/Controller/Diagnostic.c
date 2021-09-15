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


// Functions
//
Boolean DIAG_Process(Int16U Command)
{
	switch (Command)
	{
		case ACT_DBG_READ_IM:
			DataTable[REG_DBG] = ZthSB_RawReadIm();
			break;

		case ACT_DBG_READ_IH:
			DataTable[REG_DBG] = ZthSB_RawReadIh();
			break;

		case ACT_DBG_READ_TSP:
			DataTable[REG_DBG] = ZthSB_RawReadTSP();
			break;

		case ACT_DBG_READ_TCASE1:
			DataTable[REG_DBG] = ZthSB_RawReadTcase1();
			break;

		case ACT_DBG_READ_TCASE2:
			DataTable[REG_DBG] = ZthSB_RawReadTcase2();
			break;

		case ACT_DBG_READ_TCOOL1:
			DataTable[REG_DBG] = ZthSB_RawReadTcool1();
			break;

		case ACT_DBG_READ_TCOOL2:
			DataTable[REG_DBG] = ZthSB_RawReadTcool2();
			break;

		case ACT_DBG_READ_PROTECT_DATA:
			DataTable[REG_DBG] = ZthPB_FaultCheck();
			break;

		case ACT_DBG_GATE_CURRENT:
			ZthDCB_SwitchOutput(CURRENT_SOURCE);
			ZbDAC_Write((DataTable[REG_DBG] & DAC_WRITE_A), &ZbGPIO_DCB_LDAC);
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
			ZbDAC_Write((DataTable[REG_DBG] & DAC_WRITE_A), &ZbGPIO_MCB_LDAC);
			ZthMCB_CurrentControl(TRUE);
			DELAY_US(10000);
			ZthMCB_CurrentControl(FALSE);
			break;

		case ACT_DBG_REG:
			ZbDAC_Write(DataTable[REG_DBG], &ZbGPIO_RegisterRCLK);
			DELAY_US(10000);
			ZbDAC_Write(DataTable[REG_DBG], &ZbGPIO_RegisterRCLK);
			break;

		case ACT_DBG_LPS_CTRL:
			ZbGPIO_LowPowerSupplyControl(DataTable[REG_DBG]);
			break;

		case ACT_DBG_SYNC_OSC:
			ZbGPIO_SyncOscilloscope(TRUE);
			DELAY_US(1000);
			ZbGPIO_SyncOscilloscope(FALSE);
			break;

		default:
			return FALSE;
	}

	return TRUE;
}
// ----------------------------------------
