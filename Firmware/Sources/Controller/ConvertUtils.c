// Header
//
#include "ConvertUtils.h"

// Include
//
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "stdlib.h"
#include "IQmathUtils.h"
#include "ZbDAC.h"

// Types
typedef struct __ConvParameters
{
	_iq P2;
	_iq P1;
	_iq P0;
	_iq K;
	_iq B;
} ConvParameters, *pConvParameters;

// Variables
//
static ConvParameters DAC_ParamsIg, DAC_ParamsIm;
static ConvParameters DAC_ParamsIh_Range0, DAC_ParamsIh_Range1, DAC_ParamsIh;
static ConvParameters ADC_ParamsIh_Range0, ADC_ParamsIh_Range1, ADC_ParamsIh;
static ConvParameters ADC_ParamsIm, ADC_ParamsTSP, ADC_ParamsTcase1, ADC_ParamsTcase2, ADC_ParamsTcool1, ADC_ParamsTcool2;
_iq CapVoltageK = 0;

// Functions prototypes
ConvParameters CONVERT_LoadParams(Int16U RegP2, Int16U RegP1, Int16U RegP0, Int16U RegK, Int16U RegB, Int16U RegBDiv);
ConvParameters CONVERT_LoadParamsSimple(Int16U RegK, Int16U RegB);
_iq CONVERT_ADCToX(Int16U ADCInput, ConvParameters Parameters);
Int16U CONVERT_xToDAC(_iq Value, ConvParameters Parameters);

// Functions
//
void CONVERT_CacheVariables()
{
	CapVoltageK = _FPtoIQ2(DataTable[REG_CAP_VOLTAGE_K_N], DataTable[REG_CAP_VOLTAGE_K_D]);
	//
	DAC_ParamsIg = CONVERT_LoadParams(REG_DAC_IG_P2, REG_DAC_IG_P1, REG_DAC_IG_P0, REG_DAC_IG_K, REG_DAC_IG_B, 1);
	DAC_ParamsIh = DAC_ParamsIh_Range0 = CONVERT_LoadParamsSimple(REG_DAC_IH_R0_K, REG_DAC_IH_R0_B);
	DAC_ParamsIh_Range1 = CONVERT_LoadParamsSimple(REG_DAC_IH_R1_K, REG_DAC_IH_R1_B);
	DAC_ParamsIm = CONVERT_LoadParamsSimple(REG_DAC_IM_K, REG_DAC_IM_B);
	//
	ADC_ParamsIh = ADC_ParamsIh_Range0 = CONVERT_LoadParams(REG_ADC_IH_R0_P2, REG_ADC_IH_R0_P1, REG_ADC_IH_R0_P0, REG_ADC_IH_R0_K, REG_ADC_IH_R0_B, 1);
	ADC_ParamsIh_Range1 = CONVERT_LoadParams(REG_ADC_IH_R1_P2, REG_ADC_IH_R1_P1, REG_ADC_IH_R1_P0, REG_ADC_IH_R1_K, REG_ADC_IH_R1_B, 1);
	ADC_ParamsIm = CONVERT_LoadParams(REG_ADC_IM_P2, REG_ADC_IM_P1, REG_ADC_IM_P0, REG_ADC_IM_K, REG_ADC_IM_B, 100);
	ADC_ParamsTSP = CONVERT_LoadParams(REG_ADC_TSP_P2, REG_ADC_TSP_P1, REG_ADC_TSP_P0, REG_ADC_TSP_K, REG_ADC_TSP_B, 100);
	ADC_ParamsTcase1 = CONVERT_LoadParams(REG_ADC_T_CASE1_P2, REG_ADC_T_CASE1_P1, REG_ADC_T_CASE1_P0, REG_ADC_T_K, REG_ADC_T_B, 100);
	ADC_ParamsTcase2 = CONVERT_LoadParams(REG_ADC_T_CASE2_P2, REG_ADC_T_CASE2_P1, REG_ADC_T_CASE2_P0, REG_ADC_T_K, REG_ADC_T_B, 100);
	ADC_ParamsTcool1 = CONVERT_LoadParams(REG_ADC_T_COOL1_P2, REG_ADC_T_COOL1_P1, REG_ADC_T_COOL1_P0, REG_ADC_T_K, REG_ADC_T_B, 100);
	ADC_ParamsTcool2 = CONVERT_LoadParams(REG_ADC_T_COOL2_P2, REG_ADC_T_COOL2_P1, REG_ADC_T_COOL2_P0, REG_ADC_T_K, REG_ADC_T_B, 100);
}
// ----------------------------------------

