/***********************************************************************************************************************
File Name: gui_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file of gui_driver.c.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/20/2024  Fred Huang      Create file

***********************************************************************************************************************/

#ifndef _GUI_DRIVER_H_
#define _GUI_DRIVER_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_i2c_driver.h"
#include "Boot_Internal_Task.h"

//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define Create_Gui_Slave_Command(Name, \
                                 List_Length, \
                                 Index_Length)          extern GuiCommandInfo *Name##Index[]; \
                                                        extern const GuiCommandInfo Name##List[]; \
                                                        GuiCommandArray Name = \
                                                        { \
                                                            Name##List, List_Length, \
                                                            Name##Index, Index_Length \
                                                        }; \
                                                        GuiCommandInfo *Name##Index[Index_Length]; \
                                                        const GuiCommandInfo Name##List[List_Length]

#define MFR_MODE_ENTER_KEY      0x55AA
//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration
typedef enum
{
    // LED
    LED1_STATUS             = 0x00,
    LED1_CONTROL            = 0x01,
    LED2_STATUS             = 0x02,
    LED2_CONTROL            = 0x03,
    LED3_STATUS             = 0x04,
    LED3_CONTROL            = 0x05,
    LED4_STATUS             = 0x06,
    LED4_CONTROL            = 0x07,
    LED5_STATUS             = 0x08,
    LED5_CONTROL            = 0x09,
    LED6_STATUS             = 0x0A,
    LED6_CONTROL            = 0x0B,
    LED7_STATUS             = 0x0C,
    LED7_CONTROL            = 0x0D,
    LED8_STATUS             = 0x0E,
    LED8_CONTROL            = 0x0F,
    LED9_STATUS             = 0x10,
    LED9_CONTROL            = 0x11,
    LED10_STATUS            = 0x12,
    LED10_CONTROL           = 0x13,
    LED11_STATUS            = 0x14,
    LED11_CONTROL           = 0x15,
    LED12_STATUS            = 0x16,
    LED12_CONTROL           = 0x17,
    LED13_STATUS            = 0x18,
    LED13_CONTROL           = 0x19,
    LED14_STATUS            = 0x1A,
    LED14_CONTROL           = 0x1B,
    LED15_STATUS            = 0x1C,
    LED15_CONTROL           = 0x1D,
    LED16_STATUS            = 0x1E,
    LED16_CONTROL           = 0x1F,
    LED17_STATUS            = 0x20,
    LED17_CONTROL           = 0x21,
    LED18_STATUS            = 0x22,
    LED18_CONTROL           = 0x23,
    LED19_STATUS            = 0x24,
    LED19_CONTROL           = 0x25,
    LED20_STATUS            = 0x26,
    LED20_CONTROL           = 0x27,
    LED21_STATUS            = 0x28,
    LED21_CONTROL           = 0x29,
    LED22_STATUS            = 0x2A,
    LED22_CONTROL           = 0x2B,
    
    // Shade
    SHADE1_ID               = 0x80,
    SHADE1_CONTROL          = 0x81,
    SHADE2_ID               = 0x82,
    SHADE2_CONTROL          = 0x83,
    SHADE3_ID               = 0x84,
    SHADE3_CONTROL          = 0x85,
    SHADE4_ID               = 0x86,
    SHADE4_CONTROL          = 0x87,
    SHADE5_ID               = 0x88,
    SHADE5_CONTROL          = 0x89,
    SHADE6_ID               = 0x8A,
    SHADE6_CONTROL          = 0x8B,
    SHADE7_ID               = 0x8C,
    SHADE7_CONTROL          = 0x8D,
    SHADE8_ID               = 0x8E,
    SHADE8_CONTROL          = 0x8F,
    // PIR sensor
    PIR1_STATUS             = 0xA0,
    PIR2_STATUS             = 0xA1,
    PIR3_STATUS             = 0xA2,
    PIR4_STATUS             = 0xA3,
    PIR5_STATUS             = 0xA4,
    PIR6_STATUS             = 0xA5,
    PIR7_STATUS             = 0xA6,
    PIR8_STATUS             = 0xA7,
    // Bootloader
//    BOOT_PROTECTION         = 0xF0,
//    BOOT_INIT               = 0xF1,
//    BOOT_STATE              = 0xF4,
    // Manufature
    MFR_ENABLE              = 0xE0,
    MFR_MODE                = 0xE1,
    MFR_LED_ALL_MAX         = 0xE2,
    MFR_RS485_TEST          = 0xE3,



    NOT_SUPPORT_CMD         = 0xFF
} GuiSlaveCommand;

typedef enum
{
    GUI_NOT_SUPPORT         = 0,
    GUI_READ_WRITE          = 1,
    GUI_READ_ONLY           = 2,
    GUI_READ_CLEAR          = 3,
    GUI_WRITE_ONLY          = 4,
    GUI_WRITE_CLEAR         = 5
} GuiCommandType;

