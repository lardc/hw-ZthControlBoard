﻿// Header
#include "Regulator.h"
//
// Includes
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "stdlib.h"
#include "ZbBoard.h"
#include "ConvertUtils.h"
#include "IQmathLib.h"
#include "IQmathUtils.h"
#include "ZthMCurrentBoard.h"
#include "Logic.h"


// Types
typedef struct __RegulatorSettings
{
	Boolean Enabled;
	Int32U Counter;
	_iq TargetValue;
	_iq TargetValuePrev;
	_iq Error;
	_iq Kp;
	_iq Ki;
	_iq Control;
	_iq ControlSat;
	Int16U *OutputArray;
	Int16U *ErrorArray;
	Int16U *ArrayCounter;
	Int16U *ArrayLocalCounter;
} RegulatorSettings, *pRegulatorSettings;
//

typedef struct __SaveDataParams
{
	Int16U *OutputArray;
	Int16U *ErrorArray;
	Int16U *ArrayCounter;
	Int16U *ArrayLocalCounter;
} SaveDataParams, *pSaveDataParams;


// Variables
SaveDataParams DataIm = {0}, DataIh = {0}, DataP = {0};
RegulatorSettings RegulatorIm = {0}, RegulatorIh = {0}, RegulatorP = {0};
RegulatorsData RegulatorSample;
_iq Ih_PrevPulseValue = 0, Ih_ErrorThreshold = 0, Ih_PulseValue = 0;
_iq P_TargetPulseValue = 0;
//
#pragma DATA_SECTION(REGULATOR_ErrorIh, "data_mem");
Int16U REGULATOR_ErrorIh[VALUES_x_SIZE];
#pragma DATA_SECTION(REGULATOR_ErrorIm, "data_mem");
Int16U REGULATOR_ErrorIm[VALUES_x_SIZE];
#pragma DATA_SECTION(REGULATOR_ErrorP, "data_mem");
Int16U REGULATOR_ErrorP[VALUES_x_SIZE];
#pragma DATA_SECTION(REGULATOR_Im_Value, "data_mem");
Int16U REGULATOR_Im_Value[VALUES_x_SIZE];
#pragma DATA_SECTION(REGULATOR_Ih_Value, "data_mem");
Int16U REGULATOR_Ih_Value[VALUES_x_SIZE];
#pragma DATA_SECTION(REGULATOR_P_Value, "data_mem");
Int16U REGULATOR_P_Value[VALUES_x_SIZE];
//
Int16U REGULATOR_Im_Counter = 0;
Int16U REGULATOR_Ih_Counter = 0;
Int16U REGULATOR_P_Counter = 0;
//
Int16U REGULATOR_Im_LocalCounter = 0;
Int16U REGULATOR_Ih_LocalCounter = 0;
Int16U REGULATOR_P_LocalCounter = 0;
//

// Forward functions
void REGULATOR_CycleX(RegulatorSelector Selector, RegulatorsData MeasureSample);
void REGULATOR_Init(RegulatorSelector Selector);
void REGULATOR_SavePowerTarget(_iq Power);
void REGULATOR_SaveData(pSaveDataParams DataParams, _iq Output, _iq Error);
void REGULATOR_InitX(pRegulatorSettings Regulator, _iq ControlSat, Int16U Register_Kp, Int16U Register_Ki);
void REGULATOR_InitSaveData(pSaveDataParams DataParams, Int16U *OutputArray, Int16U *ErrorArray, Int16U *ArrayCounter, Int16U *ArrayLocalCounter);

// Functions
//
void REGULATOR_Cycle(RegulatorsData MeasureSample)
{
	REGULATOR_CycleX(SelectIm, MeasureSample);
	REGULATOR_CycleX(SelectIh, MeasureSample);
	REGULATOR_CycleX(SelectP, MeasureSample);

	RegulatorSample = MeasureSample;
}
// ----------------------------------------

