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
#define ACT_DBG_READ_IM				10	// Reading Im raw data
#define ACT_DBG_READ_IH				11	// Reading Ih raw data
#define ACT_DBG_READ_TSP			12	// Reading TSP raw data
#define ACT_DBG_READ_TCASE1			13	// Reading Tcase1 raw data
#define ACT_DBG_READ_TCASE2			14	// Reading Tcase2 raw data
#define ACT_DBG_READ_TCOOL1			15	// Reading Tcool1 raw data
#define ACT_DBG_READ_TCOOL2			16	// Reading Tcool2 raw data
#define ACT_DBG_READ_PROTECT_DATA	17	// Read state of ZthProtectionBoard
#define ACT_DBG_GATE_CURRENT		18	// Generate gate current
#define ACT_DBG_GATE_VOLTAGE_15V	19	// Generate gate voltage 15V
#define ACT_DBG_GATE_VOLTAGE_20V	20	// Generate gate voltage 20V
#define ACT_DBG_IM					21	// Generate Im
#define ACT_DBG_REG					22	// Generate gate pulse for power current source
#define ACT_DBG_LPS_CTRL			23	// Low power supply control
//
#define ACT_START_PROCESS			100	// Start measuring process
#define ACT_STOP_PROCESS			101	// Stop measuring process
#define ACT_STOP_HEATING			102	// Stop heating process
#define ACT_UPDATE					103	// Update parameters
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
#define REG_DAC_IG_P2				0	// Fine coefficient for Ig quadratic correction P2 x1e6
#define REG_DAC_IG_P1				1	// Fine coefficient for Ig quadratic correction P1 x1000
#define REG_DAC_IG_P0				2	// Fine coefficient for Ig quadratic correction P0 (in mA)
#define REG_DAC_IG_K				3	// Conversion coefficient K for Ig x1000
#define REG_DAC_IG_B				4	// Conversion offset for Ig (in mA)
//
#define REG_DAC_IH_P2				5	// Fine coefficient for Ih quadratic correction P2 x1e6
#define REG_DAC_IH_P1				6	// Fine coefficient for Ih quadratic correction P1 x1000
#define REG_DAC_IH_P0				7	// Fine coefficient for Ih quadratic correction P0 (in A)
#define REG_DAC_IH_K				8	// Conversion coefficient K for Ih x1000
#define REG_DAC_IH_B				9	// Conversion offset for Ih (in A)
//
#define REG_DAC_IM_P2				10	// Fine coefficient for Im quadratic correction P2 x1e6
#define REG_DAC_IM_P1				11	// Fine coefficient for Im quadratic correction P1 x1000
#define REG_DAC_IM_P0				12	// Fine coefficient for Im quadratic correction P0 (in mA)
#define REG_DAC_IM_K				13	// Conversion coefficient K for Im x1000
#define REG_DAC_IM_B				14	// Conversion offset for Im (in mA)
//
#define REG_ADC_IH_P2				15	// Fine coefficient for Ih quadratic correction P2 x1e6
#define REG_ADC_IH_P1				16	// Fine coefficient for Ih quadratic correction P1 x1000
#define REG_ADC_IH_P0				17	// Fine coefficient for Ih quadratic correction P0 (in A)
#define REG_ADC_IH_K				18	// Conversion coefficient K for Ih x1000
#define REG_ADC_IH_B				19	// Conversion offset for Ih (in A)
//
#define REG_ADC_IM_P2				20	// Fine coefficient for Im quadratic correction P2 x1e6
#define REG_ADC_IM_P1				21	// Fine coefficient for Im quadratic correction P1 x1000
#define REG_ADC_IM_P0				22	// Fine coefficient for Im quadratic correction P0 (in mA)
#define REG_ADC_IM_K				23	// Conversion coefficient K for Im x1000
#define REG_ADC_IM_B				24	// Conversion offset for Im (in mA)
//
#define REG_ADC_TSP_P2				25	// Fine coefficient for TSP quadratic correction P2 x1e6
#define REG_ADC_TSP_P1				26	// Fine coefficient for TSP quadratic correction P1 x1000
#define REG_ADC_TSP_P0				27	// Fine coefficient for TSP quadratic correction P0 (in mV)
#define REG_ADC_TSP_K				28	// Conversion coefficient K for TSP x1000
#define REG_ADC_TSP_B				29	// Conversion offset for TSP (in mV)
//
#define REG_ADC_T_K					30	// Conversion coefficient K for T x1000
#define REG_ADC_T_B					31	// Conversion offset for T (in C x100)
//
#define REG_ADC_T_CASE1_P2			32	// Fine coefficient for Tcase1 quadratic correction P2 x1e6
#define REG_ADC_T_CASE1_P1			33	// Fine coefficient for Tcase1 quadratic correction P1 x1000
#define REG_ADC_T_CASE1_P0			34	// Fine coefficient for Tcase1 quadratic correction P0 (in T x100)
//
#define REG_ADC_T_CASE2_P2			35	// Fine coefficient for Tcase2 quadratic correction P2 x1e6
#define REG_ADC_T_CASE2_P1			36	// Fine coefficient for Tcase2 quadratic correction P1 x1000
#define REG_ADC_T_CASE2_P0			37	// Fine coefficient for Tcase2 quadratic correction P0 (in T x100)
//
#define REG_ADC_T_COOL1_P2			38	// Fine coefficient for Tcool1 quadratic correction P2 x1e6
#define REG_ADC_T_COOL1_P1			39	// Fine coefficient for Tcool1 quadratic correction P1 x1000
#define REG_ADC_T_COOL1_P0			40	// Fine coefficient for Tcool1 quadratic correction P0 (in T x100)
//
#define REG_ADC_T_COOL2_P2			41	// Fine coefficient for Tcool2 quadratic correction P2 x1e6
#define REG_ADC_T_COOL2_P1			42	// Fine coefficient for Tcool2 quadratic correction P1 x1000
#define REG_ADC_T_COOL2_P0			43	// Fine coefficient for Tcool2 quadratic correction P0 (in T x100)
//
#define REG_CAP_VOLTAGE_K_N			44	// Capasitors voltage coefficient (N)
#define REG_CAP_VOLTAGE_K_D			45	// Capasitors voltage coefficient (D)
#define REG_CAP_VOLTAGE_THRESHOLD	46	// Capacitors voltage threshold ( in mV)
// 47 - 49
#define REG_PI_CTRL_IM_Kp			50	// Measurement current PI Kp x1000
#define REG_PI_CTRL_IM_Ki			51	// Measurement current PI Ki x1000
#define REG_PI_CTRL_IH_Kp			52	// Heating current PI Kp x1000
#define REG_PI_CTRL_IH_Ki			53	// Heating current PI Ki x1000
//
#define REG_MODE					128	// Operating mode (0 - Zth sequence pulses, 1 - Zth long pulse, 2 - Rth sequence pulses, 3 - graduation)
#define REG_DUT_TYPE				129	// Select DUT type (0 - thyristor, 1 - IGBT)
#define REG_COOLING_MODE			130	// Colling mode (0 - one-sided, 1 - double-sided)
#define REG_ZTH_PULSE_WIDTH_MIN		131	// Width of first pulse (in us)
#define REG_ZTH_PULSE_WIDTH_MAX		132	// Width of last pulse (in sec)
#define REG_ZTH_PAUSE				133	// Pause between pulses (in % x10 of REG_ZTH_PULSE_WIDTH_MAX)
#define REG_PULSE_WIDTH				134	// Width pulse in "Rth sequence pulses" and "graduation" modes (in ms)
#define REG_PAUSE					135	// Pause between pulses in "Rth sequence pulses" and "graduation" modes (in us)
#define REG_IMPULSE_CURRENT			136	// Heating current value if pulse width less than 10ms  (in A)
#define REG_HEATING_CURRENT			137	// Heating current value if pulse width above than 10ms  (in A)
#define REG_GATE_CURRENT			138	// Gate current value (in mA)
#define REG_MEASURING_CURRENT		139	// Measuring current value (in mA)
#define REG_DELAY					140	// Delay in measuring temperature-sensitive parameter (in us)
#define REG_T_MAX					141	// Maximum DUT temperature (in C x10)
#define REG_IGBT_VOLTAGE			142	// Set IGBT gate voltage (0 - 15V, 1 - 20V)

