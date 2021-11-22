#ifndef __DRCU_DICTIONARY_H
#define __DRCU_DICTIONARY_H

// Commands
#define DRCU_ACT_ENABLE_POWER				1		// Power on
#define DRCU_ACT_DISABLE_POWER				2		// Power off
#define DRCU_ACT_CLR_FAULT					3		// Clear fault
#define DRCU_ACT_CLR_WARNING				4		// Clear warning
//
#define DCRU_ACT_CONFIG						100		// Aplly settings

// Registers
#define DRCU_REG_CURRENT_SETPOINT			128		// Current setpoint value
//
#define DRCU_REG_DEV_STATE					192		// Device state
#define DRCU_REG_FAULT_REASON				193		// Fault code
#define DRCU_REG_DISABLE_REASON				194		// Disable code
#define DRCU_REG_WARNING					195		// Warning
#define DRCU_REG_PROBLEM					196		// Problem code

#endif // __DRCU_DICTIONARY_H
