
/**************************************************************************************************
File Name: mqtt_client.c
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
#include "mqtt_client.h"
#include "bsp_cjson.h"
#include "parse.h"
#include "main.h"


/* Local function prototypes */
void MQTT_ProcessUserMessage(MQTTUserMsgType *pUserMsg);
void MQTT_SaveUserMessage(MQTTString *pTopicName, MQTTMessageType *pMsg, MQTTUserMsgType *pUserMsg);
void MQTT_ProcessPacket(uint8_t pkType, uint8_t *pbuf, uint32_t buflen);
int32_t MQTT_Publish(int32_t sock, char *pTopic, QoSType qos, uint8_t* pMsg);
int32_t MQTT_PingReq(int32_t sock);
int32_t MQTT_SessionPingReq(int32_t sock);
int32_t MQTT_Subscribe(int32_t sock, char *pTopic, QoSType qosMax);
char *create_control_message(ControlMsg_t *pData);
char *create_availability(void);
char *create_discovery(void);
ConnectType MQTT_Connect(void);
void MQTT_ClientInit(void);
void mqtt_client_publish(void *arg);
void mqtt_client_receive(void *arg);
void mqtt_init(void);

/* Data definitions */
QueueHandle_t MQTTCommandQueueHandle = NULL;
QueueHandle_t MQTTResponseQueueHandle = NULL;
TaskHandle_t MQTTPublishTaskHandle = NULL;
TaskHandle_t MQTTReceiveTaskHandle = NULL;
MQTTUserMsgType MQTTUserMsg;
uint8_t noMQTTMsgExchange = 1;
int MQTTSock = -1;

char *create_response_message(uint8_t *pMsg)
{
    char *string = NULL;
    cJSON *pRoot = NULL;

    /* cJSON initialization */
    pRoot = cJSON_Init();

    cJSON_AddStringToObject(pRoot,CMD_MSG,(char *)pMsg);

/*convert JSON object to sting*/
    string = cJSON_Print(pRoot);

    if (string == NULL)
    {
      printf("unable to convert JSON to string object");
      goto end;
    }
 end:
    cJSON_Delete(pRoot);
    return string;
}

static void mqtt_publish_msg(uint8_t *pMsg)
{
  uint8_t topic[MAX_TOPIC_LENGTH] =  { 0 };
   char         *p = NULL;
  
  memset(topic,0, sizeof(topic));
  
  p = create_response_message(pMsg);

  snprintf((char *)topic,MAX_TOPIC_LENGTH ,"%s%s%s",
                               PUB_BASE_TOPIC,device_info.device_id,STATUS);

  if (MQTT_Publish(MQTTSock, (char *)topic, QOS1, (uint8_t *)p) < 0)
  {
     log_info("MQTT_Publish fail!\n");
     /* Close the link */
     transport_close(MQTTSock);
     /* Reconnect to the server */
     MQTT_ClientInit();
  }
   
  free(p);
}



uint16_t GetDevGrpId(uint8_t *channel_type)
{
     if(channel_type == NULL) return -1;
     
     if(strncmp((char *)channel_type,"LED",3) == 0 || (strncmp((char *)channel_type,"led",3)) == 0)
        return DEV_LED;
     else if(strncmp((char *)channel_type,"PIR",3) == 0 || (strncmp((char *)channel_type,"pir",3)) == 0)
        return DEV_PIR;
     else if(strncmp((char *)channel_type,"SHADE",5) == 0 || (strncmp((char *)channel_type,"shade",3)) == 0)
        return DEV_SHADE;
     else if(strncmp((char *)channel_type,"HMI",3) == 0 || (strncmp((char *)channel_type,"hmi",3)) == 0)
        return DEV_HMI;
     else 
        return -1;
}

/**************************************************************************************************
Function Name:
void MQTT_ProcessUserMessage(MQTTUserMsgType *pUserMsg)

Inputs:
pUserMsg - Pointer to the user message structure containing the received MQTT message and topic.

Outputs:
None (void function)

Description:
Processes the received MQTT user message based on its topic and content.

Steps:
1. Constructs a reboot command topic string using base topic, device ID, and reboot suffix.
2. Compares the message topic with the reboot topic:
- If they match, triggers a system reset via NVIC_SystemReset().
3. If the topic is different:
- Parses the JSON message payload using cJSON_Process() into a control message structure.
- Copies the original topic into the control message's response topic field.
- Attempts to send the control message to a FreeRTOS queue (MQTTCommandQueueHandle) for further processing.
- Logs an error if the message queue is full or the send operation fails.
4. Marks the user message as invalid by setting valid to 0 after processing.

**************************************************************************************************/
void MQTT_ProcessUserMessage(MQTTUserMsgType *pUserMsg)
{
  BaseType_t xReturned;
  app_evt_t e =  { 0 };
  uint8_t topic[MAX_TOPIC_LENGTH] = {0};
  
  
  if(sscanf((char*)pUserMsg->topic,"%*[^/]/%*[^/]/%*[^/]/%s",topic) == 1)
  {  
    if(strncmp((char *)topic,"reboot",6) == 0) 
    {
      uint8_t status[] = "reboot";
      mqtt_publish_msg(status);
      NVIC_SystemReset();
    } else if (strncmp((char *)topic,"status",6) == 0 ) {
      uint8_t status[] = "connected";
      mqtt_publish_msg(status);
    } else {
    
      cJSON_Process(pUserMsg->msg, &e.u.mqtt.pControMsg);

      strcpy((char *)e.u.mqtt.pControMsg.response_topic, (char *)pUserMsg->topic);

      e.id = EVT_MQTT_CMD;
      e.grp_id = 1;
      e.ts_ms = xTaskGetTickCount()*portTICK_PERIOD_MS;
      e.u.mqtt.dev_id = GetDevGrpId(e.u.mqtt.pControMsg.channel_type);


      xReturned = xQueueSend(q_events,  /* Message Queue Handle */
                             (void *)&e, /* Content of messages sent */
                             ( TickType_t )0);       /* Wait time in ms */

      if ( xReturned != SUCCESS)
      {
            
          cJSON_Process(pUserMsg->msg, &e.u.mqtt.pControMsg);
          strcpy((char *)e.u.mqtt.pControMsg.response_topic, (char *)pUserMsg->topic);

          e.id = EVT_MQTT_CMD; 
          e.grp_id = 1; 
          e.ts_ms = xTaskGetTickCount()*portTICK_PERIOD_MS;
          e.u.mqtt.dev_id = GetDevGrpId(e.u.mqtt.pControMsg.channel_type);
        
          
          xReturned = xQueueSend(q_events,  /* Message Queue Handle */
                                 (void *)&e, /* Content of messages sent */
                                 ( TickType_t )0);       /* Wait time in ms */
          
          if ( xReturned != SUCCESS) 
          {  
              log_error("Unable to push command to Queue for processing %s\n", pUserMsg->msg);
          }
          
          pUserMsg->valid = 0;
      }

      pUserMsg->valid = 0;
    }
  }
}

