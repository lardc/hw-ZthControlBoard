// Header
//
#include "ZbSPI.h"

// Include
//
#include "SysConfig.h"
#include "Global.h"
#include "ZbGPIO.h"

// Functions
//
void ZbSPIA_Write(Int16U Data, void (*ControlPinCS)(Boolean))
{
	ControlPinCS(FALSE);
	ZwSPIx_Send(&SpiaRegs, &Data, 1, IO_CL_DEF, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);
	ControlPinCS(TRUE);
}
// ----------------------------------------

void ZbSPIB_Write(Int16U Data, void (*ControlPinCS)(Boolean))
{
	ControlPinCS(FALSE);
	ZwSPIx_Send(&SpibRegs, &Data, 1, IO_CL_DEF, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);
	ControlPinCS(TRUE);
}
// ----------------------------------------

void ZbSPIC_Write(Int16U Data, void (*ControlPinCS)(Boolean))
{
	ControlPinCS(FALSE);
	ZwSPIx_Send(&SpicRegs, &Data, 1, IO_CL_DEF, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);
	ControlPinCS(TRUE);
}
// ----------------------------------------

void ZbSPID_Write(Int16U Data, void (*ControlPinCS)(Boolean))
{
	ControlPinCS(FALSE);
	ZwSPIx_Send(&SpidRegs, &Data, 1, IO_CL_DEF, STTNormal);
	DELAY_US(DAC_WRITE_DELAY_US);
	ControlPinCS(TRUE);
}
// ----------------------------------------

Int16U ZbSPIA_Read(void (*ControlPinCS)(Boolean))
{
	Int16U DataRAW = 0;

	ControlPinCS(FALSE);
	ZwSPIx_Send(&SpiaRegs, &DataRAW, 1, IO_CL_DEF, STTNormal);
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
	ZwSPIx_Send(&SpibRegs, &DataRAW, 1, IO_CL_DEF, STTNormal);
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
	ZwSPIx_Send(&SpicRegs, &DataRAW, 1, IO_CL_DEF, STTNormal);
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
	ZwSPIx_Send(&SpidRegs, &DataRAW, 1, IO_CL_DEF, STTNormal);
	while(ZwSPId_GetWordsToReceive() < 1)
	DELAY_US(1);
	ZwSPIx_EndReceive(&SpidRegs, &DataRAW, 1);
	ControlPinCS(TRUE);

	return DataRAW;
}
// ----------------------------------------

void ZbEEPROM_Write(Int16U *Data, Int16U BufferSize)
{
	ZbGPIO_EEPROM_CS(FALSE);

	ZwSPIx_Send(&SpidRegs, Data, BufferSize, MEM_CL, STTNormal);
	DELAY_US(EPROM_WRITE_DELAY_US);

	ZbGPIO_EEPROM_CS(TRUE);
}
// ----------------------------------------

void ZbEEPROM_Read(Int16U *Data, Int16U BufferSize)
{
	ZbGPIO_EEPROM_CS(FALSE);

	ZwSPIx_Send(&SpidRegs, Data, BufferSize, MEM_CL, STTNormal);
	while(ZwSPId_GetWordsToReceive() < BufferSize)
		DELAY_US(1);
	ZwSPId_EndReceive(Data, BufferSize);

	DELAY_US(DAC_WRITE_DELAY_US);

	ZbGPIO_EEPROM_CS(TRUE);
}
// ----------------------------------------



