// -----------------------------------------
// Main logic library
// ----------------------------------------

// Header
#include "Logic.h"
//
// Includes
#include "ZwDSP.h"
#include "xCCICommon.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Regulator.h"
#include "IQmathLib.h"
#include "IQMathUtils.h"
#include "ConvertUtils.h"
#include "ZbGPIO.h"
#include "DRCU.h"
#include "Controller.h"
#include "HighLevelInterface.h"

// Definitions
//
#define PULSE_WIDTH_10MS		10000	// in us

// Variables
//
volatile LogicState LOGIC_State = LS_None;
volatile DRCUDeviceState LOGIC_ExtDeviceState;
volatile Int64U LOGIC_TimeCounter = 0, LOGIC_HeatingTimeCounter = 0, LOGIC_CollingTime = 0;
volatile Int64U Timeout;
//
Int16U LOGIC_DRCU_State;
volatile Int16U LOGIC_CoolingMode;
volatile Int32U LOGIC_Pause, LOGIC_PulseWidthMin, LOGIC_PulseWidthMax, LOGIC_MeasurementDelay, LOGIC_GraduationPeriodMin;
volatile _iq LOGIC_ImpulseCurrent, LOGIC_HeatingCurrent, LOGIC_Tmax, LOGIC_ZthPause;
//
volatile Int64U LOGIC_ActualPulseWidth = 0;
volatile Int64U LOGIC_ActualDelayWidth = 0;
//
volatile Boolean TimeFlag = FALSE;
//
#pragma DATA_SECTION(LOGIC_Values_TSP, "data_mem");
Int16U LOGIC_Values_TSP[VALUES_x_SIZE];
#pragma DATA_SECTION(LOGIC_Values_Tcase1, "data_mem");
Int16U LOGIC_Values_Tcase1[VALUES_x_SIZE];
#pragma DATA_SECTION(LOGIC_Values_Tcase2, "data_mem");
Int16U LOGIC_Values_Tcase2[VALUES_x_SIZE];
#pragma DATA_SECTION(LOGIC_Values_Tcool1, "data_mem");
Int16U LOGIC_Values_Tcool1[VALUES_x_SIZE];
#pragma DATA_SECTION(LOGIC_Values_Tcool2, "data_mem");
Int16U LOGIC_Values_Tcool2[VALUES_x_SIZE];
volatile Int16U LOGIC_Values_Counter = 0;
volatile Int16U EP_DataCounter = 0;
//

// Functions prototypes
//
Boolean LOGIC_HeatingProcess();
void LOGIC_MeasuringProcess();
Boolean LOGIC_CoolingProcess(Int64U Pause);
void LOGIC_StartTimeCounter(Int32U Time_us);
Boolean LOGIC_CheckTimeCounter();
void LOGIC_SaveData(CombinedData Sample);
void LOGIC_CalculateTimeInterval(volatile Int64U *TimeInterval);
Boolean LOGIC_IndependentProcesses(RegulatorSelector Selector, Int32U PulseWidth);
void LOGIC_HandleCommunicationError();

// Functions
//
Boolean LOGIC_MeasurementCurrentProcess()
{
	return LOGIC_IndependentProcesses(SelectIm, LOGIC_PulseWidthMax);
}
// ----------------------------------------

Boolean LOGIC_HeatingCurrentProcess()
{
	return LOGIC_IndependentProcesses(SelectIh, LOGIC_PulseWidthMax);
}
// ----------------------------------------

Boolean LOGIC_IndependentProcesses(RegulatorSelector Selector, Int32U PulseWidth)
{
	Boolean Result = FALSE;

	switch (LOGIC_State)
	{
	case LS_None:
		REGULATOR_Enable(Selector, TRUE);
		LOGIC_StartTimeCounter(PulseWidth);
		LOGIC_SetState(LS_IndependentProcesses);
		break;

	case LS_IndependentProcesses:
		if(LOGIC_CheckTimeCounter())
		{
			LOGIC_SetState(LS_None);
			Result = TRUE;
		}
		break;
	}

	return Result;
}
// ----------------------------------------