/**************************************************************************************************
Function Name:
void MQTT_SaveUserMessage(MQTTString *pTopicName, MQTTMessageType *pMsg, MQTTUserMsgType *pUserMsg)

Inputs:
pTopicName - Pointer to the MQTT topic name structure of the received message.
pMsg       - Pointer to the MQTT message structure containing payload and related information.
pUserMsg   - Pointer to the user message structure where the message data will be saved.

Outputs:
None (void function)

Description:
This function extracts relevant information from the received MQTT message and stores it
into the user message structure.

Specifically:
- Saves the QoS level of the message to 'msgqos'.
- Copies the payload data into 'msg' and appends a null terminator.
- Stores the payload length in 'msglenth'.
- Copies the topic string into 'topic' and appends a null terminator.
- Saves the packet ID of the message.
- Marks the message as valid by setting 'valid' to 1.
**************************************************************************************************/
void MQTT_SaveUserMessage(MQTTString *pTopicName, MQTTMessageType *pMsg, MQTTUserMsgType *pUserMsg)
{
  /* Messages QoS */
  pUserMsg->msgqos = pMsg->qos;

  /* Saving messages */
  memcpy(pUserMsg->msg, pMsg->payload, pMsg->payloadlen);

  pUserMsg->msg[pMsg->payloadlen] = 0;

  /* Saving message length */
  pUserMsg->msglenth = pMsg->payloadlen;

  /* Message Topic */
  memcpy((char *)pUserMsg->topic, pTopicName->lenstring.data, pTopicName->lenstring.len);

  pUserMsg->topic[pTopicName->lenstring.len] = 0;

  /* Messages ID */
  pUserMsg->packetid = pMsg->id;

  /* Indicates that the message is valid */
  pUserMsg->valid = 1;
}

/**************************************************************************************************
Function Name:
void MQTT_ProcessPacket(uint8_t pkType, uint8_t *pbuf, uint32_t buflen)

Input:
pkType  - MQTT packet type identifier (e.g., PUBLISH, PUBREL, etc.).
pbuf    - Pointer to the buffer containing the received MQTT packet data.
buflen  - Length of the buffer data.

Output:
None (void function)

Comment:
This function processes incoming MQTT packets according to their type.

- For PUBLISH packets:
* Deserialize the publish message.
* Save the message into MQTTUserMsg.
* Process the message payload based on QoS:
- QoS0: Directly process the message (no ACK required).
- QoS1: Send PUBACK response after processing.
- QoS2: Send PUBREC response after reception; further PUBREL/PUBCOMP handled separately.

- For PUBREL packets (QoS2 flow):
* Deserialize PUBREL.
* If it matches the saved message packet ID, process the message.
* Send PUBCOMP response to complete the QoS2 handshake.

- Other packet types (PUBACK, PUBREC, PUBCOMP, PINGRESP) are currently ignored or handled elsewhere.

Notes:
- Relies on global MQTTUserMsg structure to track message state.
- Uses global socket MQTTSock for sending responses.
- Uses error macro ERR_PRINTF for error logging.
**************************************************************************************************/
void MQTT_ProcessPacket(uint8_t pkType, uint8_t *pbuf, uint32_t buflen)
{
  MQTTMessageType msg;
  MQTTString receivedTopic;
  uint32_t len;

  switch (pkType)
  {
  case PUBLISH:
    {
      /* Deserialize PUBLISH Messages */
      if (MQTTDeserialize_publish(&msg.dup,(int*)&msg.qos, &msg.retained, &msg.id, &receivedTopic,
                                  (unsigned char **)&msg.payload, &msg.payloadlen, pbuf, buflen) != 1)
      {
        ERR_PRINTF;
        return;
      }

      /* Save message to MQTTUserMsg */
      MQTT_SaveUserMessage(&receivedTopic, &msg, &MQTTUserMsg);

      /* Process data according to message QoS */
      if (msg.qos == QOS0) /* For QOS0, no response messages are required */
      {
        /* Process data */
        MQTT_ProcessUserMessage(&MQTTUserMsg);
      }
      else if (msg.qos == QOS1) /* For QOS1, a PUBACK response message is required */
      {
        /* Serialize PUBACK Messages */
        len = MQTTSerialize_puback(pbuf, buflen, MQTTUserMsg.packetid);

        if (len == 0 || transport_sendPacketBuffer(MQTTSock, pbuf, len) < 0) /* Send data */
        {
            /* Deserialize PUBLISH Messages */
            if (MQTTDeserialize_publish(&msg.dup,(int*)&msg.qos, &msg.retained, &msg.id, &receivedTopic,
                (unsigned char **)&msg.payload, &msg.payloadlen, pbuf, buflen) != 1)
            {
                ERR_PRINTF;
                return;
            }
            
            /* Save message to MQTTUserMsg */
            memset(&MQTTUserMsg,0, sizeof(MQTTUserMsgType));
            MQTT_SaveUserMessage(&receivedTopic, &msg, &MQTTUserMsg);
            
            /* Process data according to message QoS */
            if (msg.qos == QOS0) /* For QOS0, no response messages are required */
            {
                /* Process data */
                MQTT_ProcessUserMessage(&MQTTUserMsg);
            }
            else if (msg.qos == QOS1) /* For QOS1, a PUBACK response message is required */
            {
                /* Serialize PUBACK Messages */
                len = MQTTSerialize_puback(pbuf, buflen, MQTTUserMsg.packetid);
                
                if (len == 0 || transport_sendPacketBuffer(MQTTSock, pbuf, len) < 0) /* Send data */
                {
                    ERR_PRINTF;
                    return;
                }
                /* Process data */
                MQTT_ProcessUserMessage(&MQTTUserMsg);
            }
            else if (msg.qos == QOS2) /* For QOS2, a PUBREC response message is required */
            {
                /* Serialize ACK Messages (PUBREC) */
                len = MQTTSerialize_ack(pbuf, buflen, PUBREC, 0, MQTTUserMsg.packetid);
                
                if (len == 0)
                {
                    ERR_PRINTF;
                    return;
                }
                /* Send data */
                transport_sendPacketBuffer(MQTTSock, pbuf, len);
            }
            break;
        }
        /* Process data */
        MQTT_ProcessUserMessage(&MQTTUserMsg);
      }
      else if (msg.qos == QOS2) /* For QOS2, a PUBREC response message is required */
      {
        /* Serialize ACK Messages (PUBREC) */
        len = MQTTSerialize_ack(pbuf, buflen, PUBREC, 0, MQTTUserMsg.packetid);

        if (len == 0)
        {
          ERR_PRINTF;
          return;
        }
        /* Send data */
        transport_sendPacketBuffer(MQTTSock, pbuf, len);
      }
      break;
    }
  case PUBREL:
    {
      /* Deserialize ACK Messages (PUBREL) */
      if ((MQTTDeserialize_ack(&msg.type, &msg.dup, &msg.id, pbuf, buflen) != 1) ||
          (msg.type != PUBREL) ||
            (msg.id != MQTTUserMsg.packetid))
      {
        ERR_PRINTF;
        return;
      }
      /* Receive PUBREL, process and drop data */
      if (MQTTUserMsg.valid == 1)
      {
        /* Process data */
        MQTT_ProcessUserMessage(&MQTTUserMsg);
      }

      /* Serialize PUBCOMP Messages */
      len = MQTTSerialize_pubcomp(pbuf, buflen, msg.id);

      if (len == 0)
      {
        ERR_PRINTF;
        return;
      }

      /* Send data */
      transport_sendPacketBuffer(MQTTSock, pbuf, len);
      break;
    }
  case PUBACK:   break;
  case PUBREC:   break;
  case PUBCOMP:  break;
  case PINGRESP: break;
  default:       break;
  }
}