#define REG_DBG						150	// Debugging register
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
#define REG_ACTUAL_U_DUT			200	// Actual DUT voltage result (in mV)
#define REG_ACTUAL_I_DUT			201	// Actual DUT current result (in A)
#define REG_ACTUAL_P_DUT			202	// Actual DUT power dissipation (in W)
#define REG_ACTUAL_I_MEASUREMENT	203	// Actual measurement current (in mA)
#define REG_ACTUAL_T_CASE1			204	// Actual temperature of DUT case 1 (in C x100)
#define REG_ACTUAL_T_CASE2			205	// Actual temperature of DUT case 2 (in C x100)
#define REG_ACTUAL_T_COOL1			206	// Actual temperature of DUT cooler 1 (in C x100)
#define REG_ACTUAL_T_COOL2			207	// Actual temperature of DUT cooler 2 (in C x100)
#define REG_ACTUAL_CAP_VOLTAGE		208	// Actual capacitors voltage (in V)
//
#define REG_CANA_BUSOFF_COUNTER		220 // Counter of bus-off states
#define REG_CANA_STATUS_REG			221	// CAN status register (32 bit)
#define REG_CANA_STATUS_REG_32		222
#define REG_CANA_DIAG_TEC			223	// CAN TEC
#define REG_CANA_DIAG_REC			224	// CAN REC
// ----------------------------------------


// ENDPOINTS
//
#define EP_TSP						1	// Endpoint of temperature sensitive parameter
#define EP_T_CASE1					2	// Endpoint of DUT case 1 temperature
#define EP_T_CASE2					3	// Endpoint of DUT case 2 temperature
#define EP_T_COOL1					4	// Endpoint of DUT cooler 1 temperature
#define EP_T_COOL2					5	// Endpoint of DUT cooler 2 temperature
//



// FAULT CODES
//
#define FAULT_NONE					0	// No fault
#define FAULT_POWERON				1	// Power on fault


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
