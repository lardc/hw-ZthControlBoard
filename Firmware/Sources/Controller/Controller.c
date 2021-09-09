// -----------------------------------------
// Controller logic
// ----------------------------------------

// Header
#include "Controller.h"
//
// Includes
#include "SysConfig.h"
//
#include "Global.h"
#include "ZbBoard.h"
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "SCCISlave.h"
#include "DeviceProfile.h"
#include "Constraints.h"
#include "Diagnostic.h"
#include "Regulator.h"
#include "ZthDUTControlBoard.h"
#include "ZthMCurrentBoard.h"
#include "IQmathLib.h"
#include "IQMathUtils.h"

// Definitions
//
#define MODE_ZTH_SEQ_PULSES		0
#define MODE_ZTH_LONG_PULSE		1
#define MODE_RTH_SEQ_PULSES		2
#define MODE_GRADUATION			3

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile Int64U CONTROL_TimeCounter = 0;
static volatile Boolean CycleActive = FALSE, ReinitRS232 = FALSE;
volatile Int16U CONTROL_Mode, CONTROL_DUTType;
volatile _iq CONTROL_GateCurrent, CONTROL_MeasuringCurrent, CONTROL_GateVoltage;
volatile Int64U CONTROL_PowerOnTimeOut = 0;
// Boot-loader flag
#pragma DATA_SECTION(CONTROL_BootLoaderRequest, "bl_flag");
volatile Int16U CONTROL_BootLoaderRequest = 0;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_FillWPPartDefault();
void CONTROL_SwitchToReady();
void CONTROL_CashVariables();
void CONTROL_LowPowerSupplyControl(Boolean State);
void CONTROL_CapacitorsVoltageControl();
void CONTROL_Process();
void CONTROL_StopProcess();
void CONTROL_StartProcess();
void CONTROL_GatePulse(Boolean State);
void CONTROL_MeasuringCurrentProcess(Boolean State);

// Functions
//
void CONTROL_Init(Boolean BadClockDetected)
{
	// Variables for endpoint configuration
	Int16U EPIndexes[EP_COUNT] = { EP_TSP, EP_T_CASE1, EP_T_CASE2, EP_T_COOL1, EP_T_COOL2};

	Int16U EPSized[EP_COUNT] = { VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE};

	pInt16U EPCounters[EP_COUNT] = { (pInt16U)&LOGIC_Values_Counter, (pInt16U)&LOGIC_Values_Counter,
			(pInt16U)&LOGIC_Values_Counter, (pInt16U)&LOGIC_Values_Counter, (pInt16U)&LOGIC_Values_Counter};

	pInt16U EPDatas[EP_COUNT] = { LOGIC_Values_TSP, LOGIC_Values_Tcase1, LOGIC_Values_Tcase2, LOGIC_Values_Tcool1, LOGIC_Values_Tcool2};

	// Data-table EPROM service configuration
	EPROMServiceConfig EPROMService = { &ZbMemory_WriteValuesEPROM, &ZbMemory_ReadValuesEPROM };

	// Init data table
	DT_Init(EPROMService, BadClockDetected);
	// Fill state variables with default values
	CONTROL_FillWPPartDefault();

	// Device profile initialization
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);

	// Reset control values
	DEVPROFILE_ResetControlSection();

	if(!BadClockDetected)
	{
		if(ZwSystem_GetDogAlarmFlag())
		{
			DataTable[REG_WARNING] = WARNING_WATCHDOG_RESET;
			ZwSystem_ClearDogAlarmFlag();
		}
	}
	else
	{
		DataTable[REG_DISABLE_REASON] = DISABLE_BAD_CLOCK;
		CONTROL_SetDeviceState(DS_Disabled, SS_None);
	}
}
// ----------------------------------------

void CONTROL_Idle()
{
	// Measurement process
	CONTROL_Process();

	// Process external interface requests
	DEVPROFILE_ProcessRequests();

	// Update CAN bus status
	DEVPROFILE_UpdateCANDiagStatus();
}
// ----------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_ENABLE_POWER:
			if(CONTROL_State == DS_None)
			{
				CONTROL_LowPowerSupplyControl(TRUE);
				CONTROL_PowerOnTimeOut = CONTROL_TimeCounter + TIME_POWER_ON;

				CONTROL_SetDeviceState(DS_PowerOn, SS_None);
			}
			else if(CONTROL_State != DS_Ready)
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_DISABLE_POWER:
			if(CONTROL_State == DS_Ready)
			{
				CONTROL_LowPowerSupplyControl(FALSE);
				CONTROL_SetDeviceState(DS_None, SS_None);
			}
			else if(CONTROL_State != DS_None)
					*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_START_PROCESS:
			if (CONTROL_State == DS_Ready)
			{
				CONTROL_StartProcess();
				CONTROL_SetDeviceState(DS_InProcess, SS_None);
			}
			else
				if (CONTROL_State == DS_InProcess)
					*UserError = ERR_OPERATION_BLOCKED;
				else
					*UserError = ERR_DEVICE_NOT_READY;
			break;

		case ACT_STOP_PROCESS:
			if (CONTROL_State == DS_InProcess)
			{
				CONTROL_StopProcess();
				CONTROL_SetDeviceState(DS_Ready, SS_None);
			}
			break;

		case ACT_STOP_HEATING:
			if (CONTROL_State == DS_InProcess)
				CONTROL_SetDeviceState(DS_Ready, SS_None);
			else
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_UPDATE:
				CONTROL_CashVariables();
			break;

		case ACT_CLR_FAULT:
			if (CONTROL_State == DS_Fault)
			{
				CONTROL_SetDeviceState(DS_None, SS_None);
				DataTable[REG_FAULT_REASON] = FAULT_NONE;
			}
			break;

		case ACT_CLR_WARNING:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		default:
			if (CONTROL_State == DS_None)
				return DIAG_Process(ActionID);
			else
				*UserError = ERR_OPERATION_BLOCKED;
	}

	return TRUE;
}
// ----------------------------------------

