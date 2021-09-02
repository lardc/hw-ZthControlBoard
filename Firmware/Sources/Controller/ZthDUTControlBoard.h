#ifndef ZTHDUTCONTROLBOARD_H_
#define ZTHDUTCONTROLBOARD_H_

// Include
//
#include "stdinc.h"
#include "ZwDSP.h"
#include "IQmathLib.h"

//
typedef enum
{
	GATE_VOLTGE_0V,
	GATE_VOLTGE_15V,
	GATE_VOLTGE_20V
}DCB_GateVoltage;

typedef enum
{
	CURRENT_SOURCE,
	VOLTAGE_SOURCE
}DCB_OutputType;

// Functions
//
void ZthDCB_CurrentSet(_iq Current);
void ZthDCB_VoltageSet(DCB_GateVoltage Voltage);
void ZthDCB_SwitchOutput(DCB_OutputType Source);

#endif /* ZTHDUTCONTROLBOARD_H_ */
