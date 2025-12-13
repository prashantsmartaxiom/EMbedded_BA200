/******************************************************************************************
 * File: Hmi_panel.h
 * Description:
 *    Header defining structures, macros, and function prototypes for HMI protocol driver.
 *    Supports packing, transmitting, receiving, and unpacking of Modbus-like HMI data.
 *
 * Author: Prashant Gupta
 * Date:   2025-10-24
 ******************************************************************************************/

#ifndef __HMI_PANEL_H__
#define __HMI_PANEL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"   
   
#define SCAN_DELAY_MS           50     // Delay between requests
#define START_ADDRESS_HMI       0x0000
#define START_CONF_ADDRESS_HMI  0x0001
#define SINGLE_QTY              0x0001
#define FOUR_QTY                0x0004

#define HMI_TASK_STACK_SIZE configMINIMAL_STACK_SIZE * 3
#define HMI_TASK_PRIORITY   configMAX_PRIORITIES - 4 

typedef struct { 
     HmiProtocolObj *obj; 
     uint8_t uart_index; 
} ScanParams_t;

bool discovery_scan(HmiProtocolObj *obj, uint8_t uart_index, uint8_t addr, HmiStruct_t *info);
bool config_hmi(HmiProtocolObj *obj, uint8_t uart_index, uint8_t addr, uint16_t led_ch, uint16_t led_britness, uint16_t led_on_off, uint16_t shade_cfg, HmiStruct_t *info);
bool status_scan(HmiProtocolObj *obj, uint8_t uart_index, uint8_t addr, HmiStruct_t *info);
void Start_HMI_Scan_Task(void);

//void send_discovery_status_frame_request(HmiProtocolObj *obj, uint8_t slave_addr, uint16_t start_addr, uint16_t qty,uint8_t priority);

void send_discovery_status_frame_request(HmiProtocolObj *obj, UART_PollingContext_t *pctx, uint8_t slave_addr, uint16_t start_addr, uint16_t qty);
void send_config_frame_request(HmiProtocolObj *obj, UART_PollingContext_t *pctx, uint8_t slave_addr, uint16_t reg_addr, uint16_t qty, uint16_t led_ch, uint16_t led_britness, uint16_t led_on_off, uint16_t shade_cfg);


#ifdef __cplusplus
}
#endif

#endif /* __HMI_PANNEL_H__ */