/***********************************************************************************************************************
File Name: i2c_device_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file of i2c_device_driver.c.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    08/06/2025  Wilson Chen      Create file

***********************************************************************************************************************/

#ifndef _I2C_DEVICE_DRIVER_H_
#define _I2C_DEVICE_DRIVER_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_i2c_driver.h"

//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define Create_I2C_Master_Command(Name, \
                                 List_Length, \
                                 Index_Length)          extern I2cMasterCommandInfo *Name##Index[]; \
                                                        extern const I2cMasterCommandInfo Name##List[]; \
                                                        I2cMasterCommandArray Name = \
                                                        { \
                                                            Name##List, List_Length, \
                                                            Name##Index, Index_Length \
                                                        }; \
                                                        I2cMasterCommandInfo *Name##Index[Index_Length]; \
                                                        const I2cMasterCommandInfo Name##List[List_Length]

#define MP5920_ADDR_BASE       0x40
#define MP8880A_MFR_ADDRESS    0x00
#define TPT29555A_ADDRESS      0x20
//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration
typedef enum
{
    MP5920_CMD_READ_POUT       = 0x96
} Mp5920SlaveCommand;

typedef enum
{
    MP8880A_CMD_READ_VOUT      = 0x8B,
    MP8880A_CMD_READ_IOUT      = 0x8C
} Mp8880aSlaveCommand;

typedef enum
{
    TPT29555A_CMD_INPUT_0      = 0x00,
    TPT29555A_CMD_INPUT_1      = 0x01
} Tpt29555aSlaveCommand;

// Struct & Union
// Command structure
typedef struct
{
    u8 cmdCode;                     // Command code
    I2cTransmitReceiveType cmdType; // Command type
    void *dataPtr;                    // Data buffer pointer
    u8 dataSize;                    // Data size that it didn't include byte count and pec.
} I2cMasterCommandInfo;

typedef struct
{
    const I2cMasterCommandInfo *list;
    u32 listLength;
    I2cMasterCommandInfo **index;
    u32 indexLength;
} I2cMasterCommandArray;

// Register atructure
typedef struct
{    
    uint16_t  Shade_outputPower[4];
    uint16_t  EXP_LED_outputPower[8];
} Mp5920RegisterMap;

typedef struct
{
    uint16_t  IO_VOUT[4];
    uint16_t  IO_IOUT[4];
    uint16_t  FAN_VOUT[2];
    uint16_t  FAN_IOUT[2];
    uint16_t  RS485_VOUT[4];
    uint16_t  RS485_IOUT[4];
} Mp8880aRegisterMap;

typedef union
{
    uint8_t all;
    struct
    {
        uint8_t PORT0_0         : 1;
        uint8_t PORT0_1         : 1;
        uint8_t PORT0_2         : 1;
        uint8_t PORT0_3         : 1;
        uint8_t PORT0_4         : 1;
        uint8_t PORT0_5         : 1;
        uint8_t PORT0_6         : 1;
        uint8_t PORT0_7         : 1;
    } bit;
} Tpt29555aPort0Register;

typedef union
{
    uint8_t all;
    struct
    {
        uint8_t PORT1_0         : 1;
        uint8_t PORT1_1         : 1;
        uint8_t PORT1_2         : 1;
        uint8_t PORT1_3         : 1;
        uint8_t PORT1_4         : 1;
        uint8_t PORT1_5         : 1;
        uint8_t PORT1_6         : 1;
        uint8_t PORT1_7         : 1;
    } bit;
} Tpt29555aPort1Register;

typedef struct
{
    Tpt29555aPort0Register InputPort0;
    Tpt29555aPort1Register InputPort1;
} Tpt29555aRegisterMap;

//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
//void MpXXXX_Command_List_Setup(MpXXXXCommandArray *command);
void MpXXXX_Communication_Process(I2cMasterObj *obj2, I2cMasterObj *obj3);
void IO_Expander_Communication_Process(I2cMasterObj *obj);
//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern I2cMasterCommandArray I2cMasterCommand;
extern Mp5920RegisterMap Mp5920Register;
extern Mp8880aRegisterMap Mp8880aRegister;
//----------------------------------------------------------------------------------------------------------------------
#endif /* _I2C_DEVICE_DRIVER_H_ */
