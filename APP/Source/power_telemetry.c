/**************************************************************************************************
File Name: power_telemetry.c
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
     2025/10/13 Wilson Chen      1. Initial version

**************************************************************************************************/

/* Includes */
#include "power_telemetry.h"

/* Local function prototypes */

/* Data definitions */
TaskHandle_t PowerTaskHandle = NULL;
PowerTelemetryStruct Power_telemetry = {0};
/**************************************************************************************************
Function Name:
    void power_calc(void *arg)

Inputs:

Outputs:
    None

Description:

**************************************************************************************************/
void power_calc(void *arg)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    static uint8_t i = 0, j = 0, k = 0, l = 0, m = 0;

    while (1)
    {
        // MPQ7210:
        // The current is always 1.25A
        // MP5920:
        // Pout(SHADE) = Direct value * 1000 / 26447
        // Pout(EXP_LED) = Direct value * 1000 / 28650
        // MP8880A:
        // The voltage is always 24V
        // Iout = 0.019 * Direct value

        Power_telemetry.Led_Power[i] = (uint16_t)(((uint32_t)mpq7210RegisterVout[i] * 125) / 100); // 0.1W
        Power_telemetry.Shade_Power[j] = (uint16_t)(((uint32_t)Mp5920Register.Shade_outputPower[j] * 10 * 1000) / 26447); // 0.1W
        Power_telemetry.Exp_Led_Power[k] = (uint16_t)(((uint32_t)Mp5920Register.EXP_LED_outputPower[k] * 10 * 1000) / 28650); // 0.1W
        if (Power_telemetry.Exp_Led_Power[k] > 3000) // 300.0W
        {
            Power_telemetry.Exp_Led_Power[k] = 0;
        }
        Power_telemetry.Io_Power[l] = (uint16_t)(((uint32_t)Mp8880aRegister.IO_IOUT[l] * 10 * 24 * 19) / 1000); // 0.1W
        Power_telemetry.Rs485_Power[m] = (uint16_t)(((uint32_t)Mp8880aRegister.RS485_IOUT[m] * 10 * 24 * 19) / 1000); // 0.1W

        i = (i + 1) % 22;
        j = (j + 1) % 4;
        k = (k + 1) % 8;
        l = (l + 1) % 4;
        m = (m + 1) % 4;

        /* Update data every 500 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
/**************************************************************************************************
Function Name:
    void power_init(void)

Inputs:
    None

Outputs:
    None

Description:

**************************************************************************************************/
void power_init(void)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    if (xTaskCreate(power_calc,
                    "power_calc",
                    POWER_CALC_TASK_STACK_SIZE,
                    NULL,
                    POWER_CALC_TASK_PRIORITY,
                    &PowerTaskHandle) != pdPASS)
    {
        printf("Task <<scan_device>> creation failed!\n");
    }
}
