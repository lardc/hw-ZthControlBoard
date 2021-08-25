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

//Define
//

//


// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile IntDeviceState CONTROL_IntState = INTDS_None;
volatile Int64U CONTROL_TimeCounter = 0;
static volatile Boolean CycleActive = FALSE, ReinitRS232 = FALSE;
Int32U IhcPulseWidth_uS = 0;
Int32U MeasureAfterPulse_uS = 0;
Int32U IhcLastPulseWidth_uS = 0;
Int32U IhcDelayWidth_uS = 0;
Int32U GraduationTime = 0;
Int32U IhcLastDelayWidth_uS = 0;
Int32U IhcPulseTimeChange = 0;
Int32U IhcDelayTimeChange = 0;
Int16U* IhcPulseArray;
Int16U ReadyCounter = 0;
//
#pragma DATA_SECTION(CONTROL_Values_I, "data_mem");
Int16U CONTROL_Values_I[VALUES_x_SIZE];
#pragma DATA_SECTION(CONTROL_Values_U, "data_mem");
Int16U CONTROL_Values_U[VALUES_x_SIZE];
#pragma DATA_SECTION(CONTROL_Values_P, "data_mem");
Int16U CONTROL_Values_P[VALUES_x_SIZE];
#pragma DATA_SECTION(CONTROL_Values_Z, "data_mem");
Int16U CONTROL_Values_Z[VALUES_x_SIZE];
volatile Int16U CONTROL_Values_I_Counter = 0, CONTROL_Values_U_Counter = 0, CONTROL_Values_P_Counter = 0, CONTROL_Values_Z_Counter = 0;
//

// Boot-loader flag
#pragma DATA_SECTION(CONTROL_BootLoaderRequest, "bl_flag");
volatile Int16U CONTROL_BootLoaderRequest = 0;

// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_FillWPPartDefault();
void CONTROL_SwitchToReady();
void StartChargeCapacitors(void);
void TurnOnPowerSupply(Boolean Control);
void Measure_MainVoltage(void);
void Measure_T_Cooler(void);
void ZthModeWorks(void);

// Functions
//
void CONTROL_Init(Boolean BadClockDetected)
{
	// Variables for endpoint configuration
	Int16U EPIndexes[EP_COUNT] = { EP_I, EP_U, EP_P , EP_Z};

	Int16U EPSized[EP_COUNT] = { VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE};

	pInt16U EPCounters[EP_COUNT] = { (pInt16U)&CONTROL_Values_I_Counter, (pInt16U)&CONTROL_Values_U_Counter,
			(pInt16U)&CONTROL_Values_P_Counter, (pInt16U)&CONTROL_Values_Z_Counter};

	pInt16U EPDatas[EP_COUNT] = { CONTROL_Values_I, CONTROL_Values_U, CONTROL_Values_P, CONTROL_Values_Z};

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
		CONTROL_SetDeviceState(DS_Disabled);
	}

	CONFIG_LongTimeTimer1.TimerSet = &ZwTimer_SetT1;
	CONFIG_LongTimeTimer2.TimerSet = &ZwTimer_SetT2;
}
// ----------------------------------------



void CONTROL_Idle()
{
	// Charging capacitors after power up
	if (CONTROL_State == DS_PowerOn)
		StartChargeCapacitors();

	// Main voltage measurement process
	Measure_MainVoltage();

	// Cooler temperature measurement process
	Measure_T_Cooler();

	// Mode work change
	ZthModeWorks();

	// Process external interface requests
	DEVPROFILE_ProcessRequests();

	// Update CAN bus status
	DEVPROFILE_UpdateCANDiagStatus();
}
// ----------------------------------------

void ZthModeWorks(void)
{
	if(CONTROL_State == DS_InProcess)
	{
		switch(DataTable[REG_MODE])
		{
		case ZTH_MODE_MULTIPULSE:	{ZthModeMultiPulseProcess();	break;}
		case ZTH_MODE_SINGLEPULSE:	{ZthModeSinglePulseProcess();	break;}
		case ZTH_MODE_GRADUATION:	{ZthModeGraduationProcess();	break;}
		case ZTH_MODE_MANUAL:		{ZthModeManualProcess();		break;}
		}
	}
}
// ----------------------------------------

void Measure_T_Cooler(void)
{
	Int16U Value = 0;
	Int16U receivedValue = 0;

	ZbGPIO_T_COOL_CS(0);

	ZwSPId_BeginReceive((pInt16U)Value, sizeof(Value), SPI_RESOLUTION, STTNormal);
	while(ZwSPId_GetWordsToReceive() < sizeof(Value))
		DELAY_US(1);
	ZwSPId_EndReceive((pInt16U)&receivedValue, sizeof(Value));

	ZbGPIO_T_COOL_CS(1);

	DataTable[REG_T_COOL] = receivedValue;
}
// ----------------------------------------

