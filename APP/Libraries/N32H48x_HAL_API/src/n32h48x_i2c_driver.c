/***********************************************************************************************************************
File Name: n32h48x_i2c_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    N32H48x I2C transmission and reception driver.
    The driver shall create and fill in the required data in the driver's parameters before executing any function.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/14/2025  Fred Huang      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_i2c_driver.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
Create_I2C_Slave_Object(slaveI2c1, I2C1);
Create_I2C_Master_Object(masterI2c2, I2C2);
Create_I2C_Master_Object(masterI2c3, I2C3);
Create_I2C_Master_Object(masterI2c4, I2C4);
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void I2C_Master_Init(I2cMasterObj *obj)
Input:
    *obj    - Object of I2C peripheral and parameters of state machine for I2C driver.
Output:
    None.
Comment:
    Master mode I2C initialization function.
**************************************************************************************************/
void I2C_Master_Init(I2cMasterObj *obj)
{
    // Select the APB1 frequency, CLKFREQ = PCLK / 10
    obj->member.reg.CTRL2->bit.CLKFREQ = I2C_CLKFREQ_REG;

    obj->member.reg.CTRL1->all = 0;
    // Set the duty to Tlow/Thigh = 2
    obj->member.reg.CLKCTRL->bit.DUTY = 0;
    // Set the I2C to fast mode
    obj->member.reg.CLKCTRL->bit.FSMODE = 1;
    // Set the clock spped
    // CLKCTRL = PCLK / CLK_SPEED / DUTY_CYCLE(Tlow + Thigh) = 60e6 / 400e3 / (2 + 1) = 50
    obj->member.reg.CLKCTRL->bit.CLKCTRL = 50;
    // Set SCL rise time, TMRISE = (RISE_TIME_MAX * PCLK) + 1 = (300ns * 60e6) + 1 = 19
    obj->member.reg.TMRISE->bit.TMRISE = 0x13;
    obj->member.reg.CTRL1->bit.EN = 1;

    // Set to I2C mode
    obj->member.reg.CTRL1->bit.SMMODE = 0;
    // Enable ACK after receiving a byte
    obj->member.reg.CTRL1->bit.ACKEN = 1;
    // Set own address and 7-bits addressing mode
    obj->member.reg.OADDR1->bit.ADDRMODE = 0;
    obj->member.reg.OADDR1->bit.ADDR = I2C_MASTER_ADDR;
}
/**************************************************************************************************
Function Name:
    unsigned char I2C_Master_Write(I2cMasterObj *obj, uint8_t cmd, uint8_t addr, uint8_t length, uint8_t dataOnly)
Input:
    *obj     - Object of I2C peripheral and parameters of state machine for I2C driver.
    id       - Slave address.
    *txdPtr  - Pointer of transmit data, the function will sequentially send the specified number
               of bytes from the pointer.
    txdSize  - Size of transmit data, the specified number of bytes.
Output:
    return   - Result. 1 = completed, 0 = not complete.
Comment:
    I2C master mode process transmit function.
**************************************************************************************************/
unsigned char I2C_Master_Write(I2cMasterObj *obj, uint8_t addr, uint8_t cmd, uint8_t length, uint16_t data, uint8_t dataOnly)
{
    u16 timeoutCounter = 0;

    while (obj->member.reg.STS2->bit.BUSY);

    obj->member.reg.CTRL1->bit.STARTGEN = 1;

    while (!obj->member.reg.STS1->bit.STARTBF);

    obj->member.reg.STS1->all;
    obj->member.reg.STS2->all;

    obj->member.reg.DAT->bit.DATA = (addr << 1) & ~0x01;

    while (!obj->member.reg.STS1->bit.ADDRF && timeoutCounter < 1000)
        timeoutCounter++;
    timeoutCounter = 0;

    obj->member.reg.STS1->all;
    obj->member.reg.STS2->all;

    while (!obj->member.reg.STS1->bit.TXDATE && timeoutCounter < 1000)
        timeoutCounter++;
    timeoutCounter = 0;

    obj->member.reg.DAT->bit.DATA = cmd;

    while (!obj->member.reg.STS1->bit.TXDATE && timeoutCounter < 1000)
        timeoutCounter++;
    timeoutCounter = 0;

    if  (dataOnly == I2C_WRITE_ONLY)
    {
        if (length == 1)
        {
            obj->member.reg.DAT->bit.DATA = (uint8_t)(data & 0xFF);

            while (!obj->member.reg.STS1->bit.TXDATE);
        }
        else if (length == 2)
        {
            obj->member.reg.DAT->bit.DATA = (uint8_t)(data & 0xFF);

            while (!obj->member.reg.STS1->bit.TXDATE);

            obj->member.reg.DAT->bit.DATA = (uint8_t)(data >> 8);

            while (!obj->member.reg.STS1->bit.TXDATE);
        }
    }

    while (!obj->member.reg.STS1->bit.BSF && timeoutCounter < 1000)
        timeoutCounter++;
    timeoutCounter = 0;

    if (dataOnly == I2C_WRITE_ONLY)
    {
        obj->member.reg.CTRL1->bit.STOPGEN = 1;

        return 0;
    }

    return 1;
}
/**************************************************************************************************
Function Name:
    unsigned char I2C_Master_Read(I2cMasterObj *obj, void *data, uint8_t addr, uint8_t length)
Input:
    *obj     - Object of I2C peripheral and parameters of state machine for I2C driver.
    id       - Slave address.
    *txdPtr  - Pointer of transmit data, the function will sequentially send the specified number
               of bytes from the pointer.
    txdSize  - Size of transmit data, the specified number of bytes.
    *rxdPtr  - Pointer of receive data, the function will sequentially store the specified number of
               bytes from the register to the pointer.
    rxdSize  - Size of receive data, the specified number of bytes.
Output:
    return   - Result. 1 = completed, 0 = not complete.
Comment:
    I2C master mode process receive function.
**************************************************************************************************/
unsigned char I2C_Master_Read(I2cMasterObj *obj, void *data, uint8_t addr, uint8_t length)
{
    u16 timeoutCounter = 0;

    uint8_t *dataBuffer = data;

    obj->member.reg.CTRL1->bit.ACKEN = 1;

    obj->member.reg.CTRL1->bit.STARTGEN = 1;

    while (!obj->member.reg.STS1->bit.STARTBF);

    obj->member.reg.STS1->all;
    obj->member.reg.STS2->all;

    obj->member.reg.DAT->bit.DATA = (addr << 1) | 0x01;

    while (!obj->member.reg.STS1->bit.ADDRF && timeoutCounter < 1000)
        timeoutCounter++;
    timeoutCounter = 0;

    if (length == 1)
    {
        obj->member.reg.CTRL1->bit.ACKEN = 0;

        obj->member.reg.STS1->all;
        obj->member.reg.STS2->all;

        obj->member.reg.CTRL1->bit.STOPGEN = 1;

        while (!obj->member.reg.STS1->bit.RXDATNE && timeoutCounter < 1000)
        timeoutCounter++;
    timeoutCounter = 0;

        *dataBuffer = obj->member.reg.DAT->bit.DATA;
    }
    else if (length == 2)
    {
        obj->member.reg.CTRL1->bit.ACKPOS = 1;

        obj->member.reg.STS1->all;
        obj->member.reg.STS2->all;

        obj->member.reg.CTRL1->bit.ACKEN = 0;

        while (!obj->member.reg.STS1->bit.BSF && timeoutCounter < 1000)
        timeoutCounter++;
    timeoutCounter = 0;

        obj->member.reg.CTRL1->bit.STOPGEN = 1;

        *dataBuffer = obj->member.reg.DAT->bit.DATA;

        *(dataBuffer + 1) = obj->member.reg.DAT->bit.DATA;
    }
    else
    {
        obj->member.reg.STS1->all;
        obj->member.reg.STS2->all;

        uint8_t received_count = 0;

        while (received_count < length)
        {
            if (received_count == length - 3)
            {
                while (!obj->member.reg.STS1->bit.BSF);

                *(dataBuffer + received_count)  = (uint8_t)obj->member.reg.DAT->bit.DATA;

                received_count ++;
            }
            else if (received_count == length - 2)
            {
                obj->member.reg.CTRL1->bit.ACKEN = 0;

                *(dataBuffer + received_count)  = (uint8_t)obj->member.reg.DAT->bit.DATA;

                received_count ++;

                obj->member.reg.CTRL1->bit.STOPGEN = 1;
            }
            else if (received_count == length - 1)
            {
                while (!obj->member.reg.STS1->bit.RXDATNE);

                *(dataBuffer + received_count)  = (uint8_t)obj->member.reg.DAT->bit.DATA;

                received_count ++;
            }
            else
            {
                while (!obj->member.reg.STS1->bit.RXDATNE);

                *(dataBuffer + received_count)  = (uint8_t)obj->member.reg.DAT->bit.DATA;

                received_count ++;
            }
        }
    }

    while (obj->member.reg.STS2->bit.BUSY);

    return 1;
}
/**************************************************************************************************
Function Name:
    void I2C_Slave_Init(I2cSlaveObj *obj, u16 address)
Input:
    *obj    - Object of I2C peripheral and parameters of state machine for I2C driver.
    address - Slave address
Output:
    None.
Comment:
    Slave mode I2C initialization function.
**************************************************************************************************/
void I2C_Slave_Init(I2cSlaveObj *obj, u16 address)
{
    // Select the APB1 frequency, CLKFREQ = PCLK / 10
    obj->member.reg.CTRL2->bit.CLKFREQ = I2C_CLKFREQ_REG;

    obj->member.reg.CTRL1->all = 0;
    // Set the duty to Tlow/Thigh = 2
    obj->member.reg.CLKCTRL->bit.DUTY = 0;
    // Set the I2C to fast mode
    obj->member.reg.CLKCTRL->bit.FSMODE = 1;
    // Set the clock spped
    // CLKCTRL = PCLK / CLK_SPEED / DUTY_CYCLE(Tlow + Thigh) = 60e6 / 400e3 / (2 + 1) = 50
    obj->member.reg.CLKCTRL->bit.CLKCTRL = 50;
    // Set SCL rise time, TMRISE = (RISE_TIME_MAX * PCLK) + 1 = (300ns * 60e6) + 1 = 19
    obj->member.reg.TMRISE->bit.TMRISE = 0x13;
    obj->member.reg.CTRL1->bit.EN = 1;

    // Set to I2C mode
    obj->member.reg.CTRL1->bit.SMMODE = 0;
    // Enable ACK after receiving a byte
    obj->member.reg.CTRL1->bit.ACKEN = 1;
    // Set own address and 7-bits addressing mode
    obj->member.reg.OADDR1->bit.ADDRMODE = 0;
    obj->member.reg.OADDR1->bit.ADDR = address;
}
/**************************************************************************************************
Function Name:
    u8 I2C_Slave_Transmit(I2cSlaveObj *obj, void *data, u8 length)
Input:
    *obj    - Object of I2C peripheral and parameters of state machine for I2C driver.
    *data   - Transmit data pointer
    length  - Transmit data length
Output:
    return  - Result. 1 = data transmitted, 0 = no data transmitted.
Comment:
    I2C sequence transmit function in slave mode. The parameters of state machine shall be specified
before executing this function.
**************************************************************************************************/
u8 I2C_Slave_Transmit(I2cSlaveObj *obj, void *data, u8 length)
{
    u8 *dataBuffer = data, txCount = 0;
    u16 timeOut = 0;

    if (obj->member.reg.STS1->bit.ADDRF)
    {
        obj->member.reg.STS2->all;
        // STOP after NACk won't set STOP bit
        while (length > txCount || obj->member.reg.STS2->bit.BUSY)
        {
            if (obj->member.reg.STS1->bit.TXDATE)
            {
                obj->member.reg.DAT->bit.DATA = *(dataBuffer + txCount);
                txCount++;
                timeOut = 0;
            }
            else if (timeOut < 1000)
            {
                timeOut++;
            }
            else
            {
                obj->member.reg.DAT->all = 0;
                return 0;
            }
        }
        obj->member.reg.STS1->bit.ACKFAIL = 0;
        obj->member.reg.CTRL1->bit.EN = 1;
        return 1;
    }
    return 0;
}
/**************************************************************************************************
Function Name:
    u8 I2C_Gui_Slave_Receive(I2cSlaveObj *obj, void *data, u8 length, u8 dataOnly)
Input:
    *obj    - Object of I2C peripheral and parameters of state machine for I2C driver.
    *data   - Receive data pointer
    length  - Receive data length
Output:
    return   - Result. 1 = data received, 0 = no data received.
Comment:
    I2C sequence receive function in slave mode. The parameters of state machine shall be specified
before executing this function.
**************************************************************************************************/
u8 I2C_Gui_Slave_Receive(I2cSlaveObj *obj, void *data, u8 length, u8 dataOnly)
{
    u8 *dataBuffer = data, rxCount = 0;
    u16 timeOut = 0;
//    static u8 timeOutRecoveryFlag = 0;

    if (obj->member.reg.STS1->bit.ADDRF || dataOnly)
    {
//        timeOutRecoveryFlag = 0;
        obj->member.reg.STS2->all;
        while (length > rxCount)// && !obj->member.reg.STS1->bit.STOPF)
        {
            if (obj->member.reg.STS1->bit.RXDATNE)
            {
                *(dataBuffer + rxCount) = obj->member.reg.DAT->bit.DATA;
                rxCount++;
                timeOut = 0;
            }
            else if (timeOut < 1000)
            {
                timeOut++;
            }
            else
            {
//                timeOutRecoveryFlag = 1;
                obj->member.reg.DAT->all = 0;
                return 0;
            }
        }
        obj->member.reg.CTRL1->bit.EN = 1;
        return 1;
    }
    return 0;
}
/**************************************************************************************************
Function Name:
    void IIC_SWReset(void)
Input:

Output:

Comment:

**************************************************************************************************/
//void IIC_SWReset(I2cMasterObj *obj)
//{
//    if (obj == &masterI2c2)
//    {
//        _GPIOC_PMODE.bit.PMODE4 = 0;
//        _GPIOC_POTYPE.bit.POT4 = 1;
//        _GPIOC_PUPD.bit.PUPD4 = 1;
//        _GPIOC_AFSEL1.bit.AFSEL4 = 7;
//
//        _GPIOA_PMODE.bit.PMODE5 = 0;
//        _GPIOA_POTYPE.bit.POT5 = 1;
//        _GPIOA_PUPD.bit.PUPD5 = 1;
//        _GPIOA_AFSEL1.bit.AFSEL5 = 11;
//
//        for (;;)
//        {
//            if (_GPIOC_PID.bit.PID4 && _GPIOA_PID.bit.PID5)
//            {
//                obj->member.reg.CTRL1->bit.ALERT = 1;
//                __NOP();
//                __NOP();
//                __NOP();
//                __NOP();
//                __NOP();
//                obj->member.reg.CTRL1->bit.ALERT = 0;
//                break;
//            }
//        }
//    }
//    else if (obj == &masterI2c3)
//    {
//        _GPIOF_PMODE.bit.PMODE4 = 0;
//        _GPIOF_POTYPE.bit.POT4 = 1;
//        _GPIOF_PUPD.bit.PUPD4 = 1;
//        _GPIOF_AFSEL1.bit.AFSEL4 = 6;
//
//        _GPIOF_PMODE.bit.PMODE5 = 0;
//        _GPIOF_POTYPE.bit.POT5 = 1;
//        _GPIOF_PUPD.bit.PUPD5 = 1;
//        _GPIOF_AFSEL1.bit.AFSEL5 = 7;
//
//        for (;;)
//        {
//            if (_GPIOF_PID.bit.PID4 && _GPIOF_PID.bit.PID5)
//            {
//                obj->member.reg.CTRL1->bit.ALERT = 1;
//                __NOP();
//                __NOP();
//                __NOP();
//                __NOP();
//                __NOP();
//                obj->member.reg.CTRL1->bit.ALERT = 0;
//                break;
//            }
//        }
//    }
//}

//----------------------------------------------------------------------------------------------------------------------
