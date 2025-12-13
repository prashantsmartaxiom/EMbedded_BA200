/***********************************************************************************************************************
File Name: hmi_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    07/21/2025  Wilson Chen      Create file

***********************************************************************************************************************/

#ifndef __HMI_DRIVER_H__
#define __HMI_DRIVER_H__

/* Includes */
#include "circular_buffer.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "FreeRTOS.h"
#include "queue.h"

/* Macro definitions */
// Statement Replacement
#ifndef Check_Power_2
#define Check_Power_2(Number)               (((((unsigned)Number - 1) ^ Number) == (Number << 1) - 1) ? Number : -1)
#endif

#ifndef Check_Index
#define Check_Index(List_Length, Index_Length)  (Index_Length * (Index_Length >= List_Length))
#endif

// Group of declarations - Command index length must greater than command list length
#define Create_HMI_Command(Name, \
                           List_Length, \
                           Index_Length)        extern HmiCommandInfoStr *Name##Index[];\
                                                extern const HmiCommandInfoStr Name##List[];\
                                                HmiCommandArrayType Name = {\
                                                                            Name##List, List_Length,\
                                                                            Name##Index, Index_Length\
                                                                        };\
                                                HmiCommandInfoStr *Name##Index[Check_Index(List_Length, Index_Length)];\
                                                const HmiCommandInfoStr Name##List[List_Length]

// Group of declarations - Size MUST be power of 2 (2^n)
#define Create_HMI_Protocol_Object(ObjName, \
                                   Size)        Create_HMI_Protocol_Member(ObjName##Tx, ObjName##Rx, \
                                                                           Check_Power_2(Size));\
                                                HmiProtocolObj ObjName = \
                                                {\
                                                    {\
                                                        &ObjName##TxLink, &ObjName##RxLink, \
                                                        &ObjName##TxQueue, &ObjName##RxQueue, 0, 0\
                                                    }, \
                                                    {\
                                                        Process_HMI_Internal_RXD, \
                                                        Calculate_HMI_CRC\
                                                    }, \
                                                    {\
                                                        Init_HMI_Protocol, \
                                                        Pack_HMI_Internal_TXD, \
                                                        Unpack_HMI_Internal_RXD, \
                                                        Transmit_HMI_Internal_Data, \
                                                        Receive_HMI_Internal_Data\
                                                    }\
                                                }

