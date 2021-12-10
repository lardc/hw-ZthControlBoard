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
MovingAverageFilter AvgCapacitorsVoltage = {0};
MovingAverageFilter AvgMeasurementCurrent = {0};
MovingAverageFilter AvgPowerDissipationDUT = {0};
//
Int16U MEASURE_CapVoltage = 0;

// Functions prototypes
//
_iq MEASURE_MeasuredDataMux(pMovingAverageFilter Data);

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
	AvgMeasurementCurrent.Sample = CONVERT_ADCToIm(ZthSB_RawReadIm());
	return MEASURE_AveragingProcess(&AvgMeasurementCurrent);
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
	Sample.P = _IQmpy(_IQdiv(Sample.U, _IQI(1000)), Sample.Ih);

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
	AvgCapacitorsVoltage.Sample = CONVERT_ADCToCapVolatge(*(Int16U *)pResults);
	MEASURE_CapVoltage = DataTable[REG_ACTUAL_CAP_VOLTAGE] = _IQint(MEASURE_AveragingProcess(&AvgCapacitorsVoltage));
}
// ----------------------------------------

_iq MEASURE_AveragingProcess(pMovingAverageFilter Data)
{
	if(DataTable[REG_FILTER_ACTIVE])
	{
		Data->DataSum -= Data->Array[Data->Counter];
		Data->Array[Data->Counter] = Data->Sample;
		Data->DataSum += Data->Array[Data->Counter];

		Data->Counter++;
		if(Data->Counter >= AVERAGE_DEGREE)
			Data->Counter = 0;

		return _IQdiv(Data->DataSum, _IQI(AVERAGE_DEGREE));
	}
	else
		return Data->Sample;
}
// ----------------------------------------

void MEASURE_VariablesPrepare()
{
	Int16U i;

	for(i = 0; i < AVERAGE_DEGREE; i++)
	{
		AvgMeasurementCurrent.Array[i] = 0;
		AvgCapacitorsVoltage.Array[i] = 0;
	}
	//
	AvgMeasurementCurrent.Counter = 0;
	AvgCapacitorsVoltage.Counter = 0;
	//
	AvgMeasurementCurrent.DataSum = 0;
	AvgCapacitorsVoltage.DataSum = 0;
	//
	AvgMeasurementCurrent.FilteredDataFlag = FALSE;
	AvgCapacitorsVoltage.FilteredDataFlag = FALSE;
}
// ----------------------------------------

