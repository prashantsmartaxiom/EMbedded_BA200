/***********************************************************************************************************************
File Name: shade_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    The driver shall create and fill in the required data in the driver's parameters before executing any function.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    02/03/2025  Wilson Chen      Create file

***********************************************************************************************************************/

/* Includes */
#include "shade_driver.h"

/* Local function prototypes */
unsigned char Internal_Shade_RXD_Error_Detection(ShadeProtocolObj *obj, ShadeDataPackStr data);
unsigned char SHADE_Decoder(ShadeBufferStr *buffer, ShadeDataPackStr *data);
unsigned char Locate_SHADE_Packet(ShadeBufferStr *buffer, ShadeDataPackStr data);
ShadeStruct_t shade_regs = {0};

Create_SHADE_Command(shadeCommand, 13, 16) =
{
    {                 SHADE_UP,                 NULL,  0},
    {               SHADE_STOP,                 NULL,  0},
    {               SHADE_DOWN,                 NULL,  0},
    {     SHADE_MIDDLE_LIMIT_1,                 NULL,  0},
    {     SHADE_MIDDLE_LIMIT_2,                 NULL,  0},
    {     SHADE_MIDDLE_LIMIT_3,                 NULL,  0},
    {     SHADE_MIDDLE_LIMIT_4,                 NULL,  0},
    {    SHADE_UP_LIMIT_SELECT,                 NULL,  0},
    {SHADE_MIDDLE_LIMIT_SELECT,                 NULL,  0},
    {   SHADE_LOW_LIMIT_SELECT,                 NULL,  0},
    {  SHADE_DELETE_ALL_LIMITS,                 NULL,  0},
    {          SHADE_ID_SELECT,                 NULL,  0},
    {     SHADE_STATUS_INQUIRY, &shade_regs.position,  1}
};

