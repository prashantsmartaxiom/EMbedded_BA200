/**************************************************************************************************
File Name: device_polling.c
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
     07/18      Wilson Chen      1. Initial version

**************************************************************************************************/

/* Includes */

#include "main.h"
#include "device_polling.h"
/* Local function prototypes */

/* Data definitions */
TaskHandle_t DeviceScanTaskHandle = NULL;
TaskHandle_t DevicePollingTaskHandle = NULL;
TaskHandle_t DeviceReceivingTaskHandle = NULL;
TaskHandle_t UartDeScanTaskHandle = NULL;
UART_DeviceInfo_t uart_devices[UART_COUNT];
UART_PollingContext_t polling_ctx[UART_COUNT] =
{
    {&usart1, &usart1Dma1Rx, &usart1Dma2Tx, SHADE_STATE, 0, SHADE_START_ADDR, 0, 0, 0, 0, SHADE_STATE, 0, 0},
    {&usart2, &usart2Dma1Rx, &usart2Dma2Tx, SHADE_STATE, 0, SHADE_START_ADDR, 0, 1, 0, 0, SHADE_STATE, 0, 0},
    {&usart3, &usart3Dma1Rx, &usart3Dma2Tx, SHADE_STATE, 0, SHADE_START_ADDR, 0, 2, 0, 0, SHADE_STATE, 0, 0},
    {&usart4, &usart4Dma1Rx, &usart4Dma2Tx, SHADE_STATE, 0, SHADE_START_ADDR, 0, 3, 0, 0, SHADE_STATE, 0, 0},
    {&uart5,  &uart5Dma1Rx,  &uart5Dma2Tx,  SHADE_STATE, 0, SHADE_START_ADDR, 0, 4, 0, 0, SHADE_STATE, 0, 0},
    {&uart6,  &uart6Dma1Rx,  &uart6Dma2Tx,  SHADE_STATE, 0, SHADE_START_ADDR, 0, 5, 0, 0, SHADE_STATE, 0, 0},
    {&uart7,  &uart7Dma1Rx,  &uart7Dma2Tx,  SHADE_STATE, 0, SHADE_START_ADDR, 0, 6, 0, 0, SHADE_STATE, 0, 0},
    {&uart8,  &uart8Dma1Rx,  &uart8Dma2Tx,  SHADE_STATE, 0, SHADE_START_ADDR, 0, 7, 0, 0, SHADE_STATE, 0, 0},
};
unsigned short shade_cmd = 0;
unsigned short scan_all_done = 0;

/**************************************************************************************************
Function Name: uint16_t GetUartIndex(uint16_t dev_id)
Inputs: enter device id 
Outputs: return connect uart id
Description: 

**************************************************************************************************/
uint16_t GetUartIndex(uint16_t dev_id)
{
  uint16_t uart_index = 0;
  
  //return uart_index according shade port on the BA200 board
  switch(dev_id) {
    case 0x01: uart_index = 0x02;  break;
    case 0x02: uart_index = 0x01;  break;
    case 0x03: uart_index = 0x00;  break;
    case 0x04: uart_index = 0x03;  break;
    default:  uart_index = 0x01;   break;
  }
  
//  taskENTER_CRITICAL();
//  for(int i = 0; i < UART_COUNT; i++) {
//      if( uart_devices[i].total_devices > 0 && uart_devices[i].group.shade_count > 0) {   
//        if(uart_devices[i].group.shade[idx].id == dev_id){
//           uart_index = i;          
//        }  
//      }
//     idx = (idx + 1) % MAX_DEVICE_NUM;
//  }
//  taskEXIT_CRITICAL();
//  
  return uart_index;
}


