// -----------------------------------------
// Main logic library
// ----------------------------------------

// Header
#include "Logic.h"
//
// Includes
#include "ZwDSP.h"
#include "xCCICommon.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Regulator.h"
#include "IQmathLib.h"
#include "IQMathUtils.h"
#include "ConvertUtils.h"
#include "ZbGPIO.h"
#include "Controller.h"
#include "ZthDUTControlBoard.h"
#include "DeviceProfile.h"
#include "Constraints.h"

// Definitions
//
#define DATA_LOG_STEP						100
//
#define TIME_PULSE_WIDTH_COMPENSATION		25		// in us
#define TIME_MSR_DEL_COMPENSATION			25		// in us
//
#define LOGIC_ARRAY_SIZE_ZTH				11
#define LOGIC_ARRAY_SIZE_TIME				9
//
#define ZTH_02								20		// Zth value x100
#define ZTH_01								10		// Zth value x100
//
#define REG_OUTPUT_CLEAR_CNT_MAX_VALUE		5000	// in us x0.01

// Variables
//
_iq LOGIC_ZthArray_1ms[LOGIC_ARRAY_SIZE_TIME][LOGIC_ARRAY_SIZE_ZTH] =
{
		{_IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.20f), _IQ(0.20f), _IQ(0.5f)},	// 100us
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.20f), _IQ(0.5f)},	// 200us
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.20f), _IQ(0.5f)},	// 300us
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.20f), _IQ(0.5f)},	// 400us
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.5f)},	// 500us
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.5f)},	// 600us
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.5f)},	// 700us
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.5f)},	// 800us
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.5f)},	// 900us
};
//
_iq LOGIC_ZthArray_10ms[LOGIC_ARRAY_SIZE_TIME][LOGIC_ARRAY_SIZE_ZTH] =
{
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.10f), _IQ(0.10f), _IQ(0.20f), _IQ(0.5f)},	// 1ms
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.10f), _IQ(0.2f)},	// 2ms
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.02f), _IQ(0.05f), _IQ(0.2f)},	// 3ms
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.2f)},	// 4ms
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.2f)},	// 5ms
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.2f)},	// 6ms
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.2f)},	// 7ms
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.2f)},	// 8ms
		{_IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.01f), _IQ(0.2f)},	// 9ms
};
//
Boolean CurrentGeneratedFlag = FALSE;
//
static Int16U HeatingCurrentSetpoint = 0;
//
volatile LogicState LOGIC_State = LS_None;
volatile Int32U LOGIC_TimeCounter = 0, LOGIC_HeatingTimeCounter = 0, LOGIC_CollingTime = 0;
volatile Int64U Timeout = 0;
//
volatile Int16U LOGIC_CapVoltageThreshold = 0;
volatile Int16U LOGIC_DUTType;
volatile Int16U LOGIC_CoolingMode;
volatile Int32U LOGIC_Pause, LOGIC_PulseWidthMin, LOGIC_PulseWidthMax, LOGIC_MeasurementDelay;
volatile Int16U LOGIC_CurrentWidthLess_2ms, LOGIC_CurrentWidthLess_10ms, LOGIC_CurrentWidthAbove_10ms, LOGIC_Tmax, LOGIC_ZthPause;
volatile Int16U LOGIC_MeasuringCurrent;
//
volatile Int32U LOGIC_ActualPulseWidth = 0;
volatile Int32U LOGIC_ActualDelayWidth = 0;
//
Int16U LOGIC_Values_TSP[VALUES_x_SIZE];
Int16U LOGIC_Values_Tcase1[VALUES_x_SIZE];
Int16U LOGIC_Values_Tcase2[VALUES_x_SIZE];
Int16U LOGIC_Values_Tcool1[VALUES_x_SIZE];
Int16U LOGIC_Values_Tcool2[VALUES_x_SIZE];
volatile Int16U LOGIC_Values_Counter = 0;
volatile Int16U EP_DataCounter = 0;
//