typedef enum
{
    GUI_NOT_PACKET          = 0,
    GUI_LED                 = 1,
    GUI_SHADE               = 2,
    GUI_PIR                 = 3,
    BOOTLOADER              = 4,
    MANUFACTURE             = 7
} GuiDataType;

typedef enum
{
    GUI_READ                = 0,
    GUI_WRITE               = 1
} GuiDataDirection;

typedef enum
{
    MFR_IDLE_MODE           = 0,
    MFR_CALI_MODE           = 1,
    MFR_BURN_IN_MODE        = 2,
    MFR_COMM_TEST_MODE      = 3
} MfrModePack;

// Struct & Union
// Command structure
typedef struct
{
    u8 deviceAddress;               // I2C address
    u8 regCode;                     // Register address
    u8 regData;                     // Register data
} GuiTxBuffer;

typedef struct
{
    u8 cmdCode;                     // Command code
    GuiCommandType cmdType;         // Command type
    GuiDataType dataType;           // Data type
    void *dataPtr;                    // Data buffer pointer
    u8 dataSize;                    // Data size that it didn't include byte count and pec.
} GuiCommandInfo;

typedef struct
{
    const GuiCommandInfo *list;
    u32 listLength;
    GuiCommandInfo **index;
    u32 indexLength;
} GuiCommandArray;

// Register atructure
typedef union
{
    u8 all;
    struct
    {
        u8 IOUT_LIMIT       : 5;
        u8 Reserved         : 1;
        u8 LED_STATUS       : 2;
    } bit;
} LedStatusRegister;

typedef union
{
    u8 all;
    struct
    {
        u8 BRIGHTNESS       : 5;
        u8 COLOR_TEMP       : 3;
    } bit;
} LedControlRegister;

typedef union
{
    u8 all;
    struct
    {
        u8 SHADE_CONTROL    : 2;
        u8 Reserved         : 6;
    } bit;
} ShadeControlRegister;

typedef union
{
    u8 all;
    struct
    {
        u8 PIR_STATUS       : 2;
        u8 Reserved         : 6;
    } bit;
} PirStatusRegister;

typedef struct
{
    LedStatusRegister       led1Status;
    LedControlRegister      led1Control;
    LedStatusRegister       led2Status;
    LedControlRegister      led2Control;
    LedStatusRegister       led3Status;
    LedControlRegister      led3Control;
    LedStatusRegister       led4Status;
    LedControlRegister      led4Control;
    LedStatusRegister       led5Status;
    LedControlRegister      led5Control;
    LedStatusRegister       led6Status;
    LedControlRegister      led6Control;
    LedStatusRegister       led7Status;
    LedControlRegister      led7Control;
    LedStatusRegister       led8Status;
    LedControlRegister      led8Control;
    LedStatusRegister       led9Status;
    LedControlRegister      led9Control;
    LedStatusRegister       led10Status;
    LedControlRegister      led10Control;
    LedStatusRegister       led11Status;
    LedControlRegister      led11Control;
    LedStatusRegister       led12Status;
    LedControlRegister      led12Control;
    LedStatusRegister       led13Status;
    LedControlRegister      led13Control;
    LedStatusRegister       led14Status;
    LedControlRegister      led14Control;
    LedStatusRegister       led15Status;
    LedControlRegister      led15Control;
    LedStatusRegister       led16Status;
    LedControlRegister      led16Control;
    LedStatusRegister       led17Status;
    LedControlRegister      led17Control;
    LedStatusRegister       led18Status;
    LedControlRegister      led18Control;
    LedStatusRegister       led19Status;
    LedControlRegister      led19Control;
    LedStatusRegister       led20Status;
    LedControlRegister      led20Control;
    LedStatusRegister       led21Status;
    LedControlRegister      led21Control;
    LedStatusRegister       led22Status;
    LedControlRegister      led22Control; 
    u8                      shade1Id;
    ShadeControlRegister    shade1Control;
    u8                      shade2Id;
    ShadeControlRegister    shade2Control;
    u8                      shade3Id;
    ShadeControlRegister    shade3Control;
    u8                      shade4Id;
    ShadeControlRegister    shade4Control;
    PirStatusRegister       pir1Status;
    PirStatusRegister       pir2Status;
    PirStatusRegister       pir3Status;
    PirStatusRegister       pir4Status;
} GuiRegisterMap;

typedef struct
{
    GuiDataDirection direction;
    GuiSlaveCommand cmdCode;
} slaveCommPack;

//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
void Gui_Command_List_Setup(GuiCommandArray *command);
slaveCommPack Gui_Communication_Process(I2cSlaveObj *obj);

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern GuiCommandArray guiCommand;
extern u16 mfrKey;
extern u8 mfrMode, mfrLedResult, mfrRs485Result;

//----------------------------------------------------------------------------------------------------------------------
#endif /* _GUI_DRIVER_H_ */
