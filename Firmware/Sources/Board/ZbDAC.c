// Header
//
#include "ZbDAC.h"

// Include
//
#include "ZbGPIO.h"
#include "ZwUtils.h"
#include "Global.h"
#include "ZbSPI.h"

// Functions
//
void ZbDAC_Write(Int16U Data, void (*ControlPinLDAC)(Boolean), Boolean Invert)
{
	ZbSPIC_Write(Data, &ZbGPIO_DCB_MCB_CS);

	// Strobe to latch
	ControlPinLDAC(Invert);
	ControlPinLDAC(!Invert);
}
// ----------------------------------------




