/***********************************************************************************************************************
File Name: circular_buffer.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------
    None
Description:
    Header file of CircularBuffer.c.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/07/2025  Fred Huang      Create file

***********************************************************************************************************************/

#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "base_type.h"

//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define Priority_Flow_Limit     5
#define Write_Queue             0   // Write direction flag
#define Read_Queue              1   // Read direction flag

//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration
typedef enum
{
    LOW_PRIORITY    = 0,
    HIGH_PRIORITY   = 1
} QueuePriorityType;

typedef enum
{
    DATA_WRITE      = 0,
    DATA_READ       = 1
} QueueDirectionType;

// Struct & Union
typedef struct
{
    void *ptr;
    u16 size;
    u8 direction;           // Queue direction flag, reserved for the half-duplex mode.
} QueueDataStr;

typedef struct
{
    u16 enqueue;
    u16 dequeue;
    u8 full;
    u8 empty;
    u8 freeze;
    QueueDataStr *data;
    u32 length;             // MUST be power of 2 (2^n)
} QueueRegStr;              // Structure of common queue register

typedef struct
{
    QueueRegStr lowPriority;
    QueueRegStr highPriority;
    u8 priorityFlow;
} PriorQueueRegStr;         // Structure of priority queue register

//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
void Priority_Buffer_Enqueue(PriorQueueRegStr *reg, QueueDataStr data, u8 priority);
void Common_Buffer_Enqueue(QueueRegStr *reg, QueueDataStr data);
u8 Priority_Buffer_Dequeue(PriorQueueRegStr *reg, QueueDataStr *data);
u8 Common_Buffer_Dequeue(QueueRegStr *reg, QueueDataStr *dataPtr);

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
#endif /* _CIRCULAR_BUFFER_H_ */