/**************************************************************************************************
Function Name:
int32_t MQTT_Publish(int32_t sock, char pTopic, QoSType qos, uint8_t pMsg)

Input:
sock    - The MQTT socket file descriptor (currently unused; MQTTSock is used instead).
pTopic  - Pointer to the topic string to publish to.
qos     - Quality of Service level (QOS0, QOS1, or QOS2).
pMsg    - Pointer to the message payload to publish.

Output:
return  -  0 : Publish message sent successfully.
-1 : Failed to serialize publish packet.
-2 : Failed to send publish packet over TCP.

Comment:
This function publishes a message to the specified MQTT topic.

1. Sets retain flag to 1 so the broker retains the message even after a service reset.
2. Fills the MQTT topic using MQTTString format.
3. If QoS level is 1 or 2, a packet ID is generated from a static counter.
4. Serializes the publish packet using `MQTTSerialize_publish()`.
5. Sends the packet over the socket using `transport_sendPacketBuffer()`.

Notes:
- The sock parameter is passed but not used; the global MQTTSock is used instead.
To make the function fully parameterized, consider replacing MQTTSock with `sock`.
- The retain flag is hard-coded to 1; change to 0 if message retention is not desired.
- This function does not handle PUBACK (for QoS 1) or PUBREC/PUBREL/PUBCOMP (for QoS 2).
These should be handled externally if QoS > 0 is used.
**************************************************************************************************/
int32_t MQTT_Publish(int32_t sock, char *pTopic, QoSType qos, uint8_t* pMsg)
{
  //retain set to 1 for device will retain session if backend MQTT service reset
  int8_t retain = 1;
  uint8_t aBuf[MSG_MAX_LEN] = {0};
  int32_t len;
  uint16_t packID = 0;
  static uint16_t pubPacketID = 1;
  MQTTString TopicString = MQTTString_initializer;

  memset(aBuf, 0, sizeof(aBuf));

  /* Fill Topic */
  TopicString.cstring = (char *)pTopic;

  /* Fill Packet Identifier */
  if ((qos == QOS1) || (qos == QOS2))
  {
    packID = pubPacketID++;
  }
  else
  {
    packID = 0;
  }

  /* Serialize Publish message */
  len = MQTTSerialize_publish(aBuf, sizeof(aBuf), 0, qos, retain, packID, TopicString, (unsigned char*)pMsg, strlen((char *)pMsg));
  if (len <= 0)
  {
    /* Publish error */
    ERR_PRINTF;
    return -1;
  }

  /* Send Publish message */
  if(transport_sendPacketBuffer(MQTTSock, aBuf, len) < 0)
  {
    /* Send error */
    ERR_PRINTF;
    return -2;
  }

  return 0;
}

