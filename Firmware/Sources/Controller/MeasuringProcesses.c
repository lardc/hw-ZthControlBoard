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

// Definitins
//
#define AVERAGE_ARRAY_SIZE		16
#define AVERAGE_COUNTER_MASK	AVERAGE_ARRAY_SIZE - 1
#define AVERAGE_RESULT_SHIFT	4

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

CombinedData MEASURE_RegulatorsSample()
{
	CombinedData Sample;

	Sample.Ih = MEASURE_Ih();
	Sample.Im = MEASURE_Im();
	Sample.U = MEASURE_TSP();
	Sample.P = _IQmpy(Sample.U, Sample.Ih);

	return Sample;
}
// ----------------------------------------

void MEASURE_CapVoltageSamplingStart()
{
	ZwADC_StartSEQ1();
}
// ----------------------------------------

Int16U MEASURE_CapVoltageSamplingResult()
{
	static Int16U AverageCounter = 0;
	static Int32S DataSum = 0;
	static Int16U DataArray[AVERAGE_ARRAY_SIZE];
	pInt16U ADC_DataRaw;

	ADC_DataRaw = ZwADC_GetValues1();

	// Average process
	AverageCounter++;
	AverageCounter &= AVERAGE_COUNTER_MASK;

	DataSum -= DataArray[AverageCounter];
	if(DataSum < 0)
		DataSum = 0;

	DataArray[AverageCounter] = CONVERT_ADCToCapVolatge(*ADC_DataRaw);
	DataSum += DataArray[AverageCounter];

	return (DataSum >> AVERAGE_RESULT_SHIFT);
}
// ----------------------------------------

