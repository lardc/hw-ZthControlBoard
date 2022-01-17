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
	//
	LS_Heating				= 20,
	LS_Measuring			= 21,
	LS_Cooling				= 22
} LogicState;
//

// Variables
//
extern volatile LogicState LOGIC_State;
//
extern Int16U LOGIC_Values_TSP[VALUES_x_SIZE];
extern Int16U LOGIC_Values_Tcase1[VALUES_x_SIZE];
extern Int16U LOGIC_Values_Tcase2[VALUES_x_SIZE];
extern Int16U LOGIC_Values_Tcool1[VALUES_x_SIZE];
extern Int16U LOGIC_Values_Tcool2[VALUES_x_SIZE];
extern Int16U LOGIC_Values_ErrorIh[VALUES_x_SIZE];
extern volatile Int16U LOGIC_Values_Counter;
//
extern volatile _iq LOGIC_CurrentWidthLess_2ms;
extern volatile _iq LOGIC_CurrentWidthLess_10ms;
extern volatile _iq LOGIC_CurrentWidthAbove_10ms;
//
extern volatile Int32U LOGIC_PulseWidthMax;
//
extern volatile Boolean TimeFlag;
//

// Functions prototypes
//
Boolean LOGIC_ZthSequencePulsesProcess();
Boolean LOGIC_ZthLongPulseProcess();
Boolean LOGIC_RthSequenceProcess();
Boolean LOGIC_Graduation();
Boolean LOGIC_MeasurementCurrentProcess();
Boolean LOGIC_HeatingCurrentProcess();
void LOGIC_CacheVariables();
void LOGIC_SetState(LogicState State);
void LOGIC_Heating(Boolean State);
void LOGIC_HeatingCurrentSetRange(_iq Current);
void LOGIC_PowerOnSequence();
void LOGIC_HandleCommunicationError();
void LOGIC_GatePulse(Boolean State);
Boolean LOGIC_GateCurrentProcess();
void LOGIC_TimeCounterReset();
void LOGIC_PulseWidthControl();
void LOGIC_TimeCounterInc();

#endif // __LOGIC_H
