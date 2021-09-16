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

// Variables
Int16U MEASURE_CapVoltage = 0;

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

RegulatorsData MEASURE_RegulatorsSample()
{
	RegulatorsData Sample;

	Sample.Ih = MEASURE_Ih();
	Sample.Im = MEASURE_Im();
	Sample.U = MEASURE_TSP();
	Sample.P = _IQmpy(Sample.U, Sample.Ih);

	return Sample;
}
// ----------------------------------------

CombinedData MEASURE_CombinedData(Boolean DoubleSideCooling)
{
	CombinedData Sample;

	Sample.TSP = MEASURE_TSP();
	Sample.Tcase1 = MEASURE_Tcase1();
	Sample.Tcool1 = MEASURE_Tcool1();
	Sample.Tcase2 = 0;
	Sample.Tcool2 = 0;

	if(DoubleSideCooling)
	{
		Sample.Tcase2 = MEASURE_Tcase2();
		Sample.Tcool2 = MEASURE_Tcool2();
	}

	return Sample;
}
// ----------------------------------------

void MEASURE_CapVoltageSamplingStart()
{
	ZwADC_StartSEQ1();
}
// ----------------------------------------

void MEASURE_CapVoltageSamplingResult(Int16U * const restrict pResults)
{
	static Int16U AverageCounter = 0;
	static Int32S DataSum = 0;
	static Int16U DataArray[AVERAGE_ARRAY_SIZE];
	Int16U ADC_DataRaw = *(Int16U *)pResults;

	// Average process
	AverageCounter++;
	AverageCounter &= AVERAGE_COUNTER_MASK;

	DataSum -= DataArray[AverageCounter];
	if(DataSum < 0)
		DataSum = 0;

	DataArray[AverageCounter] = CONVERT_ADCToCapVolatge(ADC_DataRaw);
	DataSum += DataArray[AverageCounter];

	MEASURE_CapVoltage = DataSum >> AVERAGE_RESULT_SHIFT;

	DataTable[REG_ACTUAL_CAP_VOLTAGE] = MEASURE_CapVoltage;
}
// ----------------------------------------