// Functions prototypes
//
void LOGIC_IndependentProcesses();
void LOGIC_RegulatorProcess();
void LOGIC_SaveHeatingData(RegulatorsData Sample);
void LOGIC_MeasuringCurrentConfig(Int16U Current);
void LOGIC_HeatingCurrentConfig(Int32U CuurentWidth);
void LOGIC_HeatingCurrentUpdate(Int32U CurrentWidth);
void LOGIC_MeasuringProcess();
void LOGIC_SaveData(CombinedData Sample, Boolean SaveToEndpoints);
void LOGIC_CalculateTimeInterval(volatile Int32U *TimeInterval);
void LOGIC_SaveToOutputRegisters();
Boolean LOGIC_BatteryVoltageControl(Int64U Timeout);
Boolean LOGIC_TimeCounterCheck(Int32U Time);
Boolean LOGIC_MeasurementDelayProcess(Int32U Delay, Int32U MsrDelayCompens, LogicState NextState);
Int32U LOGIC_CalculatePostPulseDelay(Int32U ActualCurrentWidth);

// Functions
//
void LOGIC_GateCurrentProcess()
{
	LOGIC_IndependentProcesses();
}
// ----------------------------------------

void LOGIC_MeasurementCurrentProcess()
{
	LOGIC_IndependentProcesses();
}
// ----------------------------------------

void LOGIC_HeatingCurrentProcess()
{
	LOGIC_IndependentProcesses();
}
// ----------------------------------------

void LOGIC_IndependentProcesses()
{
	if(CONTROL_State == DS_InProcess)
	{
		switch (LOGIC_State)
		{
		case LS_ConfigIg:
			LOGIC_GatePulse(TRUE);
			LOGIC_TimeCounterReset();
			//
			LOGIC_SetState(LS_PendingCompletion);
			break;

		case LS_ConfigIm:
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			LOGIC_TimeCounterReset();
			//
			LOGIC_SetState(LS_PendingCompletion);
			break;

		case LS_ConfigIh:
			LOGIC_HeatingCurrentConfig(LOGIC_PulseWidthMax);
			LOGIC_Heating(TRUE);
			LOGIC_TimeCounterReset();
			//
			LOGIC_SetState(LS_PendingCompletion);
			break;

		case LS_PendingCompletion:
			LOGIC_RegulatorProcess();

			LOGIC_TimeCounter++;

			if(LOGIC_TimeCounterCheck(LOGIC_PulseWidthMax))
			{
				CONTROL_SetDeviceState(DS_Ready, LS_None);
				CONTROL_StopProcess(OPRESULT_OK);
			}
			break;
		}
	}
}
// ----------------------------------------

void LOGIC_ZthSequencePulsesProcess()
{
	static Int32U MosfetProtectivePause;
	switch (LOGIC_State)
		{
		case LS_ConfigAll:
			LOGIC_ActualPulseWidth = LOGIC_PulseWidthMin;
			LOGIC_ActualDelayWidth = LOGIC_ActualPulseWidth * LOGIC_ZthPause / 100;
			//
			LOGIC_GatePulse(TRUE);
			LOGIC_HeatingCurrentConfig(LOGIC_ActualPulseWidth);
			MosfetProtectivePause = LOGIC_CalculatePostPulseDelay(LOGIC_ActualPulseWidth);
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			//
			LOGIC_SetState(LS_StartHeating);
			break;

		case LS_Cooling:
			if(LOGIC_TimeCounterCheck(MosfetProtectivePause) && LOGIC_TimeCounterCheck(LOGIC_ActualDelayWidth))
			{
				LOGIC_CalculateTimeInterval(&LOGIC_ActualPulseWidth);
				MosfetProtectivePause = LOGIC_CalculatePostPulseDelay(LOGIC_ActualPulseWidth);
				LOGIC_ActualDelayWidth = LOGIC_ActualPulseWidth * LOGIC_ZthPause / 100;
				LOGIC_SetState(LS_ChargeWaiting);
			}
			else
				break;

		case LS_ChargeWaiting:
			MEASURE_CapVoltageSamplingStart();
			DELAY_US(10);
			MEASURE_CapVoltageSamplingResult(ZwADC_GetValues1());

			if(MEASURE_CapVoltage >= LOGIC_CapVoltageThreshold)
				LOGIC_SetState(LS_StartHeating);
			else
				break;

		case LS_StartHeating:
			LOGIC_HeatingCurrentUpdate(LOGIC_ActualPulseWidth);
			LOGIC_TimeCounterReset();
			LOGIC_Heating(TRUE);
			LOGIC_SetState(LS_Heating);
			LOGIC_SetOperationState(OS_Heating);
			break;

		case LS_Heating:
			if(LOGIC_TimeCounterCheck(LOGIC_ActualPulseWidth))
			{
				DELAY_US(TIME_PULSE_WIDTH_COMPENSATION);
				LOGIC_Heating(FALSE);
				//
				LOGIC_TimeCounterReset();
				LOGIC_SetState(LS_MeasurementDelay);
			}
			else
				break;

		case LS_MeasurementDelay:
			if(!LOGIC_MeasurementDelayProcess(LOGIC_MeasurementDelay, TIME_MSR_DEL_COMPENSATION, LS_Measuring))
				break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();

			if(LOGIC_ActualPulseWidth >= LOGIC_PulseWidthMax)
			{
				CONTROL_SetDeviceState(DS_Ready, LS_None);
				LOGIC_SetOperationState(OS_None);
				CONTROL_StopProcess(OPRESULT_OK);
				return;
			}
			else
			{
				LOGIC_SetState(LS_Cooling);
				LOGIC_TimeCounter++;
				return;
			}
	}

	LOGIC_RegulatorProcess();
	DEVPROFILE_ResetEPReadState();
	LOGIC_SaveToOutputRegisters();
	LOGIC_TimeCounter++;
}
// ----------------------------------------

