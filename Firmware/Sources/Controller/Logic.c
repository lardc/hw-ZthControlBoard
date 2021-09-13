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

// Definitions
//
#define PULSE_WIDTH_10MS		10000	// in us

// Variables
//
volatile DeviceSubState LOGIC_SubState = SS_None;
volatile Int64U LOGIC_TimeCounter = 0, LOGIC_HeatingTimeCounter = 0, LOGIC_CollingTime = 0;
//
volatile Int16U LOGIC_CoolingMode;
volatile Int16U LOGIC_PulseWidth, LOGIC_CoolingTime, LOGIC_ZthPulseWidthMin, LOGIC_ZthPulseWidthMax, LOGIC_MeasurementDelay;
volatile _iq LOGIC_ImpulseCurrent, LOGIC_HeatingCurrent, LOGIC_Tmax, LOGIC_ZthPause;
//
volatile Int64U LOGIC_ActualPulseWidth = 0;
volatile Int64U LOGIC_ActualDelayWidth = 0;
//
volatile Boolean DelayFlag = FALSE;
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
void LOGIC_DelayStart(Int64U Delay_us);
Boolean LOGIC_DelayCheck();
void LOGIC_SaveData(CombinedData Sample);
void LOGIC_CalculateTimeInterval(volatile Int64U *TimeInterval);

// Functions
//
Boolean LOGIC_ZthSequencePulsesProcess()
{
	Boolean Result = FALSE;
	static volatile Int64U CoolingTimeTemp = 0;

	switch (LOGIC_SubState)
	{
		case SS_None:
			LOGIC_ActualPulseWidth = LOGIC_ZthPulseWidthMin;
			LOGIC_SetState(SS_Heating);
			break;

		case SS_Heating:
			if(LOGIC_HeatingProcess())
				LOGIC_SetState(SS_Measuring);
			break;

		case SS_Measuring:
			LOGIC_MeasuringProcess();

			if(LOGIC_ActualPulseWidth >= LOGIC_ZthPulseWidthMax)
			{
				LOGIC_SetState(SS_None);
				Result = TRUE;
			}
			else
			{
				CoolingTimeTemp = _IQint(_IQdiv(_IQmpy(_IQI(LOGIC_ActualPulseWidth), LOGIC_ZthPause), 100)) - LOGIC_MeasurementDelay;
				LOGIC_SetState(SS_Cooling);
			}
			break;

		case SS_Cooling:
			if(LOGIC_CoolingProcess(CoolingTimeTemp))
			{
				LOGIC_CalculateTimeInterval(&LOGIC_ActualPulseWidth);
				LOGIC_SetState(SS_Heating);
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

	switch (LOGIC_SubState)
	{
		case SS_None:
			LOGIC_ActualPulseWidth = LOGIC_ZthPulseWidthMax;
			LOGIC_SetState(SS_Heating);
			break;

		case SS_Heating:
			if(LOGIC_HeatingProcess())
				LOGIC_SetState(SS_Measuring);
			break;

		case SS_Measuring:
			LOGIC_MeasuringProcess();

			if(LOGIC_ActualDelayWidth >= TIME_DELAY_MAX)
			{
				LOGIC_SetState(SS_None);
				Result = TRUE;
			}
			else
			{
				CoolingTimeTemp = LOGIC_ActualDelayWidth - LOGIC_MeasurementDelay;
				LOGIC_SetState(SS_Cooling);
			}
			break;

		case SS_Cooling:
			if(LOGIC_CoolingProcess(CoolingTimeTemp))
			{
				LOGIC_CalculateTimeInterval(&LOGIC_ActualDelayWidth);
				LOGIC_SetState(SS_Measuring);
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

	switch (LOGIC_SubState)
	{
		case SS_None:
			LOGIC_ActualPulseWidth = LOGIC_PulseWidth;
			LOGIC_SetState(SS_Heating);
			break;

		case SS_Heating:
			if(LOGIC_HeatingProcess())
				LOGIC_SetState(SS_Measuring);
			break;

		case SS_Measuring:
			LOGIC_MeasuringProcess();
			CoolingTimeTemp = LOGIC_CoolingTime - LOGIC_MeasurementDelay;
			LOGIC_SetState(SS_Cooling);
			break;

		case SS_Cooling:
			if(LOGIC_CoolingProcess(CoolingTimeTemp))
				LOGIC_SetState(SS_Heating);
			break;
	}

	return Result;
}
// ----------------------------------------

Boolean LOGIC_Graduation()
{
	return FALSE;
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
	LOGIC_DelayStart(LOGIC_MeasurementDelay);
	while(LOGIC_DelayCheck()){}

	LOGIC_SaveData(MEASURE_CombinedData(LOGIC_CoolingMode));
}
// ----------------------------------------

Boolean LOGIC_CoolingProcess(Int64U Pause)
{
	static Boolean DelayStartFlag = FALSE;

	if(!DelayStartFlag)
	{
		DelayStartFlag = TRUE;
		LOGIC_DelayStart(Pause);
	}
	else
	{
		if(LOGIC_DelayCheck())
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

void LOGIC_DelayStart(Int64U Delay_us)
{
	DelayFlag = FALSE;

	ZwTimer_SetT2(Delay_us);
	ZwTimer_StartT2();
}
// ----------------------------------------

Boolean LOGIC_DelayCheck()
{
	return DelayFlag;
}

void LOGIC_SetState(DeviceSubState State)
{
	LOGIC_SubState = State;
}
// ----------------------------------------

void LOGIC_IncTimeCounter()
{
	LOGIC_TimeCounter += TIMER1_PERIOD;
}
// ----------------------------------------

void LOGIC_SetDelayFlag()
{
	DelayFlag = TRUE;
}
// ----------------------------------------

void LOGIC_CashVariables()
{
	LOGIC_ZthPulseWidthMin = DataTable[REG_ZTH_PULSE_WIDTH_MIN];
	LOGIC_ZthPulseWidthMax = _IQmpy(DataTable[REG_ZTH_PULSE_WIDTH_MAX], 1000);
	LOGIC_PulseWidth = DataTable[REG_PULSE_WIDTH];
	//
	LOGIC_ImpulseCurrent = _IQI(DataTable[REG_IMPULSE_CURRENT]);
	LOGIC_HeatingCurrent = _IQI(DataTable[REG_HEATING_CURRENT]);
	//
	LOGIC_MeasurementDelay = DataTable[REG_DELAY];
	LOGIC_ZthPause = _IQdiv(DataTable[REG_ZTH_PAUSE], 10);
	LOGIC_CoolingTime = DataTable[REG_COOLING_TIME];
	//
	LOGIC_CoolingMode = DataTable[REG_COOLING_MODE];




	LOGIC_Tmax = _IQI(DataTable[REG_T_MAX]);



	// Reset variables to default states
	EP_DataCounter = 0;
}
// ----------------------------------------

// No more.
