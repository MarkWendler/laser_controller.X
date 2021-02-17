/* ************************************************************************** */
/** laser controller

  @Summary
    Brief description of the file.
 */
/* ************************************************************************** */

#include "laserControlSM.h"
#include "canComm.h"// To process CAN data
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "laserCommSM.h"
#include "laserDistanceMeasure.h"
/**
 * @param length Size of the array that must be sent
 */
void vModuleControlTask(void *pvParameter) {

    LaserCtrl_t *module = (LaserCtrl_t*) pvParameter;
    QueueHandle_t pxQueueReceiveLaserCtrlEvt = module->pxQueueReceiveLaserCtrl;
    LaserCtrlEvent_t receivedEvent;
    
    LaserCommEvent_t eventToComm;
    
    DistanceEventType_t eventToDistance;
    
    ToCANEvent_t toCANEvent;
    toCANEvent.id = module->ID;
    
    while (1) {
        
        xQueueReceive(pxQueueReceiveLaserCtrlEvt, (void *)&receivedEvent, portMAX_DELAY);
        
        switch (receivedEvent.eventType) {
            case CAN_MSG: // Message from CAN
                switch((CanData_t)receivedEvent.data){
                    case CAN_DATA_TURN_ON: //TURN ON LASER
                        
                        eventToDistance.eventType = DISTANCE_ACTIVATE_ALARM;
                        if (xQueueSend(module->pxQueueToDistance, (void *) &eventToDistance, 5) != pdPASS) {
                            //TODO handle Q full 
                        }                         
                        eventToComm = TURN_ON;
                        if (xQueueSend(module->pxQueueToComm, (void *) &eventToComm, 20) != pdPASS) {
                            //TODO handle Q full 
                        }  
                        break;
                    case CAN_DATA_TURN_OFF: //TUN OFF Laser
                        eventToDistance.eventType = DISTANCE_DEACTIVATE_ALARM;
                        if (xQueueSend(module->pxQueueToDistance, (void *) &eventToDistance, 5) != pdPASS) {
                            //TODO handle Q full 
                        }                          
                        eventToComm = TURN_OFF;
                        if (xQueueSend(module->pxQueueToComm, (void *) &eventToComm, 20) != pdPASS) {
                            //TODO handle Q full 
                        }  
                        break;
                    case CAN_DATA_CALIBRATE:
                        eventToDistance.eventType = DISTANCE_CALIBRATE;
                        if (xQueueSend(module->pxQueueToDistance, (void *) &eventToDistance, 5) != pdPASS) {
                            //TODO handle Q full 
                        }                          
                        
                        eventToComm = TURN_ON;
                        if (xQueueSend(module->pxQueueToComm, (void *) &eventToComm, 5) != pdPASS) {
                            //TODO handle Q full 
                        }                          
                        //After 150ms the calibration should be done
                        if(xQueueReceive(pxQueueReceiveLaserCtrlEvt, (void *)&receivedEvent, pdMS_TO_TICKS(150)) != pdPASS){
                            //Calib failed
                            toCANEvent.eventType = ERROR_CALIBFAILED_TO_CAN;
                            if( xQueueSend(module->pxQueueToCAN, (void *) &toCANEvent,  1) ){
                                //TODO: failed to notify upper layer
                            }                            
                        }
                        if(receivedEvent.eventType != CALIB_DONE){
                            toCANEvent.eventType = ERROR_CALIBFAILED_TO_CAN;
                            if( xQueueSend(module->pxQueueToCAN, (void *) &toCANEvent,  1) ){
                                //TODO: failed to notify upper layer
                            }                              
                        }
                        eventToComm = TURN_OFF;
                        if (xQueueSend(module->pxQueueToComm, (void *) &eventToComm, 5) != pdPASS) {
                            //TODO handle Q full 
                        }                               
                        break;
                }
                     
                
                break;
            case ALARM_EVENT: // Alarm event from distance measure task
                toCANEvent.eventType = ALARM_TO_CAN;
                if( xQueueSend(module->pxQueueToCAN, (void *) &toCANEvent,  1) ){
                    //TODO: failed to notify upper layer
                    // TODO: Lost communication, Switch off??
                }
                eventToDistance.eventType = DISTANCE_DEACTIVATE_ALARM;
                if (xQueueSend(module->pxQueueToDistance, (void *) &eventToDistance, 5) != pdPASS) {
                    //TODO handle Q full 
                }                  
                eventToComm = TURN_OFF;
                if (xQueueSend(module->pxQueueToComm, (void *) &eventToComm, 5) != pdPASS) {
                    //TODO handle Q full 
                }                   
                break;
            case CALIB_DONE: // Placeholder
                _nop();
                break;
            case ERROR_COMM: // Placeholder
                _nop();
                break;
            case ERROR_NO_CALIB: // Placeholder
                eventToComm = TURN_OFF;
                if (xQueueSend(module->pxQueueToComm, (void *) &eventToComm, 20) != pdPASS) {
                //TODO handle Q full 
                }                  
                toCANEvent.eventType = ERROR_NOCALIB_TO_CAN;
                if( xQueueSend(module->pxQueueToCAN, (void *) &toCANEvent,  1) ){
                    //TODO: failed to notify upper layer
                    // TODO: Lost communication, Switch off??
                }
                break;                
            default:
                toCANEvent.eventType = ERROR_TO_CAN;
                if( xQueueSend(module->pxQueueToCAN, (void *) &toCANEvent,  1) ){
                    //TODO: failed to notify upper layer
                    // TODO: Lost communication, Switch off??
                }
                break; 
                _nop();
                break;
        }
    }


}

/* *****************************************************************************
 End of File
 */
