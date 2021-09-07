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
typedef struct __CombinedData
{
	_iq Im;
	_iq Ih;
	_iq	U;
	_iq	P;
} CombinedData, *pCombinedData;

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
CombinedData MEASURE_RegulatorsSample();

#endif /* MEASURINGPROCESSES_H_ */
