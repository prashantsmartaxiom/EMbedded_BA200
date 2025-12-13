/***********************************************************************************************************************
File Name: n32h48x_uart_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file of n32h48x_uart_driver.c.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/20/2025  Wilson Chen      Create file

***********************************************************************************************************************/

#ifndef _N32H48X_UART_DRIVER_H_
#define _N32H48X_UART_DRIVER_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_register.h"
#include "n32h48x_dma_driver.h"

//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define Create_UART_Object(ObjName, \
                           RegName, \
                           Baud)        UartObj ObjName = \
                                        {\
                                            {\
                                                UART_Register_Group(_##RegName), \
                                                0, Baud\
                                            }, \
                                            {\
                                                UART_Init, \
                                                UART_DMA_Transmit, \
                                                UART_DMA_Receive \
                                            }\
                                        }

// Statement Replacement
#define UART_Register_Group(RegName)    { \
                                            &RegName##_CTRL1, \
                                            &RegName##_CTRL2, \
                                            &RegName##_CTRL3, \
                                            &RegName##_STS, \
                                            &RegName##_DAT, \
                                            &RegName##_BRCF, \
                                            &RegName##_GTP, \
                                            &RegName##_FIFO, \
                                            &RegName##_IFW, \
                                            &RegName##_RTO \
                                        }
//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration

// Struct & Union
// SPI module register map
typedef struct
{
    volatile _USARTx_CTRL1 *CTRL1;
    volatile _USARTx_CTRL2 *CTRL2;
    volatile _USARTx_CTRL3 *CTRL3;
    volatile _USARTx_STS   *STS;
    volatile _USARTx_DAT   *DAT;
    volatile _USARTx_BRCF  *BRCF;
    volatile _USARTx_GTP   *GTP;
    volatile _USARTx_FIFO  *FIFO;
    volatile _USARTx_IFW   *IFW;
    volatile _USARTx_RTO   *RTO;
} UartRegister;

typedef struct UartStr UartObj;
struct UartStr
{
    struct
    {
        UartRegister reg;
        unsigned short txDataCount;
        unsigned long  baudrate;
    } member;

    struct
    {
        void (*init)(UartObj*);
        unsigned char (*transmit)(void*, void*, unsigned char);
        unsigned char (*receive)(void*, void**, unsigned char*);
    } func;
};

//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
void UART_Init(UartObj *obj);
unsigned char UART_Transmit(void *obj, void *buffer, unsigned char size);
unsigned char UART_DMA_Transmit(void *obj, void *buffer, unsigned char size);
unsigned char UART_DMA_Receive(void *obj, void **buffer, unsigned char *size);

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern UartObj usart1, usart2, usart3, usart4, uart5, uart6, uart7, uart8;

//----------------------------------------------------------------------------------------------------------------------
#endif /* _N32H48X_UART_DRIVER_H_ */
