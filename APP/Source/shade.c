#include "gui_box_control.h"
#include "main.h"
#include "shade.h"

static TaskHandle_t SHADETaskHandle = NULL;
u8 shadeId[4] = {0, 0, 0, 0};
extern UART_DeviceInfo_t uart_devices[UART_COUNT];

static void shade_respond_msg(uint8_t pShadeAddr, command_t cmd)
{
  ControlMsg_t ControlMsg =  { 0 } ; 
  BaseType_t xReturned;
  
  //TODO: prepare respose to broker 
  sprintf((char *)ControlMsg.channel_addr,"SHADE%d",pShadeAddr);  
  strncpy((char *)ControlMsg.channel_type, "SHADE",5);
  
  sprintf((char *)ControlMsg.response_topic,"%s%s%s",PUB_BASE_TOPIC,device_info.device_id,STATUS);
  ControlMsg.command = cmd;
  sprintf((char *)ControlMsg.message,"%s", "CMD SUCCESS");
  
  xReturned = xQueueSend(MQTTResponseQueueHandle, /* Message Queue Handle */
                         (void *) &ControlMsg,         /* Content of messages sent */
                         0);                  /* Wait time in ms */
  
  if (xReturned == SUCCESS) {
    log_info("Publishing Command to Queue for processing \n");
  }
  
  return;
}

static void shade_hal_stop(uint16_t nShadePort,uint8_t shade_idx, uint8_t shade_cmd)
{
  // Validate LED index (1..12)
  if ((nShadePort == 0) || (nShadePort > MAX_SHADE_CHANNEL))
    return;
  
  // Get pointer to the STATUS entry
  uint16_t uart_index = GetUartIndex(nShadePort);
  
  for (int idx = 0 ; idx < MAX_DEVICE_NUM ; idx++) {
        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx ) {
           shadeProtocol.func.packTxData(&shadeProtocol,uart_devices[uart_index].group.shade[idx].id, SHADE_STOP, LOW_PRIORITY);
           shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
           return;
        }
  }
 
  
//  if(uart_devices[uart_index].total_devices > 0 && uart_devices[uart_index].group.shade_count > 0) {
//      for (int idx = 0 ; idx < uart_devices[uart_index].group.shade_count ; idx++) {
//        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx) {
//          shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[uart_index].group.shade[idx].id, SHADE_STOP, HIGH_PRIORITY);
//          shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
//        }
//      }
//  }
//  //responde to broker
  shade_respond_msg(nShadePort,(command_t)shade_cmd);
}

static void shade_hal_close(uint16_t nShadePort,uint8_t shade_idx, uint8_t shade_cmd)
{
 
  // Validate LED index (1..12)
  if ((nShadePort == 0) || (nShadePort > MAX_SHADE_CHANNEL))
    return;
  
  uint16_t uart_index = GetUartIndex(nShadePort); 
  
  for (int idx = 0 ; idx < MAX_DEVICE_NUM ; idx++) {
        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx ) {
           shadeProtocol.func.packTxData(&shadeProtocol,uart_devices[uart_index].group.shade[idx].id, SHADE_DOWN, LOW_PRIORITY);
           shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
           return;
        }
  }
 
  
//  if(uart_devices[uart_index].total_devices > 0 && uart_devices[uart_index].group.shade_count > 0) {
//      for (int idx = 0 ; idx < uart_devices[uart_index].group.shade_count ; idx++) {
//        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx) {
//          shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[uart_index].group.shade[idx].id, SHADE_DOWN, HIGH_PRIORITY);
//          shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
//        }
//      }
//  }
//  
  //responde to broker
  shade_respond_msg(nShadePort,(command_t)shade_cmd);
  
}

static void shade_hal_open(uint16_t nShadePort, uint8_t shade_idx,uint8_t shade_cmd)
{
  
  // Validate LED index (1..12)
  if ((nShadePort == 0) || (nShadePort > MAX_SHADE_CHANNEL))
    return;
  
  uint16_t uart_index = GetUartIndex(nShadePort);
  for (int idx = 0 ; idx < MAX_DEVICE_NUM ; idx++) {
        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx ) {
           shadeProtocol.func.packTxData(&shadeProtocol,uart_devices[uart_index].group.shade[idx].id, SHADE_UP, LOW_PRIORITY);
           shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
           return;
        }
  }
 
  
//  if(uart_devices[uart_index].total_devices > 0 && uart_devices[uart_index].group.shade_count > 0) {
//      for (int idx = 0 ; idx < uart_devices[uart_index].group.shade_count ; idx++) {
//        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx) {
//          shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[uart_index].group.shade[idx].id, SHADE_UP, HIGH_PRIORITY);
//          shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
//        }
//      }
//  }
  
  //responde to broker
  shade_respond_msg(nShadePort,(command_t)shade_cmd);
}



