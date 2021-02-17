/**
 * @File canCOMM header
 * 
 * Implements the laserModule object logic and behavior
 * 
 */


#ifndef CANCOMM_H
#define	CANCOMM_H
#include "FreeRTOS.h"
#include "queue.h"

typedef struct {
    QueueHandle_t pxQueueToLaserCtrl[6]; // Queue to the Laser ctrl task
    QueueHandle_t pxQueueReceiveEvent; // Queue to get events from other tasks
} CANTask_t;

/* CAN message command/data tables */
typedef enum {
    ERROR_TO_CAN = 0xF0,    // Message from CAN
    ALARM_TO_CAN = 0x11,    // Alarm event from distance measure task
    DEBUG_MSG_TO_CAN = 0xF1,
    ERROR_NOCALIB_TO_CAN = 0xF2,
    ERROR_CALIBFAILED_TO_CAN = 0xF3,
} EnumToCANEventType_t;

/* Command list come from CAN */
typedef enum {
    CAN_DATA_TURN_ON = 2,    // Message from CAN
    CAN_DATA_TURN_OFF = 3,      // Alarm event from distance measure task
    CAN_DATA_CALIBRATE = 4, // Placeholder
} CanData_t;

typedef struct {
    EnumToCANEventType_t eventType;
    uint8_t id;   
} ToCANEvent_t;

void vCanCommTask(void *pvParameter);

#endif