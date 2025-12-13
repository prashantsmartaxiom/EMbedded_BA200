/***********************************************************************************************************************
File Name: mpq7210_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    MPQ7210 control driver.
    The driver shall create and fill in the required data in the driver's parameters before executing any function.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/07/2025  Fred Huang      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "mpq7210_driver.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
Mpq7210RegisterMap mpq7210Register;
uint16_t mpq7210RegisterVout[22] = {0};
Mpq7210TxBuffer mpq7210TxBuffer[MPQ7210_Queue_Length];
u16 txBufferIndex = 0;
uint8_t max_led_pwm_duty = 50;
uint8_t brightness = 100;
uint8_t color_temp = 50;   // 0-100 (3000K-6000K)
uint8_t pwma = 0, pwmb = 0;

Create_MPQ7210_Priority_Queue(mpq7210PriorityQueue, MPQ7210_Queue_Length);
Create_MPQ7210_Master_Command(mpq7210Command, 19, 256) =
{
    //                    Code,           Command Type,  Data Type,                                DataPointer,  DataSize
    {                  SIL_REV,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,           (u8*)&mpq7210Register.siliconRev, 2}, /*list 0*/
    {                  DEV_CFG,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,         (u8*)&mpq7210Register.deviceConfig, 2}, /*list 1*/
    {                 PWM_DIM1,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,          (u8*)&mpq7210Register.pwmDimming1, 2}, /*list 2*/
    {                 PWM_DIM2,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,          (u8*)&mpq7210Register.pwmDimming2, 2}, /*list 3*/
    {                 ANA_DIM1,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,       (u8*)&mpq7210Register.analogDimming1, 2}, /*list 4*/
    {                 ANA_DIM2,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,       (u8*)&mpq7210Register.analogDimming2, 2}, /*list 5*/
    {                  INT_ALL,      MPQ7210_READ_ONLY, MPQ7210_NOT_PACKET,      (u8*)&mpq7210Register.intteruptStatus, 2}, /*list 6*/
    {                  INT_CFG,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,      (u8*)&mpq7210Register.interruptConfig, 2}, /*list 7*/
    {            FS_INT_STATUS,    MPQ7210_WRITE_CLEAR, MPQ7210_NOT_PACKET,          (u8*)&mpq7210Register.faultStatus, 2}, /*list 8*/
    {        FS_INT_RAW_STATUS,      MPQ7210_READ_ONLY, MPQ7210_NOT_PACKET,       (u8*)&mpq7210Register.faultRawStatus, 2}, /*list 9*/
    {                FS_INT_EN,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,          (u8*)&mpq7210Register.faultEnable, 2}, /*list 10*/
    {           FS_INT_MASK_EN,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,            (u8*)&mpq7210Register.faultMask, 2}, /*list 11*/
    {  FS_INT_REAL_TIME_STATUS,      MPQ7210_READ_ONLY, MPQ7210_NOT_PACKET,  (u8*)&mpq7210Register.faultRealTimeStatus, 2}, /*list 12*/
    {                BUCK1_CFG,      MPQ7210_READ_ONLY, MPQ7210_NOT_PACKET,          (u8*)&mpq7210Register.buck1Config, 2}, /*list 13*/
    {                BUCK2_CFG,      MPQ7210_READ_ONLY, MPQ7210_NOT_PACKET,          (u8*)&mpq7210Register.buck2Config, 2}, /*list 14*/
    {                 ADC_CTRL,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,           (u8*)&mpq7210Register.adcControl, 2}, /*list 15*/
    {                  ADC_OUT,      MPQ7210_READ_ONLY, MPQ7210_NOT_PACKET,              (u8*)&mpq7210Register.adcData, 2}, /*list 16*/
    {               OTP_STATUS,      MPQ7210_READ_ONLY, MPQ7210_NOT_PACKET,            (u8*)&mpq7210Register.otpStatus, 2}, /*list 17*/
    {                  BUCK_EN,     MPQ7210_READ_WRITE, MPQ7210_NOT_PACKET,           (u8*)&mpq7210Register.buckEnable, 2}, /*list 18*/
};
TaskHandle_t Mpq7210RoutineTaskHandle = NULL;
TaskHandle_t LedDimmingTaskHandle = NULL;
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void SPI_Port_to_CS_Enable(u8 port)
Input:
    port - Device port for SPI CS pin
Output:
    None.
Comment:
    This function will take the input port argument to enable the corresponding IO pin.