/**************************************************************************************************
Function Name:
int32_t MQTT_PingReq(int32_t sock)

Input:
sock    - The MQTT socket file descriptor (currently unused in implementation).

Output:
return  -  0 : PINGREQ message sent successfully.
-1 : Failed to send the PINGREQ packet.

Comment:
This function sends an MQTT PINGREQ packet to keep the connection alive.

1. Clears a local buffer aBuf for assembling the message.
2. Serializes the MQTT PINGREQ packet using `MQTTSerialize_pingreq()`.
3. Sends the packet using transport_sendPacketBuffer() with global `MQTTSock`.

Notes:
- The sock parameter is passed in but not used; the function uses global `MQTTSock`.
If socket parameterization is desired, replace MQTTSock with sock in the send call.
- This function only sends the request; response (PINGRESP) handling must occur elsewhere.
**************************************************************************************************/
int32_t MQTT_PingReq(int32_t sock)
{
  int32_t len;
  uint8_t aBuf[MSG_MAX_LEN] = {0};

  memset(aBuf, 0, sizeof(aBuf));

  /* Serialize PINGREQ Messages */
  len = MQTTSerialize_pingreq(aBuf, sizeof(aBuf));

  /* Send TCP data */
  if(transport_sendPacketBuffer(MQTTSock, aBuf, len) < 0)
  {
    /* Send error */
    ERR_PRINTF;
    return -1;
  }

  return 0;
}

/**************************************************************************************************
Function Name:
int32_t MQTT_SessionPingReq(int32_t sock)

Input:
sock    - The MQTT socket file descriptor.

Output:
return  -  0 : PINGREQ message sent successfully.
-1 : Failed to send the PINGREQ packet.

Comment:
This function sends an MQTT PINGREQ message to keep the connection alive.

1. Initializes a local buffer aBuf to zero.
2. Serializes a PINGREQ control packet using `MQTTSerialize_pingreq()`.
3. Sends the serialized packet through `transport_sendPacketBuffer()`.

Notes:
- This function does **not** wait for a PINGRESP message.
It assumes that a separate receive loop will handle the incoming PINGRESP.
- sock is passed in but not used directly (uses global MQTTSock instead).
- If the send fails, it returns -1 and triggers ERR_PRINTF (assumed to log error).
**************************************************************************************************/
int32_t MQTT_SessionPingReq(int32_t sock)
{
  int32_t len;
  uint8_t aBuf[MSG_MAX_LEN] = {0};

  memset(aBuf, 0, sizeof(aBuf));

  /* Serialize PINGREQ Messages */
  len = MQTTSerialize_pingreq(aBuf, sizeof(aBuf));

  /* Send TCP data */
  if(transport_sendPacketBuffer(MQTTSock, aBuf, len) < 0)
  {
    /* Send error */
    ERR_PRINTF;
    return -1;
  }

  return 0;
}

/**************************************************************************************************
Function Name:
int32_t MQTT_Subscribe(int32_t sock, char *pTopic, QoSType qosMax)

Input:
sock    - The MQTT socket file descriptor.
pTopic  - The topic string to subscribe to.
qosMax  - Maximum QoS (Quality of Service) level requested for this subscription.

Output:
return  -  0  : Subscription succeeded.
-1~-6: Error code indicating failure reason.
-1: Failed to send SUBSCRIBE packet.
-2: Timeout waiting for server response.
-3: No readable data detected on socket.
-4: Did not receive SUBACK packet.
-5: Failed to parse SUBACK packet.
-6: Invalid SUBACK content or mismatched packet ID.

Comment:
This function sends an MQTT SUBSCRIBE message and waits for the corresponding SUBACK
acknowledgement from the server. The process is as follows:

1. Initializes MQTTString for the topic to subscribe.
2. Serializes the SUBSCRIBE packet using `MQTTSerialize_subscribe()`, with:
- Packet ID auto-incremented via static PacketID
- One topic in the subscription list
3. Sends the serialized packet via `transport_sendPacketBuffer()`.
4. Uses select() with 2-second timeout to wait for server response.
5. If socket becomes readable, calls MQTTPacket_read() to parse the incoming packet.
6. Confirms it is a SUBACK response; then calls MQTTDeserialize_suback() to extract:
- Packet ID
- Granted QoS
7. Validates:
- Packet ID matches sent packet
- Granted QoS is not failure (0x80)

Note:
- PacketID is static and incremented on each call.
- ERR_PRINTF is assumed to log error details but is not defined in this function.
- QoS level is checked but not enforced beyond rejection.

**************************************************************************************************/
int32_t MQTT_Subscribe(int32_t sock, char *pTopic, QoSType qosMax)
{
  static uint32_t PacketID = 1;
  uint16_t packetidbk = 0;
  int32_t conutbk = 0;
  uint8_t aBuf[MSG_MAX_LEN] = {0};
  MQTTString TopicString = MQTTString_initializer;
  int32_t len;
  int32_t req_qos, qosbk;

  fd_set readfd;
  struct timeval tv;
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  FD_ZERO(&readfd);
  FD_SET(sock, &readfd);

  memset(aBuf, 0, sizeof(aBuf));

  /* Copy Topic */
  TopicString.cstring = (char *)pTopic;

  /* Maximum QoS request */
  req_qos = qosMax;

  /* Serialize Subscription Messages */
  len = MQTTSerialize_subscribe(aBuf, sizeof(aBuf), 0, PacketID++, 1, &TopicString, &req_qos);

  /* Send TCP data */
  if(transport_sendPacketBuffer(MQTTSock, aBuf, len) < 0)
  {
    /* Send error */
    ERR_PRINTF;
    return -1;
  }

  /* Wait for readable events */
  if(select(sock + 1, &readfd, NULL, NULL, &tv) <= 0)
  {
    /* Wait for timeout */
    ERR_PRINTF;
    return -2;
  }

  /* Check for readable events */
  if(FD_ISSET(sock, &readfd) == 0)
  {
    /* No readable event */
    ERR_PRINTF;
    return -3;
  }

  /* Wait for subscription return */
  if(MQTTPacket_read(aBuf, sizeof(aBuf), transport_getdata) != SUBACK)
  {
    /* No subscription return received */
    ERR_PRINTF;
    return -4;
  }

  /* Deserializes subscription response packets */
  if(MQTTDeserialize_suback(&packetidbk, 1, &conutbk, &qosbk, aBuf, sizeof(aBuf)) != 1)
  {
    /* Deserializes failure */
    ERR_PRINTF;
    return -5;
  }

  /* Check the correctness of returned data */
  if((qosbk == 0x80) || (packetidbk != (PacketID-1)))
  {
    /* The data is incorrect */
    ERR_PRINTF;
    return -6;
  }

  /* Subscription successful */
  return 0;
}

