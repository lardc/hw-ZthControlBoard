#ifndef ZTHPROTECTIONBOARD_H_
#define ZTHPROTECTIONBOARD_H_

// Include
//
#include "stdinc.h"

// Types
typedef enum
{
	PB_FAULT_NONE,
	PB_FAULT_WATER,
	PB_FAULT_TR1,
	PB_FAULT_TR2,
	PB_FAULT_REC
}PB_Fault;

// Functions prototypes
//
PB_Fault ZthPB_FaultCheck();

#endif /* ZTHPROTECTIONBOARD_H_ */
