// -----------------------------------------
// Main logic library
// ----------------------------------------

// Header
#include "Logic.h"
//
// Includes
#include "xCCICommon.h"
#include "Global.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Controller.h"
#include "ZbBoard.h"
#include "MeasuringProcesses.h"

// Definitions
//

//
#define PULSE_TIME_100US			100
#define PULSE_TIME_1MS				1000
#define PULSE_TIME_10MS				10000
#define PULSE_TIME_100MS			100000
#define PULSE_TIME_1S				1000000
#define PULSE_TIME_10S				10000000
#define PULSE_TIME_100S				100000000
//

//Structs
//


struct CONFIG_LongTimeTimer CONFIG_LongTimeTimer1;
struct CONFIG_LongTimeTimer CONFIG_LongTimeTimer2;
//

// Variables
//
Int16U ShortPulseArray[PULSE_ARRAY_LEN];
Int16U LongPulseArray[PULSE_ARRAY_LEN];
Int16U GraduationArray[GRAD_ARRAY_LENGTH];

// Forward functions
//
Int32U LogTimeChange(Int32U TimePulse);

// Functions
//
void ZthModeMultiPulseProcess(void)
{
	if(DataTable[REG_GRADUATION_STATUS] == GS_Ready)
	{
		if(CONTROL_IntState == INTDS_None)
		{
			IhcPulseWidth_uS = DataTable[REG_TIME_FIRST_PULSE]*DataTable[REG_TIME_PULSE_MULT]*10;
			IhcLastPulseWidth_uS = DataTable[REG_TIME_LAST_PULSE]*DataTable[REG_TIME_PULSE_MULT]*10;

			if(IhcPulseWidth_uS > PULSE_TIME_10MS)
				IhcPulseArray = &LongPulseArray[0];
			else
				IhcPulseArray = &ShortPulseArray[0];

			MeasureBeforeAndAfterPulse(0);

			CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcPulseWidth_uS);
			Start_Ihc();
		}

		if(CONTROL_IntState == INTDS_Ihc_Ready)
		{
			MeasureAfterPulse_uS = DataTable[REG_MEASURMENT_DELAY]*DataTable[REG_TIME_PULSE_MULT]*10;
			CONFIG_Timer(&CONFIG_LongTimeTimer1,MeasureAfterPulse_uS);
			ZwTimer_ReloadT1();
			ZwTimer_StopT1();

			IhcDelayWidth_uS = IhcPulseWidth_uS*DataTable[REG_K_TIME_BETWEEN_PULSES];
			CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcDelayWidth_uS);
			CONTROL_IntState == INTDS_Delay_Inprocess;
		}

		if(CONTROL_IntState == INTDS_Delay_Ready)
		{
			if(IhcPulseWidth_uS < IhcLastPulseWidth_uS)
			{
				IhcPulseTimeChange = LogTimeChange(IhcPulseWidth_uS);
				IhcPulseWidth_uS += IhcPulseTimeChange;
				CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcPulseWidth_uS);

				if(IhcPulseWidth_uS > PULSE_TIME_10MS)
					IhcPulseArray = &LongPulseArray[0];
				else
					IhcPulseArray = &ShortPulseArray[0];

				Start_Ihc();
			}
			else
				CONTROL_SetDeviceState(DS_Ready);
		}
	}
	else
	{
		CONTROL_SwitchToFault(DS_Fault, FAULT_GARDUATION);
	}
}
// ----------------------------------------

Int32U LogTimeChange(Int32U TimePulse)
{
	Int32U NewTime;

	if(TimePulse >= PULSE_TIME_100US)
		NewTime = PULSE_TIME_100US;

	if(TimePulse >= PULSE_TIME_1MS)
		NewTime = PULSE_TIME_1MS;

	if(TimePulse >= PULSE_TIME_10MS)
		NewTime = PULSE_TIME_10MS;

	if(TimePulse >= PULSE_TIME_100MS)
		NewTime = PULSE_TIME_100MS;

	if(TimePulse >= PULSE_TIME_1S)
		NewTime = PULSE_TIME_1S;

	if(TimePulse >= PULSE_TIME_10S)
		NewTime = PULSE_TIME_10S;

	if(TimePulse >= PULSE_TIME_100S)
		NewTime = PULSE_TIME_100S;

	return NewTime;
}
// ----------------------------------------