void LOGIC_ZthLongPulseProcess()
{
	switch (LOGIC_State)
	{
		case LS_ConfigAll:
			LOGIC_ActualDelayWidth = ZTH_PULSE_MIN_WIDTH_MIN;
			LOGIC_CalculateTimeInterval(&LOGIC_ActualDelayWidth);
			//
			LOGIC_GatePulse(TRUE);
			LOGIC_HeatingCurrentConfig(LOGIC_PulseWidthMax);
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			//
			LOGIC_SetState(LS_StartHeating);
			break;

		case LS_StartHeating:
			LOGIC_TimeCounterReset();
			LOGIC_Heating(TRUE);
			LOGIC_SetState(LS_Heating);
			LOGIC_SetOperationState(OS_Heating);
			break;

		case LS_Heating:
			REGULATOR_Update(SelectIm, _IQI(LOGIC_MeasuringCurrent));

			if(LOGIC_TimeCounterCheck(LOGIC_PulseWidthMax))
			{
				LOGIC_Heating(FALSE);
				//
				LOGIC_TimeCounterReset();
				LOGIC_SetState(LS_MeasurementDelay);
				LOGIC_SetOperationState(OS_Measuring);
			}
			else
				break;

		case LS_MeasurementDelay:
			if(!LOGIC_MeasurementDelayProcess(LOGIC_MeasurementDelay, 0, LS_Measuring))
				break;
			else
				LOGIC_TimeCounter = LOGIC_ActualDelayWidth;

		case LS_Measuring:
			LOGIC_MeasuringProcess();
			LOGIC_SetState(LS_Cooling);

		case LS_Cooling:
			if(LOGIC_TimeCounterCheck(LOGIC_ActualDelayWidth))
			{
				LOGIC_CalculateTimeInterval(&LOGIC_ActualDelayWidth);
				LOGIC_SetState(LS_Measuring);
			}
			break;
	}

	LOGIC_RegulatorProcess();
	DEVPROFILE_ResetEPReadState();
	LOGIC_SaveToOutputRegisters();
	LOGIC_TimeCounter++;
}
// ----------------------------------------

void LOGIC_RthSequenceProcess()
{
	switch (LOGIC_State)
	{
		case LS_ConfigAll:
			LOGIC_GatePulse(TRUE);
			LOGIC_HeatingCurrentConfig(LOGIC_PulseWidthMax);
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			//
			LOGIC_SetState(LS_StartHeating);
			break;

		case LS_Cooling:
			if(LOGIC_TimeCounterCheck(LOGIC_Pause))
				LOGIC_SetState(LS_StartHeating);
			else
				break;

		case LS_StartHeating:
			LOGIC_TimeCounterReset();
			LOGIC_Heating(TRUE);
			LOGIC_SetState(LS_Heating);
			LOGIC_SetOperationState(OS_Heating);
			break;

		case LS_Heating:
			if(LOGIC_TimeCounterCheck(LOGIC_PulseWidthMax))
			{
				LOGIC_Heating(FALSE);
				//
				LOGIC_TimeCounterReset();
				LOGIC_SetState(LS_MeasurementDelay);
			}
			else
				break;

		case LS_MeasurementDelay:
			if(!LOGIC_MeasurementDelayProcess(LOGIC_MeasurementDelay, 0, LS_Measuring))
				break;

		case LS_Measuring:
			LOGIC_MeasuringProcess();
			LOGIC_SetState(LS_Cooling);
			LOGIC_TimeCounter++;
			return;
	}

	LOGIC_RegulatorProcess();
	DEVPROFILE_ResetEPReadState();
	LOGIC_SaveToOutputRegisters();
	LOGIC_TimeCounter++;
}
// ----------------------------------------

