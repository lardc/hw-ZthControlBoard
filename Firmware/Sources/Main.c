// -----------------------------------------
// Program entry point
// ----------------------------------------

// Include
#include <stdinc.h>
//
#include "ZwDSP.h"
#include "ZbBoard.h"
#include "ZbGPIO.h"
//
#include "SysConfig.h"
//
#include "Controller.h"
#include "Logic.h"
#include "MeasuringProcesses.h"
#include "DeviceObjectDictionary.h"
#include "DataTable.h"

// FORWARD FUNCTIONS
// -----------------------------------------
Boolean InitializeCPU();
void InitializeTimers();
void InitializeADC();
void InitializeCAN();
void InitializeSCI();
void InitializeSPI();
void InitializeBoard();
void InitializeController(Boolean GoodClock);
// -----------------------------------------

// FORWARD ISRs
// -----------------------------------------
// CPU Timer 0 ISR
ISRCALL Timer0_ISR();
// CPU Timer 1 ISR
ISRCALL Timer1_ISR();
// CPU Timer 2 ISR
ISRCALL Timer2_ISR();
// CANa Line 0 ISR
ISRCALL CAN0A_ISR();
// ILLEGAL ISR
ISRCALL IllegalInstruction_ISR();
// -----------------------------------------

// FUNCTIONS
// -----------------------------------------
void main()
{
	Boolean clockInitResult;

	// Boot process
	clockInitResult = InitializeCPU();

	// Only if good clocking was established
	if(clockInitResult)
	{
		InitializeTimers();
		InitializeCAN();
		InitializeSPI();
		InitializeADC();
		InitializeBoard();
	}

	// Try initialize SCI in spite of result of clock initialization
	InitializeSCI();

	// Setup ISRs
	BEGIN_ISR_MAP
		ADD_ISR(TINT0, Timer0_ISR);
		ADD_ISR(TINT1_XINT13, Timer1_ISR);
		ADD_ISR(TINT2, Timer2_ISR);
		ADD_ISR(ECAN0INTA, CAN0A_ISR);
	END_ISR_MAP

	// Initialize controller logic
	InitializeController(clockInitResult);
	
	// Only if good clocking was established
	if(clockInitResult)
	{
		// Enable interrupts
		EINT;
		ERTM;

		// Set watch-dog as WDRST
		ZwSystem_SelectDogFunc(FALSE);
		// Enable watch-dog
		ZwSystem_EnableDog(SYS_WD_PRESCALER);
		// Lock WD configuration
		ZwSystem_LockDog();
	
		// Start system ticks timer
		ZwTimer_StartT0();
	}

	// Low-priority services
	while(TRUE)
		CONTROL_Idle();
}
// -----------------------------------------

Boolean InitializeCPU()
{
    Boolean clockInitResult;

	// Init clock and peripherals
    clockInitResult = ZwSystem_Init(CPU_PLL, CPU_CLKINDIV, SYS_LOSPCP, SYS_HISPCP, SYS_PUMOD);

    if(clockInitResult)
    {
		// Do default GPIO configuration
		ZwGPIO_Init(GPIO_TSAMPLE, GPIO_TSAMPLE, GPIO_TSAMPLE, GPIO_TSAMPLE, GPIO_TSAMPLE);

		// Initialize PIE and vectors
		ZwPIE_Init();
		ZwPIE_Prepare();
    }
   	
	// Config flash
	ZW_FLASH_CODE_SHADOW;
	ZW_FLASH_OPTIMIZE(FLASH_FWAIT, FLASH_OTPWAIT);

   	return clockInitResult;
}
// -----------------------------------------

void InitializeTimers()
{
    ZwTimer_InitT0();
	ZwTimer_SetT0(TIMER0_PERIOD);
	ZwTimer_EnableInterruptsT0(TRUE);

    ZwTimer_InitT1();
	ZwTimer_SetT1(TIMER1_PERIOD);
	ZwTimer_EnableInterruptsT1(TRUE);

    ZwTimer_InitT2();
	ZwTimer_SetT2(TIMER2_PERIOD);
	ZwTimer_EnableInterruptsT2(TRUE);
}
// -----------------------------------------

void InitializeSCI()
{
	ZwSCIa_Init(SCIA_BR, SCIA_DB, SCIA_PARITY, SCIA_SB, FALSE);
	ZwSCIa_InitFIFO(16, 0);
	ZwSCIa_EnableInterrupts(FALSE, FALSE);

	ZwSCI_EnableInterruptsGlobal(FALSE);
}
// -----------------------------------------