void CONTROL_Process()
{
	if(CONTROL_State == DS_InProcess)
	{
		switch(CONTROL_Mode)
		{
			case MODE_ZTH_SEQ_PULSES:
				LOGIC_ZthSequencePulsesProcess();
				break;

			case MODE_ZTH_LONG_PULSE:
				LOGIC_ZthLongPulseProcess();
				break;

			case MODE_RTH_SEQ_PULSES:
				LOGIC_RthSequenceProcess();
				break;

			case MODE_GRADUATION:
				LOGIC_Graduation();
				break;
		}
	}
}
// ----------------------------------------

void CONTROL_UpdateLow()
{
	CONTROL_CapacitorsVoltageControl();
}
// ----------------------------------------

void CONTROL_CapacitorsVoltageControl()
{
	Int16U Voltage;

	// Measuring capacitors voltage
	Voltage = MEASURE_CapVoltageSamplingResult();
	MEASURE_CapVoltageSamplingStart();

	if(CONTROL_State == DS_PowerOn)
	{
		if(Voltage >= DataTable[REG_CAP_VOLTAGE_THRESHOLD])
			CONTROL_SetDeviceState(DS_Ready, SS_None);
		else
		{
			if(CONTROL_TimeCounter >= CONTROL_PowerOnTimeOut)
			{
				CONTROL_LowPowerSupplyControl(FALSE);
				CONTROL_SwitchToFault(FAULT_POWERON);
			}
		}
	}

	DataTable[REG_ACTUAL_CAP_VOLTAGE] = Voltage;
}
// ----------------------------------------

void CONTROL_CashVariables()
{
	CONTROL_Mode = DataTable[REG_MODE];
	CONTROL_DUTType = DataTable[REG_DUT_TYPE];
	//
	CONTROL_MeasuringCurrent = _IQI(DataTable[REG_MEASURING_CURRENT]);
	CONTROL_GateCurrent = _IQI(DataTable[REG_GATE_CURRENT]);
	CONTROL_GateVoltage = _IQI(DataTable[REG_IGBT_VOLTAGE]);
	//
	LOGIC_CashVariables();
	REGULATOR_CashVariables();
}
// ----------------------------------------

void CONTROL_StartProcess()
{
	REGULATOR_InitAll();
	//
	CONTROL_GatePulse(TRUE);
	CONTROL_MeasuringCurrentProcess(TRUE);
}
// ----------------------------------------

void CONTROL_StopProcess()
{
	REGULATOR_DisableAll();
	REGULATOR_ForceOutputsToZero();
	CONTROL_MeasuringCurrentProcess(FALSE);
	CONTROL_GatePulse(FALSE);
}
// ----------------------------------------

void CONTROL_GatePulse(Boolean State)
{
	if(State)
	{
		if(CONTROL_DUTType)
		{
			ZthDCB_SwitchOutput(VOLTAGE_SOURCE);

			if(CONTROL_GateVoltage)
				ZthDCB_VoltageSet(GATE_VOLTGE_20V);
			else
				ZthDCB_VoltageSet(GATE_VOLTGE_15V);
		}
		else
		{
			ZthDCB_SwitchOutput(CURRENT_SOURCE);
			ZthDCB_CurrentSet(CONTROL_GateCurrent);
		}
	}
	else
	{
		ZthDCB_CurrentSet(0);
		ZthDCB_VoltageSet(GATE_VOLTGE_0V);
	}
}
// ----------------------------------------

void CONTROL_MeasuringCurrentProcess(Boolean State)
{
	REGULATOR_Update(SelectIm, CONTROL_MeasuringCurrent);
	REGULATOR_Enable(SelectIm, State);
	ZthMCB_CurrentSet(CONTROL_MeasuringCurrent);
	ZthMCB_CurrentControl(State);
}
// ----------------------------------------

void CONTROL_LowPowerSupplyControl(Boolean State)
{
	ZbGPIO_LowPowerSupplyControl(State);
}
// ----------------------------------------

void CONTROL_NotifyCANaFault(ZwCAN_SysFlags Flag)
{
	DEVPROFILE_NotifyCANaFault(Flag);
}
// ----------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState)
{
	// Set new state
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;

	LOGIC_SetState(NewSubState);
}
// ----------------------------------------

void CONTROL_FillWPPartDefault()
{
	// Set states
	DataTable[REG_DEV_STATE] = DS_None;
	DataTable[REG_FAULT_REASON] = FAULT_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
}
// ----------------------------------------

void CONTROL_SwitchToReady()
{
	CONTROL_SetDeviceState(DS_Ready, SS_None);
}
// ----------------------------------------

void CONTROL_SwitchToFault(Int16U FaultReason)
{
	CONTROL_SetDeviceState(DS_Fault, SS_None);
	DataTable[REG_FAULT_REASON] = FaultReason;
}
// ----------------------------------------

void CONTROL_SaveHeatingData(RegulatorsData Sample)
{
	DataTable[REG_ACTUAL_U_DUT]   = _IQint(Sample.U);
	DataTable[REG_ACTUAL_I_DUT] = _IQint(Sample.Ih);
	DataTable[REG_ACTUAL_P_DUT] = _IQint(_IQdiv(Sample.P, 1000));
	DataTable[REG_ACTUAL_I_MEASUREMENT] = _IQint(Sample.Im);
}
// ----------------------------------------

// No more.
