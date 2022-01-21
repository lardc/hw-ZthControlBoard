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
#include "Controller.h"
#include "ZthDUTControlBoard.h"

// Variables
//
volatile LogicState LOGIC_State = LS_None;
volatile Int64U LOGIC_TimeCounter = 0, LOGIC_HeatingTimeCounter = 0, LOGIC_CollingTime = 0;
volatile Int64U Timeout = 0, LOGIC_TimeCounterMaxValue = 0;
//
volatile Int16U LOGIC_DUTType;
volatile Int16U LOGIC_CoolingMode;
volatile Int32U LOGIC_Pause, LOGIC_PulseWidthMin, LOGIC_PulseWidthMax, LOGIC_MeasurementDelay;
volatile _iq LOGIC_CurrentWidthLess_2ms, LOGIC_CurrentWidthLess_10ms, LOGIC_CurrentWidthAbove_10ms, LOGIC_Tmax, LOGIC_ZthPause;
volatile _iq LOGIC_MeasuringCurrent;
//
volatile Int64U LOGIC_ActualPulseWidth = 0;
volatile Int64U LOGIC_ActualDelayWidth = 0;
//
volatile Boolean TimeFlag = TRUE;
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
void LOGIC_IndependentProcesses();
void LOGIC_RegulatorProcess();
void LOGIC_SaveHeatingData(RegulatorsData Sample);
void LOGIC_MeasuringCurrentConfig(_iq Current);
void LOGIC_HeatingCurrentConfig(Int32U CuurentWidth);
Boolean LOGIC_BatteryVoltageControl(Int64U Timeout);
Boolean LOGIC_TimeCounterCheck(Int64U Time);
void LOGIC_MeasuringProcess();
void LOGIC_SaveData(CombinedData Sample);
void LOGIC_CalculateTimeInterval(volatile Int64U *TimeInterval);


// Functions
//
void LOGIC_GateCurrentProcess()
{
	LOGIC_IndependentProcesses();
}
// ----------------------------------------

void LOGIC_MeasurementCurrentProcess()
{
	LOGIC_IndependentProcesses();
}
// ----------------------------------------

void LOGIC_HeatingCurrentProcess()
{
	LOGIC_IndependentProcesses();
}
// ----------------------------------------

void LOGIC_IndependentProcesses()
{
	if(CONTROL_State == DS_InProcess)
	{
		switch (LOGIC_State)
		{
		case LS_ConfigIg:
			LOGIC_GatePulse(TRUE);
			LOGIC_TimeCounterReset();
			//
			LOGIC_SetState(LS_PendingCompletion);
			break;

		case LS_ConfigIm:
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			LOGIC_TimeCounterReset();
			//
			LOGIC_SetState(LS_PendingCompletion);
			break;

		case LS_ConfigIh:
			LOGIC_HeatingCurrentConfig(LOGIC_PulseWidthMax);
			LOGIC_Heating(TRUE);
			LOGIC_TimeCounterReset();
			//
			LOGIC_SetState(LS_PendingCompletion);
			break;

		case LS_PendingCompletion:
			LOGIC_RegulatorProcess();

			if(LOGIC_TimeCounterCheck(LOGIC_PulseWidthMax))
			{
				CONTROL_SetDeviceState(DS_Ready, LS_None);
				CONTROL_StopProcess(OPRESULT_OK);
			}
			break;
		}
	}
}
// ----------------------------------------

