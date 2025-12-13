/**************************************************************************************************
File Name: mqtt_client.h
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
     06/18      Wilson Chen      1. Initial version

**************************************************************************************************/

#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "FreeRTOS.h"
#include "queue.h"
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/etharp.h"
#include "lwip/sockets.h"
#include "ethernetif.h"
#include "cJSON.h"
#include "transport.h"
#include "MQTTPacket.h"
#include "sys.h"

//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define HOST_IP_ADDR0                      ((uint8_t)10U)
#define HOST_IP_ADDR1                      ((uint8_t)4U)
#define HOST_IP_ADDR2                      ((uint8_t)1U)
#define HOST_IP_ADDR3                      ((uint8_t)8U)

#define MESSAGE_QUEUE_LEN                  (5)
#define HOST_PORT                          (1883)
#define MSG_MAX_LEN                        (256)

#define MSG_TOPIC_LEN                       (128)
#define PING_TIME                           (30)
#define KEEPLIVE_TIME                       (60) /* In seconds */
#define MQTT_VERSION                        (4)
#define MAX_TOPIC_LENGTH                    (128)
// Device info data length
#define SNO_LENGTH                          (25)
#define ID_LENGTH                           (11)
#define MAC_LENGTH                          (18)
#define FW_VER_LENGTH                       (9)
#define PAD_LENGTH                          ((SNO_LENGTH + ID_LENGTH + MAC_LENGTH + FW_VER_LENGTH + 7) / 8 * 8) - \
                                            (SNO_LENGTH + ID_LENGTH + MAC_LENGTH + FW_VER_LENGTH)

#define CH_T                                "ch_t"
#define CH_ADDR                             "ch_addr"
#define CMD                                 "cmd"
#define CMD_MSG                             "cmd_m"
#define FIRMWARE                            "v1.0.0.2"

#define CLIENT_ID                           "BAM200_1"
#define USER_NAME                           "mps-bam100"
#define PASSWORD                            "bam100"
#define AVAILABILITY                        "MPS/global/availability"
#define DISCOVERY                           "MPS/global/discovery"
#define TIMER                               "/timer"
#define CONFIG                              "/config"
#define STATUS                              "/status"
#define CONTROL                             "/control"
#define REBOOT                              "/reboot"
#define SCENE                               "/scene"
#define LWT_TOPIC                           "MPS/global/disconnect"
#define SUB_BASE_TOPIC                      "MPS/global/"
#define PUB_BASE_TOPIC                      "MPS/global/UP/"


#define MQTT_TASK_STACK_SIZE                 configMINIMAL_STACK_SIZE*6
#define MQTT_CLIENT_RECEIVE_TASK_PRIORITY    configMAX_PRIORITIES-4
#define MQTT_CLIENT_PUBLISH_TASK_PRIORITY    configMAX_PRIORITIES-6

#define USE_PRINTF                          (1)

#if USE_PRINTF
#define ERR_PRINTF                          printf("#err: %s, %d, %s\n", __FUNCTION__, __LINE__, __FILE__)
#else
#define ERR_PRINTF
#endif

//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
typedef enum
{
    QOS0 = 0,
    QOS1,
    QOS2
} QoSType;

typedef enum {
  LED_STATUS               = 101,
  LED_CH_BRIGHTNESS        = 102,
  LED_COLOR                = 103,
  LED_ON_OFF               = 104,
  LED_MAX_CUR              = 105,
  LED_CONFIG               = 106,
  LED_CONFIG_CH            = 107,
  SHADE_CONTROL            = 108,
  SHADE_DOWN               = 109,
  SHADE_CONFIG             = 110,
  SHADE_MQTT_STOP          = 111,
  SHADE_STATUS             = 112,
  SHADE_OPEN               = 113,
  SHADE_CLOSE              = 114,
  PIR_STATE                = 115,
  TEMP_HUM                 = 116,
  SCENE_CMD                = 117,
  PIR_CONFIG               = 118,
  SHADE_LIMIT              = 119,
  SHADE_MOVE               = 120,
  GRP_PIR_TIMER            = 121
} command_t;

typedef enum
{
    MQTT_CONNECT_OK = 0,
    MQTT_CONNECT_NOK,
    MQTT_CONNECT_NOTACK
} ConnectType;

typedef enum
{
    LED,
    SHADE,
    PIR,
    HMI
} Channel_Type_t;


typedef struct __MQTTMessageType
{
    uint32_t qos;
    uint8_t  retained;
    uint8_t  dup;
    uint16_t id;
    uint8_t  type;
    void    *payload;
    int32_t  payloadlen;
} MQTTMessageType;

typedef struct __MQTTUserMsgType
{
    uint8_t  msgqos;               /* Message QoS */
    uint8_t  msg[MSG_MAX_LEN];     /* Messages */
    uint32_t msglenth;             /* Message Length */
    uint8_t  topic[MSG_TOPIC_LEN]; /* Topic */
    uint16_t packetid;             /* Message ID */
    uint8_t  valid;                /* Message valid flag */
} MQTTUserMsgType;

typedef struct __attribute__((__packed__)) __Deviceinfo
{
    uint8_t SNO[SNO_LENGTH];                    // Serial Number, +1 for string NUL
    uint8_t device_id[ID_LENGTH];               // Device ID, +1 for string NUL
    uint8_t mac_address[MAC_LENGTH];            // MAC Address (XX:XX:XX:XX:XX:XX), +1 for string NUL
    uint8_t firmware_version[FW_VER_LENGTH];    // Firmware Version, +1 for string NUL
    uint8_t padding[PAD_LENGTH];
} __attribute__((aligned(4))) DeviceInfo_t;

typedef struct  __ControlMsg
{
  uint8_t        channel_type[6];
  uint8_t        channel_addr[10];
  command_t      command;
  uint8_t        message[MSG_MAX_LEN];
  uint8_t        response_topic[MAX_TOPIC_LENGTH];
  uint8_t        gr_addr[22];
} ControlMsg_t;

//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
void mqtt_init(void);

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern QueueHandle_t MQTTCommandQueueHandle;
extern QueueHandle_t MQTTResponseQueueHandle;
extern TaskHandle_t MQTTPublishTaskHandle;
extern TaskHandle_t MQTTReceiveTaskHandle;

#endif /* __MQTT_CLIENT_H__ */


