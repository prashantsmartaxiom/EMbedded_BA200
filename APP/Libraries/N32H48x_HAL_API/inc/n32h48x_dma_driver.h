/***********************************************************************************************************************
File Name: n32h48x_dma_driver.h
Global Data:
    Name                    Type                        Description
    ----------------------- --------------------------- ------------------------------------------------------------

Description:
    Header file of n32h48x_dma_driver.c.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/20/2025  Wilson Chen      Create file

***********************************************************************************************************************/

#ifndef _N32H48X_DMA_DRIVER_H_
#define _N32H48X_DMA_DRIVER_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_register.h"
#include "io.h"
//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define Create_UART_RX_DMA_Object(MemName, \
                                  Size, \
                                  RegName, \
                                  Channel)  unsigned char MemName##Mem[Check_Power_2(Size) * 8];\
                                            unsigned char MemName##Buffer[Check_Power_2(Size) * 8];\
                                            DmaUartObj MemName = \
                                            {\
                                                {\
                                                    DMA_Register_Group(_##RegName, Channel), \
                                                    {MemName##Mem, Size},\
                                                    {MemName##Buffer, Size},\
                                                    0, 0, Channel \
                                                },\
                                                {\
                                                    DMA_UART_RX_Init,\
                                                    Read_Circular_DMA_Data\
                                                }\
                                            }

#define Create_UART_TX_DMA_Object(MemName, \
                                  Size, \
                                  RegName, \
                                  Channel)  unsigned char MemName##Mem[Size * 8];\
                                            DmaUartObj MemName = \
                                            {\
                                                {\
                                                    DMA_Register_Group(_##RegName, Channel), \
                                                    {MemName##Mem, Size},\
                                                    {0, 0}, \
                                                    0, 0, Channel \
                                                },\
                                                {\
                                                    DMA_UART_TX_Init,\
                                                    Write_DMA_Data\
                                                }\
                                            }

// Statement Replacement
#ifndef Check_Power_2
#define Check_Power_2(Number)               (Number * ((((unsigned)Number - 1) ^ Number) == ((Number << 1) - 1)))
#endif

#define DMA_Register_Group(RegName, Channel) {  \
                                                &RegName##_CHCFG##Channel, \
                                                &RegName##_MADDR##Channel, \
                                                &RegName##_PADDR##Channel, \
                                                &RegName##_CHSEL##Channel, \
                                                &RegName##_TXNUM##Channel, \
                                                &RegName##_INTCLR, \
                                                &RegName##_INTSTS, \
                                                &RegName##_BURSTEN, \
                                             }
//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration


// Struct & Union
typedef struct
{   
    void *ptr;  
    unsigned long size;
}DmaPackStr;

typedef struct 
{
    volatile _DMAx_CHCFGy   *CHCFG;
    volatile _DMAx_MADDRy   *MADDR;
    volatile _DMAx_PADDRy   *PADDR;
    volatile _DMAx_CHSELy   *CHSEL;
    volatile _DMAx_TXNUMy   *TXNUM;
    volatile _DMAx_INTCLR   *INTCLR;
    volatile _DMAx_INTSTS   *INTSTS;
    volatile _DMAx_BURSTEN  *BURSTEN;
}DmaRegStr;

typedef struct DmaUartStr DmaUartObj;

struct DmaUartStr
{
    struct
    {
        DmaRegStr reg;
        DmaPackStr dma;
        DmaPackStr buffer;
        unsigned short index;
        unsigned short dataAmount;
        unsigned char channel;
    }member;
        
    struct
    {
        void (*init)(DmaUartObj*, unsigned long);
        unsigned char (*accessDmaData)(DmaUartObj*);
    }func;
};
//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
void DMA_UART_RX_Init(DmaUartObj *obj, unsigned long dataRegAddress);
void DMA_UART_TX_Init(DmaUartObj *obj, unsigned long dataRegAddress);
unsigned char Write_DMA_Data(DmaUartObj *obj);
unsigned char Read_Circular_DMA_Data(DmaUartObj *obj);

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern DmaUartObj usart1Dma1Rx, usart2Dma1Rx, usart3Dma1Rx, usart4Dma1Rx, uart5Dma1Rx, uart6Dma1Rx, uart7Dma1Rx, uart8Dma1Rx;
extern DmaUartObj usart1Dma2Tx, usart2Dma2Tx, usart3Dma2Tx, usart4Dma2Tx, uart5Dma2Tx, uart6Dma2Tx, uart7Dma2Tx, uart8Dma2Tx;
//----------------------------------------------------------------------------------------------------------------------
#endif /* _N32H48X_DMA_DRIVER_H_ */
