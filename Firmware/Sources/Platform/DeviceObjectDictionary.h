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
#define ACT_DBG_SYNC_OSC			24	// Generate pulse for oscilloscope
#define ACT_DBG_READ_PROTECTION		25	// Read signals states from ZthProtectionBoard
#define ACT_IH_SET_RANGE			26	// Set Ih renge in debug mode
//
#define ACT_START_IM				50	// Start generating Im
#define ACT_START_IH				51	// Start generating Ih
#define ACT_START_GATE				52	// Start generating Ig
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
// 5 - 6
#define REG_DAC_IM_K				7	// Conversion coefficient K for Im x1000
#define REG_DAC_IM_B				8	// Conversion offset for Im (in mA)
//
#define REG_IH_RANGE_THRESHOLD		9	// Heating current range threshold (in A)
//
#define REG_ADC_IH_R0_P2			10	// Fine coefficient for Ih quadratic correction P2 x1e6
#define REG_ADC_IH_R0_P1			11	// Fine coefficient for Ih quadratic correction P1 x1000
#define REG_ADC_IH_R0_P0			12	// Fine coefficient for Ih quadratic correction P0 (in A)
#define REG_ADC_IH_R0_K				13	// Conversion coefficient K for Ih x1000
#define REG_ADC_IH_R0_B				14	// Conversion offset for Ih (in A)
//
#define REG_ADC_IH_R1_P2			15	// Fine coefficient for Ih quadratic correction P2 x1e6
#define REG_ADC_IH_R1_P1			16	// Fine coefficient for Ih quadratic correction P1 x1000
#define REG_ADC_IH_R1_P0			17	// Fine coefficient for Ih quadratic correction P0 (in A)
#define REG_ADC_IH_R1_K				18	// Conversion coefficient K for Ih x1000
#define REG_ADC_IH_R1_B				19	// Conversion offset for Ih (in A)
//
#define REG_ADC_IM_P2				20	// Fine coefficient for Im quadratic correction P2 x1e6
#define REG_ADC_IM_P1				21	// Fine coefficient for Im quadratic correction P1 x1000
#define REG_ADC_IM_P0				22	// Fine coefficient for Im quadratic correction P0 (in mA)
#define REG_ADC_IM_K				23	// Conversion coefficient K for Im x1000
#define REG_ADC_IM_B				24	// Conversion offset for Im (in mA x100)
//
#define REG_ADC_TSP_P2				25	// Fine coefficient for TSP quadratic correction P2 x1e6
#define REG_ADC_TSP_P1				26	// Fine coefficient for TSP quadratic correction P1 x1000
#define REG_ADC_TSP_P0				27	// Fine coefficient for TSP quadratic correction P0 (in mV)
#define REG_ADC_TSP_K				28	// Conversion coefficient K for TSP x1000
#define REG_ADC_TSP_B				29	// Conversion offset for TSP (in mV)
// 30 - 31
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
#define REG_CAP_VOLTAGE_THRE_L		46	// Capacitors voltage low threshold ( in V x10)
// 47
#define REG_FILTER_ACTIVE			48	// Enable or disable data filtering
#define REG_FILTER_ERR_THRESHOLD	49	// Error level to switch on filtered data (in %)
//
#define REG_PI_CTRL_IM_Kp			50	// Measurement current PI Kp x1000
#define REG_PI_CTRL_IM_Ki			51	// Measurement current PI Ki
#define REG_PI_CTRL_IH_Kp			52	// Heating current PI Kp x1000
#define REG_PI_CTRL_IH_Ki			53	// Heating current PI Ki
#define REG_PI_CTRL_P_Kp			54	// Heating current PI Kp x1000
#define REG_PI_CTRL_P_Ki			55	// Heating current PI Ki
//
#define REG_DAC_IH_R0_K				56	// Conversion coefficient K for Ih range 0 x1000
#define REG_DAC_IH_R0_B				57	// Conversion offset for Ih (in A) range 0
#define REG_DAC_IH_R1_K				58	// Conversion coefficient K for Ih range 1 x1000
#define REG_DAC_IH_R1_B				59	// Conversion offset for Ih (in A) range 1
//
#define REG_DAC_IH_R0_P2			60	// Conversion coefficient P2 for Ih range 0 x1e6
#define REG_DAC_IH_R0_P1			61	// Conversion coefficient P1 for Ih range 0 x1000
#define REG_DAC_IH_R0_P0			62	// Offset P0 for Ih range 0
//
#define REG_DAC_IH_R1_P2			63	// Conversion coefficient P2 for Ih range 0 x1e6
#define REG_DAC_IH_R1_P1			64	// Conversion coefficient P1 for Ih range 0 x1000
#define REG_DAC_IH_R1_P0			65	// Offset P0 for Ih range 0
//
#define REG_MUTE_PROTECTION			66	// Ignore protection
#define REG_SCOPE_STEP				67	// Scope step
//
#define REG_CAP_VOLTAGE_THRE_H		68	// Capacitors voltage high threshold ( in V x10)
#define REG_I_THRE_SET_CAP_V_THRE	69	// Current thresholt for changing capacitors voltage threshold (in A)
//
#define REG_REGULATOR_SKIP_CYCLE	70	// Number of skip regulator cycle
#define REG_REGULATOR_POWER_CTRL	71	// Enable or disable of power regulator
#define REG_POWER_SET_DEL_BIPOLAR	72	// Time after starting the current to set the power dissipation reference for bipolar device (in us x0.01)
#define REG_POWER_SET_DEL_IGBT		73	// Time after starting the current to set the power dissipation reference for IGBT (in us x0.01)
//
#define REG_ADC_T_CASE1_K			80	// Conversion coefficient K for T x1000
#define REG_ADC_T_CASE1_B			81	// Conversion offset for T (in C x100)
#define REG_ADC_T_COOL1_K			82	// Conversion coefficient K for T x1000
#define REG_ADC_T_COOL1_B			83	// Conversion offset for T (in C x100)
#define REG_ADC_T_CASE2_K			84	// Conversion coefficient K for T x1000
#define REG_ADC_T_CASE2_B			85	// Conversion offset for T (in C x100)
#define REG_ADC_T_COOL2_K			86	// Conversion coefficient K for T x1000
#define REG_ADC_T_COOL2_B			87	// Conversion offset for T (in C x100)
//
#define REG_ADC_T_K_DIV				88	// REG_ADC_Tx_K divider
//
#define REG_MODE					128	// Operating mode (The list of modes is presented in the file Controller.c)
#define REG_DUT_TYPE				129	// Select DUT type (0 - thyristor, 1 - IGBT)
#define REG_COOLING_MODE			130	// Colling mode (0 - one-sided, 1 - double-sided)
#define REG_PULSE_WIDTH_MIN			131	// Width of first pulse (in us x0.01)
#define REG_PULSE_WIDTH_MAX_L		132	// Width of last pulse (in us x0.01, LSB)
#define REG_PULSE_WIDTH_MAX_H		133	// Width of last pulse (in us x0.01, MSB)
#define REG_ZTH_PAUSE				134	// Cooling time between pulses (in %)
#define REG_PAUSE					135	// Time between pulses in "Rth sequence pulses" and "graduation" modes (in us x0.01)
#define REG_I_WIDTH_LESS_2MS		136	// Heating current value if pulse width less than 10ms  (in A)
#define REG_I_WIDTH_LESS_10MS		137	// Heating current value if pulse width less than 10ms  (in A)
#define REG_I_WIDTH_ABOVE_10MS		138	// Heating current value if pulse width above than 10ms  (in A)
#define REG_GATE_CURRENT			139	// Gate current value (in mA)
#define REG_MEASURING_CURRENT		140	// Measuring current value (in mA)
#define REG_MEASUREMENT_DELAY		141	// Delay of measuring temperature-sensitive parameter (in us)
#define REG_T_MAX					142	// Maximum DUT temperature (in C x10)
#define REG_IGBT_V_GATE				143	// Set IGBT gate voltage (0 - 15V, 1 - 20V)
//
#define REG_DBG						150	// Debugging register
//
#define REG_DBG_WHOLE				155	// Whole part of measuring while debugging
#define REG_DBG_FRACTION			156	// Fraction part of measuring while debugging
//
#define REG_DRCU_X_NUM				160	// Register number or command number for DRCUControlBoard
#define REG_DRCU_DATA_IO			161	// Input / output DRCUControlBoard data
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
#define REG_OP_RESULT				197	// Indicates that test is done and there is result or fault
#define REG_FAULT_REASON_EXT		198	// Fault reason extended code
#define REG_LOGIC_STATE				199	// Logic state
//
#define REG_ACTUAL_U_DUT			200	// Actual DUT voltage result (in mV x10)
#define REG_ACTUAL_I_DUT			201	// Actual DUT current result (in A x10)
#define REG_ACTUAL_P_DUT_WHOLE		202	// Actual whole part of DUT power dissipation (in W)
#define REG_ACTUAL_P_DUT_FRACT		203	// Actual fraction part of DUT power dissipation (in W)
#define REG_ACTUAL_P_TARGET_WHOLE	204	// Actual whole part of target power dissipation (in W)
#define REG_ACTUAL_P_TARGET_FRACT	205	// Actual fraction part of target power dissipation (in W)
#define REG_ACTUAL_I_MEASUREMENT	206	// Actual measurement current (in mA x10)
#define REG_ACTUAL_T_CASE1			207	// Actual temperature of DUT case 1 (in C x10)
#define REG_ACTUAL_T_CASE2			208	// Actual temperature of DUT case 2 (in C x10)
#define REG_ACTUAL_T_COOL1			209	// Actual temperature of DUT cooler 1 (in C x10)
#define REG_ACTUAL_T_COOL2			210	// Actual temperature of DUT cooler 2 (in C x10)
#define REG_ACTUAL_TSP				211	// Actual TSP result (in mV)
#define REG_ACTUAL_CAP_VOLTAGE		212	// Actual capacitors voltage (in V x10)
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
#define EP_ERR_IM					6	// Endpoint of Im regulator error
#define EP_ERR_IH					7	// Endpoint of Ih regulator error
#define EP_ERR_P					8	// Endpoint of P regulator error
//
#define EP_IM						9	// Endpoint contains measured data of the measuring current
#define EP_IH						10	// Endpoint contains measured data of the heating current
#define EP_P						11	// Endpoint contains measured data of the power dissipation
//

// OPERATION RESULTS
//
#define OPRESULT_NONE				0	// No information or not finished
#define OPRESULT_OK					1	// Operation was successful
#define OPRESULT_FAIL				2	// Operation failed

// FAULT CODES
//
#define FAULT_NONE					0	// No fault
#define FAULT_POWERON				1	// Power on fault
#define FAULT_WATER					2	// There is no water
#define FAULT_TR1					3	// Transformer 1 is overheated
#define FAULT_TR2					4	// Transformer 2 is overheated
#define FAULT_REC					5	// Rectifier is overheated
//
#define FAULT_PROTOCOL				10	// Protocol error
#define FAULT_DRCU_PWRON			11	// DRCU power on fault
#define FAULT_DRCU_WRONG_STATE		12	// DRCU wrong state
#define FAULT_DRCU_WAIT_STATE		13	// DRCU waot state timeout


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