Create_SHADE_Protocol_Object(shadeProtocol, 32);
/**************************************************************************************************
Function Name:
    void Init_SHADE_Protocol(ShadeProtocolObj *obj, ShadeCommandArrayType *command)
Input:
    *obj     - Object pointer of internal protocol.
    *command - Command array. It consists of a list pointer, the list length, an index pointer,
               and the index length.
Output:
    None.
Comment:
    Init commmand index function. This function will configure the command index and update
"listIndex" and "indexLength" in the internal protocol object.
**************************************************************************************************/
void Init_SHADE_Protocol(ShadeProtocolObj *obj, ShadeCommandArrayType *command)
{
    int i;

    for (i = 0; i < command->indexLength; i++)
    {
        command->index[i] = 0;
    }

    for (i = 0; i < command->listLength; i++)
    {
        if (command->list[i].code < command->indexLength)
        {
            command->index[command->list[i].code] =(ShadeCommandInfoStr*)&command->list[i];
        }
    }

    obj->member.listIndex = command->index;
    obj->member.indexLength = command->indexLength;
}
/**************************************************************************************************
Function Name:
    unsigned char SHADE_Encoder(ShadeBufferStr *buffer, ShadeDataStr data)
Input:
    *buffer     - SHADE packet buffer.
    data        - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = data has encoded and updated to buffer, 0 = error occured.
Comment:
    Data encode to SHADE packet function.
**************************************************************************************************/
unsigned char SHADE_Encoder(ShadeProtocolObj *obj, ShadeBufferStr *buffer, ShadeDataStr data)
{
    buffer->code[0] = 0x9A;
    buffer->code[1] = data.id;
    buffer->code[2] = 0x01;
    buffer->code[3] = 0x00;
    buffer->code[4] = (data.func >> 8) & 0xFF;
    buffer->code[5] = data.func & 0xFF;
    
    buffer->size = 6;
    
    unsigned char checksum = (unsigned char)obj->method.calculateChecksum(buffer->code, buffer->size);
    buffer->code[buffer->size++] = checksum;

    return 1;
}
/**************************************************************************************************
Function Name:
    void Pack_Internal_TXD(ShadeProtocolObj *obj, unsigned char command, InternalCommandType type, unsigned char priority)
Input:
    *obj     - Object pointer of internal protocol.
    command  - Command code of internal protocol.
    type     - Command type of internal protocol.
    priority - Enqueue priority. 0 = lower priority, 1 = higher priority.
Output:
    None.
Comment:
    Pack the transmit data of internal protocol function. This function will encode and enqueue
the transmit data.
**************************************************************************************************/
void Pack_Shade_Internal_TXD(ShadeProtocolObj *obj, unsigned char id, unsigned short func, unsigned char priority)
{
    unsigned char buffer[SHADE_PACKAGE_SIZE] = {0};
    
    ShadeDataStr txData = {buffer, id, func};
    ShadeBufferStr *shadeRegPtr = (ShadeBufferStr*)obj->member.txData->buffer + obj->member.txData->index;
    
    if (!obj->member.txQueue->lowPriority.full && !obj->member.txQueue->highPriority.full &&
        SHADE_Encoder(obj, shadeRegPtr, txData))
    {
        obj->member.txData->index = (obj->member.txData->index + 1) & (obj->member.txData->length - 1);
        QueueDataStr enqueueData = {shadeRegPtr->code, shadeRegPtr->size};
        Priority_Buffer_Enqueue(obj->member.txQueue, enqueueData, priority);
    }
}
/**************************************************************************************************
Function Name:
    unsigned char SHADE_Decoder(ShadeBufferStr *buffer, ShadeDataStr *data)
Input:
    *buffer     - SHADE packet buffer.
    *data       - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = SHADE packet has decoded and updated to the data space that is the
                  data pointer points to, 0 = error occured.
Comment:
    SHADE packet decoding function.
**************************************************************************************************/
unsigned char SHADE_Decoder(ShadeBufferStr *buffer, ShadeDataPackStr *data)
{
    int i = 0, j = 0;
    
    if (buffer->size < 7)
    {
        return 0;
    }

    while (i < buffer->size)
    {
        *((unsigned char*)data->ptr + j) = buffer->code[i];
        ++j;
        ++i;
    }

    data->size = j;

    buffer->size = 0;
    buffer->offset = 0;

    return 1;
}
/**************************************************************************************************
Function Name:
    void Unpack_Internal_RXD(ShadeProtocolObj *obj)
Input:
    *obj      - Object pointer of internal protocol.
Output:
    None.
Comment:
    Unpack received data of internal protocol function. This function will dequeue, decode, verify
checksum and data size. The data process of internal protocol will be executed if the data is
correct.
**************************************************************************************************/
void Unpack_Shade_Internal_RXD(ShadeProtocolObj *obj, ShadeStruct_t *info)
{
    QueueDataStr dequeueData;

    if (Common_Buffer_Dequeue(obj->member.rxQueue, &dequeueData))
    {
        // Decode Rx data
        unsigned char buffer[SHADE_PACKAGE_SIZE] = {0};
        
        ShadeDataPackStr rxData = {buffer, 0};

        if (SHADE_Decoder((ShadeBufferStr*)dequeueData.ptr, (ShadeDataPackStr*)&rxData))
        {
            // Check Rx data checksum and size
            if (Internal_Shade_RXD_Error_Detection(obj, rxData))
            {
                // Process Rx data
                obj->method.rxDataProcess(obj, rxData, info);
            }
        }
    }
}
/**************************************************************************************************
Function Name:
    unsigned char Internal_RXD_Error_Detection(ShadeProtocolObj *obj, ShadeDataStr data)
Input:
    *obj      - Object pointer of internal protocol.
    data      - Structure of SHADE data packet. The structure includes both a data pointer and its
                size. The data pointer of the structure points to a decoded data.
Output:
    return    - Result. 0 = checksum or size incorrect, 1 = checksum and size correct.
Comment:
    Error detection of internal protocol. This function will detect the checksum and size of
received data.
**************************************************************************************************/
unsigned char Internal_Shade_RXD_Error_Detection(ShadeProtocolObj *obj, ShadeDataPackStr data)
{
    unsigned char *buffer = (unsigned char*)data.ptr;
    unsigned short received_checksum = buffer[data.size - 1];
    unsigned short calculated_checksum = obj->method.calculateChecksum(buffer, data.size - 1);
    
    if (buffer[0] != 0xD8 || data.size < 8)
    {
        return 0;
    }

    if (calculated_checksum == received_checksum)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/**************************************************************************************************
Function Name:
    void Process_Internal_RXD(ShadeProtocolObj *obj, ShadeDataStr data)
Input:
    *obj      - Object pointer of internal protocol.
    data      - Structure of SHADE data packet. The structure includes both a data pointer and its
                size. The data pointer of the structure points to a decoded data.
Output:
    None.
Comment:
    Data process of internal protocol function. The process details as follows:
    1. Update command data and clear its noAck flag if it receives a internal set or reply
       command.
    2. Send ACK if it receives a internal set command.
    3. Send command data if it receives a internal get command.
    4. Clear its noAck flag if it receives a internal ACK command.
**************************************************************************************************/
void Process_Shade_Internal_RXD(ShadeProtocolObj *obj, ShadeDataPackStr data, ShadeStruct_t *info)
{
    unsigned char *buffer = (unsigned char*)data.ptr;

    info->position = buffer[7];
}

/**************************************************************************************************
Function Name:
    void Transmit_Internal_Data(ShadeProtocolObj *obj, void *driverObj,
                                unsigned char (*driverFunc)(void*, unsigned char*, unsigned char))
Input:
    *obj        - Object pointer of internal protocol.
    *driverObj  - Object pointer of transmit driver.
    *driverFunc - Function pointer of transmit function. The transmit function shall includes two
                  parameters and a return value. The first parameter is an object pointer, the
                  second parameter is a pointer points to a data array, and the third parameter
                  is the size of the data array.
Output:
    None.
Comment:
    Transmit data of internal protocol function. This function will associate with a transmit
function of the peripheral module. The first argument is an address of the protocol object.
The second argument is an address of a transmission driver object. The third argument is an address
of the transmit function of the driver object.

The transmit function can get the transmit data and size from parameters of the transmit function.
**************************************************************************************************/
void Transmit_Shade_Internal_Data(ShadeProtocolObj *obj, void *driverObj,
                            unsigned char (*driverFunc)(void*, void*, unsigned char))
{
    // Dequeue Tx data
    QueueDataStr dequeueData;

    if (Priority_Buffer_Dequeue(obj->member.txQueue, &dequeueData))
    {
        if (obj->member.txQueue->priorityFlow > 0)
            obj->member.txQueue->highPriority.freeze = 1;
        else
            obj->member.txQueue->lowPriority.freeze = 1;

        // Reflash uart tx data register
        if (driverFunc(driverObj, dequeueData.ptr, dequeueData.size))
        {
            obj->member.txQueue->lowPriority.freeze = 0;
            obj->member.txQueue->highPriority.freeze = 0;
        }
    }
}
/**************************************************************************************************
Function Name:
    unsigned char Locate_SHADE_Packet(ShadeBufferStr *buffer, void *ptr, unsigned char size)
Input:
    *buffer     - SHADE packet buffer.
    data        - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = SHADE packet has located, 0 = Locating or stopped.
Comment:
    This function is used to locate a SHADE packet. This function will skip the locating if the
buffer has filled in a SHADE packet.
**************************************************************************************************/
unsigned char Locate_SHADE_Packet(ShadeBufferStr *buffer, ShadeDataPackStr data)
{
    int i = buffer->offset;

    while (i < data.size)
    {
        if (buffer->size < SHADE_PACKAGE_SIZE)
        {
            buffer->code[buffer->size] = *((unsigned char*)data.ptr + i);
            ++buffer->size;
        }
        ++i;
        ++buffer->offset;
    }
    return 1;
}
/**************************************************************************************************
Function Name:
    void Receive_Internal_Data(ShadeProtocolObj *obj, void *driverObj,
                               unsigned char (*driverFunc)(void*, unsigned char**, unsigned char*))
Input:
    *obj        - Object pointer of internal protocol.
    *driverObj  - Object pointer of receive driver.
    *driverFunc - Function pointer of receive function. The receive function shall includes
                  three parameters and a return value. The first parameter is an object pointer,
                  the second parameter is a double pointer points to an address of a data array,
                  and the third parameter is a pointer points to a variable updates the size of
                  the data array.
Output:
    None.
Comment:
    Receive data of internal protocol function. This function will associate with a receive
function of the peripheral module. The first argument is an address of the protocol object.
The second argument is an address of a reception driver object. The third argument is an address
of the receive function of the driver object.

The receive function shall fill the received data in the data array and update the data size to
the variable.
**************************************************************************************************/
unsigned char Receive_Shade_Internal_Data(ShadeProtocolObj *obj, void *driverObj,
                           unsigned char (*driverFunc)(void*, void**, unsigned char*))
{
    // Read uart rx data register
    ShadeBufferStr *shadeBuffer = (ShadeBufferStr*)obj->member.rxData->buffer + obj->member.rxData->index;
    ShadeDataPackStr rxData = {0, 0};

    if (driverFunc(driverObj, &rxData.ptr, (unsigned char*)&rxData.size))
    {
        long byteCount = 0;

        while (!obj->member.rxQueue->full && byteCount < rxData.size)
        {
            if (Locate_SHADE_Packet(shadeBuffer, rxData))
            {
                QueueDataStr enqueueData = {shadeBuffer, sizeof(ShadeBufferStr)};
                Common_Buffer_Enqueue(obj->member.rxQueue, enqueueData);
                byteCount = shadeBuffer->offset;
                obj->member.rxData->index = (obj->member.rxData->index + 1) & (obj->member.rxData->length - 1);
            }
            else
            {
                byteCount = shadeBuffer->offset;
            }
            shadeBuffer = (ShadeBufferStr*)obj->member.rxData->buffer + obj->member.rxData->index;
            shadeBuffer->offset = byteCount;
        }
        shadeBuffer->offset = 0;
        
        return 1;
    }
    else
    {
        return 0;
    }
}
/**************************************************************************************************
Function Name:
    unsigned long Calculate_INTP_Checksum(ShadeProtocolObj *obj, unsigned long init, unsigned char data)
Input:
    *obj          - Object pointer of internal protocol.
    init          - The inital value of checksum.
    data          - Input data of calculating.
Output:
    return        - Calculated result.
Comment:
    Checksum calculation function. This function will calculate the checksum of input data, the
calculating algorithm can be configured by the member "checksum" of internal protocol object.
**************************************************************************************************/
unsigned long Calculate_SHADE_INTP_Checksum(unsigned char* buffer, unsigned short length)
{
    uint8_t checksum = 0;

    for (int i = 1; i < length; i++)
    {
        checksum ^= buffer[i];
    }

    return checksum;
}
//---------------- END LINE -----------------------------------------------------------------------
