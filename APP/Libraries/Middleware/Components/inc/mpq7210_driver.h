/***********************************************************************************************************************
File Name: mpq7210_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file of mpq7210_driver.c.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/07/2024  Fred Huang      Create file

***********************************************************************************************************************/

#ifndef _MPQ7210_DRIVER_H_
#define _MPQ7210_DRIVER_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_spi_driver.h"
//#include "circular_buffer.h"
//#include "io.h"
#include "main.h"
//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define MPQ7210_Queue_Length        128

#define Create_MPQ7210_Priority_Queue(Name, \
                                      Size)     QueueDataStr Name##LowData[Size], Name##HighData[Size]; \
                                                PriorQueueRegStr Name = \
                                                {\
                                                   {0, 0, 0, 0, 0, Name##LowData, sizeof(Name##LowData) >> 3}, \
                                                   {0, 0, 0, 0, 0, Name##HighData, sizeof(Name##HighData) >> 3}, \
                                                   0 \
                                                }

#define Create_MPQ7210_Master_Command(Name, \
                                      List_Length, \
                                      Index_Length)     extern Mpq7210CommandInfo *Name##Index[]; \
                                                        extern const Mpq7210CommandInfo Name##List[]; \
                                                        Mpq7210CommandArray Name = \
                                                        { \
                                                            Name##List, List_Length, \
                                                            Name##Index, Index_Length \
                                                        }; \
                                                        Mpq7210CommandInfo *Name##Index[Index_Length]; \
                                                        const Mpq7210CommandInfo Name##List[List_Length]
#define MPQ7210_ROUTINE_TASK_STACK_SIZE        (1024)
#define MPQ7210_ROUTINE_TASK_PRIORITY          (2)
#define LED_DIMMING_TASK_STACK_SIZE            (256)
#define LED_DIMMING_TASK_PRIORITY              (1)
//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration
// Communication related
typedef enum
{
    SIL_REV                 = 0x01,
    DEV_CFG                 = 0x03,
    PWM_DIM1                = 0x04,
    PWM_DIM2                = 0x05,
    ANA_DIM1                = 0x06,
    ANA_DIM2                = 0x07,
    INT_ALL                 = 0x08,
    INT_CFG                 = 0x09,
    FS_INT_STATUS           = 0x0A,
    FS_INT_RAW_STATUS       = 0x0B,
    FS_INT_EN               = 0x0C,
    FS_INT_MASK_EN          = 0x0D,
    FS_INT_REAL_TIME_STATUS = 0x0E,
    BUCK1_CFG               = 0x10,
    BUCK2_CFG               = 0x11,
    ADC_CTRL                = 0x12,
    ADC_OUT                 = 0x13,
    OTP_STATUS              = 0x15,
    BUCK_EN                 = 0x20,
    NOT_SUPPORT             = 0xFF
} Mpq7210MasterCommand;

typedef enum
{
    MPQ7210_NOT_SUPPORT     = 0,
    MPQ7210_READ_WRITE      = 1,
    MPQ7210_READ_ONLY       = 2,
    MPQ7210_READ_CLEAR      = 3,
    MPQ7210_WRITE_ONLY      = 4,
    MPQ7210_WRITE_CLEAR     = 5
} Mpq7210CommandType;

typedef enum
{
    MPQ7210_NOT_PACKET      = 0
} Mpq7210DataType;

typedef enum
{
    SPI_PORT_0              = 0,
    SPI_PORT_1              = 1,
    SPI_PORT_2              = 2,
    SPI_PORT_3              = 3,
    SPI_PORT_4              = 4,
    SPI_PORT_5              = 5,
    SPI_PORT_6              = 6,
    SPI_PORT_7              = 7,
    SPI_PORT_8              = 8,
    SPI_PORT_9              = 9,
    SPI_PORT_10             = 10,
    SPI_PORT_11             = 11,
    SPI_PORT_12             = 12,
    SPI_PORT_13             = 13,
    SPI_PORT_14             = 14,
    SPI_PORT_15             = 15,
    SPI_PORT_16             = 16,
    SPI_PORT_17             = 17,
    SPI_PORT_18             = 18,
    SPI_PORT_19             = 19,
    SPI_PORT_20             = 20,
    SPI_PORT_21             = 21,
    SPI_PORT_22             = 22
} SpiPortNum;
// LED control related
typedef enum
{
    LED_OFF                 = 0,
    LED_ON                  = 1
} LedStatus;


// Struct & Union
// Command structure
typedef struct
{
    u8 devicePort;                  // SPI port
    u8 regCode;                     // Register address
    u16 regData;                    // Register data, 2bytes
} Mpq7210TxBuffer;

typedef struct
{
    u8 cmdCode;                     // Command code
    Mpq7210CommandType cmdType;     // Command type
    Mpq7210DataType dataType;       // Data type
    u8 *dataPtr;                    // Data buffer pointer
    u8 dataSize;                    // Data size that it didn't include byte count and pec.
} Mpq7210CommandInfo;

typedef struct
{
    const Mpq7210CommandInfo *list;
    u32 listLength;
    Mpq7210CommandInfo **index;
    u32 indexLength;
} Mpq7210CommandArray;

// Register atructure
typedef union
{
    u16 all;
    struct
    {
        u16 SIL_INF         : 8;
        u16 Reserved        : 7;
        u16 OTP             : 1;
    } bit;
} SiliconRevisionRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 DIM2_FREQ       : 2;
        u16 DIM1_FREQ       : 2;
        u16 DIM2_MODE       : 2;
        u16 DIM1_MODE       : 2;
        u16 PHASE           : 2;
        u16 Reserved        : 6;
    } bit;
} DeviceConfigurationRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 DIM1_DUTY       : 12;
        u16 Reserved        : 4;
    } bit;
} PwmDimming1Register;

