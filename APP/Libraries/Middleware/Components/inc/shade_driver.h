/***********************************************************************************************************************
File Name: shade_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    02/03/2025  Wilson Chen      Create file

***********************************************************************************************************************/

#ifndef __SHADE_DRIVER_H__
#define __SHADE_DRIVER_H__

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
#define Create_SHADE_Command(Name, \
                           List_Length, \
                           Index_Length)        extern ShadeCommandInfoStr *Name##Index[];\
                                                extern const ShadeCommandInfoStr Name##List[];\
                                                ShadeCommandArrayType Name = {\
                                                                            Name##List, List_Length,\
                                                                            Name##Index, Index_Length\
                                                                        };\
                                                ShadeCommandInfoStr *Name##Index[Check_Index(List_Length, Index_Length)];\
                                                const ShadeCommandInfoStr Name##List[List_Length]

// Group of declarations - Size MUST be power of 2 (2^n)
#define Create_SHADE_Protocol_Object(ObjName, \
                                   Size)        Create_SHADE_Protocol_Member(ObjName##Tx, ObjName##Rx, \
                                                                           Check_Power_2(Size));\
                                                ShadeProtocolObj ObjName = \
                                                {\
                                                    {\
                                                        &ObjName##TxLink, &ObjName##RxLink, \
                                                        &ObjName##TxQueue, &ObjName##RxQueue, 0, 0\
                                                    }, \
                                                    {\
                                                        Process_Shade_Internal_RXD, \
                                                        Calculate_SHADE_INTP_Checksum\
                                                    }, \
                                                    {\
                                                        Init_SHADE_Protocol, \
                                                        Pack_Shade_Internal_TXD, \
                                                        Unpack_Shade_Internal_RXD, \
                                                        Transmit_Shade_Internal_Data, \
                                                        Receive_Shade_Internal_Data\
                                                    }\
                                                }

#define Create_SHADE_Protocol_Member(TxName, \
                                   RxName, \
                                   Size)        Create_SAHDE_Register(TxName##Link, Size);\
                                                Create_SAHDE_Register(RxName##Link, Size);\
                                                Create_Priority_Queue(TxName##Queue, Size, Size);\
                                                Create_Circular_Queue(RxName##Queue, Size)
                                                
#define Create_SAHDE_Register(Name, Size)       ShadeBufferStr Name##Buffer[Size];\
                                                ShadeDataRegStr Name = {\
                                                                     Name##Buffer,\
                                                                     sizeof(Name##Buffer) / sizeof(ShadeBufferStr),\
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
                                                
#define SHADE_UP                   0x0ADD
#define SHADE_STOP                 0x0ACC
#define SHADE_DOWN                 0x0AEE
#define SHADE_MIDDLE_LIMIT_1       0x0A01
#define SHADE_MIDDLE_LIMIT_2       0x0A02
#define SHADE_MIDDLE_LIMIT_3       0x0A03
#define SHADE_MIDDLE_LIMIT_4       0x0A04
#define SHADE_UP_LIMIT_SELECT      0xDBDD
#define SHADE_MIDDLE_LIMIT_SELECT  0xDBCC
#define SHADE_LOW_LIMIT_SELECT     0xDBEE
#define SHADE_DELETE_ALL_LIMITS    0xDB00
#define SHADE_ID_SELECT            0xAAAA
#define SHADE_STATUS_INQUIRY       0xCC00
                                                
#define SHADE_PACKAGE_SIZE         10
                                                
/* Type definitions */  
// Enumeration
// Struct & Union
                                                
typedef struct
{
    uint8_t  position;
} ShadeStruct_t;

typedef struct
{
    unsigned char size;
    unsigned long offset;
    unsigned char code[SHADE_PACKAGE_SIZE];
}ShadeBufferStr;

typedef struct  
{   
    void *ptr;
    unsigned char id;
    unsigned short func;
    unsigned long length; 
}ShadeDataStr;

typedef struct  
{   
    void *ptr;
    unsigned long size; 
}ShadeDataPackStr;

typedef struct
{
	unsigned short code;
	void *ptr;
	unsigned long size;
}ShadeCommandInfoStr;

typedef struct
{
    const ShadeCommandInfoStr *list;
    unsigned long listLength;
    ShadeCommandInfoStr **index;     
    unsigned long indexLength; 
}ShadeCommandArrayType;

typedef struct
{
    void *buffer;
    unsigned char length;
    unsigned char index;
}ShadeDataRegStr;

typedef struct ShadeProtocolStr ShadeProtocolObj;  

struct ShadeProtocolStr
{
    struct
    {
        ShadeDataRegStr *txData;             // Transmit data register pointer
        ShadeDataRegStr *rxData;             // Receive data register pointer
        PriorQueueRegStr *txQueue;      // Transmit pointer queue
        QueueRegStr *rxQueue;           // Receive pointer queue
        ShadeCommandInfoStr **listIndex;     // Pointer of the command table index
        unsigned long indexLength;      // Command table index length
    }member;

    struct 
    {
        void (*rxDataProcess)(ShadeProtocolObj*, ShadeDataPackStr, ShadeStruct_t *);  // Process method of received data 
        unsigned long (*calculateChecksum)(unsigned char*, unsigned short);   // Method of the checksum calculation
    }method; 

    struct 
    {
        void (*init)(ShadeProtocolObj*, ShadeCommandArrayType*);                       // Initialization of command index    
        void (*packTxData)(ShadeProtocolObj*,                                     // Pack transmit data of the command
                           unsigned char, unsigned short, unsigned char);
        void (*unpackRxData)(ShadeProtocolObj*, ShadeStruct_t *);                                  // Unpack received data of all commands
        void (*transmitData)(ShadeProtocolObj *obj, void *driverObj,              // Transmit data routine
                             unsigned char (*driverFunc)(void*, void*, unsigned char)); 
        unsigned char (*receiveData)(ShadeProtocolObj *obj, void *driverObj,               // Receive data routine
                            unsigned char (*driverFunc)(void*, void**, unsigned char*));
    }func; 
};

/* Global function prototypes */
void Init_SHADE_Protocol(ShadeProtocolObj *obj, ShadeCommandArrayType *command);
void Pack_Shade_Internal_TXD(ShadeProtocolObj *obj, unsigned char id, unsigned short func, unsigned char priority);
void Unpack_Shade_Internal_RXD(ShadeProtocolObj *obj, ShadeStruct_t *info);
void Process_Shade_Internal_RXD(ShadeProtocolObj *obj, ShadeDataPackStr data, ShadeStruct_t *info);
unsigned char Write_SHADE_INTP_Not_Packet_Data(ShadeProtocolObj *obj, ShadeDataPackStr data);
void Transmit_Shade_Internal_Data(ShadeProtocolObj *obj, void *driverObj,
                            unsigned char (*driverFunc)(void*, void*, unsigned char));
unsigned char Receive_Shade_Internal_Data(ShadeProtocolObj *obj, void *driverObj, 
                           unsigned char (*driverFunc)(void*, void**, unsigned char*));
unsigned long Calculate_SHADE_INTP_Checksum(unsigned char* buffer, unsigned short length);

/* Global data declarations */
extern ShadeProtocolObj shadeProtocol;
extern ShadeCommandArrayType shadeCommand;

#endif