static void shade_hal_ID(uint16_t nShadePort, uint8_t shade_idx,uint8_t shade_cmd) 
{
  
   // Validate LED index (1..12)
  if ((nShadePort == 0) || (nShadePort > MAX_SHADE_CHANNEL))
    return;
  
  if ((shade_idx < SHADE_START_ADDR) ||   (shade_idx > SHADE_END_ADDR))
    return;
  
  uint16_t uart_index = GetUartIndex(nShadePort);
  
  for (int idx = 0 ; idx < MAX_DEVICE_NUM ; idx++) {
      if (uart_devices[uart_index].group.shade[idx].id == 0x00){
        if ( uart_devices[uart_index].group.shade[idx].id != shade_idx ) {
           uart_devices[uart_index].group.shade[idx].id = shade_idx;
           uart_devices[uart_index].group.shade_count += 1;
           uart_devices[uart_index].total_devices +=1;
           shadeProtocol.func.packTxData(&shadeProtocol,uart_devices[uart_index].group.shade[idx].id, SHADE_ID_SELECT, HIGH_PRIORITY);
           shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
           vTaskDelay(pdMS_TO_TICKS(250));
           return;
        }
      }  
  }
  
    
  
//  if(uart_devices[uart_index].total_devices > 0 && uart_devices[uart_index].group.shade_count > 0) {
//      for (int idx = 0 ; idx < uart_devices[uart_index].group.shade_count ; idx++) {
//        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx) {
//          shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[uart_index].group.shade[idx].id, SHADE_ID_SELECT, HIGH_PRIORITY);
//          shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
//        }
//      }
//  }
  
  //responde to broker
  shade_respond_msg(nShadePort,(command_t)shade_cmd);

}
static void shade_hal_upper_limit(uint16_t nShadePort, uint8_t shade_idx,uint8_t shade_cmd)
{
       // Validate LED index (1..12)
  if ((nShadePort == 0) || (nShadePort > MAX_SHADE_CHANNEL))
    return;
    
  uint16_t uart_index = GetUartIndex(nShadePort);
  for (int idx = 0 ; idx < MAX_DEVICE_NUM ; idx++) {
        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx ) {
           shadeProtocol.func.packTxData(&shadeProtocol,uart_devices[uart_index].group.shade[idx].id,SHADE_UP_LIMIT_SELECT , LOW_PRIORITY);
           shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
           return;
        }
  }
 
  
  
//  if(uart_devices[uart_index].total_devices > 0 && uart_devices[uart_index].group.shade_count > 0) {
//      for (int idx = 0 ; idx < uart_devices[uart_index].group.shade_count ; idx++) {
//        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx) {
//          shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[uart_index].group.shade[idx].id, SHADE_UP_LIMIT_SELECT, LOW_PRIORITY);
//          shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
//        }
//      }
//  }
//  
  //responde to broker
  shade_respond_msg(nShadePort,(command_t)shade_cmd);

  
}

static void shade_hal_lower_limit(uint16_t nShadePort, uint8_t shade_idx,uint8_t shade_cmd)
{
       // Validate LED index (1..12)
  if ((nShadePort == 0) || (nShadePort > MAX_SHADE_CHANNEL))
    return;
  
  uint16_t uart_index = GetUartIndex(nShadePort);
  for (int idx = 0 ; idx < MAX_DEVICE_NUM ; idx++) {
        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx ) {
           shadeProtocol.func.packTxData(&shadeProtocol,uart_devices[uart_index].group.shade[idx].id,SHADE_LOW_LIMIT_SELECT , LOW_PRIORITY);
           shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
           return;
        }
  }
 
  
//  if(uart_devices[uart_index].total_devices > 0 && uart_devices[uart_index].group.shade_count > 0) {
//      for (int idx = 0 ; idx < uart_devices[uart_index].group.shade_count ; idx++) {
//        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx) {
//          shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[uart_index].group.shade[idx].id, SHADE_LOW_LIMIT_SELECT, LOW_PRIORITY);
//          shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
//        }
//      }
//  }
//  
  //responde to broker
  shade_respond_msg(nShadePort,(command_t)shade_cmd);

  
}

