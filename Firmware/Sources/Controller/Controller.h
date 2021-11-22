// -----------------------------------------
// Logic controller
// ----------------------------------------

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "Logic.h"


// Types
//
typedef enum __DeviceState
{
	DS_None			= 0,
	DS_Fault		= 1,
	DS_Disabled		= 2,
	DS_PowerOn		= 3,
	DS_Ready		= 4,
	DS_InProcess	= 5
} DeviceState;
//

typedef void (*FUNC_AsyncDelegate)();


// Variables
//
extern volatile Int64U CONTROL_TimeCounter;
extern volatile DeviceState CONTROL_State;
//
extern volatile Int16U CONTROL_BootLoaderRequest;

// Functions
//
// Initialize controller
void CONTROL_Init(Boolean BadClockDetected);
// Update low-priority states
void CONTROL_Idle();
// Main control cycle
void CONTROL_Update();
// Notify that CAN system fault occurs
void CONTROL_NotifyCANaFault(ZwCAN_SysFlags Flag);
void CONTROL_NotifyCANbFault(ZwCAN_SysFlags Flag);
// Switch device to fault state
void CONTROL_SwitchToFault(Int16U FaultReason);
// Set device state
void CONTROL_SetDeviceState(DeviceState NewState, LogicState NewSubState);
// Save heating data
void CONTROL_SaveHeatingData(RegulatorsData Sample);
// Regulator
void CONTROL_RegulatorProcess();


#endif // __CONTROLLER_H
