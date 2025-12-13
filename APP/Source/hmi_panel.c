/* hmi_driver.c

*/

#include "hmi_panel.h"
void Hmi_task_state(void *arg);


extern UART_DeviceInfo_t uart_devices[UART_COUNT];
static TaskHandle_t HMITaskHandle = NULL;

void Hmi_task_state(void *arg)
{
    ScanParams_t *params = (ScanParams_t *)arg;
    HmiStruct_t hmi_data; /* buffer used by unpackRxData */
    
    for(;;) {
        for (int i = 0; i < UART_COUNT; i++) {
            
          if (uart_devices[i].group.hmi_count > 0) {
                
            //UART_PollingContext_t *pctx = &polling_ctx[i];
              for (int idx = 0; idx < uart_devices[i].group.hmi_count; ++idx) {
                  uint8_t sensor_id = uart_devices[i].group.hmi[idx].id;
                  HmiScanMode_e state = (HmiScanMode_e)uart_devices[i].group.hmi[idx].state;

                  switch (state) 
                  {
                    case HMI_RE_DISCOVERY_SCAN:
                    {
                         if(discovery_scan(params->obj, i, sensor_id, &hmi_data)) {
                            uart_devices[i].group.hmi[idx].state = HMI_CONFIGURE_SCAN;
                         }
                        break;
                    }

                    case HMI_CONFIGURE_SCAN:
                    {
                        //TODO. Need to change from mqtt que
                        if(config_hmi(params->obj, i, sensor_id, (uint16_t)0x0001, (uint16_t)0x0064, (uint16_t)0x0001, (uint16_t) 0x0000, &hmi_data)) {
                            uart_devices[i].group.hmi[idx].state = HMI_READ_SCAN;
                        }
                        break;
                    }

                    case HMI_READ_SCAN:
                    {
                        if(status_scan(params->obj, i, sensor_id, &hmi_data)) {
                            //nothing
                        } else {
                            uart_devices[i].group.hmi[idx].state = HMI_RE_DISCOVERY_SCAN;
                        }

                        break;
                    }
                    default:
                        break;
                 } 
              } /* for idx */
          }
          vTaskDelay(pdMS_TO_TICKS(100));
        }/* for i */
       // Delay before next round of scanning
       vTaskDelay(pdMS_TO_TICKS(100));
    } /* for(;;) */
}

/* --- Send Device Discovery Request --- */
void send_discovery_status_frame_request(HmiProtocolObj *obj, UART_PollingContext_t *pctx, uint8_t slave_addr, uint16_t start_addr, uint16_t qty)
{
   

    if (obj && obj->member.txData && obj->member.txData->buffer) {
        uint8_t tx_buf[10] = { 0 };
        uint16_t crc = 0x0000;
        int pos = 0;
        HmiBufferStr *hmi_Buf = (HmiBufferStr*)obj->member.txData->buffer;
        uint8_t *txBuf = &hmi_Buf->code[0];
   
        tx_buf[pos++] = slave_addr;
        tx_buf[pos++] = READ_HOLDING_REGISTERS;       /* 0x03 */
        tx_buf[pos++] = (start_addr >> 8) & 0xFF;     /* start hi */
        tx_buf[pos++] = start_addr & 0xFF;            /* start lo */
        tx_buf[pos++] = (qty >> 8) & 0xFF;;           /* qty hi */
        tx_buf[pos++] = qty & 0xFF;                   /* qty lo */

        crc = Calculate_HMI_CRC(tx_buf, pos);
        tx_buf[pos++] = crc & 0xFF;        /* CRC low */
        tx_buf[pos++] = (crc >> 8) & 0xFF; /* CRC high */
        memcpy(txBuf, tx_buf, pos);
        hmi_Buf->size = pos;
        obj->member.txData->length = pos;
        
        // Transmit using the UART functions from pctx (preferred)
        if (pctx != NULL && pctx->uart != NULL && pctx->uart->func.transmit) {
          /* driver transmit expects: (HmiProtocolObj*, dma_tx_ptr, uart_transmit_fn) */
          obj->func.transmitData(obj, pctx->dma_tx, pctx->uart->func.transmit);
        } else if (obj->func.transmitData) {
          /* fallback: call transmitData without pctx-specific args if supported */
          obj->func.transmitData(obj, NULL, NULL);
       }

   
   
//       HmiDataStr txData = {buffer, slave_addr,start_addr, qty};
//       HmiBufferStr *hmiRegPtr = (HmiBufferStr*)obj->member.txData->buffer + obj->member.txData->index;
//       
//       if (!obj->member.txQueue->lowPriority.full && !obj->member.txQueue->highPriority.full && 
//           HMI_Encoder(obj,hmiRegPtr,txData)) {
//         
//           obj->member.txData->index = (obj->member.txData->index + 1) & (obj->member.txData->length - 1);
//           QueueDataStr enqueueData = {hmiRegPtr->code, hmiRegPtr->size};
//           Priority_Buffer_Enqueue(obj->member.txQueue, enqueueData, priority);
//        }
    }
    
 }


