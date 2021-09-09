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
volatile Int16U LOGIC_PulseWidth, LOGIC_Pause, LOGIC_ZthPulseWidthMin, LOGIC_ZthPulseWidthMax, LOGIC_MeasurementDelay;
volatile _iq LOGIC_ImpulseCurrent, LOGIC_HeatingCurrent, LOGIC_Tmax, LOGIC_ZthPause;
//
volatile Int64U LOGIC_ActualPulseWidth = 0;
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
Boolean LOGIC_CoolingProcess();
void LOGIC_DelayStart(Int64U Delay_us);
Boolean LOGIC_DelayCheck();
void LOGIC_SaveData(CombinedData Sample);

// Functions
//
Boolean LOGIC_ZthSequencePulsesProcess()
{
	Boolean Result = FALSE;

	switch (LOGIC_SubState)
	{
		case SS_Heating:
			if(LOGIC_HeatingProcess())
				LOGIC_SetState(SS_Measuring);
			break;

		case SS_Measuring:
			LOGIC_MeasuringProcess();
			LOGIC_SetState(SS_Cooling);
			break;

		case SS_Cooling:
			if(LOGIC_ActualPulseWidth >= LOGIC_ZthPulseWidthMax)
			{
				LOGIC_SetState(SS_None);
				Result = TRUE;
			}
			else
			{
				if(LOGIC_CoolingProcess())
					LOGIC_SetState(SS_Heating);
			}
			break;
	}

	return Result;
}
// ----------------------------------------

Boolean LOGIC_ZthLongPulseProcess()
{
	return FALSE;
}
// ----------------------------------------

Boolean LOGIC_RthSequenceProcess()
{
	return FALSE;
}
// ----------------------------------------

Boolean LOGIC_Graduation()
{
	return FALSE;
}
// ----------------------------------------

Boolean LOGIC_HeatingProcess()
{
	static Boolean HeatingProcess = FALSE;

	if(!HeatingProcess)
	{
		if(LOGIC_ActualPulseWidth < PULSE_WIDTH_10MS)
			REGULATOR_Update(SelectIh, LOGIC_ImpulseCurrent);
		else
			REGULATOR_Update(SelectIh, LOGIC_HeatingCurrent);
		REGULATOR_Enable(SelectIh, TRUE);
		REGULATOR_Enable(SelectP, TRUE);

		LOGIC_TimeCounter = 0;
		HeatingProcess = TRUE;

		ZwTimer_StartT1();
	}

	if(LOGIC_ActualPulseWidth <= LOGIC_TimeCounter)
	{
		REGULATOR_Update(SelectIh, 0);
		REGULATOR_Enable(SelectIh, FALSE);
		REGULATOR_Enable(SelectP, FALSE);

		HeatingProcess = FALSE;

		ZwTimer_StopT1();

		return TRUE;
	}
	else
		return FALSE;
}
// ----------------------------------------

void LOGIC_MeasuringProcess()
{
	volatile Int64U Pause;

	Pause = _IQint(_IQdiv(_IQmpy(_IQI(LOGIC_ActualPulseWidth), LOGIC_ZthPause), 100)) - LOGIC_MeasurementDelay;

	LOGIC_DelayStart(LOGIC_MeasurementDelay);
	while(LOGIC_DelayCheck()){}
	LOGIC_DelayStart(Pause);

	LOGIC_SaveData(MEASURE_CombinedData(LOGIC_CoolingMode));
}
// ----------------------------------------

Boolean LOGIC_CoolingProcess(Int64U CoolingTime)
{
	if(LOGIC_DelayCheck())
		return TRUE;
	else
		return FALSE;
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
	LOGIC_ActualPulseWidth = LOGIC_ZthPulseWidthMin;
	//
	LOGIC_ImpulseCurrent = _IQI(DataTable[REG_IMPULSE_CURRENT]);
	LOGIC_HeatingCurrent = _IQI(DataTable[REG_HEATING_CURRENT]);
	//
	LOGIC_MeasurementDelay = DataTable[REG_DELAY];
	LOGIC_ZthPause = _IQdiv(DataTable[REG_ZTH_PAUSE], 10);
	//
	LOGIC_CoolingMode = DataTable[REG_COOLING_MODE];




	LOGIC_Tmax = _IQI(DataTable[REG_T_MAX]);
	LOGIC_PulseWidth = DataTable[REG_PULSE_WIDTH];
	LOGIC_Pause = DataTable[REG_PAUSE];

	// Reset variables
	EP_DataCounter = 0;

}
// ----------------------------------------

// No more.
