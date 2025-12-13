/***********************************************************************************************************************
File Name: n32h48x_spi_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file of n32h48x_spi_driver.c.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/02/2024  Fred Huang      Create file

***********************************************************************************************************************/

#ifndef _N32H48X_SPI_DRIVER_H_
#define _N32H48X_SPI_DRIVER_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_register.h"

//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define Create_SPI_Master_Object(ObjName, \
                                 RegName)       SpiMasterObj ObjName = \
                                                { \
                                                    { \
                                                        SPI_Register_Group(_##RegName) \
                                                    }, \
                                                    { \
                                                        SPI_Master_Init, \
                                                        SPI_Master_Transmit, \
                                                        SPI_Master_Receive \
                                                    }, \
                                                }

// Statement Replacement
#define SPI_Register_Group(RegName)             { \
                                                    &RegName##_CTRL1, \
                                                    &RegName##_CTRL2, \
                                                    &RegName##_STS, \
                                                    &RegName##_DAT, \
                                                    &RegName##_CRCTDAT, \
                                                    &RegName##_CRCRDAT, \
                                                    &RegName##_CRCPOLY, \
                                                    &RegName##_RX_FIFO, \
                                                    &RegName##_FIFO_NUM, \
                                                    &RegName##_FIFO_CNT, \
                                                    &RegName##_TRANS_NUM, \
                                                    &RegName##_CR3 \
                                                }

//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration
typedef enum
{
    SPI_PCLK_DIV_2      = 0,
    SPI_PCLK_DIV_4      = 1,
    SPI_PCLK_DIV_8      = 2,
    SPI_PCLK_DIV_16     = 3,
    SPI_PCLK_DIV_32     = 4,
    SPI_PCLK_DIV_64     = 5,
    SPI_PCLK_DIV_128    = 6,
    SPI_PCLK_DIV_256    = 7
} SpiBRPrescaler;

typedef enum
{
    SPI_8bits_Data      = 0,
    SPI_16bits_Data     = 1
} SpiDataType;

typedef enum
{
    SPI_MODE_0          = 0,
    SPI_MODE_1          = 1,
    SPI_MODE_2          = 2,
    SPI_MODE_3          = 3
} SpiClkMode;

typedef enum
{
    SPI_MSB_FIRST       = 0,
    SPI_LSB_FIRST       = 1
} SpiFrameFormat;

// Struct & Union
// SPI module register map
typedef struct
{
    volatile _SPIx_CTRL1 *CTRL1;
    volatile _SPIx_CTRL2 *CTRL2;
    volatile _SPIx_STS *STS;
    volatile _SPIx_DAT *DAT;
    volatile _SPIx_CRCTDAT *CRCTDAT;
    volatile _SPIx_CRCRDAT *CRCRDAT;
    volatile _SPIx_CRCPOLY *CRCPOLY;
    volatile _SPIx_RX_FIFO *RX_FIFO;
    volatile _SPIx_FIFO_NUM *FIFO_NUM;
    volatile _SPIx_FIFO_CNT *FIFO_CNT;
    volatile _SPIx_TRANS_NUM *TRANS_NUM;
    volatile _SPIx_CR3 *CR3;
} SpiRegister;

typedef struct SpiMasterStr SpiMasterObj;
struct SpiMasterStr
{
    struct
    {
        SpiRegister reg;
    } member;

    struct
    {
        void (*init)(SpiMasterObj *);
        u8 (*transmit)(SpiMasterObj *, void *, u8);
        u8 (*receive)(SpiMasterObj *, void *, u8);
    } func;
};

//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
void SPI_Master_Init(SpiMasterObj *obj);
u8 SPI_Master_Transmit(SpiMasterObj *obj, void *data, u8 length);
u8 SPI_Master_Receive(SpiMasterObj *obj, void *data, u8 length);

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern SpiMasterObj masterSpi1;

//----------------------------------------------------------------------------------------------------------------------
#endif /* _N32H48X_SPI_DRIVER_H_ */