typedef union
{
    u16 all;
    struct
    {
        u16 DIM2_DUTY       : 12;
        u16 Reserved        : 4;
    } bit;
} PwmDimming2Register;

typedef union
{
    u16 all;
    struct
    {
        u16 ANA_DIM1        : 8;
        u16 Reserved        : 8;
    } bit;
} AnalogDimming1Register;

typedef union
{
    u16 all;
    struct
    {
        u16 ANA_DIM2        : 8;
        u16 Reserved        : 8;
    } bit;
} AnalogDimming2Register;

typedef union
{
    u16 all;
    struct
    {
        u16 INT_STATUS      : 1;
        u16 Reserved        : 15;
    } bit;
} InterruptAllRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 TSD             : 2;
        u16 Reserved        : 6;
        u16 BUCK2_UVP_ACT   : 2;
        u16 BUCK1_UVP_ACT   : 2;
        u16 BUCK2_OCP_ACT   : 2;
        u16 BUCK1_OCP_ACT   : 2;
    } bit;
} InterruptConfigurationRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 Reserved1                   : 2;
        u16 OTP_CRC_FAIL_INT_STATUS     : 1;
        u16 OTP_PROG_FAIL_INT_STATUS    : 1;
        u16 OTP_IND_FAIL_INT_STATUS     : 1;
        u16 BST2_UV_INT_STATUS          : 1;
        u16 BST1_UV_INT_STATUS          : 1;
        u16 TSD_INT_STATUS              : 1;
        u16 TWN_INT_STATUS              : 1;
        u16 PHASE_INT_STATUS            : 1;
        u16 BUCK2_UVP_INT_STATUS        : 1;
        u16 Reserved2                   : 1;
        u16 BUCK2_OCP_INT_STATUS        : 1;
        u16 BUCK1_UVP_INT_STATUS        : 1;
        u16 Reserved3                   : 1;
        u16 BUCK1_OCP_INT_STATUS        : 1;
    } bit;
} FaultStatusRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 Reserved1                       : 2;
        u16 OTP_CRC_FAIL_INT_RAW_STATUS     : 1;
        u16 OTP_PROG_FAIL_INT_RAW_STATUS    : 1;
        u16 OTP_IND_FAIL_INT_RAW_STATUS     : 1;
        u16 BST2_UV_INT_RAW_STATUS          : 1;
        u16 BST1_UV_INT_RAW_STATUS          : 1;
        u16 TSD_INT_RAW_STATUS              : 1;
        u16 TWN_INT_RAW_STATUS              : 1;
        u16 PHASE_INT_RAW_STATUS            : 1;
        u16 BUCK2_UVP_INT_RAW_STATUS        : 1;
        u16 Reserved2                       : 1;
        u16 BUCK2_OCP_INT_RAW_STATUS        : 1;
        u16 BUCK1_UVP_INT_RAW_STATUS        : 1;
        u16 Reserved3                       : 1;
        u16 BUCK1_OCP_INT_RAW_STATUS        : 1;
    } bit;
} FaultRawStatusRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 Reserved1           : 2;
        u16 OTP_CRC_FAIL_EN     : 1;
        u16 OTP_PROG_FAIL_EN    : 1;
        u16 OTP_IND_FAIL_EN     : 1;
        u16 BST2_UV_EN          : 1;
        u16 BST1_UV_EN          : 1;
        u16 TSD_EN              : 1;
        u16 TWN_EN              : 1;
        u16 PHASE_EN            : 1;
        u16 BUCK2_UVP_EN        : 1;
        u16 Reserved2           : 1;
        u16 BUCK2_OCP_EN        : 1;
        u16 BUCK1_UVP_EN        : 1;
        u16 Reserved3           : 1;
        u16 BUCK1_OCP_EN        : 1;
    } bit;
} FaultEnableRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 Reserved1               : 2;
        u16 OTP_CRC_FAIL_MASK_EN    : 1;
        u16 OTP_PROG_FAIL_MASK_EN   : 1;
        u16 OTP_IND_FAIL_MASK_EN    : 1;
        u16 BST2_UV_MASK_EN         : 1;
        u16 BST1_UV_MASK_EN         : 1;
        u16 TSD_MASK_EN             : 1;
        u16 TWN_MASK_EN             : 1;
        u16 PHASE_MASK_EN           : 1;
        u16 BUCK2_UVP_MASK_EN       : 1;
        u16 Reserved2               : 1;
        u16 BUCK2_OCP_MASK_EN       : 1;
        u16 BUCK1_UVP_MASK_EN       : 1;
        u16 Reserved3               : 1;
        u16 BUCK1_OCP_MASK_EN       : 1;
    } bit;
} FaultEnableMaskRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 Reserved1           : 8;
        u16 TSD_STATUS          : 1;
        u16 TWN_STATUS          : 1;
        u16 BUCK2_UVP_STATUS    : 1;
        u16 Reserved2           : 1;
        u16 BUCK2_OCP_STATUS    : 1;
        u16 BUCK1_UVP_STATUS    : 1;
        u16 Reserved3           : 1;
        u16 BUCK1_OCP_STATUS    : 1;
    } bit;
} FaultRealTimeStatusRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 BUCK1_OCP_SEL       : 3;
        u16 BUCK1_FSS_EN        : 1;
        u16 BUCK1_FREQ_SEL      : 2;
        u16 Reserved            : 10;
    } bit;
} Buck1ConfigurationRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 BUCK2_OCP_SEL       : 3;
        u16 BUCK2_FSS_EN        : 1;
        u16 BUCK2_FREQ_SEL      : 2;
        u16 Reserved            : 10;
    } bit;
} Buck2ConfigurationRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 ADC_INMUX           : 3;
        u16 TRIG_ADC_READ       : 1;
        u16 ADC_READY           : 1;
        u16 CUR_STORED_CH       : 3;
        u16 Reserved            : 8;
    } bit;
} AdcControlRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 ADC_DATA            : 10;
        u16 Reserved            : 6;
    } bit;
} AdcDataRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 Reserved1               : 11;
        u16 USER_PAGE_CRC_ERR_FLAG  : 1;
        u16 TRIM_PAGE_CRC_ERR_FLAG  : 1;
        u16 OTP_CRC_ERR_FLAG        : 1;
        u16 DA_OTP_RDY              : 1;
        u16 Reserved2               : 1;
    } bit;
} OtpStatusRegister;

