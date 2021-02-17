/* ************************************************************************** */
/** laser measured distance handler
  @Summary
    Brief description of the file.

 */
/* ************************************************************************** */
#include "FreeRTOS.h"
#include "queue.h"

/**
 * Handles the distance processing, filtering, calibration and alarm
 * 
 * @param pvParameter LaserDistanceTask_t (2 queues)
 */
void vLaserDistanceMeasureTask(void *pvParameter);

typedef struct {
    QueueHandle_t pxQueueToLaserCtrl; // Queue to the Laser ctrl task
    QueueHandle_t pxQueueReceiveDistance; // Queue to get events from other tasks
} LaserDistanceTask_t;

typedef enum{
    DISTANCE_DATA = 1, //Sending distance data
    DISTANCE_ACTIVATE_ALARM, // Sending control message, activateAlarm
    DISTANCE_DEACTIVATE_ALARM,
    DISTANCE_CALIBRATE, // Sending control message: CALIBRATE
}EnumDistanceEventType_t;

typedef struct {
    EnumDistanceEventType_t eventType;
    uint32_t data;   
} DistanceEventType_t;

#define __CALIBRATE 1 // calibrate command to the Distance task
/* *****************************************************************************
 End of File
 */
