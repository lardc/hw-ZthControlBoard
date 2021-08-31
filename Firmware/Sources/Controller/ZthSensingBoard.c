// Header
//
#include "ZthSensingBoard.h"

// Include
//
#include "ZbBoard.h"

// Functions
//
Int16U ZthSB_RawReadTcase1()
{
	return ZbSPIB_Read(&ZbGPIO_SB_Tc1_CS);
}
// ----------------------------------------

Int16U ZthSB_RawReadTcase2()
{
	return ZbSPIB_Read(&ZbGPIO_SB_Tc2_CS);
}
// ----------------------------------------

Int16U ZthSB_RawReadTcool1()
{
	return ZbSPID_Read(&ZbGPIO_SB_Tcool1_CS);
}
// ----------------------------------------

Int16U ZthSB_RawReadTcool2()
{
	return ZbSPIA_Read(&ZbGPIO_SB_Tcool2_CS);
}
// ----------------------------------------

Int16U ZthSB_RawReadTSP()
{
	return ZbSPIA_Read(&ZbGPIO_SB_TSP_CS);
}
// ----------------------------------------

Int16U ZthSB_RawReadIh()
{
	return ZbSPIB_Read(&ZbGPIO_SB_Ih_CS);
}
// ----------------------------------------

Int16U ZthSB_RawReadIm()
{
	return ZbSPIB_Read(&ZbGPIO_SB_Im_CS);
}
// ----------------------------------------



