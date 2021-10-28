#ifndef __LOGIC_H
#define __LOGIC_H

// Include
//
#include "stdinc.h"
#include "Global.h"
#include "MeasuringProcesses.h"

// Types
//
typedef enum __DeviceSubState
{
	SS_None					= 0,
	SS_IndependentProcesses	= 1,
	SS_Heating				= 2,
	SS_Measuring			= 3,
	SS_Cooling				= 4
} DeviceSubState;
//

// Variables
//
extern volatile DeviceSubState LOGIC_SubState;
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
void LOGIC_SetState(DeviceSubState State);
void LOGIC_IncTimeCounter();
void LOGIC_SetDelayFlag();
void LOGIC_Heating(Boolean State);
void LOGIC_HeatingCurrentSetRange(_iq Current);

#endif // __LOGIC_H
