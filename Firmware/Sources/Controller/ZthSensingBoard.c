// Header
//
#include "ZthSensingBoard.h"

// Include
//
#include "SysConfig.h"
#include "ZbBoard.h"
#include "Global.h"
#include "ConvertUtils.h"

// Functions
//
Int16U ZthSB_ReadTcase1()
{
	return CONVERT_Tcase1(ZbSPIB_Read(&ZbGPIO_SB_Tc1_CS));
}
// ----------------------------------------

Int16U ZthSB_ReadTcase2()
{
	return CONVERT_Tcase2(ZbSPIB_Read(&ZbGPIO_SB_Tc2_CS));
}
// ----------------------------------------

Int16U ZthSB_ReadTcool1()
{
	return CONVERT_Tcool1(ZbSPID_Read(&ZbGPIO_SB_Tcool1_CS));
}
// ----------------------------------------

Int16U ZthSB_ReadTcool2()
{
	return CONVERT_Tcool2(ZbSPIA_Read(&ZbGPIO_SB_Tcool2_CS));
}
// ----------------------------------------

Int16U ZthSB_ReadTSP()
{
	return CONVERT_TSP(ZbSPIA_Read(&ZbGPIO_SB_TSP_CS));
}
// ----------------------------------------

Int16U ZthSB_ReadIh()
{
	return CONVERT_Ih(ZbSPIB_Read(&ZbGPIO_SB_Ih_CS));
}
// ----------------------------------------

Int16U ZthSB_ReadIm()
{
	return CONVERT_Im(ZbSPIB_Read(&ZbGPIO_SB_Im_CS));
}
// ----------------------------------------



