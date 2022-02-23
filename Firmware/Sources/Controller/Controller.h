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

typedef void (*CONTROL_FUNC_RealTimeRoutine)();


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
// Update high-priority states
void CONTROL_UpdateSlow();
void CONTROL_UpdateFast();
// Notify that CAN system fault occurs
void CONTROL_NotifyCANaFault(ZwCAN_SysFlags Flag);
// Switch device to fault state
void CONTROL_SwitchToFault(Int16U FaultReason);
// Set device state
void CONTROL_SetDeviceState(DeviceState NewState, LogicState NewSubState);
// Stop process
void CONTROL_StopProcess(Int16U OpResult);
void CONTROL_ForceStopProcess();
//
void CONTROL_SubcribeToCycle(CONTROL_FUNC_RealTimeRoutine Routine);


#endif // __CONTROLLER_H
