/***********************************************************************************************************************
File Name: circular_buffer.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------
    None.
External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------
    None.
Description:
    Universal circular buffer library program. The circular buffer is a FIFO buffer for asynchronous
data transmission.

Block diagram:
               Memory space                   Queue data                  Queue register
                ___________           __________________________          _______________
               |           |         |                          |        |       |       |
 Push data0--->|  [data0]  |-------->|Save data address and size|------->|       |       |
               |           |\        |__________________________|        |       |       |
               |-----------| \        __________________________         |Enqueue|Dequeue|
               |           |  \      |                          |        |       |       |
 Push data1--->|  [data1]  |---)---->|Save data address and size|------->|       |       |
               |___________|  |      |__________________________|        |_______|_______|
                ___________   |       __________________________                     |
               |           |<-*      |                          |                    |
               |  [data0]  |<--------|Load data address and size|<-------------------*
               |___________|Pop data0|__________________________|

Use case:
    char mSpace[4][8];
    QueueDataStr queueData[4];
    QueueRegStr queueReg = {0, 0, 0, 0, 0, queueData, 4};
    ...
    ..
    .
    void Push()
    {
        QueueDataStr enQueue = {&mSpace[queueReg.enqueue][0], 8, 0};

        // Push data to mSpace

        Common_Buffer_Enqueue(&queueReg, enQueue);
    }
    void Pop()
    {
        QueueDataStr deQueue;

        if (Common_Buffer_Dequeue(&queueReg, deQueue))
        {
            // Pop data form deQueue
        }
    }
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/07/2025  Fred Huang      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "..\inc\circular_buffer.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Priority_Buffer_Enqueue(PriorQueueRegStr *reg, QueueDataStr data, u8 priority)
Input:
    *reg     - Registers of priority circular buffer.
    data     - Inserted data, the function will sequentially enqueue the data to a circular
               buffer, the data contains an array size, a flag of transfer direction, and
               a pointer which points an array.
    priority - Queue priority. 0 = lower priority, 1 = higher priority.
Output:
    None.
Comment:
    Priority buffer enqueue function. This function has a priority variable to control the order
of dequeuing.
**************************************************************************************************/
void Priority_Buffer_Enqueue(PriorQueueRegStr *reg, QueueDataStr data, u8 priority)
{
    u16 highUsed = ((reg->highPriority.enqueue < reg->highPriority.dequeue) * reg->highPriority.length +
                    reg->highPriority.enqueue - reg->highPriority.dequeue) & (reg->highPriority.length - 1);

    u16 lowUsed = ((reg->lowPriority.enqueue < reg->lowPriority.dequeue) * reg->lowPriority.length +
                   reg->lowPriority.enqueue - reg->lowPriority.dequeue) & (reg->lowPriority.length - 1);

    u16 totalUsed = lowUsed + highUsed;

    if (priority)
    {
        Common_Buffer_Enqueue(&reg->highPriority, data);
    }
    else
    {
        Common_Buffer_Enqueue(&reg->lowPriority, data);
    }

//    if ((totalUsed + 1) >= reg->highPriority.length || (totalUsed + 1) >= reg->lowPriority.length)
//    {
//        reg->highPriority.full = 1;
//        reg->lowPriority.full = 1;
//    }
    if ((totalUsed + 1) >= reg->highPriority.length)
    {
        reg->highPriority.full = 1;
    }
    if ((totalUsed + 1) >= reg->lowPriority.length)
    {
        reg->lowPriority.full = 1;
    }
}
/**************************************************************************************************
Function Name:
    void Common_Buffer_Enqueue(QueueRegStr *reg, QueueDataStr data)
Input:
    *reg     - Registers of common circular buffer.
    data     - Inserted data, the function will sequentially enqueue the data to a circular
               buffer, the data contains an array size, a flag of transfer direction, and
               a pointer which points an array.
Output:
    None.
Comment:
    Common circular buffer enqueue function.
**************************************************************************************************/
void Common_Buffer_Enqueue(QueueRegStr *reg, QueueDataStr data)
{
    u16 mapping = (reg->enqueue + 1) & ((reg->length << 1) - 1);
    u16 i = mapping & (reg->length - 1);

    if (i == reg->dequeue || reg->full)
    {
        reg->full = 1;
    }
    else
    {
        reg->data[i].direction = data.direction;
        reg->data[i].ptr = data.ptr;
        reg->data[i].size = data.size;
        reg->enqueue = i;
    }
    reg->empty = 0;
}
/**************************************************************************************************
Function Name:
    u8 Priority_Buffer_Dequeue(PriorQueueRegStr *reg, QueueDataStr *data)
Input:
    *reg     - Registers of priority circular buffer.
    *dataPtr - Data pointer, the function will dequeue an array size, a flag of transfer
               direction, and an array pointer to the specified address which the pointer points
               to.
Output:
    return   - Result. 1 = an inserted data dequeued, 0 = queue is empty.
Comment:
    Priority buffer dequeue function. This function will dequeue the higher priority queue first,
the lower priority queue operates when the higher priority queue is empty or continuously
dequeuing over the priority flow limit.
**************************************************************************************************/
u8 Priority_Buffer_Dequeue(PriorQueueRegStr *reg, QueueDataStr *data)
{
    if (!reg->highPriority.empty && !reg->lowPriority.freeze &&
        (reg->priorityFlow < Priority_Flow_Limit || reg->highPriority.freeze))
    {
        if (!reg->highPriority.freeze)
        {
            ++reg->priorityFlow;
        }

        if (Common_Buffer_Dequeue(&reg->highPriority, data))
        {
            reg->lowPriority.full = 0;
            return 1;
        }
    }
    else
    {
        reg->priorityFlow = 0;

        if (Common_Buffer_Dequeue(&reg->lowPriority, data))
        {
            reg->highPriority.full = 0;
            return 1;
        }
    }
    return 0;
}
/**************************************************************************************************
Function Name:
    u8 Common_Buffer_Dequeue(QueueRegStr *reg, QueueDataStr *dataPtr)
Input:
    *reg     - Registers of common circular buffer.
    *dataPtr - Data pointer, the function will dequeue an array size, a flag of transfer
               direction, and an array pointer to the specified address which the pointer points
               to.
Output:
    None.
Comment:
    Common circular buffer enqueue function.
**************************************************************************************************/
u8 Common_Buffer_Dequeue(QueueRegStr *reg, QueueDataStr *dataPtr)
{
    u16 mapping = (reg->dequeue + 1) & ((reg->length << 1) - 1);

    if (reg->dequeue == reg->enqueue && !reg->freeze)
    {
        reg->empty = 1;
        return 0;
    }
    else
    {
        if (!reg->freeze)
        {
            reg->dequeue = mapping & (reg->length - 1);
            reg->full = 0;
        }
        dataPtr->direction = reg->data[reg->dequeue].direction;
        dataPtr->ptr = reg->data[reg->dequeue].ptr;
        dataPtr->size = reg->data[reg->dequeue].size;
        return 1;
    }
}
//----------------------------------------------------------------------------------------------------------------------