/**************************************************************************************************
Function Name:
    void uart_scan_step(UART_PollingContext_t *pctx)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void uart_scan_step(UART_PollingContext_t *pctx)
{
    if (pctx->scan_done)
    {
        return;
    }

    switch (pctx->scan_state)
    {
    case SHADE_STATE:
        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < SHADE_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->scan_index ++;
        }

        if (pctx->scan_index > SHADE_END_ADDR)
        {
            pctx->first_send_flag = 0;
            pctx->scan_index = HMI_START_ADDR;
            pctx->scan_state = HMI_STATE;
            break;
        }

        shadeProtocol.func.packTxData(&shadeProtocol, pctx->scan_index, SHADE_STATUS_INQUIRY, LOW_PRIORITY);
        shadeProtocol.func.transmitData(&shadeProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case HMI_STATE:
        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < HMI_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->scan_index ++;
        }

        if (pctx->scan_index > HMI_END_ADDR)
        {
            pctx->first_send_flag = 0;
            pctx->scan_index = METER_START_ADDR;
            pctx->scan_state = METER_STATE;
            break;
        }

        hmiProtocol.func.packTxData(&hmiProtocol, pctx->scan_index, READ_HOLDING_REGISTERS, REGISTER_UPDATE_STATUS, 1, LOW_PRIORITY);
        hmiProtocol.func.transmitData(&hmiProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case METER_STATE:
        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < METER_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->scan_index ++;
        }

        if (pctx->scan_index > METER_END_ADDR)
        {
            pctx->first_send_flag = 0;
            pctx->scan_index = AIR_START_ADDR;
            pctx->scan_state = AIR_STATE;
            break;
        }

        meterProtocol.func.packTxData(&meterProtocol, pctx->scan_index, READ_HOLDING_REGISTERS, METER_HUMIDITY, 1, LOW_PRIORITY);
        meterProtocol.func.transmitData(&meterProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case AIR_STATE:
        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < AIR_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->scan_index ++;
        }

        if (pctx->scan_index > AIR_END_ADDR)
        {
            pctx->first_send_flag = 0;
            pctx->scan_index = LIGHT_START_ADDR;
            pctx->scan_state = LIGHT_STATE;
            break;
        }

        airProtocol.func.packTxData(&airProtocol, pctx->scan_index, READ_HOLDING_REGISTERS, AIR_PM2P5, 1, LOW_PRIORITY);
        airProtocol.func.transmitData(&airProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case LIGHT_STATE:
        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < LIGHT_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->scan_index ++;
        }

        if (pctx->scan_index > LIGHT_END_ADDR)
        {
            pctx->first_send_flag = 0;
            pctx->scan_done = 1;
            break;
        }

        lightProtocol.func.packTxData(&lightProtocol, pctx->scan_index, READ_HOLDING_REGISTERS, AIR_PM2P5, 1, LOW_PRIORITY);
        lightProtocol.func.transmitData(&lightProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    }
}


/**************************************************************************************************
Function Name:
    void scan_device(void *arg)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void scan_device(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    uint8_t i = 0;

    while (1)
    {
        for (i = 0 ; i < UART_COUNT ; i ++)
        {
            uart_scan_step(&polling_ctx[i]);
        }

        if (polling_ctx[0].scan_done && polling_ctx[1].scan_done &&
            polling_ctx[2].scan_done && polling_ctx[3].scan_done &&
            polling_ctx[4].scan_done && polling_ctx[5].scan_done &&
            polling_ctx[6].scan_done && polling_ctx[7].scan_done)
        {
            scan_all_done = 1;

            if (xTaskCreate(device_polling,
                            "device_polling",
                            DEVICE_POLLING_TASK_STACK_SIZE,
                            NULL,
                            DEVICE_POLLING_TASK_PRIORITY,
                            &DevicePollingTaskHandle) != pdPASS)
            {
                printf("Task <<scan_device>> creation failed!\n");
            }

            vTaskDelete(DeviceScanTaskHandle);
        }

        /* Update data every 25 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(POLLING_TIME_INTERVAL));
    }
}
/**************************************************************************************************
Function Name:
    void check_next_state(UART_PollingContext_t *pctx, uint8_t now_state)
Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void check_next_state(UART_PollingContext_t *pctx, uint8_t now_state)
{
    switch (now_state)
    {
    case SHADE_STATE:
        if (uart_devices[pctx->uart_id].group.hmi_count != 0)
        {
            pctx->polling_state = HMI_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.light_count != 0)
        {
            pctx->polling_state = LIGHT_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.meter_count != 0)
        {
            pctx->polling_state = METER_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.air_count != 0)
        {
            pctx->polling_state = AIR_STATE;
        }
        break;
    case HMI_STATE:
        if (uart_devices[pctx->uart_id].group.light_count != 0)
        {
            pctx->polling_state = LIGHT_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.meter_count != 0)
        {
            pctx->polling_state = METER_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.air_count != 0)
        {
            pctx->polling_state = AIR_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.shade_count != 0)
        {
            pctx->polling_state = SHADE_STATE;
        }
        break;
    case LIGHT_STATE:
        if (uart_devices[pctx->uart_id].group.meter_count != 0)
        {
            pctx->polling_state = METER_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.air_count != 0)
        {
            pctx->polling_state = AIR_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.shade_count != 0)
        {
            pctx->polling_state = SHADE_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.hmi_count != 0)
        {
            pctx->polling_state = HMI_STATE;
        }
        break;
    case METER_STATE:
        if (uart_devices[pctx->uart_id].group.air_count != 0)
        {
            pctx->polling_state = AIR_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.shade_count != 0)
        {
            pctx->polling_state = SHADE_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.hmi_count != 0)
        {
            pctx->polling_state = HMI_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.light_count != 0)
        {
            pctx->polling_state = LIGHT_STATE;
        }
        break;
    case AIR_STATE:
        if (uart_devices[pctx->uart_id].group.shade_count != 0)
        {
            pctx->polling_state = SHADE_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.hmi_count != 0)
        {
            pctx->polling_state = HMI_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.light_count != 0)
        {
            pctx->polling_state = LIGHT_STATE;
        }
        else if (uart_devices[pctx->uart_id].group.meter_count != 0)
        {
            pctx->polling_state = METER_STATE;
        }
        break;
    }
}
/**************************************************************************************************
Function Name:
    void uart_scan_step(UART_ScanContext_t *ctx, uint8_t uart_id)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void uart_polling_step(UART_PollingContext_t *pctx)
{
    switch (pctx->polling_state)
    {
    case SHADE_STATE:
        if (uart_devices[pctx->uart_id].group.shade_count == 0)
        {
            pctx->polling_state = HMI_STATE;
            break;
        }

        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < SHADE_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->waiting_index = (pctx->waiting_index + 1) % uart_devices[pctx->uart_id].group.shade_count;
            if (pctx->waiting_index == 0)
            {
                pctx->waiting_index = 0;
                pctx->first_send_flag = 0;
                check_next_state(pctx, pctx->polling_state);
                break;
            }
        }

        shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[pctx->uart_id].group.shade[pctx->waiting_index].id, SHADE_STATUS_INQUIRY, LOW_PRIORITY);
        shadeProtocol.func.transmitData(&shadeProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case HMI_STATE:
        if (uart_devices[pctx->uart_id].group.hmi_count == 0)
        {
            pctx->polling_state = LIGHT_STATE;
            break;
        }

        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < HMI_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (hmi_regs.update_status & 0x0008)
        {
            if (hmi_regs.shade_motor_ctrl == 0)
            {
                shade_cmd = SHADE_STOP;
            }
            else if (hmi_regs.shade_motor_ctrl == 1)
            {
                shade_cmd = SHADE_UP;
            }
            else if (hmi_regs.shade_motor_ctrl == 2)
            {
                shade_cmd = SHADE_DOWN;
            }

            for (int j = 0; j < UART_COUNT; j++)
            {
                uart_devices[j].group.last_shade_cmd = shade_cmd;
            }

            hmi_regs.update_status = 0;
            shade_cmd = 0;
            pctx->waiting_index = 0;
            pctx->first_send_flag = 0;
            pctx->transmit_cnt = 0;
            pctx->polling_state = HMI_SHADE_CMD_STATE;

            break;
        }

        if (pctx->first_send_flag)
        {
            pctx->waiting_index = (pctx->waiting_index + 1) % uart_devices[pctx->uart_id].group.hmi_count;
            if (pctx->waiting_index == 0)
            {
                pctx->waiting_index = 0;
                pctx->first_send_flag = 0;
                check_next_state(pctx, pctx->polling_state);
                break;
            }
        }

        hmiProtocol.func.packTxData(&hmiProtocol, uart_devices[pctx->uart_id].group.hmi[pctx->waiting_index].id, READ_HOLDING_REGISTERS, REGISTER_UPDATE_STATUS, 5, LOW_PRIORITY);
        hmiProtocol.func.transmitData(&hmiProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case LIGHT_STATE:
        if (uart_devices[pctx->uart_id].group.light_count == 0)
        {
            pctx->polling_state = METER_STATE;
            break;
        }

        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < LIGHT_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->waiting_index = (pctx->waiting_index + 1) % uart_devices[pctx->uart_id].group.light_count;
            if (pctx->waiting_index == 0)
            {
                pctx->waiting_index = 0;
                pctx->first_send_flag = 0;
                check_next_state(pctx, pctx->polling_state);
                break;
            }
        }

        lightProtocol.func.packTxData(&lightProtocol, uart_devices[pctx->uart_id].group.light[pctx->waiting_index].id, READ_HOLDING_REGISTERS, LIGHT_TEMPERATURE, 7, LOW_PRIORITY);
        lightProtocol.func.transmitData(&lightProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case METER_STATE:
        if (uart_devices[pctx->uart_id].group.meter_count == 0)
        {
            pctx->polling_state = AIR_STATE;
            break;
        }

        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < METER_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->waiting_index = (pctx->waiting_index + 1) % uart_devices[pctx->uart_id].group.meter_count;
            if (pctx->waiting_index == 0)
            {
                pctx->waiting_index = 0;
                pctx->first_send_flag = 0;
                check_next_state(pctx, pctx->polling_state);
                break;
            }
        }

        meterProtocol.func.packTxData(&meterProtocol, uart_devices[pctx->uart_id].group.meter[pctx->waiting_index].id, READ_HOLDING_REGISTERS, METER_HUMIDITY, 8, LOW_PRIORITY);
        meterProtocol.func.transmitData(&meterProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case AIR_STATE:
        if (uart_devices[pctx->uart_id].group.air_count == 0)
        {
            pctx->polling_state = SHADE_STATE;
            break;
        }

        if (pctx->transmit_cnt != 0)
        {
            if (pctx->transmit_cnt < AIR_TIME_INTERVAL)
            {
                pctx->transmit_cnt ++;
                break;
            }
            else
            {
                pctx->transmit_cnt = 0;
            }
        }

        if (pctx->first_send_flag)
        {
            pctx->waiting_index = (pctx->waiting_index + 1) % uart_devices[pctx->uart_id].group.air_count;
            if (pctx->waiting_index == 0)
            {
                pctx->waiting_index = 0;
                pctx->first_send_flag = 0;
                check_next_state(pctx, pctx->polling_state);
                break;
            }
        }

        airProtocol.func.packTxData(&airProtocol, uart_devices[pctx->uart_id].group.air[pctx->waiting_index].id, READ_HOLDING_REGISTERS, AIR_PM2P5, 9, LOW_PRIORITY);
        airProtocol.func.transmitData(&airProtocol, pctx->dma_tx, pctx->uart->func.transmit);

        pctx->first_send_flag = 1;
        pctx->transmit_cnt ++;

        break;
    case HMI_SHADE_CMD_STATE:
        if (uart_devices[pctx->uart_id].group.shade_count != 0)
        {
            if (pctx->transmit_cnt != 0)
            {
                if (pctx->transmit_cnt < HMI_SHADE_CMD_TIME_INTERVAL)
                {
                    pctx->transmit_cnt ++;
                    break;
                }
                else
                {
                    pctx->transmit_cnt = 0;
                }
            }

            if (pctx->first_send_flag)
            {
                pctx->waiting_index = (pctx->waiting_index + 1) % uart_devices[pctx->uart_id].group.shade_count;
                if (pctx->waiting_index == 0)
                {
                    pctx->waiting_index = 0;
                    pctx->first_send_flag = 0;
                    uart_devices[pctx->uart_id].group.last_shade_cmd = 0;
                    pctx->polling_state = HMI_STATE;
                    break;
                }
            }

            shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[pctx->uart_id].group.shade[pctx->waiting_index].id, uart_devices[pctx->uart_id].group.last_shade_cmd, LOW_PRIORITY);
            shadeProtocol.func.transmitData(&shadeProtocol, pctx->dma_tx, pctx->uart->func.transmit);

            pctx->first_send_flag = 1;
            pctx->transmit_cnt ++;
        }
        else
        {
            uart_devices[pctx->uart_id].group.last_shade_cmd = 0;
            pctx->polling_state = HMI_STATE;
        }

        break;
    }
}

/**************************************************************************************************
Function Name:
    void device_polling(void *arg)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void device_polling(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    uint8_t i = 0;

    while (1)
    {
        for (i = 0 ; i < UART_COUNT ; i ++)
        {
            uart_polling_step(&polling_ctx[i]);
        }

        /* Update data every 25 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(25));
    }
}

/**************************************************************************************************
Function Name:
    void uart_receiving_step(UART_PollingContext_t *pctx)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void uart_receiving_step(UART_PollingContext_t *pctx)
{
    void *dummy_buf = NULL;
    uint8_t dummy_size = 0;

    if (scan_all_done)
    {
        switch (pctx->polling_state)
        {
        case SHADE_STATE:
            if (shadeProtocol.func.receiveData(&shadeProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                shadeProtocol.func.unpackRxData(&shadeProtocol, &uart_devices[pctx->uart_id].group.shade[pctx->waiting_index].shade_info);
            }
            break;
        case HMI_STATE:
            if (hmiProtocol.func.receiveData(&hmiProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                hmiProtocol.func.unpackRxData(&hmiProtocol, REGISTER_UPDATE_STATUS, &uart_devices[pctx->uart_id].group.hmi[pctx->waiting_index].hmi_info);
            }
            break;
        case LIGHT_STATE:
            if (lightProtocol.func.receiveData(&lightProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                lightProtocol.func.unpackRxData(&lightProtocol, LIGHT_TEMPERATURE, &uart_devices[pctx->uart_id].group.light[pctx->waiting_index].light_info);
            }
            break;
        case METER_STATE:
            if (meterProtocol.func.receiveData(&meterProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                meterProtocol.func.unpackRxData(&meterProtocol, METER_HUMIDITY, &uart_devices[pctx->uart_id].group.meter[pctx->waiting_index].meter_info);
            }
            break;
        case AIR_STATE:
            if (airProtocol.func.receiveData(&airProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                airProtocol.func.unpackRxData(&airProtocol, AIR_PM2P5, &uart_devices[pctx->uart_id].group.air[pctx->waiting_index].air_info);
            }
            break;
        }
    }
    else
    {
        switch (pctx->scan_state)
        {
        case SHADE_STATE:
            if (pctx->uart->func.receive(pctx->dma_rx, &dummy_buf, &dummy_size))
            {
                uint8_t *buf = (uint8_t *)dummy_buf;

                if (buf[1] == pctx->scan_index)
                {
                    uart_devices[pctx->uart_id].total_devices ++;
                    uart_devices[pctx->uart_id].group.shade[uart_devices[pctx->uart_id].group.shade_count++].id = pctx->scan_index;
                }
            }
            break;
        case HMI_STATE:
            if (hmiProtocol.func.receiveData(&hmiProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                uart_devices[pctx->uart_id].total_devices ++;
                uart_devices[pctx->uart_id].group.hmi[uart_devices[pctx->uart_id].group.hmi_count++].id = pctx->scan_index;
            }
            break;
        case LIGHT_STATE:
            if (lightProtocol.func.receiveData(&lightProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                uart_devices[pctx->uart_id].total_devices ++;
                uart_devices[pctx->uart_id].group.light[uart_devices[pctx->uart_id].group.light_count++].id = pctx->scan_index;
            }
            break;
        case METER_STATE:
            if (meterProtocol.func.receiveData(&meterProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                uart_devices[pctx->uart_id].total_devices ++;
                uart_devices[pctx->uart_id].group.meter[uart_devices[pctx->uart_id].group.meter_count++].id = pctx->scan_index;
            }
            break;
        case AIR_STATE:
            if (airProtocol.func.receiveData(&airProtocol, pctx->dma_rx, pctx->uart->func.receive))
            {
                uart_devices[pctx->uart_id].total_devices ++;
                uart_devices[pctx->uart_id].group.air[uart_devices[pctx->uart_id].group.air_count++].id = pctx->scan_index;
            }
            break;
        }
    }
}

/**************************************************************************************************
Function Name:
    void device_receiving(void *arg)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void device_receiving(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    static uint8_t receive_index = 0;

    while (1)
    {
        for (receive_index = 0 ; receive_index < UART_COUNT ; receive_index ++)
        {
            if (uartPacketSemaphore[receive_index])
            {
                uart_receiving_step(&polling_ctx[receive_index]);
                uartPacketSemaphore[receive_index] = 0;
            }
        }

        /* Update data every 1 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(1));
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
void uart_de_scan(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    while (1)
    {
        if (usart1.member.reg.STS->bit.TXC == 1)
        {
            usart1.member.reg.STS->bit.TXC = 0;
            USART1_DE_DISABLE;
        }
        if (usart2.member.reg.STS->bit.TXC == 1)
        {
            usart2.member.reg.STS->bit.TXC = 0;
            USART2_DE_DISABLE;
        }
        if (usart3.member.reg.STS->bit.TXC == 1)
        {
            usart3.member.reg.STS->bit.TXC = 0;
            USART3_DE_DISABLE;
        }
        if (usart4.member.reg.STS->bit.TXC == 1)
        {
            usart4.member.reg.STS->bit.TXC = 0;
            USART4_DE_DISABLE;
        }
        if (uart5.member.reg.STS->bit.TXC == 1)
        {
            uart5.member.reg.STS->bit.TXC = 0;
            UART5_DE_DISABLE;
        }
        if (uart6.member.reg.STS->bit.TXC == 1)
        {
            uart6.member.reg.STS->bit.TXC = 0;
            UART6_DE_DISABLE;
        }
        if (uart7.member.reg.STS->bit.TXC == 1)
        {
            uart7.member.reg.STS->bit.TXC = 0;
            UART7_DE_DISABLE;
        }
        if (uart8.member.reg.STS->bit.TXC == 1)
        {
            uart8.member.reg.STS->bit.TXC = 0;
            UART8_DE_DISABLE;
        }

        /* Update data every 1 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
/**************************************************************************************************
Function Name:
    void device_init(void)

Inputs:
    None

Outputs:
    None

Description:

**************************************************************************************************/
void device_init(void)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    if (xTaskCreate(device_receiving,
                    "device_receiving",
                    DEVICE_RECEIVING_TASK_STACK_SIZE,
                    NULL,
                    DEVICE_RECEIVING_TASK_PRIORITY,
                    &DeviceReceivingTaskHandle) != pdPASS)
    {
        printf("Task <<scan_device>> creation failed!\n");
    }

    if (xTaskCreate(scan_device,
                    "scan_device",
                    DEVICE_SCAN_TASK_STACK_SIZE,
                    NULL,
                    DEVICE_SCAN_TASK_PRIORITY,
                    &DeviceScanTaskHandle) != pdPASS)
    {
        printf("Task <<scan_device>> creation failed!\n");
    }
}

