/**
*\*\file bsp_cjson.h
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2023, Nations Technologies Inc. All rights reserved.
**/
#ifndef __BSP_CJSON_H__
#define __BSP_CJSON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stdint.h" 
#include "mqtt_client.h"  

#define NAME                 "name"
#define DEVICE_ID            "BAM-1E67892F7890"
#define SERIAL_NUM           "234BAM87695"     
#define MQTT_DEVICE_ID       "device_id"
  
#define CH_T                 "ch_t"  
#define CH_ADDR              "ch_addr"
#define CMD                  "cmd"
#define CMD_MSG              "cmd_m"
  
#define DEFAULT_CH_T         0
#define DEFAULT_CH_ADDR      "FF"
#define DEFAULT_CMD          110
#define DEFAULT_MSG          "config"  
  
//#define FIRMWARE             "v1.0.0.0"
#define MACADDR              (17)
  

#define UPDATE_SUCCESS       1
#define UPDATE_FAIL          0


cJSON* cJSON_Init(void);
uint8_t cJSON_Update(const cJSON * const object, const char * const string, void *pData);
void cJSON_Config_Process(void *pData,ControlMsg_t *ControlMsg);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_CJSON_H__ */
