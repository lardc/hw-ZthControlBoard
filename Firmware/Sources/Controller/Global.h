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
#define EP_COUNT						12
#define VALUES_x_SIZE					500
//

#define SCCI_TIMEOUT_MS					100		// in ms
#define	SCCI_SLAVE_TIMEOUT_TICKS_MS		(Int32U)(CS_T0_FREQ/1000*SCCI_TIMEOUT_MS)
//
#define BCCI_TIMEOUT_MS					1000	// in ms
#define	BCCI_MASTER_TIMEOUT_TICKS_MS	(Int32U)(CS_T0_FREQ/1000*BCCI_TIMEOUT_MS)
//
#define DAC_WRITE_DELAY_US				10		// in us
//--------------------------------------------------------

// Regulator parameters
//--------------------------------------------------------
#define REGLTR_IM_SAT					_IQ(20000.0f)	// Насыщение выхода регулятора Im (в мА)
#define REGLTR_IH_SAT					_IQ(1200.0f)	// Насыщение выхода регулятора Ih (в мА)
#define REGLTR_ERROR_I_SAT_H			_IQ(20000.0f)	// Значение насыщения для интегральной составляющей регулятора
//--------------------------------------------------------

#endif // __GLOBAL_H
