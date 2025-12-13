#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "n32h47x_48x_spi.h"
#include "main.h"

#define MAX_LED_CHANNEL         22
#define MAX_CURRENT_LIMIT       200
#define MIN_CURRENT_LIMIT       10
#define PWR_DEFAULT_LIMIT       100
#define MAX_LED_EXT_CH          8
#define LED_TASK_STACK_SIZE     configMINIMAL_STACK_SIZE*2
#define LED_TASK_PRIORITY       configMAX_PRIORITIES - 5



void led_init();
void led_task(void *arg);
void led_hal_on_off(uint8_t nChannelAddr, uint8_t ledOnOffSwitch);
#ifdef __cplusplus
}
#endif
#endif
