/***********************************************************************************************************************
File Name: air_driver.h
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

#ifndef __AIR_DRIVER_H__
#define __AIR_DRIVER_H__

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
#define Create_AIR_Command(Name, \
                           List_Length, \
                           Index_Length)        extern AirCommandInfoStr *Name##Index[];\
                                                extern const AirCommandInfoStr Name##List[];\
                                                AirCommandArrayType Name = {\
                                                                            Name##List, List_Length,\
                                                                            Name##Index, Index_Length\
                                                                        };\
                                                AirCommandInfoStr *Name##Index[Check_Index(List_Length, Index_Length)];\
                                                const AirCommandInfoStr Name##List[List_Length]

// Group of declarations - Size MUST be power of 2 (2^n)
#define Create_AIR_Protocol_Object(ObjName, \
                                   Size)        Create_AIR_Protocol_Member(ObjName##Tx, ObjName##Rx, \
                                                                           Check_Power_2(Size));\
                                                AirProtocolObj ObjName = \
                                                {\
                                                    {\
                                                        &ObjName##TxLink, &ObjName##RxLink, \
                                                        &ObjName##TxQueue, &ObjName##RxQueue, 0, 0\
                                                    }, \
                                                    {\
                                                        Process_AIR_Internal_RXD, \
                                                        Calculate_AIR_CRC\
                                                    }, \
                                                    {\
                                                        Init_AIR_Protocol, \
                                                        Pack_AIR_Internal_TXD, \
                                                        Unpack_AIR_Internal_RXD, \
                                                        Transmit_AIR_Internal_Data, \
                                                        Receive_AIR_Internal_Data\
                                                    }\
                                                }

#define Create_AIR_Protocol_Member(TxName, \
                                   RxName, \
                                   Size)        Create_AIR_Register(TxName##Link, Size);\
                                                Create_AIR_Register(RxName##Link, Size);\
                                                Create_Priority_Queue(TxName##Queue, Size, Size);\
                                                Create_Circular_Queue(RxName##Queue, Size)

#define Create_AIR_Register(Name, Size)         AirBufferStr Name##Buffer[Size];\
                                                AirDataRegStr Name = {\
                                                                     Name##Buffer,\
                                                                     sizeof(Name##Buffer) / sizeof(AirBufferStr),\
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

#define AIR_PM2P5                               0x00
#define AIR_PM10                                0x01
#define AIR_HUMIDITY                            0x02
#define AIR_TEMPERATURE                         0x03
#define AIR_LIGHT_INTENSITY                     0x05
#define AIR_CO2                                 0x08

#define READ_HOLDING_REGISTERS                  0x03
#define WRITE_SINGLE_REGISTER                   0x06    
#define WRITE_MULTIPLE_REGISTER                 0x10
#define PACKAGE_SIZE                            32                                                    

/* Type definitions */
// Enumeration
// Struct & Union
typedef struct
{
    uint16_t meter_PM_2P5;
    uint16_t meter_PM_10;
    uint16_t meter_humidity;
     int16_t meter_temperature;
    uint32_t meter_light_intensity;
    uint16_t meter_CO2;
} AirStruct_t;

typedef struct
{
    unsigned char size;
    unsigned long offset;
    unsigned char code[PACKAGE_SIZE];
}AirBufferStr;

typedef struct
{
    void *ptr;
    unsigned char id;
    unsigned char func;
    unsigned char addr;
    unsigned long length;
}AirDataStr;

typedef struct
{
    void *ptr;
    unsigned long size;
}AirDataPackStr;

typedef struct
{
	unsigned char code;
	void *ptr;
	unsigned long size;
}AirCommandInfoStr;

typedef struct
{
    const AirCommandInfoStr *list;
    unsigned long listLength;
    AirCommandInfoStr **index;
    unsigned long indexLength;
}AirCommandArrayType;

typedef struct
{
    void *buffer;
    unsigned char length;
    unsigned char index;
}AirDataRegStr;

typedef struct AirProtocolStr AirProtocolObj;

struct AirProtocolStr
{
    struct
    {
        AirDataRegStr *txData;             // Transmit data register pointer
        AirDataRegStr *rxData;             // Receive data register pointer
        PriorQueueRegStr *txQueue;      // Transmit pointer queue
        QueueRegStr *rxQueue;           // Receive pointer queue
        AirCommandInfoStr **listIndex;     // Pointer of the command table index
        unsigned long indexLength;      // Command table index length
    }member;

    struct
    {
        void (*rxDataProcess)(AirProtocolObj*, AirDataPackStr, unsigned char, AirStruct_t*); // Process method of received data
        unsigned long (*calculateChecksum)(unsigned char*, unsigned short);   // Method of the checksum calculation
    }method;

    struct
    {
        void (*init)(AirProtocolObj*, AirCommandArrayType*);                       // Initialization of command index
        void (*packTxData)(AirProtocolObj*,                                     // Pack transmit data of the command
                           unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
        unsigned char (*unpackRxData)(AirProtocolObj*, unsigned char, AirStruct_t*);          // Unpack received data of all commands
        void (*transmitData)(AirProtocolObj *obj, void *driverObj,              // Transmit data routine
                             unsigned char (*driverFunc)(void*, void*, unsigned char));
        unsigned char (*receiveData)(AirProtocolObj *obj, void *driverObj,               // Receive data routine
                            unsigned char (*driverFunc)(void*, void**, unsigned char*));
    }func;
};


/* Global function prototypes */
void Init_AIR_Protocol(AirProtocolObj *obj, AirCommandArrayType *command);
void Pack_AIR_Internal_TXD(AirProtocolObj *obj, unsigned char id, unsigned char func, unsigned char addr, unsigned char length, unsigned char priority);
unsigned char Unpack_AIR_Internal_RXD(AirProtocolObj *obj, unsigned char addr, AirStruct_t *info);
void Process_AIR_Internal_RXD(AirProtocolObj *obj, AirDataPackStr data, unsigned char addr, AirStruct_t *info);
void Transmit_AIR_Internal_Data(AirProtocolObj *obj, void *driverObj, unsigned char (*driverFunc)(void*, void*, unsigned char));
unsigned char Receive_AIR_Internal_Data(AirProtocolObj *obj, void *driverObj, unsigned char (*driverFunc)(void*, void**, unsigned char*));
unsigned long Calculate_AIR_CRC(unsigned char* buffer, unsigned short length);

/* Global data declarations */
extern AirProtocolObj airProtocol;
extern AirCommandArrayType airCommand;
extern AirStruct_t air_regs;
#endif
