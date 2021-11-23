#ifndef __COMMON_H
#define __COMMON_H

// Include
#include "stdinc.h"
#include "Logic.h"

// Types
typedef struct __DRCUConfig
{
	Int16U Current;
} DRCUConfig, *pDRCUConfig;

// States
typedef enum __DRCUDeviceState
{
	DRCU_DS_None			= 0,
	DRCU_DS_Fault			= 1,
	DRCU_DS_Disabled		= 2,
	DRCU_DS_PowerPrepare	= 3,
	DRCU_DS_Ready			= 4,
} DRCUDeviceState;

// Functions
Boolean DRCU_UpdateState(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage);
void DRCU_ResetFault(Boolean RegEmulate, Int16U NodeIDReg, Int16U StateStorage, volatile LogicState *CurrentLogicState,
		LogicState NextLogicState);
void DRCU_PowerOn(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage,
		volatile LogicState *CurrentLogicState, Int16U FaultCode, LogicState NextLogicState);
void DRCU_PowerOff(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage,
		volatile LogicState *CurrentLogicState, Int16U FaultCode, LogicState NextLogicState);
void DRCU_Config(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage, Int16U Current,
		volatile LogicState *CurrentLogicState, LogicState NextLogicState);
void DRCU_WaitReady(Int64U TimeCounter, Int64U Timeout, volatile Int16U StateStorage,
		volatile LogicState *CurrentLogicState, LogicState NextLogicState);

#endif // __COMMON_H