/* --- Send Device Discovery Request --- */
void send_config_frame_request(HmiProtocolObj *obj, UART_PollingContext_t *pctx, uint8_t slave_addr, uint16_t reg_addr, uint16_t qty, uint16_t led_ch, uint16_t led_britness, uint16_t led_on_off, uint16_t shade_cfg)
{
    uint8_t tx_buf[16];
    uint16_t crc;
    uint8_t pos = 0;
    tx_buf[pos++] = slave_addr;
    tx_buf[pos++] = WRITE_MULTIPLE_REGISTER;      /* 0x10 */
    tx_buf[pos++] = (reg_addr >> 8) & 0xFF;       /* start hi */
    tx_buf[pos++] = reg_addr & 0xFF;              /* start lo */
    tx_buf[pos++] = (qty >> 8) & 0xFF;;           /* qty hi */
    tx_buf[pos++] = qty & 0xFF;;                  /* qty hi */
    tx_buf[pos++] = (led_ch >> 8) & 0xFF;         /* led_ch hi */
    tx_buf[pos++] = led_ch & 0xFF;                /* led_ch lo */
    tx_buf[pos++] = (led_britness >> 8) & 0xFF;   /* led_britness hi */
    tx_buf[pos++] = led_britness & 0xFF;          /* led_britness lo */
    tx_buf[pos++] = (led_on_off >> 8) & 0xFF;     /* led_on_off hi */
    tx_buf[pos++] = led_on_off & 0xFF;            /* led_on_off lo */
    tx_buf[pos++] = (shade_cfg >> 8) & 0xFF;      /* shade_cfg lo */
    tx_buf[pos++] = shade_cfg & 0xFF;             /* shade_cfg hi */

    crc = Calculate_HMI_CRC(tx_buf, pos);
    tx_buf[pos++] = crc & 0xFF;        /* CRC low */
    tx_buf[pos++] = (crc >> 8) & 0xFF; /* CRC high */
    // Copy to protocol tx buffer (if present)
    if (obj && obj->member.txData && obj->member.txData->buffer) {
        HmiBufferStr *txBuf = (HmiBufferStr*)obj->member.txData->buffer;
        memcpy(txBuf->code, tx_buf, pos);
        txBuf->size = pos;
        obj->member.txData->length = pos;
    }

    // Transmit using the UART functions from pctx (preferred)
    if (pctx != NULL && pctx->uart != NULL && pctx->uart->func.transmit) {
        /* driver transmit expects: (HmiProtocolObj*, dma_tx_ptr, uart_transmit_fn) */
        obj->func.transmitData(obj, pctx->dma_tx, pctx->uart->func.transmit);
    } else if (obj->func.transmitData) {
        /* fallback: call transmitData without pctx-specific args if supported */
        obj->func.transmitData(obj, NULL, NULL);
    }
}



