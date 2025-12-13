
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "pir_task.h"   
#include "mqtt_client.h"
#include "led.h"
#include "shade.h"
#include "hmi_panel.h"
  
/* ---- Events (recap) --- */

#define MAX_LED_CHANNEL 22
#define MAX_ED_LED_CH   8
#define MAX_SHADE_CHANNEL 4  

  typedef enum {
    EVT_PIR_DETECT,
    EVT_PIR_MOTION, 
    EVT_PIR_CLEAR, 
    EVT_HMI_LED_BUTTON,
    EVT_HMI_DIM_SLIDER,
    EVT_HMI_SHADE_UP_BUTTON,
    EVT_HMI_SHADE_DOWN_BUTTON,
    EVT_HMI_SHADE_STOP_BUTTON,
    EVT_SCHEDULE_TICK,
    EVT_MQTT_CMD,
    EVT_TIMER_GROUP_HOLD_EXPIRE,
    EVT_MODBUS_INPUT_CHANGE
} event_id_t;

typedef enum {
     CMD_LED_ON_OFF,                           //lED ON/OFF
     CMD_LED_SET_LEVEL,                        // Set dimming level
     CMD_LED_CONFIG,                           // set configuration
     CMD_LED_PWR_MAX,                          // set power level
     CMD_LED_STATUS,                           // set LED status
     CMD_SHADE_OPEN_CLOSE,
     CMD_SHADE_UP,
     CMD_SHADE_DOWN,
     CMD_SHADE_STATUS
} cmd_id_t;

typedef struct __attribute__((__packed__))
{
    uint8_t id;    
    uint8_t relay_status;
    uint8_t port;
} __attribute__((aligned(4))) relay_event_t;



typedef struct {
    uint32_t dev_id;                                                   // device logical id (e.g. PIR/LED/HMI/CO2/light)
    uint32_t grp_id;                                                   // group id
    event_id_t id;                                                     // event type
    uint32_t ts_ms;                                                    // event TS
    union {
      struct {uint8_t ch; uint8_t cmd; uint8_t level; } led;                    // LED/light level 0..100l
      struct {uint8_t ch; uint8_t cmd; uint8_t pos; } shade;                                   // shade position
      struct {uint8_t reg; int32_t val ;  } mb;                       // Modbus change 
      struct {uint8_t code; } hmi;                                    // HMI button 
      struct {uint32_t dev_id; ControlMsg_t pControMsg;} mqtt;  // 
      struct {uint32_t dev_id; relay_event_t ev;} pir;  
    } u;
} app_evt_t;

typedef struct {
   uint32_t dev_id;                                                   // device logical id (e.g. PIR/LED/HMI/CO2/light) 
   union {
     struct{uint8_t ch; uint8_t cmd; uint8_t level; uint16_t pwr_max; uint16_t pwr_min;} led;
     struct{uint8_t port; uint8_t cmd; uint16_t shadeID;} shade; 
   } u;
} app_cmd_t;

typedef enum {
  DEV_PIR  = 101,
  DEV_LED  = 201,
  DEV_SHADE= 301,
  DEV_HMI  = 401,
  DEV_LIGHT= 501
}DEV_GRP;

enum
{
    LED_CH_OFF                   = 0,
    LED_CH_ON                    = 1,
    LED_CH_SET_BRIGHTNESS        = 2,
    LED_CH_MAX_PWR               = 3,
    LED_CH_STATUS                = 4,
    SHADE_CH_OPEN                = 5,
    SHADE_CH_DOWN                = 6,
    SHADE_CH_STOP                = 7,
    SHADE_CH_UPPER_LIMIT         = 8,
    SHDAE_CH_LOWER_LIMIT         = 9,
    SHADE_CH_MIDDLE_LIMIT        = 10,
    SHADE_CH_DELETE_LIMIT        = 11,
    SHADE_CH_CONFIG              = 12,
    SHADE_CH_ID_SELECT           = 13,
    SHADE_CH_MIDDLE_LIMIT_1      = 14,
    SHADE_CH_MIDDLE_LIMIT_2      = 15,
    SHADE_CH_MIDDLE_LIMIT_3      = 16,
    SHADE_CH_MIDDLE_LIMIT_4      = 17,
    SHADE_CH_STATUS_INQUIRY      = 18

};


extern QueueHandle_t q_events;     //device -> app 
extern QueueHandle_t q_cmds;        //app -> device  ( here led_cmd_t, shade_cmd_t , HMI_cmd_t etc)
extern QueueHandle_t q_shade_cmds;
#ifdef __cplusplus
}
#endif
#endif
