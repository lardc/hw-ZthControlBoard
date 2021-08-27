// Header
//
#include "ZbDAC.h"

// Include
//
#include "ZbGPIO.h"
#include "ZwUtils.h"
#include "Global.h"

// Functions
//
void ZbDAC_Write(Int16U Data, void (*ControlPinLDAC)(Boolean))
{
	ZbGPIO_DCB_MCB_CS(FALSE);

	// Send data
	ZwSPIc_Send(&Data, 1, 16, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);

	// Strobe to latch
	ControlPinLDAC(FALSE);
	DELAY_US(1);
	ControlPinLDAC(TRUE);

	ZbGPIO_DCB_MCB_CS(TRUE);
}
// ----------------------------------------




