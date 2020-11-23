/* ************************************************************************** */
/** laser controller

  @Summary
    Brief description of the file.
 */
/* ************************************************************************** */

#include "laserDescriptor.h"
#include "laserControlSM.h"

/**
 * @param length Size of the array that must be sent
 */
void vModuleControlTask(void *pvParameter) {

    LaserCtrl_t *module = (LaserCtrl_t*) pvParameter;
    QueueHandle_t pxQueueReceiveLaserCtrlEvt = module->pxQueueReceiveLaserCtrl;

    LaserCtrlEvent_t receivedEvent;
    
    while (1) {
        
        xQueueReceive(pxQueueReceiveLaserCtrlEvt, (void *)&receivedEvent, portMAX_DELAY);
        
        switch (receivedEvent.eventType) {
            case CAN_MSG: // Message from CAN
                __builtin_software_breakpoint();
                break;
            case ALARM_EVENT: // Alarm event from distance measure task
                _nop();
                break;
            case CALIB_DONE: // Placeholder
                _nop();
                break;
            case ERROR_COMM: // Placeholder
                _nop();
                break;
            default:
                //error should not come here
                _nop();
                break;
        }
    }


}

/* *****************************************************************************
 End of File
 */
