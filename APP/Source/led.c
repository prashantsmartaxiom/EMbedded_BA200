#include "main.h"
#include "led.h"

uint16_t refLimit[MAX_LED_CHANNEL] =  { 0 } ;

static void led_set_dimming(uint8_t nChannelAddr, uint16_t ledBrightness);
static uint16_t led_get_dimming(uint8_t nChannelAddr);
static uint8_t led_get_status(uint8_t nChannelAddr);

static TaskHandle_t LEDTaskHandle = NULL;


static void led_respond_msg(uint8_t pChAddr,uint8_t ledOnOffSwitch, uint8_t level,command_t cmd)
{
    ControlMsg_t ControlMsg =  { 0 } ;
    BaseType_t xReturned;

    //TODO: prepare respose to broker
    sprintf((char *)ControlMsg.channel_addr,"LED%d",pChAddr);
    strncpy((char *)ControlMsg.channel_type, "LED",3);

    sprintf((char *)ControlMsg.response_topic,"%s%s%s",PUB_BASE_TOPIC,device_info.device_id,STATUS);
    ControlMsg.command = cmd;
    sprintf((char *)ControlMsg.message,"%s", "CMD SUCCESS");

    xReturned = xQueueSend(MQTTResponseQueueHandle, /* Message Queue Handle */
                      (void *) &ControlMsg,         /* Content of messages sent */
                       0);                  /* Wait time in ms */

    if ( xReturned == SUCCESS) {
        //  log_info("Publishing Command to Queue for processing \n");
    }

    return;
 }