void REGULATOR_CycleX(RegulatorSelector Selector, RegulatorsData MeasureSample)
{
	_iq SampleValue;
	pRegulatorSettings Regulator;
	pSaveDataParams DataParams;



	switch (Selector)
	{
		case SelectIm:
			DataParams = &DataIm;
			Regulator = &RegulatorIm;
			SampleValue = MeasureSample.Im;
			break;

		case SelectIh:
			DataParams = &DataIh;
			Regulator = &RegulatorIh;
			SampleValue = MeasureSample.Ih;
			break;

		case SelectP:
			DataParams = &DataP;
			Regulator = &RegulatorP;
			AvgPowerDissipationDUT.Sample = MeasureSample.P;
			SampleValue = MEASURE_AveragingProcess(&AvgPowerDissipationDUT);

			if(!REGULATOR_GetTarget().P)
				return;
			break;
	}

	if(Regulator->Enabled)
	{
		_iq ControlI = 0;

		_iq Error = Regulator->TargetValuePrev - SampleValue;
		_iq RelativeError = ABS(_IQmpy(_IQdiv(Error, Regulator->TargetValuePrev), _IQ(100)));

		if(Selector == SelectIh)
		{
			if((RelativeError <= Ih_ErrorThreshold) && MeasureSample.P)
			{
				if(Ih_PulseValue != Ih_PrevPulseValue)
				{
					Ih_PrevPulseValue = Ih_PulseValue;
					P_TargetPulseValue = MeasureSample.P;

					REGULATOR_Update(SelectP, P_TargetPulseValue);
				}
				else
				{
					if(!REGULATOR_GetTarget().P)
						REGULATOR_Update(SelectP, P_TargetPulseValue);
				}

				REGULATOR_SavePowerTarget(P_TargetPulseValue);
			}
		}

		if(Regulator->Ki && (Regulator->Counter >= DataTable[REG_REGULATOR_SKIP_CYCLE]))
		{

			Regulator->Error += Error;

			if(_IQabs(Regulator->Error) > REGLTR_ERROR_I_SAT_H)
				Regulator->Error = (Regulator->Error > 0) ? REGLTR_ERROR_I_SAT_H : _IQmpy(_IQ(-1), REGLTR_ERROR_I_SAT_H);

			ControlI = _IQmpy(Regulator->Error, Regulator->Ki);
		}
		else
		{
			Regulator->Error = 0;
			ControlI = 0;
		}

		if(Regulator->Counter >= DataTable[REG_REGULATOR_SKIP_CYCLE])
			Regulator->Control = Regulator->TargetValue + _IQmpy(Error, Regulator->Kp) + ControlI;
		else
			Regulator->Control = Regulator->TargetValue;

		Regulator->TargetValuePrev = Regulator->TargetValue;

		if(Regulator->Control < 0)
			Regulator->Control = 0;
		else if(Regulator->Control > Regulator->ControlSat)
			Regulator->Control = Regulator->ControlSat;

		REGULATOR_SetOutput(Selector, Regulator->Control);

		REGULATOR_SaveData(DataParams, SampleValue, Error);

		Regulator->Counter++;
	}
}
// ----------------------------------------

void REGULATOR_InitX(pRegulatorSettings Regulator, _iq ControlSat, Int16U Register_Kp, Int16U Register_Ki)
{
	Regulator->Enabled				= FALSE;
	Regulator->Counter				= 0;
	Regulator->Kp					= _FPtoIQ2(DataTable[Register_Kp], 1000);
	Regulator->Ki 					= _FPtoIQ2(DataTable[Register_Ki], 1000);
	Regulator->ControlSat 			= ControlSat;

	Regulator->Control 				= 0;
	Regulator->Error 				= 0;
	Regulator->TargetValue			= 0;
	Regulator->TargetValuePrev		= 0;
}
// ----------------------------------------

void REGULATOR_InitSaveData(pSaveDataParams DataParams, Int16U *OutputArray, Int16U *ErrorArray, Int16U *ArrayCounter, Int16U *ArrayLocalCounter)
{
	DataParams->OutputArray			= OutputArray;
	DataParams->ErrorArray			= ErrorArray;
	DataParams->ArrayCounter		= ArrayCounter;
	DataParams->ArrayLocalCounter	= ArrayLocalCounter;
}
// ----------------------------------------

Boolean REGULATOR_IsIErrorSaturated(RegulatorSelector Selector)
{
	switch (Selector)
	{
		case SelectIm: return (_IQabs(RegulatorIm.Error) == REGLTR_ERROR_I_SAT_H);
		case SelectIh: return (_IQabs(RegulatorIh.Error) == REGLTR_ERROR_I_SAT_H);
		case SelectP: return (_IQabs(RegulatorIh.Error) == REGLTR_ERROR_I_SAT_H);
	}

	return FALSE;
}
// ----------------------------------------

void REGULATOR_Enable(RegulatorSelector Selector, Boolean State)
{
	switch (Selector)
	{
		case SelectIm: RegulatorIm.Enabled = State; break;
		case SelectIh:RegulatorIh.Enabled = State; break;
		case SelectP: RegulatorP.Enabled = State; break;
	}
}
// ----------------------------------------

void REGULATOR_Update(RegulatorSelector Selector, _iq Value)
{
	switch (Selector)
	{
		case SelectIm: RegulatorIm.TargetValue = Value; break;
		case SelectIh: RegulatorIh.TargetValue = Value; break;
		case SelectP: RegulatorP.TargetValue = Value; break;
	}
}
// ----------------------------------------

void REGULATOR_SetOutput(RegulatorSelector Selector, _iq Value)
{
	switch (Selector)
	{
		case SelectIm:
			RegulatorIm.Control = Value;
			ZthMCB_CurrentSet(Value);
			break;

		case SelectP:
			RegulatorP.Control = Value;
			REGULATOR_Update(SelectIh, _IQdiv(RegulatorP.Control, _IQdiv(RegulatorSample.U, _IQI(1000))));
			break;

		case SelectIh:
			RegulatorIh.Control = Value;

			if(Value)
			{
				ZbDAC_Write(CONVERT_IhToDAC(Value), &ZbGPIO_RegisterRCLK, TRUE);

				if(TimeFlag)
				{
					ZbGPIO_DRCU_Sync(TRUE);
					LOGIC_StartTimeCounter(0);
				}
			}
			else
				ZbDAC_Write(BIT15, &ZbGPIO_RegisterRCLK, TRUE);
			break;
	}
}
// ----------------------------------------

