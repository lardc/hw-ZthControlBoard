// -----------------------------------------
// Measuring processes library
// ----------------------------------------

// Header
#include "MeasuringProcesses.h"
//
// Includes
#include "ZbGPIO.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Struct
//
struct SearchInGradTable
{
	Int16U TSP1;
	Int16U TSP2;
	Int16U T1;
	Int16U T2;
	Int16U TSP_BeforePulse;
	Int16U TSP_AfterPulse;
	Int16U Tj_BeforePulse;
	Int16U Tj_AfterPulse;
};

struct SearchInGradTable SearchingGradData;

// Variables
//
Int16U ZthCounter;

// Forward functions
//
void CONTROL_MeasuredCurrent(Boolean Set);
void SearchInGradTable(struct SearchInGradTable* GradData);
void Calc_Zth(void);

// Functions
//

void MeasureBeforeAndAfterPulse(Boolean AfterPulseFlag)
{
	Int16U Tdut_Celsius;
	Int16U Im_Current;
	Int16U TSP_Voltage;

	// Start measured current
	CONTROL_MeasuredCurrent(TRUE);
	DELAY_US(10);

	Measure_TSP_Start();

	Tdut_Celsius = Measure_Im_Ih_Tdut(&ZbGPIO_T_DUT_CS);

	Im_Current = Measure_Im_Ih_Tdut(&ZbGPIO_IMC_CS);

	TSP_Voltage = Measure_TSP_Finish();

	// Transforming and enter correction ADC data to degree of Celsius
	Tdut_Celsius = Tdut_Celsius*DataTable[REG_K_ADC_TO_CELSIUS];
	Tdut_Celsius = Tdut_Celsius*DataTable[REG_T_CORR_A] + DataTable[REG_T_CORR_B];

	// Transforming and enter correction ADC data to Amphere
	Im_Current = Im_Current*DataTable[REG_K_ADC_TO_CELSIUS];
	DataTable[REG_M_CURRENT_REAL] = Im_Current*DataTable[REG_T_CORR_A] + DataTable[REG_T_CORR_B];

	// Transforming and enter correction ADC data to Volts
	TSP_Voltage = TSP_Voltage*DataTable[REG_K_ADC_TO_CELSIUS];
	TSP_Voltage = TSP_Voltage*DataTable[REG_T_CORR_A] + DataTable[REG_T_CORR_B];

	if(AfterPulseFlag)
	{
		DataTable[REG_T_DUT_AFTER_PULSE] = Tdut_Celsius;
		DataTable[REG_TSP_AFTER_PULSE] = TSP_Voltage;

		Calc_Zth();
	}
	else
	{
		DataTable[REG_T_DUT_BEFORE_PULSE] = Tdut_Celsius;
		DataTable[REG_TSP_BEFORE_PULSE] = TSP_Voltage;
	}

	// Stop measured current
	CONTROL_MeasuredCurrent(FALSE);
}
// ----------------------------------------

void Calc_Zth(void)
{
	Int16U dTj=0, dTc=0, Zth=0, Tc;

	//Searching Tc
	SearchingGradData.TSP_AfterPulse = DataTable[REG_TSP_AFTER_PULSE];
	SearchingGradData.TSP_BeforePulse = DataTable[REG_TSP_BEFORE_PULSE];
	SearchingGradData.Tj_AfterPulse = DataTable[REG_T_DUT_AFTER_PULSE];
	SearchingGradData.Tj_BeforePulse = DataTable[REG_T_DUT_BEFORE_PULSE];
	SearchInGradTable(&SearchingGradData);

	Tc = (SearchingGradData.TSP1-SearchingGradData.TSP_AfterPulse)*(SearchingGradData.T1-SearchingGradData.T2)/
			(SearchingGradData.TSP1-SearchingGradData.TSP2)+SearchingGradData.T1;

	dTj = SearchingGradData.Tj_AfterPulse - SearchingGradData.Tj_BeforePulse;
	dTc = Tc - SearchingGradData.Tj_BeforePulse;
	Zth = (dTj - dTc)/DataTable[REG_H_POWER];
	SaveZthData(Zth);
}
// ----------------------------------------

void SearchInGradTable(struct SearchInGradTable* GradData)
{
	int i;

	for(i=GRAD_ARRAY_LENGTH-1;i>0;i--)
	{
		if(GradData->TSP_AfterPulse<GraduationArray[i])
		{
			if(GradData->TSP_AfterPulse>GraduationArray[i-1])
			{
				GradData->TSP1 = GraduationArray[i];
				GradData->TSP2 = GraduationArray[i-1];
				GradData->T1 = i;
				GradData->T2 = i-1;
				break;
			}
		}
	}
}
// ----------------------------------------


void Measure_TSP_Start(void)
{
	Int16U Value = 0;

	ZwSPIa_BeginReceive((pInt16U)Value, sizeof(Value), SPI_RESOLUTION, STTNormal);
}
// ----------------------------------------

Int16U Measure_TSP_Finish(void)
{
	Int16U receivedValue = 0;

	ZwSPIb_EndReceive((pInt16U)&receivedValue, 1);

	return receivedValue;
}
// ----------------------------------------

Int16U Measure_IHC_Finish(void)
{
	Int16U receivedValue = 0;

	ZwSPIb_EndReceive((pInt16U)&receivedValue, 1);

	return receivedValue;
}
// ----------------------------------------

void CONTROL_MeasuredCurrent(Boolean Set)
{
	if(Set)
		ZbGPIO_ControlMCB(TRUE);
	else
		ZbGPIO_ControlMCB(FALSE);
}
// ----------------------------------------

Int16U Measure_Im_Ih_Tdut(void (*Source)(Boolean))
{
	Int16U Value = 0;
	Int16U receivedValue = 0;

	Source(0);

	ZwSPIb_BeginReceive((pInt16U)Value, sizeof(Value), SPI_RESOLUTION, STTNormal);
	while(ZwSPIb_GetWordsToReceive() < sizeof(Value))
		DELAY_US(1);
	ZwSPIb_EndReceive((pInt16U)&receivedValue, sizeof(Value));

	Source(1);


	return receivedValue;
}
// ----------------------------------------





