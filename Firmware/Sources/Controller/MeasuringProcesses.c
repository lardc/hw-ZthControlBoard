// -----------------------------------------
// Measuring processes library
// ----------------------------------------

// Header
#include "MeasuringProcesses.h"
//
// Includes
#include "ZbGPIO.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "ConvertUtils.h"
#include "ZthSensingBoard.h"



// Variables
//


// Forward functions
//


// Functions
//
_iq MEASURE_Tcase1()
{
	return CONVERT_ADCToTcase1(ZthSB_RawReadTcase1());
}
// ----------------------------------------

_iq MEASURE_Tcase2()
{
	return CONVERT_ADCToTcase2(ZthSB_RawReadTcase2());
}
// ----------------------------------------

_iq MEASURE_Tcool1()
{
	return CONVERT_ADCToTcool1(ZthSB_RawReadTcool1());
}
// ----------------------------------------

_iq MEASURE_Tcool2()
{
	return CONVERT_ADCToTcool2(ZthSB_RawReadTcool2());
}
// ----------------------------------------

_iq MEASURE_Ih()
{
	return CONVERT_ADCToIh(ZthSB_RawReadIh());
}
// ----------------------------------------

_iq MEASURE_Im()
{
	return CONVERT_ADCToIm(ZthSB_RawReadIm());
}
// ----------------------------------------

_iq MEASURE_TSP()
{
	return CONVERT_ADCToTSP(ZthSB_RawReadTSP());
}
// ----------------------------------------

