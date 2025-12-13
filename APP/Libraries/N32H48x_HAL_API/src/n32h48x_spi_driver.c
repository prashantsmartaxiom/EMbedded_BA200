/***********************************************************************************************************************
File Name: n32h48x_spi_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    N32H48x SPI transmission and reception driver.
    The driver shall create and fill in the required data in the driver's parameters before executing any function.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/02/2025  Fred Huang      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_spi_driver.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
Create_SPI_Master_Object(masterSpi1, SPI2);

//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void SPI_Master_Init(SpiMasterObj *obj)
Input:
    *obj    - Object of SPI peripheral and parameters of state machine for SPI driver.
Output:
    None.
Comment:
    Master mode SPI initialization function.
**************************************************************************************************/
void SPI_Master_Init(SpiMasterObj *obj)
{
    // Wait till SPI module is free then disable SPI
    while (obj->member.reg.STS->bit.BUSY)
        __NOP();
    obj->member.reg.CTRL2->bit.SPIEN = 0;
    // SPI baud rate = 120MHz / 128 = 937.5 kHz
    obj->member.reg.CTRL1->bit.BR = SPI_PCLK_DIV_128;
    // SPI mode
    // Mode 0 (CPOL=0, CPHA=0): CLK idle state = low, data sampled on rising edge.
    // Mode 1 (CPOL=0, CPHA=1): CLK idle state = low, data sampled on the falling edge.
    // Mode 2 (CPOL=1, CPHA=0): CLK idle state = high, data sampled on the falling edge.
    // Mode 3 (CPOL=1, CPHA=1): CLK idle state = high, data sampled on the rising edge.
    obj->member.reg.CTRL1->bit.CLKPOL = SPI_MODE_3 >> 1;
    obj->member.reg.CTRL1->bit.CLKPHA = SPI_MODE_3 & 0x1;
    // Transceive data in 8-bit
    obj->member.reg.CTRL1->bit.DATFF = SPI_8bits_Data;
    // Frame format, decide LSB or MSB transmit/receive first.
    obj->member.reg.CTRL1->bit.LSBFF = SPI_MSB_FIRST;
    // Enable software control slave select
    obj->member.reg.CTRL1->bit.SSMEN = 1;
    obj->member.reg.CTRL1->bit.SSEL = 1;
    // Set as master device
    obj->member.reg.CTRL1->bit.MSEL = 1;
    // Enable SPI module
    obj->member.reg.CTRL2->bit.SPIEN = 1;
}
/**************************************************************************************************
Function Name:
    u8 SPI_Master_Transmit(SpiMasterObj *obj, void *data, u8 length)
Input:
    *obj    - Object of SPI peripheral and parameters of state machine for SPI driver.
    *data   - Transmit data pointer
    length  - Transmit data length
Output:
    return  - Result. 1 = data transmitted, 0 = no data transmitted.
Comment:
    SPI master mode sequence transmit function. The parameters of state machine shall be specified
before executing this function.
**************************************************************************************************/
u8 SPI_Master_Transmit(SpiMasterObj *obj, void *data, u8 length)
{
    u8 *dataBuffer = data, txCount = 0;
    u16 timeOut = 0;

    while (length > txCount)
    {
        if (obj->member.reg.STS->bit.TE)
        {
            obj->member.reg.DAT->bit.DAT = *(dataBuffer + txCount);
            txCount++;
        }
        else if (timeOut < 1000)
        {
            timeOut++;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}
/**************************************************************************************************
Function Name:
    u8 SPI_Master_Receive(SpiMasterObj *obj, void *data, u8 length)
Input:
    *obj    - Object of SPI peripheral and parameters of state machine for SPI driver.
    *data   - Receive data pointer
    length  - Receive data length
Output:
    return   - Result. 1 = data received, 0 = no data received.
Comment:
    SPI master mode sequence receive function. The parameters of state machine shall be specified
before executing this function.
**************************************************************************************************/
u8 SPI_Master_Receive(SpiMasterObj *obj, void *data, u8 length)
{
    u8 *dataBuffer = data, rxCount = 0;
    u16 timeOut = 0;

    while (length > rxCount)
    {
        if (obj->member.reg.STS->bit.RNE)
        {
            *(dataBuffer + rxCount) = obj->member.reg.DAT->bit.DAT;
            rxCount++;
        }
        else if (timeOut < 1000)
        {
            timeOut++;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

//----------------------------------------------------------------------------------------------------------------------
