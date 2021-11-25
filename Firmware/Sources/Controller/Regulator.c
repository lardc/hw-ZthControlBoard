// Header
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


// Types
typedef struct __RegulatorSettings
{
	Boolean Enabled;
	_iq TargetValue;
	_iq TargetValuePrev;
	_iq Error;
	_iq Kp;
	_iq Ki;
	_iq Control;
	_iq ControlSat;
	Int16U *ErrorArray;
	Int16U *ArrayCounter;
} RegulatorSettings, *pRegulatorSettings;


// Variables
RegulatorSettings RegulatorIm = {0}, RegulatorIh = {0}, RegulatorP = {0};
RegulatorsData RegulatorSample;
_iq RegulatorSavedPowerTarget = 0, RegulatorPowerErrorThreshold = 0;
//
#pragma DATA_SECTION(REGULATOR_ErrorIh, "data_mem");
Int16U REGULATOR_ErrorIh[VALUES_x_SIZE];
#pragma DATA_SECTION(REGULATOR_ErrorIm, "data_mem");
Int16U REGULATOR_ErrorIm[VALUES_x_SIZE];
#pragma DATA_SECTION(REGULATOR_ErrorP, "data_mem");
Int16U REGULATOR_ErrorP[VALUES_x_SIZE];
Int16U REGULATOR_ErrorIm_Counter = 0;
Int16U REGULATOR_ErrorIh_Counter = 0;
Int16U REGULATOR_ErrorP_Counter = 0;
//

// Forward functions
void REGULATOR_CycleX(RegulatorSelector Selector, RegulatorsData MeasureSample);
void REGULATOR_Init(RegulatorSelector Selector);
void REGULATOR_InitX(pRegulatorSettings Regulator, _iq ControlSat, Int16U Register_Kp, Int16U Register_Ki, Int16U *Array, Int16U *ArrayCounter);
void REGULATOR_SaveErr(pRegulatorSettings Regulator, _iq Error);


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

	switch (Selector)
	{
		case SelectIm:
			Regulator = &RegulatorIm;
			SampleValue = MeasureSample.Im;
			break;

		case SelectIh:
			Regulator = &RegulatorIh;
			SampleValue = MeasureSample.Ih;
			break;

		case SelectP:
			Regulator = &RegulatorP;
			SampleValue = MeasureSample.P;
			break;
	}

	if(Regulator->Enabled)
	{
		_iq ControlI = 0;
		_iq Error = Regulator->TargetValuePrev - SampleValue;

		REGULATOR_SaveErr(Regulator, Error);

		if(Selector == SelectIh)
		{
			if(!RegulatorSavedPowerTarget && RegulatorIh.TargetValue && (ABS(Error) <= RegulatorPowerErrorThreshold))
			{
				RegulatorSavedPowerTarget = MeasureSample.P;
				REGULATOR_Update(SelectP, RegulatorSavedPowerTarget);
			}
		}

		if(Regulator->Ki)
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

		Regulator->Control = Regulator->TargetValue + _IQmpy(Error, Regulator->Kp) + ControlI;
		Regulator->TargetValuePrev = Regulator->TargetValue;

		if(Regulator->Control < 0)
			Regulator->Control = 0;
		else if(Regulator->Control > Regulator->ControlSat)
			Regulator->Control = Regulator->ControlSat;

		REGULATOR_SetOutput(Selector, Regulator->Control);
	}
}
// ----------------------------------------

void REGULATOR_InitX(pRegulatorSettings Regulator, _iq ControlSat, Int16U Register_Kp, Int16U Register_Ki, Int16U *Array, Int16U *ArrayCounter)
{
	Regulator->Enabled			= FALSE;
	Regulator->Kp				= _FPtoIQ2(DataTable[Register_Kp], 1000);
	Regulator->Ki 				= _FPtoIQ2(DataTable[Register_Ki], 1000);
	Regulator->ControlSat 		= ControlSat;

	Regulator->Control 			= 0;
	Regulator->Error 			= 0;
	Regulator->TargetValue		= 0;
	Regulator->TargetValuePrev	= 0;

	Regulator->ErrorArray		= Array;
	Regulator->ArrayCounter		= ArrayCounter;
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
		case SelectIh: RegulatorIh.Enabled = State; break;
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
			REGULATOR_Update(SelectIh, _IQmpy(RegulatorP.Control, RegulatorSample.U));
			break;

		case SelectIh:
			RegulatorIh.Control = Value;

			if(Value)
			{
				ZbGPIO_DRCU_Sync(TRUE);
				ZbDAC_Write(CONVERT_IhToDAC(Value), &ZbGPIO_RegisterRCLK, TRUE);
			}
			else
			{
				ZbDAC_Write(BIT15, &ZbGPIO_RegisterRCLK, TRUE);
				ZbGPIO_DRCU_Sync(FALSE);
			}
			break;
	}
}
// ----------------------------------------

void REGULATOR_Init(RegulatorSelector Selector)
{
	switch (Selector)
	{
		case SelectIm:
			REGULATOR_InitX(&RegulatorIm, REGLTR_IM_SAT, REG_PI_CTRL_IM_Kp, REG_PI_CTRL_IM_Ki, &REGULATOR_ErrorIm[0], &REGULATOR_ErrorIm_Counter);
			break;

		case SelectIh:
			REGULATOR_InitX(&RegulatorIh, REGLTR_IH_SAT, REG_PI_CTRL_IH_Kp, REG_PI_CTRL_IH_Ki, &REGULATOR_ErrorIh[0], &REGULATOR_ErrorIh_Counter);
			break;

		case SelectP:
			REGULATOR_InitX(&RegulatorP, REGLTR_IH_SAT, REG_PI_CTRL_P_Kp, REG_PI_CTRL_P_Ki, &REGULATOR_ErrorP[0], &REGULATOR_ErrorP_Counter);
			break;
	}
}
// ----------------------------------------

void REGULATOR_CacheVariables()
{
	RegulatorSavedPowerTarget = 0;
	RegulatorPowerErrorThreshold = _FPtoIQ2(DataTable[REG_P_ERROR_THRESHOLD], 10);
}
// ----------------------------------------

void REGULATOR_DisableAll()
{
	REGULATOR_Enable(SelectIm, FALSE);
	REGULATOR_Enable(SelectIh, FALSE);
	REGULATOR_Enable(SelectP, FALSE);
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

void REGULATOR_SaveErr(pRegulatorSettings Regulator, _iq Error)
{
	static Int16U ScopeLogStep = 0, LocalCounter = 0;

	if (*Regulator->ArrayCounter == 0)
		LocalCounter = 0;

	if (ScopeLogStep++ >= DataTable[REG_SCOPE_STEP])
	{
		ScopeLogStep = 0;

		*(Regulator->ErrorArray + *Regulator->ArrayCounter) = _IQint(_IQmpy(Error, _IQI(100)));
		*Regulator->ArrayCounter = LocalCounter;

		++LocalCounter;
	}

	if (*Regulator->ArrayCounter < REGULATOR_VALUES_SIZE)
		*Regulator->ArrayCounter = LocalCounter;

	if (LocalCounter >= REGULATOR_VALUES_SIZE)
		LocalCounter = 0;
}
// ----------------------------------------
