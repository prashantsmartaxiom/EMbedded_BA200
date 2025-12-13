
#include "app_manager.h"

#define MAX_CH_GROUPS (8)
#define DEFAULT_HOLD_MS (300000UL)
#define DEFAULT_OCC_LEVEL  100

QueueHandle_t q_events;     //device -> app
QueueHandle_t q_cmds;        //app -> device  ( here led_cmd_t, shade_cmd_t , HMI_cmd_t etc)
QueueHandle_t q_shade_cmds;
static TaskHandle_t APPMGRTaskHandle = NULL;

typedef struct __attribute__((__packed__)) pir_groups {
    uint8_t grp_id;
    uint8_t sensor_idx[MAX_PIR_SENSORS];
    TimerHandle_t tmr ;
    TickType_t hold_ms;
    uint8_t active;
    bool motion_detected;
    bool last_send_status;
    uint8_t num_pir_sensors;
} pir_groups_t;

static pir_groups_t g_grp[MAX_CH_GROUPS];

uint8_t occupancy_state = 0;
static uint8_t numOfGrp = 0;

// Forward decls
//static void apply_scene_occupied(uint32_t grp_id);
//static void apply_scene_vacant(uint32_t grp_id);
static void vGroupHoldCb(TimerHandle_t xTmr);
//static inline uint32_t now_ms(void) {   return xTaskGetTickCount()*portTICK_PERIOD_MS; }

// === LED helpers (sends to q_cmds) ===
static void led_send_on(uint32_t led_dev,uint8_t led_ch)
{
    app_cmd_t c = { .dev_id=led_dev, .u.led.ch = led_ch, .u.led.cmd = LED_CH_ON };
    xQueueSend(q_cmds, &c, 0);
}
static void led_send_off(uint32_t led_dev,uint8_t led_ch)
{
  app_cmd_t c = { .dev_id= led_dev, .u.led.ch = led_ch, .u.led.cmd = LED_CH_OFF };
  xQueueSend(q_cmds, &c, 0);
}
static void led_send_level(uint32_t led_dev,uint8_t led_ch, uint8_t pct)
{
  app_cmd_t c = { .dev_id= led_dev, .u.led.ch = led_ch, .u.led.cmd = LED_CH_SET_BRIGHTNESS, .u.led.level= pct };
  xQueueSend(q_cmds, &c, 0);
}

static void led_send_pwr(uint32_t led_dev,uint8_t led_ch, uint16_t pwr_max,uint16_t pwr_min)
{
  app_cmd_t c = { .dev_id= led_dev, .u.led.ch = led_ch, .u.led.cmd = LED_CH_MAX_PWR,
  .u.led.pwr_max = pwr_max,.u.led.pwr_min = pwr_min };
  xQueueSend(q_cmds, &c, 0);
}

static void led_send_status(uint32_t led_dev,uint8_t led_ch)
{
  app_cmd_t c = { .dev_id= led_dev, .u.led.ch = led_ch, .u.led.cmd = LED_CH_STATUS };
  xQueueSend(q_cmds, &c, 0);
}

// === Shade helpers (sends to q_cmds) ===
static void shade_send_open(uint32_t shade_dev, uint8_t shade_port, uint8_t shade_idx)
{
  app_cmd_t c = { .dev_id= shade_dev,.u.shade.port = shade_port, .u.shade.shadeID = shade_idx, .u.shade.cmd = SHADE_CH_OPEN };
  xQueueSend(q_shade_cmds, &c, 0);
}

