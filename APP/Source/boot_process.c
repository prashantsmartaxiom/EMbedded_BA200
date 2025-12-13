/***********************************************************************************************************************
File Name: boot_process.c
External Data:
    Name                                    Source
    --------------------------------------- ----------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ----------------------------------------------------------------------------

Description:
    PSE flow control.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ----------------------------------------------------------------------------------------
    05/15/2025  Wilson Chen      Create file

***********************************************************************************************************************/

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "boot_process.h"
//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------
unsigned char Software_Timer(TmrPkg *pkg);
void Boot_Reset_MCU(void);
//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
Create_Timer(dlyBoot, 100);
BootDataType bootInternal = {0, 0, 0, 0, 0, 0};
TaskHandle_t BootRoutineTaskHandle = NULL;
TaskHandle_t BootStateTaskHandle = NULL;
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Boot_Reset_MCU(void)
Input:
    None.
Output:
    None.
Comment:
    Reset MCU function for bootloader.
**************************************************************************************************/
void Boot_Reset_MCU(void)
{
	// Disable ISR && reset MCU
	__disable_irq();
	NVIC_SystemReset();
}

//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
void boot_init(void)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);
    
    if (xTaskCreate(Boot_Loader_State_Machine,
                    "Boot_Loader_State_Machine",
                    BOOT_STATE_TASK_STACK_SIZE,
                    NULL,
                    BOOT_STATE_TASK_PRIORITY,
                    &BootStateTaskHandle) != pdPASS)
    {
        printf("Task <<Boot_Loader_State_Machine>> creation failed!\n");
    }
}

/**************************************************************************************************
Function Name:
    void Boot_Loader_State_Machine(void *arg)
Input:
    None.
Output:
    None.
Comment:
    Boot loader state machine function.
**************************************************************************************************/
void Boot_Loader_State_Machine(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);
    
    while (1)
    {
        switch (bootInternal.state.byte.bootState)
        {
        case BOOT_LOCKED:
            if (bootInternal.protectKey == BOOT_UNLOCK_KEY)
            {
                bootInternal.state.stateCode = (BOOT_UNLOCKED << 8) + PREPARING;
                bootInternal.protectKey = 0;
            }
            bootInternal.initKey = 0;
            break;
        case BOOT_UNLOCKED:
            if (bootInternal.protectKey == BOOT_LOCK_KEY)
            {
                bootInternal.state.stateCode = BOOT_LOCKED << 8;
                bootInternal.protectKey = 0;
            }
            else if (bootInternal.state.byte.subState == PREPARING)
            {
                if (bootInternal.initKey == BOOT_INIT_KEY)
                {
                    bootInternal.state.byte.subState = READY;
                }
            }
            break;  
        }
        
        if (bootInternal.state.byte.subState == READY)
        {
            if (Call_Timer(dlyBoot))
            {
                // Write key to backup register            
                _RCC_BDCTRL.bit.RTCEN = 1;
                RTC_BKUPRgWrite(1, bootInternal.initKey);
                _RCC_BDCTRL.bit.RTCEN = 0;
                
                Boot_Reset_MCU();
            }
        }
        
        /* Update data every 100 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
//----------------------------------------------------------------------------------------------------------------------
