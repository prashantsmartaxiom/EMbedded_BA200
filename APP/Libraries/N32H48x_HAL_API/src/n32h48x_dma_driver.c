/***********************************************************************************************************************
File Name: n32h48x_dma_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    N32H48x DMA data alignment driver.
    The driver shall create and fill in the required data in the driver's parameters before executing any function.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/20/2025  Wilson Chen      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_dma_driver.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
Create_UART_RX_DMA_Object(usart1Dma1Rx, 32, DMA1, 1);
Create_UART_RX_DMA_Object(usart2Dma1Rx, 32, DMA1, 2);
Create_UART_RX_DMA_Object(usart3Dma1Rx, 32, DMA1, 3);
Create_UART_RX_DMA_Object(usart4Dma1Rx, 32, DMA1, 4);
Create_UART_RX_DMA_Object(uart5Dma1Rx,  32, DMA1, 5);
Create_UART_RX_DMA_Object(uart6Dma1Rx,  32, DMA1, 6);
Create_UART_RX_DMA_Object(uart7Dma1Rx,  32, DMA1, 7);
Create_UART_RX_DMA_Object(uart8Dma1Rx,  32, DMA1, 8);

Create_UART_TX_DMA_Object(usart1Dma2Tx, 32, DMA2, 1);
Create_UART_TX_DMA_Object(usart2Dma2Tx, 32, DMA2, 2);
Create_UART_TX_DMA_Object(usart3Dma2Tx, 32, DMA2, 3);
Create_UART_TX_DMA_Object(usart4Dma2Tx, 32, DMA2, 4);
Create_UART_TX_DMA_Object(uart5Dma2Tx,  32, DMA2, 5);
Create_UART_TX_DMA_Object(uart6Dma2Tx,  32, DMA2, 6);
Create_UART_TX_DMA_Object(uart7Dma2Tx,  32, DMA2, 7);
Create_UART_TX_DMA_Object(uart8Dma2Tx,  32, DMA2, 8);

//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void DMA_UART_RX_Init(DmaUartObj *obj, unsigned long dataRegAddress)
Input:
    *obj            - Pointer of DMA driver object.
    dataRegAddress  - Address of UART data register.
Output:
    None.
Comment:
    DMA initialization function. This function will configure the DMA's UART receiption request.
**************************************************************************************************/
void DMA_UART_RX_Init(DmaUartObj *obj, unsigned long dataRegAddress)
{
    obj->member.reg.CHCFG->bit.PRIOLVL = 1;
    obj->member.reg.CHCFG->bit.MSIZE = 0;
    obj->member.reg.CHCFG->bit.PSIZE = 0;
    obj->member.reg.CHCFG->bit.MINC = 1;
    obj->member.reg.CHCFG->bit.PINC = 0;
    obj->member.reg.CHCFG->bit.CIRC = 1;
    obj->member.reg.CHCFG->bit.DIR = 0;
    
    obj->member.reg.TXNUM->bit.NDTX = obj->member.dma.size;
    obj->member.reg.PADDR->bit.ADDR = dataRegAddress;
    obj->member.reg.MADDR->bit.ADDR = (long)obj->member.dma.ptr;
    
    if (obj->member.channel == 1)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 5;
    }
    else if (obj->member.channel == 2)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 7;
    }
    else if (obj->member.channel == 3)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 9;
    }
    else if (obj->member.channel == 4)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 11;
    }
    else if (obj->member.channel == 5)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 13;
    }
    else if (obj->member.channel == 6)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 15;
    }
    else if (obj->member.channel == 7)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 17;
    }
    else if (obj->member.channel == 8)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 19;
    }
    
    obj->member.reg.CHCFG->bit.CHEN = 1;
}
/**************************************************************************************************
Function Name:
    void DMA_UART_TX_Init(DmaUartObj *obj, unsigned long dataRegAddress)
Input:
    *obj            - Pointer of DMA driver object.
    dataRegAddress  - Address of UART data register.
Output:
    None.
Comment:
    DMA initialization function. This function will configure the DMA's UART transmission request.
**************************************************************************************************/
void DMA_UART_TX_Init(DmaUartObj *obj, unsigned long dataRegAddress)
{    
    obj->member.reg.CHCFG->bit.PRIOLVL = 0;
    obj->member.reg.CHCFG->bit.MSIZE = 0;
    obj->member.reg.CHCFG->bit.PSIZE = 0;
    obj->member.reg.CHCFG->bit.MINC = 1;
    obj->member.reg.CHCFG->bit.PINC = 0;
    obj->member.reg.CHCFG->bit.CIRC = 0;
    obj->member.reg.CHCFG->bit.DIR = 1;
    
    obj->member.reg.TXNUM->bit.NDTX = 0;
    obj->member.reg.PADDR->bit.ADDR = dataRegAddress;
    obj->member.reg.MADDR->bit.ADDR = (long)obj->member.dma.ptr;
    
    if (obj->member.channel == 1)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 4;
    }
    else if (obj->member.channel == 2)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 6;
    }
    else if (obj->member.channel == 3)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 8;
    }
    else if (obj->member.channel == 4)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 10;
    }
    else if (obj->member.channel == 5)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 12;
    }
    else if (obj->member.channel == 6)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 14;
    }
    else if (obj->member.channel == 7)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 16;
    }
    else if (obj->member.channel == 8)
    {
        obj->member.reg.CHSEL->bit.CH_SEL = 18;
    }
    
    obj->member.reg.CHCFG->bit.CHEN = 1;
}
/**************************************************************************************************
Function Name:
    unsigned char Write_DMA_Data(DmaUartObj *obj)
Input:
    *obj    - Pointer of DMA driver object.
Output:
    return  - Result. 1 = completed. 0 = does not complete.
Comment:
    Write memory of DMA function.
**************************************************************************************************/
unsigned char Write_DMA_Data(DmaUartObj *obj)
{
    if (!obj->member.reg.TXNUM->bit.NDTX)
    {    
        int i;
        
        unsigned char *dmaAddress = (unsigned char*)obj->member.dma.ptr;
        unsigned char *bufferAddress = (unsigned char*)obj->member.buffer.ptr;
        
        if ((DmaUartObj *)obj == &usart1Dma2Tx)
        {
            USART1_DE_ENABLE;
        }
        else if ((DmaUartObj *)obj == &usart2Dma2Tx)
        {
            USART2_DE_ENABLE;
        }
        else if ((DmaUartObj *)obj == &usart3Dma2Tx)
        {
            USART3_DE_ENABLE;
        }
        else if ((DmaUartObj *)obj == &usart4Dma2Tx)
        {
            USART4_DE_ENABLE;
        }
        else if ((DmaUartObj *)obj == &uart5Dma2Tx)
        {
            UART5_DE_ENABLE;
        }
        else if ((DmaUartObj *)obj == &uart6Dma2Tx)
        {
            UART6_DE_ENABLE;
        }
        else if ((DmaUartObj *)obj == &uart7Dma2Tx)
        {
            UART7_DE_ENABLE;
        }
        else if ((DmaUartObj *)obj == &uart8Dma2Tx)
        {
            UART8_DE_ENABLE;
        }
            
        for (i = 0; i < obj->member.dataAmount; i++)
        {
            *(dmaAddress + i) = *(bufferAddress + i);
        }
        
        obj->member.reg.CHCFG->bit.CHEN = 0;
        obj->member.reg.TXNUM->bit.NDTX = i;
        obj->member.reg.CHCFG->bit.CHEN = 1;
        return 1;
    }
    return 0;
}
/**************************************************************************************************
Function Name:
    unsigned char Read_Circular_DMA_Data(DmaUartObj *obj)
Input:
    *obj    - Pointer of DMA driver object.
Output:
    return  - Result. 1 = completed. 0 = does not complete.
Comment:
    Read memory of DMA function. This function will extract the unread data to the buffer of DMA 
object.
**************************************************************************************************/
unsigned char Read_Circular_DMA_Data(DmaUartObj *obj)
{
    int stop = obj->member.dma.size - obj->member.reg.TXNUM->bit.NDTX;

    if (stop != obj->member.index)
    {
        int dataCount = ((obj->member.index > stop) * obj->member.dma.size + stop - obj->member.index) & 
                        (obj->member.dma.size - 1);
        
        if (obj->member.buffer.size >= dataCount)
        {
            int i;

            unsigned char *dmaAddress = (unsigned char*)obj->member.dma.ptr;
            unsigned char *bufferAddress = (unsigned char*)obj->member.buffer.ptr;
                
            for(i = 0; i < dataCount; i++)
            {
                *(bufferAddress + i) = *(dmaAddress + obj->member.index);
                obj->member.index = (obj->member.index + 1) & (obj->member.dma.size - 1);
            }
                       
            obj->member.dataAmount = dataCount;
            return 1;
        }
    }
    return 0; 
}

//----------------------------------------------------------------------------------------------------------------------
