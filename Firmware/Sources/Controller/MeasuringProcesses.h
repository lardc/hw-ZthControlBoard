// -----------------------------------------
// Measurment processes library
// ----------------------------------------

#ifndef MEASURINGPROCESSES_H_
#define MEASURINGPROCESSES_H_

//Include
#include <stdinc.h>
#include "SysConfig.h"
#include "Logic.h"
#include "Controller.h"

// Functions
//
void MeasureBeforeAndAfterPulse(Boolean AfterPulseFlag);
void Measure_TSP_Start(void);
Int16U Measure_TSP_Finish(void);
Int16U Measure_Im_Ih_Tdut(void (*Source)(Boolean));


#endif /* MEASURINGPROCESSES_H_ */
