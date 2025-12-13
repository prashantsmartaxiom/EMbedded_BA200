/***********************************************************************************************************************
File Name: light_driver.c
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
    07/30/2025  Wilson Chen      Create file

***********************************************************************************************************************/

/* Includes */ 
#include "light_driver.h"

/* Local function prototypes */
unsigned char LIGHT_Internal_RXD_Error_Detection(LightProtocolObj *obj, LightDataPackStr data);
unsigned char LIGHT_Decoder(LightBufferStr *buffer, LightDataPackStr *data);
unsigned char Locate_LIGHT_Packet(LightBufferStr *buffer, LightDataPackStr data);
LightStruct_t light_regs = {0};

Create_LIGHT_Command(lightCommand, 3, 8) =
{
    { LIGHT_TEMPERATURE,      &light_regs.light_temperature,    2},
    {    LIGHT_HUMIDITY,         &light_regs.light_humidity,    2},
    {   LIGHT_INTENSITY,        &light_regs.light_intensity,    2}
};

Create_LIGHT_Protocol_Object(lightProtocol, 32);
/**************************************************************************************************
Function Name:
    void Init_HMI_Protocol(LightProtocolObj *obj, LightCommandArrayType *command)
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
void Init_LIGHT_Protocol(LightProtocolObj *obj, LightCommandArrayType *command)
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
            command->index[command->list[i].code] = 
                (command->list[i].ptr != NULL) ? (LightCommandInfoStr*)&command->list[i] : 0;
        }
    }
    
    obj->member.listIndex = command->index;
    obj->member.indexLength = command->indexLength;  
}
/**************************************************************************************************
Function Name:
    unsigned char HMI_Encoder(LightBufferStr *buffer, LightDataStr data)
Input:
    *buffer     - HMI packet buffer.
    data        - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = data has encoded and updated to buffer, 0 = error occured.
Comment:
    Data encode to HMI packet function. 
**************************************************************************************************/
unsigned char LIGHT_Encoder(LightProtocolObj *obj, LightBufferStr *buffer, LightDataStr data)
{
    uint16_t crc = 0;
    
    if (!obj->member.listIndex[data.addr])
    {
        return 0;
    }

    buffer->code[0] = data.id;
    buffer->code[1] = data.func;
    buffer->code[2] = (data.addr >> 8) & 0xFF;
    buffer->code[3] = data.addr & 0xFF;
    
    if (buffer->code[1] == READ_HOLDING_REGISTERS)
    {
        buffer->code[4] = (data.length >> 8) & 0xFF;
        buffer->code[5] = data.length & 0xFF;
        buffer->size = 6;
    }
    else if (buffer->code[1] == WRITE_SINGLE_REGISTER)
    {        
        if (obj->member.listIndex[data.addr]->size == 1)
        {
            buffer->code[4] = 0x00;
            buffer->code[5] = *((unsigned char*)obj->member.listIndex[data.addr]->ptr);
        }
        else if (obj->member.listIndex[data.addr]->size == 2)
        {
            buffer->code[4] = *((unsigned char*)obj->member.listIndex[data.addr]->ptr);
            buffer->code[5] = *((unsigned char*)obj->member.listIndex[data.addr]->ptr + 1);
        }
        
        buffer->size = 6;
    }
    
    crc = (uint16_t)obj->method.calculateChecksum(buffer->code, buffer->size);
    buffer->code[buffer->size++] = crc & 0xFF;
    buffer->code[buffer->size++] = (crc >> 8) & 0xFF;

    return 1;
}
/**************************************************************************************************
Function Name:
    void Pack_HMI_Internal_TXD(LightProtocolObj *obj, unsigned char command, InternalCommandType type, unsigned char priority)
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
void Pack_LIGHT_Internal_TXD(LightProtocolObj *obj, unsigned char id, unsigned char func, unsigned char addr, unsigned char length, unsigned char priority)
{
    unsigned char buffer[PACKAGE_SIZE] = {0}; 
    
    if (addr < obj->member.indexLength && 
        obj->member.listIndex[addr] && obj->member.listIndex[addr]->ptr != NULL)
    {
        LightDataStr txData = {buffer, id, func, addr, length};
        LightBufferStr *lightRegPtr = (LightBufferStr*)obj->member.txData->buffer + obj->member.txData->index;
        
        if (!obj->member.txQueue->lowPriority.full && !obj->member.txQueue->highPriority.full && 
            LIGHT_Encoder(obj, lightRegPtr, txData))
        {
            obj->member.txData->index = (obj->member.txData->index + 1) & (obj->member.txData->length - 1);
            QueueDataStr enqueueData = {lightRegPtr->code, lightRegPtr->size};
            Priority_Buffer_Enqueue(obj->member.txQueue, enqueueData, priority);
        }
    }
}

/**************************************************************************************************
Function Name:
    unsigned char HMI_Decoder(LightBufferStr *buffer, LightDataPackStr *data)
Input:
    *buffer     - PIR packet buffer.
    *data       - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = PIR packet has decoded and updated to the data space that is the 
                  data pointer points to, 0 = error occured.
Comment:
    PIR packet decoding function. 
**************************************************************************************************/
unsigned char LIGHT_Decoder(LightBufferStr *buffer, LightDataPackStr *data)
{ 
    int i = 0, j = 0;
    
    if (buffer->size < 5)
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
    void Unpack_HMI_Internal_RXD(LightProtocolObj *obj)
Input:
    *obj      - Object pointer of internal protocol. 
Output:
    None.
Comment:
    Unpack received data of internal protocol function. This function will dequeue, decode, verify 
checksum and data size. The data process of internal protocol will be executed if the data is 
correct.
**************************************************************************************************/
unsigned char Unpack_LIGHT_Internal_RXD(LightProtocolObj *obj, unsigned char addr, LightStruct_t *info)
{   
    QueueDataStr dequeueData;
    
    if (Common_Buffer_Dequeue(obj->member.rxQueue, &dequeueData))
    {
        // Decode Rx data
        unsigned char buffer[PACKAGE_SIZE] = {0};
        LightDataPackStr rxData = {buffer, 0}; 
        
        if (LIGHT_Decoder((LightBufferStr*)dequeueData.ptr, (LightDataPackStr*)&rxData))
        {
            // Check Rx data checksum and size
            if (LIGHT_Internal_RXD_Error_Detection(obj, rxData))
            {              
                // Process Rx data
                obj->method.rxDataProcess(obj, rxData, addr, info);
                
                return buffer[0];
            }
        }    
    }
    
    return 0;
}
/**************************************************************************************************
Function Name:
    unsigned char HMI_Internal_RXD_Error_Detection(LightProtocolObj *obj, LightDataPackStr data)
Input:
    *obj      - Object pointer of internal protocol. 
    data      - Structure of PIR data packet. The structure includes both a data pointer and its 
                size. The data pointer of the structure points to a decoded data.
Output:
    return    - Result. 0 = checksum or size incorrect, 1 = checksum and size correct.
Comment:
    Error detection of internal protocol. This function will detect the checksum and size of
received data.
**************************************************************************************************/
unsigned char LIGHT_Internal_RXD_Error_Detection(LightProtocolObj *obj, LightDataPackStr data)
{   
    unsigned char *buffer = (unsigned char*)data.ptr;
    unsigned short recevied_crc = (buffer[data.size - 1] << 8) | (buffer[data.size - 2]);
    unsigned short calculated_crc = obj->method.calculateChecksum(buffer, data.size - 2);
    
    if (calculated_crc == recevied_crc)
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
    void Process_LIGHT_Internal_RXD(LightProtocolObj *obj, LightDataPackStr data)
Input:
    *obj      - Object pointer of internal protocol. 
    data      - Structure of PIR data packet. The structure includes both a data pointer and its 
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
void Process_LIGHT_Internal_RXD(LightProtocolObj *obj, LightDataPackStr data, unsigned char addr, LightStruct_t *info)
{
    uint8_t byte_count = *((unsigned char *)data.ptr + 2), i = 0;
    uint16_t current_addr = addr;
    
    if (data.size < 5) return;

    if (*((unsigned char *)data.ptr + 1) == READ_HOLDING_REGISTERS || *((unsigned char *)data.ptr + 1) == WRITE_SINGLE_REGISTER)
    {
        if (!obj->member.listIndex[addr] || !obj->member.listIndex[addr]->ptr)
        {
            return;
        }

        uint8_t dataOffset = (*((unsigned char *)data.ptr + 1) == READ_HOLDING_REGISTERS) ? 3 : 4;

        if (*((unsigned char *)data.ptr + 1) == READ_HOLDING_REGISTERS)
        {
            if (data.size < dataOffset + byte_count + 2)
            {
                return;
            }
            
            for (i = 0; i < (byte_count / 2) && current_addr < obj->member.indexLength; i++)
            {
                if (!obj->member.listIndex[current_addr] || !obj->member.listIndex[current_addr]->ptr)
                {
                    current_addr++;
                    continue;
                }
                
                if (obj->member.listIndex[current_addr]->size == 1)
                {
                    if (dataOffset + i * 2 + 1 < data.size)
                    {
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr) = *((unsigned char *)data.ptr + dataOffset + i * 2 + 1);
                    }
                }
                else if (obj->member.listIndex[current_addr]->size == 2)
                {
                    if (dataOffset + i * 2 + 1 < data.size)
                    {
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr + 1) = *((unsigned char *)data.ptr + dataOffset + i * 2);
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr) = *((unsigned char *)data.ptr + dataOffset + i * 2 + 1);
                        
                        if (current_addr == LIGHT_TEMPERATURE)
                        {
                            info->light_temperature = ((*((unsigned char *)data.ptr + dataOffset + i * 2)) << 8) |
                                                      (*((unsigned char *)data.ptr + dataOffset + i * 2 + 1));
                        }
                        else if (current_addr == LIGHT_HUMIDITY)
                        {
                            info->light_humidity = ((*((unsigned char *)data.ptr + dataOffset + i * 2)) << 8) |
                                                   (*((unsigned char *)data.ptr + dataOffset + i * 2 + 1));
                        }
                        else if (current_addr == LIGHT_INTENSITY)
                        {
                            info->light_intensity = ((*((unsigned char *)data.ptr + dataOffset + i * 2)) << 8) |
                                                    (*((unsigned char *)data.ptr + dataOffset + i * 2 + 1));
                        }
                    }
                }
                else if (obj->member.listIndex[current_addr]->size == 4 && i + 1 < (byte_count / 2))
                {
                    if (dataOffset + i * 2 + 3 < data.size)
                    {
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr + 3) = *((unsigned char *)data.ptr + dataOffset + i * 2);
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr + 2) = *((unsigned char *)data.ptr + dataOffset + i * 2 + 1);
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr + 1) = *((unsigned char *)data.ptr + dataOffset + i * 2 + 2);
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr) = *((unsigned char *)data.ptr + dataOffset + i * 2 + 3);
                        i++;
                        current_addr++;
                    }
                }
                current_addr++;
            }
        }
        else if (*((unsigned char *)data.ptr + 1) == WRITE_SINGLE_REGISTER)
        {
            if ((obj->member.listIndex[addr]->size == 1 && data.size < dataOffset + 1) ||
                (obj->member.listIndex[addr]->size == 2 && data.size < dataOffset + 2) ||
                (obj->member.listIndex[addr]->size == 4 && data.size < dataOffset + 4))
            {
                return;
            }

            if (obj->member.listIndex[addr]->size == 1)
            {
                *((unsigned char *)obj->member.listIndex[addr]->ptr) = *((unsigned char *)data.ptr + dataOffset + 1);
            }
            else if (obj->member.listIndex[addr]->size == 2)
            {
                *((unsigned char *)obj->member.listIndex[addr]->ptr + 1) = *((unsigned char *)data.ptr + dataOffset);
                *((unsigned char *)obj->member.listIndex[addr]->ptr) = *((unsigned char *)data.ptr + dataOffset + 1);
            }
            else if (obj->member.listIndex[addr]->size == 4)
            {
                *((unsigned char *)obj->member.listIndex[addr]->ptr + 3) = *((unsigned char *)data.ptr + dataOffset);
                *((unsigned char *)obj->member.listIndex[addr]->ptr + 2) = *((unsigned char *)data.ptr + dataOffset + 1);
                *((unsigned char *)obj->member.listIndex[addr]->ptr + 1) = *((unsigned char *)data.ptr + dataOffset + 2);
                *((unsigned char *)obj->member.listIndex[addr]->ptr) = *((unsigned char *)data.ptr + dataOffset + 3);
            }
        }
    }
}
/**************************************************************************************************
Function Name:
    void Transmit_HMI_Internal_Data(LightProtocolObj *obj, void *driverObj, 
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
void Transmit_LIGHT_Internal_Data(LightProtocolObj *obj, void *driverObj,
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
    unsigned char Locate_HMI_Packet(LightBufferStr *buffer, void *ptr, unsigned char size)
Input:
    *buffer     - PIR packet buffer.
    data        - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = PIR packet has located, 0 = Locating or stopped.
Comment:
    This function is used to locate a PIR packet. This function will skip the locating if the 
buffer has filled in a PIR packet.
**************************************************************************************************/
unsigned char Locate_LIGHT_Packet(LightBufferStr *buffer, LightDataPackStr data)
{
    int i = buffer->offset;
    
    while (i < data.size)
    {
        if (buffer->size < PACKAGE_SIZE)
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
    unsigned char Receive_HMI_Internal_Data(LightProtocolObj *obj, void *driverObj,
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
unsigned char Receive_LIGHT_Internal_Data(LightProtocolObj *obj, void *driverObj,
                           unsigned char (*driverFunc)(void*, void**, unsigned char*))
{
    // Read uart rx data register
    LightBufferStr *lightBuffer = (LightBufferStr*)obj->member.rxData->buffer + obj->member.rxData->index;
    LightDataPackStr rxData = {0, 0};   
    
    if (driverFunc(driverObj, &rxData.ptr, (unsigned char*)&rxData.size))
    {               
        long byteCount = 0;
        
        while (!obj->member.rxQueue->full && byteCount < rxData.size)
        {            
            if (Locate_LIGHT_Packet(lightBuffer, rxData))
            {    
                QueueDataStr enqueueData = {lightBuffer, sizeof(LightBufferStr)};
                Common_Buffer_Enqueue(obj->member.rxQueue, enqueueData);
                byteCount = lightBuffer->offset;                
                obj->member.rxData->index = (obj->member.rxData->index + 1) & (obj->member.rxData->length - 1);                
            }
            else
            {
                byteCount = lightBuffer->offset;
            }
            lightBuffer = (LightBufferStr*)obj->member.rxData->buffer + obj->member.rxData->index;
            lightBuffer->offset = byteCount;
        }
        lightBuffer->offset = 0;
        
        return 1;
    }
    
    return 0;
}
/**************************************************************************************************
Function Name:
    unsigned long Calculate_LIGHT_CRC(LightProtocolObj *obj, unsigned long init, unsigned char data)
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
unsigned long Calculate_LIGHT_CRC(unsigned char* buffer, unsigned short length)
{    
    uint8_t i = 0, j =0;
    uint16_t crc = 0xFFFF;

    for (i = 0; i < length; i++)
    {
        crc ^= buffer[i];
        
        for (j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}
//---------------- END LINE -----------------------------------------------------------------------