void LOGIC_ZthSequencePulsesProcess()
{
	static volatile Int64U CoolingTimeTemp = 0;

	switch (LOGIC_State)
	{
		case LS_None:
			ZbGPIO_OuputLock(FALSE);
			LOGIC_GatePulse(TRUE);
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			LOGIC_ActualPulseWidth = LOGIC_PulseWidthMin;
			LOGIC_SetState(LS_ConfigIh);
			break;

		case LS_ConfigIh:
			LOGIC_HeatingCurrentConfig(LOGIC_ActualPulseWidth);
			LOGIC_SetState(LS_Heating);
			break;

		case LS_Heating:
			//if(LOGIC_HeatingProcess())
				LOGIC_SetState(LS_Measuring);
			break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();

			if(LOGIC_ActualPulseWidth >= LOGIC_PulseWidthMax)
			{
				LOGIC_SetState(LS_None);
			}
			else
			{
				CoolingTimeTemp = _IQint(_IQdiv(_IQmpy(_IQI(LOGIC_ActualPulseWidth), LOGIC_ZthPause), 100)) - LOGIC_MeasurementDelay;
				LOGIC_SetState(LS_Cooling);
			}
			break;

		case LS_Cooling:
			/*if(LOGIC_CoolingProcess(CoolingTimeTemp))
			{
				LOGIC_CalculateTimeInterval(&LOGIC_ActualPulseWidth);
				Timeout = CONTROL_TimeCounter + TIME_BATTERY_RECHARGE;
				LOGIC_SetState(LS_BatteryPrepare);
			}*/
			break;

		case LS_BatteryPrepare:
			if(LOGIC_BatteryVoltageControl(Timeout))
				LOGIC_SetState(LS_ConfigIh);
			break;
	}
}
// ----------------------------------------

void LOGIC_ZthLongPulseProcess()
{
	static volatile Int64U CoolingTimeTemp = 0;

	switch (LOGIC_State)
	{
		case LS_None:
			ZbGPIO_OuputLock(FALSE);
			LOGIC_GatePulse(TRUE);
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			LOGIC_ActualPulseWidth = LOGIC_PulseWidthMax;
			LOGIC_SetState(LS_ConfigIh);
			break;

		case LS_ConfigIh:
			LOGIC_HeatingCurrentConfig(LOGIC_ActualPulseWidth);
			LOGIC_SetState(LS_Heating);
			break;

		case LS_Heating:
			//if(LOGIC_HeatingProcess())
				LOGIC_SetState(LS_Measuring);
			break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();

			if(LOGIC_ActualDelayWidth >= TIME_DELAY_MAX)
			{
				LOGIC_SetState(LS_None);
			}
			else
			{
				CoolingTimeTemp = LOGIC_ActualDelayWidth - LOGIC_MeasurementDelay;
				LOGIC_SetState(LS_Cooling);
			}
			break;

		case LS_Cooling:
			/*if(LOGIC_CoolingProcess(CoolingTimeTemp))
			{
				LOGIC_CalculateTimeInterval(&LOGIC_ActualDelayWidth);
				LOGIC_SetState(LS_Measuring);
			}*/
			break;
	}
}
// ----------------------------------------

void LOGIC_RthSequenceProcess()
{
	switch (LOGIC_State)
	{
		case LS_ConfigAll:
			LOGIC_GatePulse(TRUE);
			//
			LOGIC_HeatingCurrentConfig(LOGIC_PulseWidthMax);
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			//
			LOGIC_SetState(LS_StartHeating);
			break;

		case LS_Cooling:
			if(LOGIC_TimeCounterCheck(LOGIC_Pause))
				LOGIC_SetState(LS_Updating);
			else
				break;

		case LS_Updating:
			REGULATOR_Update(SelectIm, LOGIC_MeasuringCurrent);
			LOGIC_HeatingCurrentConfig(LOGIC_PulseWidthMax);

		case LS_StartHeating:
			LOGIC_TimeCounterReset();
			LOGIC_Heating(TRUE);
			LOGIC_SetState(LS_Heating);
			break;

		case LS_Heating:
			if(LOGIC_TimeCounterCheck(LOGIC_PulseWidthMax))
			{
				LOGIC_Heating(FALSE);
				//
				LOGIC_TimeCounterReset();
				LOGIC_SetState(LS_MeasurementDelay);
			}
			else
				break;

		case LS_MeasurementDelay:
			if(LOGIC_TimeCounterCheck(LOGIC_MeasurementDelay))
				LOGIC_SetState(LS_Measuring);
			else
				break;

		case LS_Measuring:
			ZwGPIO_WritePin(PIN_LED1, TRUE);
			LOGIC_MeasuringProcess();
			ZwGPIO_WritePin(PIN_LED1, FALSE);
			LOGIC_SetState(LS_Cooling);
			LOGIC_TimeCounter++;
			return;
	}

	LOGIC_RegulatorProcess();

	LOGIC_TimeCounter++;
}
// ----------------------------------------