Boolean LOGIC_ZthSequencePulsesProcess()
{
	Boolean Result = FALSE;
	static volatile Int64U CoolingTimeTemp = 0;

	switch (LOGIC_State)
	{
		case LS_None:
			LOGIC_ActualPulseWidth = LOGIC_PulseWidthMin;
			LOGIC_SetState(LS_Heating);
			break;

		case LS_Heating:
			if(LOGIC_HeatingProcess())
				LOGIC_SetState(LS_Measuring);
			break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();

			if(LOGIC_ActualPulseWidth >= LOGIC_PulseWidthMax)
			{
				LOGIC_SetState(LS_None);
				Result = TRUE;
			}
			else
			{
				CoolingTimeTemp = _IQint(_IQdiv(_IQmpy(_IQI(LOGIC_ActualPulseWidth), LOGIC_ZthPause), 100)) - LOGIC_MeasurementDelay;
				LOGIC_SetState(LS_Cooling);
			}
			break;

		case LS_Cooling:
			if(LOGIC_CoolingProcess(CoolingTimeTemp))
			{
				LOGIC_CalculateTimeInterval(&LOGIC_ActualPulseWidth);
				LOGIC_SetState(LS_Heating);
			}
			break;
	}

	return Result;
}
// ----------------------------------------

Boolean LOGIC_ZthLongPulseProcess()
{
	Boolean Result = FALSE;
	static volatile Int64U CoolingTimeTemp = 0;

	switch (LOGIC_State)
	{
		case LS_None:
			LOGIC_ActualPulseWidth = LOGIC_PulseWidthMax;
			LOGIC_SetState(LS_Heating);
			break;

		case LS_Heating:
			if(LOGIC_HeatingProcess())
				LOGIC_SetState(LS_Measuring);
			break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();

			if(LOGIC_ActualDelayWidth >= TIME_DELAY_MAX)
			{
				LOGIC_SetState(LS_None);
				Result = TRUE;
			}
			else
			{
				CoolingTimeTemp = LOGIC_ActualDelayWidth - LOGIC_MeasurementDelay;
				LOGIC_SetState(LS_Cooling);
			}
			break;

		case LS_Cooling:
			if(LOGIC_CoolingProcess(CoolingTimeTemp))
			{
				LOGIC_CalculateTimeInterval(&LOGIC_ActualDelayWidth);
				LOGIC_SetState(LS_Measuring);
			}
			break;
	}

	return Result;
}
// ----------------------------------------

Boolean LOGIC_RthSequenceProcess()
{
	Boolean Result = FALSE;
	static volatile Int64U CoolingTimeTemp = 0;

	switch (LOGIC_State)
	{
		case LS_None:
			LOGIC_ActualPulseWidth = LOGIC_PulseWidthMax;
			LOGIC_SetState(LS_Heating);
			break;

		case LS_Heating:
			if(LOGIC_HeatingProcess())
				LOGIC_SetState(LS_Measuring);
			break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();
			CoolingTimeTemp = LOGIC_Pause - LOGIC_MeasurementDelay;
			LOGIC_SetState(LS_Cooling);
			break;

		case LS_Cooling:
			if(LOGIC_CoolingProcess(CoolingTimeTemp))
				LOGIC_SetState(LS_Heating);
			break;
	}

	return Result;
}
// ----------------------------------------

Boolean LOGIC_Graduation()
{
	Boolean Result = FALSE;
	static volatile Int64U CoolingTimeTemp = 0;
	static Boolean HeatingProcess = FALSE;

	switch (LOGIC_State)
	{
		case LS_None:
			LOGIC_ActualPulseWidth = LOGIC_PulseWidthMax;
			HeatingProcess = TRUE;
			LOGIC_SetState(LS_Heating);
			break;

		case LS_Heating:
			if(LOGIC_HeatingProcess())
			{
				if((MEASURE_Tcase1() >= LOGIC_Tmax) || (MEASURE_Tcase2() >= LOGIC_Tmax))
					LOGIC_SetState(LS_Measuring);
				else
					LOGIC_SetState(LS_Cooling);
			}
			break;

		case LS_Cooling:
			if(HeatingProcess)
			{
				if(LOGIC_CoolingProcess(LOGIC_Pause))
					LOGIC_SetState(LS_Heating);
			}
			else
				if(LOGIC_CoolingProcess(CoolingTimeTemp))
					LOGIC_SetState(LS_Measuring);
			break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();
			CoolingTimeTemp = LOGIC_GraduationPeriodMin - LOGIC_MeasurementDelay;
			HeatingProcess = FALSE;
			LOGIC_SetState(LS_Cooling);
			break;
	}

	return Result;
}
// ----------------------------------------

