/***********************************************************************************************************************
File Name: light_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    07/30/2025  Wilson Chen      Create file

***********************************************************************************************************************/

#ifndef __LIGHT_DRIVER_H__
#define __LIGHT_DRIVER_H__

/* Includes */
#include "circular_buffer.h"

/* Macro definitions */
// Statement Replacement
#ifndef Check_Power_2
#define Check_Power_2(Number)               (((((unsigned)Number - 1) ^ Number) == (Number << 1) - 1) ? Number : -1)
#endif

#ifndef Check_Index
#define Check_Index(List_Length, Index_Length)  (Index_Length * (Index_Length >= List_Length))
#endif

// Group of declarations - Command index length must greater than command list length
#define Create_LIGHT_Command(Name, \
                           List_Length, \
                           Index_Length)        extern LightCommandInfoStr *Name##Index[];\
                                                extern const LightCommandInfoStr Name##List[];\
                                                LightCommandArrayType Name = {\
                                                                            Name##List, List_Length,\
                                                                            Name##Index, Index_Length\
                                                                        };\
                                                LightCommandInfoStr *Name##Index[Check_Index(List_Length, Index_Length)];\
                                                const LightCommandInfoStr Name##List[List_Length]

// Group of declarations - Size MUST be power of 2 (2^n)
#define Create_LIGHT_Protocol_Object(ObjName, \
                                   Size)        Create_LIGHT_Protocol_Member(ObjName##Tx, ObjName##Rx, \
                                                                           Check_Power_2(Size));\
                                                LightProtocolObj ObjName = \
                                                {\
                                                    {\
                                                        &ObjName##TxLink, &ObjName##RxLink, \
                                                        &ObjName##TxQueue, &ObjName##RxQueue, 0, 0\
                                                    }, \
                                                    {\
                                                        Process_LIGHT_Internal_RXD, \
                                                        Calculate_LIGHT_CRC\
                                                    }, \
                                                    {\
                                                        Init_LIGHT_Protocol, \
                                                        Pack_LIGHT_Internal_TXD, \
                                                        Unpack_LIGHT_Internal_RXD, \
                                                        Transmit_LIGHT_Internal_Data, \
                                                        Receive_LIGHT_Internal_Data\
                                                    }\
                                                }

#define Create_LIGHT_Protocol_Member(TxName, \
                                   RxName, \
                                   Size)        Create_LIGHT_Register(TxName##Link, Size);\
                                                Create_LIGHT_Register(RxName##Link, Size);\
                                                Create_Priority_Queue(TxName##Queue, Size, Size);\
                                                Create_Circular_Queue(RxName##Queue, Size)

#define Create_LIGHT_Register(Name, Size)       LightBufferStr Name##Buffer[Size];\
                                                LightDataRegStr Name = {\
                                                                     Name##Buffer,\
                                                                     sizeof(Name##Buffer) / sizeof(LightBufferStr),\
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

#define LIGHT_TEMPERATURE                       0x00
#define LIGHT_HUMIDITY                          0x01
#define LIGHT_INTENSITY                         0x06
                                                
#define READ_HOLDING_REGISTERS                  0x03
#define WRITE_SINGLE_REGISTER                   0x06

#define PACKAGE_SIZE                            32                                                    

/* Type definitions */
// Enumeration
// Struct & Union
typedef struct
{
     int16_t light_temperature;
    uint16_t light_humidity;
    uint16_t light_intensity;
} LightStruct_t;

typedef struct
{
    unsigned char size;
    unsigned long offset;
    unsigned char code[PACKAGE_SIZE];
}LightBufferStr;

typedef struct
{
    void *ptr;
    unsigned char id;
    unsigned char func;
    unsigned char addr;
    unsigned long length;
}LightDataStr;

typedef struct
{
    void *ptr;
    unsigned long size;
}LightDataPackStr;

typedef struct
{
	unsigned char code;
	void *ptr;
	unsigned long size;
}LightCommandInfoStr;

typedef struct
{
    const LightCommandInfoStr *list;
    unsigned long listLength;
    LightCommandInfoStr **index;
    unsigned long indexLength;
}LightCommandArrayType;

typedef struct
{
    void *buffer;
    unsigned char length;
    unsigned char index;
}LightDataRegStr;

typedef struct LightProtocolStr LightProtocolObj;

struct LightProtocolStr
{
    struct
    {
        LightDataRegStr *txData;             // Transmit data register pointer
        LightDataRegStr *rxData;             // Receive data register pointer
        PriorQueueRegStr *txQueue;      // Transmit pointer queue
        QueueRegStr *rxQueue;           // Receive pointer queue
        LightCommandInfoStr **listIndex;     // Pointer of the command table index
        unsigned long indexLength;      // Command table index length
    }member;

    struct
    {
        void (*rxDataProcess)(LightProtocolObj*, LightDataPackStr, unsigned char, LightStruct_t*); // Process method of received data
        unsigned long (*calculateChecksum)(unsigned char*, unsigned short);   // Method of the checksum calculation
    }method;

    struct
    {
        void (*init)(LightProtocolObj*, LightCommandArrayType*);                       // Initialization of command index
        void (*packTxData)(LightProtocolObj*,                                     // Pack transmit data of the command
                           unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
        unsigned char (*unpackRxData)(LightProtocolObj*, unsigned char, LightStruct_t*);          // Unpack received data of all commands
        void (*transmitData)(LightProtocolObj *obj, void *driverObj,              // Transmit data routine
                             unsigned char (*driverFunc)(void*, void*, unsigned char));
        unsigned char (*receiveData)(LightProtocolObj *obj, void *driverObj,               // Receive data routine
                            unsigned char (*driverFunc)(void*, void**, unsigned char*));
    }func;
};


/* Global function prototypes */
void Init_LIGHT_Protocol(LightProtocolObj *obj, LightCommandArrayType *command);
void Pack_LIGHT_Internal_TXD(LightProtocolObj *obj, unsigned char id, unsigned char func, unsigned char addr, unsigned char length, unsigned char priority);
unsigned char Unpack_LIGHT_Internal_RXD(LightProtocolObj *obj, unsigned char addr, LightStruct_t *info);
void Process_LIGHT_Internal_RXD(LightProtocolObj *obj, LightDataPackStr data, unsigned char addr, LightStruct_t *info);
void Transmit_LIGHT_Internal_Data(LightProtocolObj *obj, void *driverObj, unsigned char (*driverFunc)(void*, void*, unsigned char));
unsigned char Receive_LIGHT_Internal_Data(LightProtocolObj *obj, void *driverObj, unsigned char (*driverFunc)(void*, void**, unsigned char*));
unsigned long Calculate_LIGHT_CRC(unsigned char* buffer, unsigned short length);

/* Global data declarations */
extern LightProtocolObj lightProtocol;
extern LightCommandArrayType lightCommand;
extern LightStruct_t light_regs;
#endif
