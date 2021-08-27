// Header
//
#include "ZthMCurrentBoard.h"

// Include
//
#include "ZbGPIO.h"
#include "ZwUtils.h"
#include "Global.h"
#include "ConvertUtils.h"

// Include
//


// Functions
//
void ZthMCB_CurrentSet(Int16U Current)
{
	Int16U DAC_Data = CONVERT_Im(Current) & BIT15;

	ZbGPIO_DCB_MCB_CS(FALSE);

	// Send data
	ZwSPIc_Send(&DAC_Data, 1, 16, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);

	// Strobe to latch
	ZbGPIO_MCB_LDAC(FALSE);
	DELAY_US(1);
	ZbGPIO_MCB_LDAC(TRUE);
	ZbGPIO_DCB_MCB_CS(TRUE);
}
// ----------------------------------------

void ZthMCB_CurrentControl(Boolean State)
{
	ZbGPIO_MCB_Sync(State);
}
// ----------------------------------------




