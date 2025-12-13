/**************************************************************************************************
File Name: parse.c
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
#include "parse.h"
#include "main.h"

/* Local function prototypes */

/* Data definitions */
TaskHandle_t ParseProcessTaskHandle = NULL;

/**
*\*\name    cJSON_Process.
*\*\fun     Processes data in cJSON format.
*\*\param   pData
*\*\         - Pointer to data to be processed
*\*\return  none
**/
void cJSON_Process(void *pData, ControlMsg_t *ControlMsg)
{
    cJSON *pRoot = NULL, *pCh_t = NULL, *pCh_addr = NULL, *pCMD_t = NULL, *pMsg = NULL;
    int array_sz    = 0;

    /* Parsed into JSON */
    pRoot = cJSON_Parse((char *)pData);

    /* Get key-value pairs */
    if( pRoot != 0x0)  {
      pCh_t    = cJSON_GetObjectItem(pRoot,CH_T);
      pCh_addr = cJSON_GetObjectItem(pRoot,CH_ADDR);
      pCMD_t   =  cJSON_GetObjectItem(pRoot,CMD);
      pMsg     = cJSON_GetObjectItem(pRoot,CMD_MSG);

      //channel type
      strncpy((char *)ControlMsg->channel_type,pCh_t->valuestring,strlen((char *)pCh_t->valuestring));

      //channel address
      if (cJSON_IsArray(pCh_addr))
      {
           array_sz = cJSON_GetArraySize(pCh_addr);
           if ( array_sz <= MAX_LED_CHANNEL)
           {
             for (int i = 0; i<array_sz;i++)
             {
                cJSON *item = cJSON_GetArrayItem(pCh_addr,i);
                ControlMsg->gr_addr[i] =  item->valueint;
             }
           }
      }
      else
      {
           strncpy((char*)ControlMsg->channel_addr, pCh_addr->valuestring,strlen((char*) pCh_addr->valuestring));
      }

      //command
      ControlMsg->command = (command_t) pCMD_t->valueint;

      //command payload
      if (cJSON_IsString(pMsg))
      {
          strcpy((char *)ControlMsg->message, pMsg->valuestring);
      }
      else if (cJSON_IsNumber(pMsg))
      {
          sprintf((char *)ControlMsg->message,"%d",pMsg->valueint);
      }
      else if (cJSON_IsArray(pMsg))
      {
          array_sz = cJSON_GetArraySize(pMsg);
          for (int i = 0; i<array_sz;i++)
          {
              cJSON *item = cJSON_GetArrayItem(pMsg,i);
              if (cJSON_IsNumber(item))
              {
                  ControlMsg->message[i] =  item->valueint;
              }
          }
      }
      else if( cJSON_IsObject(pMsg) )
      {
          cJSON *pBrightness = cJSON_GetObjectItemCaseSensitive(pMsg, "LED_BRIGHTNESS");
          sprintf((char *)ControlMsg->message,"%s:%d","LED_BRIGHTNESS",pBrightness->valueint);
      }
    }

    /* Free Memory */
    cJSON_Delete(pRoot);
}

void Pir_ProcessCommand(uint8_t *pChAddr,uint8_t pCmdType, uint8_t *pCmdMsg)
{
    BaseType_t xReturned;
    ControlMsg_t ControlMsg = {0};
    char pMsg[50] = {0};

    if ( pChAddr == NULL || pCmdMsg == NULL)
    {
        return;
    }

    sprintf((char *)ControlMsg.response_topic, "%s%s%s", PUB_BASE_TOPIC, device_info.device_id, CONFIG);
    ControlMsg.command = PIR_CONFIG;
    strncpy((char*)ControlMsg.message, pMsg, strlen(pMsg));
    snprintf((char *)ControlMsg.channel_type, sizeof(ControlMsg.channel_type), "%s", "PIR");

    xReturned = xQueueSend(MQTTResponseQueueHandle, /* Message Queue Handle */
                           (void *) &ControlMsg,    /* Content of messages sent */
                           0);                      /* Wait time in ms */

    if ( xReturned == SUCCESS)
    {
        printf("Publishing Command to Queue for processing %s\n",ControlMsg.message);
    }

}