void ZthModeSinglePulseProcess(void)
{
	if(DataTable[REG_GRADUATION_STATUS] == GS_Ready)
	{
		if(CONTROL_IntState == INTDS_None)
		{
			IhcPulseWidth_uS = DataTable[REG_TIME_FIRST_PULSE]*DataTable[REG_TIME_PULSE_MULT]*10;

			if(IhcPulseWidth_uS > PULSE_TIME_10MS)
				IhcPulseArray = &LongPulseArray[0];
			else
				IhcPulseArray = &ShortPulseArray[0];

			MeasureBeforeAndAfterPulse(0);

			CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcPulseWidth_uS);
			Start_Ihc();
		}

		if(CONTROL_IntState == INTDS_Ihc_Ready)
		{
			IhcDelayWidth_uS = DataTable[REG_TIME_FIRST_DELAY]*DataTable[REG_TIME_PULSE_MULT]*10;
			IhcLastDelayWidth_uS = DataTable[REG_TIME_LAST_DELAY]*DataTable[REG_TIME_PULSE_MULT]*10;
			CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcDelayWidth_uS);

			CONTROL_IntState == INTDS_Delay_Inprocess;
		}

		if(CONTROL_IntState == INTDS_Delay_Ready)
		{
			MeasureBeforeAndAfterPulse(0);

			if(IhcDelayWidth_uS<IhcLastDelayWidth_uS)
			{
				IhcDelayTimeChange = LogTimeChange(IhcDelayWidth_uS);
				IhcDelayWidth_uS += IhcDelayTimeChange;
				CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcPulseWidth_uS);

				CONTROL_IntState == INTDS_Delay_Inprocess;
			}
			else
				CONTROL_SetDeviceState(DS_Ready);
		}
	}
	else
	{
		CONTROL_SwitchToFault(DS_Fault, FAULT_GARDUATION);
	}
}
// ----------------------------------------

void ZthModeGraduationProcess(void)
{
	static Int16U TdutTemp = 0;

	if(DataTable[REG_GRADUATION_STATUS] == GS_Ready)
	{
		if(CONTROL_IntState == INTDS_None)
		{
			IhcPulseWidth_uS = DataTable[REG_TIME_FIRST_PULSE]*DataTable[REG_TIME_PULSE_MULT]*10;

			if(IhcPulseWidth_uS > PULSE_TIME_10MS)
				IhcPulseArray = &LongPulseArray[0];
			else
				IhcPulseArray = &ShortPulseArray[0];

			MeasureBeforeAndAfterPulse(0);

			CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcPulseWidth_uS);
			Start_Ihc();
		}

		if(CONTROL_IntState == INTDS_Ihc_Ready)
		{
			GraduationTime = DataTable[REG_TIME_REGISTRATION_TSP]*DataTable[REG_TIME_PULSE_MULT]*10;
			CONFIG_Timer(&CONFIG_LongTimeTimer2,GraduationTime);

			CONTROL_IntState == INTDS_Grad_Inprocess;
		}

		if(CONTROL_IntState == INTDS_Grad_Inprocess)
		{
			MeasureBeforeAndAfterPulse(1);

			if(!TdutTemp)
				TdutTemp = DataTable[REG_T_DUT_AFTER_PULSE];

			if((DataTable[REG_T_DUT_AFTER_PULSE] + dT_GRADUATION)<=TdutTemp)
			{
				GraduationArray[TdutTemp] = DataTable[REG_TSP_AFTER_PULSE];
			}
		}

		if(CONTROL_IntState == INTDS_Grad_Ready)
			CONTROL_SetDeviceState(DS_Ready);
	}
	else
	{
		CONTROL_SwitchToFault(DS_Fault, FAULT_GARDUATION);
	}
}
// ----------------------------------------

