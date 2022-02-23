// -----------------------------------------
// Measurment processes library
// ----------------------------------------

#ifndef MEASURINGPROCESSES_H_
#define MEASURINGPROCESSES_H_

//Include
#include <stdinc.h>
#include "SysConfig.h"
#include "IQmathLib.h"
#include "IQmathUtils.h"

// Definitions
//
#define AVERAGE_DEGREE			20

// Types
typedef struct __RegulatorsData
{
	_iq Im;
	_iq Ih;
	_iq	U;
	_iq	P;
} RegulatorsData, *pRegulatorsData;

typedef struct __CombinedData
{
	_iq TSP;
	_iq Tcase1;
	_iq Tcase2;
	_iq Tcool1;
	_iq Tcool2;
} CombinedData;

typedef struct __MovingAverageFilter
{
	_iq Sample;
	Int16U Counter;
	_iq DataSum;
	_iq Array[AVERAGE_DEGREE];
	_iq AvgResult;
	Boolean SwitchFlag;
}MovingAverageFilter, *pMovingAverageFilter;

// Variables
extern Int16U MEASURE_CapVoltage;

// Functions
//
void MEASURE_CapVoltageSamplingStart();
void MEASURE_CapVoltageSamplingResult(Int16U * const restrict pResults);
_iq MEASURE_Tcase1();
_iq MEASURE_Tcase2();
_iq MEASURE_Tcool1();
_iq MEASURE_Tcool2();
_iq MEASURE_Ih();
_iq MEASURE_Im();
_iq MEASURE_TSP();
RegulatorsData MEASURE_RegulatorsSample();
CombinedData MEASURE_CombinedData(Boolean DoubleSideCooling);
void MEASURE_VariablesPrepare();
_iq MEASURE_AveragingProcess(pMovingAverageFilter Data);

#endif /* MEASURINGPROCESSES_H_ */
