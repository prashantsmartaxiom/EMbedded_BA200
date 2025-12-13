/***********************************************************************************************************************
File Name: gui_box_control.c
External Data:
    Name                                    Source
    --------------------------------------- ----------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ----------------------------------------------------------------------------

Description:
    PSE flow control.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ----------------------------------------------------------------------------------------
    01/21/2025  Fred Huang      Create file

***********************************************************************************************************************/

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "gui_box_control.h"
#include "i2c_device_driver.h"
//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
extern uint16_t refLimit[22];
extern u8 shadeId[4] ;
TaskHandle_t GuiRoutineTaskHandle = NULL;
u8 rs485PortNo = 0;
u8 mpq7210Test = 0, mpq7210BrightnessTest;
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    uint16_t Modbus_CRC16(uint8_t *buf, uint8_t len)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
uint16_t Modbus_CRC16(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;

    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= (uint16_t)buf[i];

        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}
/**************************************************************************************************
Function Name:
    int SendModbusRequest(UartObj *uart, uint8_t slaveAddr)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
int SendModbusRequest(UartObj *uart, DmaUartObj *dma, uint8_t slaveAddr)
{
    uint8_t frame[8], frame_len;
    uint16_t crc;

    frame[0] = slaveAddr;
    frame[1] = 0x03;
    frame[2] = 0x00;
    frame[3] = 0x00;
    frame[4] = 0x00;
    frame[5] = 0x01;

    crc = Modbus_CRC16(frame, 6);
    frame[6] = crc & 0xFF;
    frame[7] = (crc >> 8) & 0xFF;

    frame_len = 8;

    return uart->func.transmit(dma, frame, frame_len);
}
//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Gui_Box_Control_Routine(slaveCommPack cmdPack)
Input:
    None.
Output:
    None.
Comment:
    Run the GUI command and control the BOX devices.
**************************************************************************************************/
void Gui_Box_Control_Routine(slaveCommPack cmdPack)
{
    u8 ledPort, shadePort, shadeIdBuffer;
    LedStatusRegister ledStatusBuffer;
    LedControlRegister ledCtrlBuffer;
    ShadeControlRegister shadeCtrlBuffer;
    void *dummy_buf = NULL;
    uint8_t dummy_size = 0;

    if (mpq7210Test)
    {
        if (mfrKey == MFR_MODE_ENTER_KEY && mfrMode == MFR_BURN_IN_MODE)
        {
            ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = mpq7210BrightnessTest;
            Mpq7210_Command_Pack_Enqueue(ANA_DIM1, mpq7210Test, DATA_WRITE, HIGH_PRIORITY);
        }
        else
        {
            Mpq7210_Command_Pack_Enqueue(DEV_CFG, mpq7210Test, DATA_READ, HIGH_PRIORITY);
        }
        mpq7210Test = 0;
    }

    if (mfrKey == MFR_MODE_ENTER_KEY && mfrMode == MFR_COMM_TEST_MODE)
    {
        switch (rs485PortNo)
        {
            case 0x1:
                if (usart1.func.receive(&usart1Dma1Rx, &dummy_buf, &dummy_size))
                {
                    mfrRs485Result = 0x50;
                    rs485PortNo = 0;
                }
                break;
            case 0x2:
                if (usart2.func.receive(&usart2Dma1Rx, &dummy_buf, &dummy_size))
                {
                    mfrRs485Result = 0x50;
                    rs485PortNo = 0;
                }
                break;
            case 0x3:
                if (usart3.func.receive(&usart3Dma1Rx, &dummy_buf, &dummy_size))
                {
                    mfrRs485Result = 0x50;
                    rs485PortNo = 0;
                }
                break;
            case 0x4:
                if (usart4.func.receive(&usart4Dma1Rx, &dummy_buf, &dummy_size))
                {
                    mfrRs485Result = 0x50;
                    rs485PortNo = 0;
                }
                break;
            case 0x5:
                if (uart5.func.receive(&uart5Dma1Rx, &dummy_buf, &dummy_size))
                {
                    mfrRs485Result = 0x50;
                    rs485PortNo = 0;
                }
                break;
            case 0x6:
                if (uart6.func.receive(&uart6Dma1Rx, &dummy_buf, &dummy_size))
                {
                    mfrRs485Result = 0x50;
                    rs485PortNo = 0;
                }
                break;
            case 0x7:
                if (uart7.func.receive(&uart7Dma1Rx, &dummy_buf, &dummy_size))
                {
                    mfrRs485Result = 0x50;
                    rs485PortNo = 0;
                }
                break;
            case 0x8:
                if (uart8.func.receive(&uart8Dma1Rx, &dummy_buf, &dummy_size))
                {
                    mfrRs485Result = 0x50;
                    rs485PortNo = 0;
                }
                break;
        }
    }

    if (cmdPack.direction == GUI_WRITE)
    {
        switch ((*(guiCommand.index + cmdPack.cmdCode))->dataType)
        {
            case GUI_LED:
                // Gete the LED port from cmdCode
                ledPort = (cmdPack.cmdCode >> 1) + 1;
                // Even cmdCode stands for status and odd cmdCode stands for control
                if (cmdPack.cmdCode & 0x1)
                {
                    ledCtrlBuffer = *((LedControlRegister *)(*(guiCommand.index + cmdPack.cmdCode))->dataPtr);
                    if (ledCtrlBuffer.bit.BRIGHTNESS * 10 < refLimit[ledPort])
                    {
                        ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all =
                            ledCtrlBuffer.bit.BRIGHTNESS * 10;
                    }
                    else
                    {
                        ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all =
                            refLimit[ledPort];
                    }
//                    Mpq7210_Command_Pack_Enqueue(ANA_DIM1, ledPort, DATA_WRITE, LOW_PRIORITY);
                }
                else
                {
                    ledStatusBuffer = *((LedStatusRegister *)(*(guiCommand.index + cmdPack.cmdCode))->dataPtr);
                    if (ledStatusBuffer.bit.LED_STATUS == LED_ON)
                    {
                      ((PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr)->all = 0xFFF;
                        Mpq7210_Command_Pack_Enqueue(PWM_DIM1, ledPort, DATA_WRITE, LOW_PRIORITY);
                        ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = 10;
                        Mpq7210_Command_Pack_Enqueue(ANA_DIM1, ledPort, DATA_WRITE, LOW_PRIORITY);
                    }
                    else
                    {
                       ((PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr)->all = 0x0;
                        Mpq7210_Command_Pack_Enqueue(PWM_DIM1, ledPort, DATA_WRITE, LOW_PRIORITY);
                        ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = 10;
                        Mpq7210_Command_Pack_Enqueue(ANA_DIM1, ledPort, DATA_WRITE, LOW_PRIORITY);
                    }
                    refLimit[ledPort] = ledStatusBuffer.bit.IOUT_LIMIT * 10;
                 }
                break;
            case GUI_SHADE:
                // Gete the shade port from cmdCode
                shadePort = ((cmdPack.cmdCode - 0x80) >> 1);
                // Even cmdCode stands for ID and odd cmdCode stands for control
                if (cmdPack.cmdCode & 0x1)
                {
                    shadeCtrlBuffer = *((ShadeControlRegister *)(*(guiCommand.index + cmdPack.cmdCode))->dataPtr);
                    shadeIdBuffer = *((u8*)(*(guiCommand.index + cmdPack.cmdCode - 1))->dataPtr);
                    if (shadeIdBuffer)
                    {
                        // shade_action = ((shadeIdBuffer - 1) * 3) + shadeCtrlBuffer.bit.SHADE_CONTROL + 1;
                        for (int i = 0; i < UART_COUNT ; i ++)
                        {
                            if ((shadeCtrlBuffer.bit.SHADE_CONTROL + 1) == 1)
                            {
                                shadeProtocol.func.packTxData(&shadeProtocol, shadeIdBuffer, SHADE_UP, HIGH_PRIORITY);
                            }
                            else if ((shadeCtrlBuffer.bit.SHADE_CONTROL + 1) == 2)
                            {
                                shadeProtocol.func.packTxData(&shadeProtocol, shadeIdBuffer, SHADE_STOP, HIGH_PRIORITY);
                            }
                            else if ((shadeCtrlBuffer.bit.SHADE_CONTROL + 1) == 3)
                            {
                                shadeProtocol.func.packTxData(&shadeProtocol, shadeIdBuffer, SHADE_DOWN, HIGH_PRIORITY);
                            }
                            shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[i].dma_tx, polling_ctx[i].uart->func.transmit);
                        }
                    }
                    else
                    {
//                        shade_action = (shadeIdBuffer * 3) + shadeCtrlBuffer.bit.SHADE_CONTROL + 1;
                    }
                }
                else
                {
                    shadeIdBuffer = *((u8*)(*(guiCommand.index + cmdPack.cmdCode))->dataPtr);
                    if (shadeIdBuffer != shadeId[shadePort])
                    {
                        shadeId[shadePort] = shadeIdBuffer;
                        for (int i = 0; i < UART_COUNT ; i ++)
                        {
                            shadeProtocol.func.packTxData(&shadeProtocol, shadeIdBuffer, SHADE_ID_SELECT, HIGH_PRIORITY);
                            shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[i].dma_tx, polling_ctx[i].uart->func.transmit);
                        }
                    }
                }
                break;
            case GUI_PIR:
                break;
            case GUI_NOT_PACKET:
                break;
            case MANUFACTURE:
                if (mfrKey == MFR_MODE_ENTER_KEY)
                {
                    switch (mfrMode)
                    {
                        case MFR_BURN_IN_MODE:
                            if (mfrLedResult == 0x31)
                            {
                                ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = 200;
                                Mpq7210_Command_Pack_Enqueue(ANA_DIM1, SPI_PORT_0, DATA_WRITE, HIGH_PRIORITY);
                                ((PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr)->all = 0xFFF;
                                Mpq7210_Command_Pack_Enqueue(PWM_DIM1, SPI_PORT_0, DATA_WRITE, HIGH_PRIORITY);
                            }
                            else
                            {
                                mfrLedResult = 0;
                                ((PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr)->all = 0x0;
                                Mpq7210_Command_Pack_Enqueue(PWM_DIM1, SPI_PORT_0, DATA_WRITE, HIGH_PRIORITY);
                                ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = 10;
                                Mpq7210_Command_Pack_Enqueue(ANA_DIM1, SPI_PORT_0, DATA_WRITE, HIGH_PRIORITY);
//                                Mpq7210_Command_Pack_Enqueue(PWM_DIM1, SPI_PORT_1, DATA_READ, HIGH_PRIORITY);
//                                Mpq7210_Command_Pack_Enqueue(ANA_DIM1, SPI_PORT_1, DATA_READ, HIGH_PRIORITY);
                            }
                            break;
                        case MFR_COMM_TEST_MODE:
                            if ((mfrRs485Result >> 4) == 0x4)
                            {
                                rs485PortNo = mfrRs485Result & 0xF;
                                switch (rs485PortNo)
                                {
                                    case 0x1:
                                        SendModbusRequest(&usart1, &usart1Dma2Tx, 0x01);
                                        break;
                                    case 0x2:
                                        SendModbusRequest(&usart2, &usart2Dma2Tx, 0x01);
                                        break;
                                    case 0x3:
                                        SendModbusRequest(&usart3, &usart3Dma2Tx, 0x01);
                                        break;
                                    case 0x4:
                                        SendModbusRequest(&usart4, &usart4Dma2Tx, 0x01);
                                        break;
                                    case 0x5:
                                        SendModbusRequest(&uart5, &uart5Dma2Tx, 0x01);
                                        break;
                                    case 0x6:
                                        SendModbusRequest(&uart6, &uart6Dma2Tx, 0x01);
                                        break;
                                    case 0x7:
                                        SendModbusRequest(&uart7, &uart7Dma2Tx, 0x01);
                                        break;
                                    case 0x8:
                                        SendModbusRequest(&uart8, &uart8Dma2Tx, 0x01);
                                        break;
                                }
                            }
                            break;
                        case MFR_IDLE_MODE:
                            // Turn off all LED port
                            if (mfrLedResult == 0x31)
                            {
                                mfrLedResult = 0;
                                ((PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr)->all = 0x0;
                                Mpq7210_Command_Pack_Enqueue(PWM_DIM1, SPI_PORT_0, DATA_WRITE, HIGH_PRIORITY);
                                ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = 10;
                                Mpq7210_Command_Pack_Enqueue(ANA_DIM1, SPI_PORT_0, DATA_WRITE, HIGH_PRIORITY);
                            }
                            break;
                    }
                }
                else
                {
                    mfrKey = 0;

                }
                break;
        }
    }
    else
    {
        switch ((*(guiCommand.index + cmdPack.cmdCode))->dataType)
        {
            case GUI_PIR:
                // If sense PIR
//                if (*(*(guiCommand.index + PIR1_STATUS))->dataPtr && ledStatusBuffer.bit.LED_STATUS != LED_ON)
//                {
//                    ledStatusBuffer = *((LedStatusRegister *)(*(guiCommand.index + cmdPack.cmdCode))->dataPtr);
//                    ledStatusBuffer.bit.LED_STATUS = LED_ON;
//                    ((PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr)->all = 0xFFF;
//                    Mpq7210_Command_Pack_Enqueue(PWM_DIM1, 0x1, DATA_WRITE, LOW_PRIORITY);
//                    ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = 10;
//                    Mpq7210_Command_Pack_Enqueue(ANA_DIM1, 0x1, DATA_WRITE, LOW_PRIORITY);
//                    scenarioTick = 25;
//                }
//                else if (scenarioTick < 0)
//                {
//                    ledStatusBuffer = *((LedStatusRegister *)(*(guiCommand.index + cmdPack.cmdCode))->dataPtr);
//                    ledStatusBuffer.bit.LED_STATUS = LED_OFF;
//                    ((PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr)->all = 0x0;
//                    Mpq7210_Command_Pack_Enqueue(PWM_DIM1, 0x1, DATA_WRITE, LOW_PRIORITY);
//                    ((AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr)->all = 10;
//                    Mpq7210_Command_Pack_Enqueue(ANA_DIM1, 0x1, DATA_WRITE, LOW_PRIORITY);
//                    scenarioTick = 0;
//                }
                break;
        }
    }
}

/**************************************************************************************************
Function Name:
    void uart_de_scan(void *arg)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void gui_routine_process(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    while (1)
    {
//        Gui_Box_Control_Routine(Gui_Communication_Process(&slaveI2c1));

        MpXXXX_Communication_Process(&masterI2c2, &masterI2c3);

//        IO_Expander_Communication_Process(&masterI2c4);

        /* Update data every 100 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**************************************************************************************************
Function Name:
    void gui_init(void)

Inputs:
    None

Outputs:
    None

Description:

**************************************************************************************************/
void gui_init(void)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    if (xTaskCreate(gui_routine_process,
                    "gui_routine_process",
                    GUI_ROUTINE_TASK_STACK_SIZE,
                    NULL,
                    GUI_ROUTINE_TASK_PRIORITY,
                    &GuiRoutineTaskHandle) != pdPASS)
    {
        printf("Task <<scan_device>> creation failed!\n");
    }
}
//----------------------------------------------------------------------------------------------------------------------