Boolean LOGIC_HeatingProcess()
{
	static Boolean HeatingState = FALSE;

	if(!HeatingState)
	{
		LOGIC_TimeCounter = 0;
		HeatingState = TRUE;
	}

	if(LOGIC_ActualPulseWidth <= LOGIC_TimeCounter)
		HeatingState = FALSE;

	LOGIC_Heating(HeatingState);

	return !HeatingState;
}
// ----------------------------------------

void LOGIC_MeasuringProcess()
{
	LOGIC_StartTimeCounter(LOGIC_MeasurementDelay);
	while(LOGIC_CheckTimeCounter()){}

	LOGIC_SaveData(MEASURE_CombinedData(LOGIC_CoolingMode));
}
// ----------------------------------------

Boolean LOGIC_CoolingProcess(Int64U Pause)
{
	static Boolean DelayStartFlag = FALSE;

	if(!DelayStartFlag)
	{
		DelayStartFlag = TRUE;
		LOGIC_StartTimeCounter(Pause);
	}
	else
	{
		if(LOGIC_CheckTimeCounter())
			DelayStartFlag = FALSE;
	}

	return !DelayStartFlag;
}
// ----------------------------------------

void LOGIC_Heating(Boolean State)
{
	if(State)
	{
		if(LOGIC_ActualPulseWidth < PULSE_WIDTH_10MS)
			REGULATOR_Update(SelectIh, LOGIC_ImpulseCurrent);
		else
			REGULATOR_Update(SelectIh, LOGIC_HeatingCurrent);
	}
	else
		REGULATOR_Update(SelectIh, 0);

	REGULATOR_Enable(SelectIh, State);
	REGULATOR_Enable(SelectP, State);
}
// ----------------------------------------

void LOGIC_SaveData(CombinedData Sample)
{
	Int16U TSP, Tcase1, Tcase2, Tcool1, Tcool2;

	TSP = _IQint(Sample.TSP);
	Tcase1 = _IQint(_IQmpy(Sample.Tcase1, 100));
	Tcase2 = _IQint(_IQmpy(Sample.Tcase2, 100));
	Tcool1 = _IQint(_IQmpy(Sample.Tcool1, 100));
	Tcool2 = _IQint(_IQmpy(Sample.Tcool2, 100));

	// Save to endpoints
	if(LOGIC_Values_Counter < VALUES_x_SIZE)
	{
		LOGIC_Values_TSP[EP_DataCounter]    = TSP;
		LOGIC_Values_Tcase1[EP_DataCounter] = Tcase1;
		LOGIC_Values_Tcase2[EP_DataCounter] = Tcase2;
		LOGIC_Values_Tcool1[EP_DataCounter] = Tcool1;
		LOGIC_Values_Tcool2[EP_DataCounter] = Tcool2;
		EP_DataCounter++;

		LOGIC_Values_Counter = EP_DataCounter;
	}

	// Save to ouput registers
	DataTable[REG_ACTUAL_TSP]   = TSP;
	DataTable[REG_ACTUAL_T_CASE1] = Tcase1;
	DataTable[REG_ACTUAL_T_CASE2] = Tcase2;
	DataTable[REG_ACTUAL_T_COOL1] = Tcool1;
	DataTable[REG_ACTUAL_T_COOL2] = Tcool2;
}
// ----------------------------------------

void LOGIC_CalculateTimeInterval(volatile Int64U *TimeInterval)
{
	Int64U Additive = 0;

	if(*TimeInterval < (Int64U)1e3)
		Additive = 100;						// 100us
	else if(*TimeInterval < (Int64U)10e3)
		Additive = 1e3;						// 1ms
	else if(*TimeInterval < (Int64U)100e3)
		Additive = 10e3;					// 10ms
	else if(*TimeInterval < (Int64U)1e6)
		Additive = 100e3;					// 100ms
	else if(*TimeInterval < (Int64U)10e6)
		Additive = 1e6;						// 1s
	else if(*TimeInterval < (Int64U)100e6)
		Additive = 10e6;					// 10s
	else
		Additive = 100e6;					// 100s

	*TimeInterval = *TimeInterval + Additive;
}
// ----------------------------------------