/**************************************************************************************************
Function Name:
char *create_control_message(ControlMsg_t *pData)

Input:
pData - Pointer to a ControlMsg_t structure containing the control message fields:
- channel_type: string representing channel type
- channel_addr: string representing channel address
- command     : integer command ID
- message     : string message content

Output:
return - Pointer to a heap-allocated JSON string representing the control message.
Returns NULL if pData is NULL or on memory allocation failure.

Comment:
This function converts a control message structure into a JSON-formatted string for MQTT
publishing. It performs the following steps:

1. Validates the input pointer `pData`.
2. Creates a root JSON object using `cJSON_CreateObject()`.
3. Adds the following fields to the JSON object:
- CH_T    : channel_type string
- CH_ADDR : channel_addr string
- CMD     : command integer
- CMD_MSG : message string
4. Converts the JSON object to a formatted string using `cJSON_Print()`.
5. Frees the original JSON object to avoid memory leaks.
6. Returns the generated JSON string, or NULL if any step fails.

Note:
- Caller is responsible for freeing the returned string using `free()`.
- Returns NULL if JSON creation or any field addition fails.
**************************************************************************************************/
char *create_control_message(ControlMsg_t *pData)
{
  if (pData == NULL)
  {
    return NULL;
  }

  char *string = NULL;
  cJSON *pRoot = cJSON_CreateObject();

  if (pRoot == NULL)
  {
    return NULL;
  }

  if (!cJSON_AddStringToObject(pRoot, CH_T, (char *)pData->channel_type) ||
      !cJSON_AddStringToObject(pRoot, CH_ADDR, (char *)pData->channel_addr) ||
        !cJSON_AddNumberToObject(pRoot, CMD, pData->command) ||
          !cJSON_AddStringToObject(pRoot, CMD_MSG, (char *)pData->message))
  {
    cJSON_Delete(pRoot);
    return NULL;
  }

  string = cJSON_Print(pRoot);
  if (string == NULL)
  {
    log_info("Failed to convert JSON to string.\n");
  }

  cJSON_Delete(pRoot);
  return string;
}

/**************************************************************************************************
Function Name:
char *create_availability(void)

Input:
None

Output:
return - Pointer to a heap-allocated JSON string indicating device availability status.
Returns NULL on failure.

Comment:
Creates a simple availability status message in JSON format, which is published to inform
the MQTT broker (and subscribers) that the device is online. The function performs the
following steps:

1. Creates a root JSON object using `cJSON_CreateObject()`.
2. Adds two string fields:
- MQTT_DEVICE_ID: the unique device ID (macro `DEVICE_ID`)
- "status": fixed value "online"
3. Converts the JSON object to a formatted string using `cJSON_Print()`.
4. Frees the original cJSON object to avoid memory leaks.
5. Returns the pointer to the resulting string.

Note:
- Caller is responsible for freeing the returned string using `free()`.
- Returns NULL if any field creation fails or if memory allocation fails.
**************************************************************************************************/
char *create_availability(void)
{
  cJSON *pRoot = NULL;
  char  *string = NULL;

  pRoot = cJSON_CreateObject();

  if (pRoot == NULL)
    return NULL;

  if (!cJSON_AddStringToObject(pRoot, "device_id", (char *)device_info.device_id) ||
      !cJSON_AddStringToObject(pRoot, "status", "online"))
  {
    cJSON_Delete(pRoot);
    return NULL;
  }

  string = cJSON_Print(pRoot);
  if (string == NULL)
  {
    log_info("Failed to create convert availability JSON to string.\n");
  }

  cJSON_Delete(pRoot);
  return string;
}

/**************************************************************************************************
Function Name:
char *create_discovery(void)

Input:
None

Output:
return - Pointer to a heap-allocated JSON string containing device discovery information.
Returns NULL on failure.

Comment:
Creates a JSON-formatted discovery message string to be sent via MQTT. The message contains
key information used for identifying the device on the server side. The function performs the
following steps:

1. Creates a root JSON object using `cJSON_CreateObject()`.
2. Adds key-value pairs for:
- Device ID (key: MQTT_DEVICE_ID)
- Serial Number (key: "SNO")
- Firmware Version (key: "Firmware")
- MAC Address (key: "MacAddr"), converted into string format (e.g., "00:1A:2B:3C:4D:5E")
3. Converts the JSON object to a formatted string using `cJSON_Print()`.
4. Deletes the original cJSON object to free memory.
5. Returns the pointer to the resulting JSON string.

Note:
- The returned string must be freed by the caller after use.
- If any JSON field creation fails, the function returns NULL and frees intermediate memory.
**************************************************************************************************/
char *create_discovery(void)
{
    cJSON *pRoot = NULL, *id = NULL, *sno = NULL, *fw_v = NULL, *macaddr = NULL;
    char  *string = NULL;

    pRoot = cJSON_CreateObject();
    if (pRoot == NULL)
    {
        log_info("Failed to create root JSON object.\n");
        return NULL;
    }

    // Device ID
    id = cJSON_CreateString((char *)device_info.device_id);
    if (id == NULL)
    {
        cJSON_Delete(pRoot);
        return NULL;
    }
    cJSON_AddItemToObject(pRoot, "device_id", id);

    // Serial Number
    sno = cJSON_CreateString((char *)device_info.SNO);
    if (sno == NULL)
    {
        cJSON_Delete(pRoot);
        return NULL;
    }
    cJSON_AddItemToObject(pRoot, "SNO", sno);

    // Firmware Version
    fw_v = cJSON_CreateString((char *)device_info.firmware_version);
    if (fw_v == NULL)
    {
        cJSON_Delete(pRoot);
        return NULL;
    }
    cJSON_AddItemToObject(pRoot, "Firmware", fw_v);

    // MAC Address
    char macStr[32];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             aMACAddr[0], aMACAddr[1], aMACAddr[2], aMACAddr[3], aMACAddr[4], aMACAddr[5]);

    macaddr = cJSON_CreateString(macStr);
    if (macaddr == NULL)
    {
        cJSON_Delete(pRoot);
        return NULL;
    }
    cJSON_AddItemToObject(pRoot, "MacAddr", macaddr);

    // Convert to string
    string = cJSON_Print(pRoot);
    if (string == NULL)
    {
        log_info("Failed to convert discovery JSON to string.\n");
    }

    cJSON_Delete(pRoot);
    return string;
}

