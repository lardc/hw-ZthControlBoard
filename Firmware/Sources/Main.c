// -----------------------------------------
// Program entry point
// ----------------------------------------

// Include
#include <stdinc.h>
//
#include "ZwDSP.h"
#include "ZbBoard.h"
//
#include "SysConfig.h"
//
#include "Controller.h"
#include "Regulator.h"

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
// ADC SEQ1 ISR
ISRCALL SEQ1_ISR();
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
		ADD_ISR(SEQ1INT, SEQ1_ISR);
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
   	
    // Configure flash
	ZW_FLASH_CODE_SHADOW;
	ZW_FLASH_MATH_SHADOW;
	ZW_FLASH_MATH_TR_SHADOW;
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
	ZwTimer_StartT1();

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
	ZwSPIa_Init(TRUE, IO_BAUDRATE_OUT, IO_CL_DEF, IO_PLR, IO_PHASE, ZW_SPI_INIT_RX, FALSE, FALSE);
	ZwSPIa_InitFIFO(0, 0);
	ZwSPIa_ConfigInterrupts(FALSE, FALSE);
	ZwSPIa_EnableInterrupts(FALSE, FALSE);

	// Init SPI-B
	ZwSPIb_Init(TRUE, IO_BAUDRATE_OUT, IO_CL_DEF, IO_PLR, IO_PHASE, ZW_SPI_INIT_RX, FALSE, FALSE);
	ZwSPIb_InitFIFO(0, 0);
	ZwSPIb_ConfigInterrupts(FALSE, FALSE);
	ZwSPIb_EnableInterrupts(FALSE, FALSE);

	// Init SPI-C
	ZwSPIc_Init(TRUE, IO_BAUDRATE_OUT, IO_CL_DEF, IO_PLR, IO_PHASE, ZW_SPI_INIT_TX, FALSE, FALSE);
	ZwSPIc_InitFIFO(0, 0);
	ZwSPIc_ConfigInterrupts(FALSE, FALSE);
	ZwSPIc_EnableInterrupts(FALSE, FALSE);

	// Init SPI-D
	ZwSPId_Init(TRUE, MEM_EPROM_BAUDRATE, IO_CL_DEF, IO_PLR, IO_PHASE, ZW_SPI_INIT_TX | ZW_SPI_INIT_RX, FALSE, FALSE);
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
#pragma INTERRUPT(SEQ1_ISR, HPI);

// timer 0 ISR
ISRCALL Timer0_ISR(void)
{
	static Int16U LedCounter = 0;

	MEASURE_CapVoltageSamplingStart();

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
	CONTROL_RegulatorProcess();

	// no PIE
	TIMER1_ISR_DONE;
}
// -----------------------------------------

// timer 2 ISR
ISRCALL Timer2_ISR(void)
{
	LOGIC_SetDelayFlag();
	ZwTimer_StopT2();

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

// ADC SEQ1 ISR
ISRCALL SEQ1_ISR(void)
{
	// Handle interrupt
	ZwADC_ProcessInterruptSEQ1();
	// Dispatch results
	ZwADC_Dispatch1();

	// allow other interrupts from group 1
	ADC_ISR_DONE;
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
