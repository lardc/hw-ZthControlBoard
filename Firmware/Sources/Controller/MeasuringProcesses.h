// -----------------------------------------
// Measurment processes library
// ----------------------------------------

#ifndef MEASURINGPROCESSES_H_
#define MEASURINGPROCESSES_H_

//Include
#include <stdinc.h>
#include "SysConfig.h"
#include "Logic.h"
#include "Controller.h"
#include "IQmathLib.h"

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
}CombinedData, *pCombinedData;

// Functions
//
void MEASURE_CapVoltageSamplingStart();
Int16U MEASURE_CapVoltageSamplingResult();
_iq MEASURE_Tcase1();
_iq MEASURE_Tcase2();
_iq MEASURE_Tcool1();
_iq MEASURE_Tcool2();
_iq MEASURE_Ih();
_iq MEASURE_Im();
_iq MEASURE_TSP();
RegulatorsData MEASURE_RegulatorsSample();
CombinedData MEASURE_CombinedData(Boolean DoubleSideCooling);

#endif /* MEASURINGPROCESSES_H_ */