void LOGIC_Graduation()
{
	static Boolean HeatingProcess = FALSE;

	switch (LOGIC_State)
	{
		case LS_ConfigAll:
			LOGIC_ActualDelayWidth = ZTH_PULSE_MIN_WIDTH_MIN;
			//
			LOGIC_GatePulse(TRUE);
			LOGIC_HeatingCurrentConfig(LOGIC_PulseWidthMax);
			LOGIC_MeasuringCurrentConfig(LOGIC_MeasuringCurrent);
			REGULATOR_Enable(SelectIm, TRUE);
			//
			LOGIC_SetState(LS_StartHeating);
			break;

		case LS_Cooling:
			if(HeatingProcess)
			{
				if(LOGIC_TimeCounterCheck(LOGIC_Pause))
					LOGIC_SetState(LS_StartHeating);
				else
					break;
			}
			else
			{
				if(LOGIC_TimeCounterCheck(LOGIC_ActualDelayWidth))
				{
					LOGIC_CalculateTimeInterval(&LOGIC_ActualDelayWidth);
					LOGIC_SetState(LS_Measuring);

				}
				break;
			}

		case LS_StartHeating:
			HeatingProcess = TRUE;
			LOGIC_TimeCounterReset();
			LOGIC_Heating(TRUE);
			LOGIC_SetState(LS_Heating);
			LOGIC_SetOperationState(OS_Heating);
			break;

		case LS_Heating:
			if(DataTable[REG_ACTUAL_T_CASE1] >= LOGIC_Tmax)
			{
				HeatingProcess = FALSE;
				LOGIC_Heating(FALSE);
				//
				LOGIC_TimeCounterReset();
				LOGIC_SetState(LS_MeasurementDelay);
				LOGIC_SetOperationState(OS_Measuring);
			}
			else
			{
				if(LOGIC_TimeCounterCheck(LOGIC_PulseWidthMax))
				{
					LOGIC_Heating(FALSE);
					//
					LOGIC_TimeCounterReset();
					LOGIC_SetState(LS_MeasurementDelay);
				}
				else
					break;
			}

		case LS_MeasurementDelay:
			if(!LOGIC_MeasurementDelayProcess(LOGIC_MeasurementDelay, 0, LS_Measuring))
				break;

		case LS_Measuring:
			(HeatingProcess) ? LOGIC_SaveToOutputRegisters() : LOGIC_MeasuringProcess();
			LOGIC_SetState(LS_Cooling);
			LOGIC_TimeCounter++;
			return;
	}

	LOGIC_RegulatorProcess();
	DEVPROFILE_ResetEPReadState();
	LOGIC_SaveToOutputRegisters();
	LOGIC_TimeCounter++;
}
// ----------------------------------------

void LOGIC_RegulatorProcess()
{
	RegulatorsData Sample;

	// Measurement process
	Sample = MEASURE_RegulatorsSample();

	// Regulator process
	REGULATOR_Cycle(Sample);

	// Save data to output regusters
	LOGIC_SaveHeatingData(Sample);
}
// ----------------------------------------

Int32U LOGIC_CalculatePostPulseDelay(Int32U ActualCurrentWidth)
{
	_iq CurrentPerMosfet, MosfetPower, MosfetVds, D;
	Int16U Zth, Pause = 0;

	if(ActualCurrentWidth < TIME_10MS)
	{
		MEASURE_CapVoltage = 240;
		MosfetVds = _FPtoIQ2((MEASURE_CapVoltage - POWER_LINE_DROP_VOLTAGE), 10);
		CurrentPerMosfet = _FPtoIQ2(HeatingCurrentSetpoint, ZTH_MOSFET_QUANTITY);
		MosfetPower = _IQmpy(CurrentPerMosfet, MosfetVds);
		Zth = _IQint(_IQmpy(_IQdiv(_IQI(MAX_JUNCTION_TEMPERATURE - MAX_CASE_TEMPERATURE), MosfetPower),_IQI(100)));

		if(Zth >= ZTH_02)
			Zth = LOGIC_ARRAY_SIZE_ZTH;
		else
		{
			if(Zth > ZTH_01)
				Zth = ZTH_01;
		}

		if(ActualCurrentWidth < TIME_1MS)
			D = LOGIC_ZthArray_1ms[ActualCurrentWidth - 1][Zth - 1];
		else
		{
			Int32U CurrentWidthTemp = ActualCurrentWidth / 10;
			D = LOGIC_ZthArray_10ms[CurrentWidthTemp - 1][Zth - 1];
		}

		Pause = _IQint(_IQdiv(_IQI(ActualCurrentWidth), D)) - ActualCurrentWidth;
	}

	return Pause;
}
// ----------------------------------------

