/***********************************************************************************************************************
File Name: n32h48x_uart_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    N32H48x UART transmission and reception driver.
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
#include "n32h48x_uart_driver.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
Create_UART_Object(usart1, USART1, 9600);
Create_UART_Object(usart2, USART2, 9600);
Create_UART_Object(usart3, USART3, 9600);
Create_UART_Object(usart4, USART4, 9600);
Create_UART_Object( uart5,  UART5, 9600);
Create_UART_Object( uart6,  UART6, 9600);
Create_UART_Object( uart7,  UART7, 9600);
Create_UART_Object( uart8,  UART8, 9600);
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void UART_Init(UartObj *obj, unsigned long feature)
Input:
    *obj      - Pointer of UART driver object.
    feature   - UART feature index. use plus(+) operator to combine multi-features.
Output:
    None.
Comment:
    UART initialization function. This function will set the mantissa and fraction of baud rate
register and control regsiers.
**************************************************************************************************/
void UART_Init(UartObj *obj)
{
    obj->member.reg.CTRL1->bit.WL = 0;
    obj->member.reg.CTRL1->bit.PCEN = 0;
    obj->member.reg.CTRL1->bit.RXEN = 1;
    obj->member.reg.CTRL1->bit.TXEN = 1;
    obj->member.reg.CTRL1->bit.IDLEIEN = 1;

    obj->member.reg.CTRL2->bit.STPB = 0;

    obj->member.reg.CTRL3->bit.DMATXEN = 1;
    obj->member.reg.CTRL3->bit.DMARXEN = 1;

    float baud_div_temp = (float)60000000 / obj->member.baudrate / 16;
    obj->member.reg.BRCF->bit.DIV_Integer = (int)baud_div_temp; // 60MHz / 9600bps / 16 = 390.625
    obj->member.reg.BRCF->bit.DIV_Decimal = (int)((baud_div_temp - (int)baud_div_temp) * 16);   // 0.625 * 16 = 10

    obj->member.reg.CTRL1->bit.UEN = 1;
}
/**************************************************************************************************
Function Name:
    unsigned char UART_Transmit(void *obj, void *buffer, unsigned char size)
Input:
    *obj      - Pointer of UART driver object.
    *buffer   - Pointer of the data array.
    size      - Size of the data array.
Output:
    return    - Result. 1 = completed. 0 = does not complete.
Comment:
    UART transmission function.
**************************************************************************************************/
unsigned char UART_Transmit(void *obj, void *buffer, unsigned char size)
{
	if (obj && ((UartObj*)obj)->member.reg.STS->bit.TXC)
	{
        if ((UartObj *)obj == &usart1)
        {
            USART1_DE_ENABLE;
        }
        else if ((UartObj *)obj == &usart2)
        {
            USART2_DE_ENABLE;
        }
        else if ((UartObj *)obj == &usart3)
        {
            USART3_DE_ENABLE;
        }
        else if ((UartObj *)obj == &usart4)
        {
            USART4_DE_ENABLE;
        }
        else if ((UartObj *)obj == &uart5)
        {
            UART5_DE_ENABLE;
        }
        else if ((UartObj *)obj == &uart6)
        {
            UART6_DE_ENABLE;
        }
        else if ((UartObj *)obj == &uart7)
        {
            UART7_DE_ENABLE;
        }
        else if ((UartObj *)obj == &uart8)
        {
            UART8_DE_ENABLE;
        }

        while (((UartObj*)obj)->member.txDataCount < size)
        {
            unsigned char *word = buffer;

            ((UartObj*)obj)->member.reg.DAT->bit.DATV = word[((UartObj*)obj)->member.txDataCount];

            while (((UartObj*)obj)->member.reg.STS->bit.TXDE == 0);

            ((UartObj*)obj)->member.txDataCount++;
        }

        while (((UartObj*)obj)->member.reg.STS->bit.TXC == 0);

        if ((UartObj *)obj == &usart1)
        {
            USART1_DE_DISABLE;
        }
        else if ((UartObj *)obj == &usart2)
        {
            USART2_DE_DISABLE;
        }
        else if ((UartObj *)obj == &usart3)
        {
            USART3_DE_DISABLE;
        }
        else if ((UartObj *)obj == &usart4)
        {
            USART4_DE_DISABLE;
        }
        else if ((UartObj *)obj == &uart5)
        {
            UART5_DE_DISABLE;
        }
        else if ((UartObj *)obj == &uart6)
        {
            UART6_DE_DISABLE;
        }
        else if ((UartObj *)obj == &uart7)
        {
            UART7_DE_DISABLE;
        }
        else if ((UartObj *)obj == &uart8)
        {
            UART8_DE_DISABLE;
        }

        ((UartObj*)obj)->member.txDataCount = 0;

        return 1;
	}
    return 0;
}
/**************************************************************************************************
Function Name:
    unsigned char UART_DMA_Transmit(void *obj, void *buffer, unsigned char size)
Input:
    *obj      - Pointer of DMA's UART driver object.
    *buffer   - Pointer of the data array.
    size      - Size of the data array.
Output:
    return    - Result. 1 = completed. 0 = does not complete.
Comment:
    UART's DMA transmission function.
**************************************************************************************************/
unsigned char UART_DMA_Transmit(void *obj, void *buffer, unsigned char size)
{
    if (obj && ((DmaUartObj*)obj)->member.dma.size >= size)
    {
        ((DmaUartObj*)obj)->member.buffer.ptr = buffer;
        ((DmaUartObj*)obj)->member.dataAmount = size;

        if(((DmaUartObj*)obj)->func.accessDmaData(obj))
        {
            return 1;
        }
    }
    return 0;
}
/**************************************************************************************************
Function Name:
    unsigned char UART_DMA_Receive(void *obj, void **buffer, unsigned char *size)
Input:
    *obj      - Pointer of DMA's UART driver object.
    **buffer  - Pointer of the address of the data array.
    *size     - Pointer of size variable of the data array.
Output:
    return    - Result. 1 = completed. 0 = does not complete.
Comment:
    UART's DMA receiption function. This function will transfer the received data to the array
which the first parameter points to and update the data size to the size variable which the second
parameter points to.
**************************************************************************************************/
unsigned char UART_DMA_Receive(void *obj, void **buffer, unsigned char *size)
{
    if (obj && ((DmaUartObj*)obj)->func.accessDmaData(obj))
    {
		*buffer = ((DmaUartObj*)obj)->member.buffer.ptr;
		*size = ((DmaUartObj*)obj)->member.dataAmount;
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