typedef union
{
    u16 all;
    struct
    {
        u16 Reserved1   : 3;
        u16 BUCK2_EN    : 1;
        u16 Reserved2   : 3;
        u16 BUCK1_EN    : 1;
        u16 Reserved3   : 8;
    } bit;
} BuckEnableRegister;

typedef struct
{
    SiliconRevisionRegister         siliconRev;
    DeviceConfigurationRegister     deviceConfig;
    PwmDimming1Register             pwmDimming1;
    PwmDimming2Register             pwmDimming2;
    AnalogDimming1Register          analogDimming1;
    AnalogDimming2Register          analogDimming2;
    InterruptAllRegister            intteruptStatus;
    InterruptConfigurationRegister  interruptConfig;
    FaultStatusRegister             faultStatus;
    FaultRawStatusRegister          faultRawStatus;
    FaultEnableRegister             faultEnable;
    FaultEnableMaskRegister         faultMask;
    FaultRealTimeStatusRegister     faultRealTimeStatus;
    Buck1ConfigurationRegister      buck1Config;
    Buck2ConfigurationRegister      buck2Config;
    AdcControlRegister              adcControl;
    AdcDataRegister                 adcData;
    OtpStatusRegister               otpStatus;
    BuckEnableRegister              buckEnable;
} Mpq7210RegisterMap;

//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
void Mpq7210_Command_List_Setup(Mpq7210CommandArray *command);
void Mpq7210_Command_Pack_Enqueue(Mpq7210MasterCommand cmdIndex, u8 devicePort, u8 direction, u8 priority);
void Mpq7210_Communication_Process(SpiMasterObj *obj, PriorQueueRegStr *queueRegPtr);
extern void mpq7210_init(void);
//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern Mpq7210CommandArray mpq7210Command;
extern PriorQueueRegStr mpq7210PriorityQueue;
extern uint16_t mpq7210RegisterVout[22];
extern uint8_t max_led_pwm_duty, brightness, color_temp;
//----------------------------------------------------------------------------------------------------------------------
#endif /* _MPQ7210_DRIVER_H_ */