void LOGIC_SaveHeatingData(RegulatorsData Sample)
{
	static Int16U RegOutClearCounter = 0;


	if(LOGIC_State == LS_Heating)
	{
		DataTable[REG_ACTUAL_U_DUT] = _IQint(Sample.U);
		DataTable[REG_ACTUAL_I_DUT] = _IQint(_IQmpy(Sample.Ih, _IQI(10)));
		DataTable[REG_ACTUAL_P_DUT_WHOLE] = _IQint(Sample.P);
		DataTable[REG_ACTUAL_P_DUT_FRACT] = _IQint(_IQmpy((Sample.P - _IQI(DataTable[REG_ACTUAL_P_DUT_WHOLE])), _IQI(100)));

		RegOutClearCounter = 0;
	}
	else
	{
		RegOutClearCounter++;

		if(RegOutClearCounter >= REG_OUTPUT_CLEAR_CNT_MAX_VALUE)
		{
			DataTable[REG_ACTUAL_U_DUT] = 0;
			DataTable[REG_ACTUAL_I_DUT] = 0;
			DataTable[REG_ACTUAL_P_DUT_WHOLE] = 0;
			DataTable[REG_ACTUAL_P_DUT_FRACT] = 0;

			RegOutClearCounter = 0;
		}
	}

	DataTable[REG_ACTUAL_I_MEASUREMENT] = _IQint(_IQmpy(Sample.Im, _IQI(10)));
}
// ----------------------------------------

void LOGIC_TimeCounterReset()
{
	LOGIC_TimeCounter = 0;
}
// ----------------------------------------

Boolean LOGIC_TimeCounterCheck(Int32U Time)
{
	return (LOGIC_TimeCounter >= Time) ? TRUE : FALSE;
}
// ----------------------------------------

void LOGIC_MeasuringCurrentConfig(Int16U Current)
{
	REGULATOR_Init(SelectIm);
	REGULATOR_Update(SelectIm, _IQI(Current));
}
// ----------------------------------------

void LOGIC_HeatingCurrentConfig(Int32U CurrentWidth)
{
	REGULATOR_Init(SelectIh);
	REGULATOR_Init(SelectP);
	LOGIC_HeatingCurrentUpdate(CurrentWidth);
}
// ----------------------------------------

void LOGIC_HeatingCurrentUpdate(Int32U CurrentWidth)
{
	if(CurrentWidth <= TIME_2MS)
		HeatingCurrentSetpoint = LOGIC_CurrentWidthLess_2ms;
	else
	{
		if(CurrentWidth > TIME_10MS)
			HeatingCurrentSetpoint = LOGIC_CurrentWidthAbove_10ms;
		else
			HeatingCurrentSetpoint = LOGIC_CurrentWidthLess_10ms;
	}

	if(HeatingCurrentSetpoint >= DataTable[REG_I_THRE_SET_CAP_V_THRE])
		LOGIC_CapVoltageThreshold = DataTable[REG_CAP_VOLTAGE_THRE_H];
	else
		LOGIC_CapVoltageThreshold = DataTable[REG_CAP_VOLTAGE_THRE_L];

	LOGIC_HeatingCurrentSetRange(HeatingCurrentSetpoint);
	REGULATOR_Update(SelectIh, _IQI(HeatingCurrentSetpoint));
}
// ----------------------------------------

