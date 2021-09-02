// Header
//
#include "ZthProtectionBoard.h"

// Include
//
#include "SysConfig.h"
#include "ZbBoard.h"

// Functions
//
PB_Fault ZthPB_FaultCheck()
{
	if(ZbGPIO_IsTransformer1Overheated())
		return PB_FAULT_TR1;
	else if(ZbGPIO_IsTransformer2Overheated())
		return PB_FAULT_TR2;
	else if(ZbGPIO_IsRectifierOverheated())
		return PB_FAULT_REC;
	else if(ZbGPIO_IsWater())
		return PB_FAULT_WATER;

	return PB_FAULT_NONE;
}
// ----------------------------------------




