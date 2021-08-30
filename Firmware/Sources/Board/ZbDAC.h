#ifndef ZBDAC_H_
#define ZBDAC_H_

// Include
//
#include "ZwBase.h"

// Definitions
//
#define DAC_WRITE_A			0
#define DAC_WRITE_B			BIT15
#include DAC_V_MAX_SET		4095
#include DAC_V_MIN_SET		0

// Functions
//
void ZbDAC_Write(Int16U Data, void (*ControlPinLDAC)(Boolean));

#endif /* ZBDAC_H_ */
