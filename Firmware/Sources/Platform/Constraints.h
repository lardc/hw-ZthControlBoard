// -----------------------------------------
// Constraints for tunable parameters
// ----------------------------------------

#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
//
#include "DataTable.h"
#include "Global.h"

// Types
//
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;

//
#define COEF_P2_MIN				0
#define COEF_P2_MAX				INT16U_MAX
#define COEF_P2_DEF				0
//
#define COEF_P1_MIN				0
#define COEF_P1_MAX				INT16U_MAX
#define COEF_P1_DEF				1000
//
#define COEF_P0_MIN				0
#define COEF_P0_MAX				INT16U_MAX
#define COEF_P0_DEF				0
//
#define DAC_IG_K_MIN			0
#define DAC_IG_K_MAX			INT16U_MAX
#define DAC_IG_K_DEF			1000
//
#define DAC_IH_K_MIN			0
#define DAC_IH_K_MAX			INT16U_MAX
#define DAC_IH_K_DEF			1000
//
#define DAC_IM_K_MIN			0
#define DAC_IM_K_MAX			INT16U_MAX
#define DAC_IM_K_DEF			1000
//
#define DAC_IG_B_MIN			0
#define DAC_IG_B_MAX			INT16U_MAX
#define DAC_IG_B_DEF			0
//
#define DAC_IH_B_MIN			0
#define DAC_IH_B_MAX			INT16U_MAX
#define DAC_IH_B_DEF			0
//
#define DAC_IM_B_MIN			0
#define DAC_IM_B_MAX			INT16U_MAX
#define DAC_IM_B_DEF			0
//
#define ADC_IH_K_MIN			0
#define ADC_IH_K_MAX			INT16U_MAX
#define ADC_IH_K_DEF			1000
//
#define ADC_IM_K_MIN			0
#define ADC_IM_K_MAX			INT16U_MAX
#define ADC_IM_K_DEF			1000
//
#define ADC_TSP_K_MIN			0
#define ADC_TSP_K_MAX			INT16U_MAX
#define ADC_TSP_K_DEF			1000
//
#define ADC_T_K_MIN				0
#define ADC_T_K_MAX				INT16U_MAX
#define ADC_T_K_DEF				1000
//
#define ADC_IH_B_MIN			0
#define ADC_IH_B_MAX			INT16U_MAX
#define ADC_IH_B_DEF			1000
//
#define ADC_IM_B_MIN			0
#define ADC_IM_B_MAX			INT16U_MAX
#define ADC_IM_B_DEF			1000
//
#define ADC_TSP_B_MIN			0
#define ADC_TSP_B_MAX			INT16U_MAX
#define ADC_TSP_B_DEF			1000
//
#define ADC_T_B_MIN				0
#define ADC_T_B_MAX				INT16U_MAX
#define ADC_T_B_DEF				1000
//
#define CAP_VOLTAGE_K_N_MIN		0
#define CAP_VOLTAGE_K_N_MAX		INT16U_MAX
#define CAP_VOLTAGE_K_N_DEF		1000
//
#define CAP_VOLTAGE_K_D_MIN		1
#define CAP_VOLTAGE_K_D_MAX		INT16U_MAX
#define CAP_VOLTAGE_K_D_DEF		1000
//
#define CAP_V_THRESHOLD_MIN		0
#define CAP_V_THRESHOLD_MAX		25000
#define	CAP_V_THRESHOLD_DEF		12000
//
#define P_ERR_THRESHOLD_MIN		0
#define P_ERR_THRESHOLD_MAX		100
#define P_ERR_THRESHOLD_DEF		5
//
#define REG_IM_KP_MIN			0
#define REG_IM_KP_MAX			INT16U_MAX
#define REG_IM_KP_DEF			0
//
#define REG_IM_KI_MIN			0
#define REG_IM_KI_MAX			INT16U_MAX
#define REG_IM_KI_DEF			0
//
#define REG_IH_KP_MIN			0
#define REG_IH_KP_MAX			INT16U_MAX
#define REG_IH_KP_DEF			0
//
#define REG_IH_KI_MIN			0
#define REG_IH_KI_MAX			INT16U_MAX
#define REG_IH_KI_DEF			0
//
#define REG_P_KP_MIN			0
#define REG_P_KP_MAX			INT16U_MAX
#define REG_P_KP_DEF			0
//
#define REG_P_KI_MIN			0
#define REG_P_KI_MAX			INT16U_MAX
#define REG_P_KI_DEF			0
//
#define MODE_GRADUATION			3
#define DUT_THYRISTOR			0
#define DUT_IGBT				1
#define COOLING_ONE_SIDE		0
#define COOLING_DOUBLE_SIDE		1
//
#define ZTH_PULSE_MIN_WIDTH_MIN	100
#define ZTH_PULSE_MIN_WIDTH_MAX	10000
//
#define ZTH_PULSE_MAX_WIDTH_MIN	1
#define ZTH_PULSE_MAX_WIDTH_MAX	1000
//
#define ZTH_COOLING_TIME_MIN	0
#define ZTH_COOLING_TIME_MAX	1000
#define ZTH_COOLING_TIME_DEF	200
//
#define PULSE_WIDTH_MIN			1
#define PULSE_WIDTH_MAX			1000
//
#define COOLING_TIME_MIN		100
#define COOLING_TIME_MAX		10000
//
#define GRADUATION_TIME_MIN		10
#define GRADUATION_TIME_MAX		100
//
#define IMPULSE_CURRENT_MIN		100
#define IMPULSE_CURRENT_MAX		2500
//
#define HEATING_CURRENT_MIN		100
#define HEATING_CURRENT_MAX		1000
//
#define GATE_CURRENT_MIN		500
#define GATE_CURRENT_MAX		3000
#define GATE_CURRENT_DEF		1000
//
#define MEASURING_CURRENT_MIN	500
#define	MEASURING_CURRENT_MAX	5000
#define MEASURING_CURRENT_DEF	2000
//
#define DELAY_MIN				100
#define DELAY_MAX				1000
#define DELAY_DEF				200
//
#define T_MIN					900
#define T_MAX					1000
#define T_DEF					950
//
#define IGBT_V_GATE_15V			0
#define IGBT_V_GATE_20V			1


// Variables
//
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];


#endif // __CONSTRAINTS_H
