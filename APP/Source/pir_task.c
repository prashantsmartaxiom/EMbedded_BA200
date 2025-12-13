#include "pir_task.h"

pir_sensor_t pir_sensors[MAX_PIR_SENSORS];
QueueHandle_t PIRMotionQueueHandle = NULL;
QueueHandle_t RelayMotionQueueHandle = NULL;

static Tpt29555aRegisterMap Tpt29555aRegister_Current;
static Tpt29555aRegisterMap Tpt29555aRegister_Previous;


/***********************************************************************************************
 * Function: pir_polling_task
 * Description:
 *   Periodically polls the PIR sensors via the TPT29555A I/O expander.
 *   Uses I2C communication to read input ports, updates current/previous
 *   register states, and detects changes (edges) for event handling.
 ***********************************************************************************************/

static void pir_polling_task(void *arg)
{
    memset(&Tpt29555aRegister_Previous, 0, sizeof(Tpt29555aRegister_Previous));
    memset(&Tpt29555aRegister_Current,  0, sizeof(Tpt29555aRegister_Current));

    while (1)
    {
   
        /* === Read I/O Expander ports using existing API ===*/
        IO_Expander_Communication_Process(&masterI2c4);

        // At this point, dataPtr[32] ? InputPort0, dataPtr[33] ? InputPort1
        // Update our current structure from those buffers
        Tpt29555aRegister_Current.InputPort0.all = *((uint8_t *)I2cMasterCommand.list[32].dataPtr);
        Tpt29555aRegister_Current.InputPort1.all = *((uint8_t *)I2cMasterCommand.list[33].dataPtr);

        uint8_t diff[2];
        diff[0] = Tpt29555aRegister_Current.InputPort0.all ^ Tpt29555aRegister_Previous.InputPort0.all;
        diff[1] = Tpt29555aRegister_Current.InputPort1.all ^ Tpt29555aRegister_Previous.InputPort1.all;

        // === Unified loop for PORT0 (0–7) and PORT1 (8–15) ===
        for (uint8_t idx = 0; idx < 16; idx++) {
            uint8_t port_num = idx / 8;   // 0 for sensors 0–7, 1 for 8–15
            uint8_t bit      = idx % 8;
            uint8_t motion_now;
            uint8_t changed;

            if (port_num == 0) {
                motion_now = (Tpt29555aRegister_Current.InputPort0.all >> bit) & 0x01;
                changed    = diff[0] & (1 << bit);
            } else {
                motion_now = (Tpt29555aRegister_Current.InputPort1.all >> bit) & 0x01;
                changed    = diff[1] & (1 << bit);
            }

            if (changed){
               app_evt_t e =  { 0 };
               e.dev_id = DEV_PIR;
               e.grp_id = 2;
               e.ts_ms = xTaskGetTickCount()*portTICK_PERIOD_MS;   
               
               // --- First detection ? configure ---
               if (!pir_sensors[idx].active && motion_now) {
                    pir_sensors[idx].active = true;
                    pir_sensors[idx].port = port_num+1;
                    e.id = EVT_PIR_DETECT;
                    e.u.pir.dev_id = DEV_PIR;
                    e.u.pir.ev.id = idx;
                    e.u.pir.ev.relay_status = motion_now;
                    e.u.pir.ev.port = port_num+1;
               } else {
                    e.id = motion_now ?EVT_PIR_MOTION : EVT_PIR_CLEAR;
                    e.u.pir.dev_id = DEV_PIR;
                    e.u.pir.ev.id = idx;
                    e.u.pir.ev.relay_status = motion_now;
                    e.u.pir.ev.port = port_num+1;
               }
               xQueueSend(q_events, &e,0); 
            }
        }
        
        // --- Update Previous snapshot ---
        Tpt29555aRegister_Previous = Tpt29555aRegister_Current; 
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}



void pir_init(void) 
{
       /*configure each PIR sensor */
     for ( int i = 0; i < MAX_PIR_SENSORS; i++) {
        pir_sensors[i].id                = (uint8_t)(i + 1);
        pir_sensors[i].port              = 1;
        pir_sensors[i].configured        = false;
        pir_sensors[i].active            = false;
     }
    
    xTaskCreate(pir_polling_task,
                "pir_polling_task", 
                PIR_TASK_STACK_SIZE, 
                NULL, 
                PIR_TASK_PRIORITY, 
                NULL);
    
}