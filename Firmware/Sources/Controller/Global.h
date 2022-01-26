// -----------------------------------------
// Global definitions
// ----------------------------------------

#ifndef __GLOBAL_H
#define __GLOBAL_H

// Include
#include "stdinc.h"
#include "SysConfig.h"

// Global parameters
//--------------------------------------------------------
#define MAIN_VOLTAGE_MAX				240 //Maximum level of main voltage, V*10
//
// Password to unlock non-volatile area for write
#define ENABLE_LOCKING					FALSE
#define UNLOCK_PWD_1					1
#define UNLOCK_PWD_2					1
#define UNLOCK_PWD_3					1
#define UNLOCK_PWD_4					1
//
#define EP_COUNT						11
#define VALUES_x_SIZE					1200
//
#define	NODEID_SCCI_SCOPE				0
//
#define	SCCI_MASTER_TIMEOUT_TICKS_MS	500		// in ms
#define SCCI_TIMEOUT_MS					100		// in ms
#define	SCCI_SLAVE_TIMEOUT_TICKS_MS		(Int32U)(CS_T0_FREQ/1000*SCCI_TIMEOUT_MS)
//
#define BCCI_TIMEOUT_MS					1000	// in ms
#define	BCCI_MASTER_TIMEOUT_TICKS_MS	(Int32U)(CS_T0_FREQ/1000*BCCI_TIMEOUT_MS)
//
#define DAC_WRITE_DELAY_US				3		// in us
//
#define TIME_CONFIG						5000	// in ms
#define TIME_POWER_ON					15000	// in ms
#define TIME_BATTERY_RECHARGE			5000	// in ms
//
#define I_MAX_WIDTH_2MS					2500	// in A
#define I_MAX_WIDTH_10MS				1500	// in A
#define I_MAX_WIDTH_DC					1000	// in A
//
#define PULSE_WIDTH_10MS				100		// in us x0.01
#define PULSE_WIDTH_2MS					20		// in us x0.01
//--------------------------------------------------------

// Regulator parameters
//--------------------------------------------------------
#define REGLTR_IM_SAT					_IQ(6000.0f)	// Насыщение выхода регулятора Im (в мА)
#define REGLTR_IH_SAT					_IQ(5000.0f)	// Насыщение выхода регулятора Ih (в А)
#define REGLTR_ERROR_I_SAT_H			_IQ(30000.0f)	// Значение насыщения для интегральной составляющей регулятора
//--------------------------------------------------------

#endif // __GLOBAL_H
