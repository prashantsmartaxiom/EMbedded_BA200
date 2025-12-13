/***********************************************************************************************************************
File Name: pir_driver.c
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
#include "hmi_driver.h"
#include <string.h>
#include "main.h"

/* Local function prototypes */
unsigned char HMI_Internal_RXD_Error_Detection(HmiProtocolObj *obj, HmiDataPackStr data);
unsigned char HMI_Decoder(HmiBufferStr *buffer, HmiDataPackStr *data);
unsigned char Locate_HMI_Packet(HmiBufferStr *buffer, HmiDataPackStr data);
HmiStruct_t hmi_regs = {0};
PanelStruct_t panel_regs =  { 0 };
QueueHandle_t HIMCommandQueueHandle = NULL;

//Create_HMI_Command(hmiCommand, 5, 8) =
//{
//    {REGISTER_UPDATE_STATUS,      &hmi_regs.update_status,    2},
//    {            LED_SWITCH,         &hmi_regs.led_switch,    1},
//    {        LED_BRIGHTNESS,     &hmi_regs.led_brightness,    1},
//    { LED_COLOR_TEMPERATURE,     &hmi_regs.led_color_temp,    2},
//    {   SHADE_MOTOR_CONTROL,   &hmi_regs.shade_motor_ctrl,    1}
//};

Create_HMI_Command(hmiCommand, 7, 14) =
{
    {PANEL_REG_ADDR,         &panel_regs.panel_id,          1},
    {PANEL_LED_CHANNEL,      &panel_regs.led_channel,       2},
    {PANEL_LED_BRIGHTNESS,   &panel_regs.led_brightness,    2},
    {PANEL_LED_STATUS,       &panel_regs.led_status,        2},
    {PANEL_LED_POWER,        &panel_regs.led_power,         2},
    {PANEL_SHADE_CHANNEL,    &panel_regs.shade_channel,     2},
    {PANEL_SHADE_STATUS,     &panel_regs.shade_status,      2}
};


Create_HMI_Protocol_Object(hmiProtocol, 32);
/**************************************************************************************************
Function Name:
    void Init_HMI_Protocol(HmiProtocolObj *obj, HmiCommandArrayType *command)
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
void Init_HMI_Protocol(HmiProtocolObj *obj, HmiCommandArrayType *command)
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
                (command->list[i].ptr != NULL) ? (HmiCommandInfoStr*)&command->list[i] : 0;
        }
    }

    obj->member.listIndex = command->index;
    obj->member.indexLength = command->indexLength;
}
/**************************************************************************************************
Function Name:
    unsigned char HMI_Encoder(HmiBufferStr *buffer, HmiDataStr data)
Input:
    *buffer     - HMI packet buffer.
    data        - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = data has encoded and updated to buffer, 0 = error occured.
Comment:
    Data encode to HMI packet function.
**************************************************************************************************/
unsigned char HMI_Encoder(HmiProtocolObj *obj, HmiBufferStr *buffer, HmiDataStr data)
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
    } else if ( buffer->code[1] == MODBUS_MULTIPLE_REGISTER) {
    
    }

    crc = (uint16_t)obj->method.calculateChecksum(buffer->code, buffer->size);
    buffer->code[buffer->size++] = crc & 0xFF;
    buffer->code[buffer->size++] = (crc >> 8) & 0xFF;

    return 1;
}
/**************************************************************************************************
Function Name:
    void Pack_HMI_Internal_TXD(HmiProtocolObj *obj, unsigned char command, InternalCommandType type, unsigned char priority)
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
void Pack_HMI_Internal_TXD(HmiProtocolObj *obj, unsigned char id, unsigned char func, unsigned char addr, unsigned char length, unsigned char priority)
{
    unsigned char buffer[PACKAGE_SIZE] = {0};

    if (addr < obj->member.indexLength &&
        obj->member.listIndex[addr] && obj->member.listIndex[addr]->ptr != NULL)
    {
        HmiDataStr txData = {buffer, id, func, addr, length};
        HmiBufferStr *hmiRegPtr = (HmiBufferStr*)obj->member.txData->buffer + obj->member.txData->index;

        if (!obj->member.txQueue->lowPriority.full && !obj->member.txQueue->highPriority.full &&
            HMI_Encoder(obj, hmiRegPtr, txData))
        {
            obj->member.txData->index = (obj->member.txData->index + 1) & (obj->member.txData->length - 1);
            QueueDataStr enqueueData = {hmiRegPtr->code, hmiRegPtr->size};
            Priority_Buffer_Enqueue(obj->member.txQueue, enqueueData, priority);
        }
    }
}

/**************************************************************************************************
Function Name:
    unsigned char HMI_Decoder(HmiBufferStr *buffer, HmiDataPackStr *data)
Input:
    *buffer     - PIR packet buffer.
    *data       - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = PIR packet has decoded and updated to the data space that is the
                  data pointer points to, 0 = error occured.
Comment:
    PIR packet decoding function.
**************************************************************************************************/
unsigned char HMI_Decoder(HmiBufferStr *buffer, HmiDataPackStr *data)
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
    void Unpack_HMI_Internal_RXD(HmiProtocolObj *obj)