void Measure_MainVoltage(void)
{
	pInt16U Results;

	ZwADC_StartSEQ1();
	DELAY_US(1000);
	Results = ZwADC_GetValues1();
	DataTable[REG_MAIN_VOLTAGE] = (*Results)*DataTable[REG_K_UBAT_TO_VOLTS];
}
// ----------------------------------------

void Start_Ihc(void)
{
	CONTROL_IntState = INTDS_Ihc_Inprocess;

	// Start timer for regulator
	ZwTimer_SetT1(TIMER1_PERIOD);
	ZwTimer_StartT1();

	// Start timer for control width pulse and delay
	ZwTimer_StartT2();
}
// ----------------------------------------

void Stop_Ihc(void)
{
	Set_MCB_GateDrv_DAC(0, &ZbGPIO_DAC_LDAC);

	CONTROL_IntState = INTDS_Delay_Inprocess;

	// Stop timer for regulator
	ZwTimer_StopT1();

	// Timer initialization for TSP measurement delay
	ZwTimer_SetT2(DataTable[REG_MEASURMENT_DELAY]);
	ZwTimer_StartT2();
}
// ----------------------------------------

void StartChargeCapacitors(void)
{
	TurnOnPowerSupply(TRUE);

	if(DataTable[REG_MAIN_VOLTAGE] == MAIN_VOLTAGE_MAX)
	{
		TurnOnPowerSupply(FALSE);
		CONTROL_SetDeviceState(DS_Ready);
	}
}
// ----------------------------------------

void TurnOnPowerSupply(Boolean Control)
{
	if(Control)
		ZwGPIO_WritePin(PIN_CHARGE_PS, TRUE);
	else
		ZwGPIO_WritePin(PIN_CHARGE_PS, FALSE);
}
// ----------------------------------------

void CONTROL_NotifyCANaFault(ZwCAN_SysFlags Flag)
{
	DEVPROFILE_NotifyCANaFault(Flag);
}
// ----------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	// Set new state
	CONTROL_State = NewState;
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
	CONTROL_SetDeviceState(DS_Ready);
}
// ----------------------------------------

void CONTROL_SwitchToFault(Int16U FaultReason, Int16U FaultReasonExt)
{
	CONTROL_SetDeviceState(DS_Fault);
	DataTable[REG_FAULT_REASON] = FaultReason;
}
// ----------------------------------------


static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if (CONTROL_State == DS_None)
				{
					CONTROL_SetDeviceState(DS_PowerOn);
				}
				else if (CONTROL_State != DS_PowerOn && CONTROL_State != DS_Ready)
					*UserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_DISABLE_POWER:
			{
				if (CONTROL_State == DS_Ready)
				{
					CONTROL_SetDeviceState(DS_None);
				}
				else if (CONTROL_State != DS_None)
					*UserError = ERR_DEVICE_NOT_READY;
			}
			break;

		case ACT_START:
			{
				if(CONTROL_State == DS_Ready)
				{
					CONFIG_IhcPulse();
					CONFIG_DUTControlBoard();
					CONFIG_MCurrentBoard();
					CONTROL_SetDeviceState(DS_InProcess);
				}
				else
					*UserError = ERR_DEVICE_NOT_READY;
			}
			break;


		case ACT_STOP:
			{
				CONTROL_SwitchToReady();
			}
			break;

		case ACT_CLR_FAULT:
			if (CONTROL_State == DS_Fault || CONTROL_State == DS_None)
			{
				CONTROL_FillWPPartDefault();

				if (CONTROL_State == DS_Fault)
				{
					CONTROL_SetDeviceState(DS_None);
				}
			}
			else
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_CLR_WARNING:
			DataTable[REG_WARNING] = 0;
			break;


		default:
			return FALSE;
	}

	return TRUE;
}
// ----------------------------------------

void Set_MCB_GateDrv_DAC(Int16U Data, void (*Source)(Boolean))
{
	ZbGPIO_SYNC(0);
	ZwSPIc_BeginReceive((pInt16U)Data, sizeof(Data), SPI_RESOLUTION, STTNormal);
	ZbGPIO_SYNC(1);

	Source(0);
	Source(1);
}
// ----------------------------------------

void SaveZthData(Int16U Data)
{
	CONTROL_Values_Z[CONTROL_Values_Z_Counter]=Data;
	CONTROL_Values_Z_Counter++;
}
// ----------------------------------------
// No more.
