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
#include "ZthMCurrentBoard.h"
#include "ZthProtectionBoard.h"
#include "IQmathLib.h"
#include "IQMathUtils.h"
#include "ConvertUtils.h"
#include "Regulator.h"

// Definitions
//
#define MODE_ZTH_SEQ_PULSES		0
#define MODE_ZTH_LONG_PULSE		1
#define MODE_RTH_SEQ_PULSES		2
#define MODE_GRADUATION			3
#define MODE_IM					4
#define MODE_IH					5
#define MODE_IG					6
#define MODE_P					7

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile Int64U CONTROL_TimeCounter = 0;
static volatile Boolean CycleActive = FALSE, ReinitRS232 = FALSE;
volatile Int16U CONTROL_Mode;
// Boot-loader flag
#pragma DATA_SECTION(CONTROL_BootLoaderRequest, "bl_flag");
volatile Int16U CONTROL_BootLoaderRequest = 0;
//
static CONTROL_FUNC_RealTimeRoutine RealTimeRoutine = NULL;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_FillWPPartDefault();
void CONTROL_SwitchToReady();
void CONTROL_CañheVariables();
void CONTROL_PowerOnProcess();
void CONTROL_ModeSelect();
void CONTROL_MeasuringCurrentProcess(Boolean State);
void CONTROL_ResetOutputRegisters();
void CONTROL_Protection();
void CONTROL_ForceStopProcess();
Boolean CONTROL_ValidationParams();

// Functions
//
void CONTROL_Init(Boolean BadClockDetected)
{
	// Variables for endpoint configuration
	Int16U EPIndexes[EP_COUNT] = { EP_TSP, EP_T_CASE1, EP_T_CASE2, EP_T_COOL1, EP_T_COOL2, EP_ERR_IM, EP_ERR_IH, EP_ERR_P, EP_IM, EP_IH, EP_P};

	Int16U EPSized[EP_COUNT] = { VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE,
			VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE};

	pInt16U EPCounters[EP_COUNT] = { (pInt16U)&LOGIC_Values_Counter, (pInt16U)&LOGIC_Values_Counter,
			(pInt16U)&LOGIC_Values_Counter, (pInt16U)&LOGIC_Values_Counter, (pInt16U)&LOGIC_Values_Counter, (pInt16U)&REGULATOR_Im_Counter,
			(pInt16U)&REGULATOR_Ih_Counter, (pInt16U)&REGULATOR_P_Counter, (pInt16U)&REGULATOR_Im_Counter, (pInt16U)&REGULATOR_Ih_Counter,
			(pInt16U)&REGULATOR_P_Counter};

	pInt16U EPDatas[EP_COUNT] = { LOGIC_Values_TSP, LOGIC_Values_Tcase1, LOGIC_Values_Tcase2, LOGIC_Values_Tcool1, LOGIC_Values_Tcool2,
			REGULATOR_ErrorIm, REGULATOR_ErrorIh, REGULATOR_ErrorP, REGULATOR_Im_Value, REGULATOR_Ih_Value, REGULATOR_P_Value};

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
		CONTROL_SetDeviceState(DS_Disabled, LS_None);
	}

	CONTROL_CañheVariables();
	ZwADC_SubscribeToResults1(&MEASURE_CapVoltageSamplingResult);
	REGULATOR_ForceOutputsToZero();
}
// ----------------------------------------

