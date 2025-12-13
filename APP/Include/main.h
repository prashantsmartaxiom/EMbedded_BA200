/**************************************************************************************************
File Name: main.h
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

#ifndef __MAIN_H__
#define __MAIN_H__

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/timeouts.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/tcpip.h"
#include "ethernetif.h"
#include "mqtt_client.h"
#include "parse.h"
#include "n32h47x_48x_flash.h"
#include "n32h47x_48x_rtc.h"
#include "n32h47x_48x_shrtim.h"
#include "n32h47x_48x_tim.h"
#include "n32h47x_48x_crc.h"
#include "n32h48x_uart_driver.h"
#include "n32h48x_dma_driver.h"
#include "hmi_driver.h"
#include "shade_driver.h"
#include "light_driver.h"
#include "meter_driver.h"
#include "air_driver.h"
#include "gui_driver.h"
#include "gui_box_control.h"
#include "mpq7210_driver.h"
#include "i2c_device_driver.h"
#include "boot_process.h"
#include "peripheral_setup.h"
#include "interrupt.h"
#include "io.h"
#include "device_polling.h"
#include "adc_routine.h"
#include "timers.h"
#include "cJSON.h"
#include "messages.h"
#include "power_telemetry.h"
//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
// Flash definition
#define App_Code_Start                          (long)(&APP_START_ADDRESS) // VTOR offset = 0x200
#define App_ROM_Size                            (long)(&APP_ROM_SIZE)

// Model name
#ifndef Model_Name
#define Model_Name                              "BA200"
#endif

// Compliant model name
#define Compliant_Model                         {\
                                                    "BA200",\
                                                }

// Function code
#ifndef Function_Code
#define Function_Code                           "CTR"
#endif

// Type code
#ifndef Type_Code
#define Type_Code                               "APP"
#endif

// Customer code
#ifndef Customer_Code
#define Customer_Code                           "STD"
#endif

// Frimware version
#ifndef Code_Version
#define Code_Version                            "S0.00B00"
#endif
#ifndef Sub_Version
#define Sub_Version                             "00"
#endif

// App header summary
#ifndef App_Header_List
#ifndef APP_ONLY_MODE
#define App_Header_List                         {\
                                                    Model_Name,\
                                                    Type_Code,\
                                                    Customer_Code,\
                                                    3,\
                                                    Function_Code,\
                                                    (long)(&APP_ROM_SIZE),\
                                                    0,\
                                                    Code_Version\
                                                }
#else
#define App_Header_List                         {\
                                                    Model_Name,\
                                                    Type_Code,\
                                                    Customer_Code,\
                                                    3,\
                                                    Function_Code,\
                                                    0x0200,\
                                                    0,\
                                                    Code_Version\
                                                }
#endif
#endif

// Version summary
#ifndef Version_List
#define Version_List                            {Code_Version, Sub_Version}
#endif

#define Create_Timer(Name, Cmp)                 TmrPkg Name = {0, Cmp}
#define Call_Timer(Name)                        Software_Timer(&Name)

#define NETWORK_APP_TASK_STACK_SIZE             (512)
#define NETWORK_APP_TASK_PRIORITY               (1)

#define FLASH_WRITE_START_ADDR                  0x08070000
#define HW_TYPE                                 "N32H487"
#define BOARD_TYPE                              "BAM"
#define BATCH_ID                                "BA200"
#define DEVICE_STRUCT_SIZE                      sizeof(DeviceInfo_t)
#define FLASH_WRITE_SIZE                        (DEVICE_STRUCT_SIZE + 7) / 8 * 8
#define FLASH_READ_SIZE                         FLASH_WRITE_SIZE
#define DEVICE_ID_SIZE                          6
#define DEVICE_SN_SIZE                          6

//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------
// Enumeration

// Struct & Union
typedef struct
{
    union
    {
        unsigned long val;             // Software timer register value
        struct
        {
            unsigned long cnt:31;      // Counter of software timer
            unsigned long flg:1;       // Timeout flag of software timer
        }bits;
    }reg;
    unsigned long cmp;                 // Compare value of software timer, maximum value = 2^31 - 2
}TmrPkg;
//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
extern unsigned char Software_Timer(TmrPkg *pkg);
//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern DeviceInfo_t device_info;
extern const unsigned char COMPLIANT_TABLE[][20];
//extern unsigned long APP_ROM_SIZE, APP_ROM_START_ADDRESS, APP_RESERVED_ADDRESS, APP_START_ADDRESS;/*, CCMRAM_START_ADDRESS;*/
#endif /* __MAIN_H__ */


