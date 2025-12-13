/***********************************************************************************************************************
File Name: n32h48x_i2c_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file of n32h48x_i2c_driver.c.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/02/2024  Fred Huang      Create file

***********************************************************************************************************************/

#ifndef _N32H48X_I2C_DRIVER_H_
#define _N32H48X_I2C_DRIVER_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_register.h"
//#include "io.h"

//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define I2C_CLOCK                   SystemCoreClock / 4
#define I2C_CLKFREQ_REG             I2C_CLOCK / 1000000
#define I2C_CLOCK_SPEED             400000
#define I2C_MASTER_ADDR             0x30
#define I2C_SLAVE_ADDR              0x21

#define Create_I2C_Master_Object(ObjName, \
                                 RegName)       I2cMasterObj ObjName = \
                                                {\
                                                    {\
                                                        I2C_Register_Group(_##RegName), \
                                                        0, I2C_IDLE, I2C_COMM_ERROR, 0, 0, 0, 0, 0, 0\
                                                    },\
                                                    {\
                                                        I2C_Master_Init, \
                                                        I2C_Master_Write, \
                                                        I2C_Master_Read \
                                                    }, \
                                                }

#define Create_I2C_Slave_Object(ObjName, \
                                 RegName)       I2cSlaveObj ObjName = \
                                                { \
                                                    { \
                                                        I2C_Register_Group(_##RegName) \
                                                    }, \
                                                    { \
                                                        I2C_Slave_Init, \
                                                        I2C_Slave_Transmit, \
                                                        I2C_Gui_Slave_Receive \
                                                    }, \
                                                }

// Statement Replacement
#define I2C_Register_Group(RegName)             { \
                                                    &RegName##_CTRL1, \
                                                    &RegName##_CTRL2, \
                                                    &RegName##_OADDR1, \
                                                    &RegName##_OADDR2, \
                                                    &RegName##_DAT, \
                                                    &RegName##_STS1, \
                                                    &RegName##_STS2, \
                                                    &RegName##_CLKCTRL, \
                                                    &RegName##_TMRISE, \
                                                    &RegName##_BYTENUM, \
                                                    &RegName##_GFLTRCTRL, \
                                                    &RegName##_FIFODAT, \
                                                }

//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration
typedef enum
{
    I2C_IDLE                    = 0,
    I2C_MASTER_WRITE            = 1,
    I2C_MASTER_READ             = 2,
    I2C_SLAVE_RECEIVED_DATA     = 3,
    I2C_SLAVE_TRANSMIT_DATA     = 4,
    I2C_BLOCK                   = 9,
} I2cState;

typedef enum
{
    I2C_COMM_ERROR      = 0,
    I2C_NO_DEVICE       = 1,
    I2C_RX_TIMEOUT      = 2,
    I2C_READ_SUCCESS    = 3,
    I2C_WRITE_SUCCESS   = 4
} I2cCommunicationResult;

typedef enum
{
    I2C_NORMAL_RCV      = 0,
    I2C_DATA_ONLY       = 1,
    I2C_WRITE_ONLY      = 2,
    I2C_WRITE_READ      = 3,
} I2cTransmitReceiveType;

// Struct & Union
// I2C module register map
typedef struct
{
    volatile _I2Cx_CTRL1 *CTRL1;
    volatile _I2Cx_CTRL2 *CTRL2;
    volatile _I2Cx_OADDR1 *OADDR1;
    volatile _I2Cx_OADDR2 *OADDR2;
    volatile _I2Cx_DAT *DAT;
    volatile _I2Cx_STS1 *STS1;
    volatile _I2Cx_STS2 *STS2;
    volatile _I2Cx_CLKCTRL *CLKCTRL;
    volatile _I2Cx_TMRISE *TMRISE;
    volatile _I2Cx_BYTENUM *BYTENUM;
    volatile _I2Cx_GFLTRCTRL *GFLTRCTRL;
    volatile _I2Cx_FIFODAT *FIFODAT;
} I2cRegister;

typedef struct I2cMasterStr I2cMasterObj;
struct I2cMasterStr
{
    struct
    {    
        I2cRegister reg;
        unsigned char address;          // I2C master address
        I2cState occupied;              // I2C state machine status
        I2cCommunicationResult result;  // Transceive result
        unsigned short txSize;          // Number of bytes to be transmitted
        unsigned short rxSize;          // Number of bytes to be received
        unsigned short txStep;          // Step count of transmitter
        unsigned short rxStep;          // Step count of receiver
        unsigned char *txdPtr;          // Data pointer of transmitter
        unsigned char *rxdPtr;          // Data pointer of receiver      
    } member;
    
    struct 
    {
        void (*init)(I2cMasterObj*);
        unsigned char (*write)(I2cMasterObj *, unsigned char, unsigned char, unsigned char, unsigned short, unsigned char);
        unsigned char (*read)(I2cMasterObj *, void *, unsigned char, unsigned char);
    } func;
};

typedef struct I2cSlaveStr I2cSlaveObj;
struct I2cSlaveStr
{
    struct
    {
        I2cRegister reg;
    } member;

    struct
    {
        void (*init)(I2cSlaveObj *, u16);
        u8 (*transmit)(I2cSlaveObj *, void *, u8);
        u8 (*receive)(I2cSlaveObj *, void *, u8, u8);
    } func;
};

//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
void I2C_Master_Init(I2cMasterObj *obj);
unsigned char I2C_Master_Write(I2cMasterObj *obj, uint8_t addr, uint8_t cmd, uint8_t length, uint16_t data, uint8_t dataOnly);
unsigned char I2C_Master_Read(I2cMasterObj *obj, void *data, uint8_t addr, uint8_t length);
//void IIC_SWReset(I2cMasterObj *obj);

void I2C_Slave_Init(I2cSlaveObj *obj, u16 address);
u8 I2C_Slave_Transmit(I2cSlaveObj *obj, void *data, u8 length);
u8 I2C_Gui_Slave_Receive(I2cSlaveObj *obj, void *data, u8 length, u8 dataOnly);
//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern I2cSlaveObj slaveI2c1;
extern I2cMasterObj masterI2c2, masterI2c3, masterI2c4;
//----------------------------------------------------------------------------------------------------------------------
#endif /* _N32H48X_I2C_DRIVER_H_ */
