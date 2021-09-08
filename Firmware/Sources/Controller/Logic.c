// -----------------------------------------
// Main logic library
// ----------------------------------------

// Header
#include "Logic.h"
//
// Includes
#include "xCCICommon.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Controller.h"
#include "ZbBoard.h"
#include "MeasuringProcesses.h"

// Variables
//
volatile DeviceSubState LOGIC_SubState = SS_None;
volatile Int64U LOGIC_TimeCounter = 0, LOGIC_HeatingTimeCounter = 0, LOGIC_CollingTime = 0;;
//
volatile Int16U LOGIC_CoolingMode;
volatile Int16U LOGIC_ZthPause, LOGIC_PulseWidth, LOGIC_Pause, LOGIC_ZthPulseWidthMin, LOGIC_ZthPulseWidthMax, LOGIC_Delay;
volatile _iq LOGIC_ImpulseCurrent, LOGIC_HeatingCurrent, LOGIC_Tmax;

// Functions
//
void LOGIC_ZthSequencePulsesProcess()
{
	switch (LOGIC_SubState)
	{
		case SS_Heating:
			break;

		case SS_Cooling:
			break;
	}
}
// ----------------------------------------

void LOGIC_ZthLongPulseProcess()
{

}
// ----------------------------------------

void LOGIC_RthSequenceProcess()
{

}
// ----------------------------------------

void LOGIC_Graduation()
{

}
// ----------------------------------------

void LOGIC_SetState(DeviceSubState State)
{
	LOGIC_SubState = State;
}
// ----------------------------------------

void LOGIC_IncTimeCounter()
{
	LOGIC_TimeCounter++;
}
// ----------------------------------------

void LOGIC_CashVariables()
{
	LOGIC_ImpulseCurrent = _IQI(DataTable[REG_IMPULSE_CURRENT]);
	LOGIC_HeatingCurrent = _IQI(DataTable[REG_HEATING_CURRENT]);
	LOGIC_CoolingMode = DataTable[REG_COOLING_MODE];
	LOGIC_Tmax = _IQI(DataTable[REG_T_MAX]);
	LOGIC_ZthPulseWidthMin = DataTable[REG_ZTH_PULSE_WIDTH_MIN];
	LOGIC_ZthPulseWidthMax = DataTable[REG_ZTH_PULSE_WIDTH_MAX];
	LOGIC_ZthPause = REG_ZTH_PAUSE;
	LOGIC_PulseWidth = DataTable[REG_PULSE_WIDTH];
	LOGIC_Pause = DataTable[REG_PAUSE];
	LOGIC_Delay = DataTable[REG_DELAY];
}
// ----------------------------------------

// No more.
