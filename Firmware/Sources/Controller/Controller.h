// -----------------------------------------
// Logic controller
// ----------------------------------------

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
#include "stdinc.h"
//
#include "ZwDSP.h"
#include "Global.h"


//Defines
//
#define ZTH_MODE_MULTIPULSE		0
#define ZTH_MODE_SINGLEPULSE	1
#define ZTH_MODE_GRADUATION		2
#define ZTH_MODE_MANUAL			3
//


// Types
//
typedef enum __DeviceState
{
	DS_None			= 0,
	DS_Fault		= 1,
	DS_Disabled		= 2,
	DS_PowerOn		= 3,
	DS_Ready		= 4,
	DS_InProcess	= 5
} DeviceState;
//

typedef enum __IntDeviceState
{
	INTDS_None					= 0,
	INTDS_Ihc_Inprocess			= 1,
	INTDS_Ihc_Ready				= 2,
	INTDS_Delay_Inprocess		= 3,
	INTDS_Delay_Ready			= 4,
	INTDS_Grad_Inprocess		= 5,
	INTDS_Grad_Ready			= 6
} IntDeviceState;
//

typedef enum __GraduationState
{
	GS_None			= 0,
	GS_Ready		= 1
} GraduationState;
//

typedef enum __CapacitorsChargingState
{
	CCS_InProcess		= 0,
	CCS_Charged			= 1,
	CCS_Failed			= 2
} CapacitorsChargingState;
//

//

typedef void (*FUNC_AsyncDelegate)();


// Variables
//
extern volatile Int64U CONTROL_TimeCounter;
extern volatile DeviceState CONTROL_State;
extern volatile IntDeviceState CONTROL_IntState;
extern Int16U* IhcPulseArray;
extern Int32U IhcDelayTimeChange;
extern Int32U IhcLastDelayWidth_uS;
//
extern Int16U CONTROL_Values_I[VALUES_x_SIZE];
extern Int16U CONTROL_Values_U[VALUES_x_SIZE];
extern Int16U CONTROL_Values_Z[VALUES_x_SIZE];
extern volatile Int16U CONTROL_Values_I_Counter, CONTROL_Values_U_Counter, CONTROL_Values_Z_Counter;
extern volatile Int16U CONTROL_BootLoaderRequest;
extern Int32U IhcPulseWidth_uS;
extern Int32U IhcDelayWidth_uS;
extern Int32U IhcPulseTimeChange;
extern Int32U IhcLastPulseWidth_uS;
extern Int32U MeasureAfterPulse_uS;
extern Int32U GraduationTime;
//
extern Int16U TimeBetweenPulses;

// Functions
//
// Initialize controller
void CONTROL_Init(Boolean BadClockDetected);
// Update low-priority states
void CONTROL_Idle();
// Main control cycle
void CONTROL_Update();
// Notify that CAN system fault occurs
void CONTROL_NotifyCANaFault(ZwCAN_SysFlags Flag);
void CONTROL_NotifyCANbFault(ZwCAN_SysFlags Flag);
// Switch device to fault state
void CONTROL_SwitchToFault(Int16U FaultReason, Int16U FaultReasonExt);
void CONTROL_SetDeviceState(DeviceState NewState);
void Start_Ihc(void);
void Stop_Ihc(void);
void Set_MCB_GateDrv_DAC(Int16U Data, void (*Source)(Boolean));
void SaveZthData(Int16U Data);


#endif // __CONTROLLER_H
