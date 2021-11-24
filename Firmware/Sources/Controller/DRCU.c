// Header
#include "DRCU.h"

// Includes
#include "HighLevelInterface.h"
#include "DataTable.h"
#include "DRCUDictionary.h"
#include "Controller.h"
#include "DeviceObjectDictionary.h"

// Functions
//
Boolean DRCU_UpdateState(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage)
{
	Int16U Register;
	Boolean Result = TRUE;
	
	if(!DataTable[RegEmulate])
	{
		if(HLI_CAN_Read16(DataTable[NodeIDReg], DRCU_REG_DEV_STATE, &Register))
			*StateStorage = Register;
		else
			Result = FALSE;
	}
	
	return Result;
}
//-----------------------------

void DRCU_ResetFault(Boolean RegEmulate, Int16U NodeIDReg, Int16U StateStorage, volatile LogicState *CurrentLogicState,
		LogicState NextLogicState)
{
	if(!DataTable[RegEmulate] && StateStorage == DRCU_DS_Fault)
	{
		if(HLI_CAN_CallAction(DataTable[NodeIDReg], DRCU_ACT_CLR_FAULT))
			*CurrentLogicState = NextLogicState;
	}
	else
		*CurrentLogicState = NextLogicState;
}
//-----------------------------

void DRCU_PowerOn(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage,
		volatile LogicState *CurrentLogicState, Int16U FaultCode, LogicState NextLogicState)
{
	if(!DataTable[RegEmulate])
	{
		switch(*StateStorage)
		{
			case DRCU_DS_None:
				if(HLI_CAN_CallAction(DataTable[NodeIDReg], DRCU_ACT_ENABLE_POWER))
					*CurrentLogicState = NextLogicState;
				break;

			case DRCU_DS_Ready:
				if(HLI_CAN_CallAction(DataTable[NodeIDReg], DRCU_ACT_DISABLE_POWER))
					if(HLI_CAN_CallAction(DataTable[NodeIDReg], DRCU_ACT_ENABLE_POWER))
						*CurrentLogicState = NextLogicState;
				break;

			case DRCU_DS_Fault:
				if(HLI_CAN_CallAction(DataTable[NodeIDReg], DRCU_ACT_CLR_FAULT))
					if(HLI_CAN_CallAction(DataTable[NodeIDReg], DRCU_ACT_ENABLE_POWER))
						*CurrentLogicState = NextLogicState;
				break;

			case DRCU_DS_Disabled:
				*CurrentLogicState = LS_Error;
				CONTROL_SwitchToFault(FaultCode);
				break;
		}
	}
	else
	{
		*StateStorage = DRCU_DS_Ready;
		*CurrentLogicState = NextLogicState;
	}
}
//-----------------------------

void DRCU_PowerOff(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage,
		volatile LogicState *CurrentLogicState, Int16U FaultCode, LogicState NextLogicState)
{
	if(!DataTable[RegEmulate])
	{
		switch(*StateStorage)
		{
			case DRCU_DS_None:
				break;

			case DRCU_DS_Ready:
				if(HLI_CAN_CallAction(DataTable[NodeIDReg], DRCU_ACT_DISABLE_POWER))
					*CurrentLogicState = NextLogicState;
				break;

			default:
				*CurrentLogicState = LS_Error;
				CONTROL_SwitchToFault(FaultCode);
				break;
		}
	}
	else
	{
		*StateStorage = DRCU_DS_None;
		*CurrentLogicState = NextLogicState;
	}
}
//-----------------------------

void DRCU_Config(Boolean RegEmulate, Int16U NodeIDReg, volatile Int16U *StateStorage, Int16U RegCurrent,
		volatile LogicState *CurrentLogicState, LogicState NextLogicState)
{
	if(!DataTable[RegEmulate])
	{
		if(HLI_CAN_Write16(DataTable[NodeIDReg], DRCU_REG_CURRENT_SETPOINT, DataTable[RegCurrent]))
				if(HLI_CAN_CallAction(DataTable[NodeIDReg], DCRU_ACT_CONFIG))
					*CurrentLogicState = NextLogicState;
	}
	else
	{
		*StateStorage = DRCU_DS_Ready;
		*CurrentLogicState = NextLogicState;
	}
}
//-----------------------------

void DRCU_WaitReady(Int64U TimeCounter, Int64U Timeout, volatile Int16U StateStorage,
		volatile LogicState *CurrentLogicState, LogicState NextLogicState)
{
	if(*CurrentLogicState != LS_None && *CurrentLogicState != LS_Error)
	{
		if(Timeout > TimeCounter)
		{
			if(StateStorage == DRCU_DS_Ready)
				*CurrentLogicState = NextLogicState;
		}
		else
		{
			*CurrentLogicState = LS_Error;
			CONTROL_SwitchToFault(FAULT_DRCU_WAIT_STATE);
		}
	}
}
//-----------------------------