void CONTROL_Idle()
{
	// Protection check
	CONTROL_Protection();

	// Power on processes
	CONTROL_PowerOnProcess();

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
				CONTROL_SetDeviceState(DS_PowerOn, LS_None);
			else if(CONTROL_State != DS_Ready)
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_DISABLE_POWER:
			if(CONTROL_State == DS_Ready)
			{
				ZbGPIO_LowPowerSupplyControl(FALSE);
				CONTROL_SetDeviceState(DS_None, LS_None);
			}
			else if(CONTROL_State != DS_None)
					*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_START_PROCESS:
			if (CONTROL_State == DS_Ready)
			{
				if(CONTROL_ValidationParams())
				{
					CONTROL_CañheVariables();
					CONTROL_ResetOutputRegisters();
					CONTROL_ModeSelect();
					CONTROL_SetDeviceState(DS_InProcess, LS_ConfigAll);
				}
				else
					*UserError = ERR_CONFIGURATION_LOCKED;
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
				CONTROL_SetDeviceState(DS_Ready, LS_None);
				CONTROL_StopProcess(OPRESULT_OK);
			}
			break;

		case ACT_STOP_HEATING:
			if ((CONTROL_State == DS_InProcess) && ((CONTROL_Mode == MODE_ZTH_LONG_PULSE) || (CONTROL_Mode == MODE_GRADUATION)))
			{
				CONTROL_SetDeviceState(DS_InProcess, LS_MeasurementDelay);
				LOGIC_TimeCounterReset();
				LOGIC_Heating(FALSE);
			}
			else
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_UPDATE:
			if(CONTROL_ValidationParams())
				LOGIC_UpdateParams();
			else
				*UserError = ERR_CONFIGURATION_LOCKED;
			break;

		case ACT_CLR_FAULT:
			if (CONTROL_State == DS_Fault)
			{
				CONTROL_SetDeviceState(DS_None, LS_None);
				DataTable[REG_FAULT_REASON] = FAULT_NONE;
			}
			break;

		case ACT_CLR_WARNING:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		case ACT_START_IM:
			if(CONTROL_State == DS_Ready)
			{
				CONTROL_CañheVariables();
				CONTROL_ResetOutputRegisters();
				CONTROL_ModeSelect();
				CONTROL_SetDeviceState(DS_InProcess, LS_ConfigIm);
			}
			break;

		case ACT_START_IH:
			if (CONTROL_State == DS_Ready)
			{
				CONTROL_CañheVariables();
				CONTROL_ResetOutputRegisters();
				CONTROL_ModeSelect();
				CONTROL_SetDeviceState(DS_InProcess, LS_ConfigIh);
			}
			else
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_START_GATE:
			if(CONTROL_State == DS_Ready)
			{
				CONTROL_CañheVariables();
				CONTROL_ResetOutputRegisters();
				CONTROL_ModeSelect();
				CONTROL_SetDeviceState(DS_InProcess, LS_ConfigIg);
			}
			break;

		default:
			if (CONTROL_State == DS_None)
			{
				CONTROL_CañheVariables();
				return DIAG_Process(ActionID);
			}
			else
				*UserError = ERR_OPERATION_BLOCKED;
	}

	return TRUE;
}
// ----------------------------------------

void CONTROL_ModeSelect()
{
	switch(CONTROL_Mode)
	{
		case MODE_IM:
			CONTROL_SubcribeToCycle(&LOGIC_MeasurementCurrentProcess);
			break;

		case MODE_IH:
			CONTROL_SubcribeToCycle(&LOGIC_HeatingCurrentProcess);
			break;

		case MODE_IG:
			CONTROL_SubcribeToCycle(&LOGIC_GateCurrentProcess);
			break;

		case MODE_ZTH_SEQ_PULSES:
			CONTROL_SubcribeToCycle(&LOGIC_ZthSequencePulsesProcess);
			break;

		case MODE_ZTH_LONG_PULSE:
			CONTROL_SubcribeToCycle(&LOGIC_ZthLongPulseProcess);
			break;

		case MODE_RTH_SEQ_PULSES:
			CONTROL_SubcribeToCycle(&LOGIC_RthSequenceProcess);
			break;

		case MODE_GRADUATION:
			CONTROL_SubcribeToCycle(&LOGIC_Graduation);
			break;
	}
}
// ----------------------------------------

void CONTROL_PowerOnProcess()
{
	if(CONTROL_State == DS_PowerOn)
		LOGIC_PowerOnSequence();
}
// ----------------------------------------

void CONTROL_UpdateSlow()
{
	MEASURE_CapVoltageSamplingStart();
}
// ----------------------------------------

void CONTROL_UpdateFast()
{
	if(RealTimeRoutine)
		RealTimeRoutine();
}
// ----------------------------------------

void CONTROL_SubcribeToCycle(CONTROL_FUNC_RealTimeRoutine Routine)
{
	RealTimeRoutine = Routine;
}
// ----------------------------------------