Boolean LOGIC_MeasurementDelayProcess(Int32U Delay, Int32U MsrDelayCompens, LogicState NextState)
{
	Int32U MeasurementDelayWhole, MeasurementDelayFraction;

	MeasurementDelayWhole = _IQint(_FPtoIQ2(Delay, 100));

	if(LOGIC_TimeCounterCheck(MeasurementDelayWhole))
	{
		ZwTimer_StopT1();
		//
		MeasurementDelayFraction = Delay - _IQint(_IQmpy(_IQI(MeasurementDelayWhole), _IQI(100)));
		if(MeasurementDelayFraction)
			DELAY_US(MeasurementDelayFraction);

		if(MsrDelayCompens)
			DELAY_US(MsrDelayCompens);

		LOGIC_SetState(NextState);
		//
		ZwTimer_StartT1();

		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------

void LOGIC_MeasuringProcess()
{
	LOGIC_SaveData(MEASURE_CombinedData(LOGIC_CoolingMode), TRUE);
}
// ----------------------------------------

void LOGIC_SaveToOutputRegisters()
{
	static Int32U DataLogStep = 0;

	DataLogStep++;

	if(DataLogStep >= DATA_LOG_STEP)
	{
		DataLogStep = 0;

		ZwTimer_StopT1();
		LOGIC_SaveData(MEASURE_CombinedData(LOGIC_CoolingMode), FALSE);
		ZwTimer_StartT1();
	}
}
// ----------------------------------------

void LOGIC_Heating(Boolean State)
{
	MEASURE_VariablesPrepare();
	ZbGPIO_SyncOscilloscope(State);
	ZbGPIO_OuputLock(!State);
	//
	REGULATOR_Enable(SelectIh, State);
	REGULATOR_Enable(SelectP, DataTable[REG_REGULATOR_POWER_CTRL]);

	if(!State)
		REGULATOR_Update(SelectP, 0);

	CurrentGeneratedFlag = State;
}
// ----------------------------------------

void LOGIC_SaveData(CombinedData Sample, Boolean SaveToEndpoints)
{
	Int16U TSP, Tcase1, Tcase2, Tcool1, Tcool2;
	static Int16U ScopeLogStep = 0;

	if(CurrentGeneratedFlag)
		TSP = DataTable[REG_ACTUAL_TSP];
	else
		TSP = _IQint(Sample.TSP);

	Tcase1 = _IQint(_IQmpy(Sample.Tcase1, _IQI(10)));
	Tcase2 = _IQint(_IQmpy(Sample.Tcase2, _IQI(10)));
	Tcool1 = _IQint(_IQmpy(Sample.Tcool1, _IQI(10)));
	Tcool2 = _IQint(_IQmpy(Sample.Tcool2, _IQI(10)));

	if (SaveToEndpoints && (ScopeLogStep++ >= DataTable[REG_SCOPE_STEP]))
	{
		ScopeLogStep = 0;

		// Save to endpoints
		if(LOGIC_Values_Counter < VALUES_x_SIZE)
		{
			LOGIC_Values_TSP[EP_DataCounter]    = TSP;
			LOGIC_Values_Tcase1[EP_DataCounter] = Tcase1;
			LOGIC_Values_Tcase2[EP_DataCounter] = Tcase2;
			LOGIC_Values_Tcool1[EP_DataCounter] = Tcool1;
			LOGIC_Values_Tcool2[EP_DataCounter] = Tcool2;
			EP_DataCounter++;
		}
	}

	LOGIC_Values_Counter = EP_DataCounter;

	// Save to ouput registers
	DataTable[REG_ACTUAL_TSP]   = TSP;
	DataTable[REG_ACTUAL_T_CASE1] = Tcase1;
	DataTable[REG_ACTUAL_T_CASE2] = Tcase2;
	DataTable[REG_ACTUAL_T_COOL1] = Tcool1;
	DataTable[REG_ACTUAL_T_COOL2] = Tcool2;
}
// ----------------------------------------

void LOGIC_CalculateTimeInterval(volatile Int32U *TimeInterval)
{
	Int32U Additive = 0;

	if(*TimeInterval < (Int32U)10)
		Additive = 1;						// 100us
	else if(*TimeInterval < (Int32U)100)
		Additive = 10;						// 1ms
	else if(*TimeInterval < (Int32U)1e3)
		Additive = 100;						// 10ms
	else if(*TimeInterval < (Int32U)10e3)
		Additive = 1e3;						// 100ms
	else if(*TimeInterval < (Int32U)100e3)
		Additive = 10e3;					// 1s
	else if(*TimeInterval < (Int32U)1e6)
		Additive = 100e3;					// 10s
	else
		Additive = 1e6;						// 100s

	*TimeInterval = *TimeInterval + Additive;
}
// ----------------------------------------

void LOGIC_SetState(LogicState State)
{
	LOGIC_State = State;
	DataTable[REG_LOGIC_STATE] = LOGIC_State;
}
// ----------------------------------------

void LOGIC_SetOperationState(OperationState NewState)
{
	DataTable[REG_OP_STATE] = NewState;
}
// ----------------------------------------

void LOGIC_CacheVariables()
{
	LOGIC_PulseWidthMin = DataTable[REG_PULSE_WIDTH_MIN];
	//
	LOGIC_PulseWidthMax = DataTable[REG_PULSE_WIDTH_MAX_H];
	LOGIC_PulseWidthMax = (LOGIC_PulseWidthMax << 16) | DataTable[REG_PULSE_WIDTH_MAX_L];
	//
	LOGIC_CurrentWidthLess_2ms = DataTable[REG_I_WIDTH_LESS_OR_EQ_2MS];
	LOGIC_CurrentWidthLess_10ms = DataTable[REG_I_WIDTH_LESS_OR_EQ_10MS];
	LOGIC_CurrentWidthAbove_10ms = DataTable[REG_I_WIDTH_ABOVE_10MS];
	//
	LOGIC_MeasurementDelay = DataTable[REG_MEASUREMENT_DELAY];
	LOGIC_ZthPause = DataTable[REG_ZTH_PAUSE];
	LOGIC_Pause = DataTable[REG_PAUSE];
	//
	LOGIC_CoolingMode = DataTable[REG_COOLING_MODE];
	LOGIC_Tmax = DataTable[REG_T_MAX];
	LOGIC_DUTType = DataTable[REG_DUT_TYPE];
	LOGIC_MeasuringCurrent = DataTable[REG_MEASURING_CURRENT];
}
// ----------------------------------------

void LOGIC_UpdateParams()
{
	LOGIC_CacheVariables();
	REGULATOR_ResetVariables();
	LOGIC_SetState(LS_ConfigAll);
}
// ----------------------------------------

void LOGIC_ResetVariables()
{
	EP_DataCounter = 0;
	LOGIC_ActualDelayWidth = 0;
}
// ----------------------------------------

void LOGIC_HeatingCurrentSetRange(Int16U Current)
{
	if(Current <= DataTable[REG_IH_RANGE_THRESHOLD])
	{
		ZbGPIO_SB_Ih_Range(0);
		CONVERT_IhSetRangeParams(0);
	}
	else
	{
		ZbGPIO_SB_Ih_Range(1);
		CONVERT_IhSetRangeParams(1);
	}
}
// ----------------------------------------

void LOGIC_PowerOnSequence()
{
	switch(LOGIC_State)
	{
		case LS_None:
			ZbGPIO_LowPowerSupplyControl(TRUE);
			Timeout = CONTROL_TimeCounter + TIME_POWER_ON;
			CONTROL_SetDeviceState(DS_PowerOn, LS_BatteryPrepare);
			break;

		case LS_BatteryPrepare:
			if(LOGIC_BatteryVoltageControl(Timeout))
				CONTROL_SetDeviceState(DS_Ready, LS_None);
			break;
	}

}
// ----------------------------------------

Boolean LOGIC_BatteryVoltageControl(Int64U Timeout)
{
	if(MEASURE_CapVoltage >= DataTable[REG_CAP_VOLTAGE_THRE_H])
		return TRUE;
	else
	{
		if(CONTROL_TimeCounter >= Timeout)
		{
			ZbGPIO_LowPowerSupplyControl(FALSE);
			CONTROL_SwitchToFault(FAULT_POWERON);
		}
	}

	return FALSE;
}
// ----------------------------------------

void LOGIC_GatePulse(Boolean State)
{
	if(State)
	{
		if(LOGIC_DUTType)
		{
			ZthDCB_SwitchOutput(VOLTAGE_SOURCE);

			if(DataTable[REG_IGBT_V_GATE])
				ZthDCB_VoltageSet(GATE_VOLTGE_20V);
			else
				ZthDCB_VoltageSet(GATE_VOLTGE_15V);
		}
		else
		{
			ZthDCB_SwitchOutput(CURRENT_SOURCE);
			ZthDCB_CurrentSet(_IQI(DataTable[REG_GATE_CURRENT]));
		}
	}
	else
	{
		ZthDCB_CurrentSet(0);
		ZthDCB_VoltageSet(GATE_VOLTGE_0V);
	}
}
// ----------------------------------------

// No more.