**************************************************************************************************/
void SPI_Port_to_CS_Enable(u8 port)
{
    switch ((SpiPortNum)port)
    {
    default:
        break;
    case SPI_PORT_0:
        Chip_Select(1);
        Chip_Select(2);
        Chip_Select(3);
        Chip_Select(4);
        Chip_Select(5);
        Chip_Select(6);
        Chip_Select(7);
        Chip_Select(8);
        Chip_Select(9);
        Chip_Select(10);
        Chip_Select(11);
        Chip_Select(12);
        Chip_Select(13);
        Chip_Select(14);
        Chip_Select(15);
        Chip_Select(16);
        Chip_Select(17);
        Chip_Select(18);
        Chip_Select(19);
        Chip_Select(20);
        Chip_Select(21);
        Chip_Select(22);
        break;
    case SPI_PORT_1:
        Chip_Select(1);
        break;
    case SPI_PORT_2:
        Chip_Select(2);
        break;
    case SPI_PORT_3:
        Chip_Select(3);
        break;
    case SPI_PORT_4:
        Chip_Select(4);
        break;
    case SPI_PORT_5:
        Chip_Select(5);
        break;
    case SPI_PORT_6:
        Chip_Select(6);
        break;
    case SPI_PORT_7:
        Chip_Select(7);
        break;
    case SPI_PORT_8:
        Chip_Select(8);
        break;
    case SPI_PORT_9:
        Chip_Select(9);
        break;
    case SPI_PORT_10:
        Chip_Select(10);
        break;
    case SPI_PORT_11:
        Chip_Select(11);
        break;
    case SPI_PORT_12:
        Chip_Select(12);
        break;
    case SPI_PORT_13:
        Chip_Select(13);
        break;
    case SPI_PORT_14:
        Chip_Select(14);
        break;
    case SPI_PORT_15:
        Chip_Select(15);
        break;
    case SPI_PORT_16:
        Chip_Select(16);
        break;
    case SPI_PORT_17:
        Chip_Select(17);
        break;
    case SPI_PORT_18:
        Chip_Select(18);
        break;
    case SPI_PORT_19:
        Chip_Select(19);
        break;
    case SPI_PORT_20:
        Chip_Select(20);
        break;
    case SPI_PORT_21:
        Chip_Select(21);
        break;
    case SPI_PORT_22:
        Chip_Select(22);
        break;
    }
};
/**************************************************************************************************
Function Name:
    void SPI_Port_to_CS_Disable(u8 port)
Input:
    port - Device port for SPI CS pin
Output:
    None.
Comment:
    This function will take the input port argument to disable the corresponding IO pin.
**************************************************************************************************/
void SPI_Port_to_CS_Disable(u8 port)
{
    switch ((SpiPortNum)port)
    {
    default:
        break;
    case SPI_PORT_0:
        Disable_Chip_Select(1);
        Disable_Chip_Select(2);
        Disable_Chip_Select(3);
        Disable_Chip_Select(4);
        Disable_Chip_Select(5);
        Disable_Chip_Select(6);
        Disable_Chip_Select(7);
        Disable_Chip_Select(8);
        Disable_Chip_Select(9);
        Disable_Chip_Select(10);
        Disable_Chip_Select(11);
        Disable_Chip_Select(12);
        Disable_Chip_Select(13);
        Disable_Chip_Select(14);
        Disable_Chip_Select(15);
        Disable_Chip_Select(16);
        Disable_Chip_Select(17);
        Disable_Chip_Select(18);
        Disable_Chip_Select(19);
        Disable_Chip_Select(20);
        Disable_Chip_Select(21);
        Disable_Chip_Select(22);
        break;
    case SPI_PORT_1:
        Disable_Chip_Select(1);
        break;
    case SPI_PORT_2:
        Disable_Chip_Select(2);
        break;
    case SPI_PORT_3:
        Disable_Chip_Select(3);
        break;
    case SPI_PORT_4:
        Disable_Chip_Select(4);
        break;
    case SPI_PORT_5:
        Disable_Chip_Select(5);
        break;
    case SPI_PORT_6:
        Disable_Chip_Select(6);
        break;
    case SPI_PORT_7:
        Disable_Chip_Select(7);
        break;
    case SPI_PORT_8:
        Disable_Chip_Select(8);
        break;
    case SPI_PORT_9:
        Disable_Chip_Select(9);
        break;
    case SPI_PORT_10:
        Disable_Chip_Select(10);
        break;
    case SPI_PORT_11:
        Disable_Chip_Select(11);
        break;
    case SPI_PORT_12:
        Disable_Chip_Select(12);
        break;
    case SPI_PORT_13:
        Disable_Chip_Select(13);
        break;
    case SPI_PORT_14:
        Disable_Chip_Select(14);
        break;
    case SPI_PORT_15:
        Disable_Chip_Select(15);
        break;
    case SPI_PORT_16:
        Disable_Chip_Select(16);
        break;
    case SPI_PORT_17:
        Disable_Chip_Select(17);
        break;
    case SPI_PORT_18:
        Disable_Chip_Select(18);
        break;
    case SPI_PORT_19:
        Disable_Chip_Select(19);
        break;
    case SPI_PORT_20:
        Disable_Chip_Select(20);
        break;
    case SPI_PORT_21:
        Disable_Chip_Select(21);
        break;
    case SPI_PORT_22:
        Disable_Chip_Select(22);
        break;
    }
};