void InitializeADC()
{
	// Initialize and prepare ADC
	ZwADC_Init(ADC_PRESCALER, ADC_CD2, ADC_SH);
	ZwADC_ConfigInterrupts(TRUE, FALSE);

	// Enable interrupts on peripheral and CPU levels
	ZwADC_EnableInterrupts(TRUE, FALSE);
	ZwADC_EnableInterruptsGlobal(TRUE);
}
// -----------------------------------------

void InitializeSPI()
{
	// Init SPI-A
	ZwSPIa_Init(TRUE, IO_BAUDRATE_OUT, IO_CL_DEF, IO_PLR, IO_PHASE, ZW_SPI_INIT_RX | ZW_SPI_INIT_CS, TRUE, FALSE);
	ZwSPIa_InitFIFO(0, 0);
	ZwSPIa_ConfigInterrupts(FALSE, FALSE);
	ZwSPIa_EnableInterrupts(FALSE, FALSE);

	// Init SPI-B
	ZwSPIb_Init(TRUE, IO_BAUDRATE_OUT, IO_CL_DEF, IO_PLR, IO_PHASE, ZW_SPI_INIT_RX, TRUE, FALSE);
	ZwSPIb_InitFIFO(0, 0);
	ZwSPIb_ConfigInterrupts(FALSE, FALSE);
	ZwSPIb_EnableInterrupts(FALSE, FALSE);

	// Init SPI-C
	ZwSPIc_Init(TRUE, IO_BAUDRATE_OUT, IO_CL_DEF, IO_PLR, IO_PHASE, ZW_SPI_INIT_TX | ZW_SPI_INIT_CS, TRUE, FALSE);
	ZwSPIc_InitFIFO(0, 0);
	ZwSPIc_ConfigInterrupts(FALSE, FALSE);
	ZwSPIc_EnableInterrupts(FALSE, FALSE);

	// Init SPI-D
	ZwSPId_Init(TRUE, IO_BAUDRATE_OUT, IO_CL_DEF, IO_PLR, IO_PHASE, ZW_SPI_INIT_RX, TRUE, FALSE);
	ZwSPId_InitFIFO(0, 0);
	ZwSPId_ConfigInterrupts(FALSE, FALSE);
	ZwSPId_EnableInterrupts(FALSE, FALSE);



	// Common (ABCD)
	ZwSPI_EnableInterruptsGlobal(FALSE);
}
// -----------------------------------------

void InitializeCAN()
{
	// Init CAN
	ZwCANa_Init(CANA_BR, CANA_BRP, CANA_TSEG1, CANA_TSEG2, CANA_SJW);

	// Register system handler
	ZwCANa_RegisterSysEventHandler(&CONTROL_NotifyCANaFault);

    // Allow interrupts for CANa (internal interface)
    ZwCANa_InitInterrupts(TRUE);
    ZwCANa_EnableInterrupts(TRUE);
}
// -----------------------------------------

void InitializeBoard()
{
	// Init DAC
	ZbMemory_Init();
	// Init on-board GPIO
	ZbGPIO_Init();
}
// -----------------------------------------

void InitializeController(Boolean GoodClock)
{
	// Init controllers and logic
	CONTROL_Init(!GoodClock);
}
// -----------------------------------------

// ISRs
// -----------------------------------------
#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(Timer0_ISR, "ramfuncs");
	#pragma CODE_SECTION(Timer1_ISR, "ramfuncs");
	#pragma CODE_SECTION(Timer2_ISR, "ramfuncs");
	#pragma CODE_SECTION(CAN0A_ISR, "ramfuncs");
	#pragma CODE_SECTION(IllegalInstruction_ISR, "ramfuncs");
#endif
//
#pragma INTERRUPT(Timer0_ISR, HPI);

// timer 0 ISR
ISRCALL Timer0_ISR(void)
{
	static Int16U LedCounter = 0;

	// Update time
	++CONTROL_TimeCounter;

	// Service watch-dogs
	if (CONTROL_BootLoaderRequest != BOOT_LOADER_REQUEST)
		ZwSystem_ServiceDog();

	++LedCounter;
	if(LedCounter == DBG_COUNTER_PERIOD)
	{
		ZbGPIO_ToggleLED1();
		LedCounter = 0;
	}

	// allow other interrupts from group 1
	TIMER0_ISR_DONE;
}
// -----------------------------------------

