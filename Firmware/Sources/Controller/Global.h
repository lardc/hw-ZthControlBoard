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
#define VALUES_x_SIZE					2000
//

#define SCCI_TIMEOUT_MS					100		// in ms
#define	SCCI_SLAVE_TIMEOUT_TICKS_MS		(Int32U)(CS_T0_FREQ/1000*SCCI_TIMEOUT_MS)
//
#define BCCI_TIMEOUT_MS					1000	// in ms
#define	BCCI_MASTER_TIMEOUT_TICKS_MS	(Int32U)(CS_T0_FREQ/1000*BCCI_TIMEOUT_MS)
//
#define PULSE_ARRAY_LEN					10
#define DIR_CUR_ERR_TICKS				10
#define REGULATOR_ERROR_READY			1
#define QI_SATURATION					0xffffffff
//--------------------------------------------------------

#endif // __GLOBAL_H
