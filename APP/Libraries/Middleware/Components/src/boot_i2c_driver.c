/***********************************************************************************************************************
File Name: boot_i2c_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    BA100 GUI control driver.
    The driver shall create and fill in the required data in the driver's parameters before executing any function.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    05/15/2025  Wilson Chen      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "boot_i2c_driver.h"
#include "boot_process.h"
//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------
unsigned char SLIP_Encoder(SlipBufferStr *buffer, SlipDataStr data);
unsigned char SLIP_Decoder(SlipBufferStr *buffer, SlipDataStr *data);
unsigned char Locate_SLIP_Packet(SlipBufferStr *buffer, SlipDataStr data);
//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
Create_INT_Command(bootSlaveCommand, 5, 256) =
{
    //   Code,      Command Type,    Data Type,                              DataPointer,   DataSize
    {SETTABLE,   BOOT_PROTECTION,   NOT_PACKET,                 &bootInternal.protectKey,          2},
    {    NACK,         BOOT_INIT,   NOT_PACKET,                    &bootInternal.initKey,          2},
    {GETTABLE,        BOOT_STATE,   NOT_PACKET,                      &bootInternal.state,          2}
};
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------
unsigned char Internal_RXD_Error_Detection(IntProtocolObj *obj, DataPackStr data);
void Set_INTP_No_Ack_Flag(IntProtocolObj *obj, unsigned char command);
void Clear_INTP_No_Ack_Flag(IntProtocolObj *obj, unsigned char command);
void Set_INTP_Reload_Flag(IntProtocolObj *obj, unsigned char command);
//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Init_Command_Index(IntProtocolObj *obj, CommandArrayType *command)
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
void Init_Command_Index(IntProtocolObj *obj, CommandArrayType *command)
{
    int i;

    for (i = 0; i < command->indexLength; i++)
        command->index[i] = 0;
    
    for (i = 0; i < command->listLength; i++)
    {
        if (command->list[i].code < command->indexLength)
        {    
            command->index[command->list[i].code] = 
                (command->list[i].flag > 0) ? (CommandInfoStr*)&command->list[i] : 0;
        }
    }
    obj->member.listIndex = command->index;
    obj->member.indexLength = command->indexLength;   
}
/**************************************************************************************************
Function Name:
    void Pack_Internal_TXD(IntProtocolObj *obj, unsigned char command, InternalCommandType type, unsigned char priority)
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
void Pack_Internal_TXD(IntProtocolObj *obj, unsigned char command, unsigned char type, unsigned char priority)
{
    unsigned char byteCount = 0, buffer[SLIP_Packet_Size] = {command, type}; 
    
    if (command < obj->member.indexLength && 
        obj->member.listIndex[command] && obj->member.listIndex[command]->flag > UNUSED)
    {
        // Update data to buffer
        if (type == INTERNAL_SET || type == INTERNAL_REPLY)
        {    
            DataFrameStr dataPack = {command, type, {buffer, sizeof(buffer)}};
            
            if (obj->member.listIndex[command]->dataType == DATA_PACKET)  
                byteCount = obj->method.readPacketData(obj, dataPack);
            else
                byteCount = obj->method.readNotPacketData(obj, dataPack);
        }
//        else if (type == INTERNAL_GET || type == INTERNAL_ACK)
//        {
//            unsigned long checksumValue = obj->method.calculateChecksum(obj, 0, command);  
//            checksumValue = obj->method.calculateChecksum(obj, checksumValue, type);       
//            Update_INTP_Checksum(obj, &buffer[2], checksumValue); 
//            byteCount = obj->member.checksum.size + 2;
//            buffer[0] = command;
//            buffer[1] = INTERNAL_ACK;
//            byteCount = 2;
//        }
         
        // Encode Tx data
        SlipDataStr txData = {buffer, byteCount}; 
        SlipBufferStr *slipRegPtr = (SlipBufferStr*)obj->member.txData->buffer + obj->member.txData->index;
        
        if (!obj->member.txQueue->lowPriority.full && !obj->member.txQueue->highPriority.full && 
            SLIP_Encoder(slipRegPtr, txData))
        {    
            // Enqueue encoded data
            obj->member.txData->index = (obj->member.txData->index + 1) & (obj->member.txData->length - 1);
            QueueDataStr enqueueData = {slipRegPtr->code, slipRegPtr->size};
            Priority_Buffer_Enqueue(obj->member.txQueue, enqueueData, priority);
            
            if (type == INTERNAL_GET || type == INTERNAL_SET)
                Set_INTP_No_Ack_Flag(obj, command);
        }
    }
}
/**************************************************************************************************
Function Name:
    unsigned char Read_INTP_Packet_Data(IntProtocolObj *obj, DataFrameStr data)
Input:
    *obj      - Object pointer of internal protocol. 
    data      - Structure of data frame.
Output:
    byteCount - The total number of bytes of the internal protocol data frame.
Comment:
    Read packet data of internal protocol function. This function will extract the packet data of 
the command list to the space is pointed to by the data pointer of the parameter "data". 
**************************************************************************************************/
unsigned char Read_INTP_Packet_Data(IntProtocolObj *obj, DataFrameStr data)
{
    int i, j;
    unsigned char *buffer = (unsigned char*)data.pack.ptr, byteCount = 2;
    DataPackStr *infoDataPtr = (DataPackStr*)obj->member.listIndex[data.command]->ptr;
    
    if (!obj->member.checksum.position)
        byteCount += obj->member.checksum.size;
    
    // Update buffer and checksum
    unsigned long checksumValue = obj->method.calculateChecksum(obj, 0, buffer[0]);  
    checksumValue = obj->method.calculateChecksum(obj, checksumValue, buffer[1]);    

    for (i = 0; i < (obj->member.listIndex[data.command]->size >> 3); i++)
    {
        for (j = 0; j < ((DataPackStr*)obj->member.listIndex[data.command]->ptr + i)->size; j++)
        {    
            buffer[byteCount] = *((unsigned char*)(infoDataPtr + i)->ptr + j);
            checksumValue = obj->method.calculateChecksum(obj, checksumValue, buffer[byteCount]);
            
            if (++byteCount > data.pack.size)
                break;
        }
    }
    
    if (!obj->member.checksum.position)
        Update_INTP_Checksum(obj, &buffer[2], checksumValue); 
    else
    {
        Update_INTP_Checksum(obj, &buffer[byteCount], checksumValue);
        byteCount += obj->member.checksum.size;
    }
    return byteCount;
}
/**************************************************************************************************
Function Name:
    unsigned char Read_INTP_Not_Packet_Data(IntProtocolObj *obj, DataFrameStr data)
Input:
    *obj      - Object pointer of internal protocol. 
    data      - Structure of data frame.
Output:
    byteCount - The total number of bytes of the internal protocol data frame.
Comment:
    Read non-packed data of internal protocol function. This function will extract the non-packed 
data of the command list to the space is pointed to by the data pointer of the parameter "data". 
**************************************************************************************************/
unsigned char Read_INTP_Not_Packet_Data(IntProtocolObj *obj, DataFrameStr data)
{
    int i;
    unsigned char *buffer = (unsigned char*)data.pack.ptr, byteCount = 0;

//    if (!obj->member.checksum.position)
//        byteCount += obj->member.checksum.size;    
    
    // Update buffer and checksum
//    unsigned long checksumValue = obj->method.calculateChecksum(obj, 0, buffer[0]);  
//    checksumValue = obj->method.calculateChecksum(obj, checksumValue, buffer[1]); 

    unsigned char *ptrBase;
    
    if (obj->member.listIndex[data.command]->dataType == POINTER_DATA)            
        ptrBase = *(unsigned char**)obj->member.listIndex[data.command]->ptr;
    else
        ptrBase = (unsigned char*)obj->member.listIndex[data.command]->ptr;
    
    for (i = 0; i < obj->member.listIndex[data.command]->size; i++)
    {   
        buffer[byteCount] = *(ptrBase + i);
//        checksumValue = obj->method.calculateChecksum(obj, checksumValue, buffer[byteCount]);
        
        if (++byteCount > data.pack.size)
            break;
    } 
    
//    if (!obj->member.checksum.position)
//        Update_INTP_Checksum(obj, &buffer[2], checksumValue); 
//    else
//    {
//        Update_INTP_Checksum(obj, &buffer[byteCount], checksumValue);
//        byteCount += obj->member.checksum.size;
//    }     
    return byteCount;    
}
/**************************************************************************************************
Function Name:
    void Unpack_Internal_RXD(IntProtocolObj *obj)
Input:
    *obj      - Object pointer of internal protocol. 
Output:
    None.
Comment:
    Unpack received data of internal protocol function. This function will dequeue, decode, verify 
checksum and data size. The data process of internal protocol will be executed if the data is 
correct.
**************************************************************************************************/
void Unpack_Internal_RXD(IntProtocolObj *obj)
{   
    QueueDataStr dequeueData;
    
    // Dequeue Rx data queue 
    if (Common_Buffer_Dequeue(obj->member.rxQueue, &dequeueData))
    {
        // Decode Rx data
        unsigned char buffer[SLIP_Packet_Size];
        DataPackStr rxData = {buffer, 0}; 

        if (SLIP_Decoder((SlipBufferStr*)dequeueData.ptr, (SlipDataStr*)&rxData))
        {
            // Check Rx data checksum and size
            if (Internal_RXD_Error_Detection(obj, rxData))
            {              
                // Process Rx data
                obj->method.rxDataProcess(obj, rxData);
            }
        }    
    }
}
/**************************************************************************************************
Function Name:
    unsigned char Internal_RXD_Error_Detection(IntProtocolObj *obj, SlipDataStr data)
Input:
    *obj      - Object pointer of internal protocol. 
    data      - Structure of SLIP data packet. The structure includes both a data pointer and its 
                size. The data pointer of the structure points to a decoded data.
Output:
    return    - Result. 0 = checksum or size incorrect, 1 = checksum and size correct.
Comment:
    Error detection of internal protocol. This function will detect the checksum and size of
received data.
**************************************************************************************************/
unsigned char Internal_RXD_Error_Detection(IntProtocolObj *obj, DataPackStr data)
{
//    int i;
//    unsigned char *buffer = (unsigned char*)data.ptr, dataPosition = 2, checksumPosition = 2;        
//    unsigned long checksumCode = 0; 
//    unsigned long checksumValue = obj->method.calculateChecksum(obj, 0, buffer[0]);    
//    checksumValue = obj->method.calculateChecksum(obj, checksumValue, buffer[1]);
//  
//    if (!obj->member.checksum.position)
//        dataPosition += obj->member.checksum.size;
//    else
//        checksumPosition = data.size - obj->member.checksum.size;
//    
//    if (data.size >= (obj->member.checksum.size + 2))
//    {    
//        // Calculate data checksum
//        for (i = 0; i < (data.size - obj->member.checksum.size - 2); i++)
//            checksumValue = obj->method.calculateChecksum(obj, checksumValue, buffer[dataPosition + i]);
//        
//        // Read checksum code
//        for (i = 0; i < obj->member.checksum.size; i++)
//        {    
//            if (!obj->member.checksum.invert)
//                checksumCode += buffer[checksumPosition + i] << (i * 8);
//            else
//                checksumCode += (0xFF ^ buffer[checksumPosition + i]) << (i * 8);
//        }
//        
//        if (checksumValue == checksumCode)
//            return 1;      
//    }
//    return 0;
    return 1;
}
/**************************************************************************************************
Function Name:
    void Process_Internal_RXD(IntProtocolObj *obj, SlipDataStr data)
Input:
    *obj      - Object pointer of internal protocol. 
    data      - Structure of SLIP data packet. The structure includes both a data pointer and its 
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
void Process_Internal_RXD(IntProtocolObj *obj, DataPackStr data)
{
    unsigned char *buffer = (unsigned char*)data.ptr;
    unsigned char command = buffer[0];//state = 0, type = buffer[1];
 
    if (command < obj->member.indexLength && 
        obj->member.listIndex[command] && obj->member.listIndex[command]->flag > UNUSED)
    {    
        obj->method.writeNotPacketData(obj, data);
        Set_INTP_Reload_Flag(obj, command);
//        switch (type)
//        {
//            case INTERNAL_SET:
//            case INTERNAL_REPLY:
//                if (obj->member.listIndex[command]->dataType == DATA_PACKET)
//                    state = obj->method.writePacketData(obj, data);
//                else
//                    state = obj->method.writeNotPacketData(obj, data);
                
//                if (state)
//                {    
//                    if (type == INTERNAL_SET)
//                    {
//                        if (obj->member.listIndex[command]->flag == NACK)
//                            Set_INTP_Reload_Flag(obj, command);
//                        else
//                            obj->func.packTxData(obj, command, INTERNAL_ACK, LOW_PRIORITY);
//                    }
//                    Clear_INTP_No_Ack_Flag(obj, command);
//                }
//                break;
//            case INTERNAL_GET:
//                if (obj->member.listIndex[command]->flag > UNUSED)
//                    obj->func.packTxData(obj, command, INTERNAL_REPLY, LOW_PRIORITY);
//                break;
//            case INTERNAL_ACK:
//                Clear_INTP_No_Ack_Flag(obj, command); 
//                break;
//        }
    }
}
/**************************************************************************************************
Function Name:
    unsigned char Write_INTP_Packet_Data(IntProtocolObj *obj, SlipDataStr data)
Input:
    *obj      - Object pointer of internal protocol. 
    data      - Structure of SLIP data packet. The structure includes both a data pointer and its 
                size. The data pointer of the structure points to a decoded data.
Output:
    return    - Result. 0 = no data updated, 1 = data have updated.
Comment:
    Write packet data of internal protocol function. This function will update the packet data 
of the command list.
**************************************************************************************************/
unsigned char Write_INTP_Packet_Data(IntProtocolObj *obj, DataPackStr data)
{
    int i, j;
    
    unsigned char *buffer = (unsigned char*)data.ptr, byteCount = 2;
    unsigned char command = buffer[0];  
    DataPackStr *infoDataPtr = (DataPackStr*)obj->member.listIndex[command]->ptr;

    if (!obj->member.checksum.position)
        byteCount += obj->member.checksum.size;   
    
    // Check data size
    unsigned char packetSize = 0;
    
    for (i = 0; i < (obj->member.listIndex[command]->size >> 3); i++)
        packetSize += (infoDataPtr + i)->size;
    
    if (packetSize == (data.size - 2 - obj->member.checksum.size) && 
        (obj->member.listIndex[command]->flag == SETTABLE || obj->member.listIndex[command]->flag == NACK))
    {    
        // Update data
        for (i = 0; i < (obj->member.listIndex[command]->size >> 3); i++)
        {
            for (j = 0; j < (infoDataPtr + i)->size; j++)
            {    
                *((unsigned char*)(infoDataPtr + i)->ptr + j) = buffer[byteCount];
                ++byteCount;
            }
        }
        return 1;
    }
    else
        return 0;
}
/**************************************************************************************************
Function Name:
    unsigned char Write_INTP_Not_Packet_Data(IntProtocolObj *obj, SlipDataStr data)
Input:
    *obj      - Object pointer of internal protocol. 
    data      - Structure of SLIP data packet. The structure includes both a data pointer and its 
                size. The data pointer of the structure points to a decoded data.
Output:
    return    - Result. 0 = no data updated, 1 = data have updated.
Comment:
    Write non-packed data of internal protocol function. This function will update the non-packed 
data of the command list.
**************************************************************************************************/
unsigned char Write_INTP_Not_Packet_Data(IntProtocolObj *obj, DataPackStr data)
{
    int i;
    unsigned char *buffer = (unsigned char*)data.ptr, byteCount = 1;
    unsigned char command = buffer[0];
    
    // Check data size
    if (obj->member.listIndex[command]->size == (data.size - 1) && 
        (obj->member.listIndex[command]->flag == SETTABLE || obj->member.listIndex[command]->flag == NACK))
    {
        unsigned char *ptrBase;
        
        if (obj->member.listIndex[command]->dataType == POINTER_DATA)            
            ptrBase = *(unsigned char**)obj->member.listIndex[command]->ptr;
        else
            ptrBase = (unsigned char*)obj->member.listIndex[command]->ptr;
            
        for (i = 0; i < obj->member.listIndex[command]->size; i++)
        {   
            *(ptrBase + i) = buffer[byteCount];           
            ++byteCount;
        }
        return 1;
    }
    else
        return 0;
}
/**************************************************************************************************
Function Name:
    void Transmit_Internal_Data(IntProtocolObj *obj, void *driverObj, 
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
void Transmit_Internal_Data(IntProtocolObj *obj, void *driverObj,
                            unsigned char (*driverFunc)(I2cSlaveObj *, void *, u8))
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
    void Receive_Internal_Data(IntProtocolObj *obj, void *driverObj,
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
void Receive_Internal_Data(IntProtocolObj *obj, I2cSlaveObj *driverObj)
{
    // Read data register
    SlipBufferStr *slipBuffer = (SlipBufferStr*)obj->member.rxData->buffer + obj->member.rxData->index;
    SlipDataStr rxData = {0, 0};
    u8 buffer[SLIP_Packet_Size];
    void *ptr = buffer;
    u8 i2cControl = BOOT_WRITE;
    u16 timeOutCount = 0;
    
    // Check if recieved data
    if (driverObj->func.boot_receive(driverObj, &ptr, 1, I2C_NORMAL_RCV))
    {
        rxData.ptr = buffer;
        
        while (driverObj->member.reg.STS2->bit.BUSY)
        {
            if (driverObj->member.reg.STS2->bit.TRF)
            {
                obj->func.packTxData(obj, buffer[0], INTERNAL_REPLY, LOW_PRIORITY);
//                driverObj->func.transmit(driverObj, obj->member.listIndex[buffer[0]]->ptr, obj->member.listIndex[buffer[0]]->size);
                
                i2cControl = BOOT_READ;
                break;
            }
            else if (timeOutCount++ > 1000)
            {
//                driverObj->member.reg.DAT->all = 0;
//                driverObj->member.reg.CTRL1->bit.EN = 1;
                break;
            }
        }
        
        if (i2cControl == BOOT_WRITE)
        {
            rxData.size = obj->member.listIndex[buffer[0]]->size;
            
            ptr = &buffer[1];
            
            if (driverObj->func.boot_receive(driverObj, &ptr, rxData.size, I2C_DATA_ONLY))
            {
                long byteCount = 0;
                
                while (!obj->member.rxQueue->full && byteCount < rxData.size)
                {
                    if (Locate_SLIP_Packet(slipBuffer, rxData))
                    {
                        QueueDataStr enqueueData = {slipBuffer, sizeof(SlipBufferStr)};
                        Common_Buffer_Enqueue(obj->member.rxQueue, enqueueData);
                        byteCount = slipBuffer->offset;                
                        obj->member.rxData->index = (obj->member.rxData->index + 1) & (obj->member.rxData->length - 1);                
                    }
                    else
                        byteCount = slipBuffer->offset; 
                    slipBuffer = (SlipBufferStr*)obj->member.rxData->buffer + obj->member.rxData->index;
                    slipBuffer->offset = byteCount;
                }
                slipBuffer->offset = 0;
            }
        }
    }
}
/**************************************************************************************************
Function Name:
    unsigned long Calculate_INTP_Checksum(IntProtocolObj *obj, unsigned long init, unsigned char data)
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
unsigned long Calculate_INTP_Checksum(IntProtocolObj *obj, unsigned long init, unsigned char data)
{
    unsigned long checksumMask = 0xFFFFFFFF >> (8 * (4 - (obj->member.checksum.size & 0x3)));

    return obj->member.checksum.algorithm(init, data) & checksumMask;
}
/**************************************************************************************************
Function Name:
    void Update_INTP_Checksum(IntProtocolObj *obj, unsigned char *buffer, unsigned long checksum)
Input:
    *obj          - Object pointer of internal protocol. 
    *buffer       - Pointer of data Buffer.
    checksum      - Checksum value.
Output:
    None.
Comment:
    Update internal protocol checksum function. This function will fill the checksum data in 
the data buffer. The check data will be inverted if the member "checksum.invert" of internal 
protocol object is asserted.
**************************************************************************************************/
void Update_INTP_Checksum(IntProtocolObj *obj, unsigned char *buffer, unsigned long checksum)
{
    int i;
    
    if (obj->member.checksum.invert)
        checksum = ~checksum;
    
    for (i = 0; i < obj->member.checksum.size; i++)   
        buffer[i] = (checksum >> (i * 8)) & 0xFF; 
}
/**************************************************************************************************
Function Name:
    void Get_INTP_No_Ack_Flag(IntProtocolObj *obj, unsigned char command)
Input:
    *obj          - Object pointer of internal protocol. 
    command       - Command code of internal protocol.
Output:
    None.
Comment:
    Get the No Ack flag of internal protocol command function.
**************************************************************************************************/
unsigned char Get_INTP_No_Ack_Flag(IntProtocolObj *obj, unsigned char command)
{
    return (obj->member.noAckBits[(command >> 4) & 0xF].all >> (command & 0xF)) & 1;
}
/**************************************************************************************************
Function Name:
    void Set_INTP_No_Ack_Flag(IntProtocolObj *obj, unsigned char command)
Input:
    *obj          - Object pointer of internal protocol. 
    command       - Command code of internal protocol.
Output:
    None.
Comment:
    Set the No Ack flag of internal protocol command function.
**************************************************************************************************/
void Set_INTP_No_Ack_Flag(IntProtocolObj *obj, unsigned char command)
{
    obj->member.noAckBits[command >> 4].all |= 1 << (command - ((command >> 4) << 4));
}
/**************************************************************************************************
Function Name:
    void Clear_INTP_No_Ack_Flag(IntProtocolObj *obj, unsigned char command)
Input:
    *obj          - Object pointer of internal protocol. 
    command       - Command code of internal protocol.
Output:
    None.
Comment:
    Clear the No Ack flag of internal protocol command function.
**************************************************************************************************/
void Clear_INTP_No_Ack_Flag(IntProtocolObj *obj, unsigned char command)
{
    obj->member.noAckBits[command >> 4].all &= ~(1 << (command - ((command >> 4) << 4)));
}
/**************************************************************************************************
Function Name:
    void Get_INTP_Reload_Flag(IntProtocolObj *obj, unsigned char command)
Input:
    *obj          - Object pointer of internal protocol. 
    command       - Command code of internal protocol.
Output:
    None.
Comment:
    Get the reload flag of internal protocol command function.
**************************************************************************************************/
unsigned char Get_INTP_Reload_Flag(IntProtocolObj *obj, unsigned char command)
{
    return (obj->member.reloadBits[(command >> 4) & 0xF].all >> (command & 0xF)) & 1;
}
/**************************************************************************************************
Function Name:
    void Set_INTP_Reload_Flag(IntProtocolObj *obj, unsigned char command)
Input:
    *obj          - Object pointer of internal protocol. 
    command       - Command code of internal protocol.
Output:
    None.
Comment:
    Set the Reload flag of internal protocol command function.
**************************************************************************************************/
void Set_INTP_Reload_Flag(IntProtocolObj *obj, unsigned char command)
{
    obj->member.reloadBits[command >> 4].all |= 1 << (command - ((command >> 4) << 4));
}
/**************************************************************************************************
Function Name:
    void Clear_INTP_Reload_Flag(IntProtocolObj *obj, unsigned char command)
Input:
    *obj          - Object pointer of internal protocol. 
    command       - Command code of internal protocol.
Output:
    None.
Comment:
    Clear the Reload flag of internal protocol command function.
**************************************************************************************************/
void Clear_INTP_Reload_Flag(IntProtocolObj *obj, unsigned char command)
{
    obj->member.reloadBits[command >> 4].all &= ~(1 << (command - ((command >> 4) << 4)));
}
/**************************************************************************************************
Function Name:
    unsigned char SLIP_Encoder(SlipBufferStr *buffer, SlipDataStr data)
Input:
    *buffer     - SLIP packet buffer.
    data        - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = data has encoded and updated to buffer, 0 = error occured.
Comment:
    Data encode to SLIP packet function. 
**************************************************************************************************/
unsigned char SLIP_Encoder(SlipBufferStr *buffer, SlipDataStr data)
{
//    unsigned short value = *(unsigned short*)obj->member.listIndex[data.cmd]->ptr;
    
//    buffer->code[0] = (value >> 8) & 0xFF;
//    buffer->code[1] = value & 0xFF;
//    buffer->size = 2;
    
    int i = 0;
        
    for (i = 0; i < data.size; i++)
    {
        buffer->code[i] = *((unsigned char*)data.ptr + i);
    }
    buffer->size = i;
    
    return 1;
}
/**************************************************************************************************
Function Name:
    unsigned char SLIP_Decoder(SlipBufferStr *buffer, SlipDataStr *data)
Input:
    *buffer     - SLIP packet buffer.
    *data       - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = SLIP packet has decoded and updated to the data space that is the 
                  data pointer points to, 0 = error occured.
Comment:
    SLIP packet decoding function. 
**************************************************************************************************/
unsigned char SLIP_Decoder(SlipBufferStr *buffer, SlipDataStr *data)
{
    int i = 0, j = 0;

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
    unsigned char Locate_SLIP_Packet(SlipBufferStr *buffer, void *ptr, unsigned char size)
Input:
    *buffer     - SLIP packet buffer.
    data        - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = SLIP packet has located, 0 = Locating or stopped.
Comment:
    This function is used to locate a SLIP packet. This function will skip the locating if the 
buffer has filled in a SLIP packet.
**************************************************************************************************/
unsigned char Locate_SLIP_Packet(SlipBufferStr *buffer, SlipDataStr data)
{
    int i = buffer->offset;

     while (i < (data.size + 1))
    {
        if (buffer->size < 64)
        {
            buffer->code[buffer->size] = *((unsigned char*)data.ptr + i);
            ++buffer->size;
        }
        ++i;
        ++buffer->offset;
    }
    return 1;
}
//----------------------------------------------------------------------------------------------------------------------
