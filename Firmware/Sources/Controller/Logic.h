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
	SS_None			= 0,
	SS_Heating		= 1,
	SS_Measuring	= 2,
	SS_Cooling		= 3
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
extern volatile Int16U LOGIC_Values_Counter;
//

// Functions prototypes
//
Boolean LOGIC_ZthSequencePulsesProcess();
Boolean LOGIC_ZthLongPulseProcess();
Boolean LOGIC_RthSequenceProcess();
Boolean LOGIC_Graduation();
void LOGIC_CashVariables();
void LOGIC_SetState(DeviceSubState State);
void LOGIC_IncTimeCounter();
void LOGIC_SetDelayFlag();

#endif // __LOGIC_H
