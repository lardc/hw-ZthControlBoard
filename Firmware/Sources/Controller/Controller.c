// -----------------------------------------
// Controller logic
// ----------------------------------------

// Header
#include "Controller.h"
//
// Includes
#include "SysConfig.h"
//
#include "ZbBoard.h"
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "SCCISlave.h"
#include "DeviceProfile.h"
#include "Constraints.h"
#include "Logic.h"
#include "MeasuringProcesses.h"
#include "Diagnostic.h"

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile DeviceSubState CONTROL_SubState = SS_None;
volatile Int64U CONTROL_TimeCounter = 0;
static volatile Boolean CycleActive = FALSE, ReinitRS232 = FALSE;
volatile Int16U CONTROL_Mode, CONTROL_DUTType, CONTROL_CoolingMode, CONTROL_ZthPulseWidthMin, CONTROL_ZthPulseWidthMax;
volatile Int16U CONTROL_ZthPause, CONTROL_PulseWidth, CONTROL_Pause, CONTROL_ImpulseCurrent, CONTROL_HeatingCurrent;
volatile Int16U CONTROL_GateCurrent, CONTROL_MeasuringCurrent, CONTROL_Delay, CONTROL_Tmax;

//
#pragma DATA_SECTION(CONTROL_Values_TSP, "data_mem");
Int16U CONTROL_Values_TSP[VALUES_x_SIZE];
#pragma DATA_SECTION(CONTROL_Values_Tcase1, "data_mem");
Int16U CONTROL_Values_Tcase1[VALUES_x_SIZE];
#pragma DATA_SECTION(CONTROL_Values_Tcase2, "data_mem");
Int16U CONTROL_Values_Tcase2[VALUES_x_SIZE];
#pragma DATA_SECTION(CONTROL_Values_Tcool1, "data_mem");
Int16U CONTROL_Values_Tcool1[VALUES_x_SIZE];
#pragma DATA_SECTION(CONTROL_Values_Tcool2, "data_mem");
Int16U CONTROL_Values_Tcool2[VALUES_x_SIZE];
volatile Int16U CONTROL_Values_Counter = 0;
//

// Boot-loader flag
#pragma DATA_SECTION(CONTROL_BootLoaderRequest, "bl_flag");
volatile Int16U CONTROL_BootLoaderRequest = 0;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_FillWPPartDefault();
void CONTROL_SwitchToReady();
void CONTROL_CashVariables();

// Functions
//
void CONTROL_Init(Boolean BadClockDetected)
{
	// Variables for endpoint configuration
	Int16U EPIndexes[EP_COUNT] = { EP_TSP, EP_T_CASE1, EP_T_CASE2, EP_T_COOL1, EP_T_COOL2};

	Int16U EPSized[EP_COUNT] = { VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE};

	pInt16U EPCounters[EP_COUNT] = { (pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter,
			(pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter};

	pInt16U EPDatas[EP_COUNT] = { CONTROL_Values_TSP, CONTROL_Values_Tcase1, CONTROL_Values_Tcase2, CONTROL_Values_Tcool1, CONTROL_Values_Tcool2};

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
	// Process external interface requests
	DEVPROFILE_ProcessRequests();

	// Update CAN bus status
	DEVPROFILE_UpdateCANDiagStatus();
}
// ----------------------------------------

void CONTROL_CashVariables()
{
	CONTROL_Mode = DataTable[REG_MODE];
	CONTROL_DUTType = DataTable[REG_DUT_TYPE];
	CONTROL_CoolingMode = DataTable[REG_COOLING_MODE];
	CONTROL_ZthPulseWidthMin = DataTable[REG_ZTH_PULSE_WIDTH_MIN];
	CONTROL_ZthPulseWidthMax = DataTable[REG_ZTH_PULSE_WIDTH_MAX];
	CONTROL_ZthPause = REG_ZTH_PAUSE;
	CONTROL_PulseWidth = DataTable[REG_PULSE_WIDTH];
	CONTROL_Pause = DataTable[REG_PAUSE];
	CONTROL_ImpulseCurrent = DataTable[REG_IMPULSE_CURRENT];
	CONTROL_HeatingCurrent = DataTable[REG_HEATING_CURRENT];
	CONTROL_GateCurrent = DataTable[REG_GATE_CURRENT];
	CONTROL_MeasuringCurrent = DataTable[REG_MEASURING_CURRENT];
	CONTROL_Delay = DataTable[REG_DELAY];
	CONTROL_Tmax = DataTable[REG_T_MAX];
}
// ----------------------------------------

void CONTROL_UpdateLow()
{
	// Update capacitor state
	DataTable[REG_ACTUAL_CAP_VOLTAGE] = MEASURE_CapVoltageSamplingResult();
	MEASURE_CapVoltageSamplingStart();
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
	CONTROL_SubState = NewSubState;
	DataTable[REG_DEV_STATE] = NewState;
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

void CONTROL_SwitchToFault(Int16U FaultReason, Int16U FaultReasonExt)
{
	CONTROL_SetDeviceState(DS_Fault, SS_None);
	DataTable[REG_FAULT_REASON] = FaultReason;
}
// ----------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_ENABLE_POWER:
			if(CONTROL_State == DS_None)
				CONTROL_SetDeviceState(DS_InProcess, SS_None);
			else if(CONTROL_State != DS_Ready)
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_DISABLE_POWER:
			if(CONTROL_State == DS_Ready)
			{
				CONTROL_SetDeviceState(DS_None, SS_None);
			}
			else if(CONTROL_State != DS_None)
					*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_START_PROCESS:
			if (CONTROL_State == DS_Ready)
			{
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
			if (CONTROL_State == DS_InProcess)
				CONTROL_SetDeviceState(DS_InProcess, SS_None);
			else
				*UserError = ERR_OPERATION_BLOCKED;
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

// No more.
