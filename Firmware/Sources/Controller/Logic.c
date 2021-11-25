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
#include "ZthDUTControlBoard.h"

// Variables
//
volatile LogicState LOGIC_State = LS_None;
volatile DRCUDeviceState LOGIC_DRCU_State;
volatile Int64U LOGIC_TimeCounter = 0, LOGIC_HeatingTimeCounter = 0, LOGIC_CollingTime = 0;
volatile Int64U Timeout;
//
volatile Int16U LOGIC_DUTType;
volatile Int16U LOGIC_CoolingMode;
volatile Int32U LOGIC_Pause, LOGIC_PulseWidthMin, LOGIC_PulseWidthMax, LOGIC_MeasurementDelay, LOGIC_GraduationPeriodMin;
volatile _iq LOGIC_CurrentWidthLess_2ms, LOGIC_CurrentWidthLess_10ms, LOGIC_CurrentWidthAbove_10ms, LOGIC_Tmax, LOGIC_ZthPause;
volatile _iq LOGIC_MeasuringCurrent;
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
Boolean LOGIC_IndependentProcesses(Int32U PulseWidth);
void LOGIC_MeasuringCurrentConfig(_iq Current);
Boolean LOGIC_HeatingCurrentConfig(Int32U CuurentWidth);

// Functions
//
Boolean LOGIC_GateCurrentProcess()
{
	return LOGIC_IndependentProcesses(LOGIC_PulseWidthMax);
}
// ----------------------------------------

Boolean LOGIC_MeasurementCurrentProcess()
{
	return LOGIC_IndependentProcesses(LOGIC_PulseWidthMax);
}
// ----------------------------------------

Boolean LOGIC_HeatingCurrentProcess()
{
	return LOGIC_IndependentProcesses(LOGIC_PulseWidthMax);
}
// ----------------------------------------

