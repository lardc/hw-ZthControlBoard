#ifndef __LOGIC_H
#define __LOGIC_H

// Include
//
#include "stdinc.h"
#include "Global.h"
#include "MeasuringProcesses.h"

// Definitions
//
typedef enum __LogicState
{
	LS_None					= 0,
	LS_Error				= 1,
	LS_BatteryPrepare		= 2,
	LS_PendingCompletion	= 3,
	//
	LS_ConfigIm				= 10,
	LS_ConfigIh				= 11,
	LS_ConfigIg				= 12,
	LS_ConfigAll			= 13,
	//
	LS_StartHeating			= 20,
	LS_Heating				= 21,
	LS_MeasurementDelay		= 22,
	LS_Measuring			= 23,
	LS_Cooling				= 24,
	LS_ChargeWaiting		= 25
} LogicState;
//

// Variables
//
extern volatile LogicState LOGIC_State;
//
extern Int16U LOGIC_Values_TSP[];
extern Int16U LOGIC_Values_Tcase1[];
extern Int16U LOGIC_Values_Tcase2[];
extern Int16U LOGIC_Values_Tcool1[];
extern Int16U LOGIC_Values_Tcool2[];
extern volatile Int16U LOGIC_Values_Counter;
//
extern volatile Int16U LOGIC_CurrentWidthLess_2ms;
extern volatile Int16U LOGIC_CurrentWidthLess_10ms;
extern volatile Int16U LOGIC_CurrentWidthAbove_10ms;
//
extern volatile Int32U LOGIC_PulseWidthMax;
//
// Functions prototypes
//
void LOGIC_ZthSequencePulsesProcess();
void LOGIC_ZthLongPulseProcess();
void LOGIC_RthSequenceProcess();
void LOGIC_Graduation();
void LOGIC_MeasurementCurrentProcess();
void LOGIC_HeatingCurrentProcess();
void LOGIC_CacheVariables();
void LOGIC_SetState(LogicState State);
void LOGIC_Heating(Boolean State);
void LOGIC_HeatingCurrentSetRange(Int16U Current);
void LOGIC_PowerOnSequence();
void LOGIC_GatePulse(Boolean State);
void LOGIC_GateCurrentProcess();
void LOGIC_TimeCounterReset();
void LOGIC_ResetVariables();
void LOGIC_UpdateParams();

#endif // __LOGIC_H
