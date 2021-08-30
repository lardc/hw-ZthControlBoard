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
_iq ZthSB_ReadTcase1()
{
	return CONVERT_ADCToTcase1(ZbSPIB_Read(&ZbGPIO_SB_Tc1_CS));
}
// ----------------------------------------

_iq ZthSB_ReadTcase2()
{
	return CONVERT_ADCToTcase2(ZbSPIB_Read(&ZbGPIO_SB_Tc2_CS));
}
// ----------------------------------------

_iq ZthSB_ReadTcool1()
{
	return CONVERT_ADCToTcool1(ZbSPID_Read(&ZbGPIO_SB_Tcool1_CS));
}
// ----------------------------------------

_iq ZthSB_ReadTcool2()
{
	return CONVERT_ADCToTcool2(ZbSPIA_Read(&ZbGPIO_SB_Tcool2_CS));
}
// ----------------------------------------

_iq ZthSB_ReadTSP()
{
	return CONVERT_ADCToTSP(ZbSPIA_Read(&ZbGPIO_SB_TSP_CS));
}
// ----------------------------------------

_iq ZthSB_ReadIh()
{
	return CONVERT_ADCToIh(ZbSPIB_Read(&ZbGPIO_SB_Ih_CS));
}
// ----------------------------------------

_iq ZthSB_ReadIm()
{
	return CONVERT_ADCToIm(ZbSPIB_Read(&ZbGPIO_SB_Im_CS));
}
// ----------------------------------------



