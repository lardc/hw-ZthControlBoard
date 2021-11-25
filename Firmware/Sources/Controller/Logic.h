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
	LS_BatteryPwrOn			= 2,
	LS_PendingCompletion	= 3,
	//
	LS_DRCU_PwrOn			= 10,
	LS_DRCU_Config			= 11,
	LS_DRCU_WaitReady		= 12,
	//
	LS_ConfigIm				= 20,
	LS_ConfigIh				= 21,
	LS_ConfigIg				= 22,
	//
	LS_Heating				= 30,
	LS_Measuring			= 31,
	LS_Cooling				= 32
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
void LOGIC_PowerOffProcess();
void LOGIC_HandleCommunicationError();
void LOGIC_ResetFaultProcess();
void LOGIC_DRCUConfigProcess(Int16U Current);
void LOGIC_GatePulse(Boolean State);
Boolean LOGIC_GateCurrentProcess();

#endif // __LOGIC_H
