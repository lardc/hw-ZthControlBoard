// -----------------------------------------
// Logic controller
// ----------------------------------------

#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// ACTIONS
//
#define ACT_ENABLE_POWER			1	// Enable flyback converter
#define ACT_DISABLE_POWER			2	// Disable flyback converter
#define ACT_CLR_FAULT				3	// Clear fault (try switch state from FAULT to NONE)
#define ACT_CLR_WARNING				4	// Clear warning
//
#define ACT_START					100	// Start measurement
#define ACT_STOP					101	// Force measurement stop
#define ACT_SINGLE_START			102	// Start single-pulse measurement
//
#define ACT_SAVE_TO_ROM				200	// Save parameters to EEPROM module
#define ACT_RESTORE_FROM_ROM		201	// Restore parameters from EEPROM module
#define ACT_RESET_TO_DEFAULT		202	// Reset parameters to default values (only in controller memory)
#define ACT_LOCK_NV_AREA			203	// Lock modifications of parameters area
#define ACT_UNLOCK_NV_AREA			204	// Unlock modifications of parameters area (password-protected)
//
#define ACT_BOOT_LOADER_REQUEST		320	// Request reboot to bootloader
//
// ----------------------------------------

// REGISTERS
//
#define REG_PULSE_EDGE_TIME			0	// Edge time of heating current
#define REG_K_ADC_TO_CELSIUS		1 	// Coefficient for transform from ADC data to degree of Celsius
#define REG_K_IHC_TO_AMPHER			2
#define REG_K_TSP_TO_VOLTAGE		3
#define REG_K_UBAT_TO_VOLTS			4
#define REG_T_CORR_A				5	// Temperature correction factor
#define REG_T_CORR_B				6	// Temperature correction shift
#define REG_P_TOP_PULSE				7
#define REG_I_TOP_PULSE				8
#define REG_P_TOP_PULSE_D			9
#define REG_I_TOP_PULSE_D			10
#define REG_P_FRONT_PULSE			11
#define REG_I_FRONT_PULSE			12
#define REG_P_FRONT_PULSE_D			13
#define REG_I_FRONT_PULSE_D			14
#define REG_K_DAC_OUT				15
// ----------------------------------------
//
#define REG_MODE					128	// Measurement mode
#define REG_CURRENT_PULSE_SHORT		129	// The value of current pulse 0.1 - 10mS
#define REG_CURRENT_PULSE_LONG		130	// The value of current pulse 10mS - 1000S
#define REG_TIME_FIRST_PULSE		131	// The value of the duration of the first pulse
#define REG_TIME_LAST_PULSE			132	// The value of the duration of the last pulse
#define REG_TIME_FIRST_DELAY		133
#define REG_TIME_LAST_DELAY			134
#define REG_TIME_PULSE_MULT			135	// Multiplier for REG_TIME_FIRST_PULSE and REG_TIME_LAST_PULSE
#define REG_K_TIME_BETWEEN_PULSES	136 // Coefficient of time between pulses
#define REG_V_GATE_DUT				137 // IGBT gate voltage
#define REG_I_GATE_DUT				138	// Thirystor gate current
#define REG_M_CURRENT_SET			139 // The value of measuring current
#define REG_MEASURMENT_DELAY		140 // Measurment delay after heating current pulse
#define REG_TIME_REGISTRATION_TSP	141	// Time of registration TSP in graduarion mode
#define REG_DUT_TYPE				142	// Type of DUT - IGBT or thirystor
//
#define REG_PWD_1					180	// Unlock password location 1
#define REG_PWD_2					181	// Unlock password location 2
#define REG_PWD_3					182	// Unlock password location 3
#define REG_PWD_4					183	// Unlock password location 4
//
// ----------------------------------------
//
#define REG_DEV_STATE				192	// Device state
#define REG_FAULT_REASON			193	// Fault reason in the case DeviceState -> FAULT
#define REG_DISABLE_REASON			194	// Disbale reason in the case DeviceState -> DISABLE
#define REG_WARNING					195	// Warning if present
#define REG_PROBLEM					196	// Problem if present
//
#define REG_MAIN_VOLTAGE			197 // Voltage on capacitors
#define REG_M_CURRENT_REAL			198	// Measured measuring current
#define REG_H_CURRENT_REAL			199	// Measured heating current
#define REG_H_POWER					200	// Calculated heating power
//
#define REG_TSP_BEFORE_PULSE		201 // The value of thermal sensitive parameter before pulse
#define REG_T_COOL					202 // The themperature value of cooler  before pulse
#define REG_T_DUT_BEFORE_PULSE		203 // The themperature value of DUT  before pulse
//
#define REG_TSP_AFTER_PULSE			204 // The value of thermal sensitive parameter after pulse
#define REG_T_DUT_AFTER_PULSE		205 // The themperature value of DUT  after pulse
//
#define REG_GRADUATION_STATUS 		206 // Graduation ststus info
//
#define REG_CANA_BUSOFF_COUNTER		220 // Counter of bus-off states
#define REG_CANA_STATUS_REG			221	// CAN status register (32 bit)
#define REG_CANA_STATUS_REG_32		222
#define REG_CANA_DIAG_TEC			223	// CAN TEC
#define REG_CANA_DIAG_REC			224	// CAN REC
// ----------------------------------------


// ENDPOINTS
//
#define EP_I						1	// Current data
#define EP_U						2	// Voltage data
#define EP_P						3	// Power data
#define EP_Z						4	// Thermal resistance data
//



// FAULT CODES
//
#define FAULT_NONE					0	// No fault
#define FAULT_GARDUATION			1	// Graduation was not executing


// WARNING CODES
//
#define WARNING_NONE				0
#define WARNING_WATCHDOG_RESET		1001	// System has been reseted by WD

// DISABLE CODES
//
#define DISABLE_NONE				0
#define DISABLE_BAD_CLOCK			1001	// Problem with main oscillator

// USER ERROR CODES
//
#define ERR_NONE					0	// No error
#define ERR_CONFIGURATION_LOCKED	1	// Device is locked for writing
#define ERR_OPERATION_BLOCKED		2	// Operation can't be done due to current device state
#define ERR_DEVICE_NOT_READY		3	// Device isn't ready to switch state
#define ERR_WRONG_PWD				4	// Wrong password - unlock failed


#endif // __DEV_OBJ_DIC_H
