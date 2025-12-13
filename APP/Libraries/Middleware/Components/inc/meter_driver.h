/***********************************************************************************************************************
File Name: meter_driver.h
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

#ifndef __METER_DRIVER_H__
#define __METER_DRIVER_H__

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
#define Create_METER_Command(Name, \
                           List_Length, \
                           Index_Length)        extern MeterCommandInfoStr *Name##Index[];\
                                                extern const MeterCommandInfoStr Name##List[];\
                                                MeterCommandArrayType Name = {\
                                                                            Name##List, List_Length,\
                                                                            Name##Index, Index_Length\
                                                                        };\
                                                MeterCommandInfoStr *Name##Index[Check_Index(List_Length, Index_Length)];\
                                                const MeterCommandInfoStr Name##List[List_Length]

// Group of declarations - Size MUST be power of 2 (2^n)
#define Create_METER_Protocol_Object(ObjName, \
                                   Size)        Create_METER_Protocol_Member(ObjName##Tx, ObjName##Rx, \
                                                                           Check_Power_2(Size));\
                                                MeterProtocolObj ObjName = \
                                                {\
                                                    {\
                                                        &ObjName##TxLink, &ObjName##RxLink, \
                                                        &ObjName##TxQueue, &ObjName##RxQueue, 0, 0\
                                                    }, \
                                                    {\
                                                        Process_METER_Internal_RXD, \
                                                        Calculate_METER_CRC\
                                                    }, \
                                                    {\
                                                        Init_METER_Protocol, \
                                                        Pack_METER_Internal_TXD, \
                                                        Unpack_METER_Internal_RXD, \
                                                        Transmit_METER_Internal_Data, \
                                                        Receive_METER_Internal_Data\
                                                    }\
                                                }

#define Create_METER_Protocol_Member(TxName, \
                                   RxName, \
                                   Size)        Create_METER_Register(TxName##Link, Size);\
                                                Create_METER_Register(RxName##Link, Size);\
                                                Create_Priority_Queue(TxName##Queue, Size, Size);\
                                                Create_Circular_Queue(RxName##Queue, Size)

#define Create_METER_Register(Name, Size)         MeterBufferStr Name##Buffer[Size];\
                                                MeterDataRegStr Name = {\
                                                                     Name##Buffer,\
                                                                     sizeof(Name##Buffer) / sizeof(MeterBufferStr),\
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


#define METER_HUMIDITY                          500
#define METER_TEMPERATURE                       501
#define METER_PM2P5                             503
#define METER_PM10                              504
#define METER_LIGHT_INTENSITY                   506

#define READ_HOLDING_REGISTERS                  0x03
#define WRITE_SINGLE_REGISTER                   0x06

#define PACKAGE_SIZE                            32                                                    

/* Type definitions */
// Enumeration
// Struct & Union
typedef struct
{
    uint16_t meter_humidity;
     int16_t meter_temperature;
    uint16_t meter_PM_2P5;
    uint16_t meter_PM_10;
    uint32_t meter_light_intensity;
} MeterStruct_t;

typedef struct
{
    unsigned char size;
    unsigned long offset;
    unsigned char code[PACKAGE_SIZE];
}MeterBufferStr;

typedef struct
{
    void *ptr;
    unsigned char id;
    unsigned char func;
    unsigned short addr;
    unsigned long length;
}MeterDataStr;

typedef struct
{
    void *ptr;
    unsigned long size;
}MeterDataPackStr;

typedef struct
{
	unsigned char code;
	void *ptr;
	unsigned long size;
}MeterCommandInfoStr;

typedef struct
{
    const MeterCommandInfoStr *list;
    unsigned long listLength;
    MeterCommandInfoStr **index;
    unsigned long indexLength;
}MeterCommandArrayType;

typedef struct
{
    void *buffer;
    unsigned char length;
    unsigned char index;
}MeterDataRegStr;

typedef struct MeterProtocolStr MeterProtocolObj;

struct MeterProtocolStr
{
    struct
    {
        MeterDataRegStr *txData;             // Transmit data register pointer
        MeterDataRegStr *rxData;             // Receive data register pointer
        PriorQueueRegStr *txQueue;      // Transmit pointer queue
        QueueRegStr *rxQueue;           // Receive pointer queue
        MeterCommandInfoStr **listIndex;     // Pointer of the command table index
        unsigned long indexLength;      // Command table index length
    }member;

    struct
    {
        void (*rxDataProcess)(MeterProtocolObj*, MeterDataPackStr, unsigned short, MeterStruct_t*); // Process method of received data
        unsigned long (*calculateChecksum)(unsigned char*, unsigned short);   // Method of the checksum calculation
    }method;

    struct
    {
        void (*init)(MeterProtocolObj*, MeterCommandArrayType*);                       // Initialization of command index
        void (*packTxData)(MeterProtocolObj*,                                     // Pack transmit data of the command
                           unsigned char, unsigned char, unsigned short, unsigned char, unsigned char);
        unsigned char (*unpackRxData)(MeterProtocolObj*, unsigned short, MeterStruct_t*);          // Unpack received data of all commands
        void (*transmitData)(MeterProtocolObj *obj, void *driverObj,              // Transmit data routine
                             unsigned char (*driverFunc)(void*, void*, unsigned char));
        unsigned char (*receiveData)(MeterProtocolObj *obj, void *driverObj,               // Receive data routine
                            unsigned char (*driverFunc)(void*, void**, unsigned char*));
    }func;
};


/* Global function prototypes */
void Init_METER_Protocol(MeterProtocolObj *obj, MeterCommandArrayType *command);
void Pack_METER_Internal_TXD(MeterProtocolObj *obj, unsigned char id, unsigned char func, unsigned short addr, unsigned char length, unsigned char priority);
unsigned char Unpack_METER_Internal_RXD(MeterProtocolObj *obj, unsigned short addr, MeterStruct_t *info);
void Process_METER_Internal_RXD(MeterProtocolObj *obj, MeterDataPackStr data, unsigned short addr, MeterStruct_t *info);
void Transmit_METER_Internal_Data(MeterProtocolObj *obj, void *driverObj, unsigned char (*driverFunc)(void*, void*, unsigned char));
unsigned char Receive_METER_Internal_Data(MeterProtocolObj *obj, void *driverObj, unsigned char (*driverFunc)(void*, void**, unsigned char*));
unsigned long Calculate_METER_CRC(unsigned char* buffer, unsigned short length);

/* Global data declarations */
extern MeterProtocolObj meterProtocol;
extern MeterCommandArrayType meterCommand;
extern MeterStruct_t meter_regs;
#endif