void REGULATOR_Init(RegulatorSelector Selector)
{
	switch (Selector)
	{
		case SelectIm:
			REGULATOR_InitX(&RegulatorIm, REGLTR_IM_SAT, REG_PI_CTRL_IM_Kp, REG_PI_CTRL_IM_Ki);
			REGULATOR_InitSaveData(&DataIm, &REGULATOR_Im_Value[0], &REGULATOR_ErrorIm[0], &REGULATOR_Im_Counter, &REGULATOR_Im_LocalCounter);
			break;

		case SelectIh:
			REGULATOR_InitX(&RegulatorIh, REGLTR_IH_SAT, REG_PI_CTRL_IH_Kp, REG_PI_CTRL_IH_Ki);
			REGULATOR_InitSaveData(&DataIh, &REGULATOR_Ih_Value[0], &REGULATOR_ErrorIh[0], &REGULATOR_Ih_Counter, &REGULATOR_Ih_LocalCounter);
			break;

		case SelectP:
			REGULATOR_InitX(&RegulatorP, REGLTR_IH_SAT, REG_PI_CTRL_P_Kp, REG_PI_CTRL_P_Ki);
			REGULATOR_InitSaveData(&DataP, &REGULATOR_P_Value[0], &REGULATOR_ErrorP[0], &REGULATOR_P_Counter, &REGULATOR_P_LocalCounter);
			break;
	}
}
// ----------------------------------------

void REGULATOR_Ih_CacheValue(_iq Current)
{
	Ih_PulseValue = Current;
}
// ----------------------------------------

void REGULATOR_CacheVariables()
{
	Ih_ErrorThreshold = _FPtoIQ2(DataTable[REG_I_ERR_THRESHOLD], 10);
}
// ----------------------------------------

void REGULATOR_ResetVariables()
{
	Ih_PrevPulseValue = 0;
	P_TargetPulseValue = 0;
}
// ----------------------------------------

void REGULATOR_DisableAll()
{
	REGULATOR_Enable(SelectIm, FALSE);
	REGULATOR_Enable(SelectIh, FALSE);
	REGULATOR_Enable(SelectP, FALSE);

	REGULATOR_ResetVariables();
}
// ----------------------------------------

void REGULATOR_InitAll()
{
	REGULATOR_Init(SelectIm);
	REGULATOR_Init(SelectIh);
	REGULATOR_Init(SelectP);
}
// ----------------------------------------

void REGULATOR_ForceOutputsToZero()
{
	REGULATOR_SetOutput(SelectIm, 0);
	REGULATOR_SetOutput(SelectIh, 0);
	REGULATOR_SetOutput(SelectP, 0);
}
// ----------------------------------------

RegulatorsData REGULATOR_GetControl()
{
	RegulatorsData ret;

	ret.Im = RegulatorIm.Control;
	ret.Ih = RegulatorIh.Control;
	ret.P = RegulatorIh.Control;

	return ret;
}
// ----------------------------------------

RegulatorsData REGULATOR_GetTarget()
{
	RegulatorsData ret;

	ret.Im = RegulatorIm.TargetValue;
	ret.Ih = RegulatorIh.TargetValue;
	ret.P = RegulatorP.TargetValue;

	return ret;
}
// ----------------------------------------

void REGULATOR_SaveData(pSaveDataParams DataParams, _iq Output, _iq Error)
{
	static Int16U ScopeLogStep = 0;

	if (*DataParams->ArrayCounter == 0)
		*DataParams->ArrayLocalCounter = 0;

	if (ScopeLogStep++ >= DataTable[REG_SCOPE_STEP])
	{
		ScopeLogStep = 0;

		*(DataParams->OutputArray + *DataParams->ArrayCounter) = _IQint(Output);
		*(DataParams->ErrorArray + *DataParams->ArrayCounter) = _IQint(_IQmpy(Error, _IQI(100)));
		*DataParams->ArrayCounter = *DataParams->ArrayLocalCounter;

		++*DataParams->ArrayLocalCounter;
	}

	if (*DataParams->ArrayCounter < REGULATOR_VALUES_SIZE)
		*DataParams->ArrayCounter = *DataParams->ArrayLocalCounter;

	if (*DataParams->ArrayLocalCounter >= REGULATOR_VALUES_SIZE)
		*DataParams->ArrayLocalCounter = 0;
}
// ----------------------------------------

void REGULATOR_SavePowerTarget(_iq Power)
{
	DataTable[REG_ACTUAL_P_TARGET_WHOLE] = _IQint(Power);
	DataTable[REG_ACTUAL_P_TARGET_FRACT] = _IQint(_IQmpy((Power - _IQI(DataTable[REG_ACTUAL_P_TARGET_WHOLE])), _IQI(100)));
}
// ----------------------------------------
