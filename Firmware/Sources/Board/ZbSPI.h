#ifndef ZBSPI_H_
#define ZBSPI_H_

// Include
#include "stdinc.h"
#include "ZwDSP.h"

// Functions
//
void ZbSPIA_Write(Int16U Data, void (*ControlPinCS)(Boolean));
void ZbSPIB_Write(Int16U Data, void (*ControlPinCS)(Boolean));
void ZbSPIC_Write(Int16U Data, void (*ControlPinCS)(Boolean));
void ZbSPID_Write(Int16U Data, void (*ControlPinCS)(Boolean));
Int16U ZbSPIA_Read(void (*ControlPinCS)(Boolean));
Int16U ZbSPIB_Read(void (*ControlPinCS)(Boolean));
Int16U ZbSPIC_Read(void (*ControlPinCS)(Boolean));
Int16U ZbSPID_Read(void (*ControlPinCS)(Boolean));

#endif /* ZBSPI_H_ */
