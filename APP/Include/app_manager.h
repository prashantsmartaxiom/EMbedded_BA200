#pragma once 
#ifndef __APP_MANAGER_H__
#define __APP_MANAGER_H__


#ifdef __cplusplus
extern "C" {
#endif

  
#include "main.h"

#define APP_MGR_TASK_STACK_SIZE     configMINIMAL_STACK_SIZE*4
#define APP_MGR_TASK_PRIORITY       configMAX_PRIORITIES - 4
#define MAX_SENSORS_PER_GROUP 5  
    
void app_manager_init(void);
void app_manager_task(void *arg); 

//runtime knobs 
void app_set_group_hold_ms(uint32_t grp_id, uint32_t hold_ms);
void app_set_occupied_level(uint32_t grp_id,uint8_t pct);



#ifdef __cplusplus
}
#endif
#endif