static void shade_send_close(uint32_t shade_dev, uint8_t shade_port,uint8_t idx)
{
  app_cmd_t c = { .dev_id= shade_dev, .u.shade.port = shade_port,.u.shade.shadeID = idx, .u.shade.cmd = SHADE_CH_DOWN };
  xQueueSend(q_shade_cmds, &c, 0);
}

 static void shade_send_stop(uint32_t shade_dev, uint8_t shade_port,uint8_t idx)
{
  app_cmd_t c = { .dev_id= shade_dev, .u.shade.port = shade_port,.u.shade.shadeID = idx,.u.shade.cmd = SHADE_CH_STOP};
  xQueueSend(q_shade_cmds, &c, 0);
}

 static void shade_set_limit(uint32_t shade_dev, uint8_t shade_port,uint8_t idx,uint8_t *msg)
{
  uint8_t ch_cmd = SHADE_CH_UPPER_LIMIT ;

  if (strncmp((char *)msg,"shade_upper_limit",17) == 0)
      ch_cmd = SHADE_CH_UPPER_LIMIT;
  else if (strncmp((char *)msg,"shade_lower_limit",17) == 0)
      ch_cmd = SHDAE_CH_LOWER_LIMIT;
  else if (strncmp((char *)msg,"shade_delete_limit",18) == 0)
      ch_cmd = SHADE_CH_DELETE_LIMIT;
  else if (strncmp((char *)msg,"shade_middle_limit",18) == 0)
      ch_cmd = SHADE_CH_MIDDLE_LIMIT;

  app_cmd_t c = { .dev_id= shade_dev, .u.shade.port = shade_port,.u.shade.shadeID = idx,.u.shade.cmd = ch_cmd};
  xQueueSend(q_shade_cmds, &c, 0);
}

 static void shade_move(uint32_t shade_dev, uint8_t shade_port,uint8_t idx,uint8_t *msg)
{
  uint8_t ch_cmd = SHADE_CH_MIDDLE_LIMIT_1 ;

  if (strncmp((char *)msg,"shade_move_limit_1",18) == 0)
      ch_cmd = SHADE_CH_MIDDLE_LIMIT_1;
  else if (strncmp((char *)msg,"shade_move_limit_2",18) == 0)
      ch_cmd = SHADE_CH_MIDDLE_LIMIT_2;
  else if (strncmp((char *)msg,"shade_move_limit_3",18) == 0)
      ch_cmd = SHADE_CH_MIDDLE_LIMIT_3;
  else if (strncmp((char *)msg,"shade_move_limit_4",18) == 0)
      ch_cmd = SHADE_CH_MIDDLE_LIMIT_4;

  app_cmd_t c = { .dev_id= shade_dev, .u.shade.port = shade_port,.u.shade.shadeID = idx,.u.shade.cmd = ch_cmd};
  xQueueSend(q_shade_cmds, &c, 0);
}

static void shade_id_setup(uint32_t shade_dev, uint8_t shade_port,uint8_t idx,uint8_t *msg)
{
  uint8_t ch_cmd = SHADE_CH_MIDDLE_LIMIT_1 ;

  if (strncmp((char *)msg,"shade_idsetup",13) == 0) {
     ch_cmd = SHADE_CH_ID_SELECT;
  }
  app_cmd_t c = { .dev_id= shade_dev, .u.shade.port = shade_port,.u.shade.shadeID = idx,.u.shade.cmd = ch_cmd};
  xQueueSend(q_shade_cmds, &c, 0);
}


void app_group_tmr_stop(uint32_t grp_id)
{
  if (grp_id>=MAX_PIR_GROUPS) return;
  xTimerStop(g_grp[grp_id].tmr, 0);
}


void app_set_group_period(uint32_t grp_id, uint32_t hold_ms)
{
  if (grp_id>=MAX_PIR_GROUPS) return;
  g_grp[grp_id].hold_ms = hold_ms ? hold_ms : PIR_5_TIMER_PERIOD;
  xTimerChangePeriod(g_grp[grp_id].tmr, g_grp[grp_id].hold_ms,0);
}


static void vGroupHoldCb(xTimerHandle xTmr)
{
  uint32_t grp_id = (uint32_t) pvTimerGetTimerID(xTmr);
  app_evt_t e =  {
    .grp_id = grp_id,
    .id = EVT_TIMER_GROUP_HOLD_EXPIRE,
    .ts_ms = xTaskGetTickCount()*portTICK_PERIOD_MS
  };

   //change group timer states
  if ( g_grp[grp_id].motion_detected ) {
      g_grp[grp_id].motion_detected = false;
      g_grp[grp_id].last_send_status = false;
  }
  xQueueSend(q_events,&e, 0);
}

static uint16_t GetChannel(uint8_t *nChannelAddr)
{
  int ch = 0x00;
  if (sscanf((char *)nChannelAddr, "%*[^0-9]%d", &ch) == 1)
     return ch;
  else
     return -1;
}

int numberOfvalidChannel(uint8_t *gr_addr)
{
  int count = 0;
  int numOfvalid_ch = 0;

  for(count=0;count<MAX_LED_CHANNEL;count++)
  {
    if(gr_addr[count] > 0) numOfvalid_ch +=1;
  }

  return numOfvalid_ch;
}

     // === Public API ===
