#ifndef CONVERTUTILS_H_
#define CONVERTUTILS_H_

// Include
//
#include "stdinc.h"
#include "ZwDSP.h"
#include "IQmathLib.h"

// Functions
//
void CONVERT_Cashe();
Int16U CONVERT_IgToDAC(_iq Current);
Int16U CONVERT_IhToDAC(_iq Current);
Int16U CONVERT_ImToDAC(_iq Current);
_iq CONVERT_ADCToIh(Int16U ADCData);
_iq CONVERT_ADCToIm(Int16U ADCData);
_iq CONVERT_ADCToTcase1(Int16U ADCData);
_iq CONVERT_ADCToTcase2(Int16U ADCData);
_iq CONVERT_ADCToTcool1(Int16U ADCData);
_iq CONVERT_ADCToTcool2(Int16U ADCData);
_iq CONVERT_ADCToTSP(Int16U ADCData);
Int16U CONVERT_ADCToCapVolatge(Int16U ADCData);

#endif /* CONVERTUTILS_H_ */
