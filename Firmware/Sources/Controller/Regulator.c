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
#include "Global.h"


// Types
typedef struct __RegulatorSettings
{
	Boolean Enabled;
	_iq TargetValue;
	_iq TargetValuePrev;
	_iq ErrorI;
	_iq Kp;
	_iq Ki;
	_iq Control;
	_iq ControlSat;
} RegulatorSettings, *pRegulatorSettings;


// Variables
RegulatorSettings RegulatorIm = {0}, RegulatorIh = {0};


// Forward functions
void REGULATOR_CycleX(RegulatorSelector Selector, CombinedData MeasureSample);
void REGULATOR_Init(RegulatorSelector Selector);
void REGULATOR_InitX(pRegulatorSettings Regulator, _iq ControlSat, Int16U Register_Kp, Int16U Register_Ki);


// Functions
//
void REGULATOR_Cycle(CombinedData MeasureSample)
{
	REGULATOR_CycleX(SelectIm, MeasureSample);
	REGULATOR_CycleX(SelectIh, MeasureSample);
}
// ----------------------------------------

void REGULATOR_CycleX(RegulatorSelector Selector, CombinedData MeasureSample)
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
	}

	if(Regulator->Enabled)
	{
		_iq ControlI = 0;
		_iq Error = Regulator->TargetValuePrev - SampleValue;

		if(Regulator->Ki)
		{
			Regulator->ErrorI += Error;

			if(_IQabs(Regulator->ErrorI) > REGLTR_ERROR_I_SAT_H)
				Regulator->ErrorI = (Regulator->ErrorI > 0) ? REGLTR_ERROR_I_SAT_H : _IQmpy(_IQ(-1), REGLTR_ERROR_I_SAT_H);

			ControlI = _IQmpy(Regulator->ErrorI, Regulator->Ki);
		}
		else
		{
			Regulator->ErrorI = 0;
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

void REGULATOR_InitX(pRegulatorSettings Regulator, _iq ControlSat, Int16U Register_Kp, Int16U Register_Ki)
{
	Regulator->Enabled			= FALSE;
	Regulator->Kp				= _FPtoIQ2(DataTable[Register_Kp], 1000);
	Regulator->Ki 				= _FPtoIQ2(DataTable[Register_Ki], 1000);
	Regulator->ControlSat 		= ControlSat;

	Regulator->Control 			= 0;
	Regulator->ErrorI 			= 0;
	Regulator->TargetValue		= 0;
	Regulator->TargetValuePrev	= 0;
}
// ----------------------------------------

Boolean REGULATOR_IsIErrorSaturated(RegulatorSelector Selector)
{
	switch (Selector)
	{
		case SelectIm: return (_IQabs(RegulatorIm.ErrorI) == REGLTR_ERROR_I_SAT_H);
		case SelectIh: return (_IQabs(RegulatorIh.ErrorI) == REGLTR_ERROR_I_SAT_H);
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
	}
}
// ----------------------------------------

void REGULATOR_Update(RegulatorSelector Selector, _iq Value)
{
	switch (Selector)
	{
		case SelectIm: RegulatorIm.TargetValue = Value; break;
		case SelectIh: RegulatorIh.TargetValue = Value; break;
	}
}
// ----------------------------------------

void REGULATOR_SetOutput(RegulatorSelector Selector, _iq Value)
{
	switch (Selector)
	{
		case SelectIm:
			RegulatorIm.Control = Value;
			ZthMCB_CurrentSet(CONVERT_ImToDAC(Value));
			break;

		case SelectIh:
			RegulatorIh.Control = Value;
			ZbDAC_Write(CONVERT_IhToDAC(Value), &ZbGPIO_RegisterRCLK);
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
			break;

		case SelectIh:
			REGULATOR_InitX(&RegulatorIh, REGLTR_IH_SAT, REG_PI_CTRL_IH_Kp, REG_PI_CTRL_IH_Ki);
			break;
	}
}
// ----------------------------------------

void REGULATOR_DisableAll()
{
	REGULATOR_Enable(SelectIm, FALSE);
	REGULATOR_Enable(SelectIh, FALSE);
}
// ----------------------------------------

void REGULATOR_InitAll()
{
	REGULATOR_Init(SelectIm);
	REGULATOR_Init(SelectIh);
}
// ----------------------------------------

void REGULATOR_ForceOutputsToZero()
{
	REGULATOR_SetOutput(SelectIm, 0);
	REGULATOR_SetOutput(SelectIh, 0);
}
// ----------------------------------------

CombinedData REGULATOR_GetControl()
{
	CombinedData ret;

	ret.Im = RegulatorIm.Control;
	ret.Ih = RegulatorIh.Control;

	return ret;
}
// ----------------------------------------

CombinedData REGULATOR_GetTarget()
{
	CombinedData ret;

	ret.Im = RegulatorIm.TargetValue;
	ret.Ih = RegulatorIh.TargetValue;

	return ret;
}
// ----------------------------------------