void app_manager_init(void)
{
  q_events =xQueueCreate(32, sizeof(app_evt_t));
  q_cmds = xQueueCreate(16,  sizeof(app_cmd_t));
  q_shade_cmds = xQueueCreate(16,  sizeof(app_cmd_t));
  configASSERT(q_events);
  configASSERT(q_cmds);
  configASSERT(q_shade_cmds);
  vQueueAddToRegistry(q_events,"q_events");
  vQueueAddToRegistry(q_cmds, "q_cmds");
  vQueueAddToRegistry(q_shade_cmds, "q_shade_cmds");

  /* Create MQTT Publish Task */
  if (xTaskCreate(app_manager_task, "app_manager_task", APP_MGR_TASK_STACK_SIZE,
                  NULL, APP_MGR_TASK_PRIORITY, &APPMGRTaskHandle) != pdPASS) {
     log_info("Task <<APP Manager Task>> creation failed!\n");
  } else {
     /* Initialize MQTT task */
     mqtt_init();

     // Initialize led controller
     led_init();
     //Initialize shade controller
     shade_init();
     //intialize pir sensors
     pir_init();

     //initialize HMI panel
     Start_HMI_Scan_Task();
  }

  //intialze sensors groups
   memset(g_grp, 0, sizeof(g_grp));

   /*initialize groups for PIR sensors*/
   for( uint32_t i = 0; i < MAX_PIR_GROUPS; i++) {
        g_grp[i].grp_id = i+1;
        g_grp[i].motion_detected = false;
        g_grp[i].num_pir_sensors = 0;
        memset(g_grp[i].sensor_idx,0, sizeof(g_grp[i].sensor_idx));
        g_grp[i].last_send_status = false;
        g_grp[i].hold_ms = PIR_5_TIMER_PERIOD;
        g_grp[i].tmr = xTimerCreate("gh", g_grp[i].hold_ms, pdFALSE, (void*)i, vGroupHoldCb);
   }
}

void app_set_group_hold_ms(uint32_t grp_id, uint32_t hold_ms)
{
  if (grp_id>=MAX_CH_GROUPS) return;
  g_grp[grp_id].hold_ms = hold_ms ? hold_ms : DEFAULT_HOLD_MS;
  xTimerChangePeriod(g_grp[grp_id].tmr, pdMS_TO_TICKS(g_grp[grp_id].hold_ms),0);
}

static void ShadeControlCmd(int32_t id, ControlMsg_t *pControlMsg)
{
   int shade_port = 0x03;
   int shade_idx = 0x11;
   uint8_t *msg = pControlMsg->message;


  if (pControlMsg == NULL)
     return;

  //read shade port and shade_id from MQTT channel address
  if (sscanf((char *)pControlMsg->channel_addr, "port-%d_%d", &shade_port, &shade_idx) == 2) {
    log_info("shade port %d and idx %d\r\n",shade_port,shade_idx);
  } else {
        printf("Parsing failed or format is incorrect.\n");
  }

  //execute command
  switch(pControlMsg->command)
  {
    case SHADE_OPEN:
     shade_send_open(id,shade_port,shade_idx);
     break;
    case SHADE_CLOSE:
     shade_send_close(id,shade_port,shade_idx);
     break;
    case  SHADE_MQTT_STOP:
     shade_send_stop(id,shade_port,shade_idx);
     break;
    case SHADE_LIMIT:
     shade_set_limit(id,shade_port,shade_idx,msg);
     break;
    case SHADE_MOVE:
     shade_move(id,shade_port,shade_idx,msg);
     break;
    case SHADE_CONFIG:
     shade_id_setup(id,shade_port,shade_idx,msg);
     break;
    case SHADE_STATUS:
      break;
    default:
      log_info("unsupport control command %d\r\n ",pControlMsg->command);
  }

  vTaskDelay(pdMS_TO_TICKS(100));

}


static void LedControlCmd(uint32_t id,ControlMsg_t *pControlMsg)
{
  if (pControlMsg == NULL)
    return;

  uint16_t ch = GetChannel(pControlMsg->channel_addr);

  switch(pControlMsg->command)
  {
  case LED_ON_OFF:
    {
      /*LED ON or OFF*/
      if (strcmp((char *)pControlMsg->message, "LED_ON") == 0)
        led_send_on(id,ch);
      else if(strcmp((char *)pControlMsg->message, "LED_OFF") == 0)
        led_send_off(id,ch);
      break;
    }
  case LED_CH_BRIGHTNESS:
    led_send_level(id,ch,atoi((char *)pControlMsg->message));
    break;
  case LED_CONFIG:
    break;
  case LED_STATUS:
    led_send_status(id,ch);
    break;
  case LED_MAX_CUR:
    led_send_pwr(id,ch,pControlMsg->message[1],pControlMsg->message[0]);
    break;
  default:
    log_info("Unsupport Command\r\n");
  }

}