//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Mpq7210_Command_List_Setup(Mpq7210CommandArray *command)
Input:
    *command - Pointer of MPQ7210 command list
Output:
    None.
Comment:
    This function will initialize master MPQ7210 command list and command address.
**************************************************************************************************/
void Mpq7210_Command_List_Setup(Mpq7210CommandArray *command)
{
    u16 i;
    QueueDataStr dequeueData;
    // Fill all indexes with 0
    for (i = 0; i < command->indexLength; i++)
    {
        command->index[i] = 0;
    }
    // Point the corresponding command code in the index array to the list array
    for (i = 0; i < command->listLength; i++)
    {
        if (command->list[i].cmdCode < command->indexLength)
        {
            command->index[command->list[i].cmdCode] =
                (command->list[i].cmdType > 0) ? (Mpq7210CommandInfo*)&command->list[i] : 0;
        }
    }
    // Dummy dequeue for first pop out
    while (!mpq7210PriorityQueue.highPriority.empty || !mpq7210PriorityQueue.lowPriority.empty)
        Priority_Buffer_Dequeue(&mpq7210PriorityQueue, &dequeueData);
}
/**************************************************************************************************
Function Name:
    void Mpq7210_Command_Pack_Enqueue(Mpq7210MasterCommand cmdIndex, u8 devicePort, u8 direction, u8 priority)
Input:
    cmdIndex - Command index.
    devicePort - Slave device port, transform to the corresponding CS.
    direction - Operation type for perform wirte or read operation.
    priority - priority of queue.
Output:
    None.
Comment:
    This function will enqueue listed SPI command into fifo.
**************************************************************************************************/
void Mpq7210_Command_Pack_Enqueue(Mpq7210MasterCommand cmdIndex, u8 devicePort, u8 direction, u8 priority)
{
    Mpq7210TxBuffer *bufferTemp = (mpq7210TxBuffer + txBufferIndex);
    Mpq7210CommandInfo *cmdTemp =  *(mpq7210Command.index + cmdIndex);
    QueueDataStr enqueueData = {bufferTemp, cmdTemp->dataSize, direction};

    bufferTemp->devicePort = devicePort;
    bufferTemp->regCode = cmdIndex;
    bufferTemp->regData = *(u16 *)cmdTemp->dataPtr;
    txBufferIndex = (txBufferIndex + 1) & (MPQ7210_Queue_Length - 1);

    Priority_Buffer_Enqueue(&mpq7210PriorityQueue, enqueueData, priority);
}
/**************************************************************************************************
Function Name:
    void Mpq7210_Communication_Process(SpiMasterObj *obj, PriorQueueRegStr *queueRegPtr)
Input:
    *obj - Pointer of SPI master object.
    *queueRegPtr - Registers pointer of priority circular buffer.
Output:
    Received data's command index.
Comment:
    Slave I2C communication process.
**************************************************************************************************/
void Mpq7210_Communication_Process(SpiMasterObj *obj, PriorQueueRegStr *queueRegPtr)
{
    QueueDataStr dequeueData = {0, 0, 0};
    Mpq7210TxBuffer *txBuffer;
    QueueDirectionType queueDirection;
    u8 txData[16], *rxData, txSize, rxSize;
    // Dequeue command
    if (Priority_Buffer_Dequeue(queueRegPtr, &dequeueData) && dequeueData.ptr)
    {
        txBuffer = dequeueData.ptr;
        queueDirection = (QueueDirectionType)dequeueData.direction;
        // Chip select
        SPI_Port_to_CS_Enable(txBuffer->devicePort);
        switch (queueDirection)
        {
            case DATA_WRITE:
                if (txBuffer)
                {
                    switch (mpq7210Command.index[txBuffer->regCode]->cmdType)
                    {
                        case MPQ7210_READ_WRITE:
                        case MPQ7210_WRITE_ONLY:
                            txSize = dequeueData.size + 1;
                            *txData = (txBuffer->regCode << 1) + queueDirection;
                            for (u8 i = 1, j = 0; i < txSize; i++, j+=8)
                            {
                                *(txData + i) = txBuffer->regData >> j;
                            }
                            obj->func.transmit(obj, txData, txSize);
                            break;
                        default:
                            break;
                    }
                }
                break;
            case DATA_READ:
                if (txBuffer)
                {
                    switch (mpq7210Command.index[txBuffer->regCode]->cmdType)
                    {
                        case MPQ7210_READ_WRITE:
                        case MPQ7210_READ_ONLY:
                            txSize = dequeueData.size + 1;
                            rxSize = dequeueData.size;
                            *txData = (txBuffer->regCode << 1) + queueDirection;
                            for (u8 i = 1; i < txSize; i++)
                            {
                                *(txData + i) = 0;
                            }
                            obj->func.transmit(obj, txData, txSize);
                            rxData = mpq7210Command.index[txBuffer->regCode]->dataPtr;
                            obj->func.receive(obj, rxData, rxSize);
                            break;
                        default:
                            break;
                    }
                }
                break;
            default:
                break;
        }
        while (obj->member.reg.STS->bit.BUSY)
            __NOP();
        SPI_Port_to_CS_Disable(txBuffer->devicePort);
    }
}
/**************************************************************************************************
Function Name:
    void mpq7210_routine(void *arg)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void mpq7210_routine(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    static uint8_t state_trans = 0, port_num = 1;
    static uint16_t delay_cnt = 0;
    uint8_t read_cnt = 0;
    uint32_t temp_adc = 0;

    while (1)
    {
        Mpq7210_Communication_Process(&masterSpi1, &mpq7210PriorityQueue);

        if (delay_cnt++ > 50)
        {
            if (state_trans == 0)
            {
                ((AdcControlRegister *)(*(mpq7210Command.index + ADC_CTRL))->dataPtr)->all = 9;
                Mpq7210_Command_Pack_Enqueue(ADC_CTRL, port_num, DATA_WRITE, LOW_PRIORITY);
                state_trans = 1;
            }
            else if (state_trans == 1)
            {
                Mpq7210_Command_Pack_Enqueue(ADC_CTRL, port_num, DATA_READ, LOW_PRIORITY);

                if((mpq7210Register.adcControl.bit.CUR_STORED_CH == mpq7210Register.adcControl.bit.ADC_INMUX) &&
                   (mpq7210Register.adcControl.bit.ADC_READY == 1))
                {
                    read_cnt = 0;
                    state_trans = 2;
                }
                else
                {
                    if (read_cnt ++ > 5)
                    {
                        port_num = ((port_num % 22) + 1);
                        read_cnt = 0;
                        state_trans = 0;
                    }
                }
            }
            else if (state_trans == 2)
            {
                Mpq7210_Command_Pack_Enqueue(ADC_OUT, port_num, DATA_READ, LOW_PRIORITY);
                temp_adc = (uint32_t)(mpq7210Register.adcData.bit.ADC_DATA * 10 * 6425 / 100000); // Vout = 0.06425 * Direct value, 0.1V
                if (temp_adc > 65535)
                {
                    temp_adc = 65535;
                }
                mpq7210RegisterVout[port_num - 1] = (uint16_t)temp_adc;
                port_num = ((port_num % 22) + 1);
                state_trans = 0;
            }
            delay_cnt = 0;
        }

//        static uint8_t test_flag_start = 0, test_flag = 0;
//        static uint16_t enter_value = 0;
//
//        if (test_flag_start == 0)
//        {
//            ((PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr)->all = 0xFFF;
//            Mpq7210_Command_Pack_Enqueue(PWM_DIM1, 0, DATA_WRITE, LOW_PRIORITY);
//
//            ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = 200;
//            Mpq7210_Command_Pack_Enqueue(ANA_DIM1, 0, DATA_WRITE, LOW_PRIORITY);
//
//            test_flag_start = 1;
//        }
//
//        if (test_flag == 1)
//        {
//            ((AdcControlRegister *)(*(mpq7210Command.index + ADC_CTRL))->dataPtr)->all = enter_value;
//            Mpq7210_Command_Pack_Enqueue(ADC_CTRL, port_num, DATA_WRITE, LOW_PRIORITY);
//
//            test_flag = 0;
//        }
//        else if (test_flag == 2)
//        {
//            Mpq7210_Command_Pack_Enqueue(ADC_CTRL, port_num, DATA_READ, LOW_PRIORITY);
//            test_flag = 0;
//        }
//        else if (test_flag == 3)
//        {
//            Mpq7210_Command_Pack_Enqueue(ADC_OUT, port_num, DATA_READ, LOW_PRIORITY);
//            test_flag = 0;
//        }
//        else if (test_flag == 4)
//        {
//            ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = enter_value;
//            Mpq7210_Command_Pack_Enqueue(ANA_DIM1, port_num, DATA_WRITE, LOW_PRIORITY);
//
//            test_flag = 0;
//        }
//        else if (test_flag == 5)
//        {
//            ((BuckEnableRegister *)(*(mpq7210Command.index + BUCK_EN))->dataPtr)->all = enter_value;
//            Mpq7210_Command_Pack_Enqueue(BUCK_EN, port_num, DATA_WRITE, LOW_PRIORITY);
//
//            test_flag = 0;
//        }

        /* Update data every 10 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
/**************************************************************************************************
Function Name:
    void led_pwm_calc(uint8_t brightness, uint8_t color_temp, uint8_t *pwm1_val, uint8_t *pwm2_val)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void led_pwm_calc(uint8_t brightness, uint8_t color_temp, uint8_t *pwma_val, uint8_t *pwmb_val)
{
    uint8_t max_duty = 0, total = 0;
    uint8_t target_pwma, target_pwmb , soft_start_step = 2;
    static uint8_t current_pwma = 0;
    static uint8_t current_pwmb = 0;

    // Validate inputs
    if (brightness <= 0) brightness = 0;
    if (brightness > 100) brightness = 100;
    if (color_temp <= 0) color_temp = 0;
    if (color_temp > 100) color_temp = 100;

    // Calculate max duty based on brightness (scale 0~max_led_pwm_duty) with rounding
    max_duty = (brightness * max_led_pwm_duty + 50) / 100;

    // Calculate PWM1 and PWM2 with integer arithmetic and rounding
//    *pwma_val = (max_duty * (100 - color_temp) + 50) / 100;
//    *pwmb_val = (max_duty * color_temp + 50) / 100;
    target_pwma = (max_duty * (100 - color_temp) + 50) / 100;
    target_pwmb = (max_duty * color_temp + 50) / 100;

    // Ensure PWM1 + PWM2 equals max_duty to avoid minor discrepancies due to rounding
//    total = *pwma_val + *pwmb_val;
    total = target_pwma + target_pwmb;
//    if (total < max_duty)
//    {
//        // Add to the larger one
//        if (*pwma_val > *pwmb_val)
//        {
//            *pwma_val += max_duty - total;
//        }
//        else
//        {
//            *pwmb_val += max_duty - total;
//        }
//    }
//    else if (total > max_duty)
//    {
//        // Subtract from the larger one
//        if (*pwma_val > *pwmb_val)
//        {
//            *pwma_val -= total - max_duty;
//        }
//        else
//        {
//            *pwmb_val -= total - max_duty;
//        }
//    }
    if (total < max_duty)
    {
        if (target_pwma > target_pwmb)
        {
            target_pwma += max_duty - total;
        }
        else
        {
            target_pwmb += max_duty - total;
        }
    }
    else if (total > max_duty)
    {
        if (target_pwma > target_pwmb)
        {
            target_pwma -= total - max_duty;
        }
        else
        {
            target_pwmb -= total - max_duty;
        }
    }
    // --- Soft Start ---
    if (target_pwma > current_pwma)
    {
        current_pwma += soft_start_step;
        if (current_pwma > target_pwma)
        {
            current_pwma = target_pwma;
        }
    }
    else if (target_pwma < current_pwma)
    {
        if (current_pwma > target_pwma && (current_pwma - target_pwma) <= soft_start_step)
        {
             current_pwma = target_pwma;
        }
        else
        {
            current_pwma -= soft_start_step;
        }
    }
    if (target_pwmb > current_pwmb)
    {
        current_pwmb += soft_start_step;
        if (current_pwmb > target_pwmb)
        {
            current_pwmb = target_pwmb;
        }
    }
    else if (target_pwmb < current_pwmb)
    {
        if (current_pwmb > target_pwmb && (current_pwmb - target_pwmb) <= soft_start_step)
        {
             current_pwmb = target_pwmb;
        }
        else
        {
            current_pwmb -= soft_start_step;
        }
    }

    *pwma_val = current_pwma;
    *pwmb_val = current_pwmb;
}
/**************************************************************************************************
Function Name:
    void Led_dimming(void *arg)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void Led_dimming(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    while (1)
    {
        led_pwm_calc(brightness, color_temp, &pwma, &pwmb);

        // Exp_LED1(EXP_CARD_05)
        TIM_SetCmp2(ATIM2, (uint16_t)(((uint32_t)pwma * (ATIM_Period - 1)) / 100));
        TIM_SetCmp1(ATIM2, (uint16_t)(((uint32_t)pwmb * (ATIM_Period - 1)) / 100));

        // Exp_LED2(EXP_CARD_06)
        TIM_SetCmp4(ATIM2, (uint16_t)(((uint32_t)pwma * (ATIM_Period - 1)) / 100));
        TIM_SetCmp4(ATIM1, (uint16_t)(((uint32_t)pwmb * (ATIM_Period - 1)) / 100));

        // Exp_LED3(EXP_CARD_07)
        TIM_SetCmp3(ATIM1, (uint16_t)(((uint32_t)pwma * (ATIM_Period - 1)) / 100));
        TIM_SetCmp2(ATIM1, (uint16_t)(((uint32_t)pwmb * (ATIM_Period - 1)) / 100));

        // Exp_LED4(EXP_CARD_08)
        TIM_SetCmp1(ATIM1, (uint16_t)(((uint32_t)pwma * (ATIM_Period - 1)) / 100));
        TIM_SetCmp3(ATIM2, (uint16_t)(((uint32_t)pwmb * (ATIM_Period - 1)) / 100));

        // Exp_LED5(EXP_CARD_01)
        SHRTIM_TIM_SetCompare3(SHRTIM1, SHRTIM_TIMER_B, (SHRTIM_Period - 1) * pwma / 100);
        SHRTIM_TIM_SetCompare1(SHRTIM1, SHRTIM_TIMER_B, (SHRTIM_Period - 1) * pwmb / 100);

        // Exp_LED6(EXP_CARD_02)
        SHRTIM_TIM_SetCompare3(SHRTIM1, SHRTIM_TIMER_F, (SHRTIM_Period - 1) * pwma / 100);
        SHRTIM_TIM_SetCompare1(SHRTIM1, SHRTIM_TIMER_F, (SHRTIM_Period - 1) * pwmb / 100);

        // Exp_LED7(EXP_CARD_03)
        SHRTIM_TIM_SetCompare3(SHRTIM1, SHRTIM_TIMER_E, (SHRTIM_Period - 1) * pwma / 100);
        SHRTIM_TIM_SetCompare1(SHRTIM1, SHRTIM_TIMER_E, (SHRTIM_Period - 1) * pwmb / 100);

        // Exp_LED8(EXP_CARD_04)
        SHRTIM_TIM_SetCompare3(SHRTIM1, SHRTIM_TIMER_A, (SHRTIM_Period - 1) * pwma / 100);
        SHRTIM_TIM_SetCompare1(SHRTIM1, SHRTIM_TIMER_A, (SHRTIM_Period - 1) * pwmb / 100);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
/**************************************************************************************************
Function Name:
    void mpq7210_init(void)

Inputs:
    None

Outputs:
    None

Description:

**************************************************************************************************/
void mpq7210_init(void)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    if (xTaskCreate(mpq7210_routine,
                    "mpq7210_routine",
                    MPQ7210_ROUTINE_TASK_STACK_SIZE,
                    NULL,
                    MPQ7210_ROUTINE_TASK_PRIORITY,
                    &Mpq7210RoutineTaskHandle) != pdPASS)
    {
        printf("Task <<scan_device>> creation failed!\n");
    }

    if (xTaskCreate(Led_dimming,
                    "Led_dimming",
                    LED_DIMMING_TASK_STACK_SIZE,
                    NULL,
                    LED_DIMMING_TASK_PRIORITY,
                    &LedDimmingTaskHandle) != pdPASS)
    {
        printf("Task <<scan_device>> creation failed!\n");
    }
}
//----------------------------------------------------------------------------------------------------------------------
