// -----------------------------------------
// Constraints for tunable parameters
// ----------------------------------------

// Header
#include "Constraints.h"
#include "DeviceObjectDictionary.h"

#define NO		0	// equal to FALSE
#define YES		1	// equal to TRUE

// Constants
//
const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE] =
                                       {
                                    		   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 0
                                    		   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 1
                                    		   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 2
                                    		   {DAC_IG_K_MIN, DAC_IG_K_MAX, DAC_IG_K_DEF},										// 3
                                    		   {DAC_IG_B_MIN, DAC_IG_B_MAX, DAC_IG_B_DEF},										// 4
                                    		   {0, 0, 0},																		// 5
                                    		   {0, 0, 0},																		// 6
											   {DAC_IM_K_MIN, DAC_IM_K_MAX, DAC_IM_K_DEF},										// 7
											   {DAC_IM_B_MIN, DAC_IM_B_MAX, DAC_IM_B_DEF},										// 8
											   {IH_RANGE_THRESHOLD_MIN, IH_RANGE_THRESHOLD_MAX, IH_RANGE_THRESHOLD_DEF},		// 9
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 10
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 11
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 12
											   {ADC_IH_R0_K_MIN, ADC_IH_R0_K_MAX, ADC_IH_R0_K_DEF},								// 13
											   {ADC_IH_R0_B_MIN, ADC_IH_R0_B_MAX, ADC_IH_R0_B_DEF},								// 14
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 15
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 16
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 17
											   {ADC_IH_R1_K_MIN, ADC_IH_R1_K_MAX, ADC_IH_R1_K_DEF},								// 18
											   {ADC_IH_R1_B_MIN, ADC_IH_R1_B_MAX, ADC_IH_R1_B_DEF},								// 19
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 20
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 21
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 22
											   {ADC_IM_K_MIN, ADC_IM_K_MAX, ADC_IM_K_DEF},										// 23
											   {ADC_IM_B_MIN, ADC_IM_B_MAX, ADC_IM_B_DEF},										// 24
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 25
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 26
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 27
											   {ADC_TSP_K_MIN, ADC_TSP_K_MAX, ADC_TSP_K_DEF},									// 28
											   {ADC_TSP_B_MIN, ADC_TSP_B_MAX, ADC_TSP_B_DEF},									// 29
											   {0, 0, 0},																		// 30
											   {0, 0, 0},																		// 31
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 32
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 33
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 34
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 35
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 36
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 37
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 38
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 39
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 40
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 41
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 42
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 43
											   {CAP_VOLTAGE_K_N_MIN, CAP_VOLTAGE_K_N_MAX, CAP_VOLTAGE_K_N_DEF},					// 44
											   {CAP_VOLTAGE_K_D_MIN, CAP_VOLTAGE_K_D_MAX, CAP_VOLTAGE_K_D_DEF},					// 45
											   {CAP_V_THRESHOLD_MIN, CAP_V_THRESHOLD_MAX, CAP_V_THRESHOLD_L_DEF},				// 46
											   {0, 0, 0},																		// 47
											   {FALSE, TRUE, FALSE},															// 48
											   {FLTR_ERR_THRESHOLD_MIN, FLTR_ERR_THRESHOLD_MAX, FLTR_ERR_THRESHOLD_DEF},		// 49
											   {REG_IM_KP_MIN, REG_IM_KP_MAX, REG_IM_KP_DEF},									// 50
											   {REG_IM_KI_MIN, REG_IM_KI_MAX, REG_IM_KI_DEF},									// 51
											   {REG_IH_KP_MIN, REG_IH_KP_MAX, REG_IH_KP_DEF},									// 52
											   {REG_IH_KI_MIN, REG_IH_KI_MAX, REG_IH_KI_DEF},									// 53
											   {REG_P_KP_MIN, REG_P_KP_MAX, REG_P_KP_DEF},										// 54
											   {REG_P_KI_MIN, REG_P_KI_MAX, REG_P_KI_DEF},										// 55
											   {DAC_IH_K_MIN, DAC_IH_K_MAX, DAC_IH_K_DEF},										// 56
											   {DAC_IH_B_MIN, DAC_IH_B_MAX, DAC_IH_B_DEF},										// 57
											   {DAC_IH_K_MIN, DAC_IH_K_MAX, DAC_IH_K_DEF},										// 58
											   {DAC_IH_B_MIN, DAC_IH_B_MAX, DAC_IH_B_DEF},										// 59
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 60
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 61
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 62
											   {COEF_P2_MIN, COEF_P2_MAX, COEF_P2_DEF},											// 63
											   {COEF_P1_MIN, COEF_P1_MAX, COEF_P1_DEF},											// 64
											   {COEF_P0_MIN, COEF_P0_MAX, COEF_P0_DEF},											// 65
											   {FALSE, TRUE, FALSE},															// 66
											   {0, INT16U_MAX, 0},																// 67
											   {CAP_V_THRESHOLD_MIN, CAP_V_THRESHOLD_MAX, CAP_V_THRESHOLD_H_DEF},				// 68
											   {I_WIDTH_LESS_2MS_MIN, I_WIDTH_LESS_2MS_MAX, I_THRE_FOR_CAP_V_THRE_DEF},			// 69
											   {0, SKIP_CYCLE_MAX, SKIP_CYCLE_DEF},												// 70
											   {FALSE, TRUE, TRUE},																// 71
											   {POWER_SET_DELAY_MIN, POWER_SET_DELAY_MAX, POWER_SET_DELAY_DEF},					// 72
											   {POWER_SET_DELAY_MIN, POWER_SET_DELAY_MAX, POWER_SET_DELAY_DEF},					// 73
											   {POWER_SET_I_ERR_MIN, POWER_SET_I_ERR_MAX, POWER_SET_I_ERR_DEF},					// 74
											   {0, 0, 0},																		// 75
											   {0, 0, 0},																		// 76
											   {0, 0, 0},																		// 77
											   {0, 0, 0},																		// 78
											   {0, 0, 0},																		// 79
											   {ADC_T_K_MIN, ADC_T_K_MAX, ADC_T_CASE1_K_DEF},									// 80
											   {ADC_T_B_MIN, ADC_T_B_MAX, ADC_T_CASE1_B_DEF},									// 81
											   {ADC_T_K_MIN, ADC_T_K_MAX, ADC_T_COOL1_K_DEF},									// 82
											   {ADC_T_B_MIN, ADC_T_B_MAX, ADC_T_COOL1_B_DEF},									// 83
											   {ADC_T_K_MIN, ADC_T_K_MAX, ADC_T_CASE2_K_DEF},									// 84
											   {ADC_T_B_MIN, ADC_T_B_MAX, ADC_T_CASE2_B_DEF},									// 85
											   {ADC_T_K_MIN, ADC_T_K_MAX, ADC_T_COOL2_K_DEF},									// 86
											   {ADC_T_B_MIN, ADC_T_B_MAX, ADC_T_COOL2_B_DEF},									// 87
											   {ADC_T_K_DIV_MIN, ADC_T_K_DIV_MAX, ADC_T_K_DIV_DEF},								// 88
											   {0, 0, 0},																		// 89
											   {0, 0, 0},																		// 90
											   {0, 0, 0},																		// 91
											   {0, 0, 0},																		// 92
											   {0, 0, 0},																		// 93
											   {0, 0, 0},																		// 94
											   {0, 0, 0},																		// 95
											   {0, 0, 0},																		// 96
											   {0, 0, 0},																		// 97
											   {0, 0, 0},																		// 98
											   {0, 0, 0},																		// 99
											   {0, 0, 0},																		// 100
                                    		   {0, 0, 0},																		// 101
                                    		   {0, 0, 0},																		// 102
                                    		   {0, 0, 0},																		// 103
                                    		   {0, 0, 0},																		// 104
                                    		   {0, 0, 0},																		// 105
                                    		   {0, 0, 0},																		// 106
                                    		   {0, 0, 0},																		// 107
                                    		   {0, 0, 0},																		// 108
                                    		   {0, 0, 0},																		// 109
                                    		   {0, 0, 0},																		// 110
                                    		   {0, 0, 0},																		// 111
                                    		   {0, 0, 0},																		// 112
                                    		   {0, 0, 0},																		// 113
                                    		   {0, 0, 0},																		// 114
                                    		   {0, 0, 0},																		// 115
                                    		   {0, 0, 0},																		// 116
                                    		   {0, 0, 0},																		// 117
                                    		   {0, 0, 0},																		// 118
                                    		   {0, 0, 0},																		// 119
                                    		   {0, 0, 0},																		// 120
                                    		   {0, 0, 0},																		// 121
                                    		   {0, 0, 0},																		// 122
                                    		   {0, 0, 0},																		// 123
                                    		   {0, 0, 0},																		// 124
                                    		   {0, 0, 0},																		// 125
                                    		   {0, 0, 0},																		// 126
                                    		   {0, 0, 0}																		// 127
                                       };