static void PirControlCommand(uint32_t id, ControlMsg_t *pControlMsg)
{
    BaseType_t xReturned;
    ControlMsg_t ControlMsg = {0};
    int gr_ch_nums = 0;
    int ticks = 0, grp_id = 0;

    if (pControlMsg == NULL)
        return;

    gr_ch_nums = numberOfvalidChannel(pControlMsg->gr_addr);

    switch (pControlMsg->command)
    {
    case PIR_CONFIG:
        break;

    case GRP_PIR_TIMER:
    {
        // 1. Parse the time information from message payload (e.g. "2:10")
        if (sscanf((char *)pControlMsg->message, "%d:%d", &grp_id, &ticks) != 2)
        {
            log_info("Invalid PIR TIMER payload: %s\r\n", pControlMsg->message);
            return;
        }

        // Default to 10 mins if ticks <= 0
        uint32_t hold_ticks = (ticks > 0) ? pdMS_TO_TICKS(ticks * 60 * 1000UL)
                                          : pdMS_TO_TICKS(10 * 60 * 1000UL);

        bool group_found_or_added = false;

            // 2. Try to find and update an existing group first (handles updates efficiently)
        for (int g_count = 0; g_count < MAX_PIR_GROUPS; g_count++) {
            if (g_grp[g_count].active && g_grp[g_count].grp_id == grp_id) {
               // Update existing group
              g_grp[g_count].hold_ms = hold_ticks;
              g_grp[g_count].num_pir_sensors = 0; // Reset count before adding
              g_grp[g_count].motion_detected = 0;
              g_grp[g_count].last_send_status = 0;

              for (int count = 0; count < gr_ch_nums; count++) {
                  uint8_t sensor_idx = pControlMsg->gr_addr[count];
                  if (sensor_idx -1 < MAX_PIR_SENSORS && g_grp[g_count].num_pir_sensors < MAX_SENSORS_PER_GROUP) {
                      g_grp[g_count].sensor_idx[g_grp[g_count].num_pir_sensors++] = sensor_idx-1;
                      pir_sensors[sensor_idx-1].configured = true;
                  }
              }
              app_group_tmr_stop(g_count);
              vTaskDelay(pdMS_TO_TICKS(2));
              app_set_group_period(g_count, g_grp[g_count].hold_ms);
              group_found_or_added = true;
              log_info("Updated group %d (ID %d)\r\n", g_count, grp_id);
              break; // Exit loop after finding the correct group
          }
        }

         // 3. If not found, try to add as a new group if space allows
        if (!group_found_or_added && numOfGrp < MAX_PIR_GROUPS) {
            int new_idx = numOfGrp; // Use the next available index

            g_grp[new_idx].grp_id = (uint8_t)grp_id;
            g_grp[new_idx].hold_ms = hold_ticks;
            g_grp[new_idx].num_pir_sensors = 0;
            g_grp[new_idx].motion_detected = 0;
            g_grp[new_idx].last_send_status = 0;

            for (int count = 0; count < gr_ch_nums; count++) {
                uint8_t sensor_idx = pControlMsg->gr_addr[count];
                if (sensor_idx - 1 < MAX_PIR_SENSORS && g_grp[new_idx].num_pir_sensors < MAX_SENSORS_PER_GROUP) {
                    g_grp[new_idx].sensor_idx[g_grp[new_idx].num_pir_sensors++] = sensor_idx-1;
                    pir_sensors[sensor_idx-1].configured = true;
                }
            }

            app_group_tmr_stop(new_idx);
            vTaskDelay(pdMS_TO_TICKS(2));
            app_set_group_period(new_idx, g_grp[new_idx].hold_ms);
            g_grp[new_idx].active = true;
            numOfGrp++;
            group_found_or_added = true;
            log_info("Adding new group %d (ID %d)\r\n", new_idx, grp_id);
          }

       // 4. Send response
        if (group_found_or_added)
            sprintf((char *)pControlMsg->message, "%d:CMD SUCCESS", grp_id);
        else
            sprintf((char *)pControlMsg->message, "%d:Error: MAX_GROUPS reached or invalid ID", grp_id);

        sprintf((char *)ControlMsg.response_topic, "%s%s%s", PUB_BASE_TOPIC, device_info.device_id, STATUS);
        ControlMsg.command = GRP_PIR_TIMER;
        break;
    }

    default:
        break;
    }

    strncpy((char *)ControlMsg.channel_type, "PIR", sizeof(ControlMsg.channel_type) - 1);
    ControlMsg.channel_type[sizeof(ControlMsg.channel_type) - 1] = '\0';
    xReturned = xQueueSend(MQTTResponseQueueHandle, &ControlMsg, 0);

    if (xReturned == pdPASS)
        log_info("Publishing Command to Queue for processing: %s\n", ControlMsg.message);
}