#define Create_HMI_Protocol_Member(TxName, \
                                   RxName, \
                                   Size)        Create_HMI_Register(TxName##Link, Size);\
                                                Create_HMI_Register(RxName##Link, Size);\
                                                Create_Priority_Queue(TxName##Queue, Size, Size);\
                                                Create_Circular_Queue(RxName##Queue, Size)

#define Create_HMI_Register(Name, Size)         HmiBufferStr Name##Buffer[Size];\
                                                HmiDataRegStr Name = {\
                                                                     Name##Buffer,\
                                                                     sizeof(Name##Buffer) / sizeof(HmiBufferStr),\
                                                                     0\
                                                                  }

#define Create_Priority_Queue(Name, \
                              L_Size, H_Size)   QueueDataStr Name##LowData[L_Size], Name##HighData[H_Size];\
                                                PriorQueueRegStr Name = \
                                                {\
                                                   {0, 0, 0, 0, 0, Name##LowData, sizeof(Name##LowData) >> 3},\
                                                   {0, 0, 0, 0, 0, Name##HighData, sizeof(Name##HighData) >> 3},\
                                                   0\
                                                }

#define Create_Circular_Queue(Name, Size)       QueueDataStr Name##Data[Size];\
                                                QueueRegStr Name = \
                                                {0, 0, 0, 0, 0, Name##Data, sizeof(Name##Data) >> 3}

#define REGISTER_UPDATE_STATUS                  0x00
#define LED_SWITCH                              0x01
#define LED_BRIGHTNESS                          0x02
#define LED_COLOR_TEMPERATURE                   0x03
#define SHADE_MOTOR_CONTROL                     0x04

#define READ_HOLDING_REGISTERS                  0x03
#define WRITE_SINGLE_REGISTER                   0x06
#define MODBUS_MULTIPLE_REGISTER                0x10

#define PANEL_REG_ADDR                          0x00                                               
#define PANEL_LED_CHANNEL                       0x01
#define PANEL_LED_BRIGHTNESS                    0x02
#define PANEL_LED_STATUS                        0x03
#define PANEL_LED_POWER                         0x04
#define PANEL_SHADE_CHANNEL                     0x05
#define PANEL_SHADE_STATUS                      0x06
                                                
#define HMI_STATUS_SUCCESS                     0x0001
                                               

#define PACKAGE_SIZE                            32                                                    
#define MAX_HMI_DEVICES 5
/* Type definitions */
// Enumeration
typedef enum
{
    HMI_RE_DISCOVERY_SCAN = 0,   // Re-discovery scan for devices
    HMI_CONFIGURE_SCAN,          // Configuration scan (send LED/config commands)
    HMI_READ_SCAN                // Read status scan (read LED status or device info)
} HmiScanMode_e;

// Add the command queue structure
typedef struct {
    uint8_t uart_port;
    uint8_t sensor_index;
    uint8_t buf[PACKAGE_SIZE];
} hmi_command_req_t;

// Add to HmiProtocolMemberStr structure
typedef struct {
    unsigned char *buffer;
    unsigned long length;
    unsigned long index;
    uint8_t *deviceState;  // Add this field
} HmiProtocolMemberStr;

// Declare the command queue handle
extern QueueHandle_t HIMCommandQueueHandle;

// Struct & Union
typedef struct
{
    uint16_t update_status;
    uint8_t  led_switch;
    uint8_t  led_brightness;
    uint16_t led_color_temp;
    uint8_t  shade_motor_ctrl;
} HmiStruct_t;


// Struct & Union
typedef struct
{
    uint8_t  panel_id;
    uint16_t led_channel;
    uint16_t led_brightness;
    uint16_t led_status;
    uint16_t led_power;
    uint16_t shade_channel;
    uint16_t shade_status;
} PanelStruct_t;

typedef struct
{
    unsigned char size;
    unsigned long offset;
    unsigned char code[PACKAGE_SIZE];
}HmiBufferStr;

typedef struct
{
    void *ptr;
    unsigned char id;
    unsigned char func;
    unsigned char addr;
    unsigned long length;
}HmiDataStr;

typedef struct
{
    void *ptr;
    unsigned long size;
}HmiDataPackStr;

typedef struct
{
	unsigned char code;
	void *ptr;
	unsigned long size;
}HmiCommandInfoStr;

typedef struct
{
    const HmiCommandInfoStr *list;
    unsigned long listLength;
    HmiCommandInfoStr **index;
    unsigned long indexLength;
}HmiCommandArrayType;

typedef struct
{
    void *buffer;
    unsigned char length;
    unsigned char index;
}HmiDataRegStr;

typedef struct HmiProtocolStr HmiProtocolObj;

struct HmiProtocolStr
{
    struct
    {
        HmiDataRegStr *txData;             // Transmit data register pointer
        HmiDataRegStr *rxData;             // Receive data register pointer
        PriorQueueRegStr *txQueue;      // Transmit pointer queue
        QueueRegStr *rxQueue;           // Receive pointer queue
        HmiCommandInfoStr **listIndex;     // Pointer of the command table index
        unsigned long indexLength;      // Command table index length
    }member;

    struct
    {
        void (*rxDataProcess)(HmiProtocolObj*, HmiDataPackStr, unsigned char, HmiStruct_t*); // Process method of received data
        unsigned long (*calculateChecksum)(unsigned char*, unsigned short);   // Method of the checksum calculation
    }method;

    struct
    {
        void (*init)(HmiProtocolObj*, HmiCommandArrayType*);                       // Initialization of command index
        void (*packTxData)(HmiProtocolObj*,                                     // Pack transmit data of the command
                           unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
        unsigned char (*unpackRxData)(HmiProtocolObj*, unsigned char, HmiStruct_t*);          // Unpack received data of all commands
        void (*transmitData)(HmiProtocolObj *obj, void *driverObj,              // Transmit data routine
                             unsigned char (*driverFunc)(void*, void*, unsigned char));
        unsigned char (*receiveData)(HmiProtocolObj *obj, void *driverObj,               // Receive data routine
                            unsigned char (*driverFunc)(void*, void**, unsigned char*));
    }func;
};

typedef struct {
    unsigned char (*unpackRxData)(HmiProtocolObj *obj, unsigned char slave_id, HmiStruct_t *data);
    void (*packTxData)(HmiProtocolObj *obj, unsigned char slave_id, 
                      unsigned char func_code, unsigned short start_addr,
                      unsigned short qty, unsigned char priority);
} HmiProtocolFuncStr;


/* Global function prototypes */
void Init_HMI_Protocol(HmiProtocolObj *obj, HmiCommandArrayType *command);
void Pack_HMI_Internal_TXD(HmiProtocolObj *obj, unsigned char id, unsigned char func, unsigned char addr, unsigned char length, unsigned char priority);
unsigned char Unpack_HMI_Internal_RXD(HmiProtocolObj *obj, unsigned char addr, HmiStruct_t *info);
void Process_HMI_Internal_RXD(HmiProtocolObj *obj, HmiDataPackStr data, unsigned char addr, HmiStruct_t *info);
void Transmit_HMI_Internal_Data(HmiProtocolObj *obj, void *driverObj, unsigned char (*driverFunc)(void*, void*, unsigned char));
unsigned char Receive_HMI_Internal_Data(HmiProtocolObj *obj, void *driverObj, unsigned char (*driverFunc)(void*, void**, unsigned char*));
unsigned long Calculate_HMI_CRC(unsigned char* buffer, unsigned short length);
unsigned char process_rx_data(HmiProtocolObj *obj, unsigned char addr, HmiStruct_t *info) ;
unsigned char HMI_Encoder(HmiProtocolObj *obj, HmiBufferStr *buffer, HmiDataStr data);
void HMI_Send_LED_Frame(HmiProtocolObj *obj, uint8_t slave_id, uint16_t start_addr, uint16_t channel, uint16_t brightness, uint16_t on_off, uint8_t mode);
void process_device_id(HmiProtocolObj *obj, uint16_t bytes_received, uint8_t device_index);
void process_channel_status(HmiProtocolObj *obj, uint16_t bytes_received, uint8_t *pConfig_frame, uint8_t device_index);
void process_led_config_response(HmiProtocolObj *obj, uint16_t bytes_received, uint8_t *pConfig_frame, uint8_t device_index);

/* Global data declarations */
extern HmiProtocolObj hmiProtocol;
extern HmiCommandArrayType hmiCommand;
extern HmiStruct_t hmi_regs;
#endif
