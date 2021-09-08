#ifndef __LOGIC_H
#define __LOGIC_H

// Include
//
#include "stdinc.h"
#include "IQmathLib.h"
#include "IQMathUtils.h"

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