static void LedSceneCmd(uint32_t id, ControlMsg_t *pMsg)
{
  BaseType_t xReturned;
  int dimming_value = 0;
  ControlMsg_t send_control_msg =  { 0 };
  uint16_t gr_ch_nums = numberOfvalidChannel(pMsg->gr_addr);

  for(int i = 0 ; i < gr_ch_nums ;i++)
  {
    if ( pMsg->gr_addr[i] > 0) {

      if (strcmp((char *)pMsg->message, "LED_ON") == 0) {
        led_send_on(id,pMsg->gr_addr[i]);
      }
      else if(strcmp((char *)pMsg->message, "LED_OFF") == 0) {
        led_send_off(id,pMsg->gr_addr[i]);
      }
      else {
        if (strncmp((char *)pMsg->message,"LED_BRIGHTNESS",strlen("LED_BRIGHTNESS")) == 0 ) {
          sscanf((char *)pMsg->message,"%*[^0-9]%d",&dimming_value);
        }
        led_send_level(id,pMsg->gr_addr[i],dimming_value);
      }
      send_control_msg.gr_addr[i] = pMsg->gr_addr[i];
    }
  }

  sprintf((char *)send_control_msg.response_topic,"%s%s%s",PUB_BASE_TOPIC,device_info.device_id,STATUS);
  send_control_msg.command = SCENE_CMD;
  strncpy((char *)send_control_msg.channel_type, "LED",3);
  sprintf((char *)send_control_msg.message,"%s", "SENSE_CMD_SUCCESS");

  xReturned = xQueueSend(MQTTResponseQueueHandle, /* Message Queue Handle */
                         (void *) &send_control_msg,         /* Content of messages sent */
                         0);                  /* Wait time in ms */
  if ( xReturned == ERROR) {
    log_info("Unable to publish message %s\n",send_control_msg.message);
  }
}


static void app_mqtt_control_cmd(uint32_t id, ControlMsg_t *pMsg)
{
    /* Publish messages  */
    switch (id)
    {
      case DEV_LED:
      {
        if(pMsg->command == SCENE_CMD)
           LedSceneCmd(id,pMsg);
        else
           LedControlCmd(id,pMsg);
      }
      break;
      case DEV_SHADE:
        ShadeControlCmd(id,pMsg);
        break;
      case DEV_HMI :
        break;
      case DEV_PIR:
        PirControlCommand(id,pMsg);
        break;
      default:
        log_info("\r\n none of the supported devivces\r\n");
    }
}

static BaseType_t publish_pir_event(relay_event_t ev)
{
    ControlMsg_t ControlMsg =  { 0 } ;
    BaseType_t xReturned;
    char pMsg[50] = {0};

    sprintf((char *)ControlMsg.response_topic,"%s%s%s",PUB_BASE_TOPIC,device_info.device_id,STATUS);
    ControlMsg.command = PIR_STATE;
    snprintf((char *)ControlMsg.channel_addr,sizeof(ControlMsg.channel_addr),"Port-%d_%d",ev.port,ev.id + 1);

    //Connected sensor event on a sepcific port
    snprintf(pMsg,sizeof(pMsg),"PIR State = %d",ev.relay_status);
    strncpy((char*)ControlMsg.message,pMsg,strlen(pMsg));
    snprintf((char *)ControlMsg.channel_type, sizeof(ControlMsg.channel_type), "PIR");

    xReturned = xQueueSend(MQTTResponseQueueHandle, /* Message Queue Handle */
                           (void *) &ControlMsg,         /* Content of messages sent */
                           0);                  /* Wait time in ms */

    if ( xReturned == SUCCESS) {
          log_info("Publishing Command to Queue for processing \n");
    }

    return xReturned;
}

