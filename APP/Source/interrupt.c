/**************************************************************************************************
File Name: interrupt.c
External Data:
    Name                                    Source
    --------------------------------------- -----------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- -----------------------------------------------------

Description:


===================================================================================================
History:
     Date       Author           Description of Change
     ---------- ---------------- -----------------------------------------------------------------
     06/18      Wilson Chen      1. Initial version

**************************************************************************************************/

/* Includes */
#include "interrupt.h"
#include "semphr.h"

/* Local function prototypes */
void xPortSysTickHandler(void);

/* Data definitions */
extern xSemaphoreHandle SemHandle;
uint8_t uartPacketSemaphore[8] = {0};

/**************************************************************************************************
Function Name:
    void SysTick_Handler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function is the interrupt handler for the SysTick timer. It is responsible for:
    1. Entering a nested-safe critical section.
    2. Calling the FreeRTOS tick handler (`xPortSysTickHandler`) if the scheduler has started.
    3. Exiting the critical section.

    The tick handler updates the system tick count and manages task switching for the RTOS.
**************************************************************************************************/
void SysTick_Handler(void)
{
    uint32_t TempReturn;
    /* Enter critical segment, which can be nested */
    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}

/**************************************************************************************************
Function Name:
    void ETH_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function is the interrupt handler for the Ethernet peripheral. It performs the following:
    1. Enters a nested-safe critical section.
    2. Checks whether a frame reception interrupt (RI flag) occurred.
    3. If so, gives the Ethernet semaphore from ISR to notify reception.
    4. Clears the relevant DMA interrupt flags (RI and NIS).
    5. Requests a context switch if a higher-priority task was unblocked.
    6. Exits the critical section.

    This enables deferred processing of Ethernet frames using a FreeRTOS task.
**************************************************************************************************/
void ETH_IRQHandler(void)
{
    uint32_t TempReturn;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* Enter critical segment, which can be nested */
    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (ETH_GetDMACh0InterruptStatus(ETH, ETH_DMACH0_INT_FLAG_RI) != RESET)
    {
        xSemaphoreGiveFromISR(SemHandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        ETH_ClearDMACh0InterruptFlag(ETH, (ETH_DMACH0_INT_FLAG_RI | ETH_DMACH0_INT_FLAG_NIS));
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}

/**************************************************************************************************
Function Name:
    void USART1_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function handles USART4 global interrupt request.
**************************************************************************************************/
void USART1_IRQHandler(void)
{
    uint32_t TempReturn;

    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (usart1.member.reg.STS->bit.IDLEF)
    {
        uartPacketSemaphore[0] = 1;

        usart1.member.reg.DAT->all;
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}
/**************************************************************************************************
Function Name:
    void USART2_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function handles USART4 global interrupt request.
**************************************************************************************************/
void USART2_IRQHandler(void)
{
    uint32_t TempReturn;

    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (usart2.member.reg.STS->bit.IDLEF)
    {
        uartPacketSemaphore[1] = 1;

        usart2.member.reg.DAT->all;
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}
/**************************************************************************************************
Function Name:
    void USART3_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function handles USART4 global interrupt request.
**************************************************************************************************/
void USART3_IRQHandler(void)
{
    uint32_t TempReturn;

    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (usart3.member.reg.STS->bit.IDLEF)
    {
        uartPacketSemaphore[2] = 1;

        usart3.member.reg.DAT->all;
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}
/**************************************************************************************************
Function Name:
    void USART4_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function handles USART4 global interrupt request.
**************************************************************************************************/
void USART4_IRQHandler(void)
{
    uint32_t TempReturn;

    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (usart4.member.reg.STS->bit.IDLEF)
    {
        uartPacketSemaphore[3] = 1;

        usart4.member.reg.DAT->all;
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}
/**************************************************************************************************
Function Name:
    void UART5_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function handles USART4 global interrupt request.
**************************************************************************************************/
void UART5_IRQHandler(void)
{
    uint32_t TempReturn;

    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (uart5.member.reg.STS->bit.IDLEF)
    {
        uartPacketSemaphore[4] = 1;

        uart5.member.reg.DAT->all;
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}
/**************************************************************************************************
Function Name:
    void UART6_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function handles USART4 global interrupt request.
**************************************************************************************************/
void UART6_IRQHandler(void)
{
    uint32_t TempReturn;

    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (uart6.member.reg.STS->bit.IDLEF)
    {
        uartPacketSemaphore[5] = 1;

        uart6.member.reg.DAT->all;
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}
/**************************************************************************************************
Function Name:
    void UART7_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function handles USART4 global interrupt request.
**************************************************************************************************/
void UART7_IRQHandler(void)
{
    uint32_t TempReturn;

    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (uart7.member.reg.STS->bit.IDLEF)
    {
        uartPacketSemaphore[6] = 1;

        uart7.member.reg.DAT->all;
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}
/**************************************************************************************************
Function Name:
    void UART8_IRQHandler(void)

Inputs:
    None

Outputs:
    None

Description:
    This function handles USART4 global interrupt request.
**************************************************************************************************/
void UART8_IRQHandler(void)
{
    uint32_t TempReturn;

    TempReturn = taskENTER_CRITICAL_FROM_ISR();

    if (uart8.member.reg.STS->bit.IDLEF)
    {
        uartPacketSemaphore[7] = 1;

        uart8.member.reg.DAT->all;
    }

    /* Exit critical segment */
    taskEXIT_CRITICAL_FROM_ISR(TempReturn);
}

