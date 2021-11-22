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

// Functions
Boolean DRCU_UpdateState(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage);
void DRCU_ResetFault(Boolean Emulate, Int16U NodeIDReg, Int16U StateStorage, volatile LogicState *CurrentLogicState,
		LogicState NextLogicState);
void DRCU_PowerOn(Boolean Emulate, Int16U NodeIDReg, volatile Int16U *StateStorage,
		volatile LogicState *CurrentLogicState, Int16U FaultCode, LogicState NextLogicState);
void DRCU_PowerOff(Boolean Emulate, Int16U NodeIDReg, volatile Int16U *StateStorage,
		volatile LogicState *CurrentLogicState, Int16U FaultCode, LogicState NextLogicState);
void DRCU_Config(Boolean Emulate, Int16U NodeIDReg, volatile Int16U *StateStorage, Int16U Current,
		volatile LogicState *CurrentLogicState, LogicState NextLogicState);
void DRCU_WaitReady(Int64U TimeCounter, Int64U Timeout, volatile Int16U StateStorage,
		volatile LogicState *CurrentLogicState);

#endif // __COMMON_H