// timer 1 ISR
ISRCALL Timer1_ISR(void)
{
	static Int32U Qi;
	static Int32U Qp;
	static Int16U PulseCounter;
	static Int16U ReadyCounter;
	Int16U RegulatorError;
	Int16U Ih_Current;
	Int16U DUT_Voltage;
	Int16U DUT_Power;
	Int16U RegulatorOut;
	Int16U Target;

	if(CONTROL_IntState == INTDS_Ihc_Inprocess)
	{
		LongTimeTime_CheckValue(&CONFIG_LongTimeTimer1);

		if(~CONFIG_LongTimeTimer1.IhcTimeCounter)
		{
			Stop_Ihc();
		}
		else
		{
			//Measure Ih current and DUT voltage
			Measure_TSP_Start();
			Ih_Current = Measure_Im_Ih_Tdut(&ZbGPIO_IHC_CS);
			DUT_Voltage = Measure_TSP_Finish();

			//Transform in Volts and Amphers
			Ih_Current = Ih_Current * DataTable[REG_K_IHC_TO_AMPHER] / 1000;
			DUT_Voltage = DUT_Voltage * DataTable[REG_K_TSP_TO_VOLTAGE] / 1000;

			//PI regulator
			if (ReadyCounter > DIR_CUR_ERR_TICKS)
			{
				DUT_Power = Ih_Current*DUT_Voltage;

				if(!DataTable[REG_H_POWER])
					DataTable[REG_H_POWER] = DUT_Power;

				RegulatorError = DataTable[REG_H_POWER] - DUT_Power;
			}
			else
			{
				Target = *(IhcPulseArray+PulseCounter);
				RegulatorError = (PulseCounter > 0) ? ((int)Target - Ih_Current) : 0;
			}


			if (PulseCounter >= (PULSE_ARRAY_LEN-1))
			{
				// Check out the task
				if (RegulatorError < REGULATOR_ERROR_READY)
					++ReadyCounter;
				else
					ReadyCounter = 0;
				//

				Qp = (RegulatorError * DataTable[REG_P_TOP_PULSE]) / DataTable[REG_P_TOP_PULSE_D];
				Qi += (RegulatorError * DataTable[REG_I_TOP_PULSE]) / DataTable[REG_I_TOP_PULSE_D];

			}
			else
			{
				PulseCounter++;

				Qp = (RegulatorError * DataTable[REG_P_FRONT_PULSE]) / DataTable[REG_P_FRONT_PULSE_D];
				Qi += (RegulatorError * DataTable[REG_I_FRONT_PULSE]) / DataTable[REG_I_FRONT_PULSE_D];
			}

			// Проверка интегральной составляющей на насыщение
			if (Qi > QI_SATURATION)
				Qi = QI_SATURATION;
			else if (Qi < -QI_SATURATION)
				Qi = -QI_SATURATION;

			RegulatorOut = (int)(Target + Qp + Qi)*DataTable[REG_K_DAC_OUT];

			Set_MCB_GateDrv_DAC(RegulatorOut, &ZbGPIO_DAC_LDAC);
		}
	}


	if(CONTROL_IntState == INTDS_Delay_Inprocess)
	{
		LongTimeTime_CheckValue(&CONFIG_LongTimeTimer1);

		if(~CONFIG_LongTimeTimer1.IhcTimeCounter)
		{
			MeasureBeforeAndAfterPulse(1);
			ZwTimer_StopT1();
		}
	}

	// no PIE
	TIMER1_ISR_DONE;
}
// -----------------------------------------

// timer 2 ISR
ISRCALL Timer2_ISR(void)
{
	if((CONTROL_IntState == INTDS_Ihc_Inprocess)||(CONTROL_IntState == INTDS_Delay_Inprocess))
	{
		LongTimeTime_CheckValue(&CONFIG_LongTimeTimer2);

		if(~CONFIG_LongTimeTimer2.IhcTimeCounter)
		{
			if(CONTROL_IntState == INTDS_Ihc_Inprocess)
				CONTROL_IntState == INTDS_Ihc_Ready;

			if(CONTROL_IntState == INTDS_Delay_Inprocess)
			{
				CONTROL_IntState == INTDS_Delay_Ready;
				ZwTimer_StopT2();
				ZwTimer_ReloadT2();
			}

			if(CONTROL_IntState == INTDS_Grad_Inprocess)
			{
				CONTROL_IntState == INTDS_Grad_Ready;
				ZwTimer_StopT2();
				ZwTimer_ReloadT2();
			}
		}
	}

	// no PIE
	TIMER2_ISR_DONE;
}
// -----------------------------------------

// Line 0 CANa ISR
ISRCALL CAN0A_ISR(void)
{
    // handle CAN system events
	ZwCANa_DispatchSysEvent();

	// allow other interrupts from group 9
	CAN_ISR_DONE;
}
// -----------------------------------------

// ILLEGAL ISR
ISRCALL IllegalInstruction_ISR(void)
{
	// Disable interrupts
	DINT;

	// Reset system using WD
	ZwSystem_ForceDog();
}
// -----------------------------------------

// No more.