/**
* @brief  Set PWM Dimming Power (12-bit control)
* @param  nChannelAddr : LED channel address
* @param  pwmPower     : 12-bit power level (0x000 to 0xFFF)
* @retval None
*/
void led_set_pwm_pwr(uint8_t nChannelAddr, uint16_t pwmPower)
{
  // Clamp value within 12-bit range
  if (pwmPower > 0x0FFF)
    pwmPower = 0x0FFF;

  // Prepare the data structure for PWM Dimming register
  PwmDimming1Register *pwmReg = (PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr;
  pwmReg->all = pwmPower;

  // Send SPI command to write PWM dimming value
  Mpq7210_Command_Pack_Enqueue(PWM_DIM1, nChannelAddr, DATA_WRITE, HIGH_PRIORITY);

}


void led_hal_on_off(uint8_t nChannelAddr, uint8_t ledOnOffSwitch)
{
    GuiCommandInfo *cmdInfo;
    LedStatusRegister *ledStatus;

    // Validate LED index (1..22)
    if ((nChannelAddr == 0) || (nChannelAddr > MAX_LED_CHANNEL))
        return;

    // Each LED has 2 entries in guiCommand.index: STATUS = even, CONTROL = odd
    uint8_t statusIndex = (nChannelAddr - 1) * 2;

    // Get pointer to the STATUS entry
    cmdInfo = *(guiCommand.index + statusIndex);

    if (cmdInfo == NULL || cmdInfo->dataPtr == NULL)
        return;

    // Cast data pointer to LedStatusRegister
    uint16_t br_level = led_get_dimming(nChannelAddr);
    ledStatus = (LedStatusRegister *)cmdInfo->dataPtr;

    // Get pointer to Analog Dimming register (8-bit)
    PwmDimming1Register *PwmReg =
        (PwmDimming1Register *)(*(mpq7210Command.index + PWM_DIM1))->dataPtr;

    // Get pointer to Analog Dimming register (8-bit)
//    AnalogDimming1Register *anaReg =
//        (AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr;

    // Update LED ON/OFF bit in structure
    if ( ledStatus->bit.LED_STATUS != ledOnOffSwitch) {

       ledStatus->bit.LED_STATUS = (ledOnOffSwitch ? LED_ON : LED_OFF);

       //update Mpq7210 registers for brightness and status bit
       if (ledStatus->bit.LED_STATUS == LED_ON) {

         PwmReg->bit.DIM1_DUTY = 0xFFF;
         Mpq7210_Command_Pack_Enqueue(PWM_DIM1, (nChannelAddr), DATA_WRITE, HIGH_PRIORITY);
        } else {
         PwmReg->bit.DIM1_DUTY  = 0x000;
         Mpq7210_Command_Pack_Enqueue(PWM_DIM1, (nChannelAddr), DATA_WRITE, HIGH_PRIORITY);
        }
    }
    //respond to Broker after change LED status ON/OFF
   // led_respond_msg(nChannelAddr,ledStatus->bit.LED_STATUS,br_level,LED_ON_OFF);

}


/*Set Current level or DIMMING*/
static void led_set_dimming(uint8_t nChannelAddr, uint16_t ledBrightness)
{
  GuiCommandInfo *cmdInfo;
  LedControlRegister *ledControl;

  // Validate LED index (1..12)
  if ((nChannelAddr == 0) || (nChannelAddr > MAX_LED_CHANNEL))
    return ;

  // Each LED has 2 entries: STATUS = even, CONTROL = odd
  uint8_t controlIndex = ((nChannelAddr - 1) * 2) + 1;

  // Get pointer to the CONTROL entry
  cmdInfo = *(guiCommand.index + controlIndex);

  if (cmdInfo == NULL || cmdInfo->dataPtr == NULL)
    return ;

  // Cast data pointer to LedControlRegister
  ledControl = (LedControlRegister *)cmdInfo->dataPtr;

  // If brightness is less than the reference limit, use it; otherwise, keep the requested value
  if (ledBrightness > MIN_CURRENT_LIMIT)
  {
      if ( ledBrightness > refLimit[nChannelAddr-1])
      {
          ledBrightness  = refLimit[nChannelAddr-1];
      }
  }
  else
  {
      ledBrightness = MIN_CURRENT_LIMIT;
  }

  //update LedRegisters
  ledControl->bit.BRIGHTNESS =  (ledBrightness/10) & 0xFF;

  // Get pointer to Analog Dimming register (8-bit)
  AnalogDimming1Register *anaReg =
    (AnalogDimming1Register *)(*(mpq7210Command.index + ANA_DIM1))->dataPtr;

  // Write brightness to 8-bit register
  anaReg->all = ledBrightness  & 0xFF;

  // Enqueue SPI write command
  Mpq7210_Command_Pack_Enqueue(ANA_DIM1, nChannelAddr, DATA_WRITE, HIGH_PRIORITY);

  // Send updated status to GUI or control system
  uint8_t led_status = led_get_status(nChannelAddr);
  led_respond_msg(nChannelAddr, led_status, ledBrightness, LED_CH_BRIGHTNESS);

}


/*-------------------------------------------------------------
* @brief   Get Analog Dimming (Brightness Level) via SPI
*-------------------------------------------------------------*/
static uint16_t led_get_dimming(uint8_t nChannelAddr)
{
  GuiCommandInfo *cmdInfo;
  LedControlRegister *ledControl;

  // Validate LED index (1..12)
  if ((nChannelAddr == 0) || (nChannelAddr > MAX_LED_CHANNEL))
    return 0;

  // Each LED has 2 entries: STATUS = even, CONTROL = odd
  uint8_t controlIndex = ((nChannelAddr - 1) * 2) + 1;

  // Get pointer to the CONTROL entry
  cmdInfo = *(guiCommand.index + controlIndex);
  if (cmdInfo == NULL || cmdInfo->dataPtr == NULL)
    return 0;

  // Cast data pointer to LedControlRegister
  ledControl = (LedControlRegister *)cmdInfo->dataPtr;

  // Return brightness value (scaled if needed)
  return ledControl->bit.BRIGHTNESS*10;
}

/*-------------------------------------------------------------
* @brief   Get LED ON/OFF status via SPI
*-------------------------------------------------------------*/
static uint8_t led_get_status(uint8_t nChannelAddr)
{
  LedStatusRegister *ledStatus;
  uint8_t status = LED_OFF;
  GuiCommandInfo *cmdInfo;

  //nChannel address
  if ((nChannelAddr == 0 ) || nChannelAddr > MAX_LED_CHANNEL)
    return LED_OFF;

  // Each LED has 2 entries in guiCommand.index: STATUS = even, CONTROL = odd
  uint8_t statusIndex = (nChannelAddr - 1) * 2;


  // Get pointer to the STATUS entry
  cmdInfo = *(guiCommand.index + statusIndex);

  if (cmdInfo == NULL || cmdInfo->dataPtr == NULL)
    return status;

  // Cast data pointer to LedStatusRegister
  ledStatus = (LedStatusRegister *)cmdInfo->dataPtr;


  // Decode ON/OFF bit
  if (ledStatus->bit.LED_STATUS == LED_ON)
    status = LED_ON;

  return status;
}

static void led_channel_status(uint8_t nChannelAddr)
{
  uint8_t status = led_get_status(nChannelAddr);
  uint16_t level = led_get_dimming(nChannelAddr);
  led_respond_msg(nChannelAddr,status,level,LED_STATUS);
}

/*-------------------------------------------------------------
* @brief   Set LED current range via SPI
*-------------------------------------------------------------*/
static void led_set_cur_range(uint8_t nChannelAddr, uint16_t ledCurrentLimit, uint16_t ledCurrentMin)
{


  if (ledCurrentLimit < MAX_CURRENT_LIMIT) {
    refLimit[nChannelAddr-1] = ledCurrentLimit;
  }
  else {
    refLimit[nChannelAddr-1] = MAX_CURRENT_LIMIT;
  }

  //send confirmation to broker
  uint8_t led_status = led_get_status(nChannelAddr);
  led_respond_msg(nChannelAddr,led_status,refLimit[nChannelAddr-1],LED_MAX_CUR);
}


/*Get Current limit*/
//static uint16_t led_get_cur_limit(uint8_t nChannelAddr)
//{
//  uint16_t currentLimit = 0;
//
//  // Optional: store in local reference array if needed
//  currentLimit  =   refLimit[nChannelAddr -1] ;
//
//  return currentLimit;
//}


void led_task(void *arg)
{
    for (;;)
    {
        app_cmd_t c = { 0 };
        if (xQueueReceive(q_cmds, &c, portMAX_DELAY) == pdTRUE)
        {
            if (c.dev_id != DEV_LED) continue; // not ours
            switch (c.u.led.cmd)
            {
                case LED_CH_ON:
                    led_hal_on_off(c.u.led.ch,c.u.led.cmd);
                    break;
                case LED_CH_OFF:
                    led_hal_on_off(c.u.led.ch,c.u.led.cmd);
                    break;
                case LED_CH_SET_BRIGHTNESS:
                    led_set_dimming(c.u.led.ch,c.u.led.level);
                    break;
                case LED_CH_MAX_PWR:
                    led_set_cur_range(c.u.led.ch,c.u.led.pwr_max,c.u.led.pwr_min);
                    break;
                case LED_CH_STATUS:
                    led_channel_status(c.u.led.ch);
                    break;
                default:
                    break;
            }

        }
    }
}


void led_init()
{
  /*Default power limit of LED channels*/
  for ( int count = 0 ; count < MAX_LED_CHANNEL ; count++)
    refLimit[count] = PWR_DEFAULT_LIMIT;


  /* Create MQTT Publish Task */
  if (xTaskCreate(led_task,
                  "led_task",
                  LED_TASK_STACK_SIZE,
                  NULL,
                  LED_TASK_PRIORITY,
                  &LEDTaskHandle) != pdPASS) {
                    log_info("Task <<Led Task>> creation failed!\n");
                  }

}

