/**************************************************************************************************
File Name: main.c
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
#include "main.h"
#include "netif.h"
#include "app_manager.h"

// App header information for entire code building
//#pragma location = ".signature"
//__root const StringWordType APP_SIGNATURE = {"APP"};
//#pragma location = ".header"
//__root const StdHeaderMainType TEXT_HEADER_ROM = App_Header_List;
//#pragma location = ".header"
//__root const unsigned long COMPLIANT_TABLE_ADDRESS = (unsigned long)COMPLIANT_TABLE;
//#pragma location = ".header"
//__root const CodeVersionType VERSION_ROM = Version_List;
//#pragma location = ".compliant"
//__root const unsigned char COMPLIANT_TABLE[][20] = Compliant_Model;

/* Local function prototypes */
void CheckDeviceInfo(void);
void Flash_Read_DeviceInfo(u8 *pBuffRead);
FLASH_STS Flash_Write_DeviceInfo(DeviceInfo_t *info);
void NetworkApplicationTask(void *arg);
void NetworkInitialize(void);
void MonitorTask(void *arg);
void Initial_Setup(void);
void GPIO_Output_Initial(void);
/* Data definitions */
TaskHandle_t NetworkApplicationTaskHandle = NULL;

struct netif sNetif;
ip4_addr_t ipaddr, netmask, gw;
DeviceInfo_t device_info;

/************************************************************************************************
Function Name:
    void CheckDeviceInfo(void)
Inputs:
    None
Outputs:
    None
Description:
    This function checks if the flash memory is empty. If it is, it generates new device info and writes
    it to the flash memory. If the flash is not empty, it loads the existing device info from flash memory.
    This function performs a device information check.
    If a valid signature ("N32H487") is found in the Flash memory, the existing device data (ID, SN, MAC) is loaded.
    If the Flash is empty or invalid, the function generates new unique device parameters based on the MCU's UID.
**************************************************************************************************/
void CheckDeviceInfo(void)
{
    const u8 *hexChars = "0123456789ABCDEF";
    const u32 *uidPtr = (const u32*)0x1FFFC760;     // N32_UID_BASE_ADDR = 0x1FFFC760
    u8 pDeviceInfo[DEVICE_STRUCT_SIZE] = {0};
    u8 hexID[DEVICE_ID_SIZE + 1] = { 0 }, hexSN[DEVICE_SN_SIZE + 1] = { 0 }, hashByte;
    u32 uidHash, idHash, snHash, deviceInfoIndex = 0;
    FLASH_STS bStatus;

    /* Read device info from flash address*/
    memcpy(pDeviceInfo, (char*)(FLASH_WRITE_START_ADDR), FLASH_WRITE_SIZE);

    /*check sentity for valid value*/
    if (strncmp((char *)pDeviceInfo, "N32H487", 7) != 0)
    {
        /* Enable CRC clock and reset the CRC data */
        RCC_EnableAHBPeriphClk(RCC_AHB_PERIPHEN_CRC, ENABLE);
        CRC32_ResetCrc();
        // Calculate a single hash from the 96-bit UID and derive SN and ID Hash
        uidHash = CRC32_CalcBufCrc(uidPtr, 3);
        snHash = (uidHash << 17) ^ (uidHash >> (32 - 17)) ^ 0xA5A5A5A5;
        idHash = (uidHash >> 11) ^ (uidHash << (32 - 11)) ^ 0x5A5A5A5A;

        // Get new Serial Number
        for (int i = 0; i < DEVICE_SN_SIZE; i++)
        {
            hashByte = (snHash >> (i * 4)) & 0xF;
            hexSN[i] = hexChars[hashByte];
        }
        hexSN[DEVICE_SN_SIZE] = '\0';
        /*generate final serial number include HW and board type, batch number and hex value*/
        sprintf((char *)device_info.SNO, "%s-%s-%s-%s", HW_TYPE, BOARD_TYPE, BATCH_ID, hexSN);

        // Get new Device ID
        for (int i = 0; i < DEVICE_ID_SIZE; i++)
        {
            hashByte = (idHash >> (i * 4)) & 0xF;
            hexID[i] = hexChars[hashByte];
        }
        hexID[DEVICE_ID_SIZE] = '\0';
        /*generate device id of BAM100 board*/
        sprintf((char *)device_info.device_id, "BAM-%s", hexID);

        // Get new Mac Address
        // Set the locallly addministered address bit and clear the multicast bit
        aMACAddr[0] = (MAC_ADDR0) & 0XFC | 0x02;
        aMACAddr[1] = MAC_ADDR1;
        aMACAddr[2] = MAC_ADDR2;
        aMACAddr[3] = (u8)(uidHash >> 24) & 0xFF;
        aMACAddr[4] = (u8)(uidHash >> 16) & 0xFF;
        aMACAddr[5] = (u8)(uidHash >> 8) & 0xFF;
        /*format mac address into string*/
        sprintf((char *)device_info.mac_address, "%02X.%02X.%02X.%02X.%02X.%02X", aMACAddr[0], aMACAddr[1], aMACAddr[2],
                aMACAddr[3], aMACAddr[4], aMACAddr[5]);

        // Get new firmware version
        strncpy((char*)device_info.firmware_version, FIRMWARE, 8);

        /* store new device info to flash at address 0x0807 0000*/
        bStatus = Flash_Write_DeviceInfo(&device_info);

        if (bStatus != FLASH_EOP)
        {
            log_info("Uable to store device Data....\r\n");
            return;
        }
    }
    else
    {
        // Copy the Serial Number from flash
        memcpy(device_info.SNO, pDeviceInfo + deviceInfoIndex, SNO_LENGTH);
        deviceInfoIndex += SNO_LENGTH;

        // Copy the Device ID from flash
        memcpy(device_info.device_id, pDeviceInfo + deviceInfoIndex, ID_LENGTH);
        deviceInfoIndex += ID_LENGTH;

        // Copy the MAC Address from flash
        memcpy(device_info.mac_address, pDeviceInfo + deviceInfoIndex, MAC_LENGTH);
        deviceInfoIndex += MAC_LENGTH;
        sscanf((const char *)device_info.mac_address, "%hhx.%hhx.%hhx.%hhx.%hhx.%hhx",
               &aMACAddr[0], &aMACAddr[1], &aMACAddr[2], &aMACAddr[3], &aMACAddr[4], &aMACAddr[5]);

        // Copy the FW Version from flash
        memcpy(device_info.firmware_version, pDeviceInfo + deviceInfoIndex, FW_VER_LENGTH);
    }
    return;
}

