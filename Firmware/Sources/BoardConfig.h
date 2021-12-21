// -----------------------------------------
// Board parameters
// ----------------------------------------

#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

// Include
#include <ZwBase.h>

// Program build mode
//
#define BOOT_FROM_FLASH						// normal mode
#define RAM_CACHE_SPI_X						// cache SPI-A(BCD) functions

// Board options
#define DSP28_2809							// on-board CPU
#define OSC_FRQ				(20MHz)			// on-board oscillator
#define CPU_FRQ_MHZ			100				// CPU frequency = 100MHz
#define CPU_FRQ				(CPU_FRQ_MHZ * 1000000L)
#define SYS_HSP_FREQ		(CPU_FRQ / 2) 	// High-speed bus frequency
#define SYS_LSP_FREQ		(CPU_FRQ / 4) 	// Low-speed bus frequency
//
#define ZW_PWM_DUTY_BASE	1024

// Peripheral options
#define HWUSE_SPI_A
#define HWUSE_SPI_B
#define HWUSE_SPI_C
#define HWUSE_SPI_D
#define HWUSE_SCI_A

// IO placement
//
//SPIA
#define SPI_A_QSEL			GPAQSEL2
#define SPI_A_MUX			GPAMUX2
#define SPI_A_SIMO			GPIO16
#define SPI_A_SOMI			GPIO17
#define SPI_A_CLK			GPIO18
#define SPI_A_CS			GPIO19

//SPIB
#define SPI_B_QSEL			GPAQSEL1
#define SPI_B_MUX			GPAMUX1
#define SPI_B_SIMO			GPIO12
#define SPI_B_SOMI			GPIO13
#define SPI_B_CLK			GPIO14
#define SPI_B_CS			GPIO15

//SPIC
#define SPI_C_QSEL			GPAQSEL2
#define SPI_C_MUX			GPAMUX2
#define SPI_C_SIMO			GPIO20
#define SPI_C_SOMI			GPIO21
#define SPI_C_CLK			GPIO22
#define SPI_C_CS			GPIO23

//SPID
#define SPI_D_QSEL			GPAQSEL1
#define SPI_D_MUX			GPAMUX1
#define SPI_D_SIMO			GPIO1
#define SPI_D_SOMI			GPIO3
#define SPI_D_CLK			GPIO5
#define SPI_D_CS			GPIO7
//
#define SCI_A_QSEL			GPAQSEL2
#define SCI_A_MUX			GPAMUX2
#define SCI_A_TX			GPIO29
#define SCI_A_RX			GPIO28
#define SCI_A_MUX_SELECTOR	1
//
#define PIN_T_TR2			0
#define PIN_T_REC			2
#define PIN_U_GATE_SEL		4
#define PIN_OUTPUT_LOCK		6
#define PIN_IH_RANGE		7
#define PIN_SB_IM_CS		8
#define PIN_SB_TSP_CS		9
#define PIN_DCB_LDAC		10
#define PIN_MCB_LDAC		11
#define PIN_SB_TK1_CS		12
#define PIN_WATER			15
#define PIN_T_TR1			16
#define PIN_PS_CTRL			19
#define PIN_LED1			21
#define PIN_MCB_DCB_CS		23
#define PIN_SB_TCOOL2_CS	24
#define PIN_SB_IH_CS		25
#define PIN_SB_TCOOL1_CS	26
#define PIN_SB_TK2_CS		27
#define PIN_EEPROM_CS		32
#define PIN_SYNC_OSC		33
#define PIN_REG_RCLK		34


//
#define AIN_U_BAT			0x00	// INA0

#endif // __BOARD_CONFIG_H
