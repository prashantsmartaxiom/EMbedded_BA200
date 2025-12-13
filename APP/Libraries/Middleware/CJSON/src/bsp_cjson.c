/**
*\*\file bsp_cjson.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2023, Nations Technologies Inc. All rights reserved.
**/
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp_cjson.h"


/**
*\*\name    cJSON_Init.
*\*\fun     Create cJSON object, add key-value pair initial value.
*\*\param   none
*\*\return  none
**/
cJSON* cJSON_Init(void)
{
    /* JSON root node */
    cJSON *pRoot = NULL;

    /* Create Project */
    pRoot = cJSON_CreateObject();
    if (pRoot == NULL)
    {
        return NULL;
    }

    /* Adds initial value of key-value pairs */
    /* Get key-value pairs */
    cJSON_AddNumberToObject(pRoot,CH_T, DEFAULT_CH_T);
    cJSON_AddStringToObject(pRoot,CH_ADDR, DEFAULT_CH_ADDR);
    cJSON_AddNumberToObject(pRoot,CMD,DEFAULT_CMD);
    cJSON_AddStringToObject(pRoot,CMD_MSG, DEFAULT_MSG);

    return pRoot;
}

/**
*\*\name    cJSON_Update.
*\*\fun     Update cJSON format data.
*\*\param   object
*\*\         - Pointer to a cJSON object
*\*\param   string
*\*\         - Pointer to the item string
*\*\param   pData
*\*\         - Pointer to the data to update
*\*\return  UPDATE_SUCCESS or UPDATE_FAIL
**/
uint8_t cJSON_Update(const cJSON * const object, const char * const string, void *pData)
{
    /* JSON root node */
    cJSON *pRoot = NULL;
    uint8_t retStatus = UPDATE_FAIL;

    pRoot = cJSON_GetObjectItem(object, string);
    
    if (pRoot == NULL) {
        retStatus = UPDATE_FAIL;
    }

    if (cJSON_IsBool(pRoot))
    {
        int *b = (int *)pData;
        cJSON_GetObjectItem(object, string)->type = *b ? cJSON_True : cJSON_False;
        retStatus = UPDATE_SUCCESS;
    }
    else if (cJSON_IsString(pRoot))
    {
        cJSON_GetObjectItem(object, string)->valuestring = (char *)pData;
        retStatus = UPDATE_SUCCESS;
    }
    else if (cJSON_IsNumber(pRoot))
    {
        double *num = (double *)pData;
        cJSON_GetObjectItem(object, string)->valuedouble = (double)*num;
        retStatus = UPDATE_SUCCESS;
    }
    else
    {
        /* Do nothing */
    }

    return retStatus;
}

/**
*\*\name    cJSON_Process.
*\*\fun     Processes data in cJSON format.
*\*\param   pData
*\*\         - Pointer to data to be processed
*\*\return  none
**/
void cJSON_Config_Process(void *pData, ControlMsg_t *ControlMsg)
{
    cJSON *pRoot    = NULL;
    cJSON *pCh_t    = NULL;
    cJSON *pCh_addr = NULL;
    cJSON *pCMD_t   = NULL;
    cJSON *pMsg     = NULL;
    
    int array_sz    = 0;

    /* Parsed into JSON */
    pRoot = cJSON_Parse((char *)pData);

    /* Get key-value pairs */
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
      

    /* Free Memory */
    cJSON_Delete(pRoot);
}


//
//char *CreateActiveLEDList() 
//{
//   cJSON *pRoot            = NULL;
//   char *c_string          = NULL;
//   
//    /* cJSON initialization */
//   pRoot = cJSON_CreateObject(); 
//  
//   //Config fpr each LED channel
//   cJSON *led_a = cJSON_CreateArray();
//   
//   for ( int count = 0; count < 4;count++)
//   {
//       char led_name[10];
//       sprintf(led_name, "LED%d",count+1);
//       
//       cJSON *led_obj = cJSON_CreateObject();
//       cJSON *led_data = cJSON_CreateObject();       
//       cJSON_AddNumberToObject(led_data, "STATUS", LED_Get_Status(count));
//       cJSON_AddNumberToObject(led_data, "BRIGHTNESS", LED_Get_Brightness(count));
//       cJSON_AddNumberToObject(led_data, "MAX_CURR", LED_Get_Currentlimit(count));                        
//       
//       cJSON_AddItemToObject(led_obj,led_name,led_data);
//       cJSON_AddItemToArray(led_a,led_obj); 
//   }
//   
//   
//   //Add LED Channel to root
//   cJSON_AddItemToObject(pRoot,"LED_CHANNELS",led_a);
//   
// /*  
//  //SHADE Channel Object 
//   cJSON *shade_channel = cJSON_CreateObject();
//   cJSON_AddNumberToObject(shade_channel, "COUNT",MAX_SHADE_CHANNEL);
//   
//   //Active LED Channel 
//   cJSON *shade_active = cJSON_CreateIntArray((int[]) {1,2,3,4},4);
//   cJSON_AddItemToObject(shade_channel, "ACTIVE",shade_active);
//   int shade_number[MAX_LED_CHANNEL] = { 0 };
//   
//   
//   //LED Active channel status 
//   for ( int shade_ch = 0 ; shade_ch < MAX_SHADE_CHANNEL; shade_ch++)
//   {
//     shade_number[shade_ch] = shade_ch+1;
//   }
//   
//   //Config array
//   cJSON *shade_a = cJSON_CreateArray();
//   
//   for (int count = 0; count < MAX_SHADE_CHANNEL;count++)
//   {
//       char shade_name[10];
//       sprintf(shade_name, "SHADE%d",shade_number[count]);
//       
//       cJSON *shade_obj = cJSON_CreateObject();
//       cJSON *shade_data = cJSON_CreateObject();
//
//       if ( count == 0  || count == 1) {       
//           cJSON_AddNumberToObject(shade_data, "RS485_ADD", count+1);
//       } else if(count == 2 || count == 3) {
//             cJSON_AddNumberToObject(shade_data, "RS485_ADD", count+1);
//       }
//      
//       cJSON_AddItemToObject(shade_obj,shade_name,shade_data);
//       cJSON_AddItemToArray(led_name_t,shade_obj); 
//   }
//   
//   //Add LED channel to array 
//   cJSON_AddItemToObject(shade_channel,"CONFIG",shade_a);
//   
//   //Add LED Channel to root
//   cJSON_AddItemToObject(pRoot,"SHADE_CHANNELS",shade_channel);
//   
//*/   
//   c_string = cJSON_PrintUnformatted(pRoot);
//   
//   if( c_string == NULL) 
//   {
//       printf("unable to assemble config object\n");
//   }
//   
//   printf("config %s\n",c_string);
//   
//   cJSON_Delete(pRoot);
//   
//   return c_string;
//    
//   
//}