void LOGIC_Graduation()
{
	static volatile Int64U CoolingTimeTemp = 0;
	static Boolean HeatingProcess = FALSE;

	switch (LOGIC_State)
	{
		case LS_None:
			ZbGPIO_OuputLock(FALSE);
			LOGIC_GatePulse(TRUE);
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			LOGIC_ActualPulseWidth = LOGIC_PulseWidthMax;
			LOGIC_SetState(LS_ConfigIh);
			break;

		case LS_ConfigIh:
			LOGIC_HeatingCurrentConfig(LOGIC_ActualPulseWidth);
			HeatingProcess = TRUE;
			LOGIC_SetState(LS_Heating);
			break;

		case LS_Heating:
			/*if(LOGIC_HeatingProcess())
			{
				if((MEASURE_Tcase1() >= LOGIC_Tmax) || (MEASURE_Tcase2() >= LOGIC_Tmax))
					LOGIC_SetState(LS_Measuring);
				else
					LOGIC_SetState(LS_Cooling);
			}*/
			break;

		case LS_Cooling:
			if(HeatingProcess)
			{
				//if(LOGIC_CoolingProcess(LOGIC_Pause))
					LOGIC_SetState(LS_Heating);
			}
			else
				//if(LOGIC_CoolingProcess(CoolingTimeTemp))
					LOGIC_SetState(LS_Measuring);
			break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();
			LOGIC_CalculateTimeInterval(&LOGIC_ActualDelayWidth);
			CoolingTimeTemp = LOGIC_ActualDelayWidth - LOGIC_MeasurementDelay;
			HeatingProcess = FALSE;
			LOGIC_SetState(LS_Cooling);
			break;
	}
}
// ----------------------------------------

void LOGIC_RegulatorProcess()
{
	RegulatorsData Sample;

	// Measurement process
	Sample = MEASURE_RegulatorsSample();

	// Regulator process
	REGULATOR_Cycle(Sample);

	// Save data to output regusters
	LOGIC_SaveHeatingData(Sample);
}
// ----------------------------------------

void LOGIC_SaveHeatingData(RegulatorsData Sample)
{
	DataTable[REG_ACTUAL_U_DUT]   = _IQint(_IQmpy(Sample.U, _IQI(10)));
	DataTable[REG_ACTUAL_I_DUT] = _IQint(_IQmpy(Sample.Ih, _IQI(10)));
	DataTable[REG_ACTUAL_P_DUT_WHOLE] = _IQint(Sample.P);
	DataTable[REG_ACTUAL_P_DUT_FRACT] = _IQint(_IQmpy((Sample.P - _IQI(DataTable[REG_ACTUAL_P_DUT_WHOLE])), _IQI(100)));
	DataTable[REG_ACTUAL_I_MEASUREMENT] = _IQint(_IQmpy(Sample.Im, _IQI(10)));
}
// ----------------------------------------

void LOGIC_TimeCounterReset()
{
	LOGIC_TimeCounter = 0;
}
// ----------------------------------------

Boolean LOGIC_TimeCounterCheck(Int64U Time)
{
	return (LOGIC_TimeCounter >= Time) ? TRUE : FALSE;
}
// ----------------------------------------

void LOGIC_MeasuringCurrentConfig(_iq Current)
{
	REGULATOR_Init(SelectIm);
	REGULATOR_Update(SelectIm, Current);
}
// ----------------------------------------