static void publish_pir_hold_expire(const app_evt_t *evt)
{
     ControlMsg_t ControlMsg = {0};
     char pMsg[50] = {0};
     uint8_t sensor_idx;

     sprintf((char *)ControlMsg.response_topic,"%s%s%s",PUB_BASE_TOPIC,device_info.device_id,STATUS);
     ControlMsg.command = PIR_STATE;

     //connet port
     sensor_idx = g_grp[evt->grp_id].sensor_idx[0];

     //connected sensor id
     snprintf((char *)ControlMsg.channel_addr,sizeof(ControlMsg.channel_addr),"Port-%d_%d",pir_sensors[sensor_idx].port,sensor_idx + 1);

     //Connected sensor event on a sepcific port
     snprintf(pMsg,sizeof(pMsg),"PIR State = %d",g_grp[evt->grp_id].motion_detected);
     strncpy((char*)ControlMsg.message,pMsg,strlen(pMsg));
     snprintf((char *)ControlMsg.channel_type, sizeof(ControlMsg.channel_type), "PIR");

     //send message to MQTT client
     xQueueSend(MQTTResponseQueueHandle, (void *) &ControlMsg, 0);
}

static uint8_t sensor_lookup(uint8_t sensor_idx)
{
     uint8_t g_count = 0;
     bool bfound = false;

     if ( sensor_idx > MAX_PIR_SENSORS)
       return MAX_PIR_GROUPS;

      while(!bfound) {
        for ( ; g_count < MAX_PIR_GROUPS ; g_count++)  {
           if(g_grp[g_count].active) {
             for (int i = 0 ; i < g_grp[g_count].num_pir_sensors; i++) {
               if (g_grp[g_count].sensor_idx[i] ==  sensor_idx) {
                    bfound =true;
                    return g_count;
               }
             }
           } else {
              if ( g_count + 1 == MAX_PIR_GROUPS) bfound = true;
           }
        }
      }
      return g_count ;
}


static void pir_motion_event(relay_event_t ev)
{
   uint8_t grp_idx = sensor_lookup(ev.id );

   if(grp_idx < MAX_CH_GROUPS) {

      if(pir_sensors[ev.id].configured && g_grp[grp_idx].active) {
          //Handle state transitions
         int motion_now = ev.relay_status > 0 ? 1 : 0;
         uint32_t count = ev.id;

         //compare previous state of group
         if (motion_now != g_grp[grp_idx].motion_detected) {
          pir_sensors[count].port = ev.port;

          //check pervious send state
          if (motion_now != g_grp[grp_idx].last_send_status) {

             //update group state motion
             g_grp[grp_idx].motion_detected = motion_now;

             if (publish_pir_event(ev) == SUCCESS){
                 g_grp[grp_idx].last_send_status = g_grp[grp_idx].motion_detected;
             }

             xTimerStart(g_grp[grp_idx].tmr, 0);
             log_info("motion on a group %d for io sensors %d \r\n",grp_idx,ev.id+1 );

           }
         } else {
            // 1 ? 0: Restart timer (will publish acd_arp_reply( 1 minute)
            xTimerReset(g_grp[grp_idx].tmr,0);
            log_info("timer reset on a group %d for io sensors %d\r\n",grp_idx,ev.id+1);

        }
      }
    }
}

// === Event handler ===
static void app_on_event(const app_evt_t *evt)
{
  switch (evt->id) {
  case EVT_PIR_DETECT:
       publish_pir_event(evt->u.pir.ev);
       break;
  case EVT_PIR_MOTION:
       pir_motion_event(evt->u.pir.ev);
       break;
  case EVT_PIR_CLEAR:
       break;
  case EVT_TIMER_GROUP_HOLD_EXPIRE:
       publish_pir_hold_expire(evt);
       break;
  case EVT_HMI_LED_BUTTON: break;
  case EVT_HMI_DIM_SLIDER: break;
  case EVT_HMI_SHADE_UP_BUTTON: break;
  case EVT_HMI_SHADE_DOWN_BUTTON: break;
  case EVT_HMI_SHADE_STOP_BUTTON: break;
  case EVT_SCHEDULE_TICK: break;
  case EVT_MQTT_CMD:
       app_mqtt_control_cmd(evt->u.mqtt.dev_id,(ControlMsg_t *)&evt->u.mqtt.pControMsg);
       break;
  case EVT_MODBUS_INPUT_CHANGE: break;
  default: break;
  }
}

void app_manager_task(void *arg)
{
  app_evt_t e;
  for (;;) {
    if (xQueueReceive(q_events, &e, portMAX_DELAY) == pdTRUE) {
      app_on_event(&e);
    }
  }
}