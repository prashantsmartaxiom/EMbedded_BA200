/**************************************************************************************************
File Name: adc_routine.c
External Data:
    Name                                    Source
    --------------------------------------- -----------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- -----------------------------------------------------

Description:  
    

===================================================================================================
History:
     Date       Author           Description of Change
     ---------- ---------------- -----------------------------------------------------------------
     08/04      Wilson Chen      1. Initial version

**************************************************************************************************/

/* Includes */
#include "adc_routine.h"

/* Local function prototypes */

/* Data definitions */
TaskHandle_t AdcConvTaskHandle = NULL;
Create_Timer(dlyProtection, 10);
CrtAdcVal(ADC_NTC1);
CrtAdcVal(ADC_NTC4);
CrtAdcVal(ADC_NTC5);
CrtAdcVal(ADC_NTC6);
CrtAdcVal(ADC_NTC7);
CrtAdcVal(ADC_Iin_sense);
CrtAdcVal(ADC_Vbus_sense);
// ERT-J1VV473J NTC R-C Table: Range[0 - 165] = [-40¢J - 125¢J] (166 point, 12bit data format)
const unsigned short NTC_Table[166] = 
{
    4089, 4088, 4088, 4087, 4086, 4086, 4085, 4084, 4083, 4082,
    4081, 4080, 4079, 4078, 4076, 4075, 4074, 4072, 4070, 4069,
    4067, 4065, 4062, 4060, 4058, 4055, 4053, 4050, 4047, 4043,
    4040, 4036, 4033, 4029, 4024, 4020, 4015, 4010, 4005, 4000,
    3994, 3988, 3981, 3974, 3967, 3960, 3952, 3944, 3935, 3926,
    3917, 3907, 3896, 3885, 3874, 3862, 3850, 3837, 3823, 3809,
    3794, 3779, 3763, 3746, 3729, 3711, 3693, 3674, 3654, 3633,
    3612, 3590, 3567, 3544, 3520, 3495, 3469, 3443, 3415, 3387,
    3359, 3329, 3299, 3268, 3237, 3204, 3171, 3137, 3103, 3068,
    3032, 2996, 2959, 2921, 2883, 2845, 2806, 2766, 2726, 2686,
    2645, 2604, 2563, 2521, 2480, 2438, 2396, 2354, 2312, 2270,
    2228, 2186, 2145, 2103, 2062, 2021, 1980, 1940, 1899, 1859,
    1820, 1781, 1742, 1703, 1665, 1628, 1591, 1554, 1518, 1482,
    1447, 1413, 1379, 1345, 1313, 1280, 1249, 1218, 1187, 1157,
    1128, 1100, 1072, 1044, 1018,  991,  966,  941,  916,  893,
    869,  846,  824,  803,  781,  761,  741,  721,  702,  683,
    665,  647,  630,  613,  597,  581,
};
/**************************************************************************************************
Function Name:
    short NTC_Lookup(short Data, unsigned short const *Ptr)
Input:
    Data     - Hexadecimal value of temperature.
    *Ptr     - Pointer of NTC table.
Output:
    Index    - Actual temperature without 40 degress offset.
Comment:
    NTC table lookup function by binary search algorithm.
**************************************************************************************************/
short NTC_Lookup(short Data, unsigned short const *Ptr)
{
    unsigned short Opr = 0,Bit = 1,Index = 0xFFFF;
    
    while(Index == 0xFFFF)
    {
        if ((NTC_Tab_Size >> Bit) > 0)
        {
            if (Data < Ptr[(NTC_Tab_Size >> Bit) + Opr]) Opr += (NTC_Tab_Size >> Bit);
        }
        else
        {
            if (Ptr[Opr] < Data)
            {
                if (Opr > 0)
                {
                    if ((Data - Ptr[Opr]) < (Ptr[Opr - 1] - Data))
                        Index = Opr;
                    else Index = Opr - 1;
                }
                else Index = 0;	
            }
            else if (Opr >= (NTC_Tab_Size - 1))
            {
                Index = NTC_Tab_Size - 1;
            }
            else Opr++;
        }
        Bit++;
    }
    
    return Index;
}
/**************************************************************************************************
Function Name:
    void Data_Hex2Dec(AdcVal *Data,unsigned short Scale,unsigned char Accuracy) 
Inputs:

Outputs:
    None

Description:
    Hexadecimal to Decimal conversion function
	- Scale = Hardware value at 3.3V
	- Accuracy = x10 or x100
**************************************************************************************************/
void Data_Hex2Dec(AdcVal *Data, unsigned short Scale, unsigned char Accuracy) 
{
	Data -> DecVal = ((unsigned long)(*Data -> ValPtr) * Scale) >> 12;
	
	if (Accuracy == 10)
    {
		Data -> DecInt = (short)(((unsigned long)Data -> DecVal * 3277) >> 15);  	// x0.1
    }
	else if (Accuracy == 100)
    {
		Data -> DecInt = (short)(((unsigned long)Data -> DecVal * 5243) >> 19);		// x0.01
    }
}
/**************************************************************************************************
Function Name:
    void adc_protection(void)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void adc_protection(void)
{
    if (Call_Timer(dlyProtection))
    {
        if ((ADC_Vbus_sense.DecInt < INPUT_UVP) || (ADC_Vbus_sense.DecInt > INPUT_OVP) ||
            (ADC_Iin_sense.DecInt > INPUT_OCP)  ||
            (ADC_NTC1.DecInt > NTC_OTP) || (ADC_NTC4.DecInt > NTC_OTP) ||
            (ADC_NTC5.DecInt > NTC_OTP) || (ADC_NTC6.DecInt > NTC_OTP) ||
            (ADC_NTC7.DecInt > NTC_OTP))
        {
            ALL_DEVICE_DISABLE;
        }
    }
}
/**************************************************************************************************
Function Name:
    void adc_conv(void *arg)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void adc_conv(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);
    
    const static uint8_t adc1_seq[5] = {  2,  3,  8,  9, 15},\
                         adc2_seq[6] = {  0,  1, 11, 12, 16, 17},\
                         adc3_seq[6] = {  4,  6,  8,  9, 11, 14},\
                         adc4_seq[6] = {  0,  2,  3,  4, 10, 16};
    static uint8_t adc1_seq_cnt = 0, adc234_seq_cnt = 0;
    
    while (1)
    {
        // ADC 1 trigger & get data 
        _ADC1_RSEQ1.bit.SEQ1 = adc1_seq[adc1_seq_cnt];
        _ADC1_CTRL2.bit.SWSTRRCH = 1;
        while (!_ADC1_STS.bit.ENDC);
        _ADC1_STS.bit.ENDC = 1;
        _ADC1_STS.bit.STR = 1;
        adc1RawData[adc1_seq_cnt] = _ADC1_DAT.all & 0x0FFF;
        
        adc1_seq_cnt = (adc1_seq_cnt + 1) % 5;
        
        // ADC 2 trigger & get data 
        _ADC2_RSEQ1.bit.SEQ1 = adc2_seq[adc234_seq_cnt];
        _ADC2_CTRL2.bit.SWSTRRCH = 1;
        while (!_ADC2_STS.bit.ENDC);
        _ADC2_STS.bit.ENDC = 1;
        _ADC2_STS.bit.STR = 1;
        adc2RawData[adc234_seq_cnt] = _ADC2_DAT.all & 0x0FFF;
        
        // ADC 3 trigger & get data 
        _ADC3_RSEQ1.bit.SEQ1 = adc3_seq[adc234_seq_cnt];
        _ADC3_CTRL2.bit.SWSTRRCH = 1;
        while (!_ADC3_STS.bit.ENDC);
        _ADC3_STS.bit.ENDC = 1;
        _ADC3_STS.bit.STR = 1;
        adc3RawData[adc234_seq_cnt] = _ADC3_DAT.all & 0x0FFF;
        
        // ADC 4 trigger & get data 
        _ADC4_RSEQ1.bit.SEQ1 = adc4_seq[adc234_seq_cnt];
        _ADC4_CTRL2.bit.SWSTRRCH = 1;
        while (!_ADC4_STS.bit.ENDC);
        _ADC4_STS.bit.ENDC = 1;
        _ADC4_STS.bit.STR = 1;
        adc4RawData[adc234_seq_cnt] = _ADC4_DAT.all & 0x0FFF;
        
        adc234_seq_cnt = (adc234_seq_cnt + 1) % 6;
        
        ADC_NTC1.HexVal = adc4RawData[0];
        ADC_NTC4.HexVal = adc1RawData[2];
        ADC_NTC5.HexVal = adc4RawData[4];
        ADC_NTC6.HexVal = adc3RawData[4];
        ADC_NTC7.HexVal = adc2RawData[1];
        ADC_Iin_sense.HexVal = adc1RawData[4];
        ADC_Vbus_sense.HexVal = adc2RawData[4];
        
        ADC_NTC1.DecInt = NTC_Lookup((short)ADC_NTC1.HexVal,&NTC_Table[0]) - 40;
        ADC_NTC4.DecInt = NTC_Lookup((short)ADC_NTC4.HexVal,&NTC_Table[0]) - 40;
        ADC_NTC5.DecInt = NTC_Lookup((short)ADC_NTC5.HexVal,&NTC_Table[0]) - 40;
        ADC_NTC6.DecInt = NTC_Lookup((short)ADC_NTC6.HexVal,&NTC_Table[0]) - 40;
        ADC_NTC7.DecInt = NTC_Lookup((short)ADC_NTC7.HexVal,&NTC_Table[0]) - 40;
        Data_Hex2Dec(&ADC_Iin_sense, IIN_SCALE, PRECISE_100);
        Data_Hex2Dec(&ADC_Vbus_sense, VBUS_SCALE, PRECISE_100);
        
//        adc_protection();
        
        /* Update data every 500 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
/**************************************************************************************************
Function Name:
    void adc_init(void)

Inputs:
    None

Outputs:
    None

Description:

**************************************************************************************************/
void adc_init(void)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);
    
    if (xTaskCreate(adc_conv,
                    "adc_conv",
                    ADC_CONV_TASK_STACK_SIZE,
                    NULL,
                    ADC_CONV_TASK_PRIORITY,
                    &AdcConvTaskHandle) != pdPASS)
    {
        printf("Task <<scan_device>> creation failed!\n");
    }
}