/**************************************************************************************************
Function Name:
    void GenerateDeviceInfo(DeviceInfo_t *info)

Inputs:
    info - Pointer to a DeviceInfo_t structure to store generated device information.

Outputs:
    None

Description:
    This function generates random values to initialize the device information, including:
    1. Serial number (SNO): Combines HW_TYPE, BOARD_TYPE, BATCH_ID and a random 6-digit hex number.
    2. Device ID: A prefix "BAM-" followed by a random 6-digit hex number.
    3. Firmware version: Copied from the predefined FIRMWARE macro.

    The randomness is based on the current system time as a seed.
**************************************************************************************************/
void GenerateDeviceInfo(DeviceInfo_t *info)
{
    uint32_t *uid_ptr  = (uint32_t *)0x1FFFC760;  // UID base, 96-bit = 3 x 32-bit

    uint32_t uid0 = uid_ptr[0];
    uint32_t uid1 = uid_ptr[1];
    uint32_t uid2 = uid_ptr[2];

    uint32_t seed = uid0 ^ uid1 ^ uid2;
    srand(seed);

    uint32_t rand_val = rand() % 0x1000000;

    sprintf((char *)info->SNO, "%s-%s-%s-%06lX", HW_TYPE, BOARD_TYPE, BATCH_ID, (unsigned long) rand_val);

    sprintf((char *)info->device_id, "BAM-%06lX", (unsigned long) rand_val);

    snprintf((char *)info->firmware_version, sizeof(info->firmware_version), "%s", FIRMWARE);
}