void CONVERT_IhSetRangeParams(Boolean Range)
{
	if(Range)
	{
		ADC_ParamsIh = ADC_ParamsIh_Range1;
		DAC_ParamsIh = DAC_ParamsIh_Range1;
	}
	else
	{
		ADC_ParamsIh = ADC_ParamsIh_Range0;
		DAC_ParamsIh = DAC_ParamsIh_Range0;
	}
}
// ----------------------------------------

Int16U CONVERT_IgToDAC(_iq Current)
{
	return CONVERT_xToDAC(Current, DAC_ParamsIg);
}
// ----------------------------------------

Int16U CONVERT_IhToDAC(_iq Current)
{
	return CONVERT_xToDAC(Current, DAC_ParamsIh);
}
// ----------------------------------------

Int16U CONVERT_ImToDAC(_iq Current)
{
	return CONVERT_xToDAC(Current, DAC_ParamsIm);
}
// ----------------------------------------

_iq CONVERT_ADCToIh(Int16U ADCData)
{
	return CONVERT_ADCToX(ADCData, ADC_ParamsIh);
}
// ----------------------------------------

_iq CONVERT_ADCToIm(Int16U ADCData)
{
	return CONVERT_ADCToX(ADCData, ADC_ParamsIm);
}
// ----------------------------------------

_iq CONVERT_ADCToTcase1(Int16U ADCData)
{
	return CONVERT_ADCToX(ADCData, ADC_ParamsTcase1);
}
// ----------------------------------------

_iq CONVERT_ADCToTcase2(Int16U ADCData)
{
	return CONVERT_ADCToX(ADCData, ADC_ParamsTcase2);
}
// ----------------------------------------

_iq CONVERT_ADCToTcool1(Int16U ADCData)
{
	return CONVERT_ADCToX(ADCData, ADC_ParamsTcool1);
}
// ----------------------------------------

_iq CONVERT_ADCToTcool2(Int16U ADCData)
{
	return CONVERT_ADCToX(ADCData, ADC_ParamsTcool2);
}
// ----------------------------------------

_iq CONVERT_ADCToTSP(Int16U ADCData)
{
	return CONVERT_ADCToX(ADCData, ADC_ParamsTSP);
}
// ----------------------------------------

_iq CONVERT_ADCToCapVolatge(Int16U ADCData)
{
	return _IQmpyI32(CapVoltageK, ADCData);
}
// ----------------------------------------

_iq CONVERT_ADCToX(Int16U ADCInput, ConvParameters Parameters)
{
	_iq tmp = _IQmpy(_IQI(ADCInput), Parameters.K) + Parameters.B;
	_iq tmp2 = _IQdiv(tmp, _IQ(1000.0f));

	_iq val = _IQmpy(tmp2, _IQmpy(tmp2, Parameters.P2)) + _IQmpy(tmp, Parameters.P1) + Parameters.P0;
	return (val > 0) ? val : 0;
}
// ----------------------------------------

Int16U CONVERT_xToDAC(_iq Value, ConvParameters Parameters)
{
	_iq tmp = _IQdiv(Value, _IQ(1000.0f));
	_iq tmp2 = _IQmpy(tmp, _IQmpy(tmp, Parameters.P2)) + _IQmpy(Value, Parameters.P1) + Parameters.P0;

	Int16S val = _IQint(_IQmpy(tmp2, Parameters.K) + Parameters.B);

	if (val > DAC_MAX_VALUE)
		return DAC_MAX_VALUE;
	else if (val < 0)
		return 0;
	else
		return val;
}
// ----------------------------------------

ConvParameters CONVERT_LoadParams(Int16U RegP2, Int16U RegP1, Int16U RegP0, Int16U RegK, Int16U RegB, Int16U RegBDiv)
{
	ConvParameters ret;

	ret.P2	= _IQI((Int16S)DataTable[RegP2]);
	ret.P1	= _FPtoIQ2(DataTable[RegP1], 1000);
	ret.P0	= _FPtoIQ2(DataTable[RegP0], 100);
	//
	ret.K	= _FPtoIQ2(DataTable[RegK], 1000);
	ret.B	= _FPtoIQ2(DataTable[RegB], RegBDiv);

	return ret;
}
// ----------------------------------------

ConvParameters CONVERT_LoadParamsSimple(Int16U RegK, Int16U RegB)
{
	ConvParameters ret;

	ret.P2	= 0;
	ret.P1	= _IQI(1);
	ret.P0	= 0;
	//
	ret.K	= _FPtoIQ2(DataTable[RegK], 1000);
	ret.B	= _IQI((Int16S)DataTable[RegB]);

	return ret;
}
// ----------------------------------------




