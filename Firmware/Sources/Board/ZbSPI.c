// Header
//
#include "ZbSPI.h"

// Include
//
#include "SysConfig.h"
#include "Global.h"

// Functions
//
void ZbSPIA_Write(Int16U Data, void (*ControlPinCS)(Boolean))
{
	ControlPinCS(FALSE);
	ZwSPIa_Send(&Data, 1, 16, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);
	ControlPinCS(TRUE);
}
// ----------------------------------------

void ZbSPIB_Write(Int16U Data, void (*ControlPinCS)(Boolean))
{
	ControlPinCS(FALSE);
	ZwSPIb_Send(&Data, 1, 16, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);
	ControlPinCS(TRUE);
}
// ----------------------------------------

void ZbSPIC_Write(Int16U Data, void (*ControlPinCS)(Boolean))
{
	ControlPinCS(FALSE);
	ZwSPIc_Send(&Data, 1, 16, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);
	ControlPinCS(TRUE);
}
// ----------------------------------------

void ZbSPID_Write(Int16U Data, void (*ControlPinCS)(Boolean))
{
	ControlPinCS(FALSE);
	ZwSPId_Send(&Data, 1, 16, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);
	ControlPinCS(TRUE);
}
// ----------------------------------------

Int16U ZbSPIA_Read(void (*ControlPinCS)(Boolean))
{
	Int16U DataRAW = 0;

	ControlPinCS(FALSE);
	ZwSPIa_Send(&DataRAW, 1, 16, STTNormal);
	while(ZwSPIa_GetWordsToReceive() < 1)
	DELAY_US(1);
	ZwSPIx_EndReceive(&SpiaRegs, &DataRAW, 1);
	ControlPinCS(TRUE);

	return DataRAW;
}
// ----------------------------------------

Int16U ZbSPIB_Read(void (*ControlPinCS)(Boolean))
{
	Int16U DataRAW = 0;

	ControlPinCS(FALSE);
	ZwSPIb_Send(&DataRAW, 1, 16, STTNormal);
	while(ZwSPIb_GetWordsToReceive() < 1)
	DELAY_US(1);
	ZwSPIx_EndReceive(&SpibRegs, &DataRAW, 1);
	ControlPinCS(TRUE);

	return DataRAW;
}
// ----------------------------------------

Int16U ZbSPIC_Read(void (*ControlPinCS)(Boolean))
{
	Int16U DataRAW = 0;

	ControlPinCS(FALSE);
	ZwSPIc_Send(&DataRAW, 1, 16, STTNormal);
	while(ZwSPIc_GetWordsToReceive() < 1)
	DELAY_US(1);
	ZwSPIx_EndReceive(&SpicRegs, &DataRAW, 1);
	ControlPinCS(TRUE);

	return DataRAW;
}
// ----------------------------------------

Int16U ZbSPID_Read(void (*ControlPinCS)(Boolean))
{
	Int16U DataRAW = 0;

	ControlPinCS(FALSE);
	ZwSPId_Send(&DataRAW, 1, 16, STTNormal);
	while(ZwSPId_GetWordsToReceive() < 1)
	DELAY_US(1);
	ZwSPIx_EndReceive(&SpidRegs, &DataRAW, 1);
	ControlPinCS(TRUE);

	return DataRAW;
}
// ----------------------------------------