void LOGIC_StartTimeCounter(Int32U Time_us)
{
	TimeFlag = FALSE;

	ZwTimer_SetPeriodT2_us(Time_us);
	ZwTimer_StartT2();
}
// ----------------------------------------

Boolean LOGIC_CheckTimeCounter()
{
	return TimeFlag;
}

void LOGIC_SetState(LogicState State)
{
	LOGIC_State = State;
}
// ----------------------------------------

void LOGIC_IncTimeCounter()
{
	LOGIC_TimeCounter ++;
}
// ----------------------------------------

void LOGIC_SetDelayFlag()
{
	TimeFlag = TRUE;
}
// ----------------------------------------

void LOGIC_CacheVariables()
{
	LOGIC_PulseWidthMin = DataTable[REG_PULSE_WIDTH_MIN];
	//
	LOGIC_PulseWidthMax = DataTable[REG_PULSE_WIDTH_MAX_H];
	LOGIC_PulseWidthMax = (LOGIC_PulseWidthMax << 16) | DataTable[REG_PULSE_WIDTH_MAX_L];
	//
	LOGIC_ImpulseCurrent = _IQI(DataTable[REG_IMPULSE_CURRENT]);
	LOGIC_HeatingCurrent = _IQI(DataTable[REG_HEATING_CURRENT]);
	//
	LOGIC_MeasurementDelay = DataTable[REG_MEASUREMENT_DELAY];
	LOGIC_ZthPause = DataTable[REG_ZTH_PAUSE] * 10;
	LOGIC_Pause = DataTable[REG_PAUSE];
	LOGIC_GraduationPeriodMin = DataTable[REG_GRADUATION_PERIOD_MIN] * 1000000;
	//
	LOGIC_CoolingMode = DataTable[REG_COOLING_MODE];
	LOGIC_Tmax = DataTable[REG_T_MAX] * 10;

	// Reset variables to default states
	EP_DataCounter = 0;
}
// ----------------------------------------

void LOGIC_HeatingCurrentSetRange(_iq Current)
{
	if(_IQint(Current) <= DataTable[REG_IH_RANGE_THRESHOLD])
	{
		CONVERT_IhSetRangeParams(0);
		ZbGPIO_SB_Ih_Range(0);
	}
	else
	{
		ZbGPIO_SB_Ih_Range(1);
		CONVERT_IhSetRangeParams(1);
	}
}
// ----------------------------------------

void LOGIC_HandleCommunicationError()
{
	HLIError err = HLI_GetError();
	if(err.ErrorCode != ERR_NO_ERROR)
	{
		// Communiction error
		LOGIC_State = LS_Error;
		CONTROL_SwitchToFault(FAULT_PROTOCOL);
	}
}
// ----------------------------------------

Boolean LOGIC_UpdateDeviceState()
{
	return DRCU_UpdateState(REG_DRCU_EMULATE, REG_DRCU_NODE_ID, &LOGIC_DRCU_State);
}
// ----------------------------------------

void LOGIC_PowerOnSequence()
{
	if(LOGIC_State == LS_PON_DRCU)
	{
		if(!LOGIC_UpdateDeviceState())
		{
			LOGIC_HandleCommunicationError();
			return;
		}

		switch(LOGIC_State)
		{
			case LS_PON_DRCU:
				DRCU_PowerOn(REG_DRCU_EMULATE, REG_DRCU_NODE_ID, &LOGIC_ExtDeviceState, &LOGIC_State,
						FAULT_DRCU_PWRON, LS_WAIT_READY);

				Timeout = CONTROL_TimeCounter + TIME_POWER_ON;
				break;

			case LS_WAIT_READY:
				DRCU_WaitReady(CONTROL_TimeCounter, Timeout, LOGIC_ExtDeviceState, &LOGIC_State);
				break;
		}

		LOGIC_HandleCommunicationError();
	}
}
// ----------------------------------------

// No more.