void CONTROL_CañheVariables()
{
	CONTROL_Mode = DataTable[REG_MODE];
	//
	LOGIC_CacheVariables();
	REGULATOR_CacheVariables();
	CONVERT_CacheVariables();
	MEASURE_VariablesPrepare();
	//
	LOGIC_ResetVariables();
	REGULATOR_ResetVariables();
}
// ----------------------------------------

Boolean CONTROL_ValidationParams()
{
	Boolean Result = TRUE;

	switch(DataTable[REG_MODE])
	{
		case MODE_ZTH_SEQ_PULSES:

			break;

		case MODE_ZTH_LONG_PULSE:

			break;

		case MODE_RTH_SEQ_PULSES:
			if((DataTable[REG_PAUSE] * 100) <= DataTable[REG_MEASUREMENT_DELAY])
				Result = FALSE;
			break;

		case MODE_GRADUATION:

			break;
	}

	return Result;
}
// ----------------------------------------

void CONTROL_StopProcess(Int16U OpResult)
{
	LOGIC_Heating(FALSE);
	REGULATOR_DisableAll();
	REGULATOR_ForceOutputsToZero();
	LOGIC_GatePulse(FALSE);
	CONTROL_SubcribeToCycle(NULL);

	DataTable[REG_OP_RESULT] = OpResult;
}
// ----------------------------------------

void CONTROL_ForceStopProcess()
{
	CONTROL_StopProcess(OPRESULT_FAIL);
	CONTROL_ResetOutputRegisters();
}
// ----------------------------------------

void CONTROL_Protection()
{
	Int16U Fault = ZthPB_FaultCheck();

	if((!DataTable[REG_MUTE_PROTECTION]) && (Fault != PB_FAULT_NONE) && (CONTROL_State == DS_InProcess))
	{
		switch(Fault)
		{
			case PB_FAULT_WATER:
				CONTROL_SwitchToFault(FAULT_WATER);
				break;

			case PB_FAULT_TR1:
				CONTROL_SwitchToFault(FAULT_TR1);
				break;

			case PB_FAULT_TR2:
				CONTROL_SwitchToFault(FAULT_TR2);
				break;

			case PB_FAULT_REC:
				CONTROL_SwitchToFault(FAULT_REC);
				break;
		}

		CONTROL_ForceStopProcess();
	}
}
// ----------------------------------------

void CONTROL_ResetOutputRegisters()
{
	DataTable[REG_OP_RESULT] = OPRESULT_NONE;
	DataTable[REG_ACTUAL_U_DUT] = 0;
	DataTable[REG_ACTUAL_I_DUT] = 0;
	DataTable[REG_ACTUAL_P_DUT_WHOLE] = 0;
	DataTable[REG_ACTUAL_P_DUT_FRACT] = 0;
	DataTable[REG_ACTUAL_P_TARGET_WHOLE] = 0;
	DataTable[REG_ACTUAL_P_TARGET_FRACT] = 0;
	DataTable[REG_ACTUAL_I_MEASUREMENT] = 0;
	DataTable[REG_ACTUAL_T_CASE1] = 0;
	DataTable[REG_ACTUAL_T_CASE2] = 0;
	DataTable[REG_ACTUAL_T_COOL1] = 0;
	DataTable[REG_ACTUAL_T_COOL2] = 0;
	DataTable[REG_ACTUAL_TSP] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
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

void CONTROL_SetDeviceState(DeviceState NewState, LogicState NewLogicState)
{
	// Set new state
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;

	LOGIC_SetState(NewLogicState);
}
// ----------------------------------------

void CONTROL_SwitchToReady()
{
	CONTROL_SetDeviceState(DS_Ready, LS_None);
}
// ----------------------------------------

void CONTROL_SwitchToFault(Int16U FaultReason)
{
	CONTROL_SetDeviceState(DS_Fault, LS_None);
	DataTable[REG_FAULT_REASON] = FaultReason;
}
// ----------------------------------------

void CONTROL_NotifyCANaFault(ZwCAN_SysFlags Flag)
{
	DEVPROFILE_NotifyCANaFault(Flag);
}
// ----------------------------------------

// No more.
