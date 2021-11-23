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
	LS_PON_Battery			= 2,
	LS_PON_DRCU				= 3,
	LS_CLR_DRCU				= 4,
	LS_POFF_DRCU			= 5,
	LS_WAIT_READY			= 6,
	//
	LS_IndependentProcesses	= 7,
	LS_Heating				= 8,
	LS_Measuring			= 9,
	LS_Cooling				= 10
} LogicState;
//

// Variables
//
extern volatile LogicState LOGIC_State;
extern Int16U LOGIC_DRCU_State;
//
extern Int16U LOGIC_Values_TSP[VALUES_x_SIZE];
extern Int16U LOGIC_Values_Tcase1[VALUES_x_SIZE];
extern Int16U LOGIC_Values_Tcase2[VALUES_x_SIZE];
extern Int16U LOGIC_Values_Tcool1[VALUES_x_SIZE];
extern Int16U LOGIC_Values_Tcool2[VALUES_x_SIZE];
extern Int16U LOGIC_Values_ErrorIh[VALUES_x_SIZE];
extern volatile Int16U LOGIC_Values_Counter;
//
extern volatile _iq LOGIC_ImpulseCurrent;
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
void LOGIC_IncTimeCounter();
void LOGIC_SetDelayFlag();
void LOGIC_Heating(Boolean State);
void LOGIC_HeatingCurrentSetRange(_iq Current);
void LOGIC_PowerOnSequence();

#endif // __LOGIC_H
