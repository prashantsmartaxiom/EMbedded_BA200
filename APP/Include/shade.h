#ifndef __SHADE_H__
#define __SHADE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define SHADE_TASK_STACK_SIZE     configMINIMAL_STACK_SIZE*2
#define SHADE_TASK_PRIORITY       configMAX_PRIORITIES - 4


void shade_init();
void shade_task(void *arg);

#ifdef __cplusplus
}
#endif
#endif