void LOGIC_HeatingCurrentConfig(Int32U CurrentWidth)
{
	static _iq CurrentSetpoint = 0;

	if(CurrentWidth <= PULSE_WIDTH_2MS)
		CurrentSetpoint = LOGIC_CurrentWidthLess_2ms;
	else
	{
		if(CurrentWidth > PULSE_WIDTH_10MS)
			CurrentSetpoint = LOGIC_CurrentWidthAbove_10ms;
		else
			CurrentSetpoint = LOGIC_CurrentWidthLess_10ms;
	}

	REGULATOR_Init(SelectIh);
	REGULATOR_Init(SelectP);
	LOGIC_HeatingCurrentSetRange(CurrentSetpoint);
	REGULATOR_Update(SelectIh, CurrentSetpoint);
}
// ----------------------------------------

void LOGIC_MeasuringProcess()
{
	LOGIC_SaveData(MEASURE_CombinedData(LOGIC_CoolingMode));
}
// ----------------------------------------

void LOGIC_Heating(Boolean State)
{
	ZbGPIO_OuputLock(!State);
	//
	REGULATOR_Enable(SelectIh, State);
	REGULATOR_Enable(SelectP, DataTable[REG_REGULATOR_POWER_CTRL]);
}
// ----------------------------------------

void LOGIC_SaveData(CombinedData Sample)
{
	Int16U TSP, Tcase1, Tcase2, Tcool1, Tcool2;

	TSP = _IQint(Sample.TSP);
	Tcase1 = _IQint(_IQmpy(Sample.Tcase1, 10));
	Tcase2 = _IQint(_IQmpy(Sample.Tcase2, 10));
	Tcool1 = _IQint(_IQmpy(Sample.Tcool1, 10));
	Tcool2 = _IQint(_IQmpy(Sample.Tcool2, 10));

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

void LOGIC_SetState(LogicState State)
{
	LOGIC_State = State;
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
	//
	LOGIC_CoolingMode = DataTable[REG_COOLING_MODE];
	LOGIC_Tmax = DataTable[REG_T_MAX] * 10;
	LOGIC_DUTType = DataTable[REG_DUT_TYPE];
	LOGIC_MeasuringCurrent = _IQI(DataTable[REG_MEASURING_CURRENT]);

	// Reset variables to default states
	EP_DataCounter = 0;
	LOGIC_ActualDelayWidth = 0;
}
// ----------------------------------------

void LOGIC_HeatingCurrentSetRange(_iq Current)
{
	if(_IQint(Current) <= DataTable[REG_IH_RANGE_THRESHOLD])
	{
		ZbGPIO_SB_Ih_Range(0);
		CONVERT_IhSetRangeParams(0);
	}
	else
	{
		ZbGPIO_SB_Ih_Range(1);
		CONVERT_IhSetRangeParams(1);
	}
}
// ----------------------------------------

void LOGIC_PowerOnSequence()
{
	switch(LOGIC_State)
	{
		case LS_None:
			ZbGPIO_LowPowerSupplyControl(TRUE);
			Timeout = CONTROL_TimeCounter + TIME_POWER_ON;
			CONTROL_SetDeviceState(DS_PowerOn, LS_BatteryPrepare);
			break;

		case LS_BatteryPrepare:
			if(LOGIC_BatteryVoltageControl(Timeout))
				CONTROL_SetDeviceState(DS_Ready, LS_None);
			break;
	}

}
// ----------------------------------------

Boolean LOGIC_BatteryVoltageControl(Int64U Timeout)
{
	if(MEASURE_CapVoltage >= DataTable[REG_CAP_VOLTAGE_THRESHOLD])
		return TRUE;
	else
	{
		if(CONTROL_TimeCounter >= Timeout)
		{
			ZbGPIO_LowPowerSupplyControl(FALSE);
			CONTROL_SwitchToFault(FAULT_POWERON);
		}
	}

	return FALSE;
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