const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START] =
                                      {
                                    		   {0, MODE_IGATE, 0},																// 128
											   {DUT_THYRISTOR, DUT_IGBT, DUT_THYRISTOR},										// 129
											   {COOLING_ONE_SIDE, COOLING_DOUBLE_SIDE, COOLING_ONE_SIDE},						// 130
											   {ZTH_PULSE_MIN_WIDTH_MIN, ZTH_PULSE_MIN_WIDTH_MAX, 0},							// 131
											   {0, INT16U_MAX, 0},																// 132
											   {0, INT16U_MAX, 0},																// 133
											   {ZTH_COOLING_TIME_MIN, ZTH_COOLING_TIME_MAX, ZTH_COOLING_TIME_DEF},				// 134
											   {COOLING_TIME_MIN, COOLING_TIME_MAX, 0},											// 135
											   {I_WIDTH_LESS_2MS_MIN, I_WIDTH_LESS_2MS_MAX, 0},									// 136
											   {I_WIDTH_LESS_10MS_MIN, I_WIDTH_LESS_10MS_MAX, 0},								// 137
											   {I_WIDTH_ABOVE_10MS_MIN, I_WIDTH_ABOVE_10MS_MAX, 0},								// 138
											   {GATE_CURRENT_MIN, GATE_CURRENT_MAX, GATE_CURRENT_DEF},							// 139
                                    		   {MEASURING_CURRENT_MIN, MEASURING_CURRENT_MAX, MEASURING_CURRENT_DEF},			// 140
                                    		   {DELAY_MIN, DELAY_MAX, DELAY_DEF},												// 141
                                    		   {T_MIN, T_MAX, T_DEF},															// 142
                                    		   {IGBT_V_GATE_15V, IGBT_V_GATE_20V, IGBT_V_GATE_15V},								// 143
                                    		   {0, 0, 0},																		// 144
                                    		   {0, 0, 0},																		// 145
                                    		   {0, 0, 0},																		// 146
                                    		   {0, 0, 0},																		// 147
											   {0, 0, 0},																		// 148
											   {0, 0, 0},																		// 149
											   {0, INT16U_MAX, 0},																// 150
											   {0, INT16U_MAX, 0},																// 151
											   {0, 0, 0},																		// 152
											   {0, 0, 0},																		// 153
											   {0, 0, 0},																		// 154
											   {0, 0, 0},																		// 155
											   {0, 0, 0},																		// 156
											   {0, 0, 0},																		// 157
											   {0, 0, 0},																		// 158
											   {0, 0, 0},																		// 159
											   {0, INT16U_MAX, 0},																// 160
											   {0, INT16U_MAX, 0},																// 161
											   {0, 0, 0},																		// 162
											   {0, 0, 0},																		// 163
											   {0, 0, 0},																		// 164
											   {0, 0, 0},																		// 165
											   {0, 0, 0},																		// 166
											   {0, 0, 0},																		// 167
											   {0, 0, 0},																		// 168
											   {0, 0, 0},																		// 169
											   {0, 0, 0},																		// 170
											   {0, 0, 0},																		// 171
											   {0, 0, 0},																		// 172
											   {0, 0, 0},																		// 173
											   {0, 0, 0},																		// 174
											   {0, 0, 0},																		// 175
											   {0, 0, 0},																		// 176
											   {0, 0, 0},																		// 177
											   {0, 0, 0},																		// 178
											   {0, 0, 0},																		// 179
											   {0, 0, 0},																		// 180
											   {0, 0, 0},																		// 181
											   {0, 0, 0},																		// 182
											   {0, 0, 0},																		// 183
											   {0, 0, 0},																		// 184
											   {0, 0, 0},																		// 185
											   {0, 0, 0},																		// 186
											   {0, 0, 0},																		// 187
											   {0, 0, 0},																		// 188
											   {0, 0, 0},																		// 189
											   {0, 0, 0},																		// 190
											   {0, 0, 0}																		// 191
                                      };

// No more
