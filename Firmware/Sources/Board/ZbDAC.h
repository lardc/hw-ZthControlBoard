#ifndef ZBDAC_H_
#define ZBDAC_H_

// Include
//
#include "ZwBase.h"

// Definitions
//
#define DAC_WRITE_A			0
#define DAC_WRITE_B			BIT15
#define DAC_MAX_VALUE		4095
#define DAC_MIN_VALUE		0

// Functions
//
void ZbDAC_Write(Int16U Data, void (*ControlPinLDAC)(Boolean));

#endif /* ZBDAC_H_ */