void ZthModeManualProcess(void)
{
	if(DataTable[REG_GRADUATION_STATUS] == GS_Ready)
	{
		if(CONTROL_IntState == INTDS_None)
		{
			IhcPulseWidth_uS = DataTable[REG_TIME_FIRST_PULSE]*DataTable[REG_TIME_PULSE_MULT]*10;
			IhcLastPulseWidth_uS = DataTable[REG_TIME_LAST_PULSE]*DataTable[REG_TIME_PULSE_MULT]*10;

			if(IhcPulseWidth_uS > PULSE_TIME_10MS)
				IhcPulseArray = &LongPulseArray[0];
			else
				IhcPulseArray = &ShortPulseArray[0];

			MeasureBeforeAndAfterPulse(0);

			CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcPulseWidth_uS);
			Start_Ihc();
		}

		if(CONTROL_IntState == INTDS_Ihc_Ready)
		{
			MeasureAfterPulse_uS = DataTable[REG_MEASURMENT_DELAY]*DataTable[REG_TIME_PULSE_MULT]*10;
			CONFIG_Timer(&CONFIG_LongTimeTimer1,MeasureAfterPulse_uS);
			ZwTimer_ReloadT1();
			ZwTimer_StopT1();

			IhcDelayWidth_uS = IhcPulseWidth_uS*DataTable[REG_K_TIME_BETWEEN_PULSES];
			CONFIG_Timer(&CONFIG_LongTimeTimer2,IhcDelayWidth_uS);
			CONTROL_IntState == INTDS_Delay_Inprocess;
		}

		if(CONTROL_IntState == INTDS_Delay_Ready)
			CONTROL_SetDeviceState(DS_Ready);
	}
	else
	{
		CONTROL_SwitchToFault(DS_Fault, FAULT_GARDUATION);
	}
}
// ----------------------------------------

void CONFIG_IhcPulse(void)
{
	int i;
	int dI;

	// Calculation table for array of short pulse
	dI = DataTable[REG_CURRENT_PULSE_SHORT]/(PULSE_ARRAY_LEN-1);
	for(i=0;i<PULSE_ARRAY_LEN-1;i++)
		ShortPulseArray[i] = i*dI;
	ShortPulseArray[PULSE_ARRAY_LEN-1] = DataTable[REG_CURRENT_PULSE_SHORT];

	// Calculation table for array of long pulse
	dI = DataTable[REG_CURRENT_PULSE_LONG]/(PULSE_ARRAY_LEN-1);
	for(i=0;i<PULSE_ARRAY_LEN-1;i++)
		LongPulseArray[i] = i*dI;
	LongPulseArray[PULSE_ARRAY_LEN-1] = DataTable[REG_CURRENT_PULSE_LONG];
}
// ----------------------------------------

void CONFIG_Timer(struct CONFIG_LongTimeTimer* LongTimeTimer, Int32U Time)
{
	if(Time > PULSE_TIME_10MS)
	{
		LongTimeTimer->IhcTimeSubtrack = PULSE_TIME_10MS;
		LongTimeTimer->TimerSet(PULSE_TIME_10MS);
	}
	else
	{
		LongTimeTimer->IhcTimeSubtrack = Time;
		LongTimeTimer->TimerSet(Time);
	}

	LongTimeTimer->IhcTimeCounter = Time;
}
// ----------------------------------------

void LongTimeTime_CheckValue(struct CONFIG_LongTimeTimer* LongTimeTimer)
{
	if(LongTimeTimer->IhcTimeCounter >= LongTimeTimer->IhcTimeSubtrack)
		LongTimeTimer->IhcTimeCounter -= LongTimeTimer->IhcTimeSubtrack;
	else
	{
		if(LongTimeTimer->IhcTimeCounter)
		{
			LongTimeTimer->IhcTimeCounter = LongTimeTimer->IhcTimeSubtrack;
			CONFIG_Timer(LongTimeTimer,LongTimeTimer->IhcTimeCounter);
		}
	}
}
// ----------------------------------------

void CONFIG_DUTControlBoard(void)
{
	Set_MCB_GateDrv_DAC(DataTable[REG_I_GATE_DUT], &ZbGPIO_DateDrv_LDAC);
}
// ----------------------------------------

void CONFIG_MCurrentBoard(void)
{
	Set_MCB_GateDrv_DAC(DataTable[REG_M_CURRENT_SET], &ZbGPIO_MCB_LDAC);
}
// ----------------------------------------

// No more.
