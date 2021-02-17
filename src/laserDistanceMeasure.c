/* ************************************************************************** */
/** laser distance measurement task

  @Summary
 Laser distance measurement task

 */
/* ************************************************************************** */
#include <stdint.h>
#include <stdbool.h>

#include "laserDistanceMeasure.h"
#include "laserControlSM.h"

#define DISTANCE_MARGIN 10 // Margin for alarm distance to calibrated distance
#define SENSITIVITY 2 //How manytimes must measure distance to alarm
typedef enum {
    INIT,
    WAITFORNEWDATA, //Wait for calibrate or activate
    CALIBRATE,      //measre distance and set as reference for alarm
    ALARM_ACTIVE,//Wait for distance data and alarm if mismatch of calibrated
            
} StateOfDistance_t;

void vLaserDistanceMeasureTask(void *pvParameter){
    
    LaserDistanceTask_t *distanceTask = (LaserDistanceTask_t*) pvParameter;
    QueueHandle_t pxQueueReceiveDistance = distanceTask->pxQueueReceiveDistance;
    
    QueueHandle_t pxQueueToCtrlEvent = distanceTask->pxQueueToLaserCtrl;
    LaserCtrlEvent_t toLaserCtrlEvent;
    
    DistanceEventType_t incomeEvent;
    uint32_t calibDistance = 0;
    uint8_t measureCount;
    
    StateOfDistance_t state = 0;
    while (1) {
        switch(state){
            case INIT: // init 
                measureCount = 0;
                calibDistance = 0;
                state = WAITFORNEWDATA;
                break;
            case WAITFORNEWDATA: //Wait for data infinitely
                xQueueReceive(pxQueueReceiveDistance, (void *)&incomeEvent, portMAX_DELAY);
                if(incomeEvent.eventType == DISTANCE_CALIBRATE){ //calibrate
                    measureCount = 0;
                    calibDistance = 0;
                    state = CALIBRATE;
                }
                if(incomeEvent.eventType == DISTANCE_ACTIVATE_ALARM){
                    if(calibDistance == 0){// in case of no calibration
                        toLaserCtrlEvent.eventType = ERROR_NO_CALIB;
                        if (xQueueSend(pxQueueToCtrlEvent, (void *) &toLaserCtrlEvent, 20) != pdPASS) {
                            //TODO handle Q full 
                        }  
                    }
                    else{
                        state = ALARM_ACTIVE;
                    }                    
                }
                break;
            case CALIBRATE:
                if(xQueueReceive(pxQueueReceiveDistance, (void *)&incomeEvent, pdMS_TO_TICKS(60)) != pdPASS){
                    toLaserCtrlEvent.eventType = CALIB_ERROR_NO_MEASUREMENT;
                    if (xQueueSend(pxQueueToCtrlEvent, (void *) &toLaserCtrlEvent, 20) != pdPASS) {
                        //TODO queue full
                    }                     
                    state = INIT;
                }
                if(incomeEvent.eventType == DISTANCE_DATA){
                    measureCount++;
                    if(measureCount>4){//skip the first 4 measurement
                        calibDistance = calibDistance + incomeEvent.data;

                        if(measureCount>=12){
                            //calc avarage
                            measureCount = 0;
                            calibDistance = calibDistance>>3;//divide by 8

                            toLaserCtrlEvent.eventType = CALIB_DONE;
                            if (xQueueSend(pxQueueToCtrlEvent, (void *) &toLaserCtrlEvent, 20) != pdPASS) {
                                //TODO queue full
                            } 
                            state = WAITFORNEWDATA;
                        }
                    }
                }
                else{ //not expected other data
                    state = INIT;
                }
                
                break;
            case ALARM_ACTIVE:
                xQueueReceive(pxQueueReceiveDistance, (void *)&incomeEvent, portMAX_DELAY);
                if (incomeEvent.eventType == DISTANCE_DATA ){ //incoming distance data
                    if(incomeEvent.data < (calibDistance - DISTANCE_MARGIN)){
                        measureCount ++;
                    }
                    else{ 
                        measureCount = 0;
                    }
                        
                    if(measureCount >= SENSITIVITY){ //Filter glitches! 2 measurement must trigger
                            toLaserCtrlEvent.eventType = ALARM_EVENT;
                        if (xQueueSend(pxQueueToCtrlEvent, (void *) &toLaserCtrlEvent, 20) != pdPASS) {
                                //TODO handle Q full 
                        } 
                    }
                }
                if (incomeEvent.eventType == DISTANCE_DEACTIVATE_ALARM){
                    state = WAITFORNEWDATA;
                }
                if(incomeEvent.eventType == DISTANCE_CALIBRATE){ //calibrate
                    measureCount = 0;
                    calibDistance = 0;
                    state = CALIBRATE;
                }
            break;
            default:
                
                break;
        }
        
        
    }
}

/* *****************************************************************************
 End of File
 */