/**************************************************************************************************
Function Name:
ConnectType MQTT_Connect(void)

Input:
None

Output:
return - MQTT_CONNECT_OK      : Authentication and connection successful
MQTT_CONNECT_NOK     : Authentication failed (server responded with error)
MQTT_CONNECT_NOTACK  : No CONNACK response received from the server

Comment:
Performs MQTT client authentication and connection with the broker. The steps include:
1. Initialize the MQTT connection parameters using `MQTTPacket_connectData`:
- Set client ID, username, password, MQTT version, keep-alive interval, clean session,
and Last Will message topic.
2. Serialize the connection packet using `MQTTSerialize_connect()`.
3. Send the serialized packet via `transport_sendPacketBuffer()`.
4. Wait for CONNACK response from the server using `MQTTPacket_read()`.
5. If response is received and return code is 0, connection is considered successful.
Otherwise, the function returns failure status.
**************************************************************************************************/
ConnectType MQTT_Connect(void)
{
  int len = 0;
  uint8_t aBuf[MSG_MAX_LEN];
  unsigned char sessionPresent, connack_rc;
  MQTTPacket_connectData sData = MQTTPacket_connectData_initializer;

  /* Set the sData */
  sData.clientID.cstring  = (char *)device_info.device_id;
  sData.keepAliveInterval = KEEPLIVE_TIME;
  sData.username.cstring  = USER_NAME;
  sData.password.cstring  = PASSWORD;
  sData.MQTTVersion       = MQTT_VERSION;
  sData.cleansession      = 1;
  sData.willFlag          = 1;
  sData.will.topicName.cstring = LWT_TOPIC;

  memset(aBuf, 0, sizeof(aBuf));

  /* Assemble message */
  len = MQTTSerialize_connect((unsigned char *)aBuf, sizeof(aBuf), &sData);

  /* Send message */
  transport_sendPacketBuffer(MQTTSock, aBuf, len);

  /* Wait for connection response */
  if (MQTTPacket_read(aBuf, sizeof(aBuf), transport_getdata) == CONNACK)
  {
    /* If the return code (connack_rc) of the CONNACK message is 0, the connection is successful */
    if ((MQTTDeserialize_connack(&sessionPresent, &connack_rc, aBuf, sizeof(aBuf)) != 1) || (connack_rc != 0))
    {
      log_info("Cannot connect, err code: %d\n", connack_rc);
      return MQTT_CONNECT_NOK;
    }
    else
    {
      log_info("Username and secret key authentication successful, MQTT connection successful!\n");
      return MQTT_CONNECT_OK;
    }
  }
  else
  {
    log_info("MQTT connection does not respond!\n");
    return MQTT_CONNECT_NOTACK;
  }
}

/**************************************************************************************************
Function Name:
void MQTT_ClientInit(void)

Input:
None

Output:
None

Comment:
Initializes the MQTT client connection and performs the following steps:
1. Sets the MQTT server IP address.
2. Repeatedly tries to establish a TCP connection to the MQTT broker using `transport_open()`.
3. Performs MQTT authentication via `MQTT_Connect()`.
4. Subscribes to a predefined list of MQTT topics (CONFIG, STATUS, CONTROL, SCENE, REBOOT).
- If any subscription fails, the connection is closed and the loop restarts.
5. Publishes an "availability" message to indicate device is online.
- If message creation or publishing fails, the connection is closed and retried.
6. Publishes a "discovery" message used for device auto-discovery (e.g., in Home Assistant).
- If message creation or publishing fails, the connection is closed and retried.
The function keeps retrying until all steps complete successfully.
**************************************************************************************************/

