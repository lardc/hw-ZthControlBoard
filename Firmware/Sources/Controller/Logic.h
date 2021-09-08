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
	SS_Cooling		= 2
} DeviceSubState;
//

// Variables
//
extern volatile DeviceSubState LOGIC_SubState;

// Functions prototypes
//
void LOGIC_ZthSequencePulsesProcess();
void LOGIC_ZthLongPulseProcess();
void LOGIC_RthSequenceProcess();
void LOGIC_Graduation();
void LOGIC_CashVariables();
void LOGIC_SetState(DeviceSubState State);
void LOGIC_IncTimeCounter();

#endif // __LOGIC_H
