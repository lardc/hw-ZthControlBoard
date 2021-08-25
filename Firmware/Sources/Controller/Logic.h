// -----------------------------------------
// Main logic library
// ----------------------------------------

#ifndef __LOGIC_H
#define __LOGIC_H

// Include
//
#include "stdinc.h"

// Definitions
//
#define dT_GRADUATION				5
#define GRAD_ARRAY_LENGTH			200

//Struct
//
struct CONFIG_LongTimeTimer
{
	void (*TimerSet)(Int32U);
	Int32U IhcTimeCounter;
	Int32U IhcTimeSubtrack;
};

extern struct CONFIG_LongTimeTimer CONFIG_LongTimeTimer1;
extern struct CONFIG_LongTimeTimer CONFIG_LongTimeTimer2;
//
extern Int16U GraduationArray[GRAD_ARRAY_LENGTH];

// Functions
void ZthModeMultiPulseProcess(void);
void ZthModeSinglePulseProcess(void);
void ZthModeGraduationProcess(void);
void ZthModeManualProcess(void);
void CONFIG_DUTControlBoard(void);
void CONFIG_MCurrentBoard(void);
void CONFIG_IhcPulse(void);
void CONFIG_Timer(struct CONFIG_LongTimeTimer* LongTimeTimer, Int32U Time);
void LongTimeTime_CheckValue(struct CONFIG_LongTimeTimer* LongTimeTimer);




#endif // __LOGIC_H