/**
*\*\name    MQTT_ClientInit.
*\*\fun     The MQTT client initialization, which consists of establishing a
*\*\        network connection, connecting to the MQTT server, and subscribing
*\*\        to messages.
*\*\param   none
*\*\return  none
**/
void MQTT_ClientInit(void)
{
    ip4_addr_t ServerAddr;
    char *p = NULL;
    uint8_t topic[MAX_TOPIC_LENGTH] =  { 0 };


#if 0
    /* Blocking domain name resolution */
    err_t Err = netconn_gethostbyname(HOST_NAME, &ServerAddr);
    if (Err != ERR_OK)
    {
        printf("netconn_gethostbyname err, err code: %d\n", Err);
    }
#else
    /* Set the server IP address */
    IP4_ADDR(&ServerAddr, HOST_IP_ADDR0, HOST_IP_ADDR1, HOST_IP_ADDR2, HOST_IP_ADDR3);
#endif


    while (1)
    {

         while (1)
        {
            /* Connect to server */
            MQTTSock = transport_open(ipaddr_ntoa(&ServerAddr), HOST_PORT);
            if (MQTTSock >= 0)
            {
                printf("Successful connection to the cloud platform server!\n");
                break;
            }
            else
            {
                printf("Failed to connect to the cloud platform server, wait 5 seconds and try to reconnect!\n");
                vTaskDelay(pdMS_TO_TICKS(2000));
            }
        }

        /* If connection fails, reconnect to the server */
        if (MQTT_Connect() != MQTT_CONNECT_OK)
        {
            printf("MQTT username and secret key authentication login failed!\n");
            /* Close the link */
            transport_close(MQTTSock);
            /* Reconnect to the server */
            continue;
        }

        /* device topic,config,status,control and scene*/
        snprintf((char *)topic,MAX_TOPIC_LENGTH,"%s%s%s",
                               SUB_BASE_TOPIC,device_info.device_id,CONFIG);

        /* If subscription fails, reconnect to the server */
        if (MQTT_Subscribe(MQTTSock, (char *)topic, QOS1) < 0)
        {
            printf("Client subscription message failed!\n");
            /* Close the link */
            transport_close(MQTTSock);
            /* Reconnect to the server */
            continue;
        }

        /*subcrib status topic*/
        memset(topic,0,MAX_TOPIC_LENGTH);
        snprintf((char *)topic,MAX_TOPIC_LENGTH ,"%s%s%s",
                               SUB_BASE_TOPIC,device_info.device_id,STATUS);

        if (MQTT_Subscribe(MQTTSock, (char *)topic, QOS1) < 0)
        {
            printf("Client subscription message failed!\n");
            /* Close the link */
            transport_close(MQTTSock);
            /* Reconnect to the server */
            continue;
        }

        /* subcrib control topic*/
        memset(topic,0,MAX_TOPIC_LENGTH);
        snprintf((char *)topic,MAX_TOPIC_LENGTH,"%s%s%s",
                               SUB_BASE_TOPIC,device_info.device_id,CONTROL);

        if (MQTT_Subscribe(MQTTSock, (char *)topic, QOS1) < 0)
        {
            printf("Client subscription message failed!\n");
            /* Close the link */
            transport_close(MQTTSock);
            /* Reconnect to the server */
            continue;
        }

        /*subscrib scene topic*/
        memset(topic,0,MAX_TOPIC_LENGTH);
        snprintf((char *)topic,MAX_TOPIC_LENGTH,"%s%s%s",
                               SUB_BASE_TOPIC,device_info.device_id,SCENE);
        if (MQTT_Subscribe(MQTTSock, (char *)topic, QOS1) < 0)
        {
            printf("Client subscription message failed!\n");
            /* Close the link */
            transport_close(MQTTSock);
            /* Reconnect to the server */
            continue;
        }

        /*subscrib reboot topic*/
        memset(topic,0,MAX_TOPIC_LENGTH);
        snprintf((char *)topic,MAX_TOPIC_LENGTH,"%s%s%s",
                               SUB_BASE_TOPIC,device_info.device_id,REBOOT);
        if (MQTT_Subscribe(MQTTSock, (char *)topic, QOS1) < 0)
        {
            printf("Client subscription message failed!\n");
            /* Close the link */
            transport_close(MQTTSock);
            /* Reconnect to the server */
            continue;
        }

        /*publish availability, if fail to send reconnect to the server*/
        p = create_availability();
        if ( p == NULL)
        {
            printf("Availability json failed!\n");
            /* Close the link */
            transport_close(MQTTSock);
            /* Reconnect to the server */
            continue;
        } else
        {
            /* Publish messages availability*/
            if (MQTT_Publish(MQTTSock, (char *)AVAILABILITY, QOS1, (uint8_t *)p) < 0)
            {
                printf("Device Availability fail!\n");
                /* Close the link */
                transport_close(MQTTSock);
                free(p);
                /* Reconnect to the server */
                continue;
            }
            else
            {
                free(p);
                p = NULL;
            }
        }

        /* Publish messages availability*/
        p = create_discovery();

        if( p == NULL)
        {
            printf("Discovery json fail!\n");
            /* Close the link */
            transport_close(MQTTSock);
             /* Reconnect to the server */
            continue;
        }
        else
        {
          /* Publish messages availability*/
          if (MQTT_Publish(MQTTSock, (char *)DISCOVERY, QOS1, (uint8_t *)p) < 0)
          {
              printf("MQTT_Publish fail!\n");
              /* Close the link */
              transport_close(MQTTSock);
              free(p);
              /* Reconnect to the server */
              continue;
          }
          else
          {
              free(p);
              p = NULL;
          }
        }
        break;
    }
}

/**************************************************************************************************
Function Name:
void mqtt_client_publish(void *arg)

Input:
*arg - Unused task argument pointer.

Output:
None

Comment:
This function implements the MQTT publish task in a FreeRTOS environment.
- Waits indefinitely (`portMAX_DELAY`) for messages from the MQTTResponseQueueHandle queue.
- Once a message is received, it is formatted using create_control_message() and
published to the corresponding MQTT topic using `MQTT_Publish()`.
- If the publish fails, it closes the MQTT connection via transport_close() and
attempts to reconnect using `MQTT_ClientInit()`.
- Uses noMQTTMsgExchange flag to indicate whether MQTT communication occurred in this cycle.
- Allocated memory for the message is freed after publishing.
**************************************************************************************************/
void mqtt_client_publish(void *arg)
{
    char         *p = NULL;
    BaseType_t   xReturn = pdTRUE;
    ControlMsg_t recvData;
    
    LWIP_UNUSED_ARG(arg);
    
    while (1) {
        /* Indicates no data exchange */
        noMQTTMsgExchange = 1;
        
        memset(&recvData,0,sizeof(ControlMsg_t));
        
        xReturn = xQueueReceive(MQTTResponseQueueHandle, /* Message queue handle */
                                &recvData,               /* Content of messages sent */
                                portMAX_DELAY);          /* Wait time in ms */
        if (xReturn == pdTRUE){
            /* Publish messages */
            p = create_control_message((void *)&recvData);
            
            if (MQTT_Publish(MQTTSock, (char *)recvData.response_topic, QOS1, (uint8_t *)p) < 0)
            {
                log_info("MQTT_Publish fail!\n");
                /* Close the link */
                transport_close(MQTTSock);
                /* Reconnect to the server */
                MQTT_ClientInit();
            }
            free(p);
            
            /* Indicates data exchange */
            noMQTTMsgExchange = 0;
        }
        
        vTaskDelay(pdMS_TO_TICKS(20));
    }

}