Boolean LOGIC_IndependentProcesses(Int32U PulseWidth)
{
	Boolean Result = FALSE;

	switch (LOGIC_State)
	{
	case LS_ConfigIg:
		LOGIC_GatePulse(TRUE);
		LOGIC_StartTimeCounter(PulseWidth);
		LOGIC_SetState(LS_PendingCompletion);
		break;

	case LS_ConfigIm:
		LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
		LOGIC_StartTimeCounter(PulseWidth);
		LOGIC_SetState(LS_PendingCompletion);
		break;

	case LS_ConfigIh:
	case LS_DRCU_Config:
	case LS_DRCU_WaitReady:
		if(LOGIC_HeatingCurrentConfig(LOGIC_PulseWidthMax))
			LOGIC_SetState(LS_PendingCompletion);
		break;

	case LS_PendingCompletion:
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

void LOGIC_MeasuringCurrentConfig(_iq Current)
{
	REGULATOR_InitAll();
	REGULATOR_Update(SelectIm, Current);
	REGULATOR_Enable(SelectIm, TRUE);
}
// ----------------------------------------

Boolean LOGIC_HeatingCurrentConfig(Int32U CurrentWidth)
{
	Boolean Result = FALSE;
	static _iq CurrentSetpoint = 0, NewCurrentSetpoint = 0;

	if(CurrentWidth <= PULSE_WIDTH_2MS)
		CurrentSetpoint = LOGIC_CurrentWidthLess_2ms;
	else
	{
		if(CurrentWidth > PULSE_WIDTH_10MS)
			CurrentSetpoint = LOGIC_CurrentWidthAbove_10ms;
		else
			CurrentSetpoint = LOGIC_CurrentWidthLess_10ms;
	}

	if(CurrentSetpoint != NewCurrentSetpoint)
	{
		CurrentSetpoint = NewCurrentSetpoint;

		switch(LOGIC_State)
		{
			case LS_DRCU_Config:
			case LS_DRCU_WaitReady:
				LOGIC_DRCUConfigProcess(CurrentSetpoint, LS_ConfigIh);
				break;

			case LS_ConfigIh:
				REGULATOR_InitAll();
				LOGIC_HeatingCurrentSetRange(CurrentSetpoint);
				REGULATOR_Update(SelectIh, CurrentSetpoint);
				REGULATOR_Enable(SelectIh, TRUE);

				Result = TRUE;
				break;
		}

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
				LOGIC_SetState(LS_BatteryPrepare);
			}
			break;

		case LS_BatteryPrepare:
			LOGIC_SetState(LS_Heating);
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
	static _iq CurrentSetpoint, NewCurrentSetpoint;

	if(State)
	{
		if(LOGIC_ActualPulseWidth <= PULSE_WIDTH_2MS)
			NewCurrentSetpoint = LOGIC_CurrentWidthLess_2ms;
		else
		{
			if(LOGIC_ActualPulseWidth > PULSE_WIDTH_10MS)
				NewCurrentSetpoint = LOGIC_CurrentWidthAbove_10ms;
			else
				NewCurrentSetpoint = LOGIC_CurrentWidthLess_10ms;
		}

		if(NewCurrentSetpoint != CurrentSetpoint)
		{
			CurrentSetpoint = NewCurrentSetpoint;

			LOGIC_HeatingCurrentSetRange(HeatingCurrentSetpoint);
			REGULATOR_Update(SelectIh, HeatingCurrentSetpoint);
		}
	}
	else
	{
		REGULATOR_Update(SelectIh, 0);
		REGULATOR_Update(SelectP, 0);
	}

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
	LOGIC_CurrentWidthLess_2ms = _IQI(DataTable[REG_I_WIDTH_LESS_2MS]);
	LOGIC_CurrentWidthLess_10ms = _IQI(DataTable[REG_I_WIDTH_LESS_10MS]);
	LOGIC_CurrentWidthAbove_10ms = _IQI(DataTable[REG_I_WIDTH_ABOVE_10MS]);
	//
	LOGIC_MeasurementDelay = DataTable[REG_MEASUREMENT_DELAY];
	LOGIC_ZthPause = DataTable[REG_ZTH_PAUSE] * 10;
	LOGIC_Pause = DataTable[REG_PAUSE];
	LOGIC_GraduationPeriodMin = DataTable[REG_GRADUATION_PERIOD_MIN] * 1000000;
	//
	LOGIC_CoolingMode = DataTable[REG_COOLING_MODE];
	LOGIC_Tmax = DataTable[REG_T_MAX] * 10;
	LOGIC_DUTType = DataTable[REG_DUT_TYPE];
	LOGIC_MeasuringCurrent = _IQI(DataTable[REG_MEASURING_CURRENT]);

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
	if(!LOGIC_UpdateDeviceState())
	{
		LOGIC_HandleCommunicationError();
		return;
	}

	switch(LOGIC_State)
	{
		case LS_DRCU_PwrOn:
			DRCU_PowerOn(REG_DRCU_EMULATE, REG_DRCU_NODE_ID, &LOGIC_DRCU_State, &LOGIC_State, FAULT_DRCU_PWRON, LS_DRCU_WaitReady);
			ZbGPIO_LowPowerSupplyControl(TRUE);
			Timeout = CONTROL_TimeCounter + TIME_POWER_ON;
			break;

		case LS_DRCU_WaitReady:
			DRCU_WaitReady(CONTROL_TimeCounter, Timeout, LOGIC_DRCU_State, &LOGIC_State, LS_BatteryPwrOn);
			break;

		case LS_BatteryPwrOn:
			{
				if(MEASURE_CapVoltage >= DataTable[REG_CAP_VOLTAGE_THRESHOLD])
					CONTROL_SetDeviceState(DS_Ready, LS_None);
				else
				{
					if(CONTROL_TimeCounter >= Timeout)
					{
						ZbGPIO_LowPowerSupplyControl(FALSE);
						CONTROL_SwitchToFault(FAULT_POWERON);
					}
				}
				break;
			}


	}

	LOGIC_HandleCommunicationError();
}
// ----------------------------------------

void LOGIC_PowerOffProcess()
{
	if(!LOGIC_UpdateDeviceState())
	{
		LOGIC_HandleCommunicationError();
		return;
	}

	ZbGPIO_LowPowerSupplyControl(FALSE);

	DRCU_PowerOff(REG_DRCU_EMULATE, REG_DRCU_NODE_ID, &LOGIC_DRCU_State, &LOGIC_State, FAULT_DRCU_WRONG_STATE, LS_None);
	LOGIC_HandleCommunicationError();
}
// ----------------------------------------

void LOGIC_ResetFaultProcess()
{
	DataTable[REG_FAULT_REASON] = FAULT_NONE;

	if(!LOGIC_UpdateDeviceState())
	{
		LOGIC_HandleCommunicationError();
		return;
	}

	DRCU_ResetFault(REG_DRCU_EMULATE, REG_DRCU_NODE_ID, LOGIC_DRCU_State, &LOGIC_State, LS_None);
	LOGIC_HandleCommunicationError();
}
// ----------------------------------------

void LOGIC_DRCUConfigProcess(_iq Current, LogicState NextState)
{
	if(!LOGIC_UpdateDeviceState())
	{
		LOGIC_HandleCommunicationError();
		return;
	}

	switch(LOGIC_State)
	{
		case LS_DRCU_Config:
			DRCU_Config(REG_DRCU_EMULATE, REG_DRCU_NODE_ID, &LOGIC_DRCU_State, _IQI(Current), &LOGIC_State, LS_DRCU_WaitReady);
			break;

		case LS_DRCU_WaitReady:
			DRCU_WaitReady(CONTROL_TimeCounter, Timeout, LOGIC_DRCU_State, &LOGIC_State, NextState);
			break;
	}

	LOGIC_HandleCommunicationError();
}
// ----------------------------------------

void LOGIC_GatePulse(Boolean State)
{
	if(State)
	{
		if(LOGIC_DUTType)
		{
			ZthDCB_SwitchOutput(VOLTAGE_SOURCE);

			if(DataTable[REG_IGBT_V_GATE])
				ZthDCB_VoltageSet(GATE_VOLTGE_20V);
			else
				ZthDCB_VoltageSet(GATE_VOLTGE_15V);
		}
		else
		{
			ZthDCB_SwitchOutput(CURRENT_SOURCE);
			ZthDCB_CurrentSet(_IQI(DataTable[REG_GATE_CURRENT]));
		}
	}
	else
	{
		ZthDCB_CurrentSet(0);
		ZthDCB_VoltageSet(GATE_VOLTGE_0V);
	}
}
// ----------------------------------------

// No more.
