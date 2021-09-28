// Header
//
#include "ZthMCurrentBoard.h"

// Include
//
#include "SysConfig.h"
#include "ZbBoard.h"
#include "Global.h"
#include "ConvertUtils.h"


// Functions
//
void ZthMCB_CurrentSet(_iq Current)
{
	ZbDAC_Write((CONVERT_ImToDAC(Current) | DAC_WRITE_A), &ZbGPIO_MCB_LDAC, FALSE);
}
// ----------------------------------------

void ZthMCB_CurrentControl(Boolean State)
{
	ZbGPIO_MCB_Sync(State);
}
// ----------------------------------------




