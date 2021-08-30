#ifndef ZTHSENSINGBOARD_H_
#define ZTHSENSINGBOARD_H_

// Include
//
#include "stdinc.h"
#include "ZwDSP.h"
#include "IQmathLib.h"

// Functions
//
_iq ZthSB_ReadTcase1();
_iq ZthSB_ReadTcase2();
_iq ZthSB_ReadTcool1();
_iq ZthSB_ReadTcool2();
_iq ZthSB_ReadTSP();
_iq ZthSB_ReadIh();
_iq ZthSB_ReadIm();

#endif /* ZTHSENSINGBOARD_H_ */