/**************************************************************************************************
Function Name:
    void ProcessCommand(ControlMsg_t *pControlMsg)

Inputs:
    pControlMsg - Pointer to a ControlMsg_t structure containing command details
                  such as channel type, address, command ID, and message string.

Outputs:
    None

Description:
    Processes control commands based on the channel type.

    Steps:
    1. If the input pointer is NULL, the function exits immediately.
    2. Checks the channel type and calls the appropriate command handler:
       - "LED": Placeholder for future LED control logic.
       - "SHADE": Placeholder for future shade control logic.
       - "PIR": Calls Pir_ProcessCommand() to handle PIR-related commands.
    3. If the channel type is unrecognized, prints an error message.

**************************************************************************************************/
void ProcessCommand(ControlMsg_t *pControlMsg)
{
    if (pControlMsg == NULL)
    {
        return;
    }

    if (strncmp((char*)pControlMsg->channel_type, "LED", 3) == 0)
    {
//        if (pControlMsg->command == SCENE_CMD)
//            Process_LED_Sense(pControlMsg);
//        else
//            LED_ProcessCommand(pControlMsg->channel_addr,pControlMsg->command,pControlMsg->message);
    }
    else if(strncmp((char*)pControlMsg->channel_type,"SHADE",5) == 0)
    {
//        Shade_ProcessCommand(pControlMsg->channel_addr,pControlMsg->command,pControlMsg->message);
    }
    else if(strncmp((char*)pControlMsg->channel_type,"PIR",3) == 0)
    {
        Pir_ProcessCommand(pControlMsg->channel_addr,pControlMsg->command,pControlMsg->message);
    }
    else
    {
        printf("Invalid channel type \n");
    }
}

//void uart_thread(void *arg)
//{
//    printf("\r\nCall %s\r\n", __FUNCTION__);
//
//    LWIP_UNUSED_ARG(arg);
//
//    while (1)
//    {
//        // PIR J6
//        if (USART_GetFlagStatus(UART5, USART_FLAG_TXC) == SET)
//        {
//            USART_ClrFlag(UART5, USART_FLAG_TXC);
//            GPIO_ResetBits(GPIOD, GPIO_PIN_0);
//        }
//
//        // PIR J31
//        if (USART_GetFlagStatus(USART2, USART_FLAG_TXC) == SET)
//        {
//            USART_ClrFlag(USART2, USART_FLAG_TXC);
//            GPIO_ResetBits(GPIOC, GPIO_PIN_7);
//        }
//
//        // Shade 1&2
//        if (USART_GetFlagStatus(USART3, USART_FLAG_TXC) == SET)
//        {
//            USART_ClrFlag(USART3, USART_FLAG_TXC);
//            GPIO_ResetBits(GPIOD, GPIO_PIN_12);
//        }
//
//        // Shade 3&4
//        if (USART_GetFlagStatus(UART8, USART_FLAG_TXC) == SET)
//        {
//            USART_ClrFlag(UART8, USART_FLAG_TXC);
//            GPIO_ResetBits(GPIOD, GPIO_PIN_4);
//        }
//    }
//}

/**************************************************************************************************
Function Name:
    void parse_process(void *arg)

Inputs:
    arg - Pointer to arguments passed when the task is created (unused in this function).

Outputs:
    None

Description:
    Task function that runs in a FreeRTOS environment to process MQTT control messages.

    Steps:
    1. Prints a message indicating the function has been called.
    2. Enters an infinite loop:
       - Clears the content of the ControlMsg_t structure.
       - Tries to receive a message from MQTTCommandQueueHandle with a 10ms timeout.
       - If a message is received:
           - Calls ProcessCommand() to handle the message.
       - Delays the task for 100ms between iterations to allow other tasks to run.

**************************************************************************************************/
void parse_process(void *arg)
{
    BaseType_t xReturn = pdTRUE;
    ControlMsg_t pControlMsg;

    printf("\r\nCall %s\r\n", __FUNCTION__);

    while (1)
    {
        memset(&pControlMsg, 0, sizeof(ControlMsg_t));

        xReturn = xQueueReceive(MQTTCommandQueueHandle,  /* Message queue handle */
                                &(pControlMsg),          /* Content of messages sent */
                                (TickType_t )10);        /* Wait time in ms */

        if (xReturn == pdTRUE)
        {
            /* Publish messages  */
            ProcessCommand(&pControlMsg);
        }

        /* Update data every 100 miliseconds */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**************************************************************************************************
Function Name:
    void parse_init(void)

Inputs:
    None

Outputs:
    None

Description:
    Initializes the parsing task by creating a FreeRTOS task named "parse_process".

    Steps:
    1. Prints a log message indicating the function has been called.
    2. Calls xTaskCreate() to start the task "parse_process" with the following parameters:
       - Task function pointer: parse_process
       - Task name: "parse_process"
       - Stack size: PARSE_PROCESS_TASK_STACK_SIZE
       - Task parameters: NULL
       - Priority: PARSE_PROCESS_TASK_PRIORITY
       - Task handle: &ParseProcessTaskHandle
    3. If task creation fails (return is not pdPASS), prints an error message.

**************************************************************************************************/
void parse_init(void)
{
    printf("\r\nCall %s\r\n", __FUNCTION__);

    if (xTaskCreate(parse_process,
                    "parse_process",
                    PARSE_PROCESS_TASK_STACK_SIZE,
                    NULL,
                    PARSE_PROCESS_TASK_PRIORITY,
                    &ParseProcessTaskHandle) != pdPASS)
    {
        log_info("Task <<parse_process>> creation failed!\n");
    }
}