/**************************************************************************************************
Function Name:
    FLASH_STS Flash_Write_DeviceInfo(DeviceInfo_t *info)
Inputs:
    info - Pointer to a DeviceInfo_t structure containing the information to write.
Outputs:
    FLASH_STS - Status of the flash write operation (FLASH_EOP if successful, error otherwise).
Description:
    This function writes the device information stored in info to flash memory. The flash page is first
    erased, and then the data is written in double-word chunks (8 bytes at a time).
    Any remaining bytes are padded with 0xFF.
**************************************************************************************************/
FLASH_STS Flash_Write_DeviceInfo(DeviceInfo_t *info)
{
    FLASH_STS status = FLASH_EOP;
    u8 buffer[FLASH_WRITE_SIZE];

    // Copy the device info from buffer
    memcpy(buffer, info, DEVICE_STRUCT_SIZE);
    // Pad with 0xFF
    memset(buffer + DEVICE_STRUCT_SIZE, 0xFF, FLASH_WRITE_SIZE - DEVICE_STRUCT_SIZE);

    FLASH_Unlock();

    status = FLASH_EraseOnePage(FLASH_WRITE_START_ADDR);

    if (status == FLASH_EOP)
    {
        for (u32 i = 0; i < FLASH_WRITE_SIZE; i += 8)
        {
            status = FLASH_ProgramdoubleWord(FLASH_WRITE_START_ADDR + i,
                                             *(u32 *)(buffer + i), *(u32 *)(buffer + i + 4));

            if (status != FLASH_EOP)
                break;
        }
    }

    FLASH_Lock();

    return status;
}
/**************************************************************************************************
Function Name:
    void NetworkApplicationTask(void *arg)

Input:
    *arg - Unused, reserved for FreeRTOS task parameter.

Output:
    None

Comment:
    This task is responsible for initializing the network interface, starting DHCP,
    printing the assigned IP configuration, and initializing other major tasks such as
    MQTT, JSON parsing, and system monitoring. Once initialization is complete,
    this task deletes itself.
**************************************************************************************************/
void NetworkApplicationTask(void *arg)
{
    /* Initialize peripheral module */
    Initial_Setup();

#if (MPQ7210_NEW  == 1)
    while ((((DeviceConfigurationRegister *)(*(mpq7210Command.index + DEV_CFG))->dataPtr)->all & 0x00FF) != 0x000A)
    {
#else
    while ((((DeviceConfigurationRegister *)(*(mpq7210Command.index + DEV_CFG))->dataPtr)->all & 0xFFF) != 0x00A)
    {
        ((DeviceConfigurationRegister *)(*(mpq7210Command.index + DEV_CFG))->dataPtr)->all = 0x30A;
        Mpq7210_Command_Pack_Enqueue(DEV_CFG, SPI_PORT_0, DATA_WRITE, HIGH_PRIORITY);
        Mpq7210_Communication_Process(&masterSpi1, &mpq7210PriorityQueue);
        Mpq7210_Command_Pack_Enqueue(DEV_CFG, SPI_PORT_1, DATA_READ, HIGH_PRIORITY);
        Mpq7210_Communication_Process(&masterSpi1, &mpq7210PriorityQueue);
    }


    while ((((DeviceConfigurationRegister *)(*(mpq7210Command.index + DEV_CFG))->dataPtr)->all & 0xFFF) != 0x00A)
    {
#endif
        ((DeviceConfigurationRegister *)(*(mpq7210Command.index + DEV_CFG))->dataPtr)->all = 0x30A;
        Mpq7210_Command_Pack_Enqueue(DEV_CFG, SPI_PORT_0, DATA_WRITE, HIGH_PRIORITY);
        Mpq7210_Communication_Process(&masterSpi1, &mpq7210PriorityQueue);
        Mpq7210_Command_Pack_Enqueue(DEV_CFG, SPI_PORT_1, DATA_READ, HIGH_PRIORITY);
        Mpq7210_Communication_Process(&masterSpi1, &mpq7210PriorityQueue);
    }

     /* Network initialization, adding network interfaces */
    NetworkInitialize();


   /* Link successful, print local IP information */
    if (netif_is_up(&sNetif)) {

      log_info("Network connection successful\n");
      /* Enable DHCP and determine whether it is enabled successfully */
      if (dhcp_start(&sNetif) == ERR_OK) {
           log_info("DHCP initialization successful, please wait...\n");
            /* Wait for IP address assignment to complete */
            while (ip_addr_cmp(&sNetif.ip_addr, &ipaddr)) {
                /* System can switch to other tasks */
               vTaskDelay(pdMS_TO_TICKS(2000));
            }

            log_info("You can ping the following printed IP address on your PC to check the correctness of DHCP and whether the network is connected or not\n");
            log_info("Local IP: %s\n", ipaddr_ntoa(&sNetif.ip_addr));
            log_info("Netmask: %s\n", ipaddr_ntoa(&sNetif.netmask));
            log_info("Gateway: %s\n", ipaddr_ntoa(&sNetif.gw));

      }
      else {
            //ToDo: Create a monitoring task for connection
            log_info("DHCP initialization failure\n");
      }
   }
    else {
       //ToDo: Create a monitoring task for connection
       log_info("Network not connected\n");
   }


    /* Initialize pasre task */
   // parse_init();

    if (xTaskCreate(uart_de_scan,
                    "uart_de_scan",
                    UART_DE_SCAN_TASK_STACK_SIZE,
                    NULL,
                    UART_DE_SCAN_TASK_PRIORITY,
                    &UartDeScanTaskHandle) != pdPASS) {
        log_info("Task <<scan_device>> creation failed!\n");
    }

    /* Initialize adc task */
    adc_init();

    /* Initialize boot task */
    boot_init();

    /* Initialize gui task */
    gui_init();

    /* Initialize mpq7210 task */
    mpq7210_init();

    /* Initialize power task */
    power_init();

    /* Initialize device task */
    device_init();

    /*initialize app manager*/
    app_manager_init();

    LED_RED_OFF;

    /* Close the task */
    vTaskDelete(NetworkApplicationTaskHandle);
}

/**************************************************************************************************
Function Name:
    void NetworkInitialize(void)

Input:
    None

Output:
    None

Comment:
    This function initializes the network interface using LwIP.
    - Initializes IP address, netmask, and gateway to zero.
    - Initializes the LwIP TCP/IP stack.
    - Adds the network interface to the LwIP stack.
    - Sets the added interface as the default.
    - Brings the interface up if the link is up; otherwise, sets it down.
**************************************************************************************************/
void NetworkInitialize(void)
{
    /* IP addresses initialization */
    ip_addr_set_zero(&ipaddr);
    ip_addr_set_zero(&netmask);
    ip_addr_set_zero(&gw);

    /* Initializes the LwIP stack and starts the tcpip_thread thread */
    tcpip_init(NULL, NULL);

    /* Add the network interface (IPv4/IPv6) */
    netif_add(&sNetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

    /* Registers the default network interface */
    netif_set_default(&sNetif);

    if (netif_is_link_up(&sNetif))
    {
        /* When the netif is fully configured this function must be called */
        netif_set_up(&sNetif);
    }
    else
    {
        /* When the netif link is down this function must be called */
        netif_set_down(&sNetif);
    }
}

/**************************************************************************************************
Function Name:
    int main(void)

Input:
    None

Output:
    return - Not used; the program enters FreeRTOS scheduler or loops forever.

Comment:
    Entry point of the application. Initializes the logging module, creates the network
    application task, and starts the FreeRTOS scheduler. If the task creation fails,
    it logs an error message and enters an infinite loop.
**************************************************************************************************/
int main(void)
{
    // Relocate the vector table
//    SCB->VTOR = 0x08004200;

    BaseType_t xReturn = pdFAIL;

    log_init();

    log_info("ETH DHCP (with FreeRTOS) demo go...\r\n\r\n");

    CheckDeviceInfo();

    /* Creates a Network Application Task */
    xReturn = xTaskCreate(NetworkApplicationTask,
                          "NetworkApplicationTask",
                          NETWORK_APP_TASK_STACK_SIZE,
                          NULL,
                          NETWORK_APP_TASK_PRIORITY,
                          &NetworkApplicationTaskHandle);

    if (xReturn == pdPASS) {
      /* Task created successfully, start scheduling */
      vTaskStartScheduler();

    }
    else
    {
        /* Task creation failed, print message */
        log_info("Task <<NetworkApplicationTask>> creation failed!!\r\n");
    }

    while (1);
}

/************************************************************************************************
Function Name:
    void Initial_Setup(void)
Input:
    NULL
Output:
    NULL
Comment:
    Initialize all setting.
************************************************************************************************/
void Initial_Setup(void)
{
    // Set up peripheral
    Peripheral_Setup();
    // Gui protocol initialization
    Gui_Command_List_Setup(&guiCommand);
    // MPQ7210 protocol initialization
    Mpq7210_Command_List_Setup(&mpq7210Command);
    // Shade protocol initialization
    shadeProtocol.func.init(&shadeProtocol, &shadeCommand);
    shadeProtocol.member.txQueue->highPriority.empty = 1;
    shadeProtocol.member.txQueue->lowPriority.empty = 1;
    // HMI protocol initialization
    hmiProtocol.func.init(&hmiProtocol, &hmiCommand);
    hmiProtocol.member.txQueue->highPriority.empty = 1;
    hmiProtocol.member.txQueue->lowPriority.empty = 1;
    // Light protocol initialization
    lightProtocol.func.init(&lightProtocol, &lightCommand);
    lightProtocol.member.txQueue->highPriority.empty = 1;
    lightProtocol.member.txQueue->lowPriority.empty = 1;
    // Meter protocol initialization
    meterProtocol.func.init(&meterProtocol, &meterCommand);
    meterProtocol.member.txQueue->highPriority.empty = 1;
    meterProtocol.member.txQueue->lowPriority.empty = 1;
    // Air protocol initialization
    airProtocol.func.init(&airProtocol, &airCommand);
    airProtocol.member.txQueue->highPriority.empty = 1;
    airProtocol.member.txQueue->lowPriority.empty = 1;
    // GPIO state initial
    GPIO_Output_Initial();
    // Enable all driver
    ALL_DEVICE_ENABLE;
    // Enable interrupt service after setup
    __enable_irq();
    // Enable watchdog after all setting procedures are done
//    Enable_Watchdog();
}
/************************************************************************************************
Function Name:
    void GPIO_Output_Initial(void)
Input:
    NULL
Output:
    NULL
Comment:
    Set GPIO output initial state.
************************************************************************************************/
void GPIO_Output_Initial(void)
{
    // Disable all LED driver's chip select
    Disable_Chip_Select(1);
    Disable_Chip_Select(2);
    Disable_Chip_Select(3);
    Disable_Chip_Select(4);
    Disable_Chip_Select(5);
    Disable_Chip_Select(6);
    Disable_Chip_Select(7);
    Disable_Chip_Select(8);
    Disable_Chip_Select(9);
    Disable_Chip_Select(10);
    Disable_Chip_Select(11);
    Disable_Chip_Select(12);
    Disable_Chip_Select(13);
    Disable_Chip_Select(14);
    Disable_Chip_Select(15);
    Disable_Chip_Select(16);
    Disable_Chip_Select(17);
    Disable_Chip_Select(18);
    Disable_Chip_Select(19);
    Disable_Chip_Select(20);
    Disable_Chip_Select(21);
    Disable_Chip_Select(22);
    // LED control
    LED_GREEN_ON;
    LED_RED_ON;
}
/**************************************************************************************************
Function Name:
    char Software_Timer(TmrPkg *pkg)
Input:
    *pkg      - The pointer points to a timer structure which is counted up by this
                function to detect a timeout condition.
Output:
    return    - Result. 1 = a timeout condition is detected, 0 = no timeout condition.
Comment:
    Software timer function.
**************************************************************************************************/
unsigned char Software_Timer(TmrPkg *pkg)
{
    if (pkg->reg.bits.flg == 0)
    {
        if (++pkg->reg.bits.cnt >= pkg->cmp)
        {
            pkg->reg.bits.cnt = 0;
            pkg->reg.bits.flg = 1;
        }
    }
    return pkg->reg.bits.flg;
}