Input:
    *obj      - Object pointer of internal protocol.
Output:
    None.
Comment:
    Unpack received data of internal protocol function. This function will dequeue, decode, verify
checksum and data size. The data process of internal protocol will be executed if the data is
correct.
**************************************************************************************************/
unsigned char Unpack_HMI_Internal_RXD(HmiProtocolObj *obj, unsigned char addr, HmiStruct_t *info)
{
    QueueDataStr dequeueData;

    if (Common_Buffer_Dequeue(obj->member.rxQueue, &dequeueData))
    {
        // Decode Rx data
        unsigned char buffer[PACKAGE_SIZE] = {0};
        HmiDataPackStr rxData = {buffer, 0};

        if (HMI_Decoder((HmiBufferStr*)dequeueData.ptr, (HmiDataPackStr*)&rxData))
        {
            // Check Rx data checksum and size
            if (HMI_Internal_RXD_Error_Detection(obj, rxData))
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
    unsigned char HMI_Internal_RXD_Error_Detection(HmiProtocolObj *obj, HmiDataStr data)
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
unsigned char HMI_Internal_RXD_Error_Detection(HmiProtocolObj *obj, HmiDataPackStr data)
{
    unsigned char *buffer = (unsigned char*)data.ptr;
    unsigned short recevied_crc = (buffer[data.size - 1] << 8) | (buffer[data.size - 2]);
    unsigned short calculated_crc = obj->method.calculateChecksum(buffer, data.size - 2);

//    buffer[0] = buffer[8];
//    buffer[1] = buffer[9];
//    buffer[2] = buffer[10];
//    buffer[3] = buffer[11];
//    buffer[4] = buffer[12];
//    buffer[5] = buffer[13];
//    buffer[6] = buffer[14];
//
//    data.size = 7;

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
    void Process_HMI_Internal_RXD(HmiProtocolObj *obj, HmiDataStr data)
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
void Process_HMI_Internal_RXD(HmiProtocolObj *obj, HmiDataPackStr data, unsigned char addr, HmiStruct_t *info)
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

                        if (current_addr == LED_SWITCH)
                        {
                            info->led_switch = *((unsigned char *)data.ptr + dataOffset + i * 2 + 1);
                        }
                        else if (current_addr == LED_BRIGHTNESS)
                        {
                            info->led_brightness = *((unsigned char *)data.ptr + dataOffset + i * 2 + 1);
                        }
                        else if (current_addr == SHADE_MOTOR_CONTROL)
                        {
                            info->shade_motor_ctrl = *((unsigned char *)data.ptr + dataOffset + i * 2 + 1);
                        }
                    }
                }
                else if (obj->member.listIndex[current_addr]->size == 2)
                {
                    if (dataOffset + i * 2 + 1 < data.size)
                    {
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr + 1) = *((unsigned char *)data.ptr + dataOffset + i * 2);
                        *((unsigned char *)obj->member.listIndex[current_addr]->ptr) = *((unsigned char *)data.ptr + dataOffset + i * 2 + 1);

                        if (current_addr == REGISTER_UPDATE_STATUS)
                        {
                            info->update_status = ((*((unsigned char *)data.ptr + dataOffset + i * 2)) << 8) |
                                                  (*((unsigned char *)data.ptr + dataOffset + i * 2 + 1));
                        }
                        else if (current_addr == LED_COLOR_TEMPERATURE)
                        {
                            info->led_color_temp = ((*((unsigned char *)data.ptr + dataOffset + i * 2)) << 8) |
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
    void Transmit_HMI_Internal_Data(HmiProtocolObj *obj, void *driverObj,
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
void Transmit_HMI_Internal_Data(HmiProtocolObj *obj, void *driverObj,
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
    unsigned char Locate_HMI_Packet(HmiBufferStr *buffer, void *ptr, unsigned char size)
Input:
    *buffer     - PIR packet buffer.
    data        - Data structure, it includes a data pointer and size of the data pinter.
Output:
    return      - Result. 1 = PIR packet has located, 0 = Locating or stopped.
Comment:
    This function is used to locate a PIR packet. This function will skip the locating if the
buffer has filled in a PIR packet.
**************************************************************************************************/
unsigned char Locate_HMI_Packet(HmiBufferStr *buffer, HmiDataPackStr data)
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
    unsigned char Receive_HMI_Internal_Data(HmiProtocolObj *obj, void *driverObj,
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
unsigned char Receive_HMI_Internal_Data(HmiProtocolObj *obj, void *driverObj,
                           unsigned char (*driverFunc)(void*, void**, unsigned char*))
{
    // Read uart rx data register
    HmiBufferStr *hmiBuffer = (HmiBufferStr*)obj->member.rxData->buffer + obj->member.rxData->index;
    HmiDataPackStr rxData = {0, 0};

    if (driverFunc(driverObj, &rxData.ptr, (unsigned char*)&rxData.size))
    {
        long byteCount = 0;

        while (!obj->member.rxQueue->full && byteCount < rxData.size)
        {
            if (Locate_HMI_Packet(hmiBuffer, rxData))
            {
                QueueDataStr enqueueData = {hmiBuffer, sizeof(HmiBufferStr)};
                Common_Buffer_Enqueue(obj->member.rxQueue, enqueueData);
                byteCount = hmiBuffer->offset;
                obj->member.rxData->index = (obj->member.rxData->index + 1) & (obj->member.rxData->length - 1);
            }
            else
            {
                byteCount = hmiBuffer->offset;
            }
            hmiBuffer = (HmiBufferStr*)obj->member.rxData->buffer + obj->member.rxData->index;
            hmiBuffer->offset = byteCount;
        }
        hmiBuffer->offset = 0;

        return 1;
    }

    return 0;
}
/**************************************************************************************************
Function Name:
    unsigned long Calculate_HMI_CRC(HmiProtocolObj *obj, unsigned long init, unsigned char data)
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
unsigned long Calculate_HMI_CRC(unsigned char* buffer, unsigned short length)
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


unsigned char process_rx_data(HmiProtocolObj *obj, unsigned char addr, HmiStruct_t *info) 
{
    QueueDataStr dequeueData;

    if (Common_Buffer_Dequeue(obj->member.rxQueue, &dequeueData))
    {
        // Decode Rx data
        unsigned char buffer[PACKAGE_SIZE] = {0};
        HmiDataPackStr rxData = {buffer, 0};

        if (HMI_Decoder((HmiBufferStr*)dequeueData.ptr, (HmiDataPackStr*)&rxData))
        {
            // Check Rx data checksum and size
            if (HMI_Internal_RXD_Error_Detection(obj, rxData))
            {
                // Process Rx data
                //process_data(obj, rxData, addr, info);

                return buffer[0];
            }
        }
    }
    return 0;
}


/**************************************************************************************************
Function Name:
    unsigned char Send_LED_Config_State_To_HMI(void *driverObj, 
                                               unsigned char (*driverFunc)(void*, void*, unsigned char),
                                               uint8_t slave_id,
                                               uint16_t start_addr,
                                               uint16_t channel,
                                               uint16_t brightness,
                                               uint16_t on_off)
Input:
    driverObj   - Pointer to transmit driver object.
    driverFunc  - Pointer to transmit function (driver specific).
    slave_id    - Modbus slave ID (HMI address).
    start_addr  - Starting register address for write.
    channel     - LED channel number.
    brightness  - LED brightness level.
    on_off      - LED ON/OFF state.
Output:
    return      - 1 if transmission successful, 0 otherwise.
Comment:
    This function prepares and transmits LED configuration (channel, brightness, state)
    to the HMI using Modbus "Write Multiple Registers (0x10)" command format.
**************************************************************************************************/
unsigned char Send_LED_Config_State_To_HMI(void *driverObj,
                                           unsigned char (*driverFunc)(void*, void*, unsigned char),
                                           uint8_t slave_id,
                                           uint16_t start_addr,
                                           uint16_t channel,
                                           uint16_t brightness,
                                           uint16_t on_off)
{
    uint8_t frame[32] = {0};
    uint8_t pos = 0;
    uint16_t register_count = 3;    // 3 registers: channel, brightness, on_off
    uint8_t byte_count = register_count * 2;

    // Construct Modbus Write Multiple Registers (0x10) frame
    frame[pos++] = slave_id;
    frame[pos++] = 0x10; // MODBUS_WRITE_MULTIPLE_REGISTERS

    frame[pos++] = (start_addr >> 8) & 0xFF;
    frame[pos++] = start_addr & 0xFF;

    frame[pos++] = (register_count >> 8) & 0xFF;
    frame[pos++] = register_count & 0xFF;

    frame[pos++] = byte_count;

    // Payload: Channel, Brightness, On/Off
    frame[pos++] = (channel >> 8) & 0xFF;
    frame[pos++] = channel & 0xFF;

    frame[pos++] = (brightness >> 8) & 0xFF;
    frame[pos++] = brightness & 0xFF;

    frame[pos++] = (on_off >> 8) & 0xFF;
    frame[pos++] = on_off & 0xFF;

    // Append CRC16
    uint16_t crc = Calculate_HMI_CRC(frame, pos);
    frame[pos++] = crc & 0xFF;          // CRC Low byte
    frame[pos++] = (crc >> 8) & 0xFF;   // CRC High byte

    // Send frame to HMI
    if (driverFunc(driverObj, frame, pos))
        return 1;

    return 0;
}


void process_led_config_response(HmiProtocolObj *obj, uint16_t bytes_received, uint8_t *pConfig_frame, uint8_t device_index)
{
    // Minimum Modbus response length for Write Multiple Registers (0x10)
    // [slave][0x10][addr_hi][addr_lo][qty_hi][qty_lo][CRC_lo][CRC_hi]
    const uint8_t MIN_RESPONSE_LENGTH = 8;
    
    if (bytes_received < MIN_RESPONSE_LENGTH) {
        log_info("LED config response too short: %d bytes", bytes_received);
        return;
    }

    HmiBufferStr *rxBuf = (HmiBufferStr*)obj->member.rxQueue->data + device_index;
    uint8_t *buf = rxBuf->code;

    // Verify slave address matches request
    if (buf[0] != pConfig_frame[0]) {
        log_info("Slave address mismatch: expected 0x%02X, got 0x%02X", 
                pConfig_frame[0], buf[0]);
        return;
    }

    // Verify function code (0x10 for Write Multiple Registers)
    if (buf[1] != MODBUS_MULTIPLE_REGISTER) {
        log_info("Invalid function code: 0x%02X", buf[1]);
        return;
    }

    // Verify CRC
    uint16_t received_crc = ((uint16_t)buf[bytes_received - 1] << 8) | 
                           (uint16_t)buf[bytes_received - 2];
    uint16_t calc_crc = Calculate_HMI_CRC(buf, bytes_received - 2);

    if (calc_crc != received_crc) {
        log_info("CRC error - received: 0x%04X, calculated: 0x%04X", 
                received_crc, calc_crc);
        return;
    }

    // Verify register address and quantity match request
    uint16_t resp_addr = ((uint16_t)buf[2] << 8) | buf[3];
    uint16_t resp_qty = ((uint16_t)buf[4] << 8) | buf[5];
    uint16_t req_addr = ((uint16_t)pConfig_frame[2] << 8) | pConfig_frame[3];
    uint16_t req_qty = ((uint16_t)pConfig_frame[4] << 8) | pConfig_frame[5];

    if (resp_addr != req_addr || resp_qty != req_qty) {
        log_info("Address/quantity mismatch - Addr: got 0x%04X expected 0x%04X, Qty: got %d expected %d",
                resp_addr, req_addr, resp_qty, req_qty);
        return;
    }

    // Update device state if valid response
    if (obj->member.listIndex[REGISTER_UPDATE_STATUS] && obj->member.listIndex[REGISTER_UPDATE_STATUS]->ptr) {
        *((uint16_t*)obj->member.listIndex[REGISTER_UPDATE_STATUS]->ptr) = HMI_STATUS_SUCCESS;
    }

    // Update status in HMI object
    if (obj->member.listIndex[REGISTER_UPDATE_STATUS]) {
        *((uint16_t*)obj->member.listIndex[REGISTER_UPDATE_STATUS]->ptr) = HMI_STATUS_SUCCESS;
    }

    log_info("LED config successful for device %d, addr:0x%04X, qty:%d", 
             device_index, resp_addr, resp_qty);
}


void HMI_Send_LED_Frame(HmiProtocolObj *obj,
                        uint8_t slave_id,
                        uint16_t start_addr,
                        uint16_t channel,
                        uint16_t brightness,
                        uint16_t on_off,
                        uint8_t mode)
{
    uint8_t frame[PACKAGE_SIZE] = {0};
    uint8_t pos = 0;
    uint16_t register_count;
    uint8_t byte_count;

    if (mode == 0)  // Write LED Config
    {
        register_count = 3;  // channel, brightness, on_off
        byte_count = register_count * 2;

        frame[pos++] = slave_id;
        frame[pos++] = MODBUS_MULTIPLE_REGISTER;

        frame[pos++] = (start_addr >> 8) & 0xFF;
        frame[pos++] = start_addr & 0xFF;

        frame[pos++] = (register_count >> 8) & 0xFF;
        frame[pos++] = register_count & 0xFF;

        frame[pos++] = byte_count;

        // Payload
        frame[pos++] = (channel >> 8) & 0xFF;
        frame[pos++] = channel & 0xFF;

        frame[pos++] = (brightness >> 8) & 0xFF;
        frame[pos++] = brightness & 0xFF;

        frame[pos++] = (on_off >> 8) & 0xFF;
        frame[pos++] = on_off & 0xFF;
    }
    else if (mode == 1) // Read LED Status
    {
        register_count = 0x01;
        frame[pos++] = slave_id;
        frame[pos++] = MODBUS_MULTIPLE_REGISTER;

        frame[pos++] = (start_addr >> 8) & 0xFF;
        frame[pos++] = start_addr & 0xFF;

        frame[pos++] = (register_count >> 8) & 0xFF;
        frame[pos++] = register_count & 0xFF;
    }

    // Compute CRC
    uint16_t crc = Calculate_HMI_CRC(frame, pos);
    frame[pos++] = crc & 0xFF;
    frame[pos++] = (crc >> 8) & 0xFF;

    // Send using HMI transmit
    HmiBufferStr txBuffer;
    memcpy(txBuffer.code, frame, pos);
    txBuffer.size = pos;

    // obj->func.transmitData(obj, &txBuffer, obj->member.txQueue->lowPriority);
    if (obj != NULL && obj->member.txData != NULL && obj->member.txData->buffer != NULL)
    {
        HmiBufferStr *savePtr = (HmiBufferStr*)obj->member.txData->buffer + obj->member.txData->index;
        /* copy only the actual size, not the whole PACKAGE_SIZE */
        memcpy(savePtr->code, txBuffer.code, (size_t)txBuffer.size);
        savePtr->size = txBuffer.size;
    }
}



void process_device_id(HmiProtocolObj *obj, uint16_t bytes_received, uint8_t device_index)
{
    if (bytes_received < 8) { // Modbus read response minimum length (1+1+1+2+2CRC)
        log_info("Device ID response too short: %d bytes", bytes_received);
        return;
    }

    // Get receive buffer for this device
    HmiBufferStr *rxBuf = (HmiBufferStr*)obj->member.rxQueue->data + device_index;
    uint8_t *buf = rxBuf->code;

    // Verify CRC
    uint16_t received_crc = ((uint16_t)buf[bytes_received - 1] << 8) | (uint16_t)buf[bytes_received - 2];
    uint16_t calc_crc = Calculate_HMI_CRC(buf, bytes_received - 2);

    if (calc_crc != received_crc) {
        log_info("CRC error - received: 0x%04X, calculated: 0x%04X", received_crc, calc_crc);
        return;
    }

    uint8_t slave_addr = buf[0];
    uint8_t func_code = buf[1];
    uint8_t byte_count = buf[2];

    if (func_code != READ_HOLDING_REGISTERS) {
        log_info("Invalid function code in Device ID response: 0x%02X", func_code);
        return;
    }

    // For Read Holding Registers, data starts after byte count
    if (bytes_received < (3 + byte_count + 2)) { // header + data + CRC
        log_info("Response too short for declared byte count");
        return;
    }

    // Process register values - assume 2 bytes per register
    for (uint8_t i = 0; i < byte_count/2; i++) {
        uint16_t reg_value = ((uint16_t)buf[3 + i*2] << 8) | buf[4 + i*2];
        
        // Update device info in HMI structure
        if (obj->member.listIndex[REGISTER_UPDATE_STATUS] && obj->member.listIndex[REGISTER_UPDATE_STATUS]->ptr) {
            *((uint16_t*)obj->member.listIndex[REGISTER_UPDATE_STATUS]->ptr) = reg_value;
        }
        
        log_info("Device ID Response -> Slave:0x%02X Register[%d]:0x%04X",
                 slave_addr, i, reg_value);
    }

    // Update device state if needed

}

void process_channel_status(HmiProtocolObj *obj, uint16_t bytes_received, uint8_t *pConfig_frame, uint8_t device_index)
{
    // Minimum Modbus response length for Read Holding Registers (0x03)
    // [slave][0x03][byte_count][data...][CRC_lo][CRC_hi]
    const uint8_t MIN_RESPONSE_LENGTH = 7;
    
    if (bytes_received < MIN_RESPONSE_LENGTH) {
        log_info("Channel status response too short: %d bytes", bytes_received);
        return;
    }

    HmiBufferStr *rxBuf = (HmiBufferStr*)obj->member.rxQueue->data + device_index;
    uint8_t *buf = rxBuf->code;

    // Verify slave address matches request
    if (buf[0] != pConfig_frame[0]) {
        log_info("Invalid slave address: expected 0x%02X, got 0x%02X", 
                pConfig_frame[0], buf[0]);
        return;
    }

    // Verify function code (0x03 for Read Holding Registers)
    if (buf[1] != READ_HOLDING_REGISTERS) {
        log_info("Invalid function code: 0x%02X", buf[1]);
        return;
    }

    uint8_t byte_count = buf[2];
    if (bytes_received < (3 + byte_count + 2)) { // header + data + CRC
        log_info("Response too short for byte count %d", byte_count);
        return;
    }

    // Verify CRC
    uint16_t received_crc = ((uint16_t)buf[bytes_received - 1] << 8) | 
                           (uint16_t)buf[bytes_received - 2];
    uint16_t calc_crc = Calculate_HMI_CRC(buf, bytes_received - 2);

    if (calc_crc != received_crc) {
        log_info("CRC error - received: 0x%04X, calculated: 0x%04X", 
                received_crc, calc_crc);
        return;
    }

    // Extract brightness and status values
    uint16_t brightness = ((uint16_t)buf[3] << 8) | buf[4];
    uint16_t status = ((uint16_t)buf[5] << 8) | buf[6];

    // Update HMI registers if values changed
    if (obj->member.listIndex[LED_BRIGHTNESS]) {
        uint16_t current_brightness = *((uint16_t*)obj->member.listIndex[LED_BRIGHTNESS]->ptr);
        if (current_brightness != brightness) {
            *((uint16_t*)obj->member.listIndex[LED_BRIGHTNESS]->ptr) = brightness;
        }
    }

    if (obj->member.listIndex[LED_SWITCH]) {
        uint16_t current_status = *((uint16_t*)obj->member.listIndex[LED_SWITCH]->ptr);
        if (current_status != status) {
            // *((uint16_t*)obj->member.listIndex[LED_SWITCH]->ptr) = status;
         //app_cmd_t c = { .dev_id= DEV_LED , .u.led.ch = LED_SWITCH, .u.led.cmd = status };
         //xQueueSend(q_cmds, &c, 0);

        }
    }

    // // If either value changed, notify through command queue
    // if (HIMCommandQueueHandle != NULL) {
    //     hmi_command_req_t cmd = {
    //         .uart_port = device_index,
    //         .sensor_index = device_index
    //     };
    //     memcpy(cmd.buf, buf, bytes_received);
    //     xQueueSend(HIMCommandQueueHandle, &cmd, 0);
    // }

    log_info("Channel status updated - Device:%d, Brightness:0x%04X, Status:0x%04X",
             device_index, brightness, status);
}