// Modify Scan_HMI_Panel to use uart_index
bool discovery_scan(HmiProtocolObj *obj, uint8_t uart_index, uint8_t addr, HmiStruct_t *info)
{
    UART_PollingContext_t *pctx = &polling_ctx[uart_index];
    //log_info("Starting HMI Panel scan on UART %d from 0x%02X to 0x%02X",uart_index,HMI_START_ADDR,HMI_END_ADDR);

    send_discovery_status_frame_request(obj, pctx, addr, (uint16_t) START_ADDRESS_HMI, (uint16_t) SINGLE_QTY);
    
     vTaskDelay(pdMS_TO_TICKS(SCAN_DELAY_MS));

    if (pctx != NULL && pctx->uart != NULL) {
        if (hmiProtocol.func.receiveData(&hmiProtocol, pctx->dma_rx, pctx->uart->func.receive))
        {
            // uint16_t bytes_received = (uint16_t)hmiProtocol.member.rxData->length;
            // Pass uart_index to help track which UART found the device
            // process_device_id(obj, bytes_received, addr);
            process_rx_data(&hmiProtocol, addr, info);
        }
    }
    return true;
}


bool status_scan(HmiProtocolObj *obj, uint8_t uart_index, uint8_t addr, HmiStruct_t *info)
{
    UART_PollingContext_t *pctx = &polling_ctx[uart_index];
    // log_info("Starting HMI Panel scan on UART%d from 0x%02X to 0x%02X", 
    //          uart_index, START_ADDR, END_ADDR);

    send_discovery_status_frame_request(obj, pctx,addr, (uint16_t) START_CONF_ADDRESS_HMI, (uint16_t) FOUR_QTY);
    vTaskDelay(pdMS_TO_TICKS(SCAN_DELAY_MS));

    if (pctx != NULL && pctx->uart != NULL) {
        if (hmiProtocol.func.receiveData(&hmiProtocol, pctx->dma_rx, pctx->uart->func.receive))
        {
            // uint16_t bytes_received = (uint16_t)hmiProtocol.member.rxData->length;
            // Pass uart_index to help track which UART found the device
            // process_device_id(obj, bytes_received, addr);
            process_rx_data(&hmiProtocol, addr, info);
        }
    }
    return true;
}



bool config_hmi(HmiProtocolObj *obj, uint8_t uart_index, uint8_t addr, uint16_t led_ch, uint16_t led_britness, uint16_t led_on_off, uint16_t shade_cfg, HmiStruct_t *info)
{
    UART_PollingContext_t *pctx = &polling_ctx[uart_index];
    // log_info("Starting HMI Panel scan on UART%d from 0x%02X to 0x%02X", 
    //          uart_index, START_ADDR, END_ADDR);

    send_config_frame_request(obj, pctx, addr, (uint16_t) START_ADDRESS_HMI, (uint16_t) FOUR_QTY, led_ch, led_britness, led_on_off, shade_cfg);
    vTaskDelay(pdMS_TO_TICKS(SCAN_DELAY_MS));

    if (pctx != NULL && pctx->uart != NULL) {
        if (hmiProtocol.func.receiveData(&hmiProtocol, pctx->dma_rx, pctx->uart->func.receive))
        {
            // uint16_t bytes_received = (uint16_t)hmiProtocol.member.rxData->length;
            // Pass uart_index to help track which UART found the device
            // process_device_id(obj, bytes_received, addr);
            process_rx_data(&hmiProtocol, addr, info);
        }
    }
    return true;
}


// create the task (call during system init)
void Start_HMI_Scan_Task(void)
{
    // adjust stack size and priority to your project
  static ScanParams_t scanParams =  { 0 };
  scanParams.obj = &hmiProtocol;
    
     if (xTaskCreate(Hmi_task_state,
                  "ScanHMI",
                  HMI_TASK_STACK_SIZE,
                  &scanParams,
                  HMI_TASK_PRIORITY,
                  &HMITaskHandle) != pdPASS) {
                    log_info("Task <<HMI Task>> creation failed!\n");
                  }
}

//  22 03 00 00 00 05 82 9A 
//  22 03 0A 00 22 00 01 00 64 00 01 00 00 EE 7C 
 // slave id
 // led_ch
 // britness lev
 // on_off_status
 //not configured
 //crc l
 //crc h