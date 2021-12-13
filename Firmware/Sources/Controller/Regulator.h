#ifndef __REGULATOR_H
#define __REGULATOR_H

// Include
#include "stdinc.h"
#include "IQmathLib.h"
#include "MeasuringProcesses.h"
#include "Global.h"

// Types
//
typedef enum __RegulatorSelector
{
	SelectIm = 0,
	SelectIh,
	SelectP
} RegulatorSelector;

// Variables
//
extern Int16U REGULATOR_ErrorIm[VALUES_x_SIZE];
extern Int16U REGULATOR_ErrorIh[VALUES_x_SIZE];
extern Int16U REGULATOR_ErrorP[VALUES_x_SIZE];
extern Int16U REGULATOR_Im_Value[VALUES_x_SIZE];
extern Int16U REGULATOR_Ih_Value[VALUES_x_SIZE];
extern Int16U REGULATOR_P_Value[VALUES_x_SIZE];
//
extern Int16U REGULATOR_Im_Counter;
extern Int16U REGULATOR_Ih_Counter;
extern Int16U REGULATOR_P_Counter;
//
extern _iq P_TargetPulseValue;

// Functions
//
void REGULATOR_Cycle(RegulatorsData MeasureSample);
void REGULATOR_Enable(RegulatorSelector Selector, Boolean State);
void REGULATOR_Update(RegulatorSelector Selector, _iq Value);
void REGULATOR_SetOutput(RegulatorSelector Selector, _iq Value);
Boolean REGULATOR_IsIErrorSaturated(RegulatorSelector Selector);
void REGULATOR_ForceOutputsToZero();
void REGULATOR_InitAll();
void REGULATOR_DisableAll();
RegulatorsData REGULATOR_GetControl();
RegulatorsData REGULATOR_GetTarget();
void REGULATOR_CacheVariables();
void REGULATOR_ResetVariables();
void REGULATOR_Ih_CacheValue(_iq Current);

#endif // __REGULATOR_H
