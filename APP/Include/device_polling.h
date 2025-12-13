/**************************************************************************************************
File Name: device_polling.h
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

#ifndef __DEVICE_POLLING_H__
#define __DEVICE_POLLING_H__

#ifdef __cplusplus
extern "C" {
#endif


//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "main.h"


//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define DEVICE_SCAN_TASK_STACK_SIZE        (512)
#define DEVICE_SCAN_TASK_PRIORITY          (3)
#define DEVICE_POLLING_TASK_STACK_SIZE     (512)
#define DEVICE_POLLING_TASK_PRIORITY       (5)
#define DEVICE_RECEIVING_TASK_STACK_SIZE   (256)
#define DEVICE_RECEIVING_TASK_PRIORITY     (5)
#define UART_DE_SCAN_TASK_STACK_SIZE       (256)
#define UART_DE_SCAN_TASK_PRIORITY         (2)

#define MAX_DEVICE_NUM                     15
#define UART_COUNT                         8

#define SHADE_START_ADDR                   0x11
#define SHADE_END_ADDR                     0x1F
#define HMI_START_ADDR                     0x21
#define HMI_END_ADDR                       0x2F
#define LIGHT_START_ADDR                   0x31
#define LIGHT_END_ADDR                     0x3F
#define METER_START_ADDR                   0x41
#define METER_END_ADDR                     0x4F
#define AIR_START_ADDR                     0x51
#define AIR_END_ADDR                       0x5F

#define POLLING_TIME_INTERVAL              25  // 25ms
#define SHADE_TIME_INTERVAL                50  / POLLING_TIME_INTERVAL
#define HMI_TIME_INTERVAL                  50  / POLLING_TIME_INTERVAL
#define LIGHT_TIME_INTERVAL                100 / POLLING_TIME_INTERVAL
#define METER_TIME_INTERVAL                100 / POLLING_TIME_INTERVAL
#define AIR_TIME_INTERVAL                  100 / POLLING_TIME_INTERVAL
#define HMI_SHADE_CMD_TIME_INTERVAL        50  / POLLING_TIME_INTERVAL

//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration
typedef enum
{
    SHADE_STATE,
    HMI_STATE,
    LIGHT_STATE,
    METER_STATE,
    AIR_STATE,
    HMI_SHADE_CMD_STATE
} PollingState;

// Struct & Union
typedef struct
{
    uint8_t id;
    ShadeStruct_t shade_info;
} Shade_Info_t;

typedef struct
{
    uint8_t id;
    uint8_t state;
    HmiStruct_t hmi_info;
} HMI_Info_t;

typedef struct
{
    uint8_t id;
    LightStruct_t light_info;
} Ceiling_Light_Info_t;

typedef struct
{
    uint8_t id;
    MeterStruct_t meter_info;
} Meterorological_Info_t;

typedef struct
{
    uint8_t id;
    AirStruct_t air_info;
} Air_Quality_Info_t;

typedef struct
{
    uint8_t hmi_count;
    uint16_t last_shade_cmd;
    HMI_Info_t hmi[MAX_DEVICE_NUM];

    uint8_t shade_count;
    Shade_Info_t shade[MAX_DEVICE_NUM];

    uint8_t light_count;
    Ceiling_Light_Info_t light[MAX_DEVICE_NUM];

    uint8_t meter_count;
    Meterorological_Info_t meter[MAX_DEVICE_NUM];

    uint8_t air_count;
    Air_Quality_Info_t air[MAX_DEVICE_NUM];
} DeviceGroup_t;

typedef struct
{
    uint8_t total_devices;
    DeviceGroup_t group;
} UART_DeviceInfo_t;

typedef struct
{
    UartObj *uart;
    DmaUartObj *dma_rx;
    DmaUartObj *dma_tx;
    uint8_t scan_state;
    uint8_t first_send_flag;
    uint8_t scan_index;
    uint8_t scan_done;
    uint8_t uart_id;
    uint8_t query_index;
    uint8_t waiting_index;
    uint8_t polling_state;
    uint8_t transmit_cnt;
    uint8_t hmi_shade_cmd_index;
} UART_PollingContext_t;
//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
//uint16_t GetUartIndex(uint16_t dev_id, uint16_t shade_cmd);
void scan_device(void *arg);
void device_polling(void *arg);
void device_receiving(void *arg);
void uart_de_scan(void *arg);
void device_init(void);
uint16_t GetUartIndex(uint16_t dev_id);

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern UART_PollingContext_t polling_ctx[UART_COUNT];
extern TaskHandle_t UartDeScanTaskHandle;

#ifdef __cplusplus
}
#endif
#endif /* __DEVICE_POLLING_H__ */


