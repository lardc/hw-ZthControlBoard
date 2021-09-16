// Header
//
#include "ZbSPI.h"

// Include
//
#include "SysConfig.h"
#include "Global.h"
#include "ZbGPIO.h"

// Functions prototypes
Int32U ZbSPIx_Read(volatile struct SPI_REGS *SpixRegs, void (*ControlPinCS)(Boolean));
Int16U ZbSPIx_GetWordsToReceive(volatile struct SPI_REGS *SpixRegs);

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
	Int16U ByteH, ByteL;

	ByteH = Data >> 8;
	ByteL = Data & 0xFF;

	ControlPinCS(FALSE);
	ZwSPIx_Send(&SpicRegs, &ByteH, 1, IO_CL_DEF, STTNormal);
	ZwSPIx_Send(&SpicRegs, &ByteL, 1, IO_CL_DEF, STTNormal);
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

Int32U ZbSPIA_Read(void (*ControlPinCS)(Boolean))
{
	return ZbSPIx_Read(&SpiaRegs, ControlPinCS);
}
// ----------------------------------------

Int32U ZbSPIB_Read(void (*ControlPinCS)(Boolean))
{
	return ZbSPIx_Read(&SpibRegs, ControlPinCS);
}
// ----------------------------------------

Int32U ZbSPIC_Read(void (*ControlPinCS)(Boolean))
{
	return ZbSPIx_Read(&SpicRegs, ControlPinCS);
}
// ----------------------------------------

Int32U ZbSPID_Read(void (*ControlPinCS)(Boolean))
{
	return ZbSPIx_Read(&SpidRegs, ControlPinCS);
}
// ----------------------------------------

Int32U ZbSPIx_Read(volatile struct SPI_REGS *SpixRegs, void (*ControlPinCS)(Boolean))
{
	Int32U DataRAW = 0;
	Int16U Buffer[3] = {0};

	ControlPinCS(FALSE);

	ZwSPIx_Send(SpixRegs, &Buffer[0], 3, IO_CL_DEF, STTNormal);
	while(ZbSPIx_GetWordsToReceive(SpixRegs) < 3)
	DELAY_US(1);
	ZwSPIx_EndReceive(SpixRegs, &Buffer[0], 3);

	ControlPinCS(TRUE);

	DataRAW = (Int32U)Buffer[0] << 16 | Buffer[1] << 8 | Buffer[2];

	return DataRAW;
}
// ----------------------------------------

Int16U ZbSPIx_GetWordsToReceive(volatile struct SPI_REGS *SpixRegs)
{
	return SpixRegs->SPIFFRX.bit.RXFFST;
}
// ----------------------------------------

void ZbEEPROM_Write(Int16U *Data, Int16U BufferSize)
{
	ZbGPIO_EEPROM_CS(FALSE);

	ZwSPIx_Send(&SpidRegs, Data, BufferSize, IO_CL_DEF, STTNormal);
	DELAY_US(EPROM_WRITE_DELAY_US);

	ZbGPIO_EEPROM_CS(TRUE);
}
// ----------------------------------------

void ZbEEPROM_Read(Int16U *Data, Int16U BufferSize)
{
	ZbGPIO_EEPROM_CS(FALSE);

	ZwSPIx_Send(&SpidRegs, Data, BufferSize, IO_CL_DEF, STTNormal);
	while(ZwSPId_GetWordsToReceive() < BufferSize)
		DELAY_US(1);
	ZwSPId_EndReceive(Data, BufferSize);

	DELAY_US(DAC_WRITE_DELAY_US);

	ZbGPIO_EEPROM_CS(TRUE);
}
// ----------------------------------------



