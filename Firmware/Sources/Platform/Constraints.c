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
                                    		   {DAC_IH_K_MIN, DAC_IH_K_MAX, DAC_IH_K_DEF},										// 5
                                    		   {DAC_IH_B_MIN, DAC_IH_B_MAX, DAC_IH_B_DEF},										// 6
											   {DAC_IM_K_MIN, DAC_IM_K_MAX, DAC_IM_K_DEF},										// 7
											   {DAC_IM_B_MIN, DAC_IM_B_MAX, DAC_IM_B_DEF},										// 8
											   {IMPULSE_CURRENT_MIN, IMPULSE_CURRENT_MAX, IH_RANGE_THRESHOLD_DEF},				// 9
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
											   {ADC_T_K_MIN, ADC_T_K_MAX, ADC_T_K_DEF},											// 30
											   {ADC_T_B_MIN, ADC_T_B_MAX, ADC_T_B_DEF},											// 31
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
											   {CAP_V_THRESHOLD_MIN, CAP_V_THRESHOLD_MAX, CAP_V_THRESHOLD_DEF},					// 46
											   {P_ERR_THRESHOLD_MIN, P_ERR_THRESHOLD_MAX, P_ERR_THRESHOLD_DEF},					// 47
											   {0, 0, 0},																		// 48
											   {0, 0, 0},																		// 49
											   {REG_IM_KP_MIN, REG_IM_KP_MAX, REG_IM_KP_DEF},									// 50
											   {REG_IM_KI_MIN, REG_IM_KI_MAX, REG_IM_KI_DEF},									// 51
											   {REG_IH_KP_MIN, REG_IH_KP_MAX, REG_IH_KP_DEF},									// 52
											   {REG_IH_KI_MIN, REG_IH_KI_MAX, REG_IH_KI_DEF},									// 53
											   {REG_P_KP_MIN, REG_P_KP_MAX, REG_P_KP_DEF},										// 54
											   {REG_P_KI_MIN, REG_P_KI_MAX, REG_P_KI_DEF},										// 55
											   {0, 0, 0},																		// 56
											   {0, 0, 0},																		// 57
											   {0, 0, 0},																		// 58
											   {0, 0, 0},																		// 59
											   {FALSE, TRUE, FALSE},															// 60
											   {0, 0, 0},																		// 61
											   {0, 0, 0},																		// 62
											   {0, 0, 0},																		// 63
											   {0, 0, 0},																		// 64
											   {0, 0, 0},																		// 65
											   {0, 0, 0},																		// 66
											   {0, 0, 0},																		// 67
											   {0, 0, 0},																		// 68
											   {0, 0, 0},																		// 69
											   {0, 0, 0},																		// 70
											   {0, 0, 0},																		// 71
											   {0, 0, 0},																		// 72
											   {0, 0, 0},																		// 73
											   {0, 0, 0},																		// 74
											   {0, 0, 0},																		// 75
											   {0, 0, 0},																		// 76
											   {0, 0, 0},																		// 77
											   {0, 0, 0},																		// 78
											   {0, 0, 0},																		// 79
											   {0, 0, 0},																		// 80
											   {0, 0, 0},																		// 81
											   {0, 0, 0},																		// 82
											   {0, 0, 0},																		// 83
											   {0, 0, 0},																		// 84
											   {0, 0, 0},																		// 85
											   {0, 0, 0},																		// 86
											   {0, 0, 0},																		// 87
											   {0, 0, 0},																		// 88
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
                                    		   {0, MODE_GRADUATION, 0},															// 128
											   {DUT_THYRISTOR, DUT_IGBT, DUT_THYRISTOR},										// 129
											   {COOLING_ONE_SIDE, COOLING_DOUBLE_SIDE, COOLING_ONE_SIDE},						// 130
											   {ZTH_PULSE_MIN_WIDTH_MIN, ZTH_PULSE_MIN_WIDTH_MAX, 0},							// 131
											   {ZTH_PULSE_MAX_WIDTH_MIN, ZTH_PULSE_MAX_WIDTH_MAX, 0},							// 132
											   {ZTH_COOLING_TIME_MIN, ZTH_COOLING_TIME_MAX, ZTH_COOLING_TIME_DEF},				// 133
											   {PULSE_WIDTH_MIN, PULSE_WIDTH_MAX, 0},											// 134
											   {COOLING_TIME_MIN, COOLING_TIME_MAX, 0},											// 135
											   {GRADUATION_TIME_MIN, GRADUATION_TIME_MAX, 0},									// 136
											   {IMPULSE_CURRENT_MIN, IMPULSE_CURRENT_MAX, 0},									// 137
											   {HEATING_CURRENT_MIN, HEATING_CURRENT_MAX, 0},									// 138
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
											   {0, 0, 0},																		// 151
											   {0, 0, 0},																		// 152
											   {0, 0, 0},																		// 153
											   {0, 0, 0},																		// 154
											   {0, 0, 0},																		// 155
											   {0, 0, 0},																		// 156
											   {0, 0, 0},																		// 157
											   {0, 0, 0},																		// 158
											   {0, 0, 0},																		// 159
											   {0, 0, 0},																		// 160
											   {0, 0, 0},																		// 161
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
