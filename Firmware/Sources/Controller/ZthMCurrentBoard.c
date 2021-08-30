// Header
//
#include "ZthMCurrentBoard.h"

// Include
//
#include "ZbGPIO.h"
#include "ZwUtils.h"
#include "Global.h"
#include "ConvertUtils.h"
#include "ZbDAC.h"


// Functions
//
void ZthMCB_CurrentSet(Int16U Current)
{
	ZbDAC_Write((CONVERT_Im(Current) & DAC_WRITE_A), &ZbGPIO_MCB_LDAC);
}
// ----------------------------------------

void ZthMCB_CurrentControl(Boolean State)
{
	ZbGPIO_MCB_Sync(State);
}
// ----------------------------------------




