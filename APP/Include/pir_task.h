#ifndef _PIR_TASK_H_
#define _PIR_TASK_H_


#include "main.h"

#define MAX_PIR_SENSORS              16
#define MAX_PIR_GROUPS               16
#define PIR_TASK_STACK_SIZE         configMINIMAL_STACK_SIZE * 3
#define PIR_TASK_PRIORITY           (configMAX_PRIORITIES - 4)

#define NORMAL_INTERVAL              pdMS_TO_TICKS(100)
#define MOTION_INTERVAL              pdMS_TO_TICKS(1000)
#define PIR_TIMER_PERIOD             pdMS_TO_TICKS(15*60*1000)
#define PIR_5_TIMER_PERIOD           pdMS_TO_TICKS(5*60*1000)
#define PIR_ONE_MIN_MS               pdMS_TO_TICKS(1*60*1000)


typedef struct __attribute__((__packed__)) {
    uint8_t id;              // sensor ID
    uint8_t port;            // port J6=1, J31=2
    bool configured;         // true if setup via MQTT
    bool active;             // runtime status
} __attribute__((aligned(4))) pir_sensor_t;

typedef struct __attribute__((__packed__)) motion_event
{
    uint8_t id;
    uint8_t port;
    uint8_t motion_status;
} __attribute__((aligned(4))) motion_event_t;


extern pir_sensor_t pir_sensors[MAX_PIR_SENSORS];
void pir_init(void) ;
void pir_task_create(void) ;

#endif