static void shade_hal_middle_limit(uint16_t nShadePort, uint8_t shade_idx,uint8_t shade_cmd)
{
       // Validate LED index (1..12)
  if ((nShadePort == 0) || (nShadePort > MAX_SHADE_CHANNEL))
    return;
  
  uint16_t uart_index = GetUartIndex(nShadePort);
  
  for (int idx = 0 ; idx < MAX_DEVICE_NUM ; idx++) {
        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx ) {
           shadeProtocol.func.packTxData(&shadeProtocol,uart_devices[uart_index].group.shade[idx].id,SHADE_MIDDLE_LIMIT_SELECT , LOW_PRIORITY);
           shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
           return;
        }
  }
 
  
//  if(uart_devices[uart_index].total_devices > 0 && uart_devices[uart_index].group.shade_count > 0) {
//      for (int idx = 0 ; idx < uart_devices[uart_index].group.shade_count ; idx++) {
//        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx) {
//          shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[uart_index].group.shade[idx].id, SHADE_MIDDLE_LIMIT_SELECT, LOW_PRIORITY);
//          shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
//        }
//      }
//  }
//  
  //responde to broker
  shade_respond_msg(nShadePort,(command_t)shade_cmd);

  
}

static void shade_hal_delete_limit(uint16_t nShadePort, uint8_t shade_idx,uint8_t shade_cmd)
{
       // Validate LED index (1..12)
  if ((nShadePort == 0) || (nShadePort > MAX_SHADE_CHANNEL))
    return;
  
  uint16_t uart_index = GetUartIndex(nShadePort);
 
   for (int idx = 0 ; idx < MAX_DEVICE_NUM ; idx++) {
        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx ) {
           shadeProtocol.func.packTxData(&shadeProtocol,uart_devices[uart_index].group.shade[idx].id,SHADE_DELETE_ALL_LIMITS , LOW_PRIORITY);
           shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
           return;
        }
  }
 
//  if(uart_devices[uart_index].total_devices > 0 && uart_devices[uart_index].group.shade_count > 0) {
//      for (int idx = 0 ; idx < uart_devices[uart_index].group.shade_count ; idx++) {
//        if ( uart_devices[uart_index].group.shade[idx].id == shade_idx) {
//          shadeProtocol.func.packTxData(&shadeProtocol, uart_devices[uart_index].group.shade[idx].id, SHADE_DELETE_ALL_LIMITS, LOW_PRIORITY);
//          shadeProtocol.func.transmitData(&shadeProtocol, polling_ctx[uart_index].dma_tx, polling_ctx[uart_index].uart->func.transmit);
//        }
//      }
//  }
//  
  //responde to broker
  shade_respond_msg(nShadePort,(command_t)shade_cmd);

  
}




void shade_task( void *arg) 
{
  for (;;) {
    app_cmd_t c = { 0 };
    if (xQueueReceive(q_shade_cmds, &c, portMAX_DELAY) == pdTRUE) {
      if (c.dev_id != DEV_SHADE) continue; // not ours
      switch(c.u.shade.cmd) {
      case SHADE_CH_OPEN:
        shade_hal_open(c.u.shade.port,c.u.shade.shadeID,c.u.shade.cmd);   
        break;
      case SHADE_CH_DOWN:
        shade_hal_close(c.u.shade.port,c.u.shade.shadeID,c.u.shade.cmd);
        break;  
      case SHADE_CH_STOP:
        shade_hal_stop(c.u.shade.port,c.u.shade.shadeID,c.u.shade.cmd);
        break;  
      case SHADE_CH_UPPER_LIMIT:
        shade_hal_upper_limit(c.u.shade.port,c.u.shade.shadeID,c.u.shade.cmd);
        break;  
      case SHDAE_CH_LOWER_LIMIT:
        shade_hal_lower_limit(c.u.shade.port,c.u.shade.shadeID,c.u.shade.cmd);
        break;  
      case SHADE_CH_MIDDLE_LIMIT:
        shade_hal_middle_limit(c.u.shade.port,c.u.shade.shadeID,c.u.shade.cmd);
        break;  
      case SHADE_CH_DELETE_LIMIT:
        shade_hal_delete_limit(c.u.shade.port,c.u.shade.shadeID,c.u.shade.cmd);
        break;  
      case SHADE_CH_CONFIG:
        break;
      case SHADE_CH_ID_SELECT:
        shade_hal_ID(c.u.shade.port,c.u.shade.shadeID,c.u.shade.cmd);
        break;
      case  SHADE_CH_STATUS_INQUIRY :
        break;
      default:
        log_info("Unsupport shade command \r\n");
      }
    }
    
  }
}

void shade_init()
{
  /* Create MQTT Publish Task */
  if (xTaskCreate(shade_task, "shade_task",SHADE_TASK_STACK_SIZE,
                  NULL, SHADE_TASK_PRIORITY,&SHADETaskHandle) != pdPASS) {
      log_info("Task <<shade Task>> creation failed!\n");
  }
}
