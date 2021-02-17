/* ************************************************************************** */
/** Laser control public declaration
  @Summary
 Laser control implements the logic of the laser module

 */
/* ************************************************************************** */

#ifndef LASER_CONTROL_H    /* Guard against multiple inclusion */
#define LASER_CONTROL_H

#include "FreeRTOS.h"
#include "queue.h"

// Input queue type for the control task
typedef enum {
    CAN_MSG,    // Message from CAN
    ALARM_EVENT,      // Alarm event from distance measure task
    CALIB_DONE, // Calibration done from distance measure task
    CALIB_ERROR_NO_MEASUREMENT,
    ERROR_COMM, // Placeholder
    ERROR_NO_CALIB,
} EnumEventType_t;

typedef struct {
    EnumEventType_t eventType;
    uint8_t data;   
} LaserCtrlEvent_t;

typedef struct {
    uint8_t ID;
    QueueHandle_t pxQueueToCAN; // Queue to the CAN communication task
    QueueHandle_t pxQueueToComm; // Queue to the Laser COmmunication task
    QueueHandle_t pxQueueToDistance; // Queue to the Distance measure task
    QueueHandle_t pxQueueReceiveLaserCtrl; // Queue to get events from other tasks
} LaserCtrl_t;

void vModuleControlTask(void *pvParameter);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