/**************************************************************************************************
Function Name:
void mqtt_client_receive(void *arg)

Input:
*arg - Unused task argument pointer.

Output:
None

Comment:
This function implements the MQTT receive task in a FreeRTOS environment.
- Uses select() and FD_SET to monitor the MQTT socket for incoming data.
- When data is available, it reads the MQTT packet using MQTTPacket_read() and
processes it with `MQTT_ProcessPacket()`.
- Maintains a tick-based timer to determine if a keep-alive (PINGREQ) packet needs to be sent.
- If no data exchange has occurred within the configured `PING_TIME`, it sends a PING request
using `MQTT_PingReq()`.
- If the PING fails, the MQTT connection is closed and re-initialized with `MQTT_ClientInit()`.
- Uses noMQTTMsgExchange flag to track whether a PING is needed.
**************************************************************************************************/
void mqtt_client_receive(void *arg)
{
    uint32_t curTick;
    uint8_t aBuf[MSG_MAX_LEN];
    int32_t type;
    fd_set readfd;
    struct timeval tv;
    
    /* 10 microseconds Timeout */
    tv.tv_sec = 0;
    tv.tv_usec = 10;
    
    LWIP_UNUSED_ARG(arg);
    
    
    /* Get the current tick as the heartbeat packet start time */
    curTick = xTaskGetTickCount();
    
    while (1)
    {
        memset(aBuf, 0, sizeof(aBuf));
        /* Indicates no data exchange */
        noMQTTMsgExchange = 1;
        
        FD_ZERO(&readfd);
        FD_SET(MQTTSock, &readfd);
        
        /* Wait for readable events */
        select(MQTTSock + 1, &readfd, NULL, NULL, &tv);
        
        /* Check if the MQTT server has data */
        if (FD_ISSET(MQTTSock, &readfd) != 0) {
            /* Read packet */
            type = MQTTPacket_read(aBuf, sizeof(aBuf), transport_getdata);
            if (type != -1) {
                /* Processing message data */
                MQTT_ProcessPacket(type, aBuf, sizeof(aBuf));
                /* Indicates data exchange */
                noMQTTMsgExchange = 0;

                /* Get the current tick as the heartbeat packet start time */
                curTick = xTaskGetTickCount();
            }
        }
        
        /* Send PING keep-alive commands to the server at regular intervals */
        if ((xTaskGetTickCount() - curTick) > (PING_TIME*1000)) {
            /* Get the current tick as the heartbeat packet start time */
            curTick = xTaskGetTickCount();
            
            /* Check for data exchange */
            if (noMQTTMsgExchange == 0) {
                /* There is data exchange, no need to send PING messages */
                continue;
            }
            
            /* Send MQTT heartbeat packets */
            if (MQTT_PingReq(MQTTSock) < 0){
                log_info("Send the keep-alive command failed!\n");
                /* Close the link */
                transport_close(MQTTSock);
                
                /* Reconnect to the server */
                MQTT_ClientInit();
            }
            
            /* Indicates data exchange */
            noMQTTMsgExchange = 0;
        }
       
       vTaskDelay(pdMS_TO_TICKS(10));
    }

}

/**************************************************************************************************
Function Name:
void mqtt_init(void)

Input:
None

Output:
None

Comment:
Initializes the MQTT-related resources and tasks.
- Prints function name for debugging.
- Creates MQTT command and response queues used for inter-task communication.
- Calls MQTT_ClientInit() to connect to the MQTT broker and subscribe to topics.
- Creates two FreeRTOS tasks:
- mqtt_client_publish: Responsible for publishing MQTT messages.
- mqtt_client_receive: Responsible for receiving MQTT messages.
Prints error messages if queue or task creation fails.
**************************************************************************************************/
void mqtt_init(void)
{
  log_info("\r\nCall %s\r\n", __FUNCTION__);

  /* Creates Command queue */
  MQTTCommandQueueHandle = xQueueCreate( /* Message queue length */
                                        (UBaseType_t )MESSAGE_QUEUE_LEN,
                                        /* Message size */
                                        (UBaseType_t )sizeof(ControlMsg_t));

  if (MQTTCommandQueueHandle == NULL) {
    log_info("\r\nFailure to create MQTT message queue!\n");
  }

  /* Creates Response queue */
  MQTTResponseQueueHandle = xQueueCreate( /* Message queue length */
                                         (UBaseType_t )MESSAGE_QUEUE_LEN,
                                         /* Message size */
                                         (UBaseType_t )sizeof(ControlMsg_t));

  if (MQTTResponseQueueHandle == NULL)
  {
    log_info("\r\nFailure to create MQTT message queue!\n");
  }

  /* Connect to server, subscribe messages */
  MQTT_ClientInit();

  /* Create MQTT Publish Task */
  if (xTaskCreate(mqtt_client_publish,
                  "mqtt_client_publish",
                  MQTT_TASK_STACK_SIZE,
                  NULL,
                  MQTT_CLIENT_PUBLISH_TASK_PRIORITY,
                  &MQTTPublishTaskHandle) != pdPASS) {
                    log_info("Task <<mqtt_client_publish>> creation failed!\n");
                  }

  /* Create MQTT Receive Task */
  if (xTaskCreate(mqtt_client_receive,
                  "mqtt_client_receive",
                  MQTT_TASK_STACK_SIZE,
                  NULL,
                  MQTT_CLIENT_RECEIVE_TASK_PRIORITY,
                  &MQTTReceiveTaskHandle) != pdPASS) {
                    log_info("Task <<mqtt_client_receive>> creation failed!\n");
                  }
